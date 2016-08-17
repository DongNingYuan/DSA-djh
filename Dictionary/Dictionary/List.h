#include "ListNode.h"
using namespace std;
template<typename T> class List {
private:
	int _size; ListNodePosi(T) header; ListNodePosi(T) tailor;
protected:
	void init();//�б���ʱ��ʼ��
	int clear();
	void copyNodes(ListNodePosi(T), int);//������λ��p���n��
	void merge(ListNodePosi(T), int, List<T>&, ListNodePosi(T), int);//�����б�����ϲ�
	void mergeSort(ListNodePosi(T)&, int);//��p��ʼ������n���ڵ�鲢����
	void selectionSort(ListNodePosi(T), int);//ѡ������
	void insertionSort(ListNodePosi(T), int);//��������
public:
	//constructor
	List() { init(); }
	List(List<T> const& L);//���帴���б�L
	List(List<T> const& L, Rank r, int n);//�����Ե�r�����n��
	List(ListNodePosi(T) p, int n);//�����б�����λ��p���n��
								   //destructor
	~List() { clear(); delete header; delete tailor; }
	//read-only interface
	Rank size() const { return _size; }
	bool empty() const { return _size <= 0; }
	T& operator[](int r) const;//ѭ�ȷ��ʣ�Ч�ʵ���
	ListNodePosi(T) first() const { return header->succ; }
	ListNodePosi(T) last() const { return tailor->pred; }
	bool valid(ListNodePosi(T) p) { return p && (p != tailor)&(p != header); }//�ж�p�Ƿ�Ϸ�
	int disordered() const;
	ListNodePosi(T) find(T const& e)const { return find(e, _size, tailor); }
	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const;//�����б��������(p�Լ�ǰn-1��ǰ��)
	ListNodePosi(T) search(T const& e)const { return find(e, _size, tailor); }
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const;//�����б��������(p�Լ�ǰn-1��ǰ��)
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n) const;//��p�Լ�n-1�������ѡ�������
	ListNodePosi(T) selectMax() const { return selectMax(header->succ, _size); }

	//read-write interface
	ListNodePosi(T) insertAsFirst(T const& e);
	ListNodePosi(T) insertAsLast(T const& e);
	ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const& e);
	ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const& e);
	T remove(ListNodePosi(T) p);//ɾ��p���Ľڵ㲢���ر�ɾ���Ľڵ�����
	void merge(List<T>& L) { merge(first(), size(), L, L.first(), L.size()); }//ȫ�б�鲢
	void sort(ListNodePosi(T) p, int n);//�б���������
	void sort() { sort(first(), _size); }
	int deduplicate();//����ȥ��
	int uniqualify();//����ȥ��
	void reverse();//ǰ����

				   //traverse
	void traverse(void(*visit)(T&)) {//����ָ��
		for (ListNodePosi(T) p = header->succ; p != tailor; p = p->succ)visit(p->data);
	}
	template<typename VST>
	void traverse(VST& visit) {//��������
		for (ListNodePosi(T) p = header->succ; p != tailor; p = p->succ) visit(p->data);
	}
	template <typename T> //�����б�Ĺ鲢����ǰ�б�����p���n��Ԫ�أ����б�L����q���m��Ԫ�ع鲢
	void List<T>::merge(ListNodePosi(T) & p, int n, List<T>& L, ListNodePosi(T) q, int m) {
		// assert:  this.valid(p) && rank(p) + n <= size && this.sorted(p, n)
		//          L.valid(q) && rank(q) + m <= L._size && L.sorted(q, m)
		// ע�⣺�ڹ鲢����֮��ĳ��ϣ��п��� this == L && rank(p) + n = rank(q)
		ListNodePosi(T) pp = p->pred; //����ǰ����������header�����Ա㷵��ǰ ...
		while (0 < m) //��q��δ�Ƴ�����֮ǰ
			if ((0 < n) && (p->data <= q->data)) //��p������������v(p) <= v(q)����
			{
				if (q == (p = p->succ)) break; n--;
			} //p����ϲ����б����滻Ϊ��ֱ�Ӻ��
			else //��p�ѳ����ҽ��v(q) < v(p)����
			{
				insertB(p, L.remove((q = q->succ)->pred)); m--;
			} //��qת����p֮ǰ
		p = pp->succ; //ȷ���鲢������ģ��£����
	}
};

template<typename T>
void List<T>::init() {//��ʼ�����ڴ����б����ʱͳһ����
	header = new ListNode<T>;
	tailor = new ListNode<T>;
	header->succ = tailor; header->pred = nullptr;
	tailor->pred = header; tailor->succ = nullptr;
	_size = 0;
}
template<typename T>
T& List<T>::operator[](int r) const {
	ListNodePosi(T) p = first();
	while (0 < r--)p = p->succ;
	return p->data;
}
template<typename T>
ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) const {
	while (0 < n--) {
		if (e == (p = p - < pred)->data)return p;//����ȶ�
	}
	return NULL;
}
template<typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const& e) {
	_size++;
	return header->insertAsSucc(e);
}
template<typename T>
ListNodePosi(T) List<T>::insertAsLast(T const & e)
{
	_size++;
	return tailor->insertAsPred(e);
}
template<typename T>
ListNodePosi(T) List<T>::insertBefore(ListNodePosi(T) p, T const & e)
{
	_size++;
	return p->insertAsPred(e);
}
template<typename T>
ListNodePosi(T) List<T>::insertAfter(ListNodePosi(T) p, T const & e)
{
	_size++;
	return p->insertAsSucc(e);
}
template<typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n) {//�����б�����p���n�Ҫ��p�Ϸ�����������n-1�����
	init();
	while (n--) {
		insertAsLast(p->data);
		p = p->succ;
	}
}
template<typename T>
List<T>::List(ListNodePosi(T) p, int n) { copyNodes(p, n); }
template<typename T>
List<T>::List(List<T> const& L) { copyNodes(L.first(), L._size); }//���帴���б�L
template<typename T>
List<T>::List(List<T> const& L, int r, int n) { copyNodes(L[r], n); }//�����б�L�Ե�r�����n��

template<typename T>
T List<T>::remove(ListNodePosi(T) p) {
	T e = p->data;
	p->pred->succ = p->succ;
	p->succ->pred = p->pred;
	delete p; _size--;
	return e;
}
template<typename T>
int List<T>::clear() {
	int oldSize = _size;
	while (0 < _size)remove(header->succ);
	return oldSize;
}
template<typename T>
int List<T>::deduplicate() {
	if (_size < 2)return 0;
	int oldSize = _size;
	ListNodePosi(T) p = header; Rank r = 0;
	while (tailor != (p = p->succ)) {
		ListNodePosi(T) q = find(p->data, r, p);//��p��r��ǰ����Ѱ����ͬԪ��
		q ? remove(q) : r++;
	}
	return _size - oldSize;
}
template<typename T>
int List<T>::uniqualify() {//�����б��Ψһ��
	if (_size < 2)return 0;
	int oldSize = _size;
	ListNodePosi(T) p; ListNodePosi(T) q;
	for (p = header, q = p->succ; tailor != q; p = q, q = q->succ) {
		if (p->data == q->data) { remove(q); q = p; }
	}
	return oldSize - _size;
}
template<typename T>
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const {//��p��n����ǰ���з��ز�����e�������
	while (0 <= n--) {//����Ƚ�
		if (((p = p->pred)->data) <= e)break;
	}
	return p;
}//ʧ��ʱ���ܷ���header��Ӧ����valid()����ȷ����Ч��
template<typename T>
void List<T>::sort(ListNodePosi(T) p, int n) {
	//insertionSort(p, n);
	//selectionSort(p, n);
	mergeSort(p, n);

	//rand() may cause compile error in gcc and g++
}
template<typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n) {
	for (int r = 0; r < n; r++) {
		insertAfter(search(p->data, r, p), p->data);
		p = p->succ; remove(p->pred);
	}
}
template<typename T>
void List<T>::selectionSort(ListNodePosi(T) p, int n) {
	ListNodePosi(T) head = p->pred; ListNodePosi(T) tail = p;
	for (int i = 0; i < n; i++)tail = tail->succ;//����������Ϊ(head,tail)
	while (1 < n) {
		ListNodePosi(T) max = selectMax(head->succ, n);
		insertBefore(tail, remove(max));
		tail = tail->pred; n--;
	}
}
template<typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n) const {
	ListNodePosi(T) max = p;
	for (ListNodePosi(T) cur = p; i < n; n--) {
		if (!((cur = cur->succ)->data < max->data)) {//������Ҫ����<
			max = cur;
		}
	}
	return max;
}/*
 template<typename T>
 void List<T>::merge(ListNodePosi(T) p, int n, List<T>& L, ListNodePosi(T) q, int m) {
 //�����б�Ĺ鲢����ǰ�б�����p���n��Ԫ�����б�L����q���m��Ԫ�ع鲢
 //assert: this.valid(p) && rank(p) + n < size && this.sorted(p,n)
 //        L.valid(p) && rank(q) + m <= L._size && L.sorted(q,m)
 ListNodePosi(T) pp = p->pred;
 while (0 < m) {
 if ((0 < n) && (p->data <= q->data)) {
 if (q == (p = p->succ))break;
 n--;
 }
 else {
 insertBefore(p, L.remove((q = q->succ)->pred));
 m--;
 }
 }
 p = pp->succ;
 }*/
template <typename T> //�����б�Ĺ鲢����ǰ�б�����p���n��Ԫ�أ����б�L����q���m��Ԫ�ع鲢
void List<T>::merge(ListNodePosi(T)  p, int n, List<T>& L, ListNodePosi(T) q, int m) {
	// assert:  this.valid(p) && rank(p) + n <= size && this.sorted(p, n)
	//          L.valid(q) && rank(q) + m <= L._size && L.sorted(q, m)
	// ע�⣺�ڹ鲢����֮��ĳ��ϣ��п��� this == L && rank(p) + n = rank(q)
	ListNodePosi(T) pp = p->pred; //����ǰ����������header�����Ա㷵��ǰ ...
	while (0 < m) //��q��δ�Ƴ�����֮ǰ
		if ((0 < n) && (p->data <= q->data)) //��p������������v(p) <= v(q)����
		{
			if (q == (p = p->succ)) break; n--;
		} //p����ϲ����б����滻Ϊ��ֱ�Ӻ��
		else //��p�ѳ����ҽ��v(q) < v(p)����
		{
			insertBefore(p, L.remove((q = q->succ)->pred)); m--;
		} //��qת����p֮ǰ
	p = pp->succ; //ȷ���鲢������ģ��£����
}

template<typename T>
void List<T>::mergeSort(ListNodePosi(T)& p, int n) {
	if (n < 2)return;
	int m = n >> 1;
	ListNodePosi(T) q = p;
	for (int i = 0; i < m; i++)q = q->succ;
	mergeSort(p, m); mergeSort(q, n - m);//�ֱ�����
	merge(p, m, *this, q, n - m);
}
