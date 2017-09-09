#
# かんたんなてすと
#

CLANGSAY="./clangsay"
COWPATH="../cows/asciiart:../cows/pixelart"
DEFAULT_COWFILE="default.cow"

function test0() {
	${CLANGSAY} --version								&& \
		${CLANGSAY} --help								&& \
		${CLANGSAY} --list								&& \
		${CLANGSAY} 'arg0' 'arg1' 'arg2' 'arg4'			&& \
		echo 'stdin' | ${CLANGSAY}						&& \
		${CLANGSAY} -f zen-noh-milk 'filename'			&& \
		${CLANGSAY} -f zen-noh-milk.cow 'filename.cow'	&& \
		return 0

	return 1
}


function test1() {
	${CLANGSAY} -b 'borg'				&& \
		${CLANGSAY} -d 'dead'			&& \
		${CLANGSAY} -g 'greedy'			&& \
		${CLANGSAY} -p 'paranoid'		&& \
		${CLANGSAY} -s 'stoned'			&& \
		${CLANGSAY} -t 'tired'			&& \
		${CLANGSAY} -w 'wired'			&& \
		${CLANGSAY} -y 'youthful'		&& \
		${CLANGSAY} -e "目" 'eye'		&& \
		${CLANGSAY} -T "舌" 'tongue'	&& \
		${CLANGSAY} --think 'think'		&& \
		${CLANGSAY} --say 'say'			&& \
		${CLANGSAY} -R 32 'recursive'
		return 0

	return 1
}

function test2() {
	touch ../testsuite/pdfile
	chmod a-r ../testsuite/pdfile

	${CLANGSAY} -R NOINT					|| \
		${CLANGSAY} -f /nofile				|| \
		${CLANGSAY} -f /					|| \
		${CLANGSAY} -f ../testsuite/pdfile  || \
		return 0

	return 1
}

#(test0 && test1 && test2) > /dev/null 2>&1
test0 && test1 && test2
