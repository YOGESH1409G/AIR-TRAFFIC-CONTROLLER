#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Airspace.h"

// ─────────────────────────────────────────────────────────────────────────────
// Controller — Monitors aircraft positions and detects collisions.
//
// Responsibilities:
//   • Monitor aircraft positions in the airspace
//   • Detect possible collisions between aircraft pairs
//   • Print warnings when distance < safe_distance
//
// Collision Rule:
//   Uses Euclidean distance: d = sqrt((x2-x1)^2 + (y2-y1)^2)
//   If d < safe_distance → WARNING printed
//
// Design:
//   - Checks all unique pairs of aircraft: O(n^2/2)
//   - Stateless — operates on current airspace snapshot
//   - Configurable safe distance threshold
// ─────────────────────────────────────────────────────────────────────────────

class Controller {
private:
    double minSafeDistance;  // Minimum safe separation between aircraft

public:
    // ─── Constructor ────────────────────────────────────────────────────
    // Sets the minimum safe distance threshold (default: 5 units).
    Controller(double minSafeDistance = 5.0);

    // ─── Collision Detection ────────────────────────────────────────────
    // Checks ALL pairs of aircraft in the airspace.
    // If Euclidean distance < minSafeDistance, prints a warning.
    // Returns the number of warnings raised.
    int checkCollisions(const Airspace& airspace) const;

    // ─── Getter / Setter ────────────────────────────────────────────────
    double getMinSafeDistance() const;
    void setMinSafeDistance(double distance);

private:
    // Compute Euclidean distance between two aircraft.
    // Formula: d = sqrt((x2-x1)^2 + (y2-y1)^2)
    double computeDistance(const Aircraft& a, const Aircraft& b) const;

    // Print collision warning to console.
    void raiseWarning(const Aircraft& a, const Aircraft& b, double distance) const;
};

#endif // CONTROLLER_H
