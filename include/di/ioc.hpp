#ifndef PATTERNS_IOC_HPP
#define PATTERNS_IOC_HPP

#include <memory>
#include <mutex>
#include <vector>

#include "exception.hpp"
#include "util/text.hpp"
#include "util/types.hpp"


namespace design_patterns {
namespace di {

#define IOC_MAX_RESOLVE_DEPTH 50


/// ioc exception
class ioc_exception : public design_pattern_exception {
public:
    explicit ioc_exception(const std::string& msg)
            :design_pattern_exception(msg) { };
};

/// Max ioc recursion exception
class ioc_max_recursion_exception : public ioc_exception {
public:
    explicit ioc_max_recursion_exception(const int max_depth)
            : ioc_exception("Max recursion depth reached (" +
                    std::to_string(max_depth) +
                    "). It is possible that you have a dependency cycle.") { };
};

class ioc_container {
public:

    /**
     * Pass in a shared pointer of class T and we will automatically deduce the
     * type using the typeid of T
     * @tparam T
     * @param obj
     */
    template<class T>
    void register_type(std::function<T*()> obj)
    {
        const std::type_info* type_id = &typeid(T);
        register_type(type_id->name(), obj);
    }

    /**
     * Pass in a shared pointer as well as a type id. This is useful for
     * registering a base class/interface alongside a derived class shared
     * pointer i.e. pass in &typeid(Base) and boost::shared_ptr<Derived>
     * @tparam T
     * @param type_id
     * @param obj
     */
    template<class T>
    void register_type(const std::type_info* type_id, std::function<T*()> obj)
    {
        if (!type_id)
            throw std::runtime_error("Invalid type id to register");
        register_type(type_id->name(), obj);
    }

    template<typename _Interface, typename _Derived = _Interface, typename..._Args>
    void register_type()
    {
        static_assert(std::is_base_of<_Interface, _Derived>::value,
                "ioc_container::() _Derived must be derived from _Interface");
        const std::type_info* type_id = &typeid(_Interface);
        auto factory_method = make_factory<_Derived, _Args...>();
        register_type(type_id->name(), factory_method);
    }

    /**
     * Pass in a shared pointer as well as a custom id. This can be useful if we
     * wish to use an interface more than once with different base classes
     * @tparam T
     * @param id
     * @param obj
     */

    template<class T>
    void register_type(const std::string& id, std::function<T*()> obj)
    {
        auto iter = m_map.find(id);
        if (iter==m_map.end()) {
            m_map[id] = obj;
            std::cout << "Registered raw TypeID="
                      << demangle(id.c_str())
                      << "("<< id << ")"
                      << std::endl;
        }
    }

    /**
     * Resolve by Type
     * @tparam T
     * @return
     */
    template<class T>
    T resolve()
    {
        std::lock_guard<std::mutex> lock(mtx);
        current_depth = 0;
        return resolve_internal<T>();
    }

    /**
     * Resolve by Type id name
     * @tparam T
     * @param id
     * @return
     */
    template<class T>
    T resolve(const std::string& id)
    {
        std::lock_guard<std::mutex> lock(mtx);
        current_depth = 0;
        return resolve_internal<T>(id);
    }

    ioc_container() = default;
    explicit ioc_container(int max_depth): max_depth(max_depth){};
private:
    typedef std::function<void*()> factory_method;

    std::map<std::string, std::function<void*()>> m_map;
    std::mutex mtx;
    const int max_depth = IOC_MAX_RESOLVE_DEPTH;
    int current_depth = 0;

    ioc_container(const ioc_container&) {};
    void operator=(const ioc_container&) {};

    /**
     * Check if we have reached our max recursion depth,
     * and otherwise we increase it.
     */
    void check_recursion_depth() {
        if (current_depth > max_depth)
            throw ioc_max_recursion_exception(max_depth);
        current_depth++;
    }

    template<class T>
    T* resolve_internal(const std::string& id)
    {
        std::cout << "Resolving for: " << id << std::endl;
        auto iter = m_map.find(id);
        if (iter!=m_map.end()) {
            return static_cast<T*>(iter->second());
        }
        throw std::runtime_error(
                "Could not locate type in IOC under name "+ id);
    }

    /**
     * Resolve shared pointers
     * @tparam T
     * @return
     */
    template<class T>
    typename std::enable_if<is_shared_ptr<T>::value==true,T>::type
    resolve_internal()
    {
        check_recursion_depth();
        typedef typename T::element_type t_type;
        const std::type_info* type_id = &typeid(t_type);
        t_type* o = resolve_internal<t_type>(type_id->name());
        std::cout << "Making shared " << type_id->name() << std::endl;
        return std::make_shared<t_type>(*o);
    }

    /**
     * Resolve unique pointers
     * @tparam T
     * @return
     */
    template<class T>
    typename std::enable_if<is_unique_ptr<T>::value==true,T>::type
    resolve_internal()
    {
        check_recursion_depth();
        typedef typename T::element_type t_type;
        const std::type_info* type_id = &typeid(t_type);
        t_type* o = resolve_internal<t_type>(type_id->name());
        std::cout << "Making unique " << type_id->name() << std::endl;
        return std::make_unique<t_type>(*o);
    }

    /**
     * Resolve Objects passed by value
     * @tparam T
     * @return
     */
    template<class T>
    typename std::enable_if<
            std::is_object<T>::value==true&&
                    is_shared_ptr<T>::value==false&&
                    is_unique_ptr<T>::value==false&&
                    std::is_pointer<T>::value==false,T>::type
    resolve_internal()
    {
        check_recursion_depth();
        const std::type_info* type_id = &typeid(T);
        T *o = resolve_internal<T>(type_id->name());
        return *o;
    }

    /**
     * Resolve Raw Pointers
     * @tparam T
     * @return
     */
    template<class T>
    typename std::enable_if<std::is_pointer<T>::value==true,T>::type
    resolve_internal()
    {
        check_recursion_depth();
        typedef typename std::remove_pointer<T>::type object_type;
        const std::type_info* type_id = &typeid(object_type);
        return resolve_internal<object_type>(type_id->name());
    }

    /**
     * Factory Method creator
     * @tparam T
     * @tparam Args
     * @return
     */
    template<class T, typename... Args>
    factory_method make_factory()
    {
        factory_method factory_fn = [&]() -> T* {
          std::cout << "Making "
                    << demangle(typeid(T).name())
                    << " ()"
                    << std::endl;
          T* obj;
          if constexpr((sizeof...(Args) > 0)) {
              obj = new T(resolve_internal<Args>()...);
          }else {
              obj = new T();
          }
          current_depth--;
          return obj;
        };
        return factory_fn;
    }
};

}
}

#endif // PATTERNS_IOC_HPP
