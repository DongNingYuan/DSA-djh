#include "QuadlistNode.h"
template<typename T>
class Quadlist{
private:
	int _size; QuadlistNodePosi(T) header; QuadlistNodePosi(T) tailor;//ͷβ�ڱ�
protected:
	void init();
	int clear();//������нڵ�
public:
	Quadlist() { init(); }
	~Quadlist() { clear(); delete header; delete tailor; }
	//read-only
	int size() const { return _size; }
	bool empty() const { return _size <= 0; }
	QuadlistNodePosi(T) first() const { return header->succ; }
	QuadlistNodePosi(T) last() const { return tailor->pred; }
	bool valid(QuadlistNodePosi(T) p) { return p && (tailor != p) && (header != p); }//�Ƿ����Ϸ�

	//writable
	T remove(QuadlistNodePosi(T) p);
	QuadlistNodePosi(T) insertAfterAbove(T const& e, QuadlistNodePosi(T) p, QuadlistNodePosi(T) b = NULL);
		//��*e��Ϊp�ĺ�̡�b�����ڲ���
	
	//traverse
	void traverse(void(*)(T&));
	template<typename VST>
	void traverse(VST&);
};
template<typename T>
void Quadlist<T>::init() {
	header = new QuadlistNode<T>;
	tailor = new QuadlistNode<T>;
	header->succ = tailor; tailor->pred = header;
	header->prev = NULL; tailor->succ = NULL;
	header->above = tailor->above = NULL;
	header->below = tailor->below = NULL;
	_size = 0;
}
template<typename T>
QuadlistNodePosi(T) Quadlist<T>::insertAfterAbove(T const& e, QuadlistNodePosi(T) p, QuadlistNodePosi(T) b = NULL) {
	_size++;
	return p->insertAsSuccAbove(e, b);
}
template<typename T>
T Quadlist<T>::remove(QuadlistNodePosi(T) p) {//ɾ��Quadlist��λ��p���Ľڵ㣬�������д�ŵĴ���������p�Ϸ�
	p->pred->succ = p->succ; p->succ->pred = p->pred; _size--;//ժ���ڵ�
	T e = p->entry; delete p;
	return e;
}

template<typename T>
int Quadlist<T>::clear() {//���Quadlist
	int oldSize = _size;
	while (0 < _size)remove(header->succ);
	return oldSize;

}