#!/bin/bash -eux
# -e: Exit immediately if a command exits with a non-zero status.
# -u: Treat unset variables as an error when substituting.
# -x: Display expanded script commands

make SHELL=/bin/bash
make SHELL=/bin/bash prefix="${INSTALL_DIR}" install

find "${INSTALL_DIR}" -type f -perm -a=x -exec m68k-atari-mint-strip -s {} \;
find "${INSTALL_DIR}" -type f \( -name '*.a' -o -name '*.o' \) -exec m68k-atari-mint-strip -S -X -w -N '.L[0-9]*' {} \;
