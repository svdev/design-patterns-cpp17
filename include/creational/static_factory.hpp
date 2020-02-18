#ifndef PATTERNS_STATIC_FACTORY_HPP
#define PATTERNS_STATIC_FACTORY_HPP

#include "factory_base.hpp"

namespace design_patterns {
namespace creational {


/**
 * Static Factory
 * @tparam T
 */
template<class T>
class static_factory final {
public:
    static_factory(const static_factory&) = default;
    void operator=(static_factory const&) = delete;

    static static_factory& get_instance(bool clear = false)
    {
        static static_factory instance;
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
        std::lock_guard<std::mutex> lock(map_holder<T>::mtx);
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
        std::lock_guard<std::mutex> lock(map_holder<T>::mtx);
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
        return demangle(typeid(static_factory<T>).name());
    }

    template<class TDerived>
    constexpr static void assert_type()
    {
        static_assert(std::is_base_of<T, TDerived>::value,
                "static_factory::register_type: "
                        "TDerived must be derived from T");
    }


    template<class TDerived>
    static void register_type(const std::string& name)
    {
        assert_type<TDerived>();
        std::lock_guard<std::mutex> lock(map_holder<T>::mtx);

        if (map_holder<T>::functions.count(name)>0) {
            registration_error(name);
        }
        map_holder<T>::functions[name] =
                &create_unique<T, TDerived>;
        clear_callback_tuple clt = std::make_tuple(name, []() {
          map_holder<T>::functions.clear();
        });
        map_holder<T>::clear_callbacks.push_back(clt);

        std::cout << "[----------] [OK]: " << static_factory<T>::name() <<": "
                  << FBLU(demangle(typeid(TDerived).name()))
                  << FBLU(" ()")
                  << " registered under name " << FGRN(name)
                  << std::endl;
    };

    template<class TDerived, typename Arg0, typename ...Args>
    static void register_type(const std::string& name)
    {
        assert_type<TDerived>();

        if (map_holder<T, Arg0, Args...>::functions.count(name)>0) {
            registration_error(name);
        }
        {
            std::lock_guard<std::mutex> lock(map_holder<T, Arg0, Args...>::mtx);
            // Register static_factory method
            map_holder<T, Arg0, Args...>::functions[name] =
                    &create_unique<T, TDerived, Arg0, Args...>;
        }
        {
            // Register static_factory method deleter
            clear_callback_tuple clt = std::make_tuple(name, []() {
              map_holder<T, Arg0, Args...>::functions.clear();
            });
            std::lock_guard<std::mutex> lock(map_holder<T>::mtx);
            map_holder<T>::clear_callbacks.push_back(clt);
        }

        auto args_str = print_args_types<Arg0, Args...>();
        std::cout << "[----------] [OK]: " << static_factory<T>::name() <<": "
                  << FBLU(demangle(typeid(TDerived).name()))
                  << FBLU(" (" << args_str << ")")
                  << " registered under name " << FGRN(name)
                  << std::endl;
    }
    /**
     * Create instance by a registered name
     * @param name  The name of the registered class type
     * @return      An instance of the requested class name, or null if not
     *              found.
     */
    template<typename ...Args>
    static std::unique_ptr<T> create(const std::string& name, Args...args)
    {
        std::lock_guard<std::mutex> lock(map_holder<T, Args ...>::mtx);
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
    static_factory() = default;

    static void registration_error(const std::string& name)
    {
        throw factory_exception(
                "static_factory<" + static_factory<T>::name() + ">"+
                        ": [ERROR]: Type already registered: " + name);
    }
};


}
}

#endif //PATTERNS_STATIC_FACTORY_HPP
