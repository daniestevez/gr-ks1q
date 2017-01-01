/* -*- c++ -*- */
/* 
 * Copyright 2016 Daniel Estevez <daniel@destevez.net>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "fec_decoder_impl.h"

extern "C" {
#include <fec.h>
}

namespace gr {
  namespace ks1q {

    fec_decoder::sptr
    fec_decoder::make(bool verbose)
    {
      return gnuradio::get_initial_sptr
        (new fec_decoder_impl(verbose));
    }

    /*
     * The private constructor
     */
    fec_decoder_impl::fec_decoder_impl(bool verbose)
      : gr::block("fec_decoder",
		  gr::io_signature::make(0, 0, 0),
		  gr::io_signature::make(0, 0, 0))
    {
      d_verbose = verbose;
      init_pn9();
      
      message_port_register_out(pmt::mp("out"));
      message_port_register_in(pmt::mp("in"));
      set_msg_handler(pmt::mp("in"),
		      boost::bind(&fec_decoder_impl::msg_handler, this, _1));

    }

    /*
     * Our virtual destructor.
     */
    fec_decoder_impl::~fec_decoder_impl()
    {
    }

    void
    fec_decoder_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
    }

    int
    fec_decoder_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      return 0;
    }

    void
    fec_decoder_impl::msg_handler (pmt::pmt_t pmt_msg) {
      pmt::pmt_t msg = pmt::cdr(pmt_msg);
      uint8_t data[FRAME_LEN];
      uint8_t tmp;
      int rs_res;
      int frame_len;
      size_t offset(0);
      int i;

      memcpy(data, pmt::uniform_vector_elements(msg, offset), sizeof(data));

      for (i = 0; i < FRAME_LEN; i++) data[i] ^= d_pn9[i];
      
      rs_res = decode_rs_ccsds(data, NULL, 0, 0);

      // Send via GNUradio message if RS ok
      if (rs_res >= 0) {
	frame_len = FRAME_LEN - 32 - 1;

	if (d_verbose) {
	  std::printf("RS decode OK. Bytes corrected: %d.\n", rs_res);
	}

	// Send by GNUradio message
	message_port_pub(pmt::mp("out"),
			 pmt::cons(pmt::PMT_NIL,
				   pmt::init_u8vector(frame_len, data)));
      }
      else if (d_verbose) {
	std::printf("RS decode failed.\n");
      }
      
      
    }

    void
    fec_decoder_impl::init_pn9()
    {
	char x[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	int i;

	/* Generate the sequence */
	memset(d_pn9, 0, FRAME_LEN);

	/* The pseudo random sequence shall be generated using the polynomial
	 * h(x) = x8 + x7 + x5 + x3 + 1 */
	for (i = 0; i < FRAME_LEN*8; i++) {
		d_pn9[i/8] = d_pn9[i/8] | x[1] << 7 >> (i%8);
		x[0] = (x[8] + x[6] + x[4] + x[1]) % 2;
		x[1] = x[2];
		x[2] = x[3];
		x[3] = x[4];
		x[4] = x[5];
		x[5] = x[6];
		x[6] = x[7];
		x[7] = x[8];
		x[8] = x[0];
	}
}

  } /* namespace ks1q */
} /* namespace gr */

