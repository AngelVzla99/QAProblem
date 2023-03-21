#pragma once 
#include <iostream>
#include <future>
#include <chrono>
#include <thread>
#include <atomic>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
  #include <windows.h>
  #include <sysinfoapi.h>
#else
  #include <unistd.h>
  #include <time.h>
#endif

using namespace std;

void multi_platform_sleep(unsigned long long time);
double measureTime(function<void()> f);

int kill_after_timeout(int seconds, int ite, function<void()> callback);