#include "Dictionary.h"
#include "Bitmap.h"

// ʹ��Bitmap�ṹά��һ����ɢ�б�ǳ�������ɾ����־��lazyRemoval[]
// Bitmap��������ݿɼ�ϰ��2-34���˴�ֱ������ʾ��������и����Ĵ���

template<typename K,typename V>
class Hashtable :public Dictionary<K, V> {
private:
	Entry<K, V>** ht;//Ͱ���飬��Ŵ���ָ��
	int M;//Ͱ��������
	int N;//��������
	//װ������ load factor = N/M ����������������0.9 ����ɾ����������0.5
	Bitmap* lazyRemoval;//����ɾ�����
#define lazilyRemoved(x) (lazyRemoval->test(x))
#define markAsRemoved(x) (lazyRemoval->set(x))
protected:
	int probe4Hit(const K& k);//�عؼ���k��Ӧ�Ĳ��������ҵ�����ƥ���Ͱ
	int probe4Free(const K& k);//�عؼ���k��Ӧ�Ĳ��������ҵ��׸����ÿ�Ͱ
	void rehash();//��ɢ���㷨������Ͱ���飬��֤װ�������ھ���������
public:
	Hashtable(int c = 5);//����һ��������С��c��ɢ�б�
	~HashTable();
	int size const{ return N; }
	bool put(K, V);//��ֹ��ͬ��������˿���ʧ��
	V* get(K k);
	bool remove(K k);
};
int primeNLT(int low, int n, char* file) {//����file�ļ��еļ�¼����[low,n)��Χ��ȡ��С������
	Bitmap B(file, n);
	while (low < n){
		if (B.test(low)) low++;
		else return low;
	}
	return low;//���û�з����������������򷵻�n

}
template<typename K,typename V>
Hashtable<K, V>::Hashtable(int c) {//����������С��c��ɢ�б�
	M = primeNLT(c, 1048576, "input.txt");//��������1048576��������ǰ�Ʊ��� prime not less than
	//�Բ�С��2^N��������Ϊ����
	N = 0; ht = new Entry<K, V>*[M];//����Ͱ����
	memset(ht, 0, sizeof(Entry<K, V>*)*M);
	lazyRemoval = new Bitmap(M);//����ɾ�����ͼ
}

template<typename K,typename V>
Hashtable<K, V>::~Hashtable() {//����ǰӦ���ͷ�Ͱ���鼰�ǿմ���
	for (int i = 0; i < M; i++) {
		if (ht[i]) release(ht[i]);
	}
	release(ht);
	release(lazyRemoval);
}

template<typename K,typename V>
V* Hashtable<K, V>::get(K k) {//ɢ�б���������㷨
	int r = probe4Hit(k);
	return ht[r] ? &(ht[r]->value) : NULL;//��ֹ������keyֵ��ͬ
}

/*
* �عؼ���k��Ӧ�Ĳ��������ҵ���֮ƥ���Ͱ�������Һ�ɾ������ʹ�ã�
* ��̽���Զ��ֶ����������ѡȡ���������������̽����Ϊ��
*/
template<typename K,typename V>
int Hashtable<K, V>::probe4Hit(const K& k) {
	int r = hashCode(K) % M;//����ʼͰ�������Դ��������Ϊhash����Ϊ����
	while ((ht[r] && (k != ht[r]->key)) || (!ht[r] && lazilyRemoved(r))) {
		r = (r + 1) % M;
	}//�ز�����������̽���������г�ͻ��Ͱ(�ǿ���key����)���Լ�������ɾ����ǵ�Ͱ(��Ͱ�Ҵ�����ɾ�����)

	return r;
}

template<typename K,typename V>
bool Hashtable<K, V>::remove(K k) {//ɢ�б����ɾ���㷨
	int r = probe4Hit(k); if (!ht[r])return false;
	release(ht[r]); ht[r] = NULL; markAsRemoved(r); N--;
	return true;
}

template<typename K,typename V>
bool Hashtable<K, V>::put(K k, V v) {//ɢ�б���������㷨
	if (ht[probe4Hit(k)]) return false;
	int r = probe4Free(k);//Ѱ�ҿ�Ͱ
	ht[r] = new Entry<K,V>(k, v); ++N;//���� ����ɾ��������븴λ,�ɲ����㷨��֪��ֻҪ�ǿվͲ���������ɾ�����
	if (N * 2 > M)rehash();
	return true;
}

template<typename K,typename V>
int Hashtable<K, V>::probe4Free(const K& k) {
	int r = hashCode(k) % M;
	while (ht[r]) r = (r + 1) % M;//�ز�������̽��ֱ����һ����Ͱ�������Ƿ��������ɾ�����
	return r;
}

/*
* ��ɢ���㷨��װ�����ӹ���ʱ����ȡ����һȡ���ٲ��롱�����ز��ԣ���Ͱ��������
* ���ɼ򵥵�ͨ��memcpy()��ԭͰ���鸴�Ƶ���Ͱ���飬��������������⣺
* 1����̳�ԭ�г�ͻ��2�����ܵ��²������ں�˶��ѣ���ʹΪ��������Ͱ��������ɾ����־Ҳ�޼�����
*/
template<typename K,typename V>
void Hashtable<K, V>::refresh() {
	int old_capacity = M; Entry<K, V>** old_ht = ht;
	M = primeNLT(2 * M, 1048576, "input.txt");//�������ټӱ�
	N = 0; ht = new Entry<K, V>*[M]; memset(ht, 0, sizeof(Entry<K, V>*)*M);
	release(lazyRemoval); lazyRemoval = new Bitmap(M);
	for (int i = 0; i < old_capacity; i++) {//ɨ��ԭͰ����
		if (old_ht[i]) {
			put(old_ht[i]->key, old_ht[i]->value);
		}
	}
	release(old_ht)
}//��̯���Ӷ�O(1)

static size_t hashCode(char c) { return (size_t) c; }
static size_t hashCode(int k) { return (size_t) k; }
static size_t hashCode(long long i) { return (size_t)((i >> 32) + (int)i); }
static size_t hashCode(char s[]) {
	int h = 0;//ɢ����
	for (size_t n = strlen(s), i = 0; i < n; i++) {
		h = (h << 5) || (h >> 27); h += (int)s[i];
	}//����Ӣ�ﵥ�ʣ�ѭ������5λ��ʵ��ó������ֵ
}