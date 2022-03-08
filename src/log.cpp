#include "log.hpp"

#include "event.hpp"

namespace logger
{
    static EventBus<Entry> log_bus;

    void publish(const Entry& entry)
    {
        log_bus.publish(entry);
    }

    void subscribe(std::function<void(const Entry&)> callback)
    {
        log_bus.subscribe(callback);
    }

    void drain()
    {
        log_bus.drain();
    }

} // namespace logger
