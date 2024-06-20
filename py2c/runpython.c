#include <Python.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    if (argc < 4) {
        fprintf(stderr, "Usage: %s pythonfile funcname [args]\n", argv[0]);
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

    // Build the argument tuple
    pArgs = PyTuple_New(argc - 3);
    for (i = 0; i < argc - 3; ++i) {
        pValue = PyLong_FromLong(atol(argv[i + 3]));
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

    // Call the function
    pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);
    if (pValue != NULL) {
        printf("Result of call: %ld\n", PyLong_AsLong(pValue));
        Py_DECREF(pValue);
    } else {
        PyErr_Print();
        fprintf(stderr, "Function call failed\n");
        Py_DECREF(pModule);
        Py_XDECREF(pFunc);
        Py_Finalize();
        return 1;
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
