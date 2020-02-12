#ifndef DESIGN_PATTERN_VISITOR_HPP
#define DESIGN_PATTERN_VISITOR_HPP

#include <memory>

namespace design_patterns {
namespace behavioral {


template<typename... Types>
class visitor;

template<typename T>
class visitor<T> {
public:
    virtual void visit(T & visitable) = 0;
};


template<typename T, typename... Types>
class visitor<T, Types...> : public visitor<Types...> {
public:
    using visitor<Types...>::visit;
    virtual void visit(T & visitable) = 0;
};

template<typename Derived, typename... Types>
class visitable {
public:
    typedef visitor<Types...>& visitor_type;
    virtual void accept(visitor<Types...>& visitor) {
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

template <typename T, typename... Args>
static std::shared_ptr<T> make_visitor(Args&& ...args)
{
    //@TODO: Check T is derived from visitor<Types...>
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}
}

#endif //DESIGN_PATTERN_VISITOR_HPP
