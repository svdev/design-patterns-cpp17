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
    my_singleton a;
    my_singleton b;

    ASSERT_STREQ(a->instance_addr().c_str(), b->instance_addr().c_str());
    ASSERT_STREQ(my_singleton::get().instance_addr().c_str(),
                 a->instance_addr().c_str());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}