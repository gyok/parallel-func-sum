#include "parallelFuncSum.h"
#include <random>
#include <iostream>
#include <algorithm>
#include <chrono>

double Duplicate(double i)
{
  return i;
}


void FillVector(std::vector<double>& v)
{
  std::random_device rd{};
  std::mt19937 mersenne_engine{rd()};
  std::normal_distribution<> normal_distribution{5., 2.};
  auto gen = [&mersenne_engine, &normal_distribution]()
  {
      return normal_distribution(mersenne_engine);
  };

  std::generate(v.begin(), v.end(), gen);
}


int main()
{
  const size_t vecSize = 16'000'000;
  std::vector<double> v(vecSize);
  FillVector(v);
  // std::cout << "Generated vector:" << std::endl;
  // for (auto elem : v) std::cout << elem << ", ";
  // std::cout << std:: endl << std:: endl;

  std::function<double(double)> Func = &Duplicate;


  auto start = std::chrono::high_resolution_clock().now();

  auto sum = func_sum(v.begin(), v.end(), Func);

  auto finish = std::chrono::high_resolution_clock().now();
  auto duration = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());


  start = std::chrono::high_resolution_clock().now();

  auto sum_parallel = parallel_func_sum(v.begin(), v.end(), Func);

  finish = std::chrono::high_resolution_clock().now();
  auto duration_parallel = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count());


  std::cout << sum << "\t"
            << sum_parallel
            << (sum == sum_parallel ? " (equal)" : " (not equal)") << std::endl
            << duration << "\t"
            << duration_parallel << " "
            << "(" << duration_parallel / duration << ")" << std::endl;

  std::cin.get();

  return 0;
}
