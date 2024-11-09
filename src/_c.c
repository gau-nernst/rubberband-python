#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <rubberband/rubberband-c.h>

static PyObject *py_rubberband(PyObject *self, PyObject *args) {
  Py_buffer in_buf;
  int num_channels;
  int sample_rate;
  double time_ratio;
  double pitch_scale;
  int fast;
  Py_buffer out_buf;

  // see https://docs.python.org/3/c-api/arg.html
  if (
    !PyArg_ParseTuple(
      args,
      "y*iiddpy*",
      &in_buf,
      &num_channels,
      &sample_rate, 
      &time_ratio,
      &pitch_scale,
      &fast,
      &out_buf
    )
  )
    return NULL;

  Py_BEGIN_ALLOW_THREADS;

  const int num_in_frames = in_buf.len / sizeof(float) / num_channels;
  const int num_out_frames = out_buf.len / sizeof(float) / num_channels;

  // set options according to https://breakfastquay.com/rubberband/integration.html
  RubberBandOptions options = RubberBandOptionProcessOffline
                            | RubberBandOptionEngineFiner
                            | RubberBandOptionChannelsTogether;
  if (!fast) {  // higher quality, slower
    if (pitch_scale != 1.0)
      options |= RubberBandOptionPitchHighQuality
               | RubberBandOptionFormantPreserved;
  }
  else {  // lower quality, faster
    options |= RubberBandOptionWindowShort;
  }
  RubberBandState state = rubberband_new(sample_rate, num_channels, options, time_ratio, pitch_scale);

  const int bsize = 1 << 14;
  rubberband_set_expected_input_duration(state, num_in_frames);
  rubberband_set_max_process_size(state, bsize);

  // in_channels[i] holds pointer to channel i
  float **in_channels = malloc(sizeof(float *) * num_channels);
  float **out_channels = malloc(sizeof(float *) * num_channels);
  for (int i = 0; i < num_channels; i++) {
    in_channels[i] = (float *)in_buf.buf + i * num_in_frames;
    out_channels[i] = (float *)out_buf.buf + i * num_out_frames;
  }

  rubberband_study(state, in_channels, num_in_frames, 1);
  for (int offset = 0; offset < num_in_frames; offset += bsize) {
    const int final = offset + bsize >= num_in_frames;
    const int current_bsize = final ? (num_in_frames - offset) : bsize;

    rubberband_process(state, in_channels, current_bsize, final);

    // increment pointer of each input channel
    for (int c = 0; c < num_channels; c++)
      in_channels[c] += bsize;

    int avail;
    while ((avail = rubberband_available(state)) > 0) {
      rubberband_retrieve(state, out_channels, avail);

      // increment pointer of each output channel
      for (int c = 0; c < num_channels; c++)
        out_channels[c] += avail;
    }
  }

  free(out_channels);
  free(in_channels);
  rubberband_delete(state);
  Py_END_ALLOW_THREADS;

  PyBuffer_Release(&in_buf);
  PyBuffer_Release(&out_buf);

  Py_INCREF(Py_None);
  return Py_None;
}

static PyMethodDef methods[] = {
  {"rubberband", py_rubberband, METH_VARARGS, NULL},
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
