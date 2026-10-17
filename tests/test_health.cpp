#include "health.h"
#include <cassert>

int main() {
    HealthMonitor monitor;
    // Chunk 1 stub always reports Ok. REWRITE in Chunk 3 once real checks land.
    assert(monitor.check(true, 0) == HealthStatus::Ok);
    return 0;
}
