/*
���ݽṹC++ �ڶ���
*/
#include <iostream>
using namespace std;
typedef int Rank;
#define DEFAULT_CAPACITY 3
template <typename T> class Vector {
protected:
	Rank _size; int _capacity; T* _elem;
	void copyFrom(T const* A, Rank lo, Rank hi);
	void expand();
	void shrink();
	bool bubble(Rank lo, Rank hi);//ɨ�轻��
	void bubbleSort(Rank lo, Rank hi);//��������
	Rank max(Rank lo, Rank hi);
	void selectionSort(Rank lo, Rank hi);//ѡ������
	void merge(Rank lo, Rank mi, Rank hi);//�鲢�㷨
	void mergeSort(Rank lo, Rank hi);//�鲢�����㷨
	Rank partition(Rank lo, Rank hi);//��㹹���㷨
	void quickSort(Rank lo, Rank hi);//���������㷨
	void heapSort(Rank lo, Rank hi);//�������㷨

									//�Ƚ���
	template<typename T> static bool lt(T* a, T* b) { return lt(*a, *b); }
	template<typename T> static bool lt(T& a, T& b) { return a<b; }
	template<typename T> static bool eq(T* a, T* b) { return eq(*a, *b); }
	template<typename T> static bool eq(T& a, T& b) { return a == b; }

public:
	//constructor
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {//����Ϊc����ģΪs������Ԫ�س�ʼ��Ϊv
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }//�������临��
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); }//�������帴��
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }//�������临��
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }//�������帴��
																 //destructor
	~Vector() { delete[] _elem; }
	//ֻ���ӿ�
	Rank size() const { return _size; }
	bool empty() const { return !_size; }
	int disordered() const;//�Ƿ�����
	Rank find(T const& e) const { return find(e, 0, (Rank)_size); }//���������������
	Rank find(T const& e, Rank lo, Rank hi)const;//���������������
	Rank search(T const& e)const { return (0 >= _size) ? -1 : search(e, 0, _size); }//���������������
	Rank search(T const& e, Rank lo, Rank hi)const;//���������������

												   //��д���ʽӿ�
	T& operator[](Rank r) { return _elem[r]; }
	Vector<T> & operator=(Vector<T> const&);
	T remove(Rank r) { T e = _elem[r]; remove(r, r + 1); return e; }
	int remove(Rank lo, Rank hi);//����ɾ���㷨
	Rank insert(Rank r, T const& e);
	Rank insert(T const& e) { return insert(_size, e); }//��ĩβ����Ԫ��
	void sort(Rank lo, Rank hi);//��[lo,hi)��������
	void sort() { sort(0, _size); }//��������
	void unsort(Rank lo, Rank hi);//��������
	void unsort() { unsort(0, _size); }//��������
	int deduplicate();//����ȥ��
	int uniquify();//����ȥ��

				   //����
	void traverse(void(*visit)(T&)) {//������ʹ�ú���ָ�룬������ֻ����ֲ����޸ģ�
		for (Rank i = 0; i < _size; i++) {
			visit(_elem[i]);
		}
	}
	template<typename VST> void traverse(VST& visit) { //������ʹ�ú�������ȫ�����޸ģ�
		for (Rank i = 0; i < _size; i++) {
			visit(_elem[i]);
		}
	}
};

template<typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
	_elem = new T[_capacity = 2 * (hi - lo)];
	_size = 0;
	while (lo < hi) {
		_elem[_size++] = A[lo++];
	}
}

template<typename T>
Vector<T> & Vector<T>::operator=(Vector<T> const& V) {
	if (_elem) { delete[] _elem; }
	copyFrom(V._elem, 0, V._size);
	return *this;
}

template<typename T>
void Vector<T>::expand() {
	if (_size < _capacity) { return; }
	if (_capacity < DEFAULT_CAPACITY) { _capacity = DEFAULT_CAPACITY; }
	T* oldelem = _elem;
	_elem = new T[_capacity <<= 1];
	for (Rank i = 0; i < _size; i++) {
		_elem[i] = oldelem[i];
	}
	delete[] oldelem;
}

template<typename T>
void Vector<T>::shrink() {
	if (_capacity < DEFAULT_CAPACITY) { return; }
	if (_size << 2 > _capacity) { return; }//��25%Ϊ��
	T* oldelem = _elem;
	_elem = new T[_capacity >>= 1];
	for (Rank i = 0; i < _size; i++) {
		_elem[i] = oldelem[i];
	}
	delete[] oldelem;
}
/*gcc������rand() compile error
template<typename T>
void Vector<T> :: unsort(Rank lo, Rank hi) {
T* V = _elem + lo;
for (Rank i = hi - lo; i > 0; i--) {
swap(V[i-1], V[rand() % i]);
}
}*/
template<typename T>//����������˳����ң��������һ��Ԫ�ص��ȣ�ʧ�ܷ���lo-1
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
	while ((lo < hi--) && (_elem[hi] != e));
	return hi;
}

template<typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
	expand();
	for (Rank i = _size; i > r; i--) {
		_elem[i] = _elem[i - 1];
	}
	_elem[r] = e; _size++;
	return r;
}

template<typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
	if (lo == hi) { return 0; }
	while (hi < _size) { _elem[lo++] = _elem[hi++]; }
	_size = lo;
	shrink();
	return hi - lo;
}

template<typename T>
int Vector<T>::deduplicate() {
	int oldsize = _size;
	Rank i = 1;
	while (i < _size) {
		(find(_elem[i], 0, i) < 0) ? i++ : remove(i);
	}
	return _size - oldsize;//����������ģ�仯��
}
template<typename T>
int Vector<T>::uniquify() {//����������Ψһ������Ч�汾��
	Rank i = 0, j = 0;
	while (++j < _size) {
		if (_elem[i] != _elem[j]) {
			_elem[++i] = _elem[j];
		}
	}
	_size = ++i; shrink();
	return j - i;//���ر�ɾ����Ԫ������
}

template<typename T>
static Rank binSearchA(T* A, T const& e, Rank lo, Rank hi) {
	while (lo < hi) {
		Rank mi = (lo + hi) >> 1;
		if (e < A[mi]) { hi = mi; }
		else if (A[mi] > e) { lo = mi; }
		else return mi;
	}
	return -1;//fail
}

//omit fibnacci search

template<typename T>
static Rank binSearchC(T* A, T const& e, Rank lo, Rank hi) {
	while (lo < hi) {
		Rank mi = (lo + hi) >> 1;
		(e < A[mi]) ? hi = mi : lo = mi + 1;
	}
	return --lo;
}
template<typename T>
Rank Vector<T>::search(T const & e, Rank lo, Rank hi) const
{
	return binSearchC(_elem, e, lo, hi);
}

template<typename T>
void Vector<T>::sort(Rank lo, Rank hi)
{
	mergeSort(lo, hi);
}
//�鲢����
template<typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
	if (hi - lo < 2) { return; }
	Rank mi = (lo + hi) >> 1;
	mergeSort(lo, mi);
	mergeSort(mi, hi);
	merge(lo, mi, hi);
}

template<typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
	T* A = _elem + lo;//�ϲ�������A[0,hi-li)=_elem[lo,hi)
	Rank lb = mi - lo; T* B = new T[lb];
	for (Rank i = 0; i < lb; B[i] = A[i++]);//����ǰ������B[lo,mi)
	Rank lc = hi - mi; T* C = _elem + mi;//��������C[mi,hi)
	for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);) {
		if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) { A[i++] = B[j++]; }
		if ((k < lc) && (!(j < lb) || (C[k] < B[j]))) { A[i++] = C[k++]; }
	}
	delete[] B;

}
