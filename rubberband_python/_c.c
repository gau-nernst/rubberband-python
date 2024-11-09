#include <Python.h>
#include <stdio.h>

static PyObject* foo(PyObject* self)
{
    return PyUnicode_FromString("bar");
}

static PyMethodDef methods[] = {
    {"foo", (PyCFunction)foo, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL},
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "our_first_module",
    NULL,
    -1,
    methods,
};

// we need a redirection layer so that a and b can be expanded if needed.
#define CONCAT(a, b) a ## b
#define CONCAT2(a, b) CONCAT(a, b)
PyMODINIT_FUNC CONCAT2(PyInit_, MODULE_NAME)(void) { return PyModule_Create(&module); }
#undef CONCAT2
#undef CONCAT
