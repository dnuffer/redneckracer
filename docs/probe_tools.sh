#!/bin/sh

set -x
set -u

toolchain_needs_exception_flags()
{
	if echo "$1" | grep -- '-cc$' >/dev/null; then
		return 1
	elif echo "x$1" | grep crystax > /dev/null; then
		return 0
	elif echo "x$1" | grep 4.4.3 >/dev/null; then
		return 0
	fi
	return 1
}

run_compiler()
{
	flags=""
	if toolchain_needs_exception_flags "$1"; then
		flags="-frtti -fexception"
	fi
	"$1" -dM -E ${flags} - </dev/null
}

check_toolchain()
{
	for basedir in "$@"; do
		if [ -d "${basedir}/build" ]; then
			basedir="${basedir}/build"
		fi

		for compbase in "cc" "c++"; do
			for compiler in `find "${basedir}" -name "*-${compbase}"`; do
				ndkname=`echo x${compiler} | tr '/' '\n' | grep ndk`
				abiname=`echo x${compiler} | tr '/' '\n' | grep abi | grep -v ${compbase}`
				run_compiler "${compiler}" > defines.${ndkname}.${abiname}.${compbase}.txt
			done
		done
	done
}

check_toolchain /usr/local/android-ndk*
