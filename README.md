clangsay
=======

[![version](http://img.shields.io/github/tag/sasairc/clangsay.svg?style=flat&label=version)](https://github.com/sasairc/clangsay/releases)
[![license](https://img.shields.io/badge/License-WTFPL2-blue.svg?style=flat)](http://www.wtfpl.net/txt/copying/)
[![issues](http://img.shields.io/github/issues/sasairc/clangsay.svg?style=flat)](https://github.com/sasairc/clangsay/issues)
[![build](https://img.shields.io/travis/sasairc/clangsay.svg?style=flat)](https://travis-ci.org/sasairc/clangsay)

![default](https://raw.githubusercontent.com/sasairc/clangsay/master/img/screenshot.png)

マルチバイト準対応のUnicode環境専用cowsayクローン。  
ただし、特殊文字が含まれる場合は、相変わらずズレてしまいます。	
cowsayの`-W`オプションに該当するものはありません。（常に`-n`つきの状態）	


## Requirements

* GNU Make
* gcc or clang
* pkg-config
* glib-2.0	


## Install

#### Debian/GNU Linux or Debian based system.(amd64)

```shellsession
# echo 'deb http://ssiserver.moe.hm/pub/debian ./' >> /etc/apt/sources.list	
# apt-get update	
# apt-get install clangsay
```

#### Mac OS X/Homebrew

依存パッケージとしてcowsay、pkg-config、glibがインストールされます。	

```shellsession
% brew install 844196/Renge/clangsay
```

上記のコマンドを実行した際に、補完関数をインストールしたディレクトリを示す下記のようなメッセージが出ます。

```shellsession
zsh completion has been installed to:
/usr/local/share/zsh/site-functions	# Homebrewの導入先により異なります
```

このパスを`$fpath`に追加するか、既にパスの通っている任意のディレクトリに`_clangsay`を移動して下さい。

#### Docker

ビルド済みバイナリとcowfileがバンドルされたイメージを生成するための `Dockerfile` が同梱されています。

```shellsession
% HEAD=`git rev-parse --short HEAD`
% make docker-image IMGTAG=$HEAD
% yasuna | docker run --rm -i clangsay:$HEAD -f yasuna_16
 _______________________
< 自分でやったんじゃん！ >
 -----------------------
   \
    \
         ..: ￣￣￣￣: :.
       ／::  /｜.:/ |.: .:＼
      ,  /｜/  |./  |.ﾊ.: .:ヽ
    ./.:ｲ__ノ   ヽ､___∨.: .:.
   ./: .:≡≡     ≡≡.|.: .:｜
   /ノ|/} }.      } } |:ﾊ:.:｜
     .ヽ{,{ -~~~- {,{｜:/ﾉ:从
      ∨v､＞z-r-x-:r＜/ﾚﾚへ
```

#### Other systems.

* 全てインストールする場合	
	```shellsession
  % git clone --recursive https://github.com/sasairc/clangsay.git && cd clangsay
	% make
	# make install
	```

* 実行ファイルのみをインストールし、cowsayとcowsを共有する場合	
	```shellsession
	% make COWPATH="/usr/local/share/cowsay/cows"	# パスは環境に合わせてください
	# make install-bin
	```

その他のオプションに関しては、`INSTALL.ja`をご覧下さい。


## zshでの補完

`$(PREFIX)/share/clangsay/compdef/_clangsay.zsh`を`_clangsay`として、`$fpath`の通ったディレクトリにコピーして下さい。	


## Usage

基本的にcowsayのオプションと同様です。		
例外として、ANSIエスケープシーケンスによる色付け等に対応しました。

```shellsession
% export COWPATH="~/.cows"	# COWPATHを指定したい場合
% clangsay foo bar baz
 ____
/ foo \
| bar |
\ baz /
 ----
        \   ^__^
         \  (oo)\_______
            (__)\       )\/\
                ||----w |
                ||     ||
% clangsay "foo bar baz"
 ____________
< foo bar baz >
 ------------
        \   ^__^
         \  (oo)\_______
            (__)\       )\/\
                ||----w |
                ||     ||
% cat /proc/meminfo | head -n 10 | clangsay
 ____________________________
/ MemTotal:        6107204 kB \
| MemFree:          614952 kB |
| MemAvailable:    4587172 kB |
| Buffers:          865520 kB |
| Cached:           666056 kB |
| SwapCached:            0 kB |
| Active:          1873528 kB |
| Inactive:         765352 kB |
| Active(anon):    1157912 kB |
\ Inactive(anon):    37476 kB /
 ----------------------------
        \   ^__^
         \  (oo)\_______
            (__)\       )\/\
                ||----w |
                ||     ||
```

`--think`オプションでは、cowthinkのような「牛さんが考える」動作になります。

```shellsession
% echo "ﾌﾟｶﾌﾟｶ" | clangsay --think -f iwashi.cow
 _______
< ﾌﾟｶﾌﾟｶ >
 -------
   o
    o
         ＿＿＿＿ ＿＿＿＿＿__
      ｨ''  ＠ :. ,! ，， ， ，￣￣ ¨` ‐-            ＿＿
       ＼    ノ   i            ’ ’’ ’’､_;:`:‐.-_-‐ニ＝=彳
         ｀ ＜. _  .ｰ ､                       !三  ＜
                 ｀¨  ‐= . ＿＿＿_.. ﾆ=-‐‐`'´｀ﾐ､   三＞
                                                 ￣￣
```

cowsayに於ける`-W`オプションはありませんが、[nkf](http://osdn.jp/projects/nkf/)の`-f`オプションを介すことで、同等の機能を実現できます。  

詳しくは`clangsay --help`をご覧下さい。


## Environment variable

### COWPATH

環境変数`$COWPATH`は、シェルの様々な検索パスと同様に`:`区切りで、複数ディレクトリの指定ができます。	

```shellsession
% export COWPATH="/foo/bar/baz:/hoge/fuga/piyo:/ramen/oudon"
```

環境変数`$COWPATH`に存在しないディレクトリが登録されている場合は無視されます。

### DEFAULT_COWFILE

環境変数`$DEFAULT_COWFILE`は、`-f, --file`オプションを使用せずに実行した場合のcowfileを置き換えます。

```shellsession
% echo $COWPATH
/usr/local/share/clangsay/cows
% clangsay -l | grep doseisan
doseisan.cow
% export DEFAULT_COWFILE="doseisan.cow"	# ファイル名
% export DEFAULT_COWFILE="doseisan"     # 拡張子省略
% export DEFAULT_COWFILE="/usr/local/share/clangsay/cows/doseisan.cow"	# パス指定
```


## Thanks

* [Masaya Tk](https://github.com/844196)	
	yasunaに続き、clangsayのHomebrew用リポジトリまで提供して頂き  
	更にはユニークなcowfileまで作って頂きました。本当に有難うございました。  
	詳細は`AUTHORS`のクレジットをご覧下さい。


## License

[WTFPL version 2](http://www.wtfpl.net/txt/copying/)


## Author

sasairc (https://github.com/sasairc)

Masaya Tk (https://github.com/844196)
