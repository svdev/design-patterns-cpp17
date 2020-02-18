#ifndef PATTERNS_FACTORY_BASE_HPP
#define PATTERNS_FACTORY_BASE_HPP

#include <memory>
#include <map>
#include <tuple>
#include <iostream>
#include <mutex>
#include <typeinfo>
#include <cxxabi.h>

#include "util/text.hpp"
#include "util/color.hpp"
#include "exception.hpp"

namespace design_patterns {
namespace creational {

/// Base factory exception
class factory_exception : public design_pattern_exception {
public:
    explicit factory_exception(const std::string& msg)
            :design_pattern_exception(msg) { };
};

/// Factory create exception
class factory_create_exception : public factory_exception {
public:
    explicit factory_create_exception(const std::string& tag,
                                      const std::string& args)
            :
            factory_exception("Unknown type under name '"+tag+"' that "
                    "can match a constructor for arguments '("+args+")'") { };
};


/// Factory method unique pointer
template<class BaseType, class... Args>
using factory_method = std::unique_ptr<BaseType>(*)(Args&& ...);


/// Clear Callback Tuple
typedef std::tuple<std::string, std::function<void()>> clear_callback_tuple;

/**
 * Map Holder of factory methods
 * @tparam BaseType
 * @tparam Args
 */
template<class BaseType, class... Args>
struct map_holder {
  static std::map<std::string, factory_method<BaseType, Args...>> functions;
  static std::vector<clear_callback_tuple> clear_callbacks;
  static std::mutex mtx;
};

/**
 * Map Holder Functions Map
 * @tparam BaseType
 * @tparam Args
 */
template<class BaseType, class... Args>
std::map<std::string, factory_method<BaseType, Args...>>map_holder<
        BaseType,
        Args...>::functions;

/**
 * Clear Map Holder callbacks vector
 * @tparam BaseType
 * @tparam Args
 */
template<class BaseType, class... Args>
std::vector<clear_callback_tuple> map_holder<BaseType,
                                             Args...>::clear_callbacks;

/**
 * Map Holder Mutex
 * @tparam BaseType
 * @tparam Args
 */
template<class BaseType, class... Args>
std::mutex map_holder<BaseType, Args...>::mtx;


/**
* Create unique pointer for class TDerived < _BaseType
* @tparam _BaseType
* @tparam TDerived
* @tparam Args
* @param args
* @return
*/
template<class _BaseType, class TDerived, typename ...Args>
std::unique_ptr<_BaseType> create_unique(Args&& ...args)
{
    return std::make_unique<TDerived>(std::forward<Args>(args)...);
}

}
}

#endif //PATTERNS_FACTORY_BASE_HPP
