#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Airspace.h"

// ─────────────────────────────────────────────────────────────────────────────
// Controller — Active ATC safety system: detects AND resolves collisions.
//
// Responsibilities:
//   • Monitor aircraft positions in the airspace
//   • Detect possible collisions between aircraft pairs
//   • Automatically reroute lower-priority aircraft (turn 90° right)
//   • Track cumulative collision avoidances for summary reporting
//
// Collision Rule:
//   Uses Euclidean distance: d = sqrt((x2-x1)^2 + (y2-y1)^2)
//   If d < safe_distance → reroute the lower-priority aircraft
//
// Avoidance Strategy:
//   - Lower priority = later in the aircraft list
//   - Rerouted aircraft turns 90° clockwise (N→E→S→W→N)
//   - Distinct from weather zone reversal (180°) for clear separation
//
// Design:
//   - Checks all unique pairs of aircraft: O(n^2/2)
//   - Configurable safe distance threshold
//   - Tracks total avoidances across all steps
// ─────────────────────────────────────────────────────────────────────────────

class Controller {
private:
    double minSafeDistance;   // Minimum safe separation between aircraft
    int totalAvoidances;      // Cumulative collision avoidances across all steps

public:
    // ─── Constructor ────────────────────────────────────────────────────
    // Sets the minimum safe distance threshold (default: 5 units).
    Controller(double minSafeDistance = 5.0);

    // ─── Collision Detection (read-only) ────────────────────────────────
    // Checks ALL pairs of aircraft in the airspace.
    // If Euclidean distance < minSafeDistance, prints a warning.
    // Returns the number of warnings raised.  Does NOT modify aircraft.
    int checkCollisions(const Airspace& airspace) const;

    // ─── Collision Avoidance (active) ───────────────────────────────────
    // Detects collisions AND automatically reroutes the lower-priority
    // aircraft by turning it 90° clockwise.  Prints:
    //   [ATC] Rerouting <ID> → new heading: <DIR> (collision avoidance)
    // Returns the number of avoidances performed this step.
    int resolveCollisions(Airspace& airspace);

    // ─── Getter / Setter ────────────────────────────────────────────────
    double getMinSafeDistance() const;
    void setMinSafeDistance(double distance);
    int getTotalAvoidances() const;

private:
    // Compute Euclidean distance between two aircraft.
    double computeDistance(const Aircraft& a, const Aircraft& b) const;

    // Print collision warning to console.
    void raiseWarning(const Aircraft& a, const Aircraft& b, double distance) const;

    // Turn 90° clockwise: N→E, E→S, S→W, W→N
    static std::string turnRight90(const std::string& dir);
};

#endif // CONTROLLER_H
