class: center, middle

# マニピュレータの作り方

## hatsusato

### 2016/04/28

---
layout: true

# 自己紹介

---

## hatsusato

- Twitter: [@hatsusato](https://twitter.com/hatsusato)

	- 今期の目標はアニメ感想をつぶやくこと

- GitHub: [hatsusato](https://github.com/hatsusato)

	- C89コンパイラの進捗だめです

- 理学部数学系 -> 情報学研究科通信情報システム専攻計算機ソフトウェア分野

- C++フリーク

- プログラミング言語を作りたい

---
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

---

# 1. C++の入出力

- C++では入出力をストリームと呼ばれる概念で抽象化している

	- ストリームによりただの文字列バッファとファイルを同一視して
	  共通の構文で制御できる

- ストリーム操作での肝になるのはシフト演算子オーバーロード

---
