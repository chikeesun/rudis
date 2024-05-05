#include "timer.hpp"
#include "timer_manager.hpp"

TimerManager::addTimer(int expire, std::function<void(void)> cb, void* args) {
    Timer* timer = new Timer(expire, cb, args);
    m_timers.push(timer);
    return timer;
}