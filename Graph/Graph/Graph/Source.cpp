typedef enum{UNDISCOVERED,DISCOVERED,VISITED} VStatus;
typedef enum{UNDETERMINED,TREE,CROSS,FORWARD,BACKWARD} EStatus;
template <typename Tv,typename Te>//�������ͣ�������
class Graph {
private:
	void reset() {
		for (int i = 0; i < n; i++) {
			status(i) = UNDISCOVERED; dtime(i) = ftime(i) = -1;
			parent(i) = -1; priority(i) = INT_MAX;
			for (int j = 0; j < n; j++) {
				if (exists(i, j)) status(i, j) = UNDETERMINED;
			}
		}
	}
	void BFS(int, int &);//(��ͨ��)�������
	void DFS(int, int &);//(��ͨ��)�������
	void BCC(int, int &, Stack<int>&);//(��ͨ��)����DFS��˫��ͨ�����ֽ��㷨
	bool TSort(int, int &, Stack<Tv>*);//(��ͨ��)����DFS�����������㷨
	template<typename PU> void PFS(int, PU);//(��ͨ��)���ȼ��������
pulic:
	//����
	int n;
	virtual int insert(T const&) = 0;//return i
	virtual Tv remove(int) = 0;
	virtual Tv& vertex(int) = 0;//����v������
	virtual int inDegree(int) = 0;//���
	virtual int outDegree(int) = 0;//����
	virtual int firstNbr(int) = 0;//����v���׸��ڽӽڵ�
	virtual int nextNbr(int, int) = 0;//����v����ڶ���j����һ���ڽӽڵ�
	virtual VStatus& status(int) = 0;
	virtual int& dTime(int) = 0;
	virtual int& fTime(int) = 0;
	virtual int& parent(int) = 0;//����v�ĸ���
	virtual int& priority(int) = 0;//����v�����ȼ���
	//�� Լ���������ͳһת��Ϊ�����һ������ߣ��Ӷ�������ͼ��������ͼ������
	int e;
	virtual bool exists(int, int) = 0;//��u��v�Ƿ����
	virtual bool insert(Te const&, int, int, int) = 0;//�ڶ���v��u֮�����Ȩ��Ϊw�ı�e
	virtual Te remove(int, int) = 0;
	virtual EStatus& status(int, int) = 0;//��(u,v)������
	virtual Te& edge(int, int) = 0;
	virtual int& weight(int, int) = 0;//��(u,v)��Ȩ��
	//algorithm
	void bfs(int);
	void dfs(int);
	void bcc(int);
	Stack<Tv>* tSort(int);//����DFS�����������㷨
	void prim(int);//��С֧����Prim�㷨
	void dijkstra(int);//���·��Dijkstra�㷨
	template<typename PU> void pfs(int, PU) = 0;//���ȼ��������
};