#ifndef PATTERNS_FACTORY_HPP
#define PATTERNS_FACTORY_HPP

#include <memory>
#include <map>
#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

#include "util/text.hpp"
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
    explicit factory_create_exception(const std::string& tag,
                                      const std::string& args) :
            factory_exception("Unknown type under name '" + tag + "' that "
                    "can match a constructor for arguments '(" + args + ")'"){};
};

template<class BaseType, class... Args>
using factory_method = std::unique_ptr<BaseType>(*)(Args&& ...);

typedef std::tuple<std::string, std::function<void()>> clear_callback_tuple;

template<class BaseType, class... Args>
struct map_holder {
  static std::map<std::string, factory_method<BaseType, Args...>> functions;
  static std::vector<clear_callback_tuple> clear_callbacks;
};

template<class BaseType, class... Args>
std::map<std::string, factory_method<BaseType, Args...>>map_holder<
        BaseType,
        Args...>::functions;

template<class BaseType, class... Args>
std::vector<clear_callback_tuple> map_holder<BaseType, Args...>::clear_callbacks;


template<class T>
class factory {
public:

    factory(const factory&) = default;
    void operator=(factory const&) = delete;

    static factory& get_instance(bool clear = false)
    {
        static factory instance;
        if (clear)
            instance.clear();
        return instance;
    }

    /**
     * Get number of registered constructors under a name
     * @tparam Args  Constructor signature types
     * @return       Number of registered classes in factory
     */
    static unsigned long registered(const std::string& name)
    {
        unsigned long counter = 0;
        for (auto it = map_holder<T>::clear_callbacks.begin();
             it!=map_holder<T>::clear_callbacks.end(); ++it)
        {
            if (std::get<0>(*it)==name)
                counter++;
        }
        return counter;
    }

    /**
     * Get all number of registered types in the factory
     * @tparam Args  Constructor signature types
     * @return       Number of registered types in factory
     */
    static unsigned long registered()
    {
        return map_holder<T>::clear_callbacks.size();
    }

    /**
     * Remove Registered factory methods
     * @return  Number of removed factory methods
     */
    static int clear()
    {
        int count = 0;
        for (auto it = map_holder<T>::clear_callbacks.begin();
             it!=map_holder<T>::clear_callbacks.end(); )
        {
            std::get<1>(*it)();
            it = map_holder<T>::clear_callbacks.erase(it);
            count ++;
        }
        return count;
    }

    static const std::string name()
    {
        return demangle(typeid(factory<T>).name());
    }

    template<class TDerived, typename ...Args>
    static bool register_type(const std::string& name)
    {
        static_assert(std::is_base_of<T, TDerived>::value,
                "factory::register_type: "
                        "TDerived must be derived from T");

        if (map_holder<T, Args...>::functions.count(name)==0) {

            // Register factory method
            map_holder<T, Args...>::functions[name] =
                    &create_func<TDerived, Args...>;

            // Register factory method deleter
            clear_callback_tuple clt = std::make_tuple(name, []() {
                map_holder<T, Args...>::functions.clear();
            });
            map_holder<T>::clear_callbacks.push_back(clt);

            auto args_str = print_args_types<Args...>();
            std::cout << "[----------] [OK]: " << factory<T>::name() << ": "
                      << FBLU(demangle(typeid(TDerived).name()))
                      << FBLU(" (" << args_str << ")")
                      << " registered under name " << FGRN(name)
                      << std::endl;
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
    std::unique_ptr<T> create(const std::string& name, Args...args)
    {
        try {
            auto el = map_holder<T, Args ...>::functions.at(name);
            return el(std::forward<Args>(args)...);
        }
        catch (std::exception& ex) {
            auto args_str = print_args_types<Args...>();
            throw factory_create_exception(name, args_str);
        }
    }

private:
    factory() = default;

    template<class TDerived, typename ...Args>
    static std::unique_ptr<T> create_func(Args&& ...args)
    {
        return std::make_unique<TDerived>(std::forward<Args>(args)...);
    }

};

/**
 * Factory Method
 * @tparam _BaseType   Base Type
 * @tparam _Args       Factory Method constructor argument types
 * @param name         The name of the registered type within the factory
 * @param args         Factory Method arguments
 * @return             Instance of a registered factory type
 */
template<typename _BaseType, typename ..._Args>
std::unique_ptr<_BaseType> make(const std::string& name, _Args&& ...args)
{
    auto fac = factory<_BaseType>::get_instance();
    return fac.create(name, std::forward<_Args>(args)...);
}

}
}

#endif //PATTERNS_FACTORY_HPP
