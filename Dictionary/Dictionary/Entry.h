template<typename K,typename V>
struct Entry {//����ģ����
	K key; V value;
	Entry(K k, V v) :key(k), value(v) {};
	Entry(Entry<K, V> const& e)key(e.key), value(e.value) {};//���ƹ��캯��
	bool operator==(Entry<K, V> const& e) { return key == e.key; }
	bool operator!=(Entry<K, V> const&e) { return key != e.key; }
	//��Dict��ֻ��Ҫ֧���еȼ���
};