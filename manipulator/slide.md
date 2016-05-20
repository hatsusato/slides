class: center, middle

# How to Manipulate Manipulators

## hatsusato

### 2016/05/23

---
name: self-introduction

# 自己紹介

## hatsusato
- KMC 5回生のM1

- 情報学研究科通信情報システム専攻計算機ソフトウェア分野
  - 理学部数学系出身

- C++フリーク
  ![enter](../enter.png)

- プログラミング言語を作りたい

- Twitter: [@hatsusato](https://twitter.com/hatsusato)

- GitHub: [hatsusato](https://github.com/hatsusato)

---

# 目次

.larger[
1. C++の入出力

2. マニピュレータとは

3. マニピュレータのしくみ
]
---
name: I/O in C++
layout: true

# 1. C++の入出力

---

## ストリーム

- 入出力をストリームという概念で抽象化する

  - ストリームに対する操作を介して任意の入出力を操作する

- シフト演算子オーバーロードによってストリーム操作を統一的に扱う

ストリームの例

```C++
iostream      // 標準入出力
fstream       // ファイル入出力
stringstream  // 文字列ストリーム
```

???
C++では入出力をストリームに対する操作として扱う

ストリームの抽象化により様々なストリームを共通の構文で操作できる

その共通の構文がシフト演算子オーバーロード

---

## シフト演算子オーバーロード

- シフト演算子がストリームに対してオーバーロードされている
  - 最左辺にストリームオブジェクトがくる

```C++
int n, m;
std::cin >> n >> m;   // 標準入力から整数を2つ取る
std::cout << n << m;  // 標準出力へ整数を2つ出す
```

- 入力: `operator>>`

- 出力: `operator<<`

- 簡単便利わかりやすい

- 以降では簡単のために `std::ostream` (出力ストリーム) のみを扱う

???
C++ではシフト演算子をストリームに対してオーバーロードしており、
それによって様々な入出力を簡便に行うことができる

右シフト演算子で入力、左シフト演算子で出力を行う

左辺にストリームオブジェクトが置かれるので、シフトの向きとI/Oが対応する

---
name: what's manipulator
layout: true

# 2. マニピュレータとは

---

- ストリームを制御する機能を持つオブジェクト

	- ストリームに対して入出力することで機能が発動する

- マニピュレータの例

	- `std::endl`
		- 改行してフラッシュ

	- `std::boolalpha`
		- 真偽値を `0/1` ではなく `false/true` で出力

	- `std::setprecision`
		- 浮動小数点数の表示精度を指定

---

- 基本的なマニピュレータは `<ios>`, `<istream>`, `<ostream>` にある

  - `<iostream>` をインクルードすると全部入る

- 引数を取ってより詳細な制御をするマニピュレータは
  `<iomanip>` ヘッダの中に入っている

???
`<iomanip>` に入っているのは引数を取るマニピュレータ

それ以外のヘッダに入っているのは引数を取らないマニピュレータ

---

```C++
#include <cmath>
#include <iomanip>
#include <iostream>

int main() {
  std::cout << true << std::endl << std::boolalpha
            << true << std::endl;
  // 1
  // true
  std::cout << M_PI << std::endl << std::setprecision(16)
            << M_PI << std::endl;
  // 3.14159
  // 3.141592653589793
}
```

???
`bool` 値はデフォルトで `0/1` で出力されるが、
`std::boolalpha` で `false/true` で表示するようにできる
逆に `std::noboolalpha` で元に戻すことができる

浮動小数点数はデフォルトで6桁までしか表示されないが、
`std::setprecision` を使うと好きな精度で
出力させることができるようになる

--

.center[.larger[どうしてそんなふうにうまくいくの？]]

---
name: mechanism
layout: true

# 3. マニピュレータのしくみ

---

`operator<<` のオーバーロードがどうなっているのか調べてみる

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
グローバルの `operator<<` と `ostream::operator<<` の違いは
`ostream`の本当の型 `basic_ostream<CharT, Traits>` の
`CharT` に依存するかどうか

---

`operator<<` のオーバーロードがどうなっているのか調べてみる

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

???
`sted::ostream&` を受け取って `std::ostream&` を返す関数ポインタ
を受け取るシフト演算子オーバーオードが
`ostream` の公開メンバに存在する

---

```C++
ostream& ostream::operator<<(ostream& (*func)(ostream&)) {
  return func(*this);
}
```

- `operator<<` のオーバーロードには関数ポインタを受け取るものがある

- このオーバーロードがマニピュレータを処理している

- 引数のマニピュレータに自分自身(`ostream`)を渡して、戻り値をそのまま返す関数

---

## 実装例

```C++
std::ostream& endl(std::ostream& os) {
  return os << '\n' << std::flush;
}

int main() {
  std::cout << 42 << endl;
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
---
name: preview
layout: true

# 次回予告

## 「マニピュレータの作り方」

- 引数をとるマニピュレータ

- 現代的なマニピュレータ

<br/><br/>
.center[.larger[.larger[近日例会講座予定！]]]

---
