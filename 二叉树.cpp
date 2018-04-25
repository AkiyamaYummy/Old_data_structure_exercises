#include <bits/stdc++.h>
using namespace std;
typedef long long Rank;
template<class T>void PRINT(T a){cout<<a<<' ';}
template<class T>class btn{
public:
    T data; Rank height;
    btn *parent,*left_son,*right_son;
    bool color;
    btn(T a,btn *pr=NULL)
    {
        parent=pr;left_son=right_son=NULL;
        data=a;height=0;
    }
    Rank size()
    {
        return 1
        +(left_son?left_son->size():0)
        +(right_son?right_son->size():0);
    }
};
template<class T>class bin_tree{
    typedef btn<T> btn;
    typedef void(*FUNC)(T);
    Rank _height(btn *n){return n?n->height:-1;}
private:
    void node_clear(btn *n)
    {
        if(!n)return;
        node_clear(n->left_son);
        node_clear(n->right_son);
        delete n; _size--;
    }
protected:
    btn *_root; Rank _size;
    virtual void updata_height(btn *n)
    {
        if(n)n->height=1+
        max(_height(n->left_son),_height(n->right_son));
    }
    virtual void updata_height_above(btn *n)
    {
        Rank t;
        for(;n;n=n->parent)
        {
            t=n->height; updata_height(n);
            if(t==n->height)return;
        }
    }
public:
    bin_tree(){_root=NULL;_size=0;}
    ~bin_tree(){node_clear(_root);}
    btn *root(){return _root;}
    Rank size(){return _size;}
    void add_root(T a)
    {
        node_clear(_root);
        _root=new btn(a); _size=1;
    }
    void add_left_son(T a,btn *n)
    {
        node_clear(n->left_son);n->left_son=new btn(a,n);
        updata_height_above(n);_size++;
    }
    void add_right_son(T a,btn *n)
    {
        node_clear(n->right_son);n->right_son=new btn(a,n);
        updata_height_above(n);_size++;
    }
    void del_root()
    {
        node_clear(_root);_root=NULL;
    }
    void del_left_son(btn *n)
    {
        node_clear(n->left_son);n->left_son=NULL;
        updata_height_above(n);
    }
    void del_right_son(btn *n)
    {
        node_clear(n->right_son);n->right_son=NULL;
        updata_height_above(n);
    }
    void pre_order(FUNC visit){if(_root)pre_order(_root,visit);}
    void pre_order(btn *n,FUNC visit)
    {
        visit(n->data);
        if(n->left_son)pre_order(n->left_son,visit);
        if(n->right_son)pre_order(n->right_son,visit);
    }
    void in_order(FUNC visit){if(_root)in_order(_root,visit);}
    void in_order(btn *n,FUNC visit)
    {
        if(n->left_son)in_order(n->left_son,visit);
        visit(n->data);
        if(n->right_son)in_order(n->right_son,visit);
    }
};
template<class T>class bst:public bin_tree<T>{
    typedef btn<T> btn;
protected:
    bool is_left(btn *n)
    {   return (n->parent?n==n->parent->left_son:0);}
    btn *bro(btn *n)
    {
        if(!n||n==this->_root)return (btn*)NULL;
        if(is_left(n))return n->parent->right_son;
        return n->parent->left_son;
    }
    btn *son(btn *n)
    {
        if(!n)return (btn*)NULL;
        if(n->left_son)return n->left_son;
        return n->right_son;
    }
    btn *&succ(btn *n)
    {
        if(!n->right_son||!n->right_son->left_son)
            return n->right_son;
        for(n=n->right_son;n->left_son->left_son;n=n->left_son);
        return n->left_son;
    }
    btn *&prec(btn *n)
    {
        if(!n->left_son||!n->left_son->right_son)
            return n->left_son;
        for(n=n->left_son;n->right_son->right_son;n=n->right_son);
        return n->right_son;
    }
    btn *&neib(btn *n)
    {
        btn *&ln=prec(n),*&rn=succ(n);
        return ln?ln:rn;
    }
    btn *&_search(T a,btn *&n)
    {
        if(!n||n->data==a)return n;
        btn *&p=(a<n->data?n->left_son:n->right_son);
        _hot=n;
        return _search(a,p);
    }
    btn *_hot;
    void add_as_left(btn *p,btn *ls)
    {   if(p)p->left_son=ls;else this->_root=ls;
        if(ls)ls->parent=p;}
    void add_as_right(btn *p,btn *rs)
    {   if(p)p->right_son=rs;else this->_root=rs;
        if(rs)rs->parent=p;}
    btn *four_and_three(btn *p1,btn *p2,btn *p3,\
                        btn *s1,btn *s2,btn *s3,btn *s4)
    {
        add_as_left(p1,s1);add_as_right(p1,s2);this->updata_height(p1);
        add_as_left(p3,s3);add_as_right(p3,s4);this->updata_height(p3);
        add_as_left(p2,p1);add_as_right(p2,p3);this->updata_height(p2);
        return p2;
    }
    btn *balance_updata(btn *n1,btn *n2,btn *n3)
    {
        bool side=is_left(n1),side1=is_left(n2),side2=is_left(n3);
        btn *pr=n1->parent,*n;
        if( side1&& side2)n=this->four_and_three(n3,n2,n1,\
            n3->left_son,n3->right_son,n2->right_son,n1->right_son);
        if(!side1&&!side2)n=this->four_and_three(n1,n2,n3,\
            n1->left_son,n2->left_son,n3->left_son,n3->right_son);
        if(!side1&& side2)n=this->four_and_three(n1,n3,n2,\
            n1->left_son,n3->left_son,n3->right_son,n2->right_son);
        if( side1&&!side2)n=this->four_and_three(n2,n3,n1,\
            n2->left_son,n3->left_son,n3->right_son,n1->right_son);
        side?(this->add_as_left(pr,n)):(this->add_as_right(pr,n));
        return n;
    }
    btn *balance_renew(btn *n1,btn *n2,btn *n3)
    {
        bool side=is_left(n1);
        btn *pr=n1->parent;
        if(is_left(n3))
        {
            btn *s1=n3->left_son,*s2=n3->right_son;
            btn *s3=n2->right_son,*s4=n1->right_son;
            this->add_as_left(n3,s1);this->add_as_right(n3,n2);
            this->add_as_left(n2,s2);this->add_as_right(n2,n1);
            this->add_as_left(n1,s3);this->add_as_right(n1,s4);
            this->updata_height(n1);
            this->updata_height(n2);
            this->updata_height(n3);
        }
        else
        {
            btn *s1=n1->left_son,*s2=n2->left_son;
            btn *s3=n3->left_son,*s4=n3->right_son;
            this->add_as_left(n1,s1);this->add_as_right(n1,s2);
            this->add_as_left(n2,n1);this->add_as_right(n2,s3);
            this->add_as_left(n3,n2);this->add_as_right(n3,s4);
            this->updata_height(n1);
            this->updata_height(n2);
            this->updata_height(n3);
        }
        side?(this->add_as_left(pr,n3)):(this->add_as_right(pr,n3));
        return n3;
    }
    void zig_or_zag(btn *p,btn *n)
    {
        bool side=is_left(p);
        btn *g=p->parent;
        if(is_left(n))
        {
            btn *s1=n->left_son,*s2=n->right_son;
            btn *s3=p->right_son;
            this->add_as_left(n,s1);this->add_as_right(n,p);
            this->add_as_left(p,s2);this->add_as_right(p,s3);
            this->updata_height(p);
            this->updata_height(n);
        }
        else
        {
            btn *s1=p->left_son,*s2=n->left_son;
            btn *s3=n->right_son;
            this->add_as_left(p,s1);this->add_as_right(p,s2);
            this->add_as_left(n,p);this->add_as_right(n,s3);
            this->updata_height(p);
            this->updata_height(n);
        }
        side?(this->add_as_left(g,n)):(this->add_as_right(g,n));
    }
public:
    virtual btn *&search(T a)
    {
        _hot=NULL;
        return _search(a,this->_root);
    }
    virtual void insert(T a)
    {
        btn *&p=search(a);
        if(p)return;
        p=new btn(a,_hot);
        this->_size++;
        this->updata_height_above(this->_hot);
    }
    virtual void erase(T a)
    {
        btn *p=search(a);
        if(!p)return;
        if(p->left_son&&p->right_son)
        {
            btn *q=succ(p);swap(p->data,q->data);
            p=q;
        }
        this->_hot=p->parent;
        btn *son=(p->left_son?p->left_son:p->right_son);
        if(is_left(p))add_as_left(this->_hot,son);
        else add_as_right(this->_hot,son);
        this->_size--;
        this->updata_height_above(this->_hot);
        delete p;
    }
};
template<class T>class AVL:public bst<T>{
    typedef btn<T> btn;
    Rank _height(btn *n){return n?n->height:-1;}
    bool is_balance(btn *n)
    {
        if(!n)return 1;
        Rank a=_height(n->left_son)-_height(n->right_son);
        return -2<a&&a<2;
    }
public:
    void insert(T a)
    {
        btn *&p=this->search(a),*ghot;
        if(p)return;
        p=new btn(a,this->_hot);
        this->_size++;
        for(btn *P=p;P;P=P->parent)
        {
            if(!is_balance(P))
            {
                this->balance_updata(P,this->_hot,ghot);
                this->updata_height_above(P);
                break;
            }
            else this->updata_height(P);
            ghot=this->_hot; this->_hot=P;
        }
    }
    void erase(T a)
    {
        btn *p=this->search(a);
        if(!p)return;
        if(p->left_son&&p->right_son)
        {
            btn *q=this->succ(p);swap(p->data,q->data);
            p=q;
        }
        this->_hot=this->bro(p);
        btn *s=(p->left_son?p->left_son:p->right_son);
        if(this->is_left(p))this->add_as_left(p->parent,s);
        else this->add_as_right(p->parent,s);
        this->_size--;
        Rank t;
        for(btn *P=p->parent;P;P=P->parent)
        {
            t=P->height;
            if(!is_balance(P))
                P=this->balance_updata(P,this->_hot,this->son(this->_hot));
            else this->updata_height(P);
            this->_hot=this->bro(P);
            if(t==P->height)break;
        }
        delete p;
    }
};
template<class T>class splay_tree:public bst<T>{
    typedef btn<T> btn;
private:
    void splay(btn *n)
    {
        btn *p,*g; bool side1,side2;
        while(n&&(p=n->parent)&&(g=p->parent))
        {
            side1=this->is_left(n);side2=this->is_left(p);
            if(side1==side2)this->balance_renew(g,p,n);
            else this->balance_updata(g,p,n);
        }
        if(n!=this->_root)this->zig_or_zag(n->parent,n);
        this->_root=n;
    }
public:
    btn *&search(T a)
    {
        btn *p=this->_root;
        for(this->_hot=NULL;p&&p->data!=a;)
        {   this->_hot=p;
            p=(a<p->data?p->left_son:p->right_son);
        }
        if(p)splay(p);
        else if(this->_hot)splay(this->_hot);
        return this->_root;
    }
    void insert(T a)
    {
        btn *p=search(a);
        if(p&&p->data==a)return;
        this->_size++;
        if(!this->_hot){this->_root=new btn(a);return;}
        bool side=(this->_root->data<a);
        btn *s1=this->_root,*s2=this->_root->left_son;
        btn *s3=this->_root->right_son;
        (side?this->_root->right_son:this->_root->left_son)=NULL;
        this->_root=new btn(a);
        this->add_as_left(this->_root,side?s1:s2);
        this->add_as_right(this->_root,side?s3:s1);
        this->updata_height(this->_root);
    }
    void erase(T a)
    {
        btn *p=search(a);
        if(!p||p->data!=a)return;
        btn *pn=this->neib(p);
        if(!pn)this->_root=NULL;
        else
        {   splay(pn);
            bool side=this->is_left(p);
            if(side)this->add_as_left(this->_root,p->left_son);
            else this->add_as_right(this->_root,p->right_son);
        }
        delete p;
        this->updata_height(this->_root);
        this->_size--;
    }
};
template<class T>class red_black_tree:public bst<T>{
    typedef btn<T> btn;
    Rank _height(btn *n){return n?n->height:-1;}
    bool _color(btn *n){return n?n->color:1;}
    void red_set(btn *n){if(n)n->color=0;}
    void black_set(btn *n){if(n)n->color=1;}
    btn *red_son(btn *n)
    {   if(n){if(!_color(n->left_son))return n->left_son;
        else if(!_color(n->right_son))return n->right_son;}
        return (btn*)NULL;}
private:
    void double_red(btn *n)
    {
        if(n==this->_root){black_set(n);return;}
        if(_color(n->parent))return;
        if(_color(this->bro(n->parent)))
        {
            btn *g=this->balance_updata(n->parent->parent,n->parent,n);
            black_set(g);
            red_set(g->left_son);red_set(g->right_son);
        }
        else
        {
            red_set(n->parent->parent);
            black_set(n->parent);black_set(this->bro(n->parent));
            double_red(n->parent->parent);
        }
    }
    void double_black(btn *n)
    {
        btn *p=n->parent,*b=this->bro(n);
        if(red_son(b)){bool cl=p->color;
            btn *g=this->balance_updata(p,b,red_son(b));
            g->color=cl; black_set(n->parent);}
        else if(!_color(p)){black_set(p);red_set(b);}
        else if(!_color(b)){this->zig_or_zag(p,b);
            black_set(b);red_set(p);double_black(n);}
        else {red_set(b);double_black(p);}
    }
public:
    void updata_height(btn *n)
    {
        if(n){n->height=
        max(_height(n->left_son),_height(n->right_son));
        if(n->color)++n->height;}
    }
    void insert(T a)
    {
        btn *&p=this->search(a);
        if(p)return;
        p=new btn(a,this->_hot); red_set(p);
        this->_size++;
        this->updata_height_above(this->_hot);
        double_red(p);
    }
    void erase(T a)
    {
        btn *p=this->search(a);
        if(!p)return;
        this->_size--;
        if(p->left_son&&p->right_son)
        {   btn *q=this->succ(p);swap(p->data,q->data);
            p=q;}
        btn *n=(p->left_son?p->left_son:p->right_son);
        btn *pr=p,*gr=p?p->parent:NULL;
        bool key=(_color(n)||_color(pr));
        if(this->is_left(p))this->add_as_left(gr,n);
        else this->add_as_right(gr,n);
        delete p; black_set(n);
        if(key){if(!n)n=gr;if(!n)return;
            double_black(n);}
    }
};
int main()
{
    srand(time(0));
    int a;
    /*
    AVL<int> a;
    for(int i=1;i<=63;i++)a.insert(i);
    a.in_order(PRINT);cout<<'\n';
    for(int i=16;i<=63;i++)
        a.erase(i);
    a.in_order(PRINT);cout<<'\n';
    splay_tree<int> s;
    for(int i=0;i<100;i++)
    {a=rand()%100;s.insert(a);}
    cout<<"after ins\n";
    s.in_order(PRINT);cout<<'\n';
    s.pre_order(PRINT);cout<<"\n\n";
    for(int i=0;i<100;i++)
    {a=rand()%100;s.erase(a);}
    cout<<"after era\n";
    s.in_order(PRINT);cout<<'\n';
    s.pre_order(PRINT);cout<<"\n\n";
    */

    red_black_tree<int> r;
    for(int i=0;i<31;i++)
    {a=rand()%100;r.insert(a);}
    cout<<"after ins\n";
    r.in_order(PRINT);cout<<'\n';
    r.pre_order(PRINT);cout<<"\n\n";
    for(int i=0;i<31;i++)
    {a=rand()%100;r.erase(a);}
    cout<<"after era\n";
    r.in_order(PRINT);cout<<'\n';
    r.pre_order(PRINT);cout<<"\n\n";

    return 0;
}
