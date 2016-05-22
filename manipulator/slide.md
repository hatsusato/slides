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

.large[
1. C++の入出力

2. マニピュレータとは

3. マニピュレータのしくみ

4. マニピュレータの作り方 (基礎編)

5. マニピュレータの作り方 (応用編)
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

.center[.large[どうしてそんなふうにうまくいくの？]]

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

関数ポインタを受け取るオーバーロードは以下のような実装になる

```C++
*ostream& ostream::operator<<(ostream& (*func)(ostream&)) {
  return func(*this);
}
```

- 引数のマニピュレータを自分自身 (`ostream&`) に適用して、戻り値をそのまま返すメンバ関数

- `ostream&` を戻り値として返すことでシフト演算子を続けて書くことができる

```C++
std::cout << std::endl << std::endl;
std::cout.operator<<(std::endl).operator<<(std::endl);
```

???
シフト演算子オーバーロードはメソッドチェーンのシンタックスシュガーである

---

以上を踏まえて `std::endl` と同じ機能をもつ `myendl` マニピュレータを実装してみる

```C++
std::ostream& myendl(std::ostream& os) {
  return os << '\n' << std::flush;
}

int main() {
  std::cout << 42 << myendl;
}
```

ね、簡単でしょう？

???
`std::flush` マニピュレータは `ostream` の `flush` メンバ関数を呼び出すマニピュレータ

`flush` メンバ関数はストリームをフラッシュする

---

## 疑問

- `std::endl` の型は `ostream& (*)(ostream&)`

- `std::setprecision` の型は？

	- 引数をとるマニピュレータは関数呼び出しの戻り値がストリームに流れる

	- 戻り値の型が `ostream& (*)(ostream&)` なのか？

	- どういうしくみ？

```C++
std::cout << std::setprecision(16) << M_PI << std::endl;
```

???
引数を取るマニピュレータは引数なしマニピュレータのように簡単には行かない

具体的には、マニピュレータが状態を持たないといけないので、関数ポインタでは表現できない

標準ライブラリの引数を取るマニピュレータの型は未規定である

---
name: making-basic
layout: true

# 4. マニピュレータの作り方 (基礎編)

---

## 引数を取るマニピュレータを作る

引数を取るマニピュレータは2つの要素からなる

1. 実際の処理を行う本体

2. 本体を生成するファクトリ

以降では例として次のようなマニピュレータを作る状況を考える

- 整数値を1つ取り、その回数分だけ改行するマニピュレータ

---
template: making-basic
layout: true

## 本体を作る

---

実際に処理を行う本体は次の3つの要素を持つクラスです

1. コンストラクタ

2. ストリームに対する処理

3. シフト演算子オーバーロード

---

クラスの定義は以下のようになる

```C++
class NewLines {
 public:
  NewLines(int n);
  void exec(std::ostream& os) const;
 private:
  int n_;
};
std::ostream& operator<<(std::ostream& os,
                         const NewLines& nl);
```

---

コンストラクタ

```C++
NewLines::NewLines(int n)
    : n_(n) {}
```

マニピュレータに渡された引数を保持するだけ

---

ストリームに対する処理

```C++
void NewLines::exec(std::ostream& os) const {
  for (int i = 0; i < n_; ++i) {
    os << std::endl;
  }
}
```

保持しているメンバ変数を使って実現したい処理を実行する

---

シフト演算子オーバーロード

```C++
std::ostream& operator<<(std::ostream& os,
                         const NewLines& nl) {
  nl.exec(os);
  return os;
}
```

ストリームに対する処理を実行してストリームを戻す

---

```C++
class NewLines {
 public:
  NewLines(int n) : n_(n) {}
  void exec(std::ostream& os) const {
    for (int i = 0; i < n_; ++i) {
      os << std::endl;
    }
  }
 private:
  int n_;
};
std::ostream& operator<<(std::ostream& os,
                         const NewLines& nl) {
  nl.exec(os);
  return os;
}
```

---
template: making-basic

## ファクトリを作る

本体を生成するファクトリはユーザが直接使う関数です

1. マニピュレータの処理に必要な引数を受け取り、

2. その引数を用いて本体オブジェクトを構築し、

3. そのオブジェクトを戻り値として返します

ファクトリの実装は次のようになる

```C++
NewLines newlines(int n) {
  return NewLines(n);
}
```

---
template: making-basic
layout: true

## まとめ

---

- 引数を取るマニピュレータは、
	- 実際の処理を行う本体クラスと、
	- そのクラスオブジェクトを作るファクトリ関数からなる。

- 本体クラスは、
	- 実行に必要な情報を保持し、
	- ストリームへのシフト演算子に引き渡されたときに、
	- ストリームに対してメンバ関数を適用して処理を行う

- ファクトリ関数は、
	- ユーザの指定した引数で本体クラスを構築する

---

```C++
void NewLines::exec(std::ostream& os) const {
  for (int i = 0; i < n_; ++i) {
    os << std::endl;
  }
}
std::ostream& operator<<(std::ostream& os,
                         const NewLines& nl) {
  nl.exec(os);
  return os;
}
NewLines newlines(int n) {
  return NewLines(n);
}

std::cout << newlines(4) << newlines(2);
```

---
name: making-advanced
layout: true

# 5. マニピュレータの作り方 (応用編)

---

- 引数を取るマニピュレータの作り方はわかったけど面倒くさい

- マニピュレータを作るたびにクラスを定義する必要がある

	- 関数オブジェクトを作るのは関数を作るように簡単には行かない

--

.huge[
→ ラムダ式！
]

---
template: making-advanced
layout: true

## C++11時代のマニピュレータ

---

1. 関数オブジェクトを実行するマニピュレータ本体を作る

2. ストリームに適用したいラムダ式でマニピュレータ本体を構築して返すファクトリ関数を作る

---

- マニピュレータ本体

```C++
class Manipulator {
 public:
  using FuncType = std::function<void(std::ostream&)>;
  Manipulator(const FuncType& f) : f_(f) {}
  void exec(std::ostream& os) const { f_(os); }
 private:
  FuncType f_;
};
std::ostream& operator<<(std::ostream& os,
                         const Manipulator& m) {
  m.exec(os);
  return os;
}
```

---

- ファクトリ関数

```C++
Manipulator newlines(int n) {
  return Manipulator([n](std::ostream& os) {
      for (int i = 0; i < n; ++i) {
        os << std::endl;
      }
    });
}
```

---

- マニピュレータ本体は1度作るだけでよい

- ストリームに適用したい処理をラムダ式で関数内に書けるので簡単にファクトリ関数が作れる

--

.large[
→ マニピュレータ作り放題！
]

---
template: making-advanced
layout: true

## 応用例

---

- 可変長引数をカンマで区切ってかっこで囲んで出力するマニピュレータ `tuple_print`
	- 引数をかっこで囲んで出力するマニピュレータ `paren_print` と、
	- 可変長引数をカンマで区切って出力するマニピュレータ `separate_print` を組み合わせる

```C++
template <typename... Args>
Manipulator tuple_print(const Args&... args) {
  const auto m = paren(separate_print(args...));
  return Manipulator([m](std::ostream& os) {
      os << m;
    });
}
std::cout << tuple_print(42, 3.14, "abc") << std::endl;
// (42, 3.14, abc)
```

---

- 引数をかっこで囲んで出力するマニピュレータ `paren_print`

```C++
template <typename T>
Manipulator paren_print(const T& v) {
  return Manipulator([v](std::ostream& os) {
      os << '(' << v << ')';
    });
}
std::cout << paren(42) << paren(3.14) << paren("abc")
          << std::endl;
// (42)(3.14)(abc)
```

---

- 可変長引数をカンマで区切って出力するマニピュレータ `separate_print`
	- `comma_separate` が出力をカンマで区切る仕事を実際に行う

```C++
template <typename... Args>
Manipulator separate_print(const Args&... args) {
  const auto t = std::make_tuple(args...);
  return Manipulator([t](std::ostream& os) {
      comma_separate(os, t);
    });
}
std::cout << separate_print(42, 3.14, "abc") << std::endl;
// 42, 3.14, abc
```

---
