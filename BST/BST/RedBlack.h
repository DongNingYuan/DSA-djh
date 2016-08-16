#include "BST.h"
#define IsBlack(p) (!(p)||(RB_BLACK == (p)->color)) //�ⲿ�ڵ�Ҳ�����ڽڵ�
#define IsRed(p) (!IsBlack(p))
#define BlackHeightUpdated(x)(\
	(stature((x).lChild)==stature((x).rChild)) && \
	((x).height == (IsRed(&x)?stature((x).lChild):stature((x).lChild)+1))\
)//x�ĺڸ߶��Ƿ��Ѹ���
template<typename T>
class RedBlack :public BST<T> {
protected:
	void solveDoubleRed(BinNodePosi(T) x);//˫������
	void solveDoubleBlack(BinNodePosi(T) x);//˫������
	int updateHeightAbove(BinNodePosi(T) x);
public:
	BinNodePosi(T) insert(const T& e);
	bool remove(const T&e);
	//BST:search�Ƚӿڿ�ֱ������
};
template<typename T>
int RedBlack<T>::updateHeightAbove(BinNodePosi(T) x) {
	x->height = max(stature(x->lChild), stature(x->rChild));//���ӵĺڸ߶�һ����ȣ����ǳ���˫��
	return IsBlack(x) ? x->height++ : x->height;
}//ͳһ����stature(NULL)=-1

template<typename T>
BinNodePosi(T) RedBlack<T>::insert(const T& e) {
	BinNodePosi(T) & x = search(e); if (x) return x;
	x = new BinNode<T>(e, _hot, NULL, NULL, -1); _size++;
	//BlackHeight=-1 color=RB_RED
	solveDoubleRed(x);
	return x ? x : _hot->parent;

}
/*
* RedBlack˫������㷨������ڵ�x�͸��׾�Ϊ��ɫ�����⣬��Ϊ�������
* RR-1: uΪ��ɫ����Ҫ2����ɫ��ת��2�κڸ߶ȸ��£�1~2����ת�����ٵݹ�
* RR-2: uΪ��ɫ����Ҫ3����ɫ��ת��3�κڸ߶ȸ��£�0����ת����Ҫ�ݹ�
* ����Լ��: xΪ����Ľڵ㣬pΪ	�丸�ף�gΪ�游��uΪg����һ�����ӣ���p���ֵܣ�x�����塣
* u����Ϊ�ⲿ�ڵ㣬���ش��ڡ���Ϊ��˫��ȱ�ݣ���x��p��Ϊ��ɫ�����g�����ұ�Ϊ��ɫ
*/
template<typename T>
void RedBlack<T>::solveDoubleRed(BinNodePosi(T) x) {
	if (IsRoot(*x)) {//�ݹ�����ѵ����ڵ㣬����ת�ڣ��߶�����
		_root->color = RB_BLACK; _root->height++; return;
	}
	BinNodePosi(T) p = x->parent; if (IsBlack(p))return;
	BinNodePosi(T) g = p->parent;
	BinNodePosi(T) u = uncle(x);//BinNode<T>���Ѿ�ʵ��
	if (IsBlack(u)) {//uΪ��ɫ������NULL��
		BinNodePosi(T) gg = g->parent;//great-grand parent
		BinNodePosi(T) r = FromParentTo(*g) = rotateAt(x);//rΪ�����������ĸ��ڵ�
		r->color = RB_BLACK; r->lChild->color = RB_RED; r->rChild->color = RB_RED;
		//���������ж�ֻ������Ⱦɫ�������ƺ��ò���ʧ
		r->parent = gg;
	}
	else {//uΪ��ɫ
		p->color = RB_BLACK; p->height++;
		u->color = RB_BLACK; u->height++;
		if (!IsRoot(*g)) g->color = RB_RED;
		solveDoubleRedI(g);//�������㣬�ݹ������ɲο�β�ݹ��Ż��ɵ�����ʽ��
	}
}

template<typename T>
bool RedBlack<T>::remove(const T& e) {
	BinNodePosi(T) x = search(e); if (!e) retuen false;
	BinNodePosi(T) r = removeAt(x, _hot); if (0 >= --_size)return true;
	//_hot��ĳһ���Ӹձ�ɾ�����ұ�r��ָ��Ľڵ㣨������NULL�����档���¼���Ƿ�ʧ�Ⲣ����Ҫ����
	if (!_hot) {//�ոձ�ɾ�����Ǹ��ڵ�
		_root->color = RB_BLACK; updateHeight(_root); return true;
	}
	//���ˣ���ԭx����r���طǸ�����_hot�ǿ�
	if (BlackHeightUpdated(*(_hot))) {//�������ȵĺ��������ƽ�⣬���������
		return true;
	}
	if (IsRed(r)) {//������rΪ��ɫ����ôֻ�轫rת��
		r->color = RB_BLACK; r->height++; return true;
	}
	//���ˣ�ԭx����r����Ϊ��ɫ
	solveDoubleBlack(r); return true;
}

/*
* RedBlack��˫��ȱ�ݵ����㷨������ڵ�x�뱻������Ľڵ�r��Ϊ��ɫ������
* ��ΪBB-1 BB-2R BB-2B BB-3�������
*/
template <typename T> void RedBlack<T>::solveDoubleBlack(BinNodePosi(T) r) {
	BinNodePosi(T) p = r ? r->parent : _hot; if (!p) return; //r�ĸ���
	BinNodePosi(T) s = (r == p->lc) ? p->rc : p->lc; //r���ֵ�
	if (IsBlack(s)) { //�ֵ�sΪ��
		BinNodePosi(T) t = NULL; //s�ĺ캢�ӣ������Һ��ӽԺ죬�������ȣ��Ժ�ʱΪNULL��
		if (IsRed(s->rc)) t = s->rc; //����
		if (IsRed(s->lc)) t = s->lc; //����
		if (t) { //��s�к캢�ӣ�BB-1
				 //*DSA*/printf("  case BB-1: Child ("); print(s->lc); printf(") of BLACK sibling ("); print(s); printf(") is RED\n");
			RBColor oldColor = p->color; //����ԭ�������ڵ�p��ɫ������t���丸�ס��游
										 // ���£�ͨ����ת��ƽ�⣬���������������Һ���Ⱦ��
			BinNodePosi(T) b = FromParentTo(*p) = rotateAt(t); //��ת
			if (HasLChild(*b)) { b->lc->color = RB_BLACK; updateHeight(b->lc); } //����
			if (HasRChild(*b)) { b->rc->color = RB_BLACK; updateHeight(b->rc); } //����
			b->color = oldColor; updateHeight(b); //���������ڵ�̳�ԭ���ڵ����ɫ
												  //*DSA*/printBinTree(b, 0, 0);
		}
		else { //��s�޺캢��
			s->color = RB_RED; s->height--; //sת��
			if (IsRed(p)) { //BB-2R
							//*DSA*/printf("  case BB-2R: Both children ("); print(s->lc); printf(") and ("); print(s->rc); printf(") of BLACK sibling ("); print(s); printf(") are BLACK, and parent ("); print(p); printf(") is RED\n"); //s���Ӿ��ڣ�p��
				p->color = RB_BLACK; //pת�ڣ����ڸ߶Ȳ���
									 //*DSA*/printBinTree(p, 0, 0);
			}
			else { //BB-2B
				   //*DSA*/printf("  case BB-2R: Both children ("); print(s->lc); printf(") and ("); print(s->rc); printf(") of BLACK sibling ("); print(s); printf(") are BLACK, and parent ("); print(p); printf(") is BLACK\n"); //s���Ӿ��ڣ�p��
				p->height--; //p���ֺڣ����ڸ߶��½�
							 //*DSA*/printBinTree(p, 0, 0);
				solveDoubleBlack(p); //�ݹ�����
			}
		}
	}
	else { //�ֵ�sΪ�죺BB-3
		   //*DSA*/printf("  case BB-3: sibling ("); print(s); printf(" is RED\n"); //s�죨˫�Ӿ�ڣ�
		s->color = RB_BLACK; p->color = RB_RED; //sת�ڣ�pת��
		BinNodePosi(T) t = IsLChild(*s) ? s->lc : s->rc; //ȡt���丸sͬ��
		_hot = p; FromParentTo(*p) = rotateAt(t); //��t���丸�ס��游��ƽ�����
												  //*DSA*/printBinTree<T>(s, 0, 0);
		solveDoubleBlack(r); //��������r��˫�ڡ�����ʱ��p��ת�죬�ʺ���ֻ����BB-1��BB-2R
	}
}