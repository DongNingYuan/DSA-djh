#include "BTNode.h"
template<typename T>
class BTree {
protected:
	int _size;//��ŵĹؼ�������
	int _order;//BTree�Ľ״Σ����ڵ���3��������һ�㲻���޸�
	BTNodePosi(T) _root;
	BTNodePosi(T) _hot;//BTree:search()�����ʵķǿսڵ�λ��
	void solveOverflow(BTNodePosi(T));
	void solveUnderflow(BTNodePosi(T));
public:
	BTree(int order = 3) :_order(order), _size(0) { _root = new BTNode<T>(); }
	~BTree() { if (_root)release(_root); }
	int const order() { return _order; }
	int const size() { return _size; }
	BTNodePosi(T) & root() { return _root; }
	bool empty()const { return !_root; }
	BTNodePosi(T) search(const T& e);
	bool insert(const T& e);
	bool remove(const T& e);
};

template<typename T>
BTNodePosi(T) BTree<T>::search(const T& e) {
	BTNodePosi(T) v = _root; _hot = NULL;
	while (v) {//�������
		Rank r = v->key.search(e);//���ز�����e�����ؼ���
		if ((0 <= r) && (e == v->key[r]))return v;
		_hot = v; v = v->child[r + 1];
	}
	return NULL;//fail
}
template<typename T>
bool BTree<T>::insert(const T&e) {
	BTNodePosi(T) v = search(e);
	if (v)return false;
	Rank r = _hot->key.search(e);//���Һ��ʵĲ���λ��
	_hot->child.insert(r + 1, e);
	_hot->child.insert(r + 2, NULL);
	_size++;
	solveOverflow(_hot);
	return true;
}
template<typename T>
void BTree<T>::solveOverflow(BTNodePosi(T) v) {
	if (_order >= v->child.size())return;//�ݹ�� ��ǰ�ڵ�δ��������
	Rank s = _order / 2;//��� ��ʱ����_order=key.size()=child.size()-1
	BTNodePosi(T) u = new BTNode<T>();//�½ڵ�����һ���պ���
	for (Rank j = 0; j < _order - s - 1; j++) {
		u->child.insert(j, v->child.remove(s + 1));
		u->key.insert(j, v->key.remove(s + 1));
	}//�ɸĽ�
	u->child[_order - s - 1] = v->child.remove(s - 1);//�ƶ�v��ҵĺ���
	if (u->child[0]) {//u�ĺ����Ƿǿ�
		for (Rank j = 0; j < _order - s; j++) {
			u->child[j]->parent = u;//���ø��׽ڵ�
		}
	}
	BTNodePosi(T) p = v->parent;
	if (!p) { _root = p = new BinNode<T>(); p->child[0] = v; v->parent = p; }//��pδ���򴴽�֮
	Rank r = 1 + p->key.search(v->key[0]);
	p->key.insert(r, v->key.remove(s));//�������
	p->child.insert(r + 1, u); u->parent = p;
	solveOverflow(p);
}
template<typename T>
bool BTree<T>::remove(const T& e) {
	BTNodePosi(T) v = search(e); 
	if (!v) return false;
	Rank r = v->key.search(e);//ȷ����
	if (v->child[0]) {//v����Ҷ��
		BTNodePosi(T) u = v->child[r + 1];
		while (u->child[0]) u = u->child[0];//����������һ·�����ҳ����
		v->key[r] = u->key[0]; v = u; r = 0;//����
	}
	//��ʱvλ����ײ㣬�ҵ�r���ؼ�����Ǵ�ɾ����
	v->key.remove(r); v->child.remove(r + 1); _size - ;
	solveUnderflow(v);
	return true;
}

template<typename T>
void BTree<T>::solveUnderflow(BTNodePosi(T) v) {
	if ((_order + 1) / 2 <= v->child.size())return;//�ݹ��:��ǰ�ڵ�δ����
	BTNodePosi(T) p = v->parent;
	if (!p) {//�ݹ����������ڵ�
		if (!v->key.size() && v->child[0]) {
			//��Ϊ������v�����ؼ��룬���У�Ψһ�ģ��ǿպ���
			_root = v->child[0]; _root->parent = NULL;
			v->child[0] = NULL; release(v);
		}//���ĸ߶�-1
		return;
	}
	Rank r = 0; while (p->child[r] != v)r++;
	//ȷ��r ��ʱv���ܲ����ؼ��룬�ʲ��ܲ���search
	//���ʵ�����е����������ͨ��vector::find()
	//���1�������ֵܽ�ؼ���
	if (0 < r) {//��v���Ǹ���p�ĵ�һ������
		BTNodePosi(T) ls = p->child[r - 1];//���ֵܱش���
		if ((_order + 1) / 2 > ls->child.size()) {
			//���ֵ��㹻�ֿ��ԽӼ�����
			v->key.insert(0, p->key[r - 1]);
			p->key[r - 1] = ls->key.remove(ls->key.size() - 1);
			v->child.insert(0��ls->child.remove(ls->child.size() - 1);//��ls�����Ҳຢ�ӹ��̸�v
			if (v->child[0]) v->child[0]->parent = v;
			return;//�������(��ǰ��μ����ϲ�ξ������)
		}
	}
	//�����ˣ������ֵ�ҪôΪ�գ�Ҫô̫��
	//���2�������ֵܽ�ؼ���
	if (r < p->child.size() - 1 ) {
		BTNodePosi(T) rs = p->child[r + 1];
		if ((_order + 1) / 2 > rs->child.size()) {
			v->key.insert(v->key.size(), p->key[r]);
			p->key[r] = rs->key.remove(0);
			v->child.insert(v->child.size(), rs->child.remove(0));
			if (v->child[v->child.size() - 1]) v->child[v->child.size() - 1]->parent = v;
			return;//�������(��ǰ��μ����ϲ�ξ������)
		}
	}
	//�����ˣ��������ֵ�ҪôΪ�գ�Ҫô̫��
	//���3����̫�ݻ�Ϊ�գ���ϲ�
	if (0 < r) {//v�����ֵܺϲ�
		BTNodePosi(T) ls = p->child[r - 1];
		ls->key.insert(ls->key.size(), p->key.remove(r - 1));
		p->child.remove(r);
		//p�ĵ�r-1���ؼ���ת��ls��v������p�ĵ�r������
		ls->child.insert(ls->child.size(), v->child.remove(0));
		if (ls->child[ls->child.size() - 1]) {//v������ຢ�ӹ��̸�ls�����Ҳຢ��
			ls->child[ls->child.size() - 1]->parent = ls;
		}
		while (!v->key.empty()) {//vʣ��Ĺؼ���ͺ���ͳͳ���̸�ls(�����ӻ�Ҫ��Ҫ����)
			ls->key.insert(ls->key.size(), v->key.remove(0));
			ls->child.insert(ls->child.size(), v->child.remove(0));
			if (ls->child[ls->child.size() - 1]) { 
				ls->child[ls->child.size() - 1]->parent = ls; 
			}
		}
		release(v);//������û�����ˣ�ɢ��ּҲ��ɣ�
	}
	else {//v�����ֵܺϲ�
		BTNodePosi(T) rs = p->child[r + 1];
		rs->key.insert(0, p->key.remove(r));
		p->child.remove(r);
		rs->child.insert(0, v->child.remove(v->child.size() - 1));
		if (rs->child[0])rs->child[0]->parent = rs;
		while (!v->key.empty()) {
			rs->key.insert(0, v->key.remove(v->key.size() - 1));
			rs->child.insert(0, v->child.remove(v->child.size() - 1));
			if (rs->child[0])rs->child[0]->parent = rs;
		}
		release(v);
	}
	//��ʱ��һ�����һ���ؼ��룬���б�Ҫ��������
	solveUnderflow(p);
	return;
	
}