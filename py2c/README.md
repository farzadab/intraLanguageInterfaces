# Python to C++ interface

Using the following instructions you can call your **existing Python modules** from C/C++. I'm using Pure Embedding instructions taken from [here](https://docs.python.org/release/3.5.5/extending/embedding.html#pure-embedding). This is not a general solution and needs a little bit more work, but using this example code you can run any Python function from C/C++ and even pass arguments (only supports integer values). Hopefully you can adapt it to your situataion.


## Runnable script
Using the `run.sh` script, the `runpython.c` code calls the `multiply` function from the `mathUtils.py` module and gets back the results. you can play around with the last command and call your own Python modules in the following way:
```bash
./a.out <module_name> <function_name> <integer_arguments_list>
```

To integrate this into your own program, you can use `runpython.c` as a template, but need to handle your Python object in C using `PyObject`. This can represent any Python object, but doing it is needs some coding. Please refer to [Python/C Api Reference Manual](https://docs.python.org/release/3.5.5/c-api/index.html#c-api-index) for more information on how to do this.