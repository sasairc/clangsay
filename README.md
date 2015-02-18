clangsay
=======
[![version](https://img.shields.io/badge/tag-0.0.3-orange.svg?style=flat)](https://github.com/sasairc/yasuna/releases)
[![license](https://img.shields.io/badge/License-WTFPL2-blue.svg?style=flat)](http://www.wtfpl.net/txt/copying/)	

![default](http://41.media.tumblr.com/d93abecb6fe04a8f6d1c38deba2e963a/tumblr_njt1q0EsZJ1u2jamko1_1280.png)

マルチバイト準対応のcowsayクローン。  
ただし、半角カナや特殊文字が含まれる場合は、相変わらずズレてしまいます。	
cowsayの`-W`オプションに該当するものはありません。（常に`-n`つきの状態）	

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

#### License
[WTFPL version 2](http://www.wtfpl.net/txt/copying/)

### Author
sasairc (https://github.com/sasairc)
