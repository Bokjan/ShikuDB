#include <iostream>
 
template <class Te>
class SBTnode
{
private:
    template <class T>
    friend class SBTree;
    SBTnode<Te> *p, *left, *right;
    Te key;    
	unsigned int Size;
	unsigned int account;
public:
    SBTnode(Te _key)
	{
        key =_key;
        Size = 1;
        account = 1;
        left = NULL;
        right = NULL;
    };
};//节点模板 
 
template <class T>
class SBTree
{
	private:
		inline void LeftRotate(SBTnode<T> * x);//左旋函数 
		inline void RightRotate(SBTnode<T> * x);//右旋函数 
		inline SBTnode<T>* DeleteSuccessor(SBTnode<T> * x);
		inline void Empty(SBTnode<T> * &x);//清空指定子树		
		inline bool Insert(T & k , SBTnode<T> * &z );//插入函数
		inline SBTnode<T>* Search(SBTnode<T>* x , T & k); //中序检索函数 
		SBTnode<T> *Root , * Nil;//根节点、哨兵节点 
		unsigned int NUM;
	public:
		SBTree()
		{
			Nil = new SBTnode<T>(0);
			Nil->Size = 0;
			Nil->account = 0;
			Nil->left = Nil;
			Nil->right = Nil;
			NUM = 0;
			Root = Nil;
		};//构造函数 
 
		~SBTree()
		{
			delete Nil;
			Empty();
		};//析构函数 
 
		inline void Insert(T  k);//插入函数 
		inline void Delete(T  k);//删除函数，若找不到所输入元素则返回 
		inline void Maintain(SBTnode<T> * x , bool IsRight);//主调整函数 
		inline SBTnode<T>* Search(T & k);//中序检索函数 		
		inline SBTnode<T>* Successor(SBTnode<T> * x);//后继 
		inline SBTnode<T>* Predecessor(SBTnode<T> * x);//前趋 
		inline SBTnode<T>* Minimum(SBTnode<T> * x);//最小值 
		inline SBTnode<T>* Maximum(SBTnode<T> * x);//最大值 
		inline void  Print(SBTnode<T> * &x);// 升序打印指定子树 
		inline void  Print();// 升序打印整棵树 
		inline void  Empty();//清空整棵树 
		inline unsigned int Num(); //整棵树元素个数 
};
 
 
template <class T>
void SBTree<T>::LeftRotate(SBTnode<T> * x)
{
	SBTnode<T> *y = x->right;
	x->right = y->left;
	if(y->left != Nil)
	{
		y->left->p =x;
	};
	y->p = x->p;
	if(x->p == Nil)
	{
		Root = y;
	}else{	
	    if(x == x->p->left)
	    {
	    	x->p->left = y;
	    }else{
	    	x->p->right = y;
	    };
	};
	x->p = y;
	y->left = x;
	y->Size = x->Size;
	x->Size = x->left->Size + x->right->Size + 1;//Size的维护 
}
 
template <class T>
void SBTree<T>::RightRotate(SBTnode<T> * x)
{
	SBTnode<T> *y = x->left;
	x->left = y->right;
	if(y->right != Nil)
	{
		y->right->p =x;
	};
	y->p = x->p;
	if(x->p == Nil)
	{
		Root = y;
	}else{	
	    if(x == x->p->right)
	    {
	    	x->p->right = y;
	    }else{
	    	x->p->left = y;
	    };
	};
	x->p = y;
	y->right = x;
	y->Size = x->Size;
	x->Size = x->left->Size + x->right->Size + 1;//Size的维护 
}
 
template <class T>
void SBTree<T>::Insert(T  k) 
{
	SBTnode<T> *x = Root, *y = Nil;
	while(x != Nil)
	{		
		if(k == x->key)
	    {
		    x->account++;
		    NUM++;
		    x = x->p;
		    while(x != Nil)
		    {
		    	x->Size--;
		    	x = x->p; 
		    };
		    return;
	    };//若有相同元素则计数加1，恢复先祖的Size 
	    x->Size++;
		y = x;
		if(k < x->key)
		{
			x = x->left;
		}else{
			x = x->right;
		};
	};
	SBTnode<T> * z = new SBTnode<T>(k);
	z->p = y;
	if(y == Nil)
	{
		Root = z;
	}else{
		if(k < y->key)
		{
			y->left = z;
		}else{
			y->right = z;
		};
	};
	z->left = Nil;
	z->right = Nil;
	while(y != Nil)
	{
		Maintain(y , k >= y->key);
		y = y->p;
	};
}
 
template <class T>
void SBTree<T>::Delete(T  k)
{
	SBTnode<T> *z = Root, *w = Nil;
	while(k != z->key && z != Nil)
	{
		z->Size--;
		w = z;
		if(k < z->key)
		{
			z = z->left;
		}else{
			z = z->right;
		};
	};
	if(z == Nil)
	{
		while(w != Nil)
		{
			w->Size++;
			w = w->p;
		};//若没找到，则恢复先祖的Size 
	}else{
		if(z->account > 1)
		{
			z->account--;
			NUM--;
			while(w != Nil)
		    {
			    w->Size++;
			    w = w->p;
		    };
			return;
		};//若计数大于1，则计数减1，并恢复先祖的Size 
		SBTnode<T> *y, *x;
	    if(z->left == Nil || z-> right == Nil)
        {
	        y = z;
        }else{
	        y = DeleteSuccessor(z);
        };
        if(y->left != Nil)
        {
	        x = y->left;
        }else{
	        x = y->right;
        };
        x->p = y->p;
        if(y->p == Nil)
        {
	        Root = x;
        }else{
            if(y == y->p->left)
            {
    	        y->p->left = x;
            }else{
    	        y->p->right = x;
            };
        };
        if(y != z)
        {
	        z->key = y->key;
	        z->account = y->account;
        };
        delete y;
	};
 
}
 
template <class T>
SBTnode<T>* SBTree<T>::DeleteSuccessor(SBTnode<T> * x)
{
	x->Size--;
	x = x->right;
	while(x->left != Nil)
	{
		x->Size--;
		x = x->left;
	};
	return x;
}//删除中使用的后继函数，一边搜索，一边减小Size 
 
 
template <class T>
void SBTree<T>::Maintain(SBTnode<T> * x , bool IsRight)
{
	if(IsRight)
	{
		if(x->right->right->Size > x->left->Size)
		{
		    LeftRotate(x);
		}else{
			if(x->right->left->Size > x->left->Size)
			{
				RightRotate(x->right);
				LeftRotate(x);
			}else{
				return;
			};
		};
	}else{
		if(x->left->left->Size > x->right->Size)
		{
		    RightRotate(x);
		}else{
			if(x->left->right->Size > x->right->Size)
			{
				LeftRotate(x->left);
				RightRotate(x);
			}else{
				return;
			};
		};
	};
	Maintain(x->p->left,false);
	Maintain(x->p->right,true);
	Maintain(x->p,true);
	Maintain(x->p,false);
}
 
template <class T>
SBTnode<T>* SBTree<T>::Successor(SBTnode<T> * x)
{
	if(x->right != Nil)
	    return Minimum(x->right);
	SBTnode<T> *y = x->p;
	while(y != Nil && x == y->right)
	{
		x = y;
		y = y->p;
	};
	return y;
}
 
 
template <class T>
SBTnode<T>* SBTree<T>::Predecessor(SBTnode<T> * x)
{
	if(x->right != Nil)
	    return Minimum(x->right);
	SBTnode<T> *y = x->p;
	while(y != Nil && x == y->right)
	{
		x = y;
		y = y->p;
	};
	return y;
}
 
template <class T>
SBTnode<T>* SBTree<T>::Minimum(SBTnode<T> * x)
{
	while(x->left != Nil)
	{
		x = x->left;
	};
	return x;
}
 
template <class T>
SBTnode<T>* SBTree<T>::Maximum(SBTnode<T> * x)
{
	while(x->right != Nil)
	{
		x = x->right;
	};
	return x;
}
 
template <class T>
SBTnode<T>* SBTree<T>::Search(T & k)
{
	return Search(Root , k);
}
 
template <class T>
SBTnode<T>* SBTree<T>::Search(SBTnode<T>* x , T & k)
{
	while(x != Nil && k != x->key)
	{
		if(k < x->key)
		{
			x = x->left;
		}else{
			x = x->right;
		};
	};
	return x;
}
 
template <class T>
void SBTree<T>::Print(SBTnode<T> * &x)
{
	if(x->left != Nil)
	    Print(x->left);
	for(int i = 0 ; i < x->account ; i++)
	{
	    std::cout<<x->key<<std::endl;
	};
	if(x->right != Nil)
	    Print(x->right);
}
 
template <class T>
void SBTree<T>::Print()
{
	if(Root == Nil)
	{
		std::cout<<"Empty tree!"<<std::endl;
		return;
	};
	Print(Root);
}
 
 
template <class T>
void SBTree<T>::Empty()
{
	if(Root != Nil)
	{
	    Empty(Root);
	    Root = Nil;
	    NUM = 0;
	};
}
 
template <class T>
void SBTree<T>::Empty(SBTnode<T> * &x)
{
	if(x->left != Nil)
	    Empty(x->left);
	if(x->right != Nil)
	    Empty(x->right);
	delete x;
}
 
template <class T>
unsigned int SBTree<T>::Num()
{
	return (Root->Size + NUM);
}
// inline void Insert(T  k);//插入函数 
// inline void Delete(T  k);//删除函数，若找不到所输入元素则返回 
// inline void Maintain(SBTnode<T> * x , bool IsRight);//主调整函数 
// inline SBTnode<T>* Search(T & k);//中序检索函数 		
// inline SBTnode<T>* Successor(SBTnode<T> * x);//后继 
// inline SBTnode<T>* Predecessor(SBTnode<T> * x);//前趋 
// inline SBTnode<T>* Minimum(SBTnode<T> * x);//最小值 
// inline SBTnode<T>* Maximum(SBTnode<T> * x);//最大值 
// inline void  Print(SBTnode<T> * &x);// 升序打印指定子树 
// inline void  Print();// 升序打印整棵树 
// inline void  Empty();//清空整棵树 
// inline unsigned int Num(); //整棵树元素个数 
int main(void)
{
	SBTree<int> sbt;
	for(int i = 1; i < 10; ++i)
		sbt.Insert(i), sbt.Insert(i);
	sbt.Print();
	return 0;
}