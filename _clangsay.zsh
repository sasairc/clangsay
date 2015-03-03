#compdef clangsay

function _clangsay() {
	_arguments -s :	\
		{-b,--borg}'[borg mode]' \
		{-d,--dead}'[dead mode]' \
		{-g,--greedy}'[greedy mode]' \
		{-s,--stoned}'[stoned mode]' \
		{-t,--tired}'[tired mode]' \
		{-w,--wired}'[wired mode]' \
		{-y,--youthful}'[youthful mode]' \
		{-e,--eyes}'[manually specifies eyes]':keyword \
		{-T,--tongue}'[manually specifies tongue]':keyword \
		{-f,--file}'[select cow file]:cowfile:_list_cowfile' \
		{-l,--list}'[display cow list]' \
		{-h,--help}'[display help]' \
		{-v,--version}'[output version]'
}

function _list_cowfile() {
	local COWPATH="_COWPATH"

	_files -W $COWPATH && \
		return 0;

	return 1;
}

_clangsay
