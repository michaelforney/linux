#!/bin/sh
# SPDX-License-Identifier: LGPL-2.1

[ $# -eq 1 ] && header_dir=$1 || header_dir=tools/include/uapi/sound/

printf "static const char *sndrv_ctl_ioctl_cmds[] = {\n"
grep "^#define[	 ]\+SNDRV_CTL_IOCTL_" $header_dir/asound.h | \
	sed -E 's/^#define +SNDRV_CTL_IOCTL_([A-Z0-9_]+)[	 ]+_IO[RW]*\( *.U., *(0x[[:xdigit:]]+),?.*/	[\2] = \"\1\",/g'
printf "};\n"
