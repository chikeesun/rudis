#include <sys/time.h>

#include "timer.hpp"
#include "timer_manager.hpp"

uint64_t getCurrentMillisecs(void) {
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

Timer* TimerManager::addTimer(int expire, std::function<void(void)> cb,
                              void* args) {
  if (expire <= 0) {
    return nullptr;
  }
  uint64_t now = getCurrentMillisecs();

  Timer* timer = new Timer(now + expire, cb, args);
  m_timers.push(timer);
  return timer;
}

void TimerManager::delTimer(Timer* timer) {
  m_del_timers.push_back(timer);
  while (!q.empty() && q.top() == m_del_timers.back()) {
    q.pop();
    m_del_timers.pop_back();
    delete timer;
  }
}

void TimerManager::getNextExpire()
{
    
}
