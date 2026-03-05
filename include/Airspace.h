#ifndef AIRSPACE_H
#define AIRSPACE_H

#include <vector>
#include "Aircraft.h"
#include "WeatherZone.h"

// ─────────────────────────────────────────────────────────────────────────────
// Airspace — Manages all aircraft within a bounded 2D simulation grid.
//
// Responsibilities:
//   • Store aircraft objects in a std::vector<Aircraft>
//   • Allow aircraft to be added at runtime
//   • Update every aircraft's position each simulation step
//   • Enforce grid boundaries (clamping)
//   • Provide read-only and mutable access to the aircraft list
//
// Designed to be easy to extend with features like:
//   • Maintain a list of restricted weather zones
//   • Prevent aircraft from entering weather zones (revert + reverse)
//
// Designed to be easy to extend with features like:
//   - landing queues
//   - dynamic aircraft removal
// ─────────────────────────────────────────────────────────────────────────────

class Airspace {
private:
    int width;                          // Grid width  (valid x: 0 to width-1)
    int height;                         // Grid height (valid y: 0 to height-1)
    std::vector<Aircraft> aircraftList;      // All active aircraft (STL vector)
    std::vector<WeatherZone> weatherZones;    // Restricted weather zones

    // Reverse a direction string: N↔S, E↔W.
    static std::string reverseDirection(const std::string& dir);

public:
    // ─── Constructor ────────────────────────────────────────────────────
    // Initialise the airspace with given grid dimensions.
    Airspace(int width, int height);

    // ─── Core Operations ────────────────────────────────────────────────

    // Add a new aircraft to the airspace.
    // Uses STL vector push_back for O(1) amortised insertion.
    void addAircraft(const Aircraft& aircraft);

    // Remove an aircraft by its unique ID.
    // Returns true if found and removed, false otherwise.
    bool removeAircraft(const std::string& id);

    // Update all aircraft positions for one simulation step.
    // Calls move() on each aircraft, then clamps to grid boundaries.
    void updateAircraftPositions();

    // Check if a given (x, y) coordinate is within the airspace bounds.
    bool isInBounds(int x, int y) const;

    // ─── Weather Zones ──────────────────────────────────────────────────

    // Add a restricted weather zone to the airspace.
    void addWeatherZone(const WeatherZone& zone);

    // Returns the list of weather zones (read-only).
    const std::vector<WeatherZone>& getWeatherZones() const;

    // Check if a point falls inside any weather zone.
    bool isInWeatherZone(int x, int y) const;

    // ─── Getters ────────────────────────────────────────────────────────

    // Returns the aircraft list (read-only reference).
    const std::vector<Aircraft>& getAircraftList() const;

    // Returns the aircraft list (mutable reference — for advanced use).
    std::vector<Aircraft>& getAircraftListMutable();

    // Grid dimensions.
    int getWidth() const;
    int getHeight() const;

    // Number of active aircraft currently tracked.
    int getAircraftCount() const;
};

#endif // AIRSPACE_H
