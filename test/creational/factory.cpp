#include "gtest/gtest.h"
#include "creational/factory.hpp"

namespace dpc = design_patterns::creational;

struct my_class {};

TEST(DessignPatternFactoryTest, FactoryRegistration)
{
    auto fac = dpc::factory<my_class>::get_instance();
    fac.register_type<my_class>("my_class");
    ASSERT_EQ (fac.registered(), 1);
}

TEST(DessignPatternFactoryTest, FactoryCreation)
{
    auto fac1 = dpc::factory<my_class>::get_instance();

    fac1.register_type<my_class>("my_class");
    auto obj = fac1.create("my_class");
    ASSERT_EQ (fac1.registered(), 1);

    auto fac2 = dpc::factory<my_class>::get_instance();
    fac2.register_type<my_class>("my_other_class");

    auto obj2 = fac2.create("my_other_class");
    ASSERT_EQ (fac2.registered(), 2);
}

TEST(DessignPatternFactoryTest, FactoryCreateException)
{
    auto fac = dpc::factory<my_class>::get_instance();

    EXPECT_THROW({
        auto obj = fac.create("undefined_type");
    }, dpc::factory_create_exception);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}