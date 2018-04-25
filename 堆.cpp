#include<iostream>
#include<vector>
#include<time.h>
template<class T> T max(T a, T b) { return a < b ? b : a; }
template<class T> T min(T a, T b) { return a < b ? a : b; }
template<class T> void PRINT(T a) { cout << a << ' '; }
using namespace std;
typedef long long Rank;
template<class T>class heap
{
	Rank left_son(Rank num) { return 2 * num + 1 < _heap.size() ? 2 * num + 1 : -1; }
	Rank right_son(Rank num) { return 2 * num + 2 < _heap.size() ? 2 * num + 2 : -1; }
	Rank parent(Rank num) { return num == 0 ? -1 : (num - 1) / 2; }
	Rank swap_data(Rank num1, Rank num2) { swap(_heap[num1], _heap[num2]);return num2; }
	Rank max_data_son(Rank num)
	{
		if (left_son(num) == right_son(num) == -1)return -1;
		if (left_son(num) == -1)return right_son(num);
		if (right_son(num) == -1)return left_son(num);
		if (_heap[left_son(num)] < _heap[right_son(num)])return right_son(num);
		return left_son(num);
	}
	bool is_ranked(Rank num)
	{
		if (left_son(num) != -1 && _heap[num] < _heap[left_son(num)])return 0;
		if (right_son(num) != -1 && _heap[num] < _heap[right_son(num)])return 0;
		return 1;
	}
protected:
	vector<T> _heap;
	void check_down(Rank num)
	{
		while (!is_ranked(num))
			num = swap_data(num, max_data_son(num));
	}
	void check_up(Rank num)
	{
		while (parent(num) != -1 && !is_ranked(parent(num)))
			num = swap_data(num, parent(num));
	}
	void build_heap(T *A, Rank n)
	{
		for (;n;n--)_heap.push_back(A[n - 1]);
		for (Rank i = _heap.size() / 2 - 1;i >= 0;i--)check_down(i);
	}
public:
	heap(T *A = NULL, Rank n = 0) { if(n)build_heap(A, n); }
	T operator[](Rank num) { return _heap[num]; }
	void insert(T a) { _heap.push_back(a); check_up(_heap.size() - 1); }
	T get_max() { return _heap[0]; }
	T remove_max()
	{
		T rt = get_max();swap_data(0, _heap.size() - 1);
		_heap.pop_back();check_down(0);return rt; 
	}
	bool empty() { return _heap.size() == 0; }
	void clear() { _heap.clear(); }
};
template<class T>class btn
{
public:
	btn *parent, *left_son, *right_son;
	Rank height; T data;
	btn(T a, btn *pr = NULL)
	{
		parent = pr;data = a; height = 0;
		left_son = right_son = NULL;
	}
};
template<class T>class bin_tree
{
public:
	typedef btn<T> btn;typedef void(*FUNC)(T);
protected:
	btn *_root; Rank _size;
	Rank _height(btn *n) { return n ? n->height : -1; }
	virtual void updata_height(btn *n)
	{
		if (n)n->height = 1 +
			max(_height(n->left_son), _height(n->right_son));
	}
	virtual void updata_height_above(btn *n)
	{
		for (;n;n = n->parent)updata_height(n);
	}
public:
	bin_tree() { _root = NULL;_size = 0; }
	void in_order(FUNC visit) { in_order(_root, visit); }
	void in_order(btn *r, FUNC visit)
	{
		if (!r)return; in_order(r->left_son, visit);
		visit(r->data); in_order(r->right_son, visit);
	}
	void pre_order(FUNC visit) { pre_order(_root, visit); }
	void pre_order(btn *r, FUNC visit)
	{
		if (!r)return; visit(r->data);
		pre_order(r->left_son, visit); pre_order(r->right_son, visit);
	}
};
template<class T>class left_heap:public bin_tree<T>
{
protected:
	void updata_height(btn *n)
	{
		if (n)n->height = 1 +
			min(this->_height(n->left_son), this->_height(n->right_son));
	}
public:
	btn *merge(btn *root1, btn *root2)
	{
		if (!root1)return root2;if (!root2)return root1;
		if (root1->data < root2->data)return merge(root2, root1);
		btn *new_right_son = merge(root1->right_son, root2);
		root1->right_son = new_right_son;
		new_right_son->parent = root1;
		this->updata_height(root1);
		if (this->_height(root1->left_son) < this->_height(root1->right_son))
			swap(root1->left_son, root1->right_son);
		return root1;
	}
	void insert(T a) { this->_root = merge(this->_root, new btn(a));this->_size++; }
	T get_max() { return this->_root->data; }
	T remove_max() 
	{ 
		this->_size--;
		T rt = get_max(); btn *t_root = this->_root;
		this->_root = merge(this->_root->left_son, this->_root->right_son);
		delete t_root; return rt;
	}
};
int main()
{
	srand(time(0));
	left_heap<int> h;
	for (int i = 1;i <= 100;i++)h.insert(rand()%100+1);
	h.pre_order(PRINT);cout << '\n';
	for (int i = 0;i < 100;i++)cout << h.remove_max() << ' ';cout << '\n';
	int A[100];
	for (int i = 0;i < 100;i++)A[i] = rand() % 100 + 1;
	heap<int> H(A, 100);
	for (int i = 0;i < 100;i++)cout << H.remove_max() << ' ';
	while (1);
}