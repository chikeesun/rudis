/*
 * @Author: Chikee royallor@163.com
 * @Date: 2024-05-02 21:17:21
 * @LastEditors: Chikee royallor@163.com
 * @LastEditTime: 2024-05-02 21:42:36
 * @FilePath: /codecrafters-redis-cpp/.history/inc/timer_manager_20240502211721.hpp
 * @Copyright (c) 2024 by Chikee Sun. All rights reserved.
 * The reproduction, distribution and utilization of this file as
 * well as the communication of its contents to others without express
 * authorization is prohibited. Offenders will be held liable for the
 * payment of damages. All rights reserved in the event of the grant
 * of a patent, utility model or design. 
 */
#ifndef RUDIS_TIMER_MANAGER_HPP
#define RUDIS_TIMER_MANAGER_HPP

#include <queue>
#include <vector>
#include <set>

#include "timer.hpp"

class TimerManager
{
public:
    TimerManager()
    {
    }

    Timer* addTimer(int expire, std::function<void(void)> cb,
                    void* args = nullptr);
    void delTimer(Timer* timer);
    uint64_t getNextExpire();
    void handleExpireTimers();

private:
    struct TimerCmp
    {
        bool operator()(const Timer* a, const Timer* b) const
        {
            return a->getExpire() < b->getExpire();
        }
    };
    std::set<Timer*, TimerCmp> m_timers;
};

#endif