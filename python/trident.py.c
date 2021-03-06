#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trident.h"

static PyObject *
pytrident_compliment(PyObject *self, PyObject *args)
{
  extern void compliment(char *reference_sequence,size_t reference_length);
  char *seq, *new_seq;
  PyObject *retval = Py_None;

  if(!PyArg_Parse(args,"(s)",&seq) || seq == NULL)
    return PyErr_Format(PyExc_TypeError,"Invalid Argument. Need 1 sequence");

  new_seq = calloc(strlen(seq),sizeof(char)+1);
  if(new_seq == NULL)
    return PyErr_Format(PyExc_MemoryError,"Could not allocate memory for sequence compliment.");

  strcpy(new_seq, seq);

  compliment(new_seq,strlen(new_seq));

  retval = Py_BuildValue("s",new_seq);

  free(new_seq);
  
  return retval;

}


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
    if(sequence[idx - 1] >= 'A' && sequence[idx-1] <= 'Z')
      break;
  end = idx-1;


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
  double energy = 42.0;
  extern double sequence_energy(const hit_struct* hit);

  idx = 0;
  for(;idx < 6;idx++)
    hit.rest[idx] = NULL;
  for(;idx < 3;idx++)
    hit.alignment[idx] = NULL;
  
  if(!PyArg_Parse(args,"(ssi)",&query_seq, &ref_seq,&current_match_type))
    return PyErr_Format(PyExc_TypeError,"Invalid Argument. Need 2 sequences and a match type.");

  if(query_seq == NULL || ref_seq == NULL)
    return PyErr_SetFromErrno(PyExc_IOError);

  // Get Alignment part of Segment
#if 1 // Is this necessary?!?
  hit.alignment[0] = get_aligment_segment(query_seq);
  if(hit.alignment[0] == NULL)
    {
      return PyErr_Format(PyExc_TypeError,"Invalid Query Sequence.");
    }
  hit.alignment[2] = get_aligment_segment(ref_seq);
  if(hit.alignment[2] == NULL)
    {
      free(hit.alignment[0]);
      return PyErr_Format(PyExc_TypeError,"Invalid Reference Sequence.");
    }
#else
  hit.alignment[0] = query_seq;
  hit.alignment[2] = ref_seq;
#endif
  
  energy = sequence_energy(&hit);

#if 1 // if alloc'ing above
  free(hit.alignment[0]);
  free(hit.alignment[2]);
#endif
  return Py_BuildValue("f",energy);
}

static PyMethodDef trident_methods[] = {
  {"sequence_energy", pytrident_sequence_energy, METH_VARARGS,"Takes a microrna and dna sequence and calculates the energy."},
  {"compliment", pytrident_compliment, METH_VARARGS, "Takes a sequence and returns the compliment."},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3
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
PyInit_core(void)
{
#if PY_MAJOR_VERSION >= 3
  PyObject* module = PyModule_Create(&tridentmodule);

  if(module == NULL)
    return module;
#else
  PyObject* module = Py_InitModule("core",trident_methods);

  if(module == NULL)
    return;
#endif
  initialize_bases();  

  PyModule_AddIntConstant(module,"MATCH_MIRANDA",MATCH_MIRANDA);
  PyModule_AddIntConstant(module,"MATCH_DIRECT_REVERSE_HOOGSTEEN",MATCH_DIRECT_REVERSE_HOOGSTEEN);
  PyModule_AddIntConstant(module,"MATCH_INDIRECT_REVERSE_HOOGSTEEN",MATCH_INDIRECT_REVERSE_HOOGSTEEN);
  PyModule_AddIntConstant(module,"MATCH_DIRECT_HOOGSTEEN",MATCH_DIRECT_HOOGSTEEN);
  PyModule_AddIntConstant(module,"MATCH_INDIRECT_HOOGSTEEN",MATCH_INDIRECT_HOOGSTEEN);

#if PY_MAJOR_VERSION >= 3
  return module;
#endif
}

#if PY_MAJOR_VERSION == 2
PyMODINIT_FUNC initcore(){PyInit_core();}
#endif
