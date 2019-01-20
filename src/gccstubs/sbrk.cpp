#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <sys/types.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

extern "C" void *_sbrk (int incr);

extern char heapstart; /* Set by linker.  */
extern char heapend; /* Set by linker.  */



extern "C" void *_sbrk (int incr)
{
    static char*  heap_end;
    char*         prev_heap_end;

    if (heap_end == 0) {
        heap_end = & heapstart;
    }

    prev_heap_end = heap_end;

    if (heap_end + incr > &heapend) {
        errno = ENOMEM;
        return (caddr_t) -1;
    }

    heap_end += incr;

    return (void *) prev_heap_end;
}
