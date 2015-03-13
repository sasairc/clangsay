clangsay
=======
[![version](https://img.shields.io/badge/tag-0.0.5-orange.svg?style=flat)](https://github.com/sasairc/yasuna/releases)
[![license](https://img.shields.io/badge/License-WTFPL2-blue.svg?style=flat)](http://www.wtfpl.net/txt/copying/)	

![default](http://41.media.tumblr.com/d93abecb6fe04a8f6d1c38deba2e963a/tumblr_njt1q0EsZJ1u2jamko1_1280.png)

マルチバイト準対応のUnicode環境専用cowsayクローン。  
ただし、特殊文字が含まれる場合は、相変わらずズレてしまいます。	
cowsayの`-W`オプションに該当するものはありません。（常に`-n`つきの状態）	
### Requirements
* GNU Make
* gcc
* pkg-config
* glib-2.0	

### Install
* Debian/GNU Linux or Debian based system.(amd64)		
	依存パッケージとしてcowsayがインストールされ	
	/usr/share/cowsay/cows以下が共有されます。
	```shellsession
	# echo 'deb http://ssiserver.moe.hm/debian ./' >> /etc/apt/sources.list	
	# apt-get update	
	# apt-get install clangsay
	```

* Debian/GNU Linux or Debian based system.(not amd 64)	
	依存パッケージとしてcowsayがインストールされ	
	/usr/share/cowsay/cows以下が共有されます。	
	```shellsession
	% cat info.txt
	注意：バージョンは、適時置き換えて下さい。
	% wget -O - http://ssiserver.moe.hm/debian/clangsay_0.0.2-1.tar.gz | tar zxvf -
	% cd clangsay
	% dpkg-buildpackage -uc -us
	# dpkg -i ../clangsay_*.deb
	```

* Mac OS X/Homebrew	
  依存パッケージとしてcowsay、pkg-config、glibがインストールされます。	

	* 全てインストールする場合（cowファイルを共有）	
	cowsay側のパスに対して、cowファイルのインストールが行われます。
	```shellsession
	% brew tap 844196/Renge	# Tap
	% brew install clangsay	# Install
	```
	
	* zsh補完関数もインストールする場合		
	環境変数`$fpath`に指定されているパス対して、補完関数ファイルである`_clangsay`がインストールされます。
	```shellsession
	% brew tap 844196/Renge	# Tap
	% brew install --zsh-completion clangsay	# Install
	```

	* cowsayとcowファイルを共有せずにインストールする場合	
	cowsay側のパスではなく、`$(brew --prefix)/cellar/clangsay/<version>/share/clangsay/cows`に対して、cowファイルがインストールされます。	
	本家cowsayと同様に、シェルの環境変数`$COWPATH`にて、動的に切り替えることも可能です。
	```shellsession
	% brew tap 844196/Renge	# Tap
	% brew install clangsay --without-cows	# Install
	```

* Other systems.
	* 全てインストールする場合
	```shellsession
	% make
	# make install
	```

	* 実行ファイルのみをインストールし、cowsayとcowsを共有する場合
	```shellsession
	% make COWPATH="/usr/local/share/cowsay/cows"	# パスは環境に合わせてください	
	# make install-bin
	```

### zshでの補完
\_clangsayの`COWPATH`変数を適時修正した後、`fpath`の通ったディレクトリへ配置して下さい。	

### Usage
基本的にcowsayのオプションと同様です。
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
詳しくは`clangsay --help`をご覧下さい。

### Thanks
* [Masaya Tk](https://github.com/844196)	
	yasunaに続き、clangsayのHomebrew用リポジトリまで提供して頂きました。	
	本当にありがとうございます。


#### License
[WTFPL version 2](http://www.wtfpl.net/txt/copying/)

### Author
sasairc (https://github.com/sasairc)
