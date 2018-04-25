#include <iostream>
using namespace std;
typedef long long Rank;
template<class T> Rank *get_next(T *P,Rank lp)
{
	Rank *next = new Rank[lp];
	for (Rank i = 0, j = next[0] = -1;i < lp - 1;)
		if (j < 0 || P[i] == P[j]) {
			j++;i++;
			next[i] = P[i] == P[j] ? next[j] : j;
		}
		else j = next[j];
	return next;
}
template<class T> Rank KMP(T *P, Rank lp, T *T,Rank lt)
{
	Rank *next = get_next(P, lp);
	Rank i = 0, j = 0;
	while (i < lp&&j < lt)
		if (i < 0 || P[i] == T[j]) { i++;j++; }
		else i = next[i];
	delete []next;
	return j - i;
}
template<class T> Rank *get_badchar(T *P, Rank lp)
{
	Rank *badchar = new Rank[256];
	for (Rank i = 0;i < 256;i++)badchar[i] = -1;
	for (Rank i = 0;i < lp;i++)badchar[P[i]] = i;
	return badchar;
}
template<class T> Rank BM_BC(T *P, Rank lp, T *T, Rank lt)
{
	Rank *badchar = get_badchar(P, lp);
	Rank i = lp - 1, j = 0;
	while (i >= 0 && j < lt)
		if (P[i] == T[i + j]) i--;
		else if (0 < i - badchar[T[j]]) { j += i - badchar[T[j]];i = lp - 1; }
		else { j++;i = lp - 1; }
	delete [] badchar;
	return j;
}
template<class T> Rank *get_SS(T *P, Rank lp)
{
	Rank *SS = new Rank[lp];SS[lp - 1] = lp;
	for (Rank lo = lp - 1, hi, i = lp - 2;i >= 0;i--)
		if (lo < i&&SS[lp - 1 - hi + i] < i - lo)
			SS[i] = SS[lp - 1 - hi + i];
		else {
			hi = i;lo = lo < hi ? lo : hi;
			while (lo >= 0 && P[lo] == P[lp - 1 - hi + lo])lo--;
			SS[i] = hi - lo;
		}
	return SS;
}
template<class T> Rank *get_GS(T *P, Rank lp)
{
	Rank *SS = get_SS(P, lp), *GS = new Rank[lp];
	for (Rank i = 0;i < lp;i++)GS[i] = lp;
	for (Rank i = lp - 1, j = 0;i >= 0;i--)
		if (SS[i] == i + 1)while (j < lp - 1 - i)
			GS[j++] = lp - 1 - i;
	for (Rank i = 0;i < lp - 1;i++)
		GS[lp - 1 - SS[i]] = lp - 1 - i;
	delete [] SS;
	return GS;
}
template<class T> Rank BM_GS(T *P, Rank lp, T *T, Rank lt)
{
	Rank *GS = get_GS(P, lp);
	Rank i = lp - 1, j = 0;
	while (i >= 0 && j < lt - lp)
		if (P[i] == T[i + j]) i--;
		else { j += GS[i]; }
	delete [] GS;
	return j;
}
int main()
{
	cout << BM_GS("100", 3, "11111100", 8) << endl;
	while (true);
}