#ifndef DESIGN_PATTERN_FACTORY_HPP
#define DESIGN_PATTERN_FACTORY_HPP

#include <memory>
#include <map>
#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

#include "util/color.hpp"
#include "exception.hpp"

namespace design_patterns {
namespace creational {

class factory_exception : public design_pattern_exception {
public:
    explicit factory_exception(const std::string& msg):design_pattern_exception(msg) { };
};

class factory_create_exception : public factory_exception {
public:
    explicit factory_create_exception(const std::string& tag) :
            factory_exception("Unknown type under name '" + tag + "'"){};
};

template<class BaseType, class... Args>
struct map_holder {
  static std::map<std::string,
                  std::unique_ptr<BaseType>(*)(Args&& ...)> functions;
};

template<class BaseType, class... Args>
std::map<std::string, std::unique_ptr<BaseType>(*)(Args&& ...)>map_holder<
        BaseType,
        Args...>::functions;

template<class T>
class factory {
public:

    factory(const factory&) = default;
    void operator=(factory const&) = delete;

    static factory& get_instance()
    {
        static factory instance;
        return instance;
    }

    template<class ...Args>
    unsigned long registered() const
    {
        return map_holder<T, Args&& ...>::functions.size();
    }

    const std::string name() const
    {
        return demangle(typeid(*this).name());
    }

    template<class TDerived, typename ...Args>
    bool register_type(const std::string& name)
    {

        if (map_holder<T, Args&& ...>::functions.count(name)==0) {

            map_holder<T, Args&& ...>::functions[name] =
                    &create_func<TDerived, Args...>;

            std::cout << "[OK]: " << (*this).name() << ": "
                      << FBLU(demangle(typeid(TDerived).name()))
                      << " type registered under name " << FGRN(name)
                      << std::endl;
            /**
            std::cout << "Ptr: "
                      << &MapHolder<T, Args...>::functions[name]
                      << std::endl;
            */
            return true;
        }

        std::cout << "[ERROR]: Type already registered: " << name <<
                  std::endl;
        return false;
    }
    /**
     * Create instance by a registered name
     * @param name  The name of the registered class type
     * @return      An instance of the requested class name, or null if not
     *              found.
     */
    template<typename ...Args>
    std::unique_ptr<T> create(const std::string& name, Args&& ...args)
    {
        try {
            auto el = map_holder<T, Args&& ...>::functions.at(name);
            return el(std::forward<Args>(args)...);
        }
        catch (std::exception& ex) {
            throw factory_create_exception(name);
        }
        /**
        auto it = MapHolder<T, Args&&...>::functions.find(name);
        if (it!=MapHolder<T, Args&&...>::functions.end()) {
            std::cout << "[OK] factory::create() " << name << std::endl;
            std::cout << "Ptr: " << &(*it).second << std::endl;
            return (*it).second(std::forward<Args>(args)...);
        }
        std::cout << "[ERROR]: factory::create() " << name << std::endl;
        return std::unique_ptr<T>(nullptr);
        */
    }
protected:
    static const std::string demangle(const char* name)
    {
        int status = -4;
        char* res = abi::__cxa_demangle(name, nullptr, nullptr, &status);
        const char* const demangled_name = (status==0) ? res : name;
        std::string ret_val(demangled_name);
        free(res);
        return ret_val;
    }

private:
    factory() = default;

    template<class TDerived, typename ...Args>
    static std::unique_ptr<T> create_func(Args&& ...args)
    {
        std::cout << "[INFO]: factory::create_func() "
                  << typeid(TDerived).name() << std::endl;
        return std::make_unique<TDerived>(std::forward<Args>(args)...);
    }

};

template<typename T, typename ...Args>
std::unique_ptr<T> make(const std::string& name, Args&& ...args)
{
    auto fac = factory<T>::get_instance();
    return fac.create(name, std::forward<Args>(args)...);
}

}
}

#endif //DESIGN_PATTERN_FACTORY_HPP
