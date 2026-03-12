#pragma once

#include <cstdio>
#include <iostream>
#include <type_traits>
#include <unordered_map>
#include <vector>

#pragma once
#include <cstdio>
#include <iostream>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace Utils {

  template <typename T> struct is_vector : std::false_type {};
  template <typename T, typename Alloc>
  struct is_vector<std::vector<T, Alloc>> : std::true_type {};

  template <typename T> struct is_unordered_map : std::false_type {};
  template <typename K, typename V, typename Hash, typename KeyEqual,
            typename Alloc>
  struct is_unordered_map<std::unordered_map<K, V, Hash, KeyEqual, Alloc>>
      : std::true_type {};

  template <typename... T> void Printf(const char* Format, T... args) {
    printf(Format, args...);
  }

  template <typename T> void Print(T val) {
    if constexpr (std::is_arithmetic_v<T>) {
      std::cout << val;
    } else if constexpr (std::is_same_v<T, std::string>) {
      std::cout << '"' << val << '"';
    } else if constexpr (is_vector<T>::value) {
      std::cout << "[";
      bool first = true;
      for (const auto& item : val) {
        if (!first) std::cout << ", ";
        Print(item);
        first = false;
      }
      std::cout << "]";
    } else if constexpr (is_unordered_map<T>::value) {
      std::cout << "{";
      bool first = true;
      for (const auto& [k, v] : val) {
        if (!first) std::cout << ", ";
        Print(k);
        std::cout << ": ";
        Print(v);
        first = false;
      }
      std::cout << "}";
    }
  };

  enum LogLevel { INFO, WARN, ERROR };

  template <typename... T, LogLevel l>
  inline void Log(const char* msg, T... args) {
    if constexpr (l == INFO) {
      Print("[INFO]");
    } else if constexpr (l == WARN) {
      Print("[WARNING]");
    } else if constexpr (l == ERROR) {
      Print("[ERROR]");
    }
    Printf(msg, args...);
    Printf("\n");
  }
} // namespace Utils
