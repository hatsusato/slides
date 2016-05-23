#include <functional>
#include <iostream>

class Manipulator {
 public:
  using FuncType = std::function<void(std::ostream&)>;
  explicit Manipulator(const FuncType& f) : f_(f) {}
  void exec(std::ostream& os) const { f_(os); }
 private:
  FuncType f_;
};

std::ostream& operator<<(std::ostream& os, const Manipulator& m) {
  m.exec(os);
  return os;
}

Manipulator newlines(int n) {
  return Manipulator([n](std::ostream& os) {
      for (int i = 0; i < n; ++i) {
        os << std::endl;
      }
    });
}

int main() {
  std::cout << newlines(4) << newlines(2);
}
