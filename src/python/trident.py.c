#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trident.h"

/**
 * Returns NULL upon error
 */
char* get_aligment_segment(const char *sequence)
{
  size_t idx,length;
  size_t start,end;
  char *retval = NULL;
  
  if(sequence == NULL)
    return NULL;
  
  idx = 0;
  length = strlen(sequence);
  for(;idx < length;idx++)
    if(sequence[idx] >= 'A' && sequence[idx] <= 'Z')
      break;
  start = idx;
  
  idx = length;
  for(;idx != 0;idx--)
    if(sequence[idx - 1] >= 'A' && sequence[idx] <= 'Z')
      break;
  end = idx;


  if(end < start)
    return NULL;// MALFORMED STRING

  retval = calloc(end - start +2,sizeof(char));
  
  strncpy(retval,sequence+start,end-start+1);
  return retval;
}

static PyObject *
pytrident_sequence_energy(PyObject *self, PyObject *args)
{
  hit_struct hit;
  char *query_seq,*ref_seq;
  size_t idx;
  double energy;

  idx = 0;
  for(;idx < 6;idx++)
    hit.rest[idx] = NULL;
  for(;idx < 3;idx++)
    hit.alignment[idx] = NULL;
  
  if(!PyArg_Parse(args,"(ss)",&query_seq, &ref_seq))
    return PyErr_Format(PyExc_TypeError,"Invalid Argument. Need 2 sequences.");

  if(query_seq == NULL || ref_seq == NULL)
    return PyErr_SetFromErrno(PyExc_IOError);

  // Get Alignment part of Segment
#if 0 // Is this necessary?!?
  hit.alignment[0] = get_aligment_segment(query_seq);
  if(hit.alignment[0] == NULL)
    {
      return PyError_Format(PyExc_TypeError,"Invalid Query Sequence.");
    }
  hit.alignment[2] = get_aligment_segment(query_seq);
  if(hit.alignment[2] == NULL)
    {
      free(hit.alignment[0]);
      return PyError_Format(PyExc_TypeError,"Invalid Reference Sequence.");
    }
#else
  hit.alignment[0] = query_seq;
  hit.alignment[2] = ref_seq;
#endif
  
  energy = sequence_energy(&hit);
  

#if 0 // if alloc'ing above
  free(hit->aligment[0]);
  free(hit->aligment[2]);
#endif
  return Py_BuildValue("d",energy);
}

static PyMethodDef trident_methods[] = {
  {"sequence_energy", pytrident_sequence_energy, METH_VARARGS,"Takes a microrna and dna sequence and calculates the energy."},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

#if 0// TODO: Adapt for python 3
static PyModuleDef tridentmodule = {
   PyModuleDef_HEAD_INIT,
   "trident", /* name of module */
   "Utility functions for TRIDENT", /* module documentation, may be NULL */
   -1, /* size of per-interpreter state of the module,
or -1 if the module keeps state in global variables. */
   trident_methods
};
#endif

PyMODINIT_FUNC
inittrident(void)
{
  (void) Py_InitModule("trident",trident_methods);
}
