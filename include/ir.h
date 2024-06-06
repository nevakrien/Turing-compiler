#include <stdint.h>

uint32_t hash_fnv1a(const char *str) {
    uint32_t hash = 2166136261U;
    int c;

    while ((c = *str++)) {
        hash ^= c;
        hash *= 16777619;
    }

    return hash;
}
