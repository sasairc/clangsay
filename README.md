clangsay
=======
[![license](https://img.shields.io/badge/License-WTFPL2-blue.svg?style=flat)](http://www.wtfpl.net/txt/copying/)	
マルチバイト準対応の*cowsay*クローン（半角カナは駄目）

### Build
適時Makefileを弄って`make`する

### Usage
* マルチバイト対応のcowsay的な使い方
```shellsession
  % export COWPATH="/usr/share/cowsay/cows"
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
