#include <MIDI.h>

void setup() {

}

int last = 0;
int current = 0;

void loop() {
    for (int i = 0; i < 4; i++) {
        if (digitalRead(i) == HIGH) {
            current |= 1 << i;
        }
    }

    if (current != last) { // && foukam
        // send midi note
    }

    // send foukani
}
