#include "gtest/gtest.h"
#include "creational/factory.hpp"

namespace dpc = design_patterns::creational;

template <typename T, typename U>
void assert_equal_types(const std::unique_ptr<U>& v){
    ASSERT_EQ(typeid(*v), typeid(T));
}

// ###############################
// ABSTRACT TYPE
// ###############################
class furniture_item : public dpc::abstract_type<furniture_item> {};

// ###############################
// FAMILY TYPES
// ###############################
class chair : public furniture_item {};
class sofa : public furniture_item {};
class table : public furniture_item {};

// ###############################
// CONCRETE TYPES
// ###############################
class fancy_chair : public chair {
public:
    fancy_chair() {
        std::cout << "Chair: Fancy()" << std::endl;
    };
    fancy_chair(int val) {
        std::cout << "Chair: Fancy(int)" << std::endl;
    }
};
class old_chair : public chair {};

class big_sofa : public sofa {
public:
    big_sofa() {
        std::cout << "Sofa: big_sofa()" << std::endl;
    }
};
class small_sofa : public sofa {};

class old_table: public table {};
class new_table: public table {};


// ###############################
// CONCRETE FACTORIES
// ###############################
class chair_factory : public dpc::factory<chair>
{
public:
    chair_factory(): factory_type("chair")  {
        register_type<fancy_chair>("fancy");
        register_type<fancy_chair, int>("fancy");
        register_type<old_chair>("old");
    }
};

class sofa_factory : public dpc::factory<sofa>
{
public:
    sofa_factory(): factory_type("sofa") {
        register_type<big_sofa>("big");
        register_type<small_sofa>("small");
    }
};
class table_factory : public dpc::factory<table>
{
public:
    table_factory(): factory_type("table") {
        register_type<old_table>("old");
        register_type<new_table>("new");
    }
};

// ###############################
// ABSTRACT FACTORY
// ###############################
class furniture_factory : public dpc::abstract_factory<furniture_item> {
public:
    furniture_factory() {
        register_factory<chair_factory>("chair");
        register_factory<sofa_factory>("sofa");
        register_factory<table_factory>("table");
    }
};


TEST(DessignPatternAbstractFactoryTest, AbstractFactoryCreate)
{
    furniture_factory ff;
    auto chair1 = ff.create("chair", "fancy");
    assert_equal_types<fancy_chair>(chair1);

    int arg0 = 1;
    std::unique_ptr<furniture_item> chair2 = ff.create("chair", "fancy", arg0);
    assert_equal_types<fancy_chair>(chair2);

    std::unique_ptr<furniture_item> sofa1 = ff.create("sofa", "big");
    assert_equal_types<big_sofa>(sofa1);
}

TEST(DessignPatternAbstractFactoryTest, AlreadyRegistered)
{
    table_factory tf;
    tf.register_type<old_table>("some_old_table");
    EXPECT_THROW({
        tf.register_type<old_table>("some_old_table");
    }, dpc::factory_exception);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}