#include "gtest/gtest.h"
#include "creational/singleton.hpp"

namespace dpc = design_patterns::creational;


class my_singleton : public dpc::singleton<my_singleton> {
public:
    std::string instance_addr()
    {
        std::ostringstream os;
        os << "My address " << &(*this) << std::endl;
        return os.str();
    }
};


TEST(DessignPatternSingletonTest, SingletonInstantiation)
{
    auto a = my_singleton::get();
    my_singleton b;

    ASSERT_FALSE(std::addressof(a) == std::addressof(b));
    ASSERT_TRUE(std::addressof(*a) == std::addressof(*b));
    ASSERT_TRUE(std::addressof(a.get()) == std::addressof(b.get()));
    ASSERT_STREQ(a->instance_addr().c_str(), b->instance_addr().c_str());
}

TEST(DessignPatternSingletonTest, SingletonEquality)
{
    my_singleton a;
    my_singleton b;

    ASSERT_EQ(a, b);
}

TEST(DessignPatternSingletonTest, SingletonNotEquality)
{
    my_singleton a;

    class my_class {};
    my_class b;
    int c = 0;

    ASSERT_TRUE(a != b);
    ASSERT_TRUE(a != c);
}


TEST(DessignPatternSingletonTest, AcquireLockedInstance)
{
    my_singleton::lock lock1;
    EXPECT_THROW({
        my_singleton::lock lock2;
    }, dpc::singleton_exception);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}