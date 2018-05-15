#!/bin/bash

# SWIG for the truly lazy (http://www.swig.org/tutorial.html)
# 0. have your c/c++ code: swig_example.c
# 1. Create a simple header file (like `header.h`)
# 2. Create a simple interface file using the previous header file (like `swig_example.i`)
# 3. Build c/c++ wrapper:
swig3.0 -python swig_example.i  # outputs `swig_example_wrap.c`
# 4. Compile the code to get the `.so` file
gcc -fPIC -c swig_example.c swig_example_wrap.c -I/usr/include/python3.5/  # outputs `swig_example.o` and `swig_example_wrap.o`
# HINT: the include paths and the python module name might be different
# HINT: use `locate libpython` to locate the library and similarly for include path
# 5. build the `.so` file
ld -shared swig_example.o swig_example_wrap.o -o _swig_example.so  # outputs `_swig_example.so`
# 5. Use it in python
python3 -c "import swig_example; print('Result of computing factorial of 5 (implemented in C++) from Python:', swig_example.fact(5))"
# HINT: or simply open the python interpreter and run the following:
# import swig_example
# swig_example.fact(10)
