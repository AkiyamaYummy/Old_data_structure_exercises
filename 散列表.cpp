#include<iostream>
#include<string>
using namespace std;
typedef long long Rank;
template<class T>class sh_node
{
public:
	Rank mode;T data;string key;
	sh_node() { mode = 0; }
	void set_node(string k,T v) { data = v;key = k;mode = 1; }
	void del_node() { if (mode)mode = -1; }
};
template<class T>class string_hash
{
	typedef sh_node<T> sh_node;
private:
	static const Rank STEP = 10003;
	sh_node *_head;
	Rank get_code_2(Rank code_old, Rank code_add,string s,bool mode)
	{
		Rank code = code_old;
		code += ((code_add / 2)*(code_add / 2)*(code_add & 1 ? -1 : 1));
		code %= STEP;if (code < 0)code += STEP;
		Rank cmode = _head[code].mode;string key = _head[code].key;
		if ((mode && cmode == 0) || STEP < code_add) throw code_old;
		if (cmode == 0 || (cmode == 1 && key == s) || (cmode == -1 && !mode))
			return code;
		else return get_code_2(code_old, code_add + 1, s, mode);
	}
	Rank get_code(string s,bool mode)//mode=0 ´æÈë mode=1 È¡³ö
	{
		Rank code = 0;
		for (Rank i = 0;i < s.length();i++)
		{
			code = (code << 5) | (code >> 27);
			code += (Rank)s[i];
		}
		return get_code_2(code % STEP, 1, s, mode);
	}
public:
	string_hash() { _head = new sh_node[STEP]; }
	void put(string key, T value) { _head[get_code(key, 0)].set_node(key, value); }
	T get(string key) { return _head[get_code(key, 1)].data; }
	void del(string key) { _head[get_code(key, 1)].del_node(); }
	T operator[](string key) { return get(key); }
};
int main()
{
	string_hash<int>s;
	s.put("shit", 1);
	s.put("dick", 2);
	s.put("cock", 3);
	cout << s["shit"] << endl;
	cout << s["dick"] << endl;
	while (1);
}