class: center, middle

# マニピュレータの作り方

## hatsusato

### 2016/04/28

---
name: self-introduction

# 自己紹介

## hatsusato

- Twitter: [@hatsusato](https://twitter.com/hatsusato)

	- 今期の目標はアニメ感想をつぶやくこと

- GitHub: [hatsusato](https://github.com/hatsusato)

	- C89コンパイラの進捗だめです

- 理学部数学系 -> 情報学研究科通信情報システム専攻計算機ソフトウェア分野

- C++フリーク

- プログラミング言語を作りたい

---
name: advertisement
layout: true

# 宣伝

---

## 終焉のC++2016

- C++を勉強するプロジェクト

- 毎週火曜20時から

- まず _Effective STL_ を読み、その後 _Effective Modern C++_ を読む予定です

	- _Effective C++_ は昨年度の 終焉のC++2015 で読みました。

- 皆さんの参加をお待ちしています

---

## C89コンパイラ

- https://github.com/kmc-jp/c89-compiler

- ノリと勢いでC89コンパイラを作る

	- コミックマーケットC90に出したい

- 手伝ってくれる仲間を絶賛募集中

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

- C++では入出力をストリームと呼ばれる概念で抽象化している

	- ストリームによりただの文字列バッファとファイルを同一視して
	  共通の構文で制御できる

- ストリーム操作での肝になるのはシフト演算子オーバーロード

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

- 入力ストリームからは `operator>>` で、出力ストリームへは `operator<<` でアクセスすることで、
  視覚的にもわかりやすく操作できる

---

```C++
int n, m;
std::cin.operator>>(n).operator>>(m);
std::cout.operator<<(n).operator<<(m);
```

- 入力ストリームからは `operator>>` で、出力ストリームへは `operator<<` でアクセスすることで、
  視覚的にもわかりやすく操作できる

- メソッドチェインを簡便に書けるので複数の要素に対する入出力も簡単にできる

- 以降では簡単のために `std::ostream` のみを扱う

---
name: what's manipulator
layout: true

# 2. マニピュレータとは

---

- ストリームを制御する機能を持つオブジェクトのこと

- 使い方はマニピュレータをストリームの入出力に混ぜるだけ

---

- マニピュレータの例
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

--

## .center[**どうしてそんなふうにうまくいくの？**]

---
name: mechanism
layout: true

# 3. マニピュレータのしくみ

---
template: mechanism
layout: true

- `operator<<` のオーバーロードがどうなっているのか調べてみる

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

	- `std::ostream&` を受け取って `std::ostream&` を返す関数ポインタ

- このオーバーロードがマニピュレータを処理している

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

- これでもう `std::endl` みたいなマニピュレータは作れますね！

---

## 疑問

- `std::endl` はそのまま `operator<<` に流すだけだが、
  `std::setprecision` みたいな引数をとるやつはどうなっているんだ？

	- 疑問は次で解決

---
