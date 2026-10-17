#include "reporting.h"
#include <cassert>
#include <cstring>

int main() {
    char buf[256];
    // Chunk 1 stub writes nothing. REWRITE in Chunk 5 once the JSON builder lands.
    size_t written = formatReport(buf, sizeof(buf), 0, true, HealthStatus::Ok);
    assert(written == 0);
    assert(strlen(buf) == 0);
    return 0;
}
