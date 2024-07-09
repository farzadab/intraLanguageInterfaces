#!/bin/bash

# Read configuration file
CONFIG_FILE="config.json"
PYTHON_ENV=$(jq -r '.python_env' $CONFIG_FILE)
MODULE_NAME=$(jq -r '.module_name' $CONFIG_FILE)
FUNCTION_NAME=$(jq -r '.function_name' $CONFIG_FILE)
ARGS=$(jq -r '.args | join(" ")' $CONFIG_FILE)


# Update the include paths and the python module name
# gcc runpython.c -I/Users/visheshyadav/anaconda3/include/python3.11 -L/Users/visheshyadav/anaconda3/lib -lpython3.11 -ldl -framework CoreFoundation
gcc runpython.c -I${PYTHON_PATH}/include/python3.11 -L${PYTHON_PATH}/lib -lpython3.11 -ldl -framework CoreFoundation

# Ensure the current directory is in the python path
export PYTHONPATH=".:$PYTHONPATH"

# Set the library path for the dynamic linker
export DYLD_LIBRARY_PATH="/Users/visheshyadav/anaconda3/lib:$DYLD_LIBRARY_PATH"

# UNCOMMENT IT TO RUN OR, YOU CAN USE THE CONFIG FILE
# ./a.out mathUtils multiply 12 5
# ./a.out mathUtils concat_sum_string 12 5 "Hello"
# ./a.out mathUtils process_data 1 2.5 "hello" "[1,2,3]" "{key:value}"


# ./a.out asyncUtils fetch_data "https://jsonplaceholder.typicode.com/todos/1"
# ./a.out asyncUtils fetch_multiple "https://jsonplaceholder.typicode.com/todos/1" "https://jsonplaceholder.typicode.com/todos/2"
# ./a.out asyncUtils post_data "https://jsonplaceholder.typicode.com/posts" "{\"title\": \"foo\", \"body\": \"bar\", \"userId\": 1}"
# ./a.out asyncUtils fetch_with_timeout "https://jsonplaceholder.typicode.com/todos/1" 5



# # Run the C program with arguments from the configuration file
# ./a.out $MODULE_NAME $FUNCTION_NAME $ARGS