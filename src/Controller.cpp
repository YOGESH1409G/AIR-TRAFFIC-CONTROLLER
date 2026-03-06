#include "Controller.h"
#include "AnsiColors.h"
#include <cmath>
#include <iostream>
#include <iomanip>

// Constructor
Controller::Controller(double minSafeDistance)
    : minSafeDistance(minSafeDistance), totalAvoidances(0) {}

// Collision detection (read-only)
int Controller::checkCollisions(const Airspace& airspace) const {
    const auto& list = airspace.getAircraftList();
    int warnings = 0;
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

// Collision avoidance (active)
int Controller::resolveCollisions(Airspace& airspace) {
    auto& list = airspace.getAircraftListMutable();
    int avoidances = 0;

    for (size_t i = 0; i < list.size(); ++i) {
        for (size_t j = i + 1; j < list.size(); ++j) {
            double dist = computeDistance(list[i], list[j]);
            if (dist < minSafeDistance) {
                raiseWarning(list[i], list[j], dist);

                std::string newDir = turnRight90(list[j].getDirection());
                list[j].changeDirection(newDir);
                ++avoidances;
                ++totalAvoidances;

                std::cout << "  " << clr::BOLD_YELLOW
                          << "[ATC] Rerouting " << list[j].getId()
                          << " -> new heading: " << newDir
                          << " (collision avoidance)"
                          << clr::RESET << std::endl;
            }
        }
    }
    return avoidances;
}

// Getters / Setters
double Controller::getMinSafeDistance() const       { return minSafeDistance; }
void   Controller::setMinSafeDistance(double dist)  { minSafeDistance = dist; }
int    Controller::getTotalAvoidances() const        { return totalAvoidances; }

// Euclidean distance
double Controller::computeDistance(const Aircraft& a, const Aircraft& b) const {
    double dx = static_cast<double>(a.getX() - b.getX());
    double dy = static_cast<double>(a.getY() - b.getY());
    return std::sqrt(dx * dx + dy * dy);
}

// Warning output (colored red)
void Controller::raiseWarning(const Aircraft& a, const Aircraft& b, double distance) const {
    std::cout << "  " << clr::BOLD_RED << "!! WARNING: " << clr::RESET
              << clr::RED << "Potential collision between Aircraft "
              << a.getId() << " and " << b.getId()
              << "  (distance: " << std::fixed << std::setprecision(2) << distance
              << ", safe: " << minSafeDistance << ")"
              << clr::RESET << std::endl;
}

// Turn right 90 degrees
std::string Controller::turnRight90(const std::string& dir) {
    if (dir == "N") return "E";
    if (dir == "E") return "S";
    if (dir == "S") return "W";
    if (dir == "W") return "N";
    return dir;
}
