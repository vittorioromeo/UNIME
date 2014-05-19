#!/bin/bash

name="${1}"
intFolder="build"

nasm -f elf32 "./$name" -o "./$intFolder/$name.o" && 
ld -m elf_i386 "./$intFolder/$name.o" -o "./$intFolder/$name.x" && 
"./$intFolder/$name.x"
