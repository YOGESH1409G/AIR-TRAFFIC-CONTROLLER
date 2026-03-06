#ifndef RADAR_H
#define RADAR_H

#include "Airspace.h"
#include <set>
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
// Radar — Color-coded display of the current airspace state.
//
// Responsibilities:
//   • Display aircraft positions with ANSI color coding
//   • Render ASCII grid with colored markers
//   • 🟢 Green  = safe aircraft
//   • 🔴 Red    = aircraft in collision danger
//   • 🟡 Yellow = aircraft near a weather zone
//   • 🔵 Bold blue = weather zone cells
//
// Design:
//   - Read-only observer: does not modify any aircraft or airspace data
//   - Accepts a "danger set" of aircraft IDs to determine color coding
// ─────────────────────────────────────────────────────────────────────────────

class Radar {
public:
    // Display aircraft list with color-coded status.
    // dangerIds = IDs of aircraft currently in collision danger.
    void displayAirspace(const Airspace& airspace,
                         const std::set<std::string>& dangerIds = {}) const;

    // Render color-coded ASCII grid.
    // dangerIds = IDs of aircraft currently in collision danger.
    void printGrid(const Airspace& airspace,
                   const std::set<std::string>& dangerIds = {}) const;
};

#endif // RADAR_H
