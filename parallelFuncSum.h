#pragma once

#include <functional>
#include <vector>
#include <thread>
#include <iostream>


template<typename TIt, typename T>
T func_sum(TIt i_begin, TIt i_end, std::function<T(T)>& i_func)
{
  T sum{};
  std::cout << *i_begin << " -> " << *i_end << std::endl;
  for (TIt it = i_begin; it != i_end; it++)
  {
    sum += i_func(*it);
  }
  return sum;
}


template<typename TIt, typename T>
T parallel_func_sum(TIt i_begin, TIt i_end, std::function<T(T)>& i_func)
{
  size_t values_count = i_end - i_begin;
  size_t thread_count = std::min(static_cast<size_t>(std::thread::hardware_concurrency()), values_count);
  std::vector<T> thread_sum(thread_count, 0);
  std::vector<std::thread> threads;
  size_t values_for_thread_count = values_count / thread_count;
  std::cout << "Threads count: " << thread_count << std::endl;

  // running threads
  for (size_t i = 0; i < thread_count - 1; i++)
  {
    threads.emplace_back(
    [&i_begin, i, &i_func, &thread_sum, &values_for_thread_count]
    {
      size_t step = i * values_for_thread_count;
      thread_sum[i] = func_sum(i_begin + step, i_begin + step + values_for_thread_count, i_func);
    });
  }
  threads.emplace_back(
    [&i_begin, &i_end, &thread_count, &i_func, &thread_sum, &values_for_thread_count]
  {
    size_t step = (thread_count - 1) * values_for_thread_count;
    thread_sum[thread_count - 1] = func_sum(i_begin + step, i_end, i_func);
  });

  // storing result of computation
  T sum{};
  for (size_t i = 0; i < thread_count; i++)
  {
      threads[i].join();
  }
  for (size_t i = 0; i < thread_count; i++)
  {
    sum += thread_sum[i];
  }

  return sum;
}
