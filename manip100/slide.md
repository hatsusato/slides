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
  std::cout << "Hello, World!" << std::endl;
}
```

```
*> Hello, World!
```

---
class: center, middle
layout: true

---

こんにちは

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
