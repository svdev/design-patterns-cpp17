#ifndef PATTERNS_UTIL_TYPES_HPP
#define PATTERNS_UTIL_TYPES_HPP

#include <type_traits>
#include <typeinfo>


template<class T>
struct is_shared_ptr : std::false_type {};

template<class T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};


template<class T>
struct is_unique_ptr : std::false_type {};

template<class T>
struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};


template<typename T>
using base_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;


template<typename T>
using is_not_smart_ptr = typename std::enable_if<
        is_shared_ptr<T>::value==false&&
                is_unique_ptr<T>::value==false,T>;


#endif //PATTERNS_UTIL_TYPES_HPP