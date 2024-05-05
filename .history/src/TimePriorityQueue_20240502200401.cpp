/*
 * @Author: Chikee royallor@163.com
 * @Date: 2024-04-29 21:30:13
 * @LastEditors: Chikee royallor@163.com
 * @LastEditTime: 2024-05-02 16:55:01
 * @FilePath: /codecrafters-redis-cpp/src/TimePriorityQueue.cpp
 * @Copyright (c) 2024 by Chikee Sun. All rights reserved.
 * The reproduction, distribution and utilization of this file as
 * well as the communication of its contents to others without express
 * authorization is prohibited. Offenders will be held liable for the
 * payment of damages. All rights reserved in the event of the grant
 * of a patent, utility model or design. 
 */
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
    while(m_del_timers.count(q.top()) && m_del_timers[q.top()])
    {
        m_del_timers.erase(q.top());
        delete q.top();
        q.pop();
    }
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