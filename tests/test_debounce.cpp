#include "debounce.h"
#include "config.h"
#include <cassert>

int main() {
    // Starts stable at false (matches Debouncer's default).
    {
        Debouncer d;
        assert(d.update(false, 0) == false);
    }

    // A sustained new level commits only after DEBOUNCE_MS has elapsed.
    {
        Debouncer d;
        assert(d.update(true, 0) == false);                       // just appeared
        assert(d.update(true, DEBOUNCE_MS - 1) == false);          // not held long enough yet
        assert(d.update(true, DEBOUNCE_MS) == true);               // held for DEBOUNCE_MS -> commits
        assert(d.update(true, DEBOUNCE_MS + 1000) == true);        // stays committed
    }

    // Chatter (flips faster than DEBOUNCE_MS) never commits.
    {
        Debouncer d;
        uint32_t t = 0;
        bool raw = false;
        for (int i = 0; i < 20; ++i) {
            raw = !raw;
            d.update(raw, t);
            t += DEBOUNCE_MS / 2; // always flips before the timer can elapse
        }
        assert(d.update(false, t) == false); // stable never left its initial value
    }

    // After committing true, a sustained return to false also commits.
    {
        Debouncer d;
        d.update(true, 0);
        d.update(true, DEBOUNCE_MS);
        assert(d.update(false, DEBOUNCE_MS + 1) == true);               // just appeared
        assert(d.update(false, DEBOUNCE_MS + 1 + DEBOUNCE_MS) == false); // held long enough
    }

    return 0;
}
