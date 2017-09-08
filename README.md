clangsay
=======

[![version](http://img.shields.io/github/tag/sasairc/clangsay.svg?style=flat&label=version)](https://github.com/sasairc/clangsay/releases)
[![license](https://img.shields.io/badge/License-WTFPL2-blue.svg?style=flat)](http://www.wtfpl.net/txt/copying/)
[![issues](http://img.shields.io/github/issues/sasairc/clangsay.svg?style=flat)](https://github.com/sasairc/clangsay/issues)
[![build](https://img.shields.io/travis/sasairc/clangsay.svg?style=flat)](https://travis-ci.org/sasairc/clangsay)

![default](https://raw.githubusercontent.com/sasairc/clangsay/master/img/screenshot.png)

マルチバイト文字準対応の Unicode 環境専用 cowsay (設定可能な喋る牛) クローン。  
ただし、特殊文字が含まれる場合は、相変わらずズレてしまいます。  
cowsay の`-W`オプションに該当するものはありませんが、後述の方法により同様の機能は実現可能です。


## Requirements

* pkg-config >= 0.26
* GLib >= 2.0
* libbenly >= 1.0.0 (同梱)

* GNU make
* gcc or llvm+clang


## Install

### Debian GNU/Linux or Debian based system (amd64)

もし、あなたが 64bit PC (amd64) アーキテクチャ用 Debian GNU/Linux を利用している場合、以下の方法で簡単にコンパイル済みバイナリパッケージをインストールする事ができます。また、これをベースとした各 GNU/Linux ディストリビューションでも多くの場合で利用可能でしょう。

```shellsession
# echo 'deb [trusted=yes] http://ssiserver.moe.hm/pub/debian ./' >> /etc/apt/sources.list	
# apt-get update	
# apt-get install clangsay
```

ソースパッケージをダウンロードし、自分好みの変更や修正を加えたのち、コンパイル済みバイナリパッケージを作成したいというケースもあるでしょう。その場合には、概ね以下の手順で可能です。

```shellsession
# cat <<EOF >> /etc/apt/sources.list
deb [trusted=yes] http://ssiserver.moe.hm/pub/debian ./
deb-src [trusted=yes] http://ssiserver.moe.hm/pub/debian ./
EOF
# apt-get update
# apt-get build-dep clangsay
% apt-get source clangsay    # または apt-source -b clangsay
% cd clangsay-${VERSION}
% dpkg-buildpackage -uc -us -d
# dpkg -i ../*.deb
```

詳しい情報については[Debian 新メンテナーガイド 日本語版](https://www.debian.org/doc/manuals/maint-guide/index.ja.html)をご覧下さい。


### Mac OS X/Homebrew

依存パッケージとして cowsay、pkg-config、glib がインストールされます。	

```shellsession
% brew install 844196/Renge/clangsay
```

上記のコマンドを実行した際に、補完関数をインストールしたディレクトリを示す下記のようなメッセージが出ます。

```shellsession
zsh completion has been installed to:
/usr/local/share/zsh/site-functions	# Homebrewの導入先により異なります
```

このパスを`$fpath`に追加するか、既にパスの通っている任意のディレクトリに`_clangsay`を移動して下さい。

### Docker

ビルド済みバイナリと cowfile がバンドルされたイメージを生成するための `Dockerfile` が同梱されています。

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

### Other systems.

* 全てインストールする場合  
	```shellsession
  % git clone --recursive https://github.com/sasairc/clangsay.git && cd clangsay
	% make
	# make install
	```

* *libbenly*を共有ライブラリとしてインストールし、それを利用する場合  
	```shellsession
	# make install-dep
	# make WITH_SHARED=1 install
	```

* 実行ファイルのみをインストールし、cowsay と cows を共有する場合  
	```shellsession
	% make COWPATH="/usr/local/share/cowsay/cows"	# パスは環境に合わせてください
	# make install-bin
	```

その他のオプションに関しては、`INSTALL.ja`をご覧下さい。


## zshでの補完

`$(PREFIX)/share/clangsay/compdef/_clangsay.zsh`を`_clangsay`として、`$fpath`の通ったディレクトリにコピーして下さい。	


## Usage

基本的に cowsay のオプションと同様です。  
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

`--think`オプションでは、 cowthink のような「牛さんが考える」動作になります。

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

`-R`オプションは何かを強調したい時に便利だと思います。これはあくまで主観。

```shellsession
% lsb_release | clangsay -R 10
 ________________________________________________________________________________
/  ____________________________________________________________________________   \
| /  ________________________________________________________________________   \ |
| | /  ____________________________________________________________________   \ | |
| | | /  ________________________________________________________________   \ | | |
| | | | /  ____________________________________________________________   \ | | | |
| | | | | /  ________________________________________________________   \ | | | | |
| | | | | | /  ____________________________________________________   \ | | | | | |
| | | | | | | /  ________________________________________________   \ | | | | | | |
| | | | | | | | /  ____________________________________________   \ | | | | | | | |
| | | | | | | | | / Distributor ID: Debian                      \ | | | | | | | | |
| | | | | | | | | | Description: Debian GNU/Linux 9.1 (stretch) | | | | | | | | | |
| | | | | | | | | | Release: 9.1                                | | | | | | | | | |
| | | | | | | | | \ Codename: stretch                           / | | | | | | | | |
| | | | | | | | \  --------------------------------------------   / | | | | | | | |
| | | | | | | \  ------------------------------------------------   / | | | | | | |
| | | | | | \  ----------------------------------------------------   / | | | | | |
| | | | | \  --------------------------------------------------------   / | | | | |
| | | | \  ------------------------------------------------------------   / | | | |
| | | \  ----------------------------------------------------------------   / | | |
| | \  --------------------------------------------------------------------   / | |
| \  ------------------------------------------------------------------------   / |
\  ----------------------------------------------------------------------------   /
 --------------------------------------------------------------------------------
  \
   \
    \
    
           ____
       ,: .: .: :.ヽ
     ,'       /\   ｉ
     {: .:ﾉﾚﾍ/  Viﾍ:}
    .{,､〈 Ｏ   Ｏ{.:.
    ノヽ\!"       }.:ﾊ
      Ｗﾊw=-､へ,ｬ<,V'      
         /ﾍ }{./\
        ;: i:V:!;}
        |:｜: :｜}
        |:|:｡: ｡l}
        >-'-ﾟ-'`ﾟu
        ｰi-i～i-i~
         |.|  |.|
         |-|  |-|
         ヒｺ  ヒｺ 
```

cowsay に於ける`-W`オプションはありませんが、[nkf](http://osdn.jp/projects/nkf/)の`-f`オプションを介すことで、同等の機能を実現できます。  

詳しくは`clangsay --help`をご覧下さい。


## Environment variable

### COWPATH

環境変数`$COWPATH`は、シェルの様々な検索パスと同様に`:`区切りで、複数ディレクトリの指定ができます。	

```shellsession
% export COWPATH="/foo/bar/baz:/hoge/fuga/piyo:/ramen/oudon"
```

環境変数`$COWPATH`に存在しないディレクトリが登録されている場合は無視されます。

### DEFAULT_COWFILE

環境変数`$DEFAULT_COWFILE`は、`-f, --file`オプションを使用せずに実行した場合の cowfile を置き換えます。

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
	yasuna に続き、clangsay まで MacOS X 向け Homebrew リポジトリへ追加して頂きました。  
	更には多くのユニーク(！)な cowfile の作成および提供、加えて Dockerfile の作成等々。   
	全て書きたくても書ききれない程のご厚意に、この場を借りて謝辞を申し上げます。  
	詳細は`AUTHORS`のクレジットをご覧下さい。


## License

[WTFPL version 2](http://www.wtfpl.net/txt/copying/)


## Original

[cowsay and cowthink](https://web.archive.org/web/20071026043648/http://www.nog.net/~tony/warez/cowsay.shtml)  
Archived from the original on February 25, 2012.

[schacon/cowsay](https://github.com/schacon/cowsay)  
Git version of awesome cowsay project.


## Author

sasairc (https://github.com/sasairc)

Masaya Tk (https://github.com/844196)
