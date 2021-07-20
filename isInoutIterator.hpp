//
//  isInoutIterator.hpp
//  ft_containers
//
//  Created by Frenica Torn on 4/26/21.
//  Copyright © 2021 Frenica Torn. All rights reserved.
//

#ifndef isInoutIterator_hpp
#define isInoutIterator_hpp

namespace ft {
template <class _Tp>
struct is_input_iterator : public std::__has_iterator_category_convertible_to<_Tp, std::input_iterator_tag> {};
template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T *, class Reference = T &>
class iterator : public Category {
public:
	typedef T				value_type;
	typedef Distance		difference_type;
	typedef Pointer			pointer;
	typedef Reference		reference;
	typedef Category		iterator_category;
};
}

#endif /* isInoutIterator_hpp */
