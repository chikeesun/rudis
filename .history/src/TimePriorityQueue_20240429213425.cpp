#include "timer.hpp"
#include "timer_manager.hpp"

#include <sys/time.h>

TimerManager::addTimer(int expire, std::function<void(void)> cb, void* args) {
    if(expire <= 0) {
        return nullptr;
    }
    uint64_t now = getCurrentMillisecs();

    Timer* timer = new Timer(expire, cb, args);
    m_timers.push(timer);
    return timer;
}