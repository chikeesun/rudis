#ifndef RUDIS_TIMER_MANAGER_HPP
#define RUDIS_TIMER_MANAGER_HPP

#include <queue>
#include <vector>
#include <unordered_map>

#include "timer.hpp"

class TimerManager {
 public:
  TimerManager() {}

  Timer* addTimer(int expire, std::function<void(void)> cb,
                  void* args = nullptr);
  void delTimer(Timer* timer);
  uint64_t getNextExpire();
  void handleExpireTimers();

 private:
  struct TimerCmp {
    bool operator()(const Timer* a, const Timer* b) const {
      return a->getExpire() > b->getExpire();
    }
  };

  std::priority_queue<Timer*, std::vector<Timer*>, TimerCmp> m_timers;
  std::unordered_map<Timer*, bool> m_del_timers;
};

#endif