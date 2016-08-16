#include "BinTree.h"
template<typename T>
class BST :public BinTree<T> {
protected:
	BinNodePosi(T) _hot;//BST::search()�����ʵķǿսڵ�λ��
	BinNodePosi(T) connect34(BinNodePosi(T), BinNodePosi(T), //����3+4�ṹ�����������ڵ㼰������
		BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T))��
		BinNodePosi(T) rotateAt(BinNodePosi(T) x);//��x���丸�ס��游��ͳһ��ת����
public:
	virtual BinNodePosi(T) & search(const T& e);
	virtual BinNodePosi(T) insert(const T& e);
	virtual bool remove(const T& e);
};

template<typename T>
static BinNodePosi(T) & searchIn(BinNodePosi(T)& v, const T& e, BinNodePosi(T)& hot) {
	if (!v || (e == v->data)) return v;
	hot = v;//���µ�ǰ�ڵ��ټ���
	return searchIn(((e < v->data) ? v->lChild : v->rChild), e, hot);
}//����ֵָ�����нڵ���������нڵ㣬hotָ���丸��
template<typename T>
BinNodePosi(T) & BST<T>::search(const T& e) {//������BST�в���
	return searchIn(_root, e, _hot = NULL);//����Ŀ��ڵ�λ�õ�����
}
template<typename T>
BinNodePosi(T) BST<T>::insert(const T& e) {
	BinNodePosi(T) & x = search(e);
	if (x) return x;//ȷ��Ŀ��ڵ㲻����
	x = new BinNode<T>(e, _hot);
	_size++;
	updateHeightAbove(x);
	return x;
}
template<typename T>
bool BST<T>::remove(const T&e) {
	BinNodePosi(T) & x = search(e);
	if (!x)return false;
	removeAt(x, _hot); _size--;
	updateHeightAbove(_hot);
	return true;
}//�����Ƿ�ɹ�ɾ��
template<typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T)& x, BinNodePosi(T)& hot) {
	/* BST�ڵ��ɾ���㷨��ɾ��λ��x��ָ�Ľڵ㣬������AVL��Splay��RedBlack�ȸ���BST
	*  ����ʱ��֤x��NULL
	*  ����ֵָ��ʵ�ʱ�ɾ���ڵ�Ľ����ߣ�hotָ��ɾ���ڵ�ĸ��� ������ΪNULL
	*/
	BinNodePosi(T) w = x;//ʵ�ʱ�ժ���Ľڵ㣬��ֵΪx
	BinNodePosi(T) succ = NULL;
	if (!HasLChild(*x))//������Ϊ��
		succ = x = x->rChild;
	else if (!HasRChild(*x))//������Ϊ��
		succ = x = x->lChild;
	else {//�������������ڣ���ѡ��x��ֱ�Ӻ����Ϊʵ�ʱ�ժ���Ľڵ�
		w = w->succ();
		swap(x->data, w->data);
		BinNodePosi(T) u = w->parent;
		((u == x) ? u->rChild : u->lChild) = succ = w->rChild;//����ڵ�w
	}
	hot = w->parent;
	if (succ) succ->parent = hot;
	release(w->data); release(w); return succ;

}
/*
* ����3+4�ṹ�����������Ŀ������������飬���������ֲ��������ڵ��λ��b
* �������ڵ����ϲ�ڵ�֮���˫�����ӣ������ϲ���������
* ����AVL��RedBlack�ľֲ�ƽ�����
* �����������Ϊ{T0,a,T1,b,T2,c,T3}ת��Ϊ
*                  b
*             a         c
*           T0  T1    T2   T3
*/
template<typename T>
BinNodePosi(T) BST<T>::connect34(BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,
	BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3) {
	a->lChild = T0; if (T0)T0->parent = a;
	a->rChild = T1; if (T1)T1->parent = a;
	c->lChild = T2; if (T2)T2->parent = c;
	c->rChild = T3; if (T3)T3->parent = c;
	b->lChild = a; a->parent = b;
	b->rChild = c; c->parent = b;
	updateHeight(b);
	return b;
}

/*
* BST�ڵ���תͳһ�㷨�����ص���֮��ֲ��������ڵ�λ��
* �����ĸ�ָ���ϲ�ڵ㣬�����ϲ�ڵ㵽����������Ӧ�ɵ��������
*/
template<typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v) {//vΪ�ǿյ��ﱲ�ڵ� g-p-v����
	BinNodePosi(T) p = v->parent; BinNodePosi(T) g = p->parent;
	//��g-p-v�����λ�÷��������
	if (IsLChild(*p)) {/* zig */
		if (IsLChild(*v)) {/* zig-zig */
			p->parent = g->parent;
			return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
		}
		else {/* zig-zag */
			v->parent = g->parent;
			return connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
		}
	}
	else {/* zag */
		if (IsLChild(*v)) {/* zag-zig */
			v->parent = g->parent;
			return connect34(g, v, p, g->lChild, v->lChild, v->rChild, p->rChild);
		}
		else {/* zig-zig */
			p->parent = g->parent;
			return connect34(g, p, c, g->lChild, p->lChild, v->lChild, v->rChild);
		}
	}

}

