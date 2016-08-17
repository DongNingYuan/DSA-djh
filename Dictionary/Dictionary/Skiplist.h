#include "Dictionary.h"
#include "Entry.h"
#include "List.h"
#include "Quadlist.h"
/*
�����֮�����򹹳�һ��list��ÿ��Ϊһ��Quadlist����Ӧ��һ��ListNode��data
*/
template<typename K,typename V>//��ת��ģ����
class Skiplist :public Dictionary<K, V>, public List<Quadlist<Entry<K, V>>*> {
protected:
	bool skipSearch(ListNode<Quadlist<Entry<K, V>>*>* &qlist, QuadlistNode<Entry<K, V>>* &p, K& k);
public:
	int size() const { return empty() ? 0 : last()->data->size(); }//List::last()��ӦQuadlist��bottom
	int level() { return List::size(); }//��� == #Quadlist
	bool put(K, V);//���루��Ϊdict�����ظ���Ȼ�ɹ���
	V* get(K k);
	bool remove(K k);//ɾ��
};

template<typename K,typename V>
V* Skiplist<K, V>::get(K k) {//��ת����������㷨
	if (empty()) return NULL;
	ListNode<Quadlist<Entry<K, V>>*>* qlist = first();//����Quadlist��
	QuadlistNode<Entry<K, V>>* p = qlist->data->first();//��һ���ڵ㿪ʼ
	return skipSearch(qlist, p, k) ? &(p->entry.value) : NULL;
}//������п���������

/*
* Skiplist���������㷨�������ڲ�����
* ��ڣ�qlistΪ�����б�pΪqlist���׽ڵ�
* ���ڣ����ɹ���pΪ���йؼ����������Ķ����ڵ㣬qlistΪp�����б�
* 	   ����pΪ�������Ļ�����������Ӧ�ڲ�����k���������ҵĹؼ��룬qlistΪ��
* Լ���������������ʱ����������ȡ�����
*/
template<typename K,typename V>
bool Skiplist<K, V>::skipSearch(
	ListNode<Quadlist<Entry<K, V>>*>* &qlist,//ָ�����list
	QuadlistNode<Entry<K, V>>* &p,//�׽ڵ�p����
	K& k) {//�������²��ҹؼ���k
	while (true) {
		while (p->succ && (p->entry.key <= k)) {
			p = p->succ;
		}//ֱ����������key����p=tailor
		p = p->pred;
		if (p->pred && (k == p->entry.key))return true;//����
		qlist = qlist->succ;//ת����һ��
		if (!qlist->succ)return false;//�ѵ��ײ�
		p = (p->pred) ? p->below() : qlist->data->first();//ת����ǰ������һ�ڵ�
	}
}

template<typename K,typename V>
bool Skiplist<K, V>::put(K k, V v) {
	Entry<K, V> e = Entry<K, V>(k, v);//������Ĵ���
	if (empty()) insertAsFisrt(new Quadlist<Entry<K, V>>);
	ListNode<Quadlist<Entry<K, V>>*>* qlist = first();//�Ӷ����������
	QuadlistNode<Entry<K, V>>* p = qlist->data->first();//�׽ڵ����
	if (skipSearch(qlist, p, k))//���Һ��ʵĲ���λ�ã������ڹؼ���k�����һ���ڵ�p
		while (p->below)p = p->below;//ǿ��ת������
	qlist = last();//���£�������p���Ҳ࣬һ��ո�µ������Ե׶��ϰεض����������
	QuadlistNode<Entry<K, V>>* b = qlist->data->insertAfterAbove(e, p);//bΪ�����Ļ���
	while (rand() % 1) {//��Ӳ����ȷ�����Ƿ���Ҫ����һ��
		while (qlist->data->valid(p) && !p->above)p = p->pred;//�ҳ������ڴ˸߶ȵ����ǰ������һ����ڵģ�
		if (!qlist->data->valid(p)) {//����ǰ����header
			if (qlist == first()) {//��ǰ�������
				insertAsFirst(new Quadlist<Entry<K, V>>);//���ȴ����µ�һ��
			}
			p = qlist->pred->data->first()->pred;//pת����һ��skiplist��header
		}
		else {
			p = p->above;
		}
		qlist = qlist->pred;
		b = qlist->data->insertAfterAbove(e, p, b);//�����½ڵ�
	}
	return true;//Dictionary�����ظ�Ԫ�أ��ʲ����Ȼ�ɹ�����hashtable��map����
}

template<typename K,typename V>
bool Skiplist<K, V>::remove(K k) {//��ת�����ɾ���㷨
	if (empty())return false;
	ListNode<Quadlist<Entry<K, V>>*>* qlist = first();//�Ӷ����������
	QuadlistNode<Entry<K, V>>* p = qlist->data->first();//�׽ڵ����
	if (!skipSearch(qlist, p, k))return false;//Ŀ����������ڣ�ֱ�ӷ���
	do {
		QuadlistNode<Entry<K, V>>* lower = p->below;
		qlist->data->remove(p);//ɾ����ǰ��ڵ�
		p = lower; qlist = qlist->succ;//ת����һ��
	} while (qlist->succ);//ֱ������
	while (!empty() && first()->data->empty()) {
		List::remove(first());//������ܲ��������Ķ���Quadlist
	}
	return true;
}