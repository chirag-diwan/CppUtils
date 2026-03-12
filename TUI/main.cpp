#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>

#define NOTSELECTED "[ ] "
#define SELECTED "[X] "
#define TOPLEFT "\033[H"
#define HIDECURSOR "\033[?25l"
#define SHOWCURSOR "\033[?25h"
#define CLEAR "\033[2J"
#define WHITEBG "\033[47m\033[30m"
#define RESET "\033[0m"

struct termios orig_termios;

void disableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);
  struct termios raw = orig_termios;

  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();
  std::cout << CLEAR << HIDECURSOR;

  std::atomic<bool> running{true};
  std::mutex mtx;
  char key;

  std::vector<std::pair<std::string, bool>> Options = {
      {"Options1", false}, {"Options2", false}, {"Options3", false},
      {"Options4", false}, {"Options5", false}, {"Options6", false}};

  uint8_t Cursor = 0;

  std::thread t([&]() {
    while (running) {

      if (read(STDIN_FILENO, &key, 1) == 1) {
        std::lock_guard<std::mutex> lock(mtx);

        switch (key) {
        case 'j':
          if (Cursor < Options.size() - 1)
            Cursor++;
          break;

        case 'k':
          if (Cursor > 0)
            Cursor--;
          break;

        case 's':
          Options[Cursor].second = !Options[Cursor].second;
          break;

        case 'q':
          running = false;
          break;

        default:
          break;
        }
      }
    }
  });

  while (running) {
    {
      std::lock_guard<std::mutex> lock(mtx);

      std::cout << TOPLEFT;

      for (size_t i = 0; i < Options.size(); ++i) {
        if (i == Cursor)
          std::cout << WHITEBG;

        if (Options[i].second)
          std::cout << SELECTED;
        else
          std::cout << NOTSELECTED;

        std::cout << Options[i].first;

        if (i == Cursor)
          std::cout << RESET;

        std::cout << "\n";
      }
    }

    std::this_thread::sleep_for(std::chrono::nanoseconds(100));
  }

  t.join();
  std::cout << SHOWCURSOR << RESET << CLEAR << TOPLEFT;
  return 0;
}
