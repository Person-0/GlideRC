# GlideRC Flight Controller Firmware

This is the firmware source code for GlideRC that will be running on the Pico 2 (RP2350).<br>

## Tests directory
The `tests` directory contains tests for modules in [`/src`](./src/). These tests can be compiled with GCC and ran on a PC.

To make running tests easier, I use a [`.prayer`](./tests/.prayer) file (see [pray](https://github.com/Person-0/pray)). This lets you run commands like:

```bash
pray sbus
```

instead of typing the full GCC command to build and run the `sbus.c` test.