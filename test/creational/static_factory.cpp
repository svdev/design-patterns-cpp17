#include "gtest/gtest.h"
#include "creational/factory.hpp"

namespace dpc = design_patterns::creational;

struct my_base_class {};

struct my_derived_class : public my_base_class {
  my_derived_class() = default;
  explicit my_derived_class(int val1) { }
  explicit my_derived_class(float val1) { }
  explicit my_derived_class(float val1, int val2) { }
  explicit my_derived_class(double val1, int val2) { }
};

TEST(DessignPatternFactoryTest, FactoryRegistration)
{
    auto fac = dpc::static_factory<my_base_class>::get_instance(true);
    ASSERT_EQ(fac.registered(), 0);

    fac.register_type<my_derived_class>("my_derived_class");
    fac.register_type<my_derived_class, int>("my_derived_class");
    fac.register_type<my_derived_class, float>("my_derived_class");
    fac.register_type<my_derived_class, float, int>("my_derived_class");
    fac.register_type<my_derived_class, double, int>("my_derived_class");

    ASSERT_EQ(fac.registered("my_derived_class"), 5);
}

TEST(DessignPatternFactoryTest, FactoryGetEmpty)
{
    auto fac = dpc::static_factory<my_base_class>::get_instance(true);
    ASSERT_EQ(fac.registered(), 0);
}

TEST(DessignPatternFactoryTest, FactoryClear)
{
    auto fac = dpc::static_factory<my_base_class>::get_instance(true);
    ASSERT_EQ(fac.registered(), 0);

    fac.register_type<my_derived_class>("my_derived_class");
    fac.register_type<my_derived_class, int>("my_derived_class");
    fac.register_type<my_derived_class, float>("my_derived_class");
    fac.register_type<my_derived_class, float, int>("my_derived_class");
    fac.register_type<my_derived_class, double, int>("my_derived_class");

    ASSERT_EQ(fac.clear(), 5);
}

TEST(DessignPatternFactoryTest, FactoryCreate)
{
    auto fac = dpc::static_factory<my_base_class>::get_instance(true);
    ASSERT_EQ(fac.registered(), 0);

    fac.register_type<my_derived_class>("my_derived_class");
    fac.register_type<my_derived_class, int>("my_derived_class");
    fac.register_type<my_derived_class, float>("my_derived_class");
    fac.register_type<my_derived_class, float, int>("my_derived_class");
    fac.register_type<my_derived_class, double, int>("my_derived_class");

    int x = 1;
    float y = 2.3f;
    double z = 1.5;
    auto obj1 = fac.create("my_derived_class", y, 1);
    auto obj2 = fac.create("my_derived_class", 2.3f, x);
    auto obj3 = fac.create("my_derived_class", z, 1);
    auto obj4 = fac.create("my_derived_class", z, x);
    auto obj5 = fac.create("my_derived_class", z, 1);

    EXPECT_THROW({
        auto obj6 = fac.create("my_derived_class", z, "undefined");
    }, dpc::factory_create_exception);

}

TEST(DessignPatternFactoryTest, FactoryCreateException)
{
    auto fac = dpc::static_factory<my_base_class>::get_instance(true);
    ASSERT_EQ(fac.registered(), 0);

    EXPECT_THROW({
        auto obj = fac.create("undefined_type");
    }, dpc::factory_create_exception);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}