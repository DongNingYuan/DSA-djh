#include <stack>
#include "BinNode.h"
using namespace std;
template<typename T> class BinTree {
protected:
	int _size;
	BinNodePosi(T) _root;
	virtual int updateHeight(BinNodePosi(T) x);//����x�ĸ߶�
	void updateHeightAbove(BinNodePosi(T) x);//����x�������ȵĸ߶�
public:
	BinTree() :_size(0), _root(NULL) {}//constructor
	~BinTree() { if (0 < _size) remove(_root); }//destructor
	int size() const { return _size; }
	bool empty() const { return !_root; }
	BinNodePosi(T) root() const { return _root; }
	BinNodePosi(T) insertAsRoot(T const& e);
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e);//insert e as left child of x
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &T);//insert T as left subtree of x
	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &T);
	int remove(BinNodePosi(T) x);//delete the subtree whose root is x, return original size of the subtree
	BinTree<T>* secede(BinNodePosi(T) x);//ժ������x����Ϊһ�ö�����������
	template<typename VST>
	void travLevel(VST& visit) { if (_root) _root->travLevel(visit); }//��α���
	template<typename VST>
	void travPre(VST& visit) { if (_root) _root->travPre(visit); }
	template<typename VST>
	void travIn(VST& visit) { if (_root) _root->travIn(visit); }
	template<typename VST>
	void travPost(VST& visit) { if (_root) _root->travPost(visit); }

	//comparator
	bool operator<(BinTree<T> const& t) { return _root && t._root && lt(_root, t._root); }
	bool operator==(BinTree<T> const& t) { return _root &&t._root && (_root == t._root); }
};

template<typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) {//O(depth(v)+1)
	return x->height = 1 + max(stature(x->lChild), stature(x->rChild));//����������������
}
template<typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) {
	while (x) { updateHeight(x); x = x->parent; }
}
template<typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &S) {
	//��S��Ϊ�ڵ�x�����������룬S�����ÿ�
	if (x->lChild = S->_root) x->lChild->parent = x;//??
	_size += S->_size; updateHeightAbove(x);
	S->_root = NULL; S->_size = 0; release(S); S = NULL; return x;
}
template<typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &S) {
	//��S��Ϊ�ڵ�x�����������룬S�����ÿ�
	if (x->rChild = S->_root) x->rChild->parent = x;//??
	_size += S->_size; updateHeightAbove(x);
	S->_root = NULL; S->_size = 0; release(S); S = NULL; return x;
}
template<typename T>
int BinTree<T>::remove(BinNodePosi(T) x) {//assert:x is valid in the tree
	FromParentTo(*x) = NULL;
	updateHeightAbove(x->parent);//�������ȸ߶�
	int n = removeAt(x); _size -= n; return n;
}

template<typename T>
static int removeAt(BinNodePosi(T) x) {//ɾ��x�����������ر�ɾ���ڵ����ֵ
	if (!x)return 0;
	int n = 1 + removeAt(x->lChild) + removeAt(x->rChild);
	release(x->data); release(x); return n;
}

template<typename T>
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) {//������x�ӵ�ǰ����ժ��������װΪһ�ö�����������
	FromParentTo(*x) = NULL;
	updateHeightAbove(x->parent);
	BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = NULL;
	S->_size = x->size(); _size -= S->_size; return S;
}
//�������
template<typename T, typename VST>
void travPre_R(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	visit(x->data);
	travPre_R(x->lChild, visit);
	travPre_R(x->rChild, visit);
}
template<typename T, typename VST>
void travPre_I1(BinNodePosi(T) x, VST& visit) {
	stack<BinNodePosi(T)> S;
	if (x) S.push(x);
	while (!S.empty()) {
		x = S.pop();
		if (HasRChild(*x))S.push(x->rChild);
		if (HasLChild(*x))S.push(x->lChild);
	}
}
template<typename T, typename VST>
static void visitAlongLeftBranch(BinNodePosi(T) x, VST& visit, stack<BinNodePosi(T)>& S) {
	//�ӵ�ǰ�ڵ�����������֧��������ֱ��û�����֧�Ľڵ㣬��;�����ڵ���������
	while (x) {
		visit(x->data);
		S.push(x->rChild);
		x = x->lChild;
	}
}
template<typename T, typename VST>
void travPre_I2(BinNodePosi(T) x, VST& visit) {
	stack<BinNodePosi(T)> S;
	while (true) {
		visitAlongLeftBranch(x, visit, S);
		if (S.empty())break;
		x = S.pop();
	}
}
//

//�������
template<typename T,typename VST>
void travIn_R(BinNodePosi(T) x, VST& visit) {
	if (x) return;
	if (HasRChild(*x)) travIn_R(x->rChild, visit);
	visit(x->data);
	if (HasLChild(*x))travIn_R(x->lChild, visit);
}

template<typename T>
static void goAlongLeftBranch(BinNodePosi(T) x, stack<BinNodePosi(T)>& S) {
	while (x) { S.push(x); x = x->lChild; }
}
template<typename T,typename VST>
void travIn_I1(BinNodePosi(T) x, VST& visit) {
	stack<BinNodePosi(T)> S;
	while (true) {
		goAlongLeftBranch(x, S);
		if (S.empty())break;
		x = S.pop();
		visit(x->data);
		x = x->rChild;
	}
}
template<typename T,typename VST>
void travIn_I2(BinNodePosi(T) x, VST& visit) {
	stack<BinNodePosi(T)> S;
	while (true) {
		if (x) {
			S.push(x);
			x = x->lChild;
		}
		else if (!S.empty()) {
			x = S.pop();
			visit(x->data);
			x = x->rChild;
		}
		else break;
	}//ΪI�ĵ�Ч��ʽ
}
template<typename T,typename VST>
void travIn_I3(BinNodePosi(T) x, VST& visit) {
	//��2��չ���������븨��ջ
	bool backtrack = false;//ǰһ���Ƿ�մ�����������
	while (true) {
		if (!backtrack && HasLchild(*x)) //�����������Ҳ��Ǹոջ���
			x = x->lChild;
		else {//��������������ոջ��ݣ������������������
			visit(x->data);
			if (HasRChild(*x)) {
				x = x->rChild;
				backtrack = false;
			}
			else {//������Ϊ��
				if (!(x = x->succ())) break;//���ݣ����ҵִ�ĩ�ڵ�ʱ�˳�ѭ��
				backtrack = true;
			}

		}
	}
}

//�������
template<typename T>
void travPost_R(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	if (HasRChild(*x)) travPost_R(x->rChild, visit);
	if (HasLChild(*x)) travPost_R(x->lChild, visit);
	visit(x->data);
}
template<typename T>
static void gotoHLVFL(stack<BinNodePosi(T)>& S) {
	//����S��ջ���ڵ�Ϊ���������У��ҵ������Ŀɼ�Ҷ�ڵ㣬����������Ҳ�������Һ��ӣ����������������ʵĽڵ�
	while (BinNodePosi(T) x = S.top()) {//�Զ����·������ջ���ڵ�
		if (HasLChild(*x)) {
			if (HasRChild(*x))S.push(x->rChild);
			S.push(x->lChild);
		}
		else
			S.push(x->rChild);
		S.pop();
	}
}
template<typename T,typename VST>
void travPost_I(BinNodePosi(T) x, VST& visit) {
	stack<BinNodePosi(T)> S;
	if (x) S.push();//���ڵ���ջ
	while (!S.empty()) {
		if (S.top() != x->parent)//��ջ�����ǵ�ǰ�ڵ�֮�������Ϊ�����ֵ�
			gotoHLVFL(S);//����������Ϊ���������У��ҵ�HLVFL
		x = S.pop(); visit(x->data);
	}
}

int main() {
	return 0;
}