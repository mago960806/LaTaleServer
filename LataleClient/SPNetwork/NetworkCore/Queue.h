#pragma once

#include <Windows.h>

#include "packet.h"
//#include "DebugPrint.h"

//------------------------------------------------------------------------
//  Node
//------------------------------------------------------------------------
template <class T>
class CNode
{
public:
	T Data;

	CNode<T>* next;

	CNode() {next=NULL;}
	CNode(T *data)
	{
		Data=(*data);
		next=NULL;
	}
};

//------------------------------------------------------------------------
//	Queue
//------------------------------------------------------------------------
template <class T>
class CQueue
{
protected :
	CNode<T> *Front,*Tail;
	void push(T* data);
	CNode<T> pop();

public:
	CQueue() {Front=NULL;Tail=NULL;}
	~CQueue() {reset();}

	void reset();
	bool empty();

	int size(); //[xialin 2008/09/08]add
	
	virtual void AddQ(T* data)	
	{
		push(data);
	}

	virtual bool PopQ(T& data)
	{
		if(empty()) return false;
		data = pop().Data;
		return true;
	}
};

//[xialin 2008/09/08]add
template <class T>
int CQueue<T>::size()
{
	int iCount = 0;
	if (Front!=NULL)
	{
		CNode<T> * pTmp = Front;
		for ( ; pTmp;  ++iCount)
			pTmp = pTmp->next;
	}
	return iCount;
}

template <class T>
void CQueue<T>::reset()
{
	while(Front!=NULL)
	{
		pop();
	}
	Front=NULL;
	Tail=NULL;
}

template <class T>
void CQueue<T>::push(T* data)
{
	if(data==NULL) return;

	if(Front==NULL)
	{
		Front=new CNode<T>(data);
		Front->next=NULL;
		Tail=Front;
	}
	else
	{
		CNode<T> *t_Node;
		t_Node=new CNode<T>(data);
		assert(Tail);
		Tail->next=t_Node;
		Tail=t_Node;
	}
}

template <class T>
CNode<T> CQueue<T>::pop(void)
{
	CNode<T> t_Node;
	if(Front==NULL){Tail=NULL;return t_Node;}
	t_Node=*Front;
	delete Front;
	Front=t_Node.next;
	return t_Node;
}

template <class T>
bool CQueue<T>::empty()
{
	if(Front==NULL)
		return true;
	return false;
}
//------------------------------------------------------------------------
// Queue 를 이용한 Memory Pool 버전 Queue
// 최초 생성시 N개의 TNode를 생성해두고 
// N개의 TNode를 모두 소진한후에는 New 로 생성한다. 
// 생성한 TNode는 delete 하지 않고 MemoryPool에 넣어서 재활용해준다.
//------------------------------------------------------------------------

template <class T>
class CMemoryPoolQueue
{
protected :
	CNode<T> *Front,*Tail;
	void push(T* data);
	CNode<T> pop();

	CQueue<CNode<T>*> m_MemoryPool;
public:
	CMemoryPoolQueue() 
	{
		int iSize = 0;
		for(int i = 0; i<iSize; ++i)
		{
			CNode<T>* pNode = new CNode<T>; // 노드를 iSize 만큼 만들어 둔다
			m_MemoryPool.AddQ(&pNode);
			DebugPrint("New Node %p\n", pNode);
		}
		Front=NULL;Tail=NULL;
	}
	~CMemoryPoolQueue() {reset();}

	void reset();
	bool empty();

	virtual void AddQ(T* data)	
	{
		push(data);
	}

	virtual bool PopQ(T& data)
	{
		if(empty()) return false;
		data = pop().Data;
		return true;
	}
};

template <class T>
void CMemoryPoolQueue<T>::reset()
{
	DebugPrint("CMemoryPoolQueue<T>::reset()");
	m_MemoryPool.reset();
	Front=NULL;
	Tail=NULL;
}

template <class T>
void CMemoryPoolQueue<T>::push(T* data)
{
	if(data==NULL) return;

	CNode<T>* pNode = NULL;
	m_MemoryPool.PopQ(pNode); // Pool에 존재하는 미리 만들어진 Value(CNode의 주소값)를 가져오기
	if(!pNode)
	{
		pNode = new CNode<T>(data);
		DebugPrint("New Node %p\n", pNode);
	}
	else
	{
		pNode->Data = (*data);
		DebugPrint("Use Node %p\n", pNode);
	}

	if(Front==NULL)
	{
		Front=pNode;
		Front->next=NULL;
		Tail=Front;
	}
	else
	{
		Tail->next=pNode;
		Tail=pNode;
	}
}

template <class T>
CNode<T> CMemoryPoolQueue<T>::pop(void)
{
	CNode<T> t_Node;
	if(Front==NULL){Tail=NULL; return t_Node;}
	t_Node=*Front;
	Front->next=NULL;
	m_MemoryPool.AddQ(&Front); // Front 안에 있는 값(주소)를 돌려준다
	DebugPrint("Free Node %p\n", Front);
	Front=t_Node.next;
	return t_Node;
}

template <class T>
bool CMemoryPoolQueue<T>::empty()
{
	if(Front==NULL)
		return true;
	return false;
}

///////////////////////////////////////////////
// 
//  Thread Safety Queue
//
//  CQueue Wrapped 
//  by Sohn, Jeong E
//

// This is dummy lock, after Replace your Released Lock Object..
class CLock
{
public :
	CLock();
	~CLock();
	void lock();
	void unlock();	
	CRITICAL_SECTION m_CS;
	
};


template <class T>
class CLockedQueue : public CQueue<CPacket>
{
private:
	CLock m_cLock;
public:

	CLockedQueue() { Init(); }
	//CLockedQueue(CLock *m_cLock) { Init(); }
	~CLockedQueue(){;}
	
	void Init();
	void AddQ(T* data);
	bool PopQ(T& data);
	
};

// Thread Safety Queue

template <class T>
bool CLockedQueue<T>::PopQ(T& data) 
{
	m_cLock.lock();

	if(empty())
	{
		m_cLock.unlock();
		return false;
	}

	// Packet 처리
	data=pop().Data;
	m_cLock.unlock();
	return true;
}
template <class T>
void CLockedQueue<T>::AddQ(T* data)
{
	
	m_cLock.lock();
	push(data);
	m_cLock.unlock();
}

template <class T>
void CLockedQueue<T>::Init()
{
	m_cLock.lock();
	reset();
	m_cLock.unlock();
}


