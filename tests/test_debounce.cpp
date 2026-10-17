#include "debounce.h"
#include <cassert>

int main() {
    Debouncer debouncer;
    // Chunk 1 stub passes the raw level through unchanged. This assertion is
    // expected to be REWRITTEN in Chunk 2 once real stabilization lands.
    assert(debouncer.update(true, 0) == true);
    assert(debouncer.update(false, 10) == false);
    return 0;
}
