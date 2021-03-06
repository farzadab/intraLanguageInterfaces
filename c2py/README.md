# C++ to Python interface

Using the following instructions you can call your **existing C/C++ modules** from Python. There are multiple ways you can do this, but here I'm using _[SWIG](http://www.swig.org/index.php) for the truly lazy_ following instructions that can be found [here](http://www.swig.org/tutorial.html).

## Installing SWIG
You need to install SWIG:
- you can download it from [here](http://www.swig.org/download.html);
- or install it using a package manager:
```bash
sudo apt install -y swig3.0   # Ubuntu, Elementary, ...
sudo pacman -S --noconfirm swig  # Arch, Manjaro, ...
sudo yum install swig  # Fedora, CentOS, Redhat
```

## TL;DR
The scripts `run_c.sh` and `run_cpp.sh` do everything explained here for C and C++ languages, respectively. You can just run them after installing SWIG and they should do everything, hopefully without a problem.


## Step by step example
The codes in this repo represent a (hopefully) working example. The instructions assume you are using C, but using C++ is not much different and the differences are explained at each step. Also refer to `run_c.sh` and `run_cpp.sh` if you get confused. I also try to specify which files each command produces at each step:

0. Have your C/C++ code ready: `swig_example.c`
1. Create a simple header file that exposes the functionality you want to use (like `header.h`)
2. Create a simple interface file using the previous header file (like `swig_example.i`)
3. Build the C/C++ wrapper:
    ```bash
    swig3.0 -python swig_example.i  # outputs `swig_example_wrap.c`
    ```
    in C++ you have to add the `-c++` option as well.
    ```bash
    swig3.0 -python -c++ swig_example.i  # outputs `swig_example_wrap.cxx`
    ```
4. Compile the code to get the `.so` file:
    ```bash
    gcc -fPIC -c swig_example.c swig_example_wrap.c -I /usr/include/python3.5/  # outputs `swig_example.o` and `swig_example_wrap.o`
    ```
    in C++ you need to use `g++` instead:
    ```bash
    g++ -fPIC -c swig_example.cpp swig_example_wrap.cxx -I /usr/include/python3.5/  # outputs `swig_example.o` and `swig_example_wrap.o`
    ```
    - If you have a C++ code, just use `g++` instaed of `gcc`
    - These commands might not work out of the box in your system. These are some hints that might help you in that case:
        - the include paths and the python module name might be different
        - you can use `locate include/python` to locate the include path
        - system-wide installations of python usually use `/usr/include/` or `/usr/local/include/` but Anaconda normally uses the home directory, e.g. `/home/<user>/miniconda3/include/`
5. build the `.so` file:
    ```bash
    ld -shared swig_example.o swig_example_wrap.o -o _swig_example.so  # outputs `_swig_example.so`
    ```
    in C++ you have to use `g++` here as well:
    ```bash
    g++ -shared swig_example.o swig_example_wrap.o -o _swig_example.so  # outputs `_swig_example.so`
    ```
6. Use it in python!
    ```bash
    python3 -c "import swig_example; print('Result of computing factorial of 5 (implemented in C++) from Python:', swig_example.fact(5))"
    ```
    or simply open the Python interpreter and run the following:
    ```python
    import swig_example
    swig_example.fact(10)
    ```