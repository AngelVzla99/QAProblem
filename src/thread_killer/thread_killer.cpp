#include "thread_killer.hpp"

atomic<int> is_finished = {0};
atomic<int> iteration = {0};

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
void multi_platform_sleep(unsigned long long time){
  Sleep(time);
}
double measureTime(function<void()> f){
  long long int begin = GetTickCount64();
  f(); 
  long long int end = GetTickCount64();
  double elapsed = (end - begin);
  return elapsed;
}
#else

void multi_platform_sleep(unsigned long long time){
  sleep(time);
}

double measureTime(function<void()> f){
  struct timespec begin, end; 
  clock_gettime(CLOCK_REALTIME, &begin);
  f();
  clock_gettime(CLOCK_REALTIME, &end);
  long seconds = end.tv_sec - begin.tv_sec;
  long nanoseconds = end.tv_nsec - begin.tv_nsec;
  double elapsed = seconds * 1000  + nanoseconds*1e-6;
  return elapsed;
}

#endif

int kill_after_timeout(int seconds, int ite, function<void()> callback){
  iteration.store(ite,memory_order_relaxed);
  
  is_finished.store(0,memory_order_relaxed);
  thread first ([callback,is_finished,ite](){
    callback(); 
    if( iteration.load(memory_order_relaxed) == ite )
      is_finished.store(1,memory_order_relaxed); 
    return;
  });

  thread second ([callback,is_finished,seconds,ite](){
    multi_platform_sleep(seconds);
    if (is_finished.load(memory_order_relaxed) == 0 && iteration.load(memory_order_relaxed) == ite )
      is_finished.store(-1,memory_order_relaxed);
    return;
  });

  int finish_code = 0;
  while (true){
    multi_platform_sleep(1);
    finish_code = is_finished.load(memory_order_relaxed);
    if (finish_code != 0)
      break;
  }

  first.detach();
  second.detach();

  return finish_code;
  //terminate();
}