#pragma once

struct RepeatState {
    unsigned long delay;
    unsigned long interval;
    unsigned long pressStart = 0;
    unsigned long lastRepeat = 0;
    unsigned long lastPress = 0;

    RepeatState(unsigned long d = 0, unsigned long i = 0)
        : delay(d), interval(i) {}

    bool enabled() const { return delay && interval; }

    void reset() {
        pressStart = 0;
        lastRepeat = 0;
    }

    void start(unsigned long now) {
        pressStart = now;
        lastRepeat = 0;
        lastPress = now;
    }

    bool startIfDebounced(unsigned long now, unsigned long debounceTime) {
        if (now - lastPress <= debounceTime) {
            return false;
        }
        start(now);
        return true;
    }

    unsigned long lastEvent() const { return lastPress; }

    bool shouldRepeat(unsigned long now) {
        if (!enabled()) return false;
        if (now - pressStart >= delay &&
            (lastRepeat == 0 || now - lastRepeat >= interval)) {
            lastRepeat = now;
            lastPress = now;
            return true;
        }
        return false;
    }
};