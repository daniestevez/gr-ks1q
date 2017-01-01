# gr-ks1q
KS1-Q GNU Radio decoder

You can find a complete receivers for KS-1Q
using this software in [gr-satellites](https://github.com/daniestevez/gr-satellites).

You need to install Phil Karn's KA9Q libfec.
This [libfec fork](https://github.com/daniestevez/libfec) builds correctly on
modern systems.

You also need to install the following OOT modules:
  * [gr-synctags](https://github.com/daniestevez/gr-synctags)
  * [gr-csp](https://github.com/daniestevez/gr-csp)
  * [gr-kiss](https://github.com/daniestevez/gr-kiss)

To test the decoders you can open the files in the `examples/` folder with
`gnuradio-companion`.