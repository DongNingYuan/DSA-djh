#include "Entry.h"
#include <algorithm>
#define QuadlistNodePosi(T) QuadlishNode<T>*

template<typename T>
struct QuadlistNode {//������ڵ�ģ����
	T entry;
	QuadlistNodePosi(T) pred; QuadlistNodePosi(T) succ;//predecessor and successor
	QuadlistNodePosi(T) above; QuadlistNodePosi(T) below;
	QuadlistNode(T e = T(), QuadlistNodePosi(T) p = NULL, QuadlistNodePosi(T) s = NULL,
		QuadlistNodePosi(T) a = NULL, QuadlistNodePosi(T) b = NULL) :
		entry(e), pred(p), succ(s), above(a), below(b) {}
	QuadlistNodePosi(T) insertAsSuccAbove(T const& e, QuadlistNodePosi(T) b = NULL);
	//�����½ڵ㣬�Ե�ǰ�ڵ�Ϊǰ�����ڵ�bΪ����
};

template<typename T>
QuadlistNodePosi(T) QuadlistNode<T>::insertAsSuccAbove(T const &e, QuadlistNodePosi(T) b = NULL) {
	QuadlistNodePosi(T) x = new QuadlistNode<T>(e, this, succ, NULL, b);
	succ->pred = x; succ = x;
	if (b) b->above = x;
	return x;
}//����ǰ�ڵ�b������Ϊ��