#ifndef PATTERNS_OBSERVER_HPP
#define PATTERNS_OBSERVER_HPP

#include <iostream>
#include <memory>
#include <vector>

namespace design_patterns {
namespace behavioral {

template<typename... _NotificationTypes>
class observer;

template<typename _NotificationType>
class observer<_NotificationType> {
public:
    virtual void handle(_NotificationType& notification) = 0;
};

template<typename _N, typename... _NotificationTypes>
class observer<_N, _NotificationTypes...> : public observer<_NotificationTypes...>{
public:
    using observer<_NotificationTypes...>::handle;
    virtual void handle(_N& notification) = 0;
};

template <typename _TObserver>
class observable {
public:
    typedef std::weak_ptr<_TObserver> observer_weak_ptr;

    ~observable() {}

    /**
     * Add an observer to the list.
     * @param observer
     */
    void add_observer(std::shared_ptr<_TObserver>& observer)
    {
        for (const auto& o : observers)
            if (o.lock() == observer)
                return;
        observer_weak_ptr wo(observer);
        observers.push_back(wo);
    }

    /**
     * Remove a particular observer from the list.
     * @param observer
     */
    void remove_observer(const observer_weak_ptr& observer)
    {
        for(auto it = observers.begin(); it!=observers.end(); ++it) {
            auto ptr = (*it).lock();
            if (ptr == observer.lock()) {
                observers.erase(it);
                return;
            }
        }
    }

    /**
     * Clear all observers from the list.
     */
    void clear_observers() { observers.clear(); }

    /**
     * Returns the number of observers.
     * @return
     */
    std::size_t count_observers() const { return observers.size(); }

    /**
     * Notify all of the observers, sending them the notification.
     * TNotification is the notification type.
     * @tparam _TNotification
     * @param notification
     */
    template<typename _NotificationType>
    void notify(_NotificationType& notification)
    {
        auto it = observers.begin();
        while(it!=observers.end()) {
            auto ptr = (*it).lock();
            if(ptr) {
                ptr->handle(notification);
                it++;
            } else {
                it = observers.erase(it);
            }
        }
    }

private:
    /// The list of observers.
    std::vector<observer_weak_ptr> observers;

};

/**
 * Observer factory method
 * @tparam T      Observer type
 * @tparam Args   Observer constructor arguments types
 * @param args    Observer arguments
 * @return        Observer instance
 */
template <typename T, typename... Args>
static std::shared_ptr<T> make_observer(Args&& ...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}
}
#endif // PATTERNS_OBSERVER_HPP
