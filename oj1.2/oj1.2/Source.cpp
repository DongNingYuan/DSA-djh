/*
����(Zuma)
Description
Let's play the game Zuma!

There are a sequence of beads on a track at the right beginning. All the beads are colored but no three adjacent ones are allowed to be with a same color. You can then insert beads one by one into the sequence. Once three (or more) beads with a same color become adjacent due to an insertion, they will vanish immediately.



Note that it is possible for such a case to happen for more than once for a single insertion. You can't insert the next bead until all the eliminations have been done.

Given both the initial sequence and the insertion series, you are now asked by the fans to provide a playback tool for replaying their games. In other words, the sequence of beads after all possible eliminations as a result of each insertion should be calculated.

Input
The first line gives the initial bead sequence. Namely, it is a string of capital letters from 'A' to 'Z', where different letters correspond to beads with different colors.

The second line just consists of a single interger n, i.e., the number of insertions.

The following n lines tell all the insertions in turn. Each contains an integer k and a capital letter ��, giving the rank and the color of the next bead to be inserted respectively. Specifically, k ranges from 0 to m when there are currently m beads on the track.

Output
n lines of capital letters, i.e., the evolutionary history of the bead sequence.

Specially, "-" stands for an empty sequence.

Example
Input

ACCBA
5
1 B
0 A
2 B
4 C
0 A
Output

ABCCBA
AABCCBA
AABBCCBA
-
A
Restrictions
0 <= n <= 10^4

0 <= length of the initial sequence <= 10^4

Time: 2 sec

Memory: 256 MB

Hints
List

����
������һ����������ȫ�����Ϸ�����淨�ǣ���һ������ϳ�ʼ���������ɸ���ɫ���ӣ����������������ڵ����Ӳ�����ȫͬɫ���˺�����Է������ӵ�����ϲ�����ԭ�������С�һ�������������ͬɫ�����ӱ�����ڣ����Ǿͻ�������ʧ����������������ܻ�����ʽ����������㽫��ʱ���ܷ������ӡ�

���������׼��Ϊ���дһ����Ϸ���̵ĻطŹ��ߡ������Ѿ�����Ϸ������˹��̼�¼�Ĺ��ܣ����طŹ��ܵ�ʵ����ί��������ɡ�

��Ϸ���̵ļ�¼�У������ǹ���ϳ�ʼ���������У�Ȼ������ҽ�����������һϵ�в�������������ǣ��ڸ��β���֮��ʱ������µ��������С�

����
��һ����һ���ɴ�д��ĸ'A'~'Z'��ɵ��ַ�������ʾ����ϳ�ʼ���������У���ͬ����ĸ��ʾ��ͬ����ɫ��

�ڶ�����һ������n����ʾ�����طŹ��̹���n�β�����

��������n�����ζ�Ӧ�ڸ��β�����ÿ�β�����һ������k��һ����д��ĸ���������Կո�ָ������У���Ϊ�����ӵ���ɫ��������ǰ����m�����ӣ���k �� [0, m]��ʾ������Ƕ��֮����δ��������֮ǰ���ڹ���ϵ�λ��

���
�����n�У����θ������β������������漴��������������֮�����ϵ��������С�

����������û�����ӣ����ԡ�-����ʾ��

����
��Ӣ������

����
0 �� n �� 10^4

0 �� ��ʼ�������� �� 10^4

ʱ�䣺2 sec

�ڴ棺256 MB

��ʾ
�б�
*/
//WA on test2 TLE on test20
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
using namespace std;

typedef int Rank;
#define ListNodePosi(T) ListNode<T>*//�б�ڵ�λ��

template <typename T> struct ListNode { //�б�ڵ�ģ���ࣨ��˫��������ʽʵ�֣�
										// ��Ա
	T data; ListNodePosi(T) pred; ListNodePosi(T) succ; //��ֵ��ǰ�������
														// ���캯��
	ListNode() {} //���header��trailer�Ĺ���
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
		: data(e), pred(p), succ(s) {} //Ĭ�Ϲ�����
									   // �����ӿ�
	ListNodePosi(T) insertAsPred(T const& e); //������ǰ�ڵ�֮ǰ�����½ڵ�
	ListNodePosi(T) insertAsSucc(T const& e); //���浱ǰ�ڵ�֮������½ڵ�
};

template<typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e) {
	ListNodePosi(T) x = new ListNode(e, pred, this);
	pred->succ = x;
	pred = x;
	return x;
}
template<typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e) {
	ListNodePosi(T) x = new ListNode(e, this, succ);
	succ->pred = x;
	succ = x;
	return x;
}
template<typename T> class List {
public:
	int _size; ListNodePosi(T) header; ListNodePosi(T) tailor;
protected:
	void init();//�б���ʱ��ʼ��
	int clear();
public:
	//constructor
	List() { init(); }
	~List() { clear(); delete header; delete tailor; }
	//read-only interface
	Rank size() const { return _size; }
	bool empty() const { return _size <= 0; }
	T& operator[](int r) const;//ѭ�ȷ��ʣ�Ч�ʵ���
	ListNodePosi(T) first() const { return header->succ; }
	ListNodePosi(T) last() const { return tailor->pred; }
	bool valid(ListNodePosi(T) p) { return p && (p != tailor)&(p != header); }//�ж�p�Ƿ�Ϸ�
	void printAll()const;
	//read-write interface
	ListNodePosi(T) insertAsFirst(T const& e);
	ListNodePosi(T) insertAsLast(T const& e);
	ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const& e);
	ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const& e);
	ListNodePosi(T) insertAt(Rank i, T const& e);
	ListNodePosi(T) visitByRank(Rank r);
	T remove(ListNodePosi(T) p);//ɾ��p���Ľڵ㲢���ر�ɾ���Ľڵ�����

};

template<typename T>
void List<T>::init() {//��ʼ�����ڴ����б����ʱͳһ����
	header = new ListNode<T>;
	tailor = new ListNode<T>;
	header->succ = tailor; header->pred = NULL;
	tailor->pred = header; tailor->succ = NULL;
	_size = 0;
}
template<typename T>
void List<T>::printAll() const
{
	if (empty()) {
		printf("-\n");
	}
	else {
		for (ListNodePosi(T) p = header->succ; p != tailor; p = p->succ) {
			printf("%c", p->data);
		}
		printf("\n");
	}
}
template<typename T>
ListNodePosi(T) List<T>::visitByRank(Rank r)
{
	ListNodePosi(T) p = first();
	while (0 < r--)p = p->succ;
	return p;
}
template<typename T>
T& List<T>::operator[](int r) const {
	ListNodePosi(T) p = first();
	while (0 < r--)p = p->succ;
	return p->data;
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
ListNodePosi(T) List<T>::insertAt(Rank i, T const & e)
{
	ListNodePosi(T) p = first();
	while (0 < i--)p = p->succ;
	_size++;
	return p->insertAsPred(e);
}

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



#define maxL 10010
List<char> str;
void solve(int n) {
	bool flag = true;
	int l = n, r = n;
	ListNodePosi(char) pl = str.visitByRank(n);
	while (flag) {
		ListNodePosi(char) p = pl;;
		ListNodePosi(char) pr = p;
		for (; pl->data == p->data; pl = pl->pred) {
			l--;
			if (pl == str.header) break;
			
		}
		for (; pr->data == p->data; pr = pr->succ) {
			r++;
			if (pr == str.tailor)break;
		}
		if (r - l >= 4) {
			for (int i = 1; i < r - l; i++) {
				str.remove(pl->succ);
			}				
			flag = true;
			r = l;

		}
		else {
			flag = false;
		}
	}

}
int main() {
	char s[maxL];
	scanf("%s", s);
	for (int i = 0; s[i] != '\0'; i++) {
		str.insertAsLast(s[i]);
	}
	int N; scanf("%d", &N);
	int id; char letter;
	for (int i = 0; i < N; i++) {
		scanf("%d %c", &id, &letter);
		str.insertAt(id,letter);
		solve(id);
		str.printAll();
	}
	return 0;
}