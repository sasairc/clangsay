clangsay
=======
[![license](https://img.shields.io/badge/License-WTFPL2-blue.svg?style=flat)](http://www.wtfpl.net/txt/copying/)	

![default](http://41.media.tumblr.com/dda874e62f22a91a49839874676e3df1/tumblr_njoy8cC8CO1u2jamko1_1280.png)

マルチバイト準対応の*cowsay*クローン（半角カナは駄目）	
cowsayの`-W`オプションに該当するものはありません。（常に`-n`つきの状態）	

### Build
普通に`make`をします。	

* 全てインストールする場合
```shellsession
% make
# make install
```

* 実行ファイルだけをインストールし、cowsayとcowsを共有する場合
```shellsession
% make COWPATH="/usr/local/share/cowsay/cows"
# make install-bin
```

### Usage
* マルチバイト対応のcowsay的な使い方
```shellsession
  % export COWPATH="/usr/share/cowsay/cows"	# COWPATHを指定したい場合
  % yasuna --list | head -n 10 | clangsay
   ___________________________________________
  / 0 100円くらいはもらえると思ったのに！      \
  | 1 100円払うから喋らせて                    |
  | 2 DVD！？                                  |
  | 3 UFO・・・とか・・・                      |
  | 4 Yes                                      |
  | 5 Yes!!                                    |
  | 6 「いつも迷惑かけてすいません」           |
  | 7 「お笑い写真部門」                       |
  | 8 「千里眼の術」・・・                     |
  \ 9 「心の冷たい人は手が温かい」の噂の真相は /
   -------------------------------------------
          \   ^__^
           \  (oo)\_______
              (__)\       )\\/\\
                  ||----w |
                  ||     ||
```

#### License
[WTFPL version 2](http://www.wtfpl.net/txt/copying/)

### Author
sasairc (https://github.com/sasairc)
