#ifndef RADAR_H
#define RADAR_H

#include "Airspace.h"

// ─────────────────────────────────────────────────────────────────────────────
// Radar — Displays the current state of the airspace.
//
// Responsibilities:
//   • Display the current state of the airspace
//   • Print all aircraft IDs and their positions
//
// Design:
//   - Read-only observer: does not modify any aircraft or airspace data
//   - Simple and clean interface with a single primary method
// ─────────────────────────────────────────────────────────────────────────────

class Radar {
public:
    // Display the current state of the airspace.
    // Prints each aircraft's ID and position in the format:
    //   Aircraft <ID> → (<x>,<y>)
    void displayAirspace(const Airspace& airspace) const;

    // Render a simple ASCII grid showing aircraft positions.
    // Each aircraft is shown as the first character of its ID.
    void printGrid(const Airspace& airspace) const;
};

#endif // RADAR_H
