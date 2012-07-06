#!/bin/sh
python ./cpplint.py \
	--filter=-whitespace/tab,\
-whitespace/braces,\
-whitespace/blank_line,\
-whitespace/line_length,\
-whitespace/parens,\
-whitespace/comments,\
-whitespace/labels,\
-whitespace/end_of_line,\
-whitespace/newline \
	jni/game/RedneckRacerGame.cpp
