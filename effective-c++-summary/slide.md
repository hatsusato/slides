class: center, middle

# .title[Effective C++ 総集編]

## hatsusato

KMC春合宿2017

---

# Self-introduction

![enter](enter.png)

## @hatsusato

- KMC5回生 まもなくM2

- 理学部数学系卒

- 五十嵐研究室

- プログラミング言語つくる

- コンパイラ進捗ダメです

---
name: introduction
layout: true

# Introduction

---

- C++は *"expert friendly"*[1] とも呼ばれるように、習得の難しい言語です。
  - 実際、初心者が入門書を読んだだけでは、堅牢で効率のよいコード(正しいコード)をC++で書くことは非常に困難です。
- こういったC++の学習をとりまく状況の中で、 *Effective C++* は中級以上へとステップアップするために欠かせない本として不動の地位を築いてきた。

.footnote[[1] by Bjarne Stroustrup, [The Problem with Programming](https://www.technologyreview.com/s/406923/the-problem-with-programming/)]

---

- 一方、時間の流れの中で *Effective C++* は着実に古びていっている。
  - 現在手に入る日本語の第3版の初刷は2006年のことであり、C++11以後の現在とは隔世の感がある。
- 確かに *Effective C++* には現在にもなお通じる重要なアドバイスが豊富にまとまった良書ではあるが、C++11以後では推奨されなくなっていたり、誤っていたりする主張も含まれていて、現在ではおすすめしづらい本へと変わってきている。
- C++11対応を謳った *Effective Modern C++* は、結局 *Effective C++* とは別の内容を扱っており、著者もどうやらこれ以上 *Effective C++* を更新するつもりはないらしい[2]。

.footnote[[2] http://scottmeyers.blogspot.jp/2015/12/good-to-go.html]

---

- 以上を踏まえて、*Effective C++* の古い内容を整理して、重要な情報のみを凝縮したドキュメントが必要だという結論に至りました。
- 古い内容を改める部分などは、C++11以降の状況や個人の考えも入ったものになっているので、元の本から大きく異なる部分もあります。ご了承ください。

---

- *Effective C++* 全体の内容の分量は発表するには多すぎるので、今回はその一部を発表します。
- 本来は副読本として *Effective C++* と合わせて読んだり、のちの勉強会などで使ってもらうことを想定しています。

---
name: table-of-contents
layout: true

# Table of contents

---

- **第1章 C++に慣れよう**
- 第2章 コンストラクタ、デストラクタ、コピー代入演算子
- 第3章 リソース管理
- 第4章 デザインと宣言
- 第5章 実装
- 第6章 継承とオブジェクト指向設計
- 第7章 テンプレートとジェネリックプログラミング
- 第8章 `new`と`delete`のカスタマイズ
- 第9章 いろいろな事

---
name: item-1
layout: true

## 1項 C++を複数の言語の連合とみなそう

---

- C++はマルチパラダイムな言語だと言われる。
- 正しく効率的なコードを書くために従うべきルールは、コードがどのパラダイムを利用しているかに応じて変わってくる。
  - 例えば、値渡しにするか参照渡しにするかの選択など。

---

C++は以下の4つの言語の連合とみなすべきです。

- C
  - C++の基本的は部分はCから来ている。
- オブジェクト指向C++
  - Cにはないクラスを扱うときにはオブジェクト指向のルールに従う。
- テンプレートC++
  - ジェネリックプログラミングをサポートする。他のパラダイムとは交わらない部分が多い。
- STL
  - STLをうまく扱うには独自の規約を理解する必要がある。関数型っぽいところがある。

---
name: item-2
layout: true

## 2項 `#define` より、`const`、`enum`、`inline`を使おう

---

- 今やC言語の方でも成り立つ指針になっていますが、定数やインライン関数を表すのにマクロを使うのをやめましょう。
- *`enum`を使おう* というのは`enum`ハックの話で、もはや現代的ではありません。`static const`を用いましょう。
- `inline`に関しては、**現代のコンパイラは人間より賢い**です。`inline`指定ではなく最適化オプションを用いましょう。
  - 最適化オプションを使えば、**たとえ`inline`指定していなくても**、インライン化すべきとコンパイラが判断すればインライン化されます。
  - テンプレート関数などとは違って、ソースファイルに書かれたコードであっても、リンク時最適化を行えばインラインにできます。

---
name: item-3
layout: true

## 3項 可能ならいつでも`const`を使おう

---

次の違いを指摘できますか？

```C++
char greeting[] = "Hello";

char *p0 = greeting;
char const *p1 = greeting;
const char *p2 = greeting;
char *const p3 = greeting;
char const *const p4 = greeting;
const char *const p5 = greeting;
```

--

- `*`の**左**に`const`があれば、ポインタの参照先の領域が`const`になる。
  `"Hello"`を書き換えられない。
- `*`の**右**に`const`があれば、ポインタ変数自身の領域が`const`になる。
  別のオブジェクトを指すようにしたり、イテレートしたりできない。
- `const`が`char`の右か左かは関係ない。

---

- 関数の引数やローカル変数など、`const`指定できるものはできるだけしておいた方が安全です。
  - 値渡しのときは`const`指定してもあまり意味がないです。
  - 戻り値に`const`指定もできますが、その指定が適切な場合はあまり無いです。
	組み込み型を厳密に模倣するようなクラスではありえるかもしれません。

ラムダ式を使えば、どこでも`const`初期化できる。
```C++
const auto line = [](){
  std::string buf;
  std::getline(std::cin, buf);
  return buf;
}();
```

---

- `iterator`の代わりに`const_iterator`を積極的に利用することも重要です。
  - `cbegin`などの`const_iterator`を返してくれる関数をチェックしておきましょう。
- メンバ関数に`const`指定をすることは特に重要です。
  場合によっては`const`版と非`const`版のオーバーロードの提供も考えましょう。

---

- メンバ関数への`const`指定は、あくまでもそのクラスのメンバ変数の*ビットレベル不変性*を保証するものでしかない。
  - つまり `T *const` として扱うということであって、 `const T*` という意味ではない。
  - よって、ポインタの指す先ならば`const`メンバ関数でも書き換えられるので注意。

---

- クラスの*論理的な不変性*を保証するための機能として`mutable`というキーワードがありますが、スレッドセーフを勉強してから使うようにしてください。
  - cf. *Effective Modern C++ 項目16 `const`メンバ関数はスレッドセーフにする*
- ラムダ式に指定できる`mutable`は、全くの別物なので、スレッドセーフを気にする必要はありません。
  - 単にラムダ式の関数呼び出しオーバーロードに暗黙についている`const`指定を剥がすだけのものです。

```C++
int x = 0;
auto f = [x]() mutable {
  x = 1;
  return x;
};
```

---

- 以下は`operator[]`オーバーロードなどの実装でしばしば用いられるイディオムです。
  - アクセサ関数の提供という限られた場面でしか使えないですが、知っておいて悪くないです。

```C++
class string {
 public:
  // ...
  const char& operator[](std::size_t pos) const {
    return str_[pos];
  }
  char& operator[](std::size_t pos) {
    return const_cast<char&>(
*       static_cast<const string&>(*this)[pos]
    );
  }
  // ...
 private:
  char *str_;
};
```

---
name: item-4
layout: true

## 4項 オブジェクトは、使う前に初期化しよう

---

- 組み込み型に対して初期化式を指定しないと、Cと同様にオブジェクトの値は不定になります。
  必ず初期化式を与えるようにしましょう。
- クラスを作るときは、未初期化状態になることがないように、適切にコンストラクタを定義しましょう。
- 初期化と代入は異なります。
  特にコンストラクタでは*メンバ初期化リスト*という初期化構文を用いましょう。
  - 初期化の順序はメンバ初期化リストの順ではなく、クラス内のメンバ変数の宣言順です。
	混同がないように、メンバ初期化リストの順番はメンバ変数の宣言順と合わせておきましょう。

```C++
struct X {
  X() : a(0), b(1.0), c('2') {}
  int a;
  double b;
  char c;
};
```

---

- グローバルな静的オブジェクトは、本質的に初期化順序の問題を抱えており、**解決は不可能**です。
  - C++は異なる翻訳単位における静的オブジェクトの初期化順序を定めていません。
  - グローバルな静的オブジェクトを使うのをやめて、代わりにローカルな静的オブジェクトを使うようにしましょう。

```C++
Singleton& static_object() {
  static Singleton object;
  return object;
}
```

---
template: table-of-contents

- 第1章 C++に慣れよう
- **第2章 コンストラクタ、デストラクタ、コピー代入演算子**
- 第3章 リソース管理
- 第4章 デザインと宣言
- 第5章 実装
- 第6章 継承とオブジェクト指向設計
- 第7章 テンプレートとジェネリックプログラミング
- 第8章 `new`と`delete`のカスタマイズ
- 第9章 いろいろな事

---
name: item-5
layout: true

## 5項 C++が自動で書き、自動で呼び出す関数を知ろう

---

- 以下のメンバ関数を*特殊メンバ関数*と呼ぶ。
  - デフォルトコンストラクタ
  - デストラクタ
  - コピーコンストラクタ
  - コピー代入演算子
  - ムーブコンストラクタ
  - ムーブ代入演算子
- 特殊メンバ関数は、宣言していなくても、特定の条件を満たせば暗黙に生成されます。
- cf. *Effective Modern C++ 項目17 自動的に生成される特殊メンバ関数を理解する*

---

- コンストラクタが1つも宣言されていないとき、デフォルトコンストラクタは暗黙生成されます。
  - ここでのコンストラクタには、コピーコンストラクタやムーブコンストラクタを含みます。
- それ以外の特殊メンバ関数が暗黙に生成される条件、および暗黙生成が抑制される条件は、それ自体を覚えても解決とはなりません。
  次の3つのルールを覚えておきましょう。
  - *Rule of three*
  - *Rule of five*
  - *Rule of zero*

---

*Rule of three*

- もし、デストラクタ・コピーコンストラクタ・コピー代入演算子のうち、1つでも宣言する必要があるのならば、この3つ全てを定義する必要がある。
  - このとき、ムーブコンストラクタ・ムーブ代入演算子の暗黙生成は抑制される。

*Rule of five*

- もし、ムーブコンストラクタ・ムーブ代入演算子のうち、1つでも宣言する必要があるのならば、デストラクタ・コピーコンストラクタ・コピー代入演算子を含めた5つ全てを定義する必要がある。

---

*Rule of zero*

- デフォルトコンストラクタを除く特殊メンバ関数は、極力宣言しないようにする。
- もしそれらが必要ならば、その必要性を細かく分割し、それぞれ別のクラスで適切に管理するべきです(*Single Responsibility Principle*)。
  通常は、それらのクラスをメンバに持ったり継承したりするだけで十分です。
  - 例えば`vector`、`string`、`unique_ptr`などのクラスは*適切に管理*されており、我々はそれらを組み合わせるだけでよい。

---

- 与えたい定義が暗黙に生成されるものと同じになるような場面では、これらの特殊メンバ関数の定義を明示的に`default`生成させることができる。

```C++
struct Empty {
  Empty() = default;
  ~Empty() = default;
  Empty(const Empty&) = default;
  Empty& operator=(const Empty&) = default;
  Empty(Empty&&) = default;
  Empty& operator=(Empty&&) = default;
};
```

---
name: item-6
layout: true

## 6項 コンパイラが自動生成することを望まない関数は、使用を禁止しよう

---

- 特殊メンバ関数に`delete`を指定することで、その関数の生成を明示的に禁止することができる。
- 以下はコピーできないクラスの例です。

```C++
template <typename T>
struct Uncopyable {
 protected:
  Uncopyable() = default;
  ~Uncopyable() = default;
* Uncopyable(const Uncopyable&) = delete;
* Uncopyable& operator=(const Uncopyable&) = delete;
  Uncopyable(Uncopyable&&) = default;
  Uncopyable& operator=(Uncopyable&&) = default;
};
```

---

- 特殊メンバ関数に`delete`を指定することで、その関数の生成を明示的に禁止することができる。
- 以下はムーブできないクラスの例です。

```C++
template <typename T>
struct Unmovable {
 protected:
  Unmovable() = default;
  ~Unmovable() = default;
  Unmovable(const Unmovable&) = default;
  Unmovable& operator=(const Unmovable&) = default;
* Unmovable(Unmovable&&) = delete;
* Unmovable& operator=(Unmovable&&) = delete;
};
```

---

- 一般に、コピーやムーブを禁止したいクラスを作るときには、次のイディオムがしばしば用いられる。
  - *Rule of zero* を思い出そう。

```C++
class YourClass : private Uncopyable<YourClass>,
                  private Unmovable<YourClass> {
  // YourClass can neither copy nor move
};
```

- 先ほどの`Uncopyable`や`Unmovable`の実装において、テンプレート(CRTP)や`protected`が使われていたのは、このイディオムのように`private`継承での使い方を想定しているからです。
  - *空の基底クラスの最適化*を利用し、*菱型継承問題*に対処している。

---
name: item-7
layout: true

## 7項 ポリモーフィズムのための基底クラスには仮想デストラクタを宣言しよう

---

- 動的ポリモーフィズムを利用するために継承を行うときは、基底クラスのデストラクタを`virtual`にする必要があります。

```C++
class Base {
 public:
  // ...
* ~Base() = default;
  virtual void f() = 0;
};

class VirtualBase {
 public:
  // ...
* virtual ~VirtualBase() = default;
  virtual void f() = 0;
};
```

---

- 動的ポリモーフィズムを利用するために継承を行うときは、基底クラスのデストラクタを`virtual`にする必要があります。

```C++
class Derived : public Base {
 public:
  void f() override {}
};

class VirtualDerived : public VirtualBase {
 public:
  void f() override {}
};
```

---

- さもないと、派生クラスのオブジェクトを基底クラスの型として破棄したときに、派生クラスのデストラクタが呼び出されず、メモリリークにつながります。
  - デストラクタが仮想関数になっていないと、動的なオブジェクトの型を探索できずに、静的な型のデストラクタが呼び出されてしまいます。
- したがって、STLコンテナなど、仮想デストラクタをもたないクラスから継承してはいけない。

```C++
#include <memory>
int main() {
  std::unique_ptr<Base> b = std::make_unique<Derived>();
  std::unique_ptr<VirtualBase> vb =
      std::make_unique<VirtualDerived>();
  // Derived::~Derived() will not be called
}
```

---

- 仮想関数(仮想デストラクタを含む)をもつクラスは、よくある実装では仮想関数テーブルを指す暗黙のポインタ領域をもつので、クラスのサイズは見かけよりもポインタ1つ分多くなります。
  - 仮想関数の導入は動的ポリモーフィズムが必要な場面に限るようにしましょう。
- ちなみに、デストラクタは純粋仮想にすることができます。他に純粋仮想にする関数がないときは、デストラクタを利用しましょう。

```C++
class ABC {
  virtual ~ABC() = 0;
};
ABC::~ABC() = default;
```
