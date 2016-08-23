/*
���鼯���˲��ֵ˿�����ʦ���������������õ��ǡ���ս������ƾ������ڶ��棩���������ʵ������2013��6�°棩P84 2.44�ڵ�ʵ�֡�
*/
#define maxN 100
int par[maxN];//fathers
int rank[maxN];//height of each tree

void init(int n) {
	for (int i = 0; i < n; i++) {
		par[i] = i;
		rank[i] = 0;
	}
}

//find the root of x
int find(int x) {
	if (par[x] == x) {
		return x;
	}
	else {
		return par[x] = find(par[x]);
	}
}
//unite x's and y's root
void unite(int x, int y) {
	x = find(x);
	y = find(y);
	if (x == y)return;
	if (rank[x] < rank[y]) {
		par[x] = y;
	}
	else {
		par[y] = x;
		if (rank[x] == rank[y]) rank[x]++;
	}
}

bool same(int x, int y) {
	return find(x) == find(y);
}