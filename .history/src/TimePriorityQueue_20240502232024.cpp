/*
 * @Author: Chikee royallor@163.com
 * @Date: 2024-04-29 21:30:13
 * @LastEditors: Chikee royallor@163.com
 * @LastEditTime: 2024-05-02 23:16:03
 * @FilePath: /codecrafters-redis-cpp/src/TimePriorityQueue.cpp
 * @Copyright (c) 2024 by Chikee Sun. All rights reserved.
 * The reproduction, distribution and utilization of this file as
 * well as the communication of its contents to others without express
 * authorization is prohibited. Offenders will be held liable for the
 * payment of damages. All rights reserved in the event of the grant
 * of a patent, utility model or design. 
 */
#include <time.h>

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
    m_timers.insert(timer);
    return timer;
}

void TimerManager::delTimer(Timer* timer)
{
    if(m_timers.contains(timer)){
        m_timers.erase(timer);
        delete timer;
    }
}

uint64_t TimerManager::getNextExpire()
{
    uint64_t expire = -1;
    if (m_timers.empty())
        return expire;
    uint64_t now = getCurrentMillisecs();
    if(*m_timers.begin().getExpire() > now){
        return *m_timers.begin().getExpire() - now;
    }
    return 0;
}

void TimerManager::handleExpireTimers()
{
    uint64_t now = getCurrentMillisecs();
    while (!m_timers.empty() && m_timers.begin()->getExpire() <= now)
    {
        Timer* timer = *m_timers.begin();
        m_timers.erase(m_timers.begin());
        timer->activate();
        delete timer;
    }
}