#ifndef PATTERNS_ABSTRACT_FACTORY_HPP
#define PATTERNS_ABSTRACT_FACTORY_HPP

#include "factory_base.hpp"

namespace design_patterns {
namespace creational {


/**
 * Abstract Type
 *
 * @tparam _AbstractType
 */
template <class _AbstractType>
class abstract_type
{
public:
    using abstract_type__ = _AbstractType;

    abstract_type() = default;
    virtual ~abstract_type() = default;

};

/**
 * Base Factory Class
 * @tparam _AbstractType
 */
template <class _AbstractType>
class base_factory{
public:
    const std::string name;

    explicit base_factory(const std::string& name): name(name) {
        static_assert(std::is_polymorphic<_AbstractType>::value,
            "base_factory:: _AbstractType must be an abstract class");
        static_assert(std::is_base_of<
                        abstract_type<_AbstractType>, _AbstractType>::value,
                "base_factory::()"
                        "_AbstractType must be derived from abstract_type");
    }

    virtual ~base_factory() {
        auto deleted = clear();
        std::cout << "Destroying Factory "
                  << name << " "
                  << deleted << std::endl;
    };

    template <typename... _Args>
    std::unique_ptr<_AbstractType> create(const std::string& name,_Args&& ...args)
    {
        std::lock_guard<std::mutex> lock(map_holder<_AbstractType, _Args ...>::mtx);
        try {
            const std::string prefixed_name = this->name + "_" + name;
            auto el = map_holder<_AbstractType, _Args ...>::functions.at(prefixed_name);
            return std::move(el(std::forward<_Args>(args)...));
        }
        catch (std::exception& ex) {
            auto args_str = print_args_types<_Args...>();
            throw factory_create_exception(name, args_str);
        }
    }

    /**
     * Remove factory methods in map holder that belong to the current
     * factory
     * @return  Number of removed factory methods
     */
    int clear()
    {
        std::lock_guard<std::mutex> lock(map_holder<_AbstractType>::mtx);
        int count = 0;
        for (auto it = map_holder<_AbstractType>::clear_callbacks.begin();
             it!=map_holder<_AbstractType>::clear_callbacks.end(); )
        {
            if(std::get<0>(*it)==name) {
                std::get<1>(*it)();
                it = map_holder<_AbstractType>::clear_callbacks.erase(it);
                count ++;
            }else{
                it++;
            }
        }
        return count;
    }

};

/**
 * Factory Class
 * @tparam _BaseType
 * @tparam _AbstractType
 */
template <class _BaseType, class _AbstractType = typename _BaseType::abstract_type__>
class factory : public base_factory<_AbstractType> {
public:
    typedef factory<_BaseType, _AbstractType> factory_type;

    explicit factory(const std::string& name): base_factory<_AbstractType>(name)
    {
        static_assert(std::is_base_of<_AbstractType, _BaseType>::value,
                "factory::()"
                        "_BaseType must be derived from _AbstractType");
    };
    ~factory() = default;

    template<class _ConcreteType>
    constexpr static void assert_type()
    {
        static_assert(std::is_base_of<_BaseType, _ConcreteType>::value,
                "factory::()"
                        "_ConcreteType must be derived from _BaseType");
    }

    template<class _ConcreteType>
    void register_type(const std::string& name)
    {
        assert_type<_ConcreteType>();
        const std::string prefixed_name = this->name + "_" + name;
        std::lock_guard<std::mutex> lock(map_holder<_AbstractType>::mtx);
        if (map_holder<_AbstractType>::functions.count(prefixed_name)>0) {
            registration_error(name);
        }
        map_holder<_AbstractType>::functions[prefixed_name] =
                &create_unique<_AbstractType, _ConcreteType>;
        clear_callback_tuple clt = std::make_tuple(this->name, []() {
          map_holder<_AbstractType>::functions.clear();
        });
        map_holder<_AbstractType>::clear_callbacks.push_back(clt);
        print_registration_message<_ConcreteType>(name);
    };

    template<class _ConcreteType, typename _Arg0, typename... _Args>
    void register_type(const std::string& name)
    {
        assert_type<_ConcreteType>();
        const std::string prefixed_name = this->name + "_" + name;
        if (map_holder<_AbstractType, _Arg0, _Args...>::functions.count(prefixed_name)>0) {
            registration_error(name);
        }
        {
            std::lock_guard<std::mutex> lock(map_holder<_AbstractType, _Arg0,_Args...>::mtx);
            map_holder<_AbstractType, _Arg0, _Args...>::functions[prefixed_name] =
                    &create_unique<_AbstractType, _ConcreteType, _Arg0, _Args...>;
        }
        {
            // Register deleter
            clear_callback_tuple clt = std::make_tuple(this->name, []() {
              map_holder<_AbstractType, _Arg0, _Args...>::functions.clear();
            });
            std::lock_guard<std::mutex> lock_clear(map_holder<_AbstractType>::mtx);
            map_holder<_AbstractType>::clear_callbacks.push_back(clt);
        }
        print_registration_message<_ConcreteType, _Arg0, _Args...>(name);
    }
private:
    void registration_error(const std::string& name)
    {
        throw factory_exception(
                "factory<" +
                        demangle(typeid(_BaseType).name()) + "," +
                        demangle(typeid(_AbstractType).name()) + ">" +
                        ": [ERROR]: Type already registered: " +
                        KRED + name + RST);
    }

    template<class _ConcreteType, typename... _Args>
    void print_registration_message(const std::string& name)
    {
        auto args_str = print_args_types<_Args...>();
        std::cout << "[----------] [OK]: " << this->name << ": "
                  << FBLU(demangle(typeid(_ConcreteType).name()))
                  << FBLU(" (" << args_str << ")")
                  << FBLU(" < " << demangle(typeid(_BaseType).name()))
                  << " registered under name " << FGRN(name)
                  << std::endl;
    }

};


template <typename _AbstractType>
class abstract_factory {
public:

    template<typename ..._Args>
    std::unique_ptr<_AbstractType> create(const std::string& name,
                                          const std::string& family_name,
                                          _Args ...args)
    {
        return std::move(factories[name]->create(
                family_name, std::forward<_Args>(args)...));
    }

    template<class _FactoryType>
    void register_factory(const std::string& family_name,
                          std::unique_ptr<_FactoryType> factory_)
    {
        factories[family_name] = std::move(factory_);
        std::cout << "[----------] Registered factory: "
                  << family_name << std::endl;
    }

    /**
     * Register concrete factory in the abstract factory
     * @tparam _FactoryType  Concrete factory type
     * @param name           The registration name
     */
    template<class _FactoryType>
    void register_factory(const std::string& name)
    {
        register_factory(name, std::move(std::make_unique<_FactoryType>()));
    }
protected:
    abstract_factory() = default;
private:
    std::map<std::string, std::unique_ptr<base_factory<_AbstractType>>>factories;
};



}
}

#endif //PATTERNS_ABSTRACT_FACTORY_HPP
