#ifndef PATTERNS_SINGLETON_HPP
#define PATTERNS_SINGLETON_HPP

#include <mutex>
#include "util/text.hpp"
#include "exception.hpp"

namespace design_patterns {
namespace creational {

class singleton_exception: public design_pattern_exception {
public:
    singleton_exception(const std::string& msg): design_pattern_exception(msg){}
};


template<class T>
struct singleton_container {
    static T instance;
    static std::mutex mtx;
};

template<class T>
T singleton_container<T>::instance;

template<class T>
std::mutex singleton_container<T>::mtx;


template<typename T>
class singleton {
public:
    singleton(const singleton<T>&) = default;
    singleton& operator=(const singleton<T>&) = delete;

    static T& get() { return singleton_container<T>::instance;}
    T* operator->() { return &singleton_container<T>::instance; }
    const T* operator->() const { return &singleton_container<T>::instance; }
    T& operator*() { return singleton_container<T>::instance; }
    const T& operator*() const { return singleton_container<T>::instance; }

    template <typename U>
    inline bool operator==(const U& rhs) const {
        return std::is_same<T,U>::value;
    }
    template <typename U>
    inline bool operator!=(const U& rhs) const {
        return !(*this == rhs);
    }
    struct lock {
      explicit lock(){
          if (!singleton_container<T>::mtx.try_lock())
              throw singleton_exception("Could not acquire singleton lock");
      }
      ~lock(){ singleton_container<T>::mtx.unlock();}
    };
protected:
    singleton() = default;
    virtual ~singleton() = default;
};

}
}

#endif //PATTERNS_SINGLETON_HPP
