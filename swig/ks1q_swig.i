/* -*- c++ -*- */

#define KS1Q_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "ks1q_swig_doc.i"

%{
#include "ks1q/fec_decoder.h"
%}


%include "ks1q/fec_decoder.h"
GR_SWIG_BLOCK_MAGIC2(ks1q, fec_decoder);
