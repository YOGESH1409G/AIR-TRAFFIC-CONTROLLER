#include "Controller.h"
#include <cmath>
#include <iostream>
#include <iomanip>

// ─── Constructor ────────────────────────────────────────────────────────────
// Sets the safe distance threshold. Default is 5 units.
Controller::Controller(double minSafeDistance)
    : minSafeDistance(minSafeDistance) {}

// ─── Collision Detection ────────────────────────────────────────────────────
// Checks ALL unique pairs of aircraft in the airspace.
// Uses Euclidean distance: d = sqrt((x2-x1)^2 + (y2-y1)^2)
// If d < minSafeDistance → prints a collision warning.
// Returns the total number of warnings raised.
int Controller::checkCollisions(const Airspace& airspace) const {
    const auto& list = airspace.getAircraftList();
    int warnings = 0;

    // Check every unique pair
    for (size_t i = 0; i < list.size(); ++i) {
        for (size_t j = i + 1; j < list.size(); ++j) {
            double dist = computeDistance(list[i], list[j]);
            if (dist < minSafeDistance) {
                raiseWarning(list[i], list[j], dist);
                ++warnings;
            }
        }
    }
    return warnings;
}

// ─── Getters / Setters ─────────────────────────────────────────────────────
double Controller::getMinSafeDistance() const       { return minSafeDistance; }
void   Controller::setMinSafeDistance(double dist)  { minSafeDistance = dist; }

// ─── Euclidean Distance ─────────────────────────────────────────────────────
// d = sqrt((x2 - x1)^2 + (y2 - y1)^2)
double Controller::computeDistance(const Aircraft& a, const Aircraft& b) const {
    double dx = static_cast<double>(a.getX() - b.getX());
    double dy = static_cast<double>(a.getY() - b.getY());
    return std::sqrt(dx * dx + dy * dy);
}

// ─── Warning Output ─────────────────────────────────────────────────────────
// ─── Warning Output ─────────────────────────────────────────────────────────
// Prints the collision warning in the required format.
void Controller::raiseWarning(const Aircraft& a, const Aircraft& b, double distance) const {
    std::cout << "  WARNING: Potential collision between Aircraft "
              << a.getId() << " and " << b.getId()
              << "  (distance: " << std::fixed << std::setprecision(2) << distance
              << ", safe: " << minSafeDistance << ")"
              << std::endl;
}
