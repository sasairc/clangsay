clangsay
=======
[![license](https://img.shields.io/badge/License-WTFPL2-blue.svg?style=flat)](http://www.wtfpl.net/txt/copying/)	

![default](http://41.media.tumblr.com/d93abecb6fe04a8f6d1c38deba2e963a/tumblr_njt1q0EsZJ1u2jamko1_1280.png)

マルチバイト準対応のcowsayクローン。  
ただし、半角カナや特殊文字が含まれる場合は、相変わらずズレてしまいます。	
cowsayの`-W`オプションに該当するものはありません。（常に`-n`つきの状態）	

### Build & Install
普通に`make`をします。	

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
