#include "Radar.h"
#include <iostream>
#include <vector>

// ─── displayAirspace ────────────────────────────────────────────────────────
// Displays the current state of the airspace.
// Prints all aircraft IDs and their positions in a clean format.
void Radar::displayAirspace(const Airspace& airspace) const {
    const auto& list = airspace.getAircraftList();

    std::cout << "\n  ┌────────────────────────────────────┐\n";
    std::cout << "  │   RADAR — Current Airspace State   │\n";
    std::cout << "  └────────────────────────────────────┘\n";

    for (const auto& a : list) {
        std::cout << "  Aircraft " << a.getId()
                  << " \u2192 (" << a.getX() << "," << a.getY() << ")"
                  << std::endl;
    }

    std::cout << "\n  Total aircraft: " << list.size() << "\n";

    // Show weather zones if any exist
    const auto& zones = airspace.getWeatherZones();
    if (!zones.empty()) {
        std::cout << "  Weather Zones: ";
        for (size_t i = 0; i < zones.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << zones[i].name << " ("
                      << zones[i].x << "," << zones[i].y << " "
                      << zones[i].width << "x" << zones[i].height << ")";
        }
        std::cout << "\n";
    }
}

// ─── Print ASCII Grid ───────────────────────────────────────────────────────
void Radar::printGrid(const Airspace& airspace) const {
    int w = airspace.getWidth();
    int h = airspace.getHeight();
    const auto& list = airspace.getAircraftList();

    // Create empty grid
    std::vector<std::vector<char>> grid(h, std::vector<char>(w, '.'));

    // Render weather zones as '#'
    const auto& zones = airspace.getWeatherZones();
    for (const auto& zone : zones) {
        for (int zy = zone.y; zy < zone.y + zone.height; ++zy) {
            for (int zx = zone.x; zx < zone.x + zone.width; ++zx) {
                if (zx >= 0 && zx < w && zy >= 0 && zy < h) {
                    grid[h - 1 - zy][zx] = '#';
                }
            }
        }
    }

    // Place aircraft on grid (use first char of ID — overwrites '#')
    for (const auto& a : list) {
        int gx = a.getX();
        int gy = a.getY();

        // Clamp to grid bounds
        if (gx >= 0 && gx < w && gy >= 0 && gy < h) {
            char marker = a.getId().empty() ? '*' : a.getId()[0];
            // Flip Y so row 0 = top of screen = max Y
            grid[h - 1 - gy][gx] = marker;
        }
    }

    // Print grid with border
    std::cout << "\n  +" << std::string(w, '-') << "+\n";
    for (int row = 0; row < h; ++row) {
        std::cout << "  |";
        for (int col = 0; col < w; ++col) {
            std::cout << grid[row][col];
        }
        std::cout << "|\n";
    }
    std::cout << "  +" << std::string(w, '-') << "+\n";
}
