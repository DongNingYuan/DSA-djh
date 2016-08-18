#include "PQ.h"
#include "vector.h"

#define InHeap(n,i) (((-1)<(i))&&((i)<(n)))//PQ[i]�Ƿ�Ϸ�
#define Parent(i) ((i-1)>>1)//����
#define LastInternal(n) Parent(n-1)//���һ���ڲ��ڵ㣬��ĩ�ڵ�ĸ���
#define LChild(i) (1+((i)<<1)) //PQ[i]������
#define RChild(i) ((1+(i))<<1)
#define ParentValid(i) (0<i) //�ж�PQ[i]�Ƿ��и���
#define LChildValid(i) InHeap(n,LChild(i))
#define RChildValid(i) InHeap(n,RChild(i)) //��ȫ�������Ľṹ �����ȼ����ж��Ƿ�����������
#define Bigger(PQ,i,j) (lt(PQ[i],PQ[j])?j:i) //ȡ���ߣ����ǰ������
#define ProperParent(PQ,n,i)\
			(RChildValid(n,i)?Bigger(PQ,Bigger(PQ,i,LChild(i)),RChild(i)):\
			(LChildValid(n,i)?Bigger(PQ,i,LChild(i)):i\
			)\
			)//���������е�����ߣ����ʱ���ڵ����ȣ��ɱ��ⲻ��Ҫ�Ľ�����

template<typename T>
class PQ_ComplHeap :public PQ<T>, public Vector<T> {//��������������ȫ�������ʽʵ�ֵ����ȼ�����
protected:
	Rank percolateDown(Rank n, Rank i);//����
	Rank percolateUp(Rank i);//����
	void heapify(Rank n);//Floyd�����㷨
public:
	PQ_ComplHeap() {}
	PQ_ComplHeap(T* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); heapify(hi - lo); }
	PQ_ComplHeap(T* A, Rank n) { copyFrom(A, 0, n); heapify(n); }
	void insert(T);//�������ȼ��������
	T getMax() { return _elem[0]; }
	T delMax();
};

template<typename T>
void PQ_ComplHeap<T>::insert(T e) {
	Vector<T>::insert(e);//����ĩβ�����������������뿼��������ݵ��������
	percolateUp(_size - 1);
}

template<typename T>
Rank PQ_ComplHeap<T>::percolateUp(Rank i) {//�������еĵ�i������ʵʩ���˲�����i<_size
	while (ParentValid(i)) {//��δ�ִ�Ѷ�
		Rank j = Parent(i);
		if (lt(_elem[i], _elem[j])) break;
		swap(_elem[i], _elem[j]); i = j;
		//�ڳ�ϵ�������¿��ԸĽ����μ�ϰ�� 10-3 10-4
	}
	return i;
}

template<typename T>
T PQ_ComplHeap<T>::delMax() {
	T maxElem = _elem[0]; 
	_elem[0] = remove(_size - 1);//��ĩ��������Ѷ�
	percolateDown(_size, 0);
	return maxElem;
}

template<typename T>//������ǰn�������еĵ�i��ʵʩ���˲�����i<n
Rank PQ_ComplHeap<T>::percolateDown(Rank n, Rank i) {
	Rank j;//jΪi�����������������У�����������
	while (i != (j = ProperParent(_elem, n, j))) {
		swap(_elem[i], _elem[j]);i=j
	}
	return i;//�������˵ִ��λ�ã�i=j
}

template<typename T>
void PQ_ComplHeap<T>::heapify(Rank n) {//Floyd�����㷨��O(n)ʱ��
	for (int i = LastInternal(n); InHeap(n, i); i--) {//�Ե׶���
		percolateDown(n, i);
	}
	//ʱ�临�Ӷ����൱�ڸ��ڵ�߶ȵ��ܺͣ������϶��½����������൱�ڸ��ڵ���ȵ��ܺͣ�ΪO(nlogn)
}