#include "BST.h"
#define Balanced(x) (stature((x).lChild)==stature((x).rChild))//����ƽ������
#define BalFac(x) (stature((x).lChild)-stature((x).rChild))//ƽ������
#define AvlBalanced(x) ((-2<BalFac(x)) && (BalFac(x)<2))
/*
*����x���ߵĺ��ӣ��ȸ����븸��pͬ�������ȣ����ȷ���ع�����
*/
#define tallerChild(x) (\
	stature((x)->lChild) > stature((x)->rChild) ? (x)->lChild : (\
	stature((x)->rChild) > stature((x)->lChild) ? (x)->rChild : (\
	IsLChild(*(x))?(x)->lChild:(x)->rChild\
	)\
	)\
)
template<typename T>
class AVL :public BST<T> {
public:
	BinNodePosi(T) insert(const T& e);
	bool remove(const T& e);
	//�����ӿھ�������
};
template<typename T>
BinNodePosi(T) AVL<T>::insert(const T& e) {
	BinNodePosi(T)& x = search(e);
	if (x)return x;//ȷ��x������
	BinNodePosi(T) xx = new BinNode(e, _hot); _size++;//�˴����ܵ����游���游����ʧ��
	for (BinNodePosi(T) g = _hot; g; g = g->parent) {//��x�ĸ��׿�ʼ������ϼ����������
		if (!AvlBalanced(*g)) {
			FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));//����������ԭ����
			break;//g�ľֲ��������������Ҳ��Ȼ����
		}
		else
			updateHeight(g);//��gδʧ�⣬������߶ȼ���
	}//����һ�ε�����ȫ������ƽ�⣬�߶ȸ�ԭ
	return xx;
}

template<typename T>
bool AVL<T>::remove(const T& e) {
	BinNodePosi(T) & x = search(e);
	if (!x) return false;
	removeAt(x, _hot); _size--;//�Ȱ���BST�Ĺ������ɾ�����˴����ܵ���ʧ��
	for (BinNodePosi(T) g = _hot; g; g = g->parent) {//��_hot�������ϣ�������������g
		if (!AvlBalanced(*g))
			g = FromParentTo(*x) = rotateAt(tallerChild(tallerChild(g)));
		updateHeight(g);
	}//������ҪO(logn)�ε����������Ƿ������ȫ���߶ȶ����ܽ���
	//ɾ���ڵ㵼�µ�ʧ����д����ԣ��ʲ���һ�ε�����ֱ��break
	return true;
}


