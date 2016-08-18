#include <cstdlib>
#include <queue>
using namespace std;
#define BinNodePosi(T) BinNode<T>* //node position
#define stature(p) ((p)?(p)->height:-1)// node height(empty tree is -1)
typedef enum { RB_RED, RB_BLACK } RBColor;//node color

										  //�ú�������һЩ������������Ԥ����ʱֱ���滻��������ȥ���ݲ��������ݼĴ����Ŀ�������Ч����CPU cache�����������ִ���ļ����
										  //һ��꺯�����ڼ�̵ı�Ƶ�����õĺ���

										  /*
										  * BinNode״̬�����ʵ��ж�
										  */
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x)&&(&(x)==(x).parent->lChild))
#define IsRChild(x) (!IsRoot(x)&&(&(x)==(x).parent->rChild))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lChild)
#define HasRChild(x) ((x).rChild)
#define HasChild(x) (HasLChild(x)||HasRChild(x))
#define HasBothChild(x) (HasLChild(x)&&HasRChild(x))
#define IsLeaf(x) (!HasChild(x))
										  /*
										  * ��BinNode���ض���ϵ�Ľڵ��ָ�루�꺯������Ӧ����ĩ��/��
										  */
#define sibling(p)(\
	IsLChild(*(p)) ? \
		(p)->parent->rChild: \
		(p)->parent->lChild \
)//�ֵ�
#define uncle(x)(\
	IsLChild(*((x)->parent)) ? \
		(p)->parent->parent->rChild: \
		(p)->parent->parent->lChild \
)//����
#define FromParentTo(x)(\
	IsRoot(x) ?_root : (\
		IsLChild(x)?(x).parent->lChild:(x).parent->rChild \
	)\
)//��������x�ĸ��׵�ָ��x��ָ��


template<typename T> struct BinNode {
	//members
	T data;
	BinNodePosi(T) parent; BinNodePosi(T) lChild; BinNodePosi(T) rChild;
	int height;
	int npl;//Null Path Length(�սڵ�ͨ·���ȣ�������ʽ��,Ҳ��ֱ����height����??)
	RBColor color;//Red/Black Tree

				  //constructor
	BinNode() :parent(NULL), lChild(NULL), rChild(NULL), height(0), npl(1), color(RB_RED) {}
	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NUll, BinNodePosi(T) rc = NULL,
		int h = 0, int l = 1, RBColor c = RB_RED) :
		data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c) {}

	//operations
	int size();//ͳ�Ƶ�ǰ�ڵ�ĺ���������༴Ϊ����Ϊ���������Ĺ�ģ
	BinNodePosi(T) insertAsLC(T const&);//insert as left child
	BinNodePosi(T) insertAsRC(T const&);
	BinNodePosi(T) succ();//ȡ��ǰ�ڵ��ֱ�Ӻ��
	template<typename VST> void travLevel(VST&);//��α���
	template<typename VST> void travPre(VST&);//�������
	template<typename VST> void travIn(VST&);//�������
	template<typename VST> void travPost(VST&);//�������

											   //comparators
	bool operator<(BinNode const& bn) { return data < bn.data; }
	bool operator<=(BinNode const& bn) { return data <= bn.data; }
	bool operator>(BinNode const& bn) { return data > bn.data; }
	bool operator>=(BinNode const& bn) { return data >= bn.data; }
	bool operator==(BinNode const& bn) { return data == bn.data; }
};

template<typename T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const& e) { return lChild = new BinNode(e, this); }
template<typename T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const& e) { return rChild = new BinNode(e, this); }

template<typename T> template<typename VST>
void BinNode<T>::travIn(VST& visit) {//�������������ͳһ�ӿ�
	switch (rand() % 5)
	{
	case 1:travIn_I1(this, visit); break;//������#1
	case 2:travIn_I2(this, visit); break;//������#2
	case 3:travIn_I3(this, visit); break;//������#3
	case 4:travIn_I4(this, visit); break;//������#4
	default:travIn_R(this, visit); break;//�ݹ��
	}
}

template<typename T>
BinNodePosi(T) BinNode<T>::succ() {
	//��λ��ǰ�ڵ��ֱ�Ӻ�̣�������������ֱ�Ӻ�̣���Ϊ����������ֱ�Ӻ��
	BinNodePosi(T) s = this;//��¼��̵���ʱ����
	if (rChild) {//������Һ��ӣ���ôֱ�Ӻ�̱�Ȼ���������������Ǹ�
		s = rChild;
		while (HasLChild(*s)) s = s->lChild;
	}
	else {//����ֱ�Ӻ��Ӧ���ǡ�����ǰ�ڵ���������������е�������ȡ�
		while (IsRChild(*s)) s = s->parent;//�����Ϸ��ƶ�
		s = s->parent;//�������Ϸ��ƶ�һ�������ִ���ֱ�Ӻ�̣���ͼ���ɷ�����һ�㣩
	}
	return s;//����NULL ˵����ǰ�ڵ��Ѿ�������������յ�
}
template<typename T> template<typename VST>
void BinNode<T>::travLevel(VST& visit) {
	//��α���
	queue<BinNodePosi(T)> Q;
	Q.enqueue(this);
	while (!Q.empty()) {
		BinNodePosi(T) x = Q.dequeue(); visit(x->data);
		if (HasLChild(*x)) Q.enqueue(x->lChild);
		if (HasRChild(*x)) Q.enqueue(x->rChild);
	}
}
