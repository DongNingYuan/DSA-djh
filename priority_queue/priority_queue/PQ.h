template<typename T>
struct PQ {//priority_queueģ����
	virtual void insert(T) = 0;
	virtual T getMax() = 0;
	virtual T delMax() = 0;
};