#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

template <class T> class CLinkedList;

template <class T> class CListEntry
{
public:
	CListEntry();
	CListEntry( T* );
	~CListEntry();

	void Insert( T* data );
	void Remove();
	CListEntry<T>* GetNext();
	CListEntry<T>* GetPrev();

	T* data;

	CListEntry<T>* prev;
	CListEntry<T>* next;
	CLinkedList<T>* parent;
};

template <class T> class CLinkedList
{
public:
	CLinkedList();
	~CLinkedList();

	void Add( T* data );
	void Add( T* data, int index );
	void RemoveEntry( CListEntry<T>* l );
	CListEntry<T>* FindEntry( T* data );
	int GetLength();
	CListEntry<T>* GetFirst();
	CListEntry<T>* GetLast();

	int length;
	CListEntry<T>* first;
	CListEntry<T>* last;
};

// Templated Class, Need To Include Implementation Directly
#include "list.cpp"

#endif
