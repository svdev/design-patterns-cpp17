#include "gtest/gtest.h"
#include "behavioral/observer.hpp"


namespace dpb = design_patterns::behavioral;

struct notification1 {};
struct notification2 {};

template <typename ...>
class my_observer_type: public dpb::observer<notification1, notification2> {};

class my_observer: public my_observer_type<> {
public:
    bool notified1 = false;
    bool notified2 = false;
    void handle(notification1& notification) override
    {
        std::cout << "[my_observer]: View notification 1" << std::endl;
        notified1 = true;
    }
    void handle(notification2& notification) override
    {
        std::cout << "[my_observer]: View notification 2" << std::endl;
        notified2 = true;
    }
};

class my_observable: public dpb::observable<my_observer> {};


TEST(DessignPatternObserverTest, Registration)
{
    auto o = dpb::make_observer<my_observer>();
    my_observable observable;
    observable.add_observer(o);
    observable.add_observer(o);
    ASSERT_EQ(observable.count_observers(), 1);

    observable.remove_observer(o);
    ASSERT_EQ(observable.count_observers(), 0);

}

TEST(DessignPatternObserverTest, Clear)
{
    auto o = dpb::make_observer<my_observer>();
    my_observable observable;
    observable.add_observer(o);
    ASSERT_EQ(observable.count_observers(), 1);
    observable.clear_observers();
    ASSERT_EQ(observable.count_observers(), 0);
}

TEST(DessignPatternObserverTest, NotifyWeakReference)
{
    notification1 n1;

    auto existing_observer = dpb::make_observer<my_observer>();
    my_observable some_observable;

    some_observable.add_observer(existing_observer);

    {
        auto expired_observer = dpb::make_observer<my_observer>();
        some_observable.add_observer(expired_observer);
    }

    ASSERT_EQ(some_observable.count_observers(), 2);
    some_observable.notify(n1);

    // When calling notify() and a weak observer reference is detected we should
    // remove it from the current list of observers
    ASSERT_EQ(some_observable.count_observers(), 1);
}

TEST(DessignPatternObserverTest, Notify)
{
    // Notifications
    notification1 n1;
    notification2 n2;

    // Observers
    auto observer1 = dpb::make_observer<my_observer>();
    auto observer2 = dpb::make_observer<my_observer>();

    // Observables
    my_observable observable1;

    observable1.add_observer(observer1);
    observable1.add_observer(observer2);

    observable1.notify(n1);
    observable1.notify(n2);

    ASSERT_TRUE(observer1->notified1);
    ASSERT_TRUE(observer2->notified2);

    ASSERT_TRUE(observer2->notified1);
    ASSERT_TRUE(observer2->notified2);

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}