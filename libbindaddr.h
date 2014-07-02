/*
 * libbindaddr.so
 *
 * Compile with: gcc -Wall -fPIC -shared -o libbindaddr.so libbindaddr.c -ldl
 * Usage: LD_PRELOAD=./libbindaddr.so LIBBINDADDR_V4="127.0.0.1" yourprogram
 *
 * After executing the above yourprogram should now bind to the address in
 * the LIBBINDADDR_V4 environment variable instead of the address it would
 * normally bind to.
 *
 * You may also set the LIBBINDADDR_V6 environment variable which will be used
 * if bind is used with an IPv6 address.
 *
 * Defining DEBUG when compiling will result in some (possibly) helpful
 * messages being printed to stderr if the library ends up returning early.
 */
#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <dlfcn.h>

#define ENVVAR_V4 "LIBBINDADDR_V4"
#define ENVVAR_V6 "LIBBINDADDR_V6"

#ifdef DEBUG
#include <stdio.h>
#define LOG_PREFIX "libbindaddr: "
#define LOG_OVERRIDE_SUCCESS LOG_PREFIX "Overriding bind '%s' with '%s'\n"
#define LOG_ADDR_CONV LOG_PREFIX "Failed to convert address '%s into "\
	"a usable form.\n"
#define LOG_NO_ENVVAR LOG_PREFIX "Could not get appropriate environment "\
	"variable for binding.\n"
#define LOG_OH_NO LOG_PREFIX "Oh no, how did I get here. I am not good with "\
	"computers.\n"
#define LOG_UNHANDLED_AF LOG_PREFIX "Not handling this address family (%d).\n"
#endif
