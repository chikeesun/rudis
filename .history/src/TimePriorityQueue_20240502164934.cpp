#include <sys/time.h>

#include "timer.hpp"
#include "timer_manager.hpp"

uint64_t getCurrentMillisecs(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

Timer* TimerManager::addTimer(int expire, std::function<void(void)> cb,
                              void* args)
{
    if (expire <= 0)
    {
        return nullptr;
    }
    uint64_t now = getCurrentMillisecs();

    Timer* timer = new Timer(now + expire, cb, args);
    m_timers.push(timer);
    m_del_timers[timer] = false;
    return timer;
}

void TimerManager::delTimer(Timer* timer)
{
    if (!q.empty() && q.top() == timer)
    {
        q.pop();
        delete timer;
        return;
    }
    m_del_timers[timer] = true;
}

void TimerManager::getNextExpire()
{
    uint64_t expire = -1;
    if (q.empty())
        return expire;
    uint64_t now = getCurrentMillisecs();
    if (q.top()->getExpire() > now)
        return q.top()->getExpire() - now;
    return 0;
}

void TimerManager::handleExpireTimers()
{
    uint64_t now = getCurrentMillisecs();
    while (!q.empty() && q.top()->getExpire() <= now)
    {
        Timer* timer = q.top();
        q.pop();
        if (timer == m_del_timers.back())
        {
            m_del_timers.pop_back();
            delete timer;
            continue;
        }
        timer->activate();
        delete timer;
    }
}
