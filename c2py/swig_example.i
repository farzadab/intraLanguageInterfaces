%module swig_example
%{
/* Includes the header in the wrapper code */
#include "header.h"
%}

/* Parse the header file to generate wrappers */
%include "header.h"