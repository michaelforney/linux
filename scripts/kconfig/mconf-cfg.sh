#!/bin/sh
# SPDX-License-Identifier: GPL-2.0

set -eu

cflags=$1
libs=$2

echo -D_GNU_SOURCE > ${cflags}
echo -lcurses -lterminfo > ${libs}
exit 0
