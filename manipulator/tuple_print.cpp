#include <functional>
#include <iostream>
#include <tuple>

class Manipulator {
 public:
  using FuncType = std::function<void(std::ostream&)>;
  Manipulator(const FuncType& f) : f_(f) {}
  void exec(std::ostream& os) const { f_(os); }
 private:
  FuncType f_;
};

std::ostream& operator<<(std::ostream& os, const Manipulator& m) {
  m.exec(os);
  return os;
}

void comma_separate(std::ostream&) {}
template <typename T>
void comma_separate(std::ostream& os, const T& v) {
  os << v;
}
template <typename T, typename U, typename... Rest>
void comma_separate(std::ostream& os, const T& t,
                    const U& u, const Rest&... rest) {
  os << first << ", ";
  comma_separate(os, second, rest...);
}

template <typename... Args>
Manipulator separate_print(const Args&... args) {
  return Manipulator([args...](std::ostream& os) {
      comma_separate(os, args...);
    });
}

template <typename T>
Manipulator paren_print(const T& v) {
  return Manipulator([v](std::ostream& os) {
      os << '(' << v << ')';
    });
}

template <typename... Args>
Manipulator tuple_print(const Args&... args) {
  return Manipulator([args...](std::ostream& os) {
      os << paren_print(separate_print(args...));
    });
}

int main() {
  std::cout << tuple_print(42, 3.14, "abc") << std::endl;
  std::cout << tuple_print() << std::endl;
}
