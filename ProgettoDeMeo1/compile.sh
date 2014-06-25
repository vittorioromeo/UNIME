#!/bin/bash

clang -O3 -DNDEBUG -lm ./$1 -o /tmp/$1.temp && /tmp/$1.temp