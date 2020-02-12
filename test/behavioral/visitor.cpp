#include "gtest/gtest.h"
#include "behavioral/visitor.hpp"


namespace dpb = design_patterns::behavioral;


// Forward declarations of visitables
class my_visitable_a;
class my_visitable_b;

/**
 * Base Visitable Definition
 * @tparam T  The Visitor Implementation class
 */
template <class T>
class base_visitable: public dpb::visitable<T, my_visitable_a, my_visitable_b> {};

/**
 * Base Visitor Definition
 */
template <typename ...>
class base_visitor: public dpb::visitor<my_visitable_a, my_visitable_b> {};

// #################################
// VISITABLES DEFINITION
// #################################

// Simple visitable
class my_visitable_a: public base_visitable<my_visitable_a>{};

// visitable With custom accept() override
class my_visitable_b: public base_visitable<my_visitable_b> {
public:
    int accepted = 0;
    void accept(visitable::visitor_type visitor) override
    {
        std::cout << "[my_visitable_b]: accept()" << std::endl;
        visitable::accept(visitor);
        accepted++;
    }
};

// #################################
// VISITORS DEFINITIONS
// #################################

// visitor
class my_visitor : public dpb::visitor<my_visitable_a, my_visitable_b> {
public:
    int visitedA = 0;
    int visitedB = 0;
    void visit(my_visitable_a& visitable) override
    {
        std::cout << "[my_visitor]: visiting my_visitable_a" << std::endl;
        visitedA++;
    }
    void visit(my_visitable_b& visitable) override
    {
        std::cout << "[my_visitor]: visiting my_visitable_b" << std::endl;
        visitedB++;
    }
};

// Other visitor
class other_visitor: public base_visitor<> {
public:
    void visit(my_visitable_a& visitable) override
    {
        std::cout << "[other_visitor]: visiting my_visitable_a" << std::endl;
    }
    void visit(my_visitable_b& visitable) override
    {
        std::cout << "[other_visitor]: visiting my_visitable_b" << std::endl;
    }
};

TEST(DessignPatternVisitorTest, VisitVisitables)
{
    my_visitor visitor;
    other_visitor other_visitor;

    my_visitable_a visitable_a;
    my_visitable_b visitable_b;

    visitable_a.accept(visitor);
    visitable_b.accept(visitor);

    ASSERT_EQ(visitable_b.accepted, 1);
    ASSERT_EQ(visitor.visitedA, 1);
    ASSERT_EQ(visitor.visitedB, 1);

    visitable_a.accept(other_visitor);
    visitable_b.accept(other_visitor);
}

TEST(DessignPatternVisitorTest, VisitorFactoryMethod)
{
    auto visitor = dpb::make_visitor<my_visitor>();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}