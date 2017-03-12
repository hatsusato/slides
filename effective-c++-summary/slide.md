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
