#ifndef RUDIS_TIMER_MANAGER_HPP
#define RUDIS_TIMER_MANAGER_HPP

#include <vector>

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
  std::vector<Timer*> m_timers;
};

#endif