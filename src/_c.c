#include <Python.h>

static PyMethodDef methods[] = {
    {NULL, NULL, 0, NULL},
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    Py_STRINGIFY(MODULE_NAME),
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
