#include "libbindaddr.h"

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	// Find the original bind.
	int (*original_bind)(int, const struct sockaddr*, socklen_t);

	// original_bind = dlsym(RTLD_NEXT, "bind"); causes gcc -pedantic to
	// complain. This cast fixes that.
	*(void **) (&original_bind) = dlsym(RTLD_NEXT, "bind");

	// Get the appropriate environment variable if we're handling that AF.
	// Call the bind with original arguments if we aren't handling it.
	char *env_bind_address;
	switch(addr->sa_family) {
		case AF_INET:
			env_bind_address = getenv(ENVVAR_V4);
			break;
		case AF_INET6:
			env_bind_address = getenv(ENVVAR_V6);
			break;
		default:
#if DEBUG
			fprintf(stderr, LOG_UNHANDLED_AF, addr->sa_family);
#endif
			return (*original_bind)(sockfd, addr, addrlen);
	}

	// No address to bind to? Just call bind with original arguments.
	if (env_bind_address == NULL) {
#if DEBUG
		fprintf(stderr, LOG_NO_ENVVAR);
#endif
		return (*original_bind)(sockfd, addr, addrlen);
	}

	// Cast sockaddr to a sockaddr_in.
	struct sockaddr_in *bind_sockaddr = (struct sockaddr_in *)addr;

	// Storage size up to the max required for an IPv6 address.
	unsigned char dst[sizeof(struct in6_addr)];
	int ret = inet_pton(addr->sa_family, env_bind_address, dst);

	// Call bind with the original arguments if inet_pton fails.
	if (ret != 1) {
#if DEBUG
		fprintf(stderr, LOG_ADDR_CONV, env_bind_address);
#endif
		return (*original_bind)(sockfd, addr, addrlen);
	}

	// Work out address size for the memcpy.
	size_t addrsize = 0;
	switch(addr->sa_family) {
		case AF_INET:
			addrsize = sizeof(struct in_addr);
			break;
		case AF_INET6:
			addrsize = sizeof(struct in6_addr);
			break;
		// We should never be able to get to this default since we
		// check for AF_INET || AF_INET6 above, but better safe than
		// sorry.
		default:
#if DEBUG
			fprintf(stderr, LOG_OH_NO);
#endif
			return (*original_bind)(sockfd, addr, addrlen);
	}

	// addrsize has somehow remained 0, call bind with original arguments.
	if (addrsize == 0) {
		return (*original_bind)(sockfd, addr, addrlen);
	}

#if DEBUG
	// Convert original address to string before overwriting it.
	char old_bind_address[INET6_ADDRSTRLEN];
	inet_ntop(addr->sa_family,
			&(bind_sockaddr->sin_addr),
			old_bind_address,
			sizeof(old_bind_address));
#endif

	// Everything until now has been planning, this memcpy is the magic.
	memcpy(&(bind_sockaddr->sin_addr), dst, addrsize);

#if DEBUG
	fprintf(stderr, LOG_OVERRIDE_SUCCESS, old_bind_address, env_bind_address);
#endif

	// Finally call bind.
	return (*original_bind)(sockfd, addr, addrlen);
}
