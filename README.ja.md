[English](README.md)/日本語
# Brownie
Brownie (ブラウニー) はオフラインで物体検出を行う M5StickV 専用アプリです。かわいい声でモノの名前を教えてくれます！
2021年以降の M5StickV をお持ちの場合は [Brownie TMMF 2020](https://github.com/ksasao/brownie/blob/master/src/brownie_learn/doc/TMMF2020/readme.md) をご利用ください。ファームウェアが異なります。

- [Maker Faire Taipei 2019 Kit](https://github.com/ksasao/brownie/releases/tag/v2.0) をダウンロード!

[![Tweet](https://user-images.githubusercontent.com/179872/61575967-48284000-ab0e-11e9-9309-c4792fd5a473.png)](https://twitter.com/ksasao/status/1151984910703288321)

## セットアップ方法
0. class10 の microSDHC カードを用意してください。microSDカードの相性問題が報告されています。[動作確認済みリスト](https://docs.google.com/spreadsheets/d/10Vv8ZQkbXX59aT_GkoolTMHf83zroIT21uNjvQMaGng/edit#gid=0)を確認してください。

1. このリポジトリの src ディレクトリ以下にあるすべてのファイルを、SDカードのルートディレクトリにコピーしてください。下記のZIPファイルを利用することも可能です。

- https://github.com/ksasao/brownie/releases/download/1.0.1/brownie_v1.0.1.zip
- [すべてのバージョン](https://github.com/ksasao/brownie/releases)

2. microSDカードを M5StickV に挿入してください。

以上です!

### うまく動きませんか？
- 2019年11月時点での最新ファームウェアではうまく動作しないことが確認されています。[maixpy_v0.4.0_47_g39bb8bf_m5stickv.bin](https://github.com/ksasao/brownie/raw/master/materials/maixpy_v0.4.0_47_g39bb8bf_m5stickv.bin) を [
kflash_gui](https://docs.m5stack.com/en/quick_start/m5stickv/maixpy) (Windows/Mac/Linux版があります) などで書き込んでみてください。

## 遊び方
カメラをものに向けてAボタン(いちばん大きいボタン)を押してください。
