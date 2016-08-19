#include <string>
typedef int size_t;
//Brutal-Force �O(m*n) expected�ɴ�O(n)
/*
* Text:     0 1 2 3 - - - - - i-j - - - - i - - - - n-1
							   |- - - - - |
* Pattern:					   0 - - - - -j
*/
int BFmatch1(char* P, char *T) {//��ƥ���㷨(Brute-force-1)
	size_t n = strlen(T), i = 0;//�������ȣ���ǰ���ܱȶ��ַ�λ��
	size_t m = strlen(P), j = 0;//ģʽ������
	while (i < n && j < m) {
		if (T[i] == P[j]) {
			i++; j++;
		}
		else {
			i -= j - 1; j = 0;//�������ˣ�ģʽ����λ
		}
	}
	return i - j;//���ɹ�����i-jΪ�׸�ƥ��λ�ã���ʧ�ܣ���i-j>n-m
}


/*
* Text:     0 1 2 3 - - - - - i i+1 - - - i+j - - - - n-1
|  - - - - - |
* Pattern:					  0  1 - - - - j
*/
int BFmatch2(char* P, char *T) {//��ƥ���㷨(Brute-force-2)
	size_t n = strlen(T), i = 0;//�������ȣ���ģʽ�����ַ������λ��
	size_t m = strlen(P), j;//ģʽ������,��ǰ���ܱȶ�λ��
	for (i = 0; i < n - m + 1; i++) {
		for (j = 0; j < m; j++) {
			if (T[i + j] != P[j])break;
		}
		if (j >= m)break;//�ҵ�ƥ���ִ�
	}
	
	return i - j;//���ɹ�����i-jΪ�׸�ƥ��λ�ã���ʧ�ܣ���i-j>n-m
}
//

//KMP �O(n+m)
int *buildNext(char* P) {
	size_t m = strlen(P), j = 0;
	int* N = new int[m];
	int t = N[0] = -1;
	while (j < m - 1) {
		if (0 > t || P[j] == P[t]) {
			j++; t++;
			//N[j]=t;
			//�Ľ�
			N[j] = (P[j] != P[t]) ? t : N[t];
		}
		else {
			t = N[t];
		}
	}
	return N;
}
/*
* Text:     0 1 2 3 - - - - - i-j - - - - i - - - - n-1
							   |- - - - - |
* Pattern:					   0 - - - - -j
*/
int KMPmatchz(char* P, char* T) {//KMP�㷨
	int* next = buildNext(P);
	int n = (int)strlen(T), i = 0;
	int m = (int)strlen(P), j = 0;
	while ((j < m) && (i < n)) {
		if ((0 > j) || (T[i] == P[j])) {
			i++; j++;
		}
		else {
			j = next[j];
		}
	}
	delete[] next;
	return i - j;

}