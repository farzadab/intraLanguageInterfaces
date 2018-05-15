#!/bin/bash

# the include paths and the python module name might be different
# use `locate libpython` to locate the library and `locate include/python` for include path
gcc runpython.c -I/usr/include/python3.5 -lpython3.5m


# not sure why, but the current directory is not in the python path when you run this
export PYTHONPATH=".:$PYTHONPATH"
./a.out mathUtils multiply 12 5
