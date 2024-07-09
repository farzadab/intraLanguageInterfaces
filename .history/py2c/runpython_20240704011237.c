#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
    PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    if (argc < 4) {
        fprintf(stderr, "Usage: %s <module_name> <function_name> [args]\n", argv[0]);
        return 1;
    }

    // Initialize the Python interpreter
    if (Py_IsInitialized() == 0) {
        Py_Initialize();
        if (!Py_IsInitialized()) {
            fprintf(stderr, "Failed to initialize Python interpreter\n");
            return 1;
        }
    }

    // Import the module
    pName = PyUnicode_DecodeFSDefault(argv[1]);
    if (pName == NULL) {
        PyErr_Print();
        fprintf(stderr, "Failed to decode module name\n");
        Py_Finalize();
        return 1;
    }

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    if (pModule == NULL) {
        PyErr_Print();
        fprintf(stderr, "Failed to load module \"%s\"\n", argv[1]);
        Py_Finalize();
        return 1;
    }

    // Get the function from the module
    pDict = PyModule_GetDict(pModule);
    if (pDict == NULL) {
        PyErr_Print();
        fprintf(stderr, "Failed to get module dictionary\n");
        Py_DECREF(pModule);
        Py_Finalize();
        return 1;
    }

    pFunc = PyObject_GetAttrString(pModule, argv[2]);
    if (pFunc == NULL || !PyCallable_Check(pFunc)) {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
        Py_Finalize();
        return 1;
    }

    // Build the argument tuple to handle strings, ints, floats, lists, dictionaries, and tuples
    pArgs = PyTuple_New(argc - 3);
    for (i = 0; i < argc - 3; ++i) {
        char *arg = argv[i + 3];
        if (isdigit(arg[0]) || (arg[0] == '-' && isdigit(arg[1]))) {
            // Check if the argument is an integer
            pValue = PyLong_FromLong(atol(arg));
        } else if (strchr(arg, '.')) {
            // Check if the argument is a float
            pValue = PyFloat_FromDouble(atof(arg));
        } else if (arg[0] == '[' && arg[strlen(arg) - 1] == ']') {
            // Check if the argument is a list
            pValue = PyList_New(0);
            char *token = strtok(arg + 1, ",");
            while (token != NULL) {
                PyObject *item = PyUnicode_FromString(token);
                PyList_Append(pValue, item);
                Py_DECREF(item);
                token = strtok(NULL, ",");
            }
        } else if (arg[0] == '{' && arg[strlen(arg) - 1] == '}') {
            // Check if the argument is a dictionary
            pValue = PyDict_New();
            char *token = strtok(arg + 1, ",");
            while (token != NULL) {
                char *key = strtok(token, ":");
                char *value = strtok(NULL, ":");
                PyObject *pyKey = PyUnicode_FromString(key);
                PyObject *pyValue = PyUnicode_FromString(value);
                PyDict_SetItem(pValue, pyKey, pyValue);
                Py_DECREF(pyKey);
                Py_DECREF(pyValue);
                token = strtok(NULL, ",");
            }
        } else if (arg[0] == '(' && arg[strlen(arg) - 1] == ')') {
            // Check if the argument is a tuple
            pValue = PyTuple_New(0);
            char *token = strtok(arg + 1, ",");
            int j = 0;
            while (token != NULL) {
                PyObject *item = PyUnicode_FromString(token);
                PyTuple_SetItem(pValue, j++, item);
                token = strtok(NULL, ",");
            }
        } else {
            // Treat the argument as a string
            pValue = PyUnicode_FromString(arg);
        }
        if (!pValue) {
            Py_DECREF(pArgs);
            Py_DECREF(pModule);
            Py_XDECREF(pFunc);
            PyErr_Print();
            fprintf(stderr, "Cannot convert argument\n");
            Py_Finalize();
            return 1;
        }
        PyTuple_SetItem(pArgs, i, pValue);
    }

    // Measure the start time
    clock_t start_time = clock();

    // Check if the function is a coroutine
    if (PyCoro_CheckExact(pFunc)) {
        // Import asyncio module
        PyObject *asyncio = PyImport_ImportModule("asyncio");
        if (asyncio == NULL) {
            PyErr_Print();
            fprintf(stderr, "Failed to import asyncio module\n");
            Py_DECREF(pArgs);
            Py_DECREF(pModule);
            Py_XDECREF(pFunc);
            Py_Finalize();
            return 1;
        }

        // Get asyncio.run function
        PyObject *asyncio_run = PyObject_GetAttrString(asyncio, "run");
        Py_DECREF(asyncio);
        if (asyncio_run == NULL) {
            PyErr_Print();
            fprintf(stderr, "Failed to get asyncio.run function\n");
            Py_DECREF(pArgs);
            Py_DECREF(pModule);
            Py_XDECREF(pFunc);
            Py_Finalize();
            return 1;
        }

        // Call the coroutine using asyncio.run
        PyObject *coroutine = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);
        if (coroutine == NULL) {
            PyErr_Print();
            fprintf(stderr, "Failed to create coroutine\n");
            Py_DECREF(asyncio_run);
            Py_DECREF(pModule);
            Py_XDECREF(pFunc);
            Py_Finalize();
            return 1;
        }

        pValue = PyObject_CallFunctionObjArgs(asyncio_run, coroutine, NULL);
        Py_DECREF(coroutine);
        Py_DECREF(asyncio_run);
    } else {
        // Call the function
        pValue = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);
    }

    // Measure the end time
    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", time_spent);

    if (pValue != NULL) {
        if (PyLong_Check(pValue)) {
            long result = PyLong_AsLong(pValue);
            printf("Result of call (int): %ld\n", result);
        } else if (PyFloat_Check(pValue)) {
            double result = PyFloat_AsDouble(pValue);
            printf("Result of call (float): %f\n", result);
        } else if (PyUnicode_Check(pValue)) {
            const char *result = PyUnicode_AsUTF8(pValue);
            printf("Result of call (string): %s\n", result);
        } else if (PyList_Check(pValue)) {
            printf("Result of call (list):\n");
            for (Py_ssize_t i = 0; i < PyList_Size(pValue); i++) {
                PyObject *item = PyList_GetItem(pValue, i);
                PyObject *str_item = PyObject_Str(item);
                const char *str = PyUnicode_AsUTF8(str_item);
                printf("  %s\n", str);
                Py_DECREF(str_item);
            }
        } else if (PyDict_Check(pValue)) {
            printf("Result of call (dict):\n");
            PyObject *key, *value;
            Py_ssize_t pos = 0;
            while (PyDict_Next(pValue, &pos, &key, &value)) {
                PyObject *str_key = PyObject_Str(key);
                PyObject *str_value = PyObject_Str(value);
                const char *str_k = PyUnicode_AsUTF8(str_key);
                const char *str_v = PyUnicode_AsUTF8(str_value);
                printf("  %s: %s\n", str_k, str_v);
                Py_DECREF(str_key);
                Py_DECREF(str_value);
            }
        } else {
            printf("Error: Function returned an unsupported type\n");
        }
        Py_DECREF(pValue);
    } else {
        PyErr_Print();
        fprintf(stderr, "Call failed\n");
    }

    // Clean up
    Py_XDECREF(pFunc);
    Py_DECREF(pModule);

    // Finalize the Python interpreter
    if (Py_FinalizeEx() < 0) {
        return 120;
    }

    return 0;
}