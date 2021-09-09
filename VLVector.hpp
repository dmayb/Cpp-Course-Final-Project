/**
 * author: May Bibi
 * some notes:
 * i did not have too much time to check for edge cases (especially in insert & erase)
 * i did not have time to insert noexcept where needed
 * i did not have time to make the code shorter
 */

#ifndef EXAMCPP_VLVECTOR_HPP
#define EXAMCPP_VLVECTOR_HPP

#include <iostream>
#include <cmath>
#include <algorithm>

#define DEFAULT_STATIC_CAP 16
#define OUT_OF_RANGE_ERR "out of range index. range is [0,size)\n"

/**
 * for VLVector class - memory state of the container
 */
enum arrayState
{
	Static,
	Dynamic
};

/**
 * @class VLVector
 * @brief : Template class for maintaining a std::vector-like container. VLVector uses both
 * stack&heap memory. It holds its elements on stack-array of size StaticCapacity when
 * numOfElements <= StaticCapacity and on a hep-allocated array when numOfElements > StaticCapacity
 * Its iterator is a pointer to T.
 * @tparam T
 * @tparam StaticCapacity
 */
template<class T, size_t StaticCapacity = DEFAULT_STATIC_CAP>
class VLVector
{
private:
	T _staticArray[StaticCapacity]; // for holding the static memory (stack)
	T *_dynamicArray; // for holding the dynamic memory (heap)
	size_t _size;
	size_t _capacity;
	arrayState _arrayState; // enum of Dynamic/Static

	/**
	 * calc and update data structure capacity
	 */
	void _updateCapacity()
	{ _capacity = _size <= StaticCapacity ? StaticCapacity : (size_t) floor((3 * _size) / 2); }

	/**
	 * NOTE: i wrote this function before i knew about std::copy and copy_n.
	 * in some of the code i use std::Copy and in some of it i use _copyArray2Array
	 * i wanted to change it all to std::copy but i didnt have time to do that.
	 * @param copyTo
	 * @param copyFrom
	 * @param numVals
	 */
	void _copyArray2Array(const T *copyFrom, T *copyTo, const size_t numVals);

public:
	/**
	 * iterator - pointer T*
	 */
	typedef T *iterator;
	typedef const T *const_iterator;

	iterator begin()
	{ return data(); }

	iterator end()
	{ return data() + _size; }

	const_iterator begin() const
	{ return data(); }

	const_iterator end() const
	{ return data() + _size; }

	const_iterator cbegin() const
	{ return data(); }

	const_iterator cend() const
	{ return data() + _size; }

	/**
	 * default constructor
	 */
	VLVector() : _staticArray{}, _dynamicArray(nullptr), _size(0), _capacity(StaticCapacity),
				 _arrayState(Static)
	{};

	/**
	 * copy constructor
	 * @param vec
	 */
	VLVector(const VLVector &vec);

	/**
	 * constructor by iterator
	 * @param first
	 * @param last
	 */
	template<class InputIterator>
	VLVector(InputIterator first, InputIterator last);

	/**
	 * destructor
	 */
	~VLVector();

	/**
	 * @return the size of the vector
	 */
	size_t size() const
	{ return _size; };

	/**
	 * @return the capacity of the vector
	 */
	size_t capacity() const
	{ return _capacity; };

	/**
	 * check if empty
	 * @return true if empty false otherwise
	 */
	bool empty() const
	{ return _size == 0; }

	/**
	 * safe access an index in VLVector. raises exception index is of out of bound
	 * @tparam T
	 * @tparam StaticCapacity
	 * @param i
	 * @return the i'th value in the VLVector
	 */
	T &at(const size_t i);

	/**
	 * safe access an index in VLVector. raises exception index is of out of bound
	 * @tparam T
	 * @tparam StaticCapacity
	 * @param i
	 * @return the i'th value in the VLVector
	 */
	const T &at(const size_t i) const;

	/**
	 * push new value to VLVector
	 * @tparam T
	 * @tparam StaticCapacity
	 * @param newVar
	 */
	void push_back(const T &newVar);

	/**
	 * insert new value at position
	 * @param position
	 * @param newVal
	 * @return iterator to new value
	 */
	iterator insert(const_iterator position, const T &newVal);

	/**
	 * insert range [first,last) to position
	 * @param position
	 * @param first
	 * @param last
	 * @return iterator to the first value that was inserted
	 */
	template<class InputIterator>
	iterator insert(const_iterator position, InputIterator first, InputIterator last);

	/**
	 * remove the last value in VLVector
	 */
	void pop_back();

	/**
	 * erase the element that position points to
	 * @tparam T
	 * @tparam StaticCapacity
	 * @param position
	 * @return  iterator to the element after erased element
	 */
	iterator erase(const_iterator position);

	/**
	 * erase all elements in the range [first,last)
	 * @param first - const_iterator to VLVector
	 * @param last - const_iterator to VLVector
	 * @return iterator to the first element after erased elements
	 */
	iterator erase(const_iterator first, const_iterator last);

	/**
	 * clears all
	 */
	void clear();

	/**
	 * @return  returns pointer to data storage (non-const version, data can be changed from
	 * outside)
	 */
	T *data()
	{ return _arrayState == Static ? _staticArray : _dynamicArray; }

	/**
	 * @return  returns pointer to data storage (const version, data cant be changed from outside)
	 */
	const T *data() const
	{ return _arrayState == Static ? _staticArray : _dynamicArray; }

	//operators:
	/**
	 * copy given VLVector object to this
	 * @tparam T
	 * @tparam StaticCapacity
	 * @param vec
	 * @return ref to self
	 */
	VLVector &operator=(const VLVector &vec);

	/**
	 * access data with index - non-const version
	 * @tparam T
	 * @tparam StaticCapacity
	 * @param i
	 * @return  the i'th element in the data structure
	 */
	T &operator[](const size_t i)
	{ return data()[i]; }

	/**
	 * access data with index - const version
	 * @tparam T
	 * @tparam StaticCapacity
	 * @param i
	 * @return  the i'th element in the data structure
	 */
	const T &operator[](const size_t i) const
	{ return data()[i]; }

	/**
	 * check for equality
	 * @param vec
	 * @return
	 */
	bool operator==(const VLVector &vec) const;

	/**
	 * check for inequality
	 * @param vec
	 * @return
	 */
	bool operator!=(const VLVector &vec) const
	{ return !(*this == vec); }
};

/**
 *
 * @tparam T
 * @tparam StaticCapacity
 * @param copyTo
 * @param copyFrom
 * @param numVals
 */
template<class T, size_t StaticCapacity>
void
VLVector<T, StaticCapacity>::_copyArray2Array(const T *copyFrom, T *copyTo, const size_t numVals)
{
	for (size_t i = 0; i < numVals; ++i)
	{
		copyTo[i] = copyFrom[i];
	}
}

/**
 * copy constructor
 * @tparam T
 * @tparam StaticCapacity
 * @param vec
 */
template<class T, size_t StaticCapacity>
VLVector<T, StaticCapacity>::VLVector(const VLVector &vec)
		: _staticArray{}, _dynamicArray(nullptr), _size(vec._size), _capacity(vec._capacity),
		  _arrayState(vec._arrayState)
{
	if (vec._arrayState == Dynamic)
	{
		_arrayState = Dynamic;
		_dynamicArray = new T[_capacity];
		_copyArray2Array(vec._dynamicArray, _dynamicArray, _size);
	}
	else
	{
		_arrayState = Static;
		_copyArray2Array(vec._staticArray, _staticArray, _size);
	}
}

/**
 * constructor by iterator
 * @tparam T
 * @tparam StaticCapacity
 * @tparam InputIterator
 * @param first - InputIterator
 * @param last - InputIterator
 */
template<class T, size_t StaticCapacity>
template<class InputIterator>
VLVector<T, StaticCapacity>::VLVector(InputIterator first, InputIterator last)
		: _staticArray{}, _dynamicArray(nullptr), _size(0), _capacity(StaticCapacity),
		  _arrayState(Static)
{
	while (!(first == last))
	{
		push_back(*first);
		first++;
	}
}

/**
 * destructor
 * @tparam T
 * @tparam StaticCapacity
 */
template<class T, size_t StaticCapacity>
VLVector<T, StaticCapacity>::~VLVector()
{
	if (_arrayState == Dynamic)
	{
		delete[] _dynamicArray;
	}
};

/**
 * safe access an index in VLVector. raises exception index is of out of bound
 * @tparam T
 * @tparam StaticCapacity
 * @param i
 * @return the i'th value in the VLVector
 */
template<class T, size_t StaticCapacity>
T &VLVector<T, StaticCapacity>::at(const size_t i)
{
	if (i < 0 || i >= _size)
	{
		throw std::out_of_range(OUT_OF_RANGE_ERR);
	}
	return data()[i];
}

/**
 * safe access an index in VLVector. raises exception index is of out of bound
 * @tparam T
 * @tparam StaticCapacity
 * @param i
 * @return the i'th value in the VLVector
 */
template<class T, size_t StaticCapacity>
const T &VLVector<T, StaticCapacity>::at(const size_t i) const
{
	if (i < 0 || i >= _size)
	{
		throw std::out_of_range(OUT_OF_RANGE_ERR);
	}

	return data()[i];
}

/**
 * push new value to VLVector
 * @tparam T
 * @tparam StaticCapacity
 * @param newVar
 */
template<class T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::push_back(const T &newVar)
{
	_size++;
	if (_size <= _capacity)
	{
		data()[_size - 1] = newVar;
	}
	else
	{
		_updateCapacity();
		if (_arrayState == Dynamic)
		{
			T *temp = new T[_capacity];
			_copyArray2Array(_dynamicArray, temp, _size - 1);
			temp[_size - 1] = newVar;
			delete[] _dynamicArray;
			_dynamicArray = temp;
		}
		else // moving from static to dynamic
		{
			_arrayState = Dynamic;
			_dynamicArray = new T[_capacity];
			_copyArray2Array(_staticArray, _dynamicArray, _size - 1);
			_dynamicArray[_size - 1] = newVar;
		}
	}
}

/**
 * insert new value at position
 * @tparam T
 * @tparam StaticCapacity
 * @param position
 * @param newVal
 * @return iterator to new value
 */
template<class T, size_t StaticCapacity>
typename VLVector<T, StaticCapacity>::iterator VLVector<T, StaticCapacity>::insert
		(const_iterator position, const T &newVal)
{
	size_t dist = std::distance(cbegin(), position);
	iterator pos = begin() + dist;
	_size++;
	if (_size <= _capacity)
	{
		std::copy(pos, end() - 1, pos + 1);
		*pos = newVal;
		return pos;
	}
	//now we have to increase the array
	_updateCapacity();
	if (_arrayState == Dynamic)
	{
		T *temp = new T[_capacity];
		std::copy(begin(), pos, temp);
		temp[dist] = newVal;
		std::copy(pos, end() - 1, temp + dist + 1);
		delete[] _dynamicArray;
		_dynamicArray = temp;
	}
	else // moving from static to dynamic
	{
		_arrayState = Dynamic;
		_dynamicArray = new T[_capacity];
		std::copy_n(_staticArray, dist, _dynamicArray);
		_dynamicArray[dist] = newVal;
		try{
			std::copy(_staticArray + dist, end() - 1, _dynamicArray + dist + 1);
		}
		catch (std::exception e){
			std::cout << e.what();
		}
	}
	return &(_dynamicArray[dist]);
}

/**
 * insert range [first,last) to position
 * @tparam T
 * @tparam StaticCapacity
 * @tparam InputIterator
 * @param position
 * @param first
 * @param last
 * @return iterator to the first value that was inserted
 */
template<class T, size_t StaticCapacity>
template<class InputIterator>
typename VLVector<T, StaticCapacity>::iterator
VLVector<T, StaticCapacity>::insert(const_iterator position, InputIterator first,
									InputIterator last)
{
	size_t dist = std::distance(cbegin(), position);
	iterator pos = begin() + dist;
	if (first == last)
	{
		return pos;
	}
	size_t numOfVals = std::distance(first, last);
	_size += numOfVals;
	if (_size <= _capacity)
	{
		std::copy(pos, end() - numOfVals, pos + numOfVals);
		std::copy(first, last, pos);
		return pos;
	}
	//now we have to increase the array
	_updateCapacity();
	if (_arrayState == Dynamic)
	{
		T *temp = new T[_capacity];
		std::copy(begin(), pos, temp);
		std::copy(first, last, temp + dist);
		std::copy(pos, end() - numOfVals, temp + dist + numOfVals);
		delete[] _dynamicArray;
		_dynamicArray = temp;
	}
	else // moving from static to dynamic
	{
		_arrayState = Dynamic;
		_dynamicArray = new T[_capacity];
		std::copy_n(_staticArray, dist, _dynamicArray);
		std::copy(first, last, _dynamicArray + dist);
		std::copy(_staticArray + dist, end() - numOfVals, _dynamicArray + dist + numOfVals);
	}
	return &(_dynamicArray[dist]);
}

/**
 * remove the last value in VLVector
 * @tparam T
 * @tparam StaticCapacity
 */
template<class T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::pop_back()
{
	if (_size == 0)
	{
		return;
	}
	_size--;
	if (_arrayState == Static)
	{
		return;
	}
	if (_size == StaticCapacity) // means we move back to static memory
	{
		_arrayState = Static;
		_copyArray2Array(_dynamicArray, _staticArray, _size);
		delete[] _dynamicArray;
		_dynamicArray = nullptr;
		_updateCapacity();
	}
}

/**
 * erase the element that position points to
 * @tparam T
 * @tparam StaticCapacity
 * @param position
 * @return  iterator to the element after erased element
 */
template<class T, size_t StaticCapacity>
typename VLVector<T, StaticCapacity>::iterator
VLVector<T, StaticCapacity>::erase(const_iterator position)
{
	if (position == cend() - 1) //todo check what to return
	{
		pop_back();
		return end();
	}
	size_t dist = std::distance(cbegin(), position);
	iterator pos = begin() + dist;
	_size--;
	if (_arrayState == Static || _size > StaticCapacity) // we either static or stay dynamic
	{
		std::copy(pos + 1, end() + 1, pos);
		return pos;
	}
	// now we have to move back to static
	_arrayState = Static;
	_updateCapacity();
	std::copy_n(_dynamicArray, dist, _staticArray);
	std::copy_n(_dynamicArray + dist + 1, _size - dist, _staticArray + dist);
	return _staticArray + dist;
}

/**
 * erase all elements in the range [first,last)
 * @tparam T
 * @tparam StaticCapacity
 * @param first - const_iterator to VLVector
 * @param last - const_iterator to VLVector
 * @return iterator to the first element after erased elements
 */
template<class T, size_t StaticCapacity>
typename VLVector<T, StaticCapacity>::iterator
VLVector<T, StaticCapacity>::erase(const_iterator first, const_iterator last)
{
	size_t dist = std::distance(cbegin(), first);
	iterator posFirst = begin() + dist;
	if (first == last)
	{
		return posFirst;
	}
	size_t numOfVals = std::distance(first, last);
	iterator posLast = posFirst + numOfVals;
	_size -= numOfVals;
	if (_arrayState == Static || _size > StaticCapacity) // we either static or stay dynamic
	{
		std::copy(posLast, end() + numOfVals, posFirst);
		return posLast;
	}
	// now we have to move back to static
	_arrayState = Static;
	_updateCapacity();

	std::copy_n(_dynamicArray, dist, _staticArray);
	std::copy_n(_dynamicArray + dist + numOfVals, _size - dist, _staticArray + dist);

	return _staticArray + dist;
}

/**
 * clears all
 * @tparam T
 * @tparam StaticCapacity
 */
template<class T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::clear()
{
	_size = 0;
	_updateCapacity();
	_arrayState = Static;
	if (_arrayState == Dynamic)
	{
		delete[] _dynamicArray;
	}
}

/**
 * copy given VLVector object to this
 * @tparam T
 * @tparam StaticCapacity
 * @param vec
 * @return ref to self
 */
template<class T, size_t StaticCapacity>
VLVector<T, StaticCapacity> &VLVector<T, StaticCapacity>::operator=(const VLVector &vec)
{
	if (&vec == this)
	{
		return *this;
	}
	_size = vec._size;
	_capacity = vec._capacity;

	if (_arrayState == Dynamic)
	{
		delete[] _dynamicArray;
	}

	if (vec._arrayState == Dynamic)
	{
		_arrayState = Dynamic;
		_dynamicArray = new T[_capacity];
		_copyArray2Array(vec._dynamicArray, _dynamicArray, _size);
	}
	else
	{
		_arrayState = Static;
		_copyArray2Array(vec._staticArray, _staticArray, _size);
	}
	return *this;
}

/**
 *
 * @tparam T
 * @tparam StaticCapacity
 * @param vec
 * @return true if each of the elements are equal. false otherwise
 */
template<class T, size_t StaticCapacity>
bool VLVector<T, StaticCapacity>::operator==(const VLVector &vec) const
{
	if (_size != vec._size)
	{
		return false;
	}
	if (_arrayState == Static && vec._arrayState == Static)
	{
		return std::equal(_staticArray, _staticArray + _size, vec._staticArray);
	}
	if (_arrayState == Static && vec._arrayState == Dynamic)
	{
		return std::equal(_staticArray, _staticArray + _size, vec._dynamicArray);
	}
	if (_arrayState == Dynamic && vec._arrayState == Static)
	{
		return std::equal(_dynamicArray, _dynamicArray + _size, vec._staticArray);
	}
	// _arrayState == Dynamic && vec._arrayState == Dynamic - only option
	return std::equal(_dynamicArray, _dynamicArray + _size, vec._dynamicArray);
}

#endif //EXAMCPP_VLVECTOR_HPP
