#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dlfcn.h>
#include <net/if.h>
#include <string.h>
#include <errno.h>

// Source: https://unix.stackexchange.com/a/648791

int socket(int family, int type, int protocol)
{
    // socket is called twice. First is for DNS-Lookup:
    // If your first nameserver is not reachable via bound interface,
    // then it will try the next nameserver until it succeeds or stops with name resolution error.
    // This is why it could take significantly longer than curl --interface wlan0 ifconfig.me

    int *(*original_socket)(int, int, int);
    
    char *error;    
    dlerror(); // Reset previous errors
    *(void **)(&original_socket) = dlsym(RTLD_NEXT, "socket");
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "Error: Cannot load library 'socket': %s\n", error);
        return -1;
    }

    int fd = (intptr_t)(*original_socket)(family, type, protocol);

    char *evn_b2d_string = secure_getenv("BIND2DEVICE");
    size_t evn_b2d_len   = (evn_b2d_string == NULL) ? 0 : strnlen(evn_b2d_string, IFNAMSIZ - 1);

    if (evn_b2d_len <= 0 || evn_b2d_len >= (IFNAMSIZ - 1))
    {
        fprintf(stderr, "Warning: Program with LD_PRELOAD startet, but BIND_INTERFACE environment variable not set\n");
        return fd;
    }

    int errorCode = setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, evn_b2d_string, evn_b2d_len + 1);
    if (errorCode < 0)
    {
        perror("setsockopt failed for SO_BINDTODEVICE");
        errno = EINVAL;
        return -1;
    };

    return fd;
}