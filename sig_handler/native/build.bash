#!/bin/bash
set -e

FILES="metalogger.c sampler.c logger.c segfault.c handler.c fsutils.c launcher.c"
clang -O2 -g -shared -arch x86_64 $FILES -o handler.x64.dylib
clang -O2 -g -shared -arch arm64  $FILES -o handler.arm64.dylib
lipo -create handler.arm64.dylib  handler.x64.dylib -output handler.dylib

rm handler.arm64.dylib handler.x64.dylib
clang -g main.c $FILES -o main
