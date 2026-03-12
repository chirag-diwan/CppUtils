#pragma once

#include <thread>
#include <type_traits>
template <typename T, typename... Args> void Dispatch(T functor, Args... args) {
  static_assert(std::is_invocable<T>(functor, args...),
                "Expected a functor , with valid arguments");
  std::is_same<T, std::string>::value;
  std::thread t(functor(args...));
  t.join();
}
