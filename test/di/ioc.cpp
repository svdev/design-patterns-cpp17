#include "gtest/gtest.h"
#include "di/ioc.hpp"

namespace di = design_patterns::di;


class InterfaceA {
public:
    virtual ~InterfaceA() {};
};
class InterfaceB {
public:
    virtual ~InterfaceB() {};
};

class C {
public:
    C() = default;
    ~C() {
        std::cout << "Destroying C ()" << std::endl;
    }
};

class A: public InterfaceA {
public:
    A(C c, C *cc) {
        std::cout << "A (C c, C *cc)" << std::endl;
    }
    // cyclic dependency with InterfaceB
    A(C c, C *cc, std::shared_ptr<InterfaceB> b)
    {
        std::cout << "A (C c, C *cc, shared_ptr<InterfaceB> b)" << std::endl;
    }
    ~A() {
        std::cout << "Destroying A ()" << std::endl;
    }
};
class B: public InterfaceB {
public:
    B(std::shared_ptr<InterfaceA> a,
      std::unique_ptr<InterfaceA> b): a_shared(a), b_unique(std::move(b)) {
        std::cout << "B (shared_ptr<InterfaceA> a, unique_ptr<InterfaceA> b)"
                  << std::endl;
    }
    ~B(){
        std::cout << "Destroying B ()" << std::endl;
    }
private:
    std::shared_ptr<InterfaceA> a_shared;
    std::unique_ptr<InterfaceA> b_unique;
};


TEST(DessignPatternIOCTest, Register)
{
    di::ioc_container container;

    container.register_type<C>();
    container.register_type<InterfaceA, A, C, C*>();
    container.register_type<InterfaceB,
                            B,
                            std::shared_ptr<InterfaceA>,
                            std::unique_ptr<InterfaceA>>();

    auto resolved = container.resolve<InterfaceB>();
};

TEST(DessignPatternIOCTest, CyclicDependency)
{
    // create container allowing a max of resolution depth of 5
    di::ioc_container container(5);

    container.register_type<C>();
    // take constructor with cyclic dependency
    container.register_type<InterfaceA, A, C, C*, std::shared_ptr<InterfaceB>>();
    container.register_type<InterfaceB,
                            B,
                            std::shared_ptr<InterfaceA>,
                            std::unique_ptr<InterfaceA>>();
    EXPECT_THROW({
        auto resolved = container.resolve<InterfaceB>();
    }, di::ioc_max_recursion_exception);

}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

