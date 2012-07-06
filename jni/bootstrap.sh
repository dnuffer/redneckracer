#!/bin/sh

touch NEWS README AUTHORS ChangeLog
autoreconf -fvi
./configure --enable-maintainer-mode --enable-debug-mode
make
