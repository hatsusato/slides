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

// require C++14 (std::index_sequence, std::make_index_sequence)

template <typename RetType, typename Tuple, size_t... Ns>
RetType tail_impl(const Tuple& t,
                  std::index_sequence<Ns...>) {
  return std::make_tuple(std::get<Ns + 1u>(t)...);
}
template <typename Head, typename... Ts>
std::tuple<Ts...> tail(const std::tuple<Head, Ts...>& t) {
  return tail_impl<std::tuple<Ts...>>(
      t, std::make_index_sequence<sizeof...(Ts)>());
}

void comma_separate(std::ostream&, const std::tuple<>&) {}
template <typename T>
void comma_separate(std::ostream& os,
                    const std::tuple<T>& t) {
  os << std::get<0>(t);
}
template <typename U, typename V, typename... Ts>
void comma_separate(std::ostream& os,
                    const std::tuple<U, V, Ts...>& t) {
  os << std::get<0>(t) << ", ";
  comma_separate(os, tail(t));
}

template <typename... Args>
Manipulator separate_print(const Args&... args) {
  const auto t = std::make_tuple(args...);
  return Manipulator([t](std::ostream& os) {
      comma_separate(os, t);
    });
}

template <typename T>
Manipulator paren(const T& v) {
  return Manipulator([v](std::ostream& os) {
      os << '(' << v << ')';
    });
}

template <typename... Args>
Manipulator tuple_print(const Args&... args) {
  const auto m = paren(separate_print(args...));
  return Manipulator([m](std::ostream& os) {
      os << m;
    });
}

int main() {
  std::cout << tuple_print(42, 3.14, "abc") << std::endl;
}
