#
# かんたんなてすと
#

function cleanup() {
    echo "*** cleanup directory ***"
    make clean && \
        rm -r "$1"

    return $?
}

function install_all() {
    echo "*** target install-all ***"
    make CC="$1" PREFIX="$2" install && \
        tree "$2"

    return $?
}

function install_bin() {
    echo "*** target install-bin ***"
    make CC="$1" PREFIX="$2" install-bin && \
        tree "$2"

    return $?
}

function install_cows() {
    echo "*** target install-cows ***"
    make CC="$1" PREFIX="$2" install-cows && \
        tree "$2"

    return $?
}

function install_cows_asciiart() {
    echo "*** target install-cows-asciiart ***"
    make CC="$1" PREFIX="$2" install-cows-asciiart && \
        tree "$2"

    return $?
}

function install_cows_pixelart() {
    echo "*** target install-cows-pixelart ***"
    make CC="$1" PREFIX="$2" install-cows-pixelart && \
        tree "$2"

    return $?
}

function install_man() {
    echo "*** target install-man ***"
    LANG="ja_JP.UTF-8" make CC="$1" PREFIX="$2" install-man && \
        tree "$2"

    return $?
}

function install_zsh_compdef() {
    echo "*** target install-zsh-compdef ***"
    make CC="$1" PREFIX="$2" install-zsh-compdef && \
        tree "$2"

    return $?
}
