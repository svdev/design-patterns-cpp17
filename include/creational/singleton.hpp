#ifndef DESIGN_PATTERN_SINGLETON_HPP
#define DESIGN_PATTERN_SINGLETON_HPP

namespace design_patterns {
namespace creational {


template<class T>
struct singleton_container {
    static T instance;
};

template<class T>
T singleton_container<T>::instance;


template<typename T>
class singleton {

public:
    singleton(const singleton<T>&) = delete;
    singleton& operator=(const singleton<T>&) = delete;

protected:
    singleton()
    {
        /**
         * @NOTE: We defer the singleton initialization and make it thread
         * safe by using a local static variable helper and the
         * C++11 static variable initialization guarantee
         */
        // std::cout << "singleton Constructor" << std::endl;
        static bool static_init = []() -> bool {
          singleton_container<T>::instance;
          // std::cout << "singleton Initialized" << std::endl;
          return true;
        }();
    }
    ~singleton() = default;
public:
    static T& get() { return singleton_container<T>::instance; }
    T* operator->() { return &singleton_container<T>::instance; }
    const T* operator->() const { return &singleton_container<T>::instance; }
    T& operator*() { return singleton_container<T>::instance; }
    const T& operator*() const { return singleton_container<T>::instance; }
};

}
}

#endif //DESIGN_PATTERN_SINGLETON_HPP
