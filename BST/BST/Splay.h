#include "BST.h"
template<typename T>
class Splay :publi BST<T> {
protected:
	BinNodePosi(T) splay(BinNodePosi(T) v);//��v��չ������
public:
	BinNodePosi(T) & search(const T&e);
	BinNodePosi(T) & insert(const T&e);
	bool remove(const T& e);
};
template<typename NodePosi>inline//�ڽڵ�*p��*lc֮�佨�����������ӹ�ϵ
//��������������ʱ���ô�ֱ���滻����������ջ����
void attachAsLChild(NodePosi p, NodePosi lc) { p->lChild = lc; if (lc) lc->parent = p; }
template<typename NodePosi>inline
void attachAsRChild(NodePosi p, NodePosi rc) { p->rChild = rc; if (rc) rc->parent = p; }

template<typename T>
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v) {
	if (!v) return NULL;
	BinNodePosi(T) p, BinNodePosi(T) g;//parent and grandparent
	while ((p = v->parent) && (g = p->parent)) {//���¶��Ͻ���˫����չ
		BinNodePosi(T) gg = g->parent;//ÿ��֮��*v����ԭ���游(great-grand parent)Ϊ��
		if (IsLChild(*v)) {
			if (IsLChild(*p)) {//�Լ�����ͼ��֤��
				attachAsLChild(p, v->rChild); attachAsLChild(g, p->rChild);
				attachAsRChild(v, p); attachAsRChild(p, g);
			}
			else {
				attachAsRChild(g, v->lChild); attachAsLChild(p, v->rChild);
				attachAsLChild(v, g); attachAsRChild(v, p);
			}
		}
		else if (IsRChild(*p)) {
			attachAsRChild(g, p->lChild); attachAsRChild(p, v->lChild);
			attachAsLChild(p, g); attachAsLChild(v, p);
		}
		else {
			attachAsRChild(p, v->lChild); attachAsLChild(g, v->rChild);
			attachAsRChild(v, g); attachAsLChild(v, p);
		}
		if (!gg) v->parent = NULL;//��gg�����ڣ���*vΪ����
		else(g == gg->lChild) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);
		updateHeight(g); updateHeight(p); updateHeight(v);
	}//˫����չ������gһ��ΪNULL����p���ܷǿգ���ʱ��Ҫ�ٽ���һ�ε���
	if (p = v->parent) {
		if(IsLChild(*v)){
			attachAsLChild(p, v->rChild); attachAsRChild(v, p);
		}
		else {
			attachAsRChild(p, v->lChild;); attachAsLChild(v, p);
		}
		updateHeight(p); updateHeight(v);
	}
	v->parent = NULL; 
	return v;
}
template<typename T>
BinNodePosi(T) & Splay<T>::search(const T&e) {
	BinNodePosi(T) p = searchIn(_root, e, _hot = NULL);
	_root = splay((p ? p : hot));
	return _root;
}//���۲����Ƿ�ɹ���_root��ָ����󱻷��ʵĽڵ�
template<typename T>
BinNodePosi(T) Splay<T>::insert(const T& e) {
	if (!_root) { _size++; return _root = new BinNode<T>(e); }//ԭ��Ϊ�յ��˻����
	if (e == search(e)->data) return _root;//ȷ��Ŀ��ڵ㲻����
	_size++; BinNodePosi(T) t = _root;
	if (_root->data < e) {
		t->parent = _root = new BinNode<T>(e, NULL, t, t->rChild);
		if (HasRChild(*t)) { t->rChild->parent = _root; t->rChild = NULL; }
	}
	else {
		t->parent = _root = new BinNode<T>(e, NULL, t->lChild, t);
		if (HasLChild(*t)) { t->lChild->parent = _root; t->lChild = NULL; }
	}
	updateHeightAbove(t);//����t�������ȵĸ߶ȣ�ʵ����ֻ��_root��
	return _root;
}


template<typename T>
bool Splay<T>::remove(const T& e) {
	if (!root || (e != search(e) - data))return false;//�˻����
	BinNodePosi(T) w = _root;//search֮���ɾ���ڵ��Ѿ�����������
	if (!HasLChild(*_root)) {//��������
		_root = _root->rChild; if (_root)_root->parent = NULL;
	}
	else if (!HasRChild(*_root)) {
		_root = _root->lChild; if (_root)_root->parent = NULL;
	}
	else {//��������ͬʱ����
		BinNodePosi(T) lTree = _root->lChild;
		lTree->parent = NULL; _root->lChild = NULL;//��ʱ����������
		_root = _root->rChild; _root->parent = NULL;//����������
		search(w->data);//��Ȼʧ�ܵĲ��ң������������е���СԪ�ر�������rootλ�ã�������Ϊ��
		_root->lChild = lTree; lTree->parent = _root;
	}
	release(w->data); release(w); _size--;
	if (_root) updateHeight(_root);//ɨβ����
	return true;
}
