/*  
 * Copyright (c) 2015 ArmyAnt
 * 版权所有 (c) 2015 ArmyAnt
 *
 * Licensed under the BSD License, Version 2.0 (the License); 
 * 本软件使用BSD协议保护, 协议版本:2.0
 * you may not use this file except in compliance with the License. 
 * 使用本开源代码文件的内容, 视为同意协议
 * You can read the license content in the file "ARMYANT.COPYRIGHT.BSD_LICENSE.MD" at the root of this project
 * 您可以在本项目的根目录找到名为"ARMYANT.COPYRIGHT.BSD_LICENSE.MD"的文件, 来阅读协议内容
 * You may also obtain a copy of the License at 
 * 您也可以在此处获得协议的副本:
 * 
 *     http://opensource.org/licenses/BSD-3-Clause
 * 
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an AS IS BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * 除非法律要求或者版权所有者书面同意,本软件在本协议基础上的发布没有任何形式的条件和担保,无论明示的或默许的.
 * See the License for the specific language governing permissions and limitations under the License. 
 * 请在特定限制或语言管理权限下阅读协议
 */

#ifndef A_A_TREE_HPP_2015_12_26
#define A_A_TREE_HPP_2015_12_26

/*	* @ author			: Jason
	* @ date			: 12/26/2015
	* @ nearly update	: 01/18/2016
	* @ small version	: 0.2
	* @ summary			: 树及其遍历迭代器
	* @ uncompleted		: 
	* @ untested		: all
	* @ tested			:
	*/

#include "AADefine.h"
#include <functional>
#include <vector>

namespace  ArmyAnt {

typedef std::function<void(void*node, void*param)> BrowseFunc;

template <class T_Val, class T_Tag = uint32>
class TreeNode;
template <class T_Val, class T_Tag = uint32>
class Tree;
template <class T_Val, class T_Tag = uint32>
class Iterator_Tree;

template <class T_Val, class T_Tag>
class TreeNode
{
public:
	//根据键值对创建节点
	TreeNode(T_Val&element, T_Tag tag);
	//复制节点,不会复制父节点关系
	TreeNode(const TreeNode<T_Val, T_Tag>&value, bool withChildren = false);
	//拷贝节点内容,不会复制父节点关系,但会复制全部子节点
	TreeNode<T_Val, T_Tag>&operator=(const TreeNode<T_Val, T_Tag>&value);
	//析构函数
	virtual ~TreeNode();

public:
	//取得节点的键
	T_Tag GetTag()const;
	//取得节点的值内容
	T_Val*GetValue();
	inline const T_Val*GetValue()const;
	//取得节点所在的父节点
	TreeNode<T_Val, T_Tag>*GetParent();
	inline const TreeNode<T_Val, T_Tag>*GetParent()const;
	//取得节点的指定键对应的子节点
	TreeNode<T_Val, T_Tag>*GetChild(T_Tag tag);
	inline const TreeNode<T_Val, T_Tag>*GetChild(T_Tag tag)const;
	//取得第一个子节点
	TreeNode<T_Val, T_Tag>*GetFirstChild();
	inline const TreeNode<T_Val, T_Tag>* GetFirstChild()const;
	//取得最后一个子节点
	TreeNode<T_Val, T_Tag>*GetLastChild();
	inline const TreeNode<T_Val, T_Tag>* GetLastChild()const;
	//取得位于某个节点之后的子节点
	TreeNode<T_Val, T_Tag>*GetNextChild(T_Tag tag);
	inline const TreeNode<T_Val, T_Tag>*GetNextChild(T_Tag tag)const;
	//取得位于某个节点之前的子节点
	TreeNode<T_Val, T_Tag>*GetBackChild(T_Tag tag);
	inline const TreeNode<T_Val, T_Tag>*GetBackChild(T_Tag tag)const;
	//取得所有子节点,返回子节点个数
	T_Tag GetChildren(TreeNode<T_Val, T_Tag>*children);

public:
	//取得所在的树
	Tree<T_Val, T_Tag> GetTree();
	inline const Tree<T_Val, T_Tag> GetTree()const;
	//修改键
	bool SetTag(T_Tag tag);
	//修改值内容
	bool SetValue(T_Val&value);
	//重置值内容
	bool SetValue();
	//从父节点和父树中移除
	bool RemoveFromParent();
	//添加到某个节点之下(如果原先有父节点,则会先从父节点上移除)
	bool SetParent(TreeNode<T_Val, T_Tag>*parent);

public:
	//添加一个子节点(如果被添加的节点原先有父节点,则会先从父节点上移除)
	bool AddChild(const TreeNode<T_Val, T_Tag>*child);
	//以指定的内容,新建一个子节点
	bool AddChild(T_Tag tag, T_Val value);
	//移除指定键对应的子节点
	bool RemoveChild(T_Tag tag);
	//移除指定子节点
	bool RemoveChild(const TreeNode<T_Val, T_Tag>*child);
	//移除所有子节点
	bool ClearChildren();

public:
	//清空节点值内容
	inline T_Val* operator=(std::nullptr_t);
	//获取指定键处的节点
	inline TreeNode<T_Val, T_Tag>* operator[](T_Tag tag);
	inline const TreeNode<T_Val, T_Tag>* operator[](T_Tag tag)const;

private:
	//键
	T_Tag tag;
	//值
	T_Val* value = nullptr;
	//父节点
	TreeNode<T_Val, T_Tag>* parent = nullptr;
	//子节点数组
	std::vector<TreeNode<T_Val, T_Tag>*> children;
};

template <class T_Val, class T_Tag>
class Tree
{
public:
	Tree();
	Tree(TreeNode<T_Val, T_Tag>* root);
	virtual ~Tree();

public:
	//取得根节点
	TreeNode<T_Val, T_Tag>*GetRoot();
	inline const TreeNode<T_Val, T_Tag>*GetRoot()const;
	//取得根节点的键
	inline T_Tag GetTag()const;
	//取得树中具有指定键的所有子节点,返回取得的总个数
	int GetChild(T_Tag tag, TreeNode<T_Val, T_Tag>**rets);
	//取得第一个搜寻到的具有指定键的子节点,搜寻方式为逐层遍历
	TreeNode<T_Val, T_Tag>*GetFirstChild(T_Tag tag);
	inline const TreeNode<T_Val, T_Tag>*GetFirstChild(T_Tag tag)const;
	//获取子树,仅搜寻根节点的子节点
	Tree<T_Val, T_Tag>*GetChildTree(T_Tag tag);
	inline const Tree<T_Val, T_Tag>*GetChildTree(T_Tag tag)const;
	//获取树的父节点,如果没有,返回nullptr
	inline TreeNode<T_Val, T_Tag>*GetParent();
	inline const TreeNode<T_Val, T_Tag>*GetParent()const;
	//获取父树,如果没有,返回nullptr
	Tree<T_Val, T_Tag>*GetParentTree();
	inline const Tree<T_Val, T_Tag>*GetParentTree()const;

public:
	// Recursion traversal, 可递归的遍历算法
	// 先序遍历（深度优先遍历）
	Iterator_Tree<T_Val, T_Tag> GetPreorderTraversal(T_Tag first);
	inline const Iterator_Tree<T_Val, T_Tag> GetPreorderTraversal(T_Tag first)const;
	// 后序遍历
	Iterator_Tree<T_Val, T_Tag> GetPostorderTraversal(T_Tag first);
	inline const Iterator_Tree<T_Val, T_Tag> GetPostorderTraversal(T_Tag first)const;

	// Un-recursion-able traversal, 不可递归的遍历算法
	// 逐层遍历（广度优先遍历）
	Iterator_Tree<T_Val, T_Tag> GetLayerorderTraversal(T_Tag first);
	inline const Iterator_Tree<T_Val, T_Tag> GetLayerorderTraversal(T_Tag first)const;
	// 随机遍历
	Iterator_Tree<T_Val, T_Tag> GetRandomTraversal();
	inline const Iterator_Tree<T_Val, T_Tag> GetRandomTraversal()const;

public:
	//获取一个空的迭代器,也就是末尾
	inline const Iterator_Tree<T_Val, T_Tag> End() const;

private:
	//根节点
	TreeNode<T_Val, T_Tag>* root = nullptr;

	friend class Iterator_Tree<T_Val, T_Tag>;

	AA_FORBID_ASSGN_OPR(Tree);
	AA_FORBID_COPY_CTOR(Tree);
};

//遍历方式
enum class TraversalType : uint8
{
	Unknown,
	PreorderTraversal,	//先序遍历（深度优先遍历）
	InorderTraversal,	//中序遍历
	PostorderTraversal,	//后序遍历	
	LayerorderTraversal,//逐层遍历（广度优先遍历）
	RandomTraversal		//随机遍历
};

template <class T_Val, class T_Tag>
class Iterator_Tree
{
public:
	//按树和遍历方式创建迭代器,非二叉树不能使用中序遍历
	Iterator_Tree(TraversalType type, const Tree<T_Val, T_Tag>&tree);
	//复制迭代器
	Iterator_Tree(const Iterator_Tree<T_Val, T_Tag>&value);
	//复制迭代器的值
	Iterator_Tree&operator=(const Iterator_Tree<T_Val, T_Tag>&value);
	//析构函数
	virtual ~Iterator_Tree();

public:
	//迭代器按照规定的遍历方式,跳转到下一个
	inline bool Next();
	//迭代器跳转到上一个
	inline bool Back();
	//判断迭代器是否在开头(随机遍历永远返回false)
	bool IsBegin();
	//判断迭代器是否在结尾
	bool IsEnd();

public:
	//获取迭代器所属的树
	Tree<T_Val, T_Tag>* GetTree();
	const Tree<T_Val, T_Tag>* GetTree()const;
	//获取迭代器指向的节点
	TreeNode<T_Val, T_Tag>* GetNode();
	const TreeNode<T_Val, T_Tag>* GetNode()const;
	//获取迭代器的遍历方式
	TraversalType GetTraversalType()const;
	//重设迭代器的遍历方式
	bool SetTraversalType(TraversalType type);
	//从树中清除迭代器所指的节点,并将迭代器指向该节点的上一个目标
	bool Erase();

public:
	//检验迭代器是否为末尾
	inline bool operator==(std::nullptr_t);
	inline bool operator!=(std::nullptr_t);
	inline operator bool();
	inline bool operator!();
	inline TreeNode<T_Val, T_Tag>& operator*();
	inline const TreeNode<T_Val, T_Tag>& operator*()const;
	inline const TreeNode<T_Val, T_Tag>* operator->()const;

private:
	bool Go(bool isNext);

private:
	TraversalType type = TraversalType::Unknown;
	const Tree<T_Val, T_Tag>*tree = nullptr;
	TreeNode<T_Val, T_Tag>* nownode = nullptr;
};


/******************************* Source : TreeNode ***********************************************************/


template <class T_Val, class T_Tag /*= uint32*/>
TreeNode<T_Val, T_Tag>::TreeNode(T_Val&element, T_Tag tag)
	:tag(tag), value(&element), parent(nullptr), children()
{
}

template <class T_Val, class T_Tag /*= uint32*/>
TreeNode<T_Val, T_Tag>::TreeNode(const TreeNode<T_Val, T_Tag>&value, bool withChildren/* = false*/)
	: tag(value.tag), value(&value.value), parent(nullptr), children()
{
	this->children.clear();
	if(withChildren)
		for(auto i = value.children.begin(); i != value.children.end(); i++)
		{
			children.push_back(*i);
		}
}

template <class T_Val, class T_Tag /*= uint32*/>
TreeNode<T_Val, T_Tag>& TreeNode<T_Val, T_Tag>::operator=(const TreeNode<T_Val, T_Tag>&value)
{
	this->tag = value.tag;
	this->value = value.value;
	this->children.clear();
	for(auto i = value.children.begin(); i != value.children.end(); i++)
	{
		children.push_back(*i);
	}
}

template <class T_Val, class T_Tag /*= uint32*/>
TreeNode<T_Val, T_Tag>::~TreeNode()
{
}

template<class T_Val, class T_Tag>
T_Tag TreeNode<T_Val, T_Tag>::GetTag() const
{
	return tag;
}

template<class T_Val, class T_Tag>
T_Val * TreeNode<T_Val, T_Tag>::GetValue()
{
	return value;
}

template<class T_Val, class T_Tag>
const T_Val * TreeNode<T_Val, T_Tag>::GetValue() const
{
	return value;
}

template<class T_Val, class T_Tag>
TreeNode<T_Val, T_Tag> * TreeNode<T_Val, T_Tag>::GetParent()
{
	return parent;
}

template<class T_Val, class T_Tag>
const TreeNode<T_Val, T_Tag> * TreeNode<T_Val, T_Tag>::GetParent() const
{
	return parent;
}

template<class T_Val, class T_Tag>
TreeNode<T_Val, T_Tag> * TreeNode<T_Val, T_Tag>::GetChild(T_Tag tag)
{
	for(auto i = children.begin(); i != children.end(); i++)
	{
		if(i->tag == tag)
			return &*i;
	}
	return nullptr;
}

template<class T_Val, class T_Tag>
const TreeNode<T_Val, T_Tag> * TreeNode<T_Val, T_Tag>::GetChild(T_Tag tag) const
{
	return const_cast<TreeNode<T_Val, T_Tag> *>(this)->GetChild(tag);
}

template<class T_Val, class T_Tag>
TreeNode<T_Val, T_Tag> * TreeNode<T_Val, T_Tag>::GetFirstChild()
{
	if(children.empty())
		return nullptr;
	return children[0];
}

template<class T_Val, class T_Tag>
const TreeNode<T_Val, T_Tag> * TreeNode<T_Val, T_Tag>::GetFirstChild() const
{
	if(children.empty())
		return nullptr;
	return children[0];
}

template<class T_Val, class T_Tag>
TreeNode<T_Val, T_Tag>* TreeNode<T_Val, T_Tag>::GetLastChild()
{
	if(children.empty())
		return nullptr;
	return children[children.size()-1];
}

template<class T_Val, class T_Tag>
inline const TreeNode<T_Val, T_Tag>* TreeNode<T_Val, T_Tag>::GetLastChild() const
{
	if(children.empty())
		return nullptr;
	return children[children.size() - 1];
}

template<class T_Val, class T_Tag>
TreeNode<T_Val, T_Tag> * TreeNode<T_Val, T_Tag>::GetNextChild(T_Tag tag)
{
	if(children.empty())
		return nullptr;
	for(auto i = children.begin(); i != children.end(); i++)
	{
		if(i->tag == tag)
			if(++i == children.end())
				return GetFirstChild();
			else
				return *i;
	}
	return nullptr;
}

template<class T_Val, class T_Tag>
const TreeNode<T_Val, T_Tag> * TreeNode<T_Val, T_Tag>::GetNextChild(T_Tag tag) const
{
	return const_cast<TreeNode<T_Val, T_Tag> *>(this)->GetNextChild(tag);
}

template<class T_Val, class T_Tag>
TreeNode<T_Val, T_Tag> * TreeNode<T_Val, T_Tag>::GetBackChild(T_Tag tag)
{
	if(children.empty())
		return nullptr;
	for(auto i = children.begin(); i != children.end(); i++)
	{
		if(i->tag == tag)
			if(i == children.begin())
				return children.back();
			else
				return --i;
	}
	return nullptr;
}

template<class T_Val, class T_Tag>
const TreeNode<T_Val, T_Tag> * TreeNode<T_Val, T_Tag>::GetBackChild(T_Tag tag) const
{
	return const_cast<TreeNode<T_Val, T_Tag> *>(this)->GetBackChild(tag);
}

template<class T_Val, class T_Tag>
T_Tag TreeNode<T_Val, T_Tag>::GetChildren(TreeNode<T_Val, T_Tag> * children)
{
	for(auto i = this.children.begin(); i != this->children.end(); i++)
	{
		children[i] = *i;
	}
	return this->children.size();
}

template<class T_Val, class T_Tag>
Tree<T_Val, T_Tag> TreeNode<T_Val, T_Tag>::GetTree()
{
	return Tree<T_Tag>(this);
}

template<class T_Val, class T_Tag>
const Tree<T_Val, T_Tag> TreeNode<T_Val, T_Tag>::GetTree() const
{
	return const_cast<TreeNode<T_Val, T_Tag> *>(this)->GetTree();
}

template<class T_Val, class T_Tag>
bool TreeNode<T_Val, T_Tag>::SetTag(T_Tag tag)
{
	this->tag = tag;
	return true;
}

template<class T_Val, class T_Tag>
bool TreeNode<T_Val, T_Tag>::SetValue(T_Val & value)
{
	this->value = &value;
	return true;
}

template<class T_Val, class T_Tag>
bool TreeNode<T_Val, T_Tag>::SetValue()
{
	this->value = nullptr;
	return true;
}

template<class T_Val, class T_Tag>
bool TreeNode<T_Val, T_Tag>::RemoveFromParent()
{
	return this->parent->RemoveChild(this);
}

template<class T_Val, class T_Tag>
bool TreeNode<T_Val, T_Tag>::SetParent(TreeNode<T_Val, T_Tag> * parent)
{
	return parent->AddChild(this);
}

template<class T_Val, class T_Tag>
bool TreeNode<T_Val, T_Tag>::AddChild(const TreeNode<T_Val, T_Tag> * child)
{
	if(child->parent != nullptr && !child->RemoveFromParent())
		return false;
	children.push_back(child);
	child->parent = this;
	return true;
}

template<class T_Val, class T_Tag>
bool TreeNode<T_Val, T_Tag>::AddChild(T_Tag tag, T_Val value)
{
	children.push_back(TreeNode(tag, value));
	return true;
}

template<class T_Val, class T_Tag>
bool TreeNode<T_Val, T_Tag>::RemoveChild(T_Tag tag)
{
	for(auto i = this.children.begin(); i != this->children.end();)
	{
		if(i->tag == tag)
		{
			i->parent = nullptr;
			i = children.erase(i);
			return true;
		}
		else
			i++;
	}
	return false;
}

template<class T_Val, class T_Tag>
bool TreeNode<T_Val, T_Tag>::RemoveChild(const TreeNode<T_Val, T_Tag> * child)
{
	for(auto i = this.children.begin(); i != this->children.end();)
	{
		if(&*i == child)
		{
			i->parent = nullptr;
			i = children.erase(i);
			return true;
		}
		else
			i++;
	}
	return false;
}

template<class T_Val, class T_Tag>
bool TreeNode<T_Val, T_Tag>::ClearChildren()
{
	for(auto i = this.children.begin(); i != this->children.end(); i++)
	{
		i->parent = nullptr;
	}
	children.clear();
	return true;
}

template<class T_Val, class T_Tag>
T_Val * TreeNode<T_Val, T_Tag>::operator=(std::nullptr_t)
{
	value = nullptr;
	return nullptr;
}

template<class T_Val, class T_Tag>
TreeNode<T_Val, T_Tag> * TreeNode<T_Val, T_Tag>::operator[](T_Tag tag)
{
	return GetChild(tag);
}

template<class T_Val, class T_Tag>
const TreeNode<T_Val, T_Tag> * TreeNode<T_Val, T_Tag>::operator[](T_Tag tag) const
{
	return GetChild(tag);
}


/******************************* Source : Tree ***************************************************************/


template<class T_Val, class T_Tag>
Tree<T_Val, T_Tag>::Tree()
	:root(nullptr)
{
}

template<class T_Val, class T_Tag>
Tree<T_Val, T_Tag>::Tree(TreeNode<T_Val, T_Tag> * root)
	: root(root)
{
}

template<class T_Val, class T_Tag>
Tree<T_Val, T_Tag>::~Tree()
{
}

template<class T_Val, class T_Tag>
TreeNode<T_Val, T_Tag> * Tree<T_Val, T_Tag>::GetRoot()
{
	return root;
}

template<class T_Val, class T_Tag>
const TreeNode<T_Val, T_Tag> * Tree<T_Val, T_Tag>::GetRoot() const
{
	return root;
}

template<class T_Val, class T_Tag>
T_Tag Tree<T_Val, T_Tag>::GetTag() const
{
	return root->tag;
}

template<class T_Val, class T_Tag>
int Tree<T_Val, T_Tag>::GetChild(T_Tag tag, TreeNode<T_Val, T_Tag>**rets)
{
	if(root == nullptr)
		return 0;
	auto ret = std::vector<TreeNode<T_Val, T_Tag>*>();
	for(auto i = GetLayerorderTraversal(root->tag); i != End(); i++)
	{
		if(i->tag == tag)
			ret.push_back(&*i);
	}
	for(int i = 0; i < ret.size(); i++)
		rets[i] = ret[i];
	return ret.size();
}

template<class T_Val, class T_Tag>
TreeNode<T_Val, T_Tag> * Tree<T_Val, T_Tag>::GetFirstChild(T_Tag tag)
{
	for(auto i = GetLayerorderTraversal(root->tag); i != End(); i++)
	{
		if(i->tag == tag)
			return &*i;
	}
	return nullptr;
}

template<class T_Val, class T_Tag>
const TreeNode<T_Val, T_Tag> * Tree<T_Val, T_Tag>::GetFirstChild(T_Tag tag) const
{
	return const_cast<Tree<T_Val, T_Tag> *>(this)->GetFirstChild(tag);
}

template<class T_Val, class T_Tag>
Tree<T_Val, T_Tag> * Tree<T_Val, T_Tag>::GetChildTree(T_Tag tag)
{
	if(root->GetChild(tag) != nullptr)
		return Tree(root->GetChild(tag));
	return nullptr;
}

template<class T_Val, class T_Tag>
const Tree<T_Val, T_Tag> * Tree<T_Val, T_Tag>::GetChildTree(T_Tag tag) const
{
	return const_cast<Tree<T_Val, T_Tag> *>(this)->GetChildTree(tag);
}

template<class T_Val, class T_Tag>
TreeNode<T_Val, T_Tag> * Tree<T_Val, T_Tag>::GetParent()
{
	return root->GetParent();
}

template<class T_Val, class T_Tag>
const TreeNode<T_Val, T_Tag> * Tree<T_Val, T_Tag>::GetParent() const
{
	return root->GetParent();
}

template<class T_Val, class T_Tag>
Tree<T_Val, T_Tag> * Tree<T_Val, T_Tag>::GetParentTree()
{
	if(root->GetParent() != nullptr)
		return Tree(root->GetParent());
	return nullptr;
}

template<class T_Val, class T_Tag>
const Tree<T_Val, T_Tag> * Tree<T_Val, T_Tag>::GetParentTree() const
{
	if(root->GetParent() != nullptr)
		return Tree(root->GetParent());
	return nullptr;
}

template<class T_Val, class T_Tag>
Iterator_Tree<T_Val, T_Tag> Tree<T_Val, T_Tag>::GetPreorderTraversal(T_Tag first)
{
	// TODO: solve the param "first"
	return Iterator_Tree<T_Val, T_Tag>(TraversalType::PreorderTraversal, *this);
}

template<class T_Val, class T_Tag>
const Iterator_Tree<T_Val, T_Tag> Tree<T_Val, T_Tag>::GetPreorderTraversal(T_Tag first) const
{
	return Iterator_Tree<T_Val, T_Tag>(TraversalType::PreorderTraversal, *this);
}

template<class T_Val, class T_Tag>
Iterator_Tree<T_Val, T_Tag> Tree<T_Val, T_Tag>::GetPostorderTraversal(T_Tag first)
{
	return Iterator_Tree<T_Val, T_Tag>(TraversalType::PostorderTraversal, *this);
}

template<class T_Val, class T_Tag>
const Iterator_Tree<T_Val, T_Tag> Tree<T_Val, T_Tag>::GetPostorderTraversal(T_Tag first) const
{
	return Iterator_Tree<T_Val, T_Tag>(TraversalType::PostorderTraversal, *this);
}

template<class T_Val, class T_Tag>
Iterator_Tree<T_Val, T_Tag> Tree<T_Val, T_Tag>::GetLayerorderTraversal(T_Tag first)
{
	return Iterator_Tree<T_Val, T_Tag>(TraversalType::LayerorderTraversal, *this);
}

template<class T_Val, class T_Tag>
const Iterator_Tree<T_Val, T_Tag> Tree<T_Val, T_Tag>::GetLayerorderTraversal(T_Tag first) const
{
	return Iterator_Tree<T_Val, T_Tag>(TraversalType::LayerorderTraversal, *this);
}

template<class T_Val, class T_Tag>
Iterator_Tree<T_Val, T_Tag> Tree<T_Val, T_Tag>::GetRandomTraversal()
{
	return Iterator_Tree<T_Val, T_Tag>(TraversalType::RandomTraversal, *this);
}

template<class T_Val, class T_Tag>
const Iterator_Tree<T_Val, T_Tag> Tree<T_Val, T_Tag>::GetRandomTraversal() const
{
	return Iterator_Tree<T_Val, T_Tag>(TraversalType::RandomTraversal, *this);
}

template<class T_Val, class T_Tag>
const Iterator_Tree<T_Val, T_Tag> Tree<T_Val, T_Tag>::End() const
{
	return Iterator_Tree<T_Val, T_Tag>(TraversalType::Unknown, *this);
}


/******************************* Source : Iterator_Tree ******************************************************/


template<class T_Val, class T_Tag>
Iterator_Tree<T_Val, T_Tag>::Iterator_Tree(TraversalType type, const Tree <T_Val, T_Tag>&tree)
	:tree(&tree), nownode(tree.root)
{
	if(type == TraversalType::InorderTraversal || type == TraversalType::PostorderTraversal)
		while(nownode->GetFirstChild() != nullptr)
			nownode = nownode->GetFirstChild();
}

template<class T_Val, class T_Tag>
Iterator_Tree<T_Val, T_Tag>::Iterator_Tree(const Iterator_Tree<T_Val, T_Tag> & value)
	:tree(value.tree),nownode(value.nownode)
{
}

template<class T_Val, class T_Tag>
Iterator_Tree<T_Val, T_Tag> & Iterator_Tree<T_Val, T_Tag>::operator=(const Iterator_Tree<T_Val, T_Tag> & value)
{
	tree = value.tree;
	nownode = value.nownode;
}

template<class T_Val, class T_Tag>
Iterator_Tree<T_Val, T_Tag>::~Iterator_Tree()
{
}

template<class T_Val, class T_Tag>
bool Iterator_Tree<T_Val, T_Tag>::Next()
{
	return Go(true);
}

template<class T_Val, class T_Tag>
bool Iterator_Tree<T_Val, T_Tag>::Back()
{
	return Go(false);
}

template<class T_Val, class T_Tag>
bool Iterator_Tree<T_Val, T_Tag>::IsBegin()
{
	switch(type)
	{
		case TraversalType::PreorderTraversal:
		case TraversalType::LayerorderTraversal:
			return nownode == tree->GetRoot();
		case TraversalType::InorderTraversal:
		case TraversalType::PostorderTraversal:
			{
				auto tmp = nownode;
				if(nownode->GetFirstChild() != nullptr)
					return false;
				while(nownode->GetParent() != tree->GetRoot())
					nownode = nownode->GetParent();
				return nownode == tree->GetRoot()->GetFirstChild();
			}
		case TraversalType::RandomTraversal:
		case TraversalType::Unknown:
			return false;
		default:
			AAAssert(type <= TraversalType::RandomTraversal, false);
			return false;
	}
}

template<class T_Val, class T_Tag>
bool Iterator_Tree<T_Val, T_Tag>::IsEnd()
{
	switch(type)
	{
		case TraversalType::PreorderTraversal:
		case TraversalType::InorderTraversal:
		case TraversalType::LayerorderTraversal:
		case TraversalType::PostorderTraversal:
			return nownode == nullptr;
		case TraversalType::RandomTraversal:
		case TraversalType::Unknown:
			return false;
		default:
			AAAssert(type <= TraversalType::RandomTraversal, false);
			return false;
	}
}

template<class T_Val, class T_Tag>
Tree<T_Val, T_Tag>* Iterator_Tree<T_Val, T_Tag>::GetTree()
{
	return tree;
}

template<class T_Val, class T_Tag>
const Tree<T_Val, T_Tag>* Iterator_Tree<T_Val, T_Tag>::GetTree() const
{
	return tree;
}

template<class T_Val, class T_Tag>
TreeNode<T_Val, T_Tag>* Iterator_Tree<T_Val, T_Tag>::GetNode()
{
	return nownode;
}

template<class T_Val, class T_Tag>
const TreeNode<T_Val, T_Tag>* Iterator_Tree<T_Val, T_Tag>::GetNode() const
{
	return nownode;
}

template<class T_Val, class T_Tag>
TraversalType Iterator_Tree<T_Val, T_Tag>::GetTraversalType() const
{
	return type;
}

template<class T_Val, class T_Tag>
bool Iterator_Tree<T_Val, T_Tag>::SetTraversalType(TraversalType type)
{
	if(type > TraversalType::RandomTraversal)
		return false;
	this->type = type;
	return true;

}

template<class T_Val, class T_Tag>
bool Iterator_Tree<T_Val, T_Tag>::Erase()
{
	if(tree == nullptr || nownode == nullptr)
		return false;
	if(nownode == tree->GetRoot() && nownode->GetParent() == nullptr)
		return tree->GetRoot()->ClearChildren() && tree->GetRoot()->SetValue();
	return nownode->RemoveFromParent();
}

template<class T_Val, class T_Tag>
bool Iterator_Tree<T_Val, T_Tag>::operator==(std::nullptr_t)
{
	return IsEnd();
}

template<class T_Val, class T_Tag>
bool Iterator_Tree<T_Val, T_Tag>::operator!=(std::nullptr_t)
{
	return !operator==(nullptr);
}

template<class T_Val, class T_Tag>
Iterator_Tree<T_Val, T_Tag>::operator bool()
{
	return operator==(nullptr);
}

template<class T_Val, class T_Tag>
bool Iterator_Tree<T_Val, T_Tag>::operator!()
{
	return operator!=(nullptr);
}

template<class T_Val, class T_Tag>
TreeNode<T_Val, T_Tag>& Iterator_Tree<T_Val, T_Tag>::operator*()
{
	return nownode;
}

template<class T_Val, class T_Tag>
const TreeNode<T_Val, T_Tag>& Iterator_Tree<T_Val, T_Tag>::operator*() const
{
	return nownode;
}

template<class T_Val, class T_Tag>
const TreeNode<T_Val, T_Tag>* Iterator_Tree<T_Val, T_Tag>::operator->() const
{
	return nownode;
}

template<class T_Val, class T_Tag>
bool Iterator_Tree<T_Val, T_Tag>::Go(bool isNext)
{
	if(tree == nullptr || nownode == nullptr)
		return false;
	switch(type)
	{
		case TraversalType::PreorderTraversal: // 先序遍历,深度优先遍历
			if(isNext)
			{
				if(nownode->GetFirstChild() != nullptr)
					nownode = nownode->GetFirstChild();
				else if(nownode != tree->root)
				{
					while(nownode->GetParent()->GetNextChild(nownode->tag) == nullptr)
					{
						if(nownode == tree->root)
						{
							nownode = nullptr;
							break;
						}
						nownode = nownode->GetParent();
					}
					nownode = nownode->GetParent()->GetNextChild(nownode->tag);
				}
				else
					nownode = nullptr;
			}
			else
			{
				if(nownode == tree->root)
					return false;
				if(nownode->GetParent()->GetBackChild(nownode->GetTag()) != nullptr)
				{
					nownode = nownode->GetParent()->GetBackChild(nownode->GetTag());
					while(nownode->GetLastChild() != nullptr)
						nownode = nownode->GetLastChild();
				}
				else
					nownode = nownode->GetParent();
			}
			break;
		case TraversalType::PostorderTraversal: // 后序遍历,逆向遍历
			if(isNext)
			{
				if(nownode == tree->root)
					nownode = nullptr;
				else if(nownode->GetParent()->GetNextChild(nownode->tag) != nullptr)
				{
					nownode = nownode->GetParent()->GetNextChild(nownode->tag);
					while(nownode->GetFirstChild() != nullptr)
						nownode = nownode->GetFirstChild();
				}
				else
					nownode = nownode->GetParent();
			}
			else
			{
				if(nownode->GetLastChild() != nullptr)
					nownode = nownode->GetLastChild();
				else if(nownode != tree->root)
				{
					while(nownode->GetParent()->GetBackChild(nownode->tag) == nullptr)
					{
						if(nownode == tree->root)
						{
							return false;
							break;
						}
						nownode = nownode->GetParent();
					}
					nownode = nownode->GetParent()->GetBackChild(nownode->tag);
				}
				else
					return false;
			}
			break;
		case TraversalType::LayerorderTraversal: // 逐层遍历,广度优先遍历
			{
				//先查出所有同层元素
				int depth = 0;
				auto ptr = nownode;
				while(ptr != tree->root)
				{
					depth++;
					ptr = ptr->GetParent();
				}
				const TreeNode<T_Val, T_Tag>* firstC = tree->root;
				std::vector<TreeNode<T_Val, T_Tag>*> coll;
				int nowdep = 0;
				while(firstC->GetFirstChild() != nullptr)
				{
					firstC = firstC->GetFirstChild();
					nowdep++;
				}
				while(firstC != tree->root)
				{
					if(nowdep == depth)
						coll.push_back(firstC);
					if(firstC->GetParent()->GetNextChild(firstC->tag) != nullptr)
					{
						firstC = firstC->GetParent()->GetNextChild(firstC->tag);
						while(firstC->GetFirstChild() != nullptr)
						{
							firstC = firstC->GetFirstChild();
								nowdep++;
						}
					}
					else
					{
						firstC = firstC->GetParent();
						nowdep--;
					}
				}
				for(int i = 1; i < coll.size() - 1; i++)
				{
					if(nownode == coll[i])
					{
						nownode = coll[isNext ? (i + 1) : (i - 1)];
						return true;
					}
				}
				AAAssert(nownode != coll[coll.size() - 1] && nownode != coll[0], false);
				nownode = nullptr;
				if(isNext)
				{
					if(nownode == coll[0])
						nownode = coll[0];
					else for(int i = 0; i < coll.size(); i++)
					{
						if(coll[i]->GetFirstChild() != nullptr)
						{
							nownode = coll[i]->GetFirstChild();
							break;
						}
					}
				}
				else
				{
					if(nownode == coll[coll.size() - 1])
						nownode = coll[coll.size() - 1];
					else if(nownode == tree->GetRoot())
						return false;
					else nownode = nownode->GetParent();
				}
			}
			break;
		case TraversalType::RandomTraversal: // 随机遍历,机会均等,与树形无关
			{
				const TreeNode<T_Val, T_Tag>* firstC = tree->root;
				std::vector<TreeNode<T_Val, T_Tag>*> coll;
				while(firstC->GetFirstChild() != nullptr)
				{
					firstC = firstC->GetFirstChild();
				}
				while(firstC != tree->root)
				{
					coll.push_back(firstC);
					if(firstC->GetParent()->GetNextChild(firstC->tag) != nullptr)
					{
						firstC = firstC->GetParent()->GetNextChild(firstC->tag);
						while(firstC->GetFirstChild() != nullptr)
							firstC = firstC->GetFirstChild();
					}
					else
						firstC = firstC->GetParent();
				}
				srand(time(nullptr));
				auto ret = rand() % coll.size();
				nownode = coll[ret];
			}
			break;
		case TraversalType::Unknown:
			nownode = nullptr;
			break;
		default:
			return false;
	}
	return true;
}

} // ArmyAnt

#endif
