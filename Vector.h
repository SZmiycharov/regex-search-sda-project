#pragma once
#include <iostream>
#include <typeinfo>

template <typename T>
class Vector
{
private:
	//actual members
	
	//length of array
	int m_capacity;
	T * vectArr;
	//call copy whenever capacity changes
	void copy(T**, int);

public:
	int m_size;
	Vector();
	Vector(int);
	Vector(const Vector<T>&);
	Vector<T>& operator=(const Vector<T>&);
	~Vector();

	void push_back(const T&);
	void pop_back();
	void resize(int);

	T& back();
	T& front();

	void insert(int, T);
	void erase(int);
	T& operator[](int);
	int size();
	int capacity();
	bool empty();
	void clear();
	void changeByIndex(int index, T value);

	void shrinkToSize();
	void substitute(Vector<T>&);
	size_t maxAcceptableSize() const;
};

template <typename T>
Vector<T>::Vector()
{
	m_capacity = 4;
	m_size = 0;
	vectArr = new T[m_capacity];
}

template<typename T>
Vector<T>::Vector(int v)
{
	if (v >= 1)
	{
		m_capacity = v;
		m_size = v;
		vectArr = new T[v];
	}
	else
	{
		throw std::invalid_argument("Invalid capacity !");
	}
}

template<typename T>
Vector<T>::Vector(const Vector<T>& v)
{
	m_size = v.m_size;
	m_capacity = v.m_capacity;
	vectArr = new T[m_capacity];
	copy(&vectArr, m_size);
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T> & v)
{
	delete[] vectArr;
	m_size = v.m_size;
	m_capacity = v.m_capacity;
	vectArr = new T[m_capacity];
	for (int i = 0; i < m_size; i++)
	{
		vectArr[i] = v.vectArr[i];
	}
	return *this;
}

template<typename T>
Vector<T>::~Vector()
{
	delete[] vectArr;
}

template<typename T>
void Vector<T>::push_back(const T& value)
{
	if (m_capacity == m_size)
	{
		m_capacity *= 2;
		copy(&vectArr, m_size);
	}
	vectArr[m_size] = value;
	m_size++;
}

template<typename T>
void Vector<T>::pop_back()
{
	if (!empty())
	{
		shrinkToSize();
		copy(&vectArr, m_size);
		m_size--;
	}
	else
	{
		throw std::out_of_range("The vector is out of range !");
	}
}

template<typename T>
void Vector<T>::resize(int newSize)
{
	if (newSize > m_capacity) {
		m_capacity *= 2;
		copy(&vectArr, m_size);
	}
	int i = newSize - (newSize - m_size);
	m_size = newSize;
	string type = typeid(vectArr[0]).name();
	T temp;
	if (type.find("string") != -1)
	{
		temp = T("0");
	}
	else
	{
		temp = T(0);
	}
	for (i; i < newSize; i++)
	{
		vectArr[i] = temp;
	}
}

template<typename T>
T& Vector<T>::back()
{
	if (empty())
	{
		throw std::out_of_range("The vector is empty !");
	}
	else
	{
		return vectArr[m_size - 1];
	}
}

template<typename T>
T& Vector<T>::front()
{
	if (empty())
	{
		throw std::out_of_range("The vector is empty !");
	}
	else
	{
		return vectArr[0];
	}
}

template<typename T>
void Vector<T>::insert(int index, T value)
{
	if (m_capacity > index)
	{
		if (m_size > index)
		{
			if (m_size == m_capacity)
			{
				m_capacity *= 2;
			}
			m_size++;
			T *temp = new T[m_capacity];

			if (index != 0)
			{
				int i = 0;
				for (i; i < index; i++)
				{
					temp[i] = vectArr[i];
				}
			}
			temp[index] = value;
			int i = index + 1;
			for (i; i < m_size; i++)
			{
				temp[i] = vectArr[i - 1];
			}

			delete[] vectArr;
			vectArr = temp;
		}
		else
		{
			vectArr[index] = value;
			m_size = index;
		}
	}
	else
	{
		m_capacity *= 2;
		copy(&vectArr, m_size);
		insert(index, value);
	}
}

template<typename T>
void Vector<T>::erase(int index)
{
	if (m_capacity > index)
	{
		T *temp = new T[m_capacity];
		int i = 0;
		for (i; i < index; i++)
		{
			temp[i] = vectArr[i];
		}
		i = index + 1;
		for (i; i < m_size; i++)
		{
			temp[i - 1] = vectArr[i];
		}
		delete[] vectArr;
		vectArr = temp;
		shrinkToSize();
		m_size--;
	}
}

template<typename T>
T& Vector<T>::operator[](int index)
{
	return vectArr[index];
}

template<typename T>
int Vector<T>::size()
{
	return m_size;
}

template<typename T>
int Vector<T>::capacity()
{
	return m_capacity;
}

template<typename T>
bool Vector<T>::empty()
{
	if (m_size == 0)
	{
		return true;
	}
	return false;
}

template<typename T>
void Vector<T>::clear()
{
	m_capacity = 4;
	m_size = 0;
	vectArr = new T[4];
}

template<typename T>
void Vector<T>::changeByIndex(int index, T value)
{
	if (index > size) return;

	vectArr[index] = value;
}

template<typename T>
void Vector<T>::shrinkToSize()

{
	if (((double)(m_size + 1) / m_capacity) < 0.25)
	{
		m_capacity /= 2;
	}
}

template<typename T>
void Vector<T>::substitute(Vector<T> & v)
{
	Vector <T> * temp = new Vector<T>();
	*temp = *this;
	*this = v;
	v = *temp;
}

template<typename T>
size_t Vector<T>::maxAcceptableSize() const
{
	return size_t(-1) / sizeof(T);
}

template<typename T>
void Vector<T>::copy(T **arr, int end)
{
	T *temp = new T[m_capacity];
	int i = 0;
	for (i; i < end; i++)
	{
		temp[i] = (*arr)[i];
	}
	delete[] * arr;
	*arr = temp;
}