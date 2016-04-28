class: center, middle

# マニピュレータのしくみ

## hatsusato

### 2016/04/28

---
name: self-introduction

# 自己紹介

## hatsusato

- KMC 5回生のM1

- 理学部数学系 -> 情報学研究科通信情報システム専攻計算機ソフトウェア分野

- C++フリーク

- プログラミング言語を作りたい

- Twitter: [@hatsusato](https://twitter.com/hatsusato)

- GitHub: [hatsusato](https://github.com/hatsusato)

---
name: advertisement
layout: true

# 宣伝

---

## 終焉のC++2016

- C++を勉強するプロジェクト

- 毎週火曜20時から

- 予定

	- _Effective STL_

	- _Effective Modern C++_

- 既読

	- _Effective C++_

- 皆さんの参加をお待ちしています

???
プロコンなどでC++を使う機会がある人が対象

これからC++を勉強するという人はこのプロジェクトだけでは不十分

_Effective C++_ は昨年度の 終焉のC++2015 で読んだ
---

## C89コンパイラ

- https://github.com/kmc-jp/c89-compiler

- ノリと勢いでC89コンパイラを作る

	- コミックマーケットC90に出したい

- 手伝ってくれる仲間を絶賛募集中

???
C89には間に合わなかったので今度こそ完成させたい

開発メンバーが若くないので勢いが足りない

C言語がわかる人もわからない人も募集中
---
layout: false

# 目次

1. C++の入出力

2. マニピュレータとは

3. マニピュレータのしくみ

---
name: I/O in C++
layout: true

# 1. C++の入出力

---

- ストリーム: 入出力を抽象化した概念

- ストリーム操作の肝はシフト演算子オーバーロード

???
C++では入出力をストリームに対する操作として扱う

ストリームの抽象化により `stringstream` や `iostream` などを共通の構文で操作できる
---
template: I/O in C++
layout: true

## シフト演算子オーバーロード

---

```C++
int n, m;
std::cin >> n >> m;
std::cout << n << m;
```

- 入力: `operator>>`

- 出力: `operator<<`

- 視覚的にわかりやすい

- 以降では簡単のために `std::ostream` のみを扱う

---

```C++
int n, m;
std::cin.operator>>(n).operator>>(m);
std::cout.operator<<(n).operator<<(m);
```

- 入力: `operator>>`

- 出力: `operator<<`

- 視覚的にわかりやすい

- 以降では簡単のために `std::ostream` のみを扱う

???
シフト演算子は実際にはこのように呼び出されている

二項演算子でつなぐだけでメソッドチェインが書けるという発明
---
name: what's manipulator
layout: true

# 2. マニピュレータとは

---

- ストリームを制御する機能を持つオブジェクト

- 使い方: マニピュレータをストリームの入出力に混ぜるだけ

---

## マニピュレータの例
- `std::endl`
- `std::boolalpha`
- `std::setprecision`
- etc.

```C++
std::cout << true << std::endl << std::boolalpha
          << true << std::endl;
// 1
// true
std::cout << M_PI << std::endl << std::setprecision(16)
          << M_PI << std::endl;
// 3.14159
// 3.141592653589793
```
???
endl: 改行してフラッシュ

boolalpha: 真偽値を(0/1)ではなく(false/true)で出力

setprecision: 浮動小数点数の表示精度を指定
--

## .center[**どうしてそんなふうにうまくいくの？**]

---
name: mechanism
layout: true

# 3. マニピュレータのしくみ

---
template: mechanism
layout: true

`operator<<` のオーバーロードがどうなっているのか調べてみる

---

```C++
ostream& operator<<(ostream& os, char ch);
ostream& operator<<(ostream& os, const char* s);
ostream& operator<<(ostream& os, const string& str);
ostream& ostream::operator<<(short value);
ostream& ostream::operator<<(int value);
ostream& ostream::operator<<(long value);
ostream& ostream::operator<<(double value);
ostream& ostream::operator<<(bool value);
ostream& ostream::operator<<(const void* value);
ostream& ostream::operator<<(ostream& (*func)(ostream&));
etc...
```

???
`operator<<` と `ostream::operator<<` の違いは
`ostream`の本当の型 `basic_ostream<CharT, Traits>` の
`CharT` に依存するかどうか

```C++
template< class CharT, class Traits>
basic_ostream<CharT,Traits>&
operator<<(basic_ostream<CharT,Traits>& os, CharT ch);

template< class CharT, class Traits>
basic_ostream<CharT,Traits>&
operator<<(basic_ostream<CharT,Traits>& os, char ch);

template< class Traits >
basic_ostream<char,Traits>&
operator<<(basic_ostream<char,Traits>& os, char ch);

template< class Traits >
basic_ostream<char,Traits>&
operator<<(basic_ostream<char,Traits>& os, signed char ch);
```
---

```C++
ostream& operator<<(ostream& os, char ch);
ostream& operator<<(ostream& os, const char* s);
ostream& operator<<(ostream& os, const string& str);
ostream& ostream::operator<<(short value);
ostream& ostream::operator<<(int value);
ostream& ostream::operator<<(long value);
ostream& ostream::operator<<(double value);
ostream& ostream::operator<<(bool value);
ostream& ostream::operator<<(const void* value);
*ostream& ostream::operator<<(ostream& (*func)(ostream&));
etc...
```

ん？

---
template: mechanism
layout: true

---

```C++
ostream& ostream::operator<<(ostream& (*func)(ostream&));
```

- `operator<<` のオーバーロードには関数ポインタを受け取るものがある

- このオーバーロードがマニピュレータを処理している

???
`std::ostream&` を受け取って `std::ostream&` を返す関数ポインタ
---

## 実装例

```C++
namespace std {

ostream& ostream::operator<<(ostream& (*func)(ostream&)) {
  return func(os);
}

ostream& endl(ostream& os) {
  return os << '\n' << flush;
}

}  // namespace std

int main() {
  std::cout << 42 << std::endl;
}
```

これでもう `std::endl` みたいなマニピュレータは自分で作れますね！

---

## 疑問

- `std::endl` の型は `ostream& (*)(ostream&)`

- `std::setprecision` の型は？

	- 引数をとるマニピュレータは関数呼び出しの戻り値がストリームに流れる

	- 戻り値の型が  `ostream& (*)(ostream&)` なのか？

	- どういうしくみ？

```C++
std::cout << std::setprecision(16) << M_PI << std::endl;
```

???
疑問はマニピュレータの作り方で解消
---
