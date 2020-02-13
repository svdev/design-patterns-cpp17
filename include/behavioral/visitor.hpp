#ifndef PATTERNS_VISITOR_HPP
#define PATTERNS_VISITOR_HPP

#include <memory>
#include "util/text.hpp"

namespace design_patterns {
namespace behavioral {


template<typename... Types>
class visitor;

template<typename _VisitableType>
class visitor<_VisitableType> {
public:
    virtual void visit(_VisitableType & visitable) = 0;
    const std::string name() const {
        return demangle(typeid(*this).name());
    }
};

template<typename _VisitableType, typename... _VisitableTypes>
class visitor<_VisitableType, _VisitableTypes...> : public visitor<_VisitableTypes...> {
public:
    using visitor<_VisitableTypes...>::visit;
    virtual void visit(_VisitableType & visitable) = 0;
};

template<typename Derived, typename... Types>
class visitable {
public:
    typedef visitor<Types...>& visitor_type;
    virtual void accept(visitor_type visitor) {
        visitor.visit(static_cast<Derived&>(*this));
    }
};

/**
template<typename Derived, typename... Types>
class VisitableImpl : public Visitable<Types...> {
public:
    typedef Visitor<Types...>& VisitorType;
    virtual void accept(Visitor<Types...>& visitor) {
        visitor.visit(static_cast<Derived&>(*this));
    }
};
*/

template <typename _TDerived, typename... _Args>
static std::shared_ptr<_TDerived> make_visitor(_Args&& ...args)
{
    return std::make_shared<_TDerived>(std::forward<_Args>(args)...);
}

}
}

#endif //PATTERNS_VISITOR_HPP
