# libbindaddr.so

Force `bind(2)` to bind to an address of your choosing.

## Building

`gcc -Wall -fPIC -shared -o libbindaddr.so libbindaddr.c -ldl`

## Usage

`LD_PRELOAD=./libbindaddr.so LIBBINDADDR_V4="127.0.0.1" yourprogram`

After executing the above yourprogram should now bind to the address in
the `LIBBINDADDR_V4` environment variable instead of the address it would
normally bind to.

You may also set the `LIBBINDADDR_V6` environment variable which will be used
if bind is used with an IPv6 address.

Defining `DEBUG` when compiling will result in some (possibly) helpful
messages being printed to stderr if the library ends up returning early.
