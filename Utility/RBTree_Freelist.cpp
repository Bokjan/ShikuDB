#include <algorithm>
#include "DbfsManager.hpp"
class RBTree_Freelist
{
public:
	enum class COLOR
	{
		RED, 
		BLACK
	};
private:
	struct Node 
	{
		int value;
		COLOR color;
		Node *leftTree, *rightTree, *parent;
		Node(void)
		{
			color = COLOR::RED;
			leftTree = nullptr;
			rightTree = nullptr;
			parent = nullptr;
			value = 0;
		}
		Node *Grandparent(void) 
		{
			if(parent == nullptr)
				return nullptr;
			return parent->parent;
		}
		Node *Uncle(void) 
		{
			if(Grandparent() == nullptr) 
				return nullptr;
			if(parent == Grandparent()->rightTree)
				return Grandparent()->leftTree;
			return Grandparent()->rightTree;
		}
		Node *Sibling(void) 
		{
			if(parent->leftTree == this)
				return parent->rightTree;
			return parent->leftTree;
		}
	};
	void RotateRight(Node *p)
	{
		Node *gp = p->Grandparent();
		Node *fa = p->parent;
		Node *y = p->rightTree;
		fa->leftTree = y;
		if(y != NIL)
			y->parent = fa;
		p->rightTree = fa;
		fa->parent = p;
		if(root == fa)
			root = p;
		p->parent = gp;
		if(gp != nullptr)
		{
			if(gp->leftTree == fa)
				gp->leftTree = p;
			else
				gp->rightTree = p;
		}
	}
	void RotateLeft(Node *p)
	{
		if(p->parent == nullptr)
		{
			root = p;
			return;
		}
		Node *gp = p->Grandparent();
		Node *fa = p->parent;
		Node *y = p->leftTree;
		fa->rightTree = y;
		if(y != NIL)
			y->parent = fa;
		p->leftTree = fa;
		fa->parent = p;
		if(root == fa)
			root = p;
		p->parent = gp;
		if(gp != nullptr)
		{
			if(gp->leftTree == fa)
				gp->leftTree = p;
			else
				gp->rightTree = p;
		}
	}
	Node* GetSmallestChild(Node *p){
		if(p->leftTree == NIL)
			return p;
		return GetSmallestChild(p->leftTree);
	}
	bool DeleteChild(Node *p, int data)
	{
		if(p->value > data)
		{
			if(p->leftTree == NIL)
				return false;
			return DeleteChild(p->leftTree, data);
		} 
		else if(p->value < data)
		{
			if(p->rightTree == NIL)
				return false;
			return DeleteChild(p->rightTree, data);
		} 
		else if(p->value == data)
		{
			if(p->rightTree == NIL)
			{
				DeleteOneChild(p);
				return true;
			}
			Node *smallest = GetSmallestChild(p->rightTree);
			std::swap(p->value, smallest->value);
			DeleteOneChild(smallest);
			return true;
		}
		return true;
	}
	void DeleteOneChild(Node *p)
	{
		Node *child = p->leftTree == NIL ? p->rightTree : p->leftTree;
		if(p->parent == nullptr && p->leftTree == NIL && p->rightTree == NIL)
		{
			p = nullptr;
			root = p;
			return;
		}
		if(p->parent == nullptr)
		{
			delete p;
			child->parent = nullptr;
			root = child;
			root->color = COLOR::BLACK;
			return;
		}
		if(p->parent->leftTree == p)
			p->parent->leftTree = child;
		else
			p->parent->rightTree = child;
		child->parent = p->parent;
		if(p->color == COLOR::BLACK)
		{
			if(child->color == COLOR::RED)
				child->color = COLOR::BLACK;
			else
				DeleteCase(child);
		}
		delete p;
	}
	void DeleteCase(Node *p)
	{
		if(p->parent == nullptr)
		{
			p->color = COLOR::BLACK;
			return;
		}
		if(p->Sibling()->color == COLOR::RED) 
		{
			p->parent->color = COLOR::RED;
			p->Sibling()->color = COLOR::BLACK;
			if(p == p->parent->leftTree)
				RotateLeft(p->Sibling());
			else
				RotateRight(p->Sibling());
		}
		if(p->parent->color == COLOR::BLACK && p->Sibling()->color == COLOR::BLACK
			&& p->Sibling()->leftTree->color == COLOR::BLACK && p->Sibling()->rightTree->color == COLOR::BLACK)
		{
			p->Sibling()->color = COLOR::RED;
			DeleteCase(p->parent);
		} 
		else if(p->parent->color == COLOR::RED && p->Sibling()->color == COLOR::BLACK
			&& p->Sibling()->leftTree->color == COLOR::BLACK && p->Sibling()->rightTree->color == COLOR::BLACK) 
		{
			p->Sibling()->color = COLOR::RED;
			p->parent->color = COLOR::BLACK;
		} 
		else 
		{
			if(p->Sibling()->color == COLOR::BLACK) 
			{
				if(p == p->parent->leftTree && p->Sibling()->leftTree->color == COLOR::RED
						&& p->Sibling()->rightTree->color == COLOR::BLACK) 
				{
					p->Sibling()->color = COLOR::RED;
					p->Sibling()->leftTree->color = COLOR::BLACK;
					RotateRight(p->Sibling()->leftTree);
				} 
				else if(p == p->parent->rightTree && p->Sibling()->leftTree->color == COLOR::BLACK
					&& p->Sibling()->rightTree->color == COLOR::RED) 
				{
					p->Sibling()->color = COLOR::RED;
					p->Sibling()->rightTree->color = COLOR::BLACK;
					RotateLeft(p->Sibling()->rightTree);
				}
			}
			p->Sibling()->color = p->parent->color;
			p->parent->color = COLOR::BLACK;
			if(p == p->parent->leftTree)
			{
				p->Sibling()->rightTree->color = COLOR::BLACK;
				RotateLeft(p->Sibling());
			} 
			else 
			{
				p->Sibling()->leftTree->color = COLOR::BLACK;
				RotateRight(p->Sibling());
			}
		}
	}
	void Insert(Node *p, int data)
	{
		if(p->value >= data)
		{
			if(p->leftTree != NIL)
				Insert(p->leftTree, data);
			else 
			{
				Node *tmp = new Node();
				tmp->value = data;
				tmp->leftTree = tmp->rightTree = NIL;
				tmp->parent = p;
				p->leftTree = tmp;
				InsertCase(tmp);
			}
		} 
		else 
		{
			if(p->rightTree != NIL)
				Insert(p->rightTree, data);
			else 
			{
				Node *tmp = new Node();
				tmp->value = data;
				tmp->leftTree = tmp->rightTree = NIL;
				tmp->parent = p;
				p->rightTree = tmp;
				InsertCase(tmp);
			}
		}
	}
	void InsertCase(Node *p)
	{
		if(p->parent == nullptr)
		{
			root = p;
			p->color = COLOR::BLACK;
			return;
		}
		if(p->parent->color == COLOR::RED)
		{
			if(p->Uncle()->color == COLOR::RED) 
			{
				p->parent->color = p->Uncle()->color = COLOR::BLACK;
				p->Grandparent()->color = COLOR::RED;
				InsertCase(p->Grandparent());
			} 
			else 
			{
				if(p->parent->rightTree == p && p->Grandparent()->leftTree == p->parent) 
				{
					RotateLeft (p);
					RotateRight (p);
					p->color = COLOR::BLACK;
					p->leftTree->color = p->rightTree->color = COLOR::RED;
				} 
				else if(p->parent->leftTree == p && p->Grandparent()->rightTree == p->parent) 
				{
					RotateRight (p);
					RotateLeft (p);
					p->color = COLOR::BLACK;
					p->leftTree->color = p->rightTree->color = COLOR::RED;
				} 
				else if(p->parent->leftTree == p && p->Grandparent()->leftTree == p->parent) 
				{
					p->parent->color = COLOR::BLACK;
					p->Grandparent()->color = COLOR::RED;
					RotateRight(p->parent);
				} 
				else if(p->parent->rightTree == p && p->Grandparent()->rightTree == p->parent) 
				{
					p->parent->color = COLOR::BLACK;
					p->Grandparent()->color = COLOR::RED;
					RotateLeft(p->parent);
				}
			}
		}
	}
	void DeleteTree(Node *p)
	{
		if(!p || p == NIL)
			return;
		DeleteTree(p->leftTree);
		DeleteTree(p->rightTree);
		delete p;
	}
public:
	RBTree_Freelist(void) 
	{
		NIL = new Node();
		NIL->color = COLOR::BLACK;
		root = nullptr;
	}
	~RBTree_Freelist(void)
	{
		if(root)
			DeleteTree(root);
		delete NIL;
	}
	void Insert(int x) 
	{
		if(root == nullptr)
		{
			root = new Node();
			root->color = COLOR::BLACK;
			root->leftTree = root->rightTree = NIL;
			root->value = x;
		} 
		else 
			Insert(root, x);
	}
	bool DeleteValue(int data)
	{
		return DeleteChild(root, data);
	}
private:
	Node *root, *NIL;
};