/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Tree.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TREE_H_
#define __INC_TREE_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** A single node of a tree.
	*/
	template <typename T>
	struct STreeNode
	{
		T value;
		STreeNode* pChild;
		STreeNode* pParent;
		STreeNode* pNextSibling;
		STreeNode* pPrevSibling;
	};

	/** Generic list-like tree class.
	*/
	template <typename T>
	class Tree
	{
	protected:
		typedef Tree <T> TTree;
		typedef size_type size_t;
		typedef TreeIterator <T> iterator;
		typedef TreeIterator <T> const_iterator;

	public:
		Tree();
		virtual ~Tree();

		void push_front(const TTree& _tree);
		void push_back(const TTree& _tree);
		void pop_front();
		void pop_back();
		void clear();

		iterator insert(iterator _pos, const TTree& _tree);
		iterator erase(iterator& _pos);

		iterator splice(iterator _pos, iterator _first, iterator _last);
		
		size_type size() const;
		bool empty() const;
	}



//----------------------------------------------------------------------
	template <typename T>
	Tree::Tree()
	{
	}
//----------------------------------------------------------------------
	template <typename T>
	Tree::~Tree()
	{
	}
//----------------------------------------------------------------------
	template <typename T>
	void Tree::push_front(const TTree &_tree)
	{
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------



	template <typename T>
	class TreeIterator: public std::iterator<std::bidirectional_iterator_tag, value_type>
	{
	public:
		TreeIterator();

		TTree& operator*();
		TTree* operator->();

		TreeIterator <T>& operator++();
		TreeIterator <T>& operator--();
		const TreeIterator <T>& operator++(int);
		const TreeIterator <T>& operator--(int);

		bool operator==(const TreeIterator& _iter);
		bool operator!=(const TreeIterator& _iter);
	}



//----------------------------------------------------------------------
	template <typename T>
	TreeIterator::TreeIterator()
	{
	}
//----------------------------------------------------------------------
}


#endif