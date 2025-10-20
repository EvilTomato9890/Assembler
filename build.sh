#!/usr/bin/env bash
set -e
cd "$(dirname "$0")/Assembler-New_structure"
mkdir -p bin build/compiler build/cpu
CXX=${CXX:-g++}
CF='-std=c++17 -I include -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -fstack-protector-strong -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -Wall -DCANARY_DEBUG -DHASH_DEBUG -DLOGGER_ALL -c'
LF='-fsanitize=address,undefined,float-divide-by-zero,float-cast-overflow -fsanitize=null,bounds,alignment,object-size,return,vptr,leak -fsanitize-address-use-after-scope -no-pie'

# assembler -> build/compiler
for f in source/common/*.cpp; do [ -e "$f" ] || continue; [ "$(basename "$f")" = main.cpp ] && continue; b=${f##*/}; $CXX $CF "$f" -o build/compiler/${b%.cpp}.o; done
for f in source/compiler/*.cpp; do [ -e "$f" ] || continue; b=${f##*/}; $CXX $CF "$f" -o build/compiler/${b%.cpp}.o; done
$CXX $LF build/compiler/*.o -o bin/assembler

# processor -> build/cpu
for f in source/common/*.cpp; do [ -e "$f" ] || continue; [ "$(basename "$f")" = main.cpp ] && continue; b=${f##*/}; $CXX $CF "$f" -o build/cpu/${b%.cpp}.o; done
for f in source/cpu/*.cpp; do [ -e "$f" ] || continue; b=${f##*/}; $CXX $CF "$f" -o build/cpu/${b%.cpp}.o; done
$CXX $LF build/cpu/*.o -lm -o bin/processor
