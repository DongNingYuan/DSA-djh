/*
���ڲ���Fibonaaci���и�����ⷽ����Ч�ʲ��
�����������ݽṹ��C++���԰棩P24-25
*/
#include <iostream>
#include <time.h>
using namespace std;
__int64 pre;
__int64 fib1(int n) {//���ֵݹ�� ʱ�临�Ӷ�O(2^n)
	if (2 > n) { return n; }
	return fib1(n - 1) + fib1(n - 2);
}
__int64 fib2(int n, __int64 & prev) {//���Եݹ�� ʱ�临�Ӷ�O(n)
	if (0 == n) {
		prev = 1; return 0;
	}
	else {
		__int64 prevprev; prev = fib2(n - 1, prevprev);
		return prevprev + prev;
	}
}

__int64 fib3(int n) {//������ ʱ�临�Ӷ�O(n)
	__int64 f = 1, g = 0;
	while (n--) {
		f = f + g;
		g = f - g;
	}
	return g;
}
int main() {
	cout << "Please enter the max entry:" << endl;
	int N; cin >> N;
	for(int i=1;i<=N;i++) {
		clock_t start1, end1;
		start1 = clock();
		cout << "fib1(" << i << ") = " << fib1(i);
		end1 = clock();
		cout << " , time = " << end1 - start1 << "ms" << endl;
		clock_t start2, end2;
		start2 = clock();
		cout << "fib2(" << i << ") = " << fib2(i,pre);
		end2 = clock();
		cout << " , time = " << end2 - start2 << "ms" << endl;
		clock_t start3, end3;
		start3 = clock();
		cout << "fib3(" << i << ") = " << fib3(i);
		end3 = clock();
		cout << " , time = " << end3 - start3 << "ms" << endl;

	}
	return 0;
}