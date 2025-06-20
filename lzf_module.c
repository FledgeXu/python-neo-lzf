#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "lzf.h"
#include <errno.h>

#define PYBYTES_FSAS PyBytes_FromStringAndSize

static PyObject *python_compress(PyObject *self, PyObject *args) {
  char *input, *output;
  Py_ssize_t inlen;
  PyObject *pyoutlen = Py_None;
  long outlen;
  PyObject *result;

  if (!PyArg_ParseTuple(args, "s#|O", &input, &inlen, &pyoutlen))
    return NULL;

  if (pyoutlen == Py_None) {
    outlen = inlen - 1;
  } else if (PyLong_CheckExact(pyoutlen)) {
    outlen = PyLong_AsLong(pyoutlen);
  } else {
    PyErr_SetString(PyExc_TypeError, "max_len must be an integer");
    return NULL;
  }

  if (inlen == 1)
    outlen++; // workaround for liblzf
  if (outlen <= 0) {
    PyErr_SetString(PyExc_ValueError, "max_len must be > 0");
    return NULL;
  }

  output = (char *)malloc(outlen + 1);
  if (!output) {
    PyErr_SetString(PyExc_MemoryError, "out of memory");
    return NULL;
  }

  outlen = lzf_compress(input, inlen, output, outlen + 1);

  if (outlen)
    result = PYBYTES_FSAS(output, outlen);
  else {
    Py_XINCREF(Py_None);
    result = Py_None;
  }

  free(output);
  return result;
}

static PyObject *python_decompress(PyObject *self, PyObject *args) {
  char *input, *output;
  Py_ssize_t inlen;
  long outlen;
  PyObject *result;

  if (!PyArg_ParseTuple(args, "s#l", &input, &inlen, &outlen))
    return NULL;

  if (outlen < 0) {
    PyErr_SetString(PyExc_ValueError, "max_len cannot be less than 0");
    return NULL;
  }

  output = (char *)malloc(outlen);
  if (!output) {
    PyErr_SetString(PyExc_MemoryError, "out of memory");
    return NULL;
  }

  outlen = lzf_decompress(input, inlen, output, outlen);

  if (outlen)
    result = PYBYTES_FSAS(output, outlen);
  else {
    if (errno == EINVAL) {
      PyErr_SetString(PyExc_ValueError, "error in compressed data");
      free(output);
      return NULL;
    }
    Py_XINCREF(Py_None);
    result = Py_None;
  }

  free(output);
  return result;
}

static PyMethodDef methods[] = {
    {"compress", python_compress, METH_VARARGS,
     "compress(input, max_length=None)\n\nReturn the compressed string, or "
     "None if it doesn't compress smaller."},
    {"decompress", python_decompress, METH_VARARGS,
     "decompress(input, max_length)\n\nReturn the decompressed string, or "
     "raise error on failure."},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef lzfmodule = {
    PyModuleDef_HEAD_INIT, "lzf", NULL, -1, methods, NULL, NULL, NULL, NULL};

PyMODINIT_FUNC PyInit_lzf(void) { return PyModule_Create(&lzfmodule); }
