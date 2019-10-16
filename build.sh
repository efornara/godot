#! /bin/sh
set -e

die() {
	echo $1
	exit 1
}

usage() {
	die "usage: build.sh [nolto | nice | debug | lto | frt | all]"
}

[ -x build.sh ] || die "Please run from project root directory."
case $# in
	0) arg="nolto" ;;
	1) arg="$1" ;;
	*) usage
esac

common_opts="\
	use_llvm=yes \
	builtin_zlib=no \
	builtin_freetype=no \
	builtin_libpng=no \
	builtin_libogg=no \
	builtin_libvorbis=no \
	builtin_opus=no \
	builtin_libglew=no \
	builtin_libwebp=no \
	builtin_openssl=no \
	"

ccflags_pi2="\
	-mcpu=cortex-a7 \
	-mfpu=neon-vfpv4 \
	-mfloat-abi=hard \
	-mlittle-endian \
	-munaligned-access \
	"

editor_opts="\
	$common_opts \
	platform=x11 \
	tools=yes \
	builtin_libsquish=no \
	builtin_libtheora=no \
	"

export BUILD_REVISION=rpi

case $arg in
	nolto)
		nice scons \
			$editor_opts \
			target=release_debug \
			use_lto=no \
			extra_suffix=nolto \
			CCFLAGS="$ccflags_pi2" \
			-j 4
		;;
	nice)
		nice scons \
			$editor_opts \
			target=release_debug \
			use_lto=no \
			extra_suffix=nolto \
			CCFLAGS="$ccflags_pi2" \
			-j 1
		;;
	debug)
		nice scons \
			$editor_opts \
			target=debug \
			use_lto=no \
			CCFLAGS="$ccflags_pi2" \
			-j 4
		;;
	lto)
		nice scons \
			$editor_opts \
			target=release_debug \
			use_lto=yes \
			extra_suffix=lto \
			CCFLAGS="$ccflags_pi2" \
			-j 4
		;;
	frt)
		nice scons \
			platform=frt \
			frt_arch=pi1 \
			target=release \
			use_lto=yes \
			$common_opts \
			builtin_libtheora=yes \
			-j 4
		;;
	all)
		for i in nolto debug lto frt ; do
			./build.sh $i
			sleep 5m
		done
		;;
	*) usage
esac
