#include "PQ.h"
#include "BinTree.h"

template<typename T>
class PQ_LeftHeap :public PQ<T>, public BinTree<T> {
public:
	PQ_LeftHeap() {}
	PQ_LeftHeap(T* E, int n) {//�ɸĽ�ΪFloyd�����㷨,O(n)
		for (int i = 0; i < n; i++) insert(E[i]);//O(nlogn)
	}
	void insert(T);
	T getMax();
	T delMax();
};

template<typename T>
static void exchangeChildren(BinNodePosi(T) v) {//exchange v's left and right child
	BinNodePosi(T) t = v->lChild;
	v->lChild = v->rChild;
	v->rChild = t;
}

template<typename T>
static BinNodePosi(T) merge1(BinNodePosi(T) a, BinNodePosi(T) b) {//�ϲ���a��bΪ���ڵ��������ʽ�ѣ�����a�����ȼ�����
	if (!HasLChild(*a)) {//��ʽ��û�����ӣ���ô���ǹ��
		a->lChild = b; b->parent = a;
	}
	else {
		a->rChild = merge(a->rChild, b);
		a->rChild->parent = a;
		if (a->lChild->npl < a->rChild->npl) {
			exchangeChildren(a);
		}
		a->npl = a->rChild->npl + 1;
	}
	return a;//���غϲ���ĶѶ�
}

template<typename T>
static BinNodePosi(T) merge(BinNodePosi(T) a, BinNodePosi(T) b) {//����������ȼ�ȷ�����˵ķ�ʽ���ϲ���a��bΪ���ڵ��������ʽ��
	if (!a) return b;
	if (!b) return a;//�ݹ��
	return lt(a->data, b->data) ? merge1(b, a) : merge1(a, b);
}//�ѵĹ�ģ���ɵ����߸������

template<typename T>
T PQ_LeftHeap<T>::delMax() {
	BinNodePosi(T) lHeap = _root->lChild;
	BinNodePosi(T) rHeap = _root->rChild;
	T e = _root->data; delete _root; _size--;
	_root = merge(lHeap, rHeap);
	if (_root) _root->parent = NULL;
	return e;
}

template<typename T>
void PQ_LeftHeap<T>::insert(T e) {
	BinNodePosi(T) v = new BinNode<T>(e);
	_root = merge(_root, e);
	_root->parent = NULL;
	_size++;
}