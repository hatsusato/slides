class: center, middle
layout: true

---

# .size100[マニピュレータを100倍活用する]

## hatsusato

### KMC春合宿2017

---
class: middle
layout: true

---

```C++
#include <iostream>
int main() {
  std::cout << "Hello, everyone!" << std::endl;
}
```

```
*> Hello, everyone!
```

---
class: center, middle
layout: true

---

C++のプロ

hatsusatoです

---

今回はC++の

マニピュレータを

---

めちゃくちゃ

活用する方法を

伝授します

---

突然ですが

C++を書いたこと

ある人　ノ

---

マニピュレータを

使ったこと

ある人　ノ

---

こういうやつ

つ `std::endl`

---

マニピュレータを

作ったこと

ある人　ノ

---

あまりいないと

思います

---

そこで

---

マニピュレータの

作り方を

マスターして

しまいましょう

---
class: none
layout: true

---

# Step 1

.size50[
`std::function<void(std::ostream&)>` に別名 (`Manip`) をつけましょう。
]
.footnote[
`Manip` は `std::ostream&` を受け取って、値を返さない関数オブジェクトの型です。
]

```C++
#include <functional>
#include <iostream>
using Manip = std::function<void(std::ostream&)>;
```

---

# Step 2

.size50[
`Manip` に対して `operator<<` を<br>オーバーロードしましょう。
]

```C++
std::ostream& operator<<(std::ostream& os, Manip m) {
  m(os);
  return os;
}
```

---

# Step 3

.size50[
`Manip` を返す関数を書きましょう。
]
.footnote[
`Manip` は `std::ostream&` を受け取って、値を返さない関数オブジェクトの型です。
]

```C++
Manip coord(double x, double y) {
  return [x, y](std::ostream& os) {
    os << '(' << x << ", " << y << ')';
  };
}
```

---

# Step 4

.size50[
できあがり！
]

```C++
int main() {
  std::cout << coord(1.2, 3.4) << std::endl;
}
```

```
*> (1.2, 3.4)
```

---
class: center, middle
layout: true

---

ね、簡単でしょう？

---
class: none
layout: true

# マニピュレータギャラリー

---

.size50[
ここで僕の考えたマニピュレータを紹介します
]

---

.size40[
`paren`
- 引数を丸かっこで囲んで出力する
]

```C++
template <typename T>
Manip paren(T&& x) {
  return [x = std::forward<T>(x)]
      (std::ostream& os) { os << '(' << x << ')'; };
}
```

---

.size40[
`join`
- 第1引数で区切って第2引数を順に出力する
]

```C++
template <typename S, typename T, typename U,
          typename... Args>
Manip join(S s, T&& t, U&& u, Args&&... args) {
  return [s = s, t = std::forward<T>(t),
          r = join(s, std::forward<U>(u),
                   std::forward<Args>(args)...)]
      (std::ostream& os){ os << t << s << r; };
}
```

---

.size40[
`tuple`
- 丸かっこで囲まれた中に、引数全体をカンマ区切りで出力する
]

```C++
template <typename... Args>
Manip tuple(Args&&... args) {
  return [t = join(", ", std::forward<Args>(args)...)]
      (std::ostream& os){ os << paren(t); };
}
```

---

.size50[
みなさんもよいマニピュレータを思いついたら教えてください。
]

---
class: center, middle
layout: false

ご清聴ありがとう

ございました
