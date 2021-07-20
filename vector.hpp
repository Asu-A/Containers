#ifndef vector_hpp
#define vector_hpp

#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include "isInoutIterator.hpp"

namespace ft {
template < class T, class Alloc = std::allocator<T> > class vector {
private:
	T*				_data;
	size_t			_capacity;
	size_t			_size;
	Alloc			_dataAlloc;
public:
	//MARK: - Member types
	typedef T value_type;
	typedef Alloc allocator_type;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::const_reference const_reference;
	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::const_pointer const_pointer;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	
	//MARK: - Constructors
	explicit vector (const allocator_type& alloc = allocator_type()) : _data(0), _capacity(0), _size(0), _dataAlloc(alloc) {};
	explicit vector (size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()) : _dataAlloc(alloc) {
		_size = n;
		_capacity = n;
		_data = createArr(n, val);
	};
	template <class InputIterator>
	vector (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(), typename std::enable_if<ft::is_input_iterator<InputIterator>::value>::type* = 0) : _data(0), _capacity(0), _size(0), _dataAlloc(alloc) {
		size_t dist = std::distance(first, last);
		_data = createArrIter(dist, first, last);
		_size = dist;
		_capacity = dist;
	};
	vector (const vector& x) {
		_capacity = x._size;
		_size = x._size;
		const_iterator it = x.cbegin();
		const_iterator ite = x.cend();
		_data = createArrIter(_capacity, it, ite);
	};
	
	//MARK: - Destructor
	~vector() {
		clear();
		_dataAlloc.deallocate(_data, _capacity);
		_capacity = 0;
	};
	
	//MARK: - Assign
	vector& operator=(const vector& x) {
		if (this == &x)
			return (*this);
		const_iterator it = x.cbegin();
		const_iterator ite = x.cend();
		T* tmp = createArrIter(x._capacity, it, ite);
		clear();
		_dataAlloc.deallocate(_data, _capacity);
		_capacity = x._size;
		_size = x._size;
		_data = tmp;
		return (*this);
	};
	
	//MARK: - Iterators
	class iterator : public std::iterator<std::random_access_iterator_tag, value_type> {
	private:
		T*		_data;
	public:
		T* getData() { return _data; }
		iterator() : _data(0) {};
		iterator(T* src) : _data(src) {}
		iterator(const iterator& src) {
			_data = src._data;
		}
		iterator& operator=(const iterator& src) {
			if (*this == src)
				return (*this);
			this->_data = src._data;
			return (*this);
		}
		virtual ~iterator() {};
		virtual iterator& operator++() {
			_data++;
			return *this;
		}
		virtual iterator operator++(int){
			iterator tmp = *this;
			operator++();
			return tmp;
		}
		virtual iterator& operator--() {
			_data--;
			return *this;
		}
		virtual iterator operator--(int) {
			iterator tmp = *this;
			operator--();
			return tmp;
		}
		virtual bool operator==(const iterator& rhs) const {
			return _data == rhs._data;
		}
		virtual bool operator!=(const iterator& rhs) const {
			return _data != rhs._data;
		}
		virtual value_type& operator*() {
			return *_data;
		}
		virtual value_type operator->() {
			return *_data;
		}
		virtual iterator operator+(int num) {
			_data += num;
			return *this;
		}
		virtual iterator operator-(int num) {
			_data -= num;
			return *this;
		}
		virtual value_type& operator[](const size_t num) {
			return (*(_data + num));
		}
		virtual bool operator<(const iterator x) {
			return (_data < x._data);
		}
		virtual bool operator>(const iterator x) {
			return (_data > x._data);
		}
		virtual bool operator<=(const iterator x) {
			return (_data <= x._data);
		}
		virtual bool operator>=(const iterator x) {
			return (_data >= x._data);
		}
		virtual iterator& operator+=(const size_t num) {
			_data += num;
			return *this;
		}
		virtual iterator& operator-=(const size_t num) {
			_data -= num;
			return *this;
		}
	};
	class const_iterator : public std::iterator<std::random_access_iterator_tag, value_type> {
	private:
		T*		_data;
	public:
		T* getData() { return _data; }
		const_iterator() : _data(0) {};
		const_iterator(T* src) : _data(src) {}
		const_iterator(const const_iterator& src) {
			*this = src;
		}
		const_iterator& operator=(const const_iterator& src) {
			if (*this == src)
				return (*this);
			this->_data = src._data;
			return (*this);
		}
		virtual ~const_iterator() {};
		virtual const_iterator& operator++() {
			_data++;
			return *this;
		}
		virtual const_iterator operator++(int){
			const_iterator tmp = *this;
			operator++();
			return tmp;
		}
		virtual const_iterator& operator--() {
			_data--;
			return *this;
		}
		virtual const_iterator operator--(int) {
			const_iterator tmp = *this;
			operator--();
			return tmp;
		}
		virtual bool operator==(const const_iterator& rhs) const {
			return _data == rhs._data;
		}
		virtual bool operator!=(const const_iterator& rhs) const {
			return _data != rhs._data;
		}
		virtual const value_type& operator*() {
			return *_data;
		}
		virtual const value_type operator->() {
			return *_data;
		}
		virtual const_iterator operator+(int num) {
			_data += num;
			return *this;
		}
		virtual const_iterator operator-(int num) {
			_data -= num;
			return *this;
		}
		virtual value_type& operator[](const size_t num) {
			return (*(_data + num));
		}
		virtual bool operator<(const const_iterator x) {
			return (_data < x._data);
		}
		virtual bool operator>(const const_iterator x) {
			return (_data > x._data);
		}
		virtual bool operator<=(const const_iterator x) {
			return (_data <= x._data);
		}
		virtual bool operator>=(const const_iterator x) {
			return (_data >= x._data);
		}
		virtual const_iterator& operator+=(const size_t num) {
			_data += num;
			return *this;
		}
		virtual const_iterator& operator-=(const size_t num) {
			_data -= num;
			return *this;
		}
	};
	class reverse_iterator : public std::iterator<std::random_access_iterator_tag, value_type> {
		private:
			T*		_data;
		public:
			T* getData() { return _data; }
		reverse_iterator() : _data(0) {};
		reverse_iterator(T* src) : _data(src) {}
		reverse_iterator(const reverse_iterator& src) {
			_data = src._data;
		}
		reverse_iterator& operator=(const reverse_iterator& src) {
			if (*this == src)
				return (*this);
			this->_data = src._data;
			return (*this);
		}
		virtual ~reverse_iterator() {};
		virtual reverse_iterator& operator++() {
			_data--;
			return *this;
		}
		virtual reverse_iterator operator++(int){
			reverse_iterator tmp = *this;
			operator++();
			return tmp;
		}
		virtual reverse_iterator& operator--() {
			_data++;
			return *this;
		}
		virtual reverse_iterator operator--(int) {
			reverse_iterator tmp = *this;
			operator--();
			return tmp;
		}
		virtual bool operator==(const reverse_iterator& rhs) const {
			return _data == rhs._data;
		}
		virtual bool operator!=(const reverse_iterator& rhs) const {
			return _data != rhs._data;
		}
		virtual value_type& operator*() {
			return *_data;
		}
		virtual value_type operator->() {
			return *_data;
		}
		virtual reverse_iterator operator+(int num) {
			_data -= num;
			return *this;
		}
		virtual reverse_iterator operator-(int num) {
			_data += num;
			return *this;
		}
		virtual value_type& operator[](const size_t num) {
			return (*(_data + num));
		}
		virtual bool operator<(const reverse_iterator x) {
			return (_data < x._data);
		}
		virtual bool operator>(const reverse_iterator x) {
			return (_data > x._data);
		}
		virtual bool operator<=(const reverse_iterator x) {
			return (_data <= x._data);
		}
		virtual bool operator>=(const reverse_iterator x) {
			return (_data >= x._data);
		}
		virtual reverse_iterator& operator+=(const size_t num) {
			_data -= num;
			return *this;
		}
		virtual reverse_iterator& operator-=(const size_t num) {
			_data += num;
			return *this;
		}
	};
	class const_reverse_iterator : public std::iterator<std::random_access_iterator_tag, value_type> {
		private:
			T*		_data;
		public:
			T* getData() { return _data; }
		const_reverse_iterator() : _data(0) {};
		const_reverse_iterator(T* src) : _data(src) {}
		const_reverse_iterator(const const_reverse_iterator& src) {
			*this = src;
		}
		const_reverse_iterator& operator=(const const_reverse_iterator& src) {
			if (*this == src)
				return (*this);
			this->_data = src._data;
			return (*this);
		}
		virtual ~const_reverse_iterator() {};
		virtual const_reverse_iterator& operator++() {
			_data--;
			return *this;
		}
		virtual const_reverse_iterator operator++(int){
			const_reverse_iterator tmp = *this;
			operator++();
			return tmp;
		}
		virtual const_reverse_iterator& operator--() {
			_data++;
			return *this;
		}
		virtual const_reverse_iterator operator--(int) {
			const_reverse_iterator tmp = *this;
			operator--();
			return tmp;
		}
		virtual bool operator==(const const_reverse_iterator& rhs) const {
			return _data == rhs._data;
		}
		virtual bool operator!=(const const_reverse_iterator& rhs) const {
			return _data != rhs._data;
		}
		virtual value_type& operator*() {
			return *_data;
		}
		virtual value_type operator->() {
			return *_data;
		}
		virtual const_reverse_iterator operator+(int num) {
			_data -= num;
			return *this;
		}
		virtual const_reverse_iterator operator-(int num) {
			_data += num;
			return *this;
		}
		virtual value_type& operator[](const size_t num) {
			return (*(_data + num));
		}
		virtual bool operator<(const const_reverse_iterator x) {
			return (_data < x._data);
		}
		virtual bool operator>(const const_reverse_iterator x) {
			return (_data > x._data);
		}
		virtual bool operator<=(const const_reverse_iterator x) {
			return (_data <= x._data);
		}
		virtual bool operator>=(const const_reverse_iterator x) {
			return (_data >= x._data);
		}
		virtual const_reverse_iterator& operator+=(const size_t num) {
			_data -= num;
			return *this;
		}
		virtual const_reverse_iterator& operator-=(const size_t num) {
			_data += num;
			return *this;
		}
	};
	iterator begin() {
		return iterator(_data);
	};
	iterator end() {
		iterator res = iterator(_data) + static_cast<int>(_size);
		return res;
	};
	reverse_iterator rbegin() {
		reverse_iterator res = reverse_iterator(_data + _size - 1);
		return res;
	};
	reverse_iterator rend() {
		reverse_iterator res = reverse_iterator(_data - 1);
		return res;
	};
	const_iterator cbegin() const {
		return const_iterator(_data);
	};
	const_iterator cend() const {
		const_iterator res = const_iterator(_data + _size);
		return res;
	};
	const_reverse_iterator crbegin() const {
		const_reverse_iterator res = const_reverse_iterator(_data + _size - 1);
		return res;
	};
	const_reverse_iterator crend() const {
		const_reverse_iterator res = const_reverse_iterator(_data - 1);
		return res;
	};
	
	//MARK: - Capacity
	size_type size() const {
		return _size;
	};
	size_type max_size() const {
		return std::numeric_limits<size_type>::max() / sizeof(value_type);
	};
	void resize (size_type n, value_type val = value_type()) {
		size_type temp = _capacity;
		if (n > _capacity) {
			if (_capacity == 0)
				_capacity = 1;
			while (_capacity < n)
				_capacity = _capacity * 2;
		}
		iterator it = begin();
		iterator ite = end();
		T* tmp = createArrIter(_capacity, it, ite);
		if (n > _size)
			fillArr(val, _size, n, tmp);
		clear();
		_dataAlloc.deallocate(_data, temp);
		_data = tmp;
		_size = n;
	};
	size_type capacity() const {
		return _capacity;
	};
	bool empty() const {
		return (_size == 0);
	};
	void reserve (size_type n) {
		if (n > _capacity) {
			size_type temp = _size;
			iterator it = begin();
			iterator ite = end();
			T* tmp = createArrIter(n, it, ite);
			clear();
			_dataAlloc.deallocate(_data, _capacity);
			_data = tmp;
			_capacity = n;
			_size = temp;
		}
	};
	
	//MARK: - Element access
	reference operator[] (size_type n) {
		return reference(_data[n]);
	};
	const_reference operator[] (size_type n) const {
		return const_reference(_data[n]);
	};
	reference at (size_type n) {
		if (n >= _size)
			throw std::out_of_range("index out of range");
		return reference(_data[n]);
	};
	const_reference at (size_type n) const {
		if (n >= _size)
			throw std::out_of_range("index out of range");
		return const_reference(_data[n]);
	};
	reference front() {
		return reference(_data[0]);
	};
	const_reference front() const {
		return const_reference(_data[0]);
	};
	reference back() {
		return reference(_data[_size - 1]);
	};
	const_reference back() const {
		return const_reference(_data[_size - 1]);
	};
	
	//MARK: - Modifiers
	template <class InputIterator>
	void assign (InputIterator first, InputIterator last, typename std::enable_if<ft::is_input_iterator<InputIterator>::value>::type* = 0) {
		clear();
		iterator ite = end();
		long i = 0;
		for (InputIterator tmp = first; tmp != last; tmp++)
			i++;
		_data = vecRealloc(&_capacity, _size + i);
//		move(begin(), end(), i);
		for (int i = 0; first != last; i++) {
			_data[i] = *first;
			_size++;
			first++;
		}
	};
	void assign (size_type n, const value_type& val) {
		clear();
		_size = n;
		_capacity = n;
		_data = createArr(n, val);
	};
	void push_back (const value_type& val) {
		_data = vecRealloc(&_capacity, _size + 1);
		_dataAlloc.construct(&_data[_size], val);
		_size++;
	};
	void pop_back() {
		_size--;
		_dataAlloc.destroy(&_data[_size]);
	};
	iterator insert (iterator position, const value_type& val) {
		long tmp = position.getData() - _data;
		_data = vecRealloc(&_capacity, _size + 1);
		iterator res(_data + tmp);
		move(res, end(), 1);
		_data[tmp] = val;
		_size++;
		return res;
	};
	void insert (iterator position, size_type n, const value_type& val) {
		iterator ite = end();
		long tmp = position.getData() - _data;
		_data = vecRealloc(&_capacity, _size + n);
		iterator res(_data + tmp);
		move(res, end(), static_cast<int>(n));
		for (size_t i = 0; i < n; i++) {
			_data[tmp] = val;
			_size++;
			tmp++;
		}
	};
	template <class InputIterator>
	void insert (iterator position, InputIterator first, InputIterator last, typename std::enable_if<ft::is_input_iterator<InputIterator>::value>::type* = 0) {
		long i = 0;
		for (InputIterator tmp = first; tmp != last; tmp++)
			i++;
		long tmp = position.getData() - _data;
		_data = vecRealloc(&_capacity, _size + i);
		iterator res(_data + tmp);
		move(res, end(), i);
		for (; first != last; first++) {
			*res = *first;
			res++;
			_size++;
		}
	};
	iterator erase (iterator position) {
		moveBack(position, end(), 1);
		_size--;
		return position;
	};
	iterator erase (iterator first, iterator last) {
		size_type dist = 0;
		iterator tmp = first;
		for (; tmp != last; tmp++)
			dist++;
		moveBack(first, end(), dist);
		_size -= dist;
		return first;
	};
	void swap (vector& x) {
		value_type* tmpData = _data;
		size_type tmpSize = _size;
		size_type tmpCapacity = _capacity;
		_data = x._data;
		_size = x._size;
		_capacity = x._capacity;
		x._data = tmpData;
		x._size = tmpSize;
		x._capacity = tmpCapacity;
	};
	void clear() {
		while (_size != 0)
			pop_back();
	};
	
	//MARK: - Utility
private:
	T* createArr(size_type n, const value_type& val) {
		T* cont = _dataAlloc.allocate(n);
		T* tmp = cont;
		for (size_t i = 0; i < n; i++)
			_dataAlloc.construct(tmp++, val);
		return cont;
	}
	template <class InputIterator>
	T* createArrIter(size_type n, InputIterator first, InputIterator last, typename std::enable_if<ft::is_input_iterator<InputIterator>::value>::type* = 0) {
		T* cont = _dataAlloc.allocate(n);
		T* tmp = cont;
		for (; first < last; first++)
			_dataAlloc.construct(tmp++, *first);
		return cont;
	}
	T* createArrConstIter(size_type n, iterator first, iterator last) {
		T* cont = _dataAlloc.allocate(n);
		T* tmp = cont;
		for (; first < last; first++)
			_dataAlloc.construct(tmp++, *first);
		return cont;
	}
	void fillArr(const value_type& val, size_type start, size_type finish, T* data) {
		for (; start < finish; start++) {
			_dataAlloc.construct(&data[start], val);
		}
	}
	template <class InputIterator>
	size_t copyArr(InputIterator first, InputIterator last, T* data, typename std::enable_if<ft::is_input_iterator<InputIterator>::value>::type* = 0) {
		size_t arrSize = 0;
		for (; first < last; first++) {
			data[arrSize] = *first;
			arrSize++;
		}
		return arrSize;
	}
	void constCopyArr(const_iterator first, const_iterator last, T* data) {
		for (size_t i = 0; first < last; first++) {
			data[i] = *first;
			i++;
		}
	}
	T* vecRealloc(size_type *capacity, size_type n) {
		size_type temp = _capacity;
		if (n > *capacity) {
			if (*capacity == 0)
				*capacity = 1;
			while (*capacity < n)
				*capacity = *capacity * 2;
			T* tmp = _dataAlloc.allocate(*capacity);
			size_type i = 0;
			for (; i < _size; i++)
				_dataAlloc.construct(&tmp[i], _data[i]);
			clear();
			_dataAlloc.deallocate(_data, temp);
			_data = tmp;
			_size = i;
		}
		return _data;
	}
	void move(iterator pos, iterator end, long n) {
		end--;
		iterator dst = end;
		iterator src = end;
		dst += n;
		end++;
		while (src >= pos) {
			if (dst >= end)
				_dataAlloc.construct(&*dst, *src);
			else
				*dst = *src;
			src--;
			dst--;
		}
	}
	void moveBack(iterator pos, iterator end, long n) {
		iterator src = pos;
		iterator point = end;
		src += static_cast<int>(n);
		point -= static_cast<int>(n);
		while (src < end) {
			*pos = *src;
			if (src > point)
				_dataAlloc.destroy(&*src);
			++src;
			++pos;
		}
	}
};

//MARK: - Non-member function overloads
template <class T, class Alloc>
bool operator== (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
	if (lhs.size() != rhs.size())
		return false;
	typename ft::vector<T>::const_iterator lit = lhs.cbegin();
	typename ft::vector<T>::const_iterator lite = lhs.cend();
	typename ft::vector<T>::const_iterator rit = rhs.cbegin();
	typename ft::vector<T>::const_iterator rite = rhs.cend();
	while (lit != lite) {
		if (*lit != *rit)
			return false ;
		lit++;
		rit++;
	}
	if (lhs.size() == rhs.size())
		return true ;
	return false;
};
template <class T, class Alloc>
bool operator!= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
	return !(lhs == rhs);
};
template <class T, class Alloc>
bool operator<  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
	size_t size;
	if (lhs.size() < rhs.size())
		size = lhs.size();
	else
		size = rhs.size();
	typename ft::vector<T>::const_iterator lit = lhs.cbegin();
	typename ft::vector<T>::const_iterator lite = lhs.cend();
	typename ft::vector<T>::const_iterator rit = rhs.cbegin();
	typename ft::vector<T>::const_iterator rite = rhs.cend();
	for (size_t i = 0; i < size; i++) {
		if (*rit < *lit)
			return false;
		else if (*lit < *rit)
			return true;
		lit++;
		rit++;
	}
	return false;
};
template <class T, class Alloc>
bool operator<= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
	return (lhs < rhs || lhs == rhs);
};
template <class T, class Alloc>
bool operator>  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
	return !(lhs <= rhs);
};
template <class T, class Alloc>
bool operator>= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
	return (!(lhs < rhs) || lhs == rhs);
};
template <class T, class Alloc>
void swap (vector<T,Alloc>& x, vector<T,Alloc>& y) {
	x.swap(y);
};
};

#endif
