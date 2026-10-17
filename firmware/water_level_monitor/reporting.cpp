#include "reporting.h"

size_t formatReport(char *buf, size_t bufLen, uint32_t nowMs,
                     bool stablePresence, HealthStatus health) {
    (void)nowMs;
    (void)stablePresence;
    (void)health;
    // TODO(Chunk 5): JSON line builder, unit-tested on the host.
    if (bufLen == 0) return 0;
    buf[0] = '\0';
    return 0;
}
