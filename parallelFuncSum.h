#pragma once
#include <functional>
#include <vector>
#include <thread>

template<typename TIt, typename T>
T parallel_func_sum(TIt i_begin, TIt i_end, std::function<T(T)>& i_func)
{
  int values_count = i_end - i_begin;
  int thread_count = std::thread::hardware_concurrency();
  std::vector<T> thread_sum(thread_count, 0);
  std::vector<std::thread> threads;
  int values_for_thread_count = values_count / thread_count;

  for (int i = 0; i < thread_count - 1; i++)
  {
    threads.emplace_back(std::thread(
    [&i_begin,i, &i_func, &thread_sum, &values_for_thread_count]
    {
      int step = i * values_for_thread_count;
      thread_sum[i] = func_sum(i_begin + step, i_begin + step + 1, i_func);
    });
  }
  threads.emplace_back(std::thread(
    [&i_begin, &i_end, &thread_count, &i_func, &thread_sum, &values_for_thread_count]
  {
    int step = (thread_count - 1) * values_for_thread_count;
    thread_sum[i] = func_sum(i_begin + step, i_end, i_func);
  });
  T sum{};
  for (int i = 0; i < thread_count; i++)
  {
    sum += thread_sum[i];
  }
  return sum;
}

template<typename TIt, typename T>
T func_sum(TIt& i_begin, TIt& i_end, std::function<T(T)>& i_func)
{
  T sum{};
  for (TIt it = i_begin; it != i_end; it++)
  {
    sum += i_func(*i_begin);
  }
  return sum;
}