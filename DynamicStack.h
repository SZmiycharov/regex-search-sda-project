/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Stanislav Zmiycharov
* @idnumber 61883
* @task 0
* @compiler VC
*
*/

#include <assert.h>
#include <exception>
#include <stdlib.h>

template <typename T>
class DynamicStack {
public:
	DynamicStack();
	~DynamicStack();
	DynamicStack(DynamicStack const & obj);
	DynamicStack& operator=(DynamicStack const & obj);

	void Push(const T & Element);
	void Pop();
	T& Top();
	bool isEmpty();
	int GetLength();

private:
	void RemoveAll();
	void CopyFrom(DynamicStack const & obj);

	struct Container{
		T Value;
		Container* pNext;
		Container(T Value, Container* pNext = NULL);
	};
	Container* pTop;
	size_t Used;
};

template <typename T>
DynamicStack<T>::Container::Container(T Value, Container* pNext)
{
	this->Value = Value;
	this->pNext = pNext;
}

template <typename T>
DynamicStack<T>::DynamicStack()
{
	pTop = 0;
	Used = 0;
}

template <typename T>
DynamicStack<T>::~DynamicStack()
{
	RemoveAll();
}

template <typename T>
DynamicStack<T>::DynamicStack(DynamicStack const & obj)
{
	pTop = 0;
	Used = 0;
	CopyFrom(obj);
}

template <typename T>
DynamicStack<T>& DynamicStack<T>::operator=(DynamicStack<T> const & obj)
{
	if (this != &obj)
	{
		RemoveAll();
		CopyFrom(obj);
	}

	return *this;
}

template <typename T>
void DynamicStack<T>::Push(const T & Element)
{
	pTop = new Container(Element, pTop);

	++Used;
}

template<typename T>
void DynamicStack<T>::Pop()
{
	assert(Used > 0);

	Container* pOldTop = pTop;
	pTop = pTop->pNext;
	delete pOldTop;

	--Used;
}

template<typename T>
T& DynamicStack<T>::Top()
{
	assert(Used > 0);

	return pTop->Value;
}

template <typename T>
bool DynamicStack<T>::isEmpty()
{
	if (Used == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
int DynamicStack<T>::GetLength()
{
	return Used;
}

template <typename T>
void DynamicStack<T>::RemoveAll()
{
	Container* p;

	while (pTop)
	{
		p = pTop;
		pTop = pTop->pNext;
		delete p;
	}

	pTop = 0;
	Used = 0;
}

template <typename T>
void DynamicStack<T>::CopyFrom(DynamicStack const& obj)
{
	if (obj.Used == 0)
	{
		return;
	}

	try
	{
		pTop = new Container(obj.pTop->Value, NULL);
		Container *outCurrentElement = pTop;
		Container *theirCurrentElement = obj.pTop->pNext;

		while (theirCurrentElement)
		{
			outCurrentElement->pNext = new Container(theirCurrentElement->Value, NULL);
			outCurrentElement = outCurrentElement->pNext;
			theirCurrentElement = theirCurrentElement->pNext;
		}
		Used = obj.Used;
	}
	catch (const bad_alloc& err)
	{
		RemoveAll();
		cerr << "Exception caught: " << err.what() << '\n';
		exit(EXIT_FAILURE);
	}
}