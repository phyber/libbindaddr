# libbindaddr.so

Force `bind(2)` to bind to an address of your choosing.

## Building

Simply execute `make` in the source directory. To enable debug output, execute
`make debug` in the source directory.

## Usage

`LD_PRELOAD=./libbindaddr.so LIBBINDADDR_V4="127.0.0.1" yourprogram`

After executing the above yourprogram should now bind to the address in
the `LIBBINDADDR_V4` environment variable instead of the address it would
normally bind to.

You may also set the `LIBBINDADDR_V6` environment variable which will be used
if bind is used with an IPv6 address.
