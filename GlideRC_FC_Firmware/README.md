# GlideRC Flight Controller Firmware

This is the firmware source code for GlideRC that will be running on the Pico 2 (RP2350).<br>

Currently, It is set to be compiled with RP2040 (Pico) since I had one of those lying around and wanted to test whether it compiles successfully or not.

Since the same code can be compiled for both Pico and Pico 2, It should be easy to switch the board using the "Select Board" option at the bottom right corner of VS Code, if using the [Raspberry Pi Pico extension](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico) (which is **highly recommended**).

## Tests directory
The tests directory contains tests for the codebase. These tests can be compiled with GCC and ran on a PC and help with combating logical or runtime errors.

To make running tests easier, I use a [`.prayer`](./tests/.prayer) file.
> [pray](https://github.com/Person-0/pray) is a terminal program that you can add in your PATH which runs commands specified in a file in the same directory.

This lets you run commands like:

```bash
pray sbus
```

Instead of typing the full GCC command to build and run the `sbus.c` test.

## Available tests

- `sbus`: Injects custom sbus packets using `inject_sbus_byte` (see [hardware.c](./src/hardware.c)) with proper debug logs.
- `main`: Tests the main code flow by mocking `hardware.c` (see [mocks/hardware.c](./tests/mocks/hardware.c)).

Please do check the [`.prayer`](./tests/.prayer) file too incase I've forgotten to update them here. <br>
Run these tests by cd ing into /tests and running `pray test_name`.

## Compiling / Running the code

This *should* be fairly easy using [VS Code](https://code.visualstudio.com/).

- Install the [Raspberry Pi Pico extension](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico)
- Directly open the firmware folder
- The extension auto downloads required dependencies and toolchain
- Use the `Compile` or `Run` button at bottom right to Compile the code or Run it respectively.
> Make sure that the board selected matches with what you have.<br>
> Code compiled for Pico (RP2040) will not run on Pico 2 (RP2350) and vice versa.

Also See:
- [Getting started with C/C++ development with Raspberry Pi Pico-series and other
Raspberry Pi microcontroller-based boards](https://pip-assets.raspberrypi.com/categories/610-raspberry-pi-pico/documents/RP-008276-DS-1-getting-started-with-pico.pdf)
- [Raspberry Pi C/C++ SDK](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html)