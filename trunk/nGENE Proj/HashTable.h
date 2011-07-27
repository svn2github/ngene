/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		HashTable.h
Version:	0.54
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_HASHTABLE_H_
#define __INC_HASHTABLE_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** Default hashing function used by a hash table.
		@par
			In case you want to use other hashing method than provided,
			you just need to code appropriate class and pass it as one
			of arguments while creating new hash table.
		@remarks
			Unfortunately function used does not work correctly for strings
			and wstrings so partial specialization is provided later on.
	*/
	template <typename T>
	class DefaultHashMethod
	{
	protected:
		/// How many buckets are being used
		int m_nNumBuckets;

	public:
		DefaultHashMethod(int numBuckets=200);

		/** The core function of the class doing real job
			@param
				key a key of the element
		*/
		int hash(const T& key) const;
		/** Simply returns number of buckets used
		*/
		int getNumBuckets() const{return m_nNumBuckets;};
	};

	template <typename T>
	DefaultHashMethod<T>::DefaultHashMethod(int numBuckets): m_nNumBuckets(numBuckets)
	{
	}
//----------------------------------------------------------------------
	template <typename T>
	int DefaultHashMethod<T>::hash(const T& key) const
	{
		int nBytes=sizeof(key);
		unsigned long lRes=0;

		for(int i=0; i<nBytes; ++i)
			lRes+=*((char*)(&key)+i);
		return (lRes%m_nNumBuckets);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------


	/** Specialization of DefaulHashMethod class for string.
	*/
	template <>
	class DefaultHashMethod <std::string>
	{
		protected:
			/// How many buckets are being used
			int m_nNumBuckets;

		public:
			inline DefaultHashMethod(int numBuckets=200);

			/** The core function of the class doing real job
				@param
					key a key of the element
			*/
			inline int hash(const std::string& key) const;
			/** Simply returns number of buckets used
			*/
			int getNumBuckets() const{return m_nNumBuckets;};
	};

	DefaultHashMethod<std::string>::DefaultHashMethod(int numBuckets): m_nNumBuckets(numBuckets)
	{
	}
//----------------------------------------------------------------------
	int DefaultHashMethod<std::string>::hash(const std::string& key) const
	{
		int sum=0;
		for(size_t i=0; i<key.size(); ++i)
			sum+=key[i];

		return (sum%m_nNumBuckets);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------


	/** Specialization of DefaulHashMethod class for wstring.
	*/
	template <>
	class DefaultHashMethod <std::wstring>
	{
		protected:
			/// How many buckets are being used
			int m_nNumBuckets;

		public:
			inline DefaultHashMethod(int numBuckets=100);

			/** The core function of the class doing real job
				@param
					key a key of the element
			*/
			inline int hash(const std::wstring& key) const;
			/** Simply returns number of buckets used
			*/
			int getNumBuckets() const{return m_nNumBuckets;};
	};

	DefaultHashMethod<std::wstring>::DefaultHashMethod(int numBuckets): m_nNumBuckets(numBuckets)
	{
	}
//----------------------------------------------------------------------
	int DefaultHashMethod<std::wstring>::hash(const std::wstring& key) const
	{
		int sum=0;
		for(size_t i=0; i<key.size(); ++i)
			sum+=key[i];

		return (sum%m_nNumBuckets);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------


	/** Template hash table implementation.
		@remarks
			This hash table implementation simply extends STL. Hash table
			is implemented as an associative container. You should use it
			whenever you need key-value access as it is faster than	regular
			map from STL.
	*/
	template <typename Key, typename T, typename Compare=std::equal_to<Key>,
		typename Hash=DefaultHashMethod <Key> >
	class HashTable
	{
	public:
		friend class HashIterator <Key, T, Compare, Hash>;

		typedef Key key_type;
		typedef Compare key_compare;
		typedef T mapped_type;
		typedef pair <const Key, T> value_type;
		typedef pair <const Key, T>& reference;
		typedef const pair <const Key, T>& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef HashIterator <Key, T, Compare, Hash> iterator;
		typedef HashIterator <Key, T, Compare, Hash> const_iterator;

		class value_compare: public std::binary_function <value_type, value_type, bool>
		{
		protected:
			Compare m_Comp;
			value_compare(Compare c): m_Comp(c){}

		public:
			friend class HashTable <Key, T, Compare, Hash>;

			bool operator() (const value_type& x, const value_type& y)
			{
				return m_Comp(x.first, y.first);
			}
		};

	public:
		explicit HashTable(const Compare& compare=Compare(), const Hash& hash=Hash());
		HashTable(const HashTable<Key, T, Compare, Hash>& src);
		virtual ~HashTable();

		template <class InputIterator>
		HashTable(InputIterator first, InputIterator last, const Compare& compare=Compare(),
			const Hash& hash=Hash());

		pair <iterator, bool> insert(const value_type& value);
		iterator insert(iterator position, const value_type& value);
		template <class InputIterator> void insert(InputIterator first, InputIterator last);

		size_type erase(const key_type& key);
		void erase(iterator position);
		void erase(iterator first, iterator last);

		void swap(HashTable <Key, T, Compare, Hash>& hashIn);
		void clear();

		key_compare key_comp() const;
		value_compare value_comp() const;

		bool empty() const;
		size_type size() const;
		size_type max_size() const;

		iterator find(const key_type& key);
		const_iterator find(const key_type& key) const;
		size_type count(const key_type& key) const;

		T& operator[] (const key_type& key);

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;

	protected:
		typedef list <value_type> list_type;
		vector <list_type>* m_Elements;

		int m_nSize;
		Compare m_Compare;
		Hash m_Hash;


		typename list_type::iterator findElement(const key_type& key, int& bucket) const;
	};



	template <typename Key, typename T, typename Compare, typename Hash>
	template <class InputIterator>
	HashTable<Key, T, Compare, Hash>::HashTable(InputIterator first, InputIterator last, 
		const Compare& compare, const Hash& hash)
	{
		m_Elements=new vector <list <value_type> >(m_pHashTable.getNumBuckets());
		insert(first, last);
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	HashTable<Key, T, Compare, Hash>::HashTable(const Compare& compare=Compare(),
		const Hash& hash=Hash()): m_nSize(0), m_Compare(compare), m_Hash(hash)
	{
		m_Elements=new vector <list <value_type> >(m_Hash.getNumBuckets());
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	HashTable<Key, T, Compare, Hash>::HashTable(const HashTable<Key, T, Compare, Hash>& src):
		m_nSize(src.m_nSize), m_Compare(src.m_Compare), m_Hash(src.m_Hash)
	{
		m_Elements=new vector<list <value_type> >(*(src.m_Elements));
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	HashTable<Key, T, Compare, Hash>::~HashTable()
	{
		delete m_Elements;
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	bool HashTable<Key, T, Compare, Hash>::empty() const
	{
		return (m_nSize==0);
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	typename HashTable<Key, T, Compare, Hash>::size_type
	HashTable<Key, T, Compare, Hash>::size() const
	{
		return (m_nSize);
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	typename HashTable<Key, T, Compare, Hash>::size_type
	HashTable<Key, T, Compare, Hash>::max_size() const
	{
		return ((*m_Elements[0]).max_size());
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	void typename HashTable<Key, T, Compare, Hash>::swap(HashTable <Key, T, Compare, Hash>& hashIn)
	{
		swap(*this, hashIn);
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	typename list <pair <const Key, T> >::iterator
	HashTable<Key, T, Compare, Hash>::findElement(const key_type& key, int& bucket) const
	{
		bucket=m_Hash.hash(key);
		for(typename list_type::iterator it=(*m_Elements)[bucket].begin();
			it!=(*m_Elements)[bucket].end(); ++it)
		{
			if(m_Compare(it->first, key))
				return (it);
		}

		return ((*m_Elements)[bucket].end());
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	typename HashTable <Key, T, Compare, Hash>::iterator
	HashTable<Key, T, Compare, Hash>::find(const key_type& key)
	{
		int bucket;
		typename list_type::iterator it=findElement(key, bucket);
		if(it==(*m_Elements)[bucket].end())
			return end();
		
		return (HashIterator <Key, T, Compare, Hash>(bucket, it, this));
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	typename HashTable <Key, T, Compare, Hash>::const_iterator
	HashTable<Key, T, Compare, Hash>::find(const key_type& key) const
	{
		int bucket;
		typename list_type::iterator it=findElement(key, bucket);
		if(it==(*m_Elements)[bucket].end())
			return end();
		
		return (HashIterator <Key, T, Compare, Hash>(bucket, it, this));
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	T& HashTable<Key, T, Compare, Hash>::operator[] (const key_type& key)
	{
		return (((insert(make_pair(key, T()))).first)->second);
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	typename HashTable <Key, T, Compare, Hash>::size_type
	HashTable <Key, T, Compare, Hash>::count(const key_type& key) const
	{
		if(find(key) == end())
			return (0);
		else
			return (1);
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	pair <typename HashTable <Key, T, Compare, Hash>::iterator, bool>
	HashTable<Key, T, Compare, Hash>::insert(const value_type& type)
	{
		int bucket;
		typename list_type::iterator it=findElement(type.first, bucket);

		if(it!=(*m_Elements)[bucket].end())
		{
			HashIterator <Key, T, Compare, Hash> newIt(bucket, it, this);
			pair <HashIterator <Key, T, Compare, Hash>, bool> p(newIt, false);
			return (p);
		}
		else
		{
			++m_nSize;
			typename list_type::iterator endIt=(*m_Elements)[bucket].insert((*m_Elements)[bucket].end(),
				type);
			pair <HashIterator <Key, T, Compare, Hash>, bool> p(HashIterator <Key, T, Compare,
				Hash>(bucket, endIt, this), true);
			return (p);
		}
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	typename HashTable <Key, T, Compare, Hash>::iterator HashTable<Key, T, Compare, Hash>::insert
		(typename HashTable <Key, T, Compare, Hash>::iterator position, const value_type& value)
	{
		return (insert(value).first);
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	template <class InputIterator>
	void HashTable <Key, T, Compare, Hash>::insert(InputIterator first, InputIterator last)
	{
		insert_iterator <HashTable <Key, T, Compare, Hash> > inserter(*this, begin());
		copy(first, last, inserter);
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	typename HashTable <Key, T, Compare, Hash>::size_type
	HashTable <Key, T, Compare, Hash>::erase(const key_type& key)
	{
		int bucket;

		typename list_type::iterator it=findElement(key, bucket);

		if(it!=(*m_Elements)[bucket].end())
		{
			(*m_Elements)[bucket].erase(it);
			--m_nSize;
			return 1;
		}
		else
			return 0;
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	void HashTable <Key, T, Compare, Hash>::erase(typename HashTable <Key, T, Compare, Hash>::iterator position)
	{
		(*m_Elements)[position.m_nBucket].erase(position.m_Iter);
		--m_nSize;
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	void HashTable <Key, T, Compare, Hash>::erase(typename HashTable <Key, T, Compare, Hash>::iterator first,
		typename HashTable <Key, T, Compare, Hash>::iterator last)
	{
		typename HashTable <Key, T, Compare, Hash>::iterator cur, next;
		
		for(next=first; next!=last;)
		{
			cur=next++;
			erase(cur);
		}
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	void HashTable <Key, T, Compare, Hash>::clear()
	{
		for_each(m_Elements->begin(), m_Elements->end(), mem_fun_ref(&list_type::clear));
		m_nSize=0;
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	typename HashTable <Key, T, Compare, Hash>::key_compare
	HashTable <Key, T, Compare, Hash>::key_comp() const
	{
		return (m_Compare);
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	typename HashTable <Key, T, Compare, Hash>::value_compare
	HashTable <Key, T, Compare, Hash>::value_comp() const
	{
		return (value_compare(m_Compare));
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	typename HashTable <Key, T, Compare, Hash>::iterator
	HashTable<Key, T, Compare, Hash>::begin()
	{
		// If there is no elements return end() iterator
		if(!m_nSize)
			return end();
		
		for(size_t i=0; i<m_Elements->size(); ++i)
		{
			if(!((*m_Elements)[i].empty()))
				return (HashIterator <Key, T, Compare, Hash>(i, (*m_Elements)[i].begin(), this));
		}

		return end();
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	typename HashTable <Key, T, Compare, Hash>::iterator
	HashTable<Key, T, Compare, Hash>::end()
	{
		return (HashIterator <Key, T, Compare, Hash>(m_Elements->size()-1,
			(*m_Elements)[m_Elements->size()-1].end(), this));
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	typename HashTable <Key, T, Compare, Hash>::const_iterator
	HashTable<Key, T, Compare, Hash>::begin() const
	{
		// If there is no elements return end() iterator
		if(!m_nSize)
			return end();
		
		for(size_t i=0; i<m_Elements->size(); ++i)
		{
			if(!((*m_Elements)[i].empty()))
				return (HashIterator <Key, T, Compare, Hash>(i, (*m_Elements)[i].begin(), this));
		}

		return end();
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	typename HashTable <Key, T, Compare, Hash>::const_iterator
	HashTable<Key, T, Compare, Hash>::end() const
	{
		return (HashIterator <Key, T, Compare, Hash>(m_Elements->size()-1,
			(*m_Elements)[m_Elements->size()-1].end(), this));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------


	/** Let you iterate through hash table elements
	*/
	template <typename Key, typename T, typename Compare, typename Hash>
	class HashIterator: public std::iterator <std::bidirectional_iterator_tag, pair <const Key, T> >
	{
		friend class HashTable <Key, T, Compare, Hash>;

	protected:
		int m_nBucket;
		typename list <pair <const Key, T> >::iterator m_Iter;
		const HashTable <Key, T, Compare, Hash>* m_pHashTable;

		void increment();
		void decrement();

	public:
		HashIterator();
		HashIterator(int bucket, typename list <pair <const Key, T> >::iterator listIt,
					 const HashTable <Key, T, Compare, Hash>* inHashTable);

		pair <const Key, T>& operator*() const;
		pair <const Key, T>* operator->() const;
		HashIterator <Key, T, Compare, Hash>& operator++();
		const HashIterator <Key, T, Compare, Hash> operator++(int);
		HashIterator <Key, T, Compare, Hash>& operator--();
		const HashIterator <Key, T, Compare, Hash> operator--(int);

		bool operator==(const HashIterator& rhs) const;
		bool operator!=(const HashIterator& rhs) const;
	};

	
	template <typename Key, typename T, typename Compare, typename Hash>
	HashIterator <Key, T, Compare, Hash>::HashIterator()
	{
		m_nBucket=-1;
		m_Iter=list <pair <const Key, T> >::iterator();
		m_pHashTable=NULL;
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	HashIterator <Key, T, Compare, Hash>::HashIterator(int bucket, 
		typename list<pair<const Key,T> >::iterator listIt,
		const HashTable<Key,T,Compare,Hash>* inHashTable): m_nBucket(bucket),
			m_Iter(listIt), m_pHashTable(inHashTable)
	{
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	pair <const Key, T>& HashIterator <Key, T, Compare, Hash>::operator*() const
	{
		return (*m_Iter);
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	pair <const Key, T>* HashIterator <Key, T, Compare, Hash>::operator->() const
	{
		return (&(*m_Iter));
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	HashIterator <Key, T, Compare, Hash>& HashIterator <Key, T, Compare, Hash>::operator++()
	{
		increment();
		return (*this);
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	const HashIterator <Key, T, Compare, Hash>
		HashIterator <Key, T, Compare, Hash>::operator ++(int)
	{
		HashIterator <Key, T, Compare, Hash> oldIter=*this;
		increment();
		return (oldIter);
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	HashIterator <Key, T, Compare, Hash>& HashIterator <Key, T, Compare, Hash>::operator--()
	{
		decrement();
		return (*this);
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	const HashIterator <Key, T, Compare, Hash>
		HashIterator <Key, T, Compare, Hash>::operator --(int)
	{
		HashIterator <Key, T, Compare, Hash> newIter=*this;
		decrement();
		return (newIter);
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	void HashIterator <Key, T, Compare, Hash>::increment()
	{
		++m_Iter;

		if(m_Iter==(*m_pHashTable->m_Elements)[m_nBucket].end())
		{
			for(uint i=m_nBucket+1; i<(*m_pHashTable->m_Elements).size(); ++i)
			{
				if(!((*m_pHashTable->m_Elements)[i].empty()))
				{
					m_Iter=(*m_pHashTable->m_Elements)[i].begin();
					m_nBucket=i;
					return;
				}
			}

			m_nBucket=(*m_pHashTable->m_Elements).size()-1;
			m_Iter=(*m_pHashTable->m_Elements)[m_nBucket].end();
		}
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	void HashIterator <Key, T, Compare, Hash>::decrement()
	{
		if(m_Iter==(*m_pHashTable->m_Elements)[m_nBucket].begin())
		{
			for(int i=m_nBucket-1; i>=0; --i)
			{
				if(!((*m_pHashTable->m_Elements)[i].empty()))
				{
					m_Iter=(*m_pHashTable->m_Elements)[i].end();
					--m_Iter;
					m_nBucket=i;
					return;
				}
			}

			m_Iter=(*m_pHashTable->m_nElements)[0].begin();
			--m_Iter;
			m_nBucket=0;
		}
		else
			--m_Iter;
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	bool HashIterator <Key, T, Compare, Hash>::operator==(const HashIterator& rhs) const
	{
		return ((m_pHashTable==rhs.m_pHashTable)&&(m_nBucket==rhs.m_nBucket)&&
				(m_Iter==rhs.m_Iter));
	}
//----------------------------------------------------------------------
	template <typename Key, typename T, typename Compare, typename Hash>
	bool HashIterator <Key, T, Compare, Hash>::operator!=(const HashIterator& rhs) const
	{
		return (!operator==(rhs));
	}
//----------------------------------------------------------------------
}


#endif