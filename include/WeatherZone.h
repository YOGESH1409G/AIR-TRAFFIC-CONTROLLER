#ifndef WEATHERZONE_H
#define WEATHERZONE_H

#include <string>

// ─────────────────────────────────────────────────────────────────────────────
// WeatherZone — A rectangular restricted area within the airspace.
//
// Aircraft must avoid these zones.  When an aircraft tries to enter a
// weather zone, the Airspace will revert its move and reverse its direction.
//
// Defined by:
//   (x, y)          — bottom-left corner of the rectangle
//   (width, height) — size of the rectangle
//   name            — human-readable label (e.g. "Storm", "Fog")
//
// A point (px, py) is inside the zone if:
//   x <= px < x + width   AND   y <= py < y + height
// ─────────────────────────────────────────────────────────────────────────────

struct WeatherZone {
    std::string name;   // Human-readable label
    int x;              // Bottom-left corner X
    int y;              // Bottom-left corner Y
    int width;          // Zone width  (extends rightward)
    int height;         // Zone height (extends upward)

    // Check if a point falls inside this zone.
    bool contains(int px, int py) const {
        return px >= x && px < x + width
            && py >= y && py < y + height;
    }
};

#endif // WEATHERZONE_H
