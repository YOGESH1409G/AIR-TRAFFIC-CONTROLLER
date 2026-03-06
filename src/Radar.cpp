#include "Radar.h"
#include "AnsiColors.h"
#include <iostream>
#include <vector>

// Helper: is aircraft near any weather zone? (within 2 cells of boundary)
static bool isNearWeatherZone(const Aircraft& a, const Airspace& airspace) {
    const int margin = 2;
    int ax = a.getX(), ay = a.getY();
    for (const auto& z : airspace.getWeatherZones()) {
        if (ax >= z.x - margin && ax < z.x + z.width + margin &&
            ay >= z.y - margin && ay < z.y + z.height + margin) {
            return true;
        }
    }
    return false;
}

// displayAirspace: color-coded aircraft list
void Radar::displayAirspace(const Airspace& airspace,
                            const std::set<std::string>& dangerIds) const {
    const auto& list = airspace.getAircraftList();

    std::cout << "\n  " << clr::BOLD_CYAN
              << "+------------------------------------+" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN
              << "|   RADAR - Current Airspace State   |" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN
              << "+------------------------------------+" << clr::RESET << "\n";

    for (const auto& a : list) {
        const char* color = clr::BOLD_GREEN;
        std::string status = "SAFE";

        if (dangerIds.count(a.getId())) {
            color = clr::BOLD_RED;
            status = "DANGER";
        } else if (isNearWeatherZone(a, airspace)) {
            color = clr::BOLD_YELLOW;
            status = "CAUTION";
        }

        std::cout << "  " << color << "  * " << a.getId()
                  << " -> (" << a.getX() << "," << a.getY() << ")"
                  << "  Dir: " << a.getDirection()
                  << "  [" << status << "]" << clr::RESET << "\n";
    }

    std::cout << "\n  " << clr::DIM << "Total aircraft: " << list.size() << clr::RESET << "\n";

    const auto& zones = airspace.getWeatherZones();
    if (!zones.empty()) {
        std::cout << "  " << clr::BOLD_BLUE << "Weather Zones: ";
        for (size_t i = 0; i < zones.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << zones[i].name << " ("
                      << zones[i].x << "," << zones[i].y << " "
                      << zones[i].width << "x" << zones[i].height << ")";
        }
        std::cout << clr::RESET << "\n";
    }
}

// printGrid: color-coded ASCII radar grid
void Radar::printGrid(const Airspace& airspace,
                      const std::set<std::string>& dangerIds) const {
    int w = airspace.getWidth();
    int h = airspace.getHeight();
    const auto& list = airspace.getAircraftList();

    // colorMap: 0=empty, 1=weather zone, 2=safe, 3=danger, 4=caution
    std::vector<std::vector<char>> grid(h, std::vector<char>(w, '.'));
    std::vector<std::vector<int>>  colorMap(h, std::vector<int>(w, 0));

    // Weather zones as '#'
    const auto& zones = airspace.getWeatherZones();
    for (const auto& zone : zones) {
        for (int zy = zone.y; zy < zone.y + zone.height; ++zy) {
            for (int zx = zone.x; zx < zone.x + zone.width; ++zx) {
                if (zx >= 0 && zx < w && zy >= 0 && zy < h) {
                    int row = h - 1 - zy;
                    grid[row][zx] = '#';
                    colorMap[row][zx] = 1;
                }
            }
        }
    }

    // Aircraft markers
    for (const auto& a : list) {
        int gx = a.getX();
        int gy = a.getY();
        if (gx >= 0 && gx < w && gy >= 0 && gy < h) {
            char marker = a.getId().empty() ? '*' : a.getId()[0];
            int row = h - 1 - gy;
            grid[row][gx] = marker;

            if (dangerIds.count(a.getId()))          colorMap[row][gx] = 3;
            else if (isNearWeatherZone(a, airspace)) colorMap[row][gx] = 4;
            else                                     colorMap[row][gx] = 2;
        }
    }

    // Render grid
    std::cout << "\n  " << clr::DIM << "+" << std::string(w, '-') << "+" << clr::RESET << "\n";
    for (int row = 0; row < h; ++row) {
        std::cout << "  " << clr::DIM << "|" << clr::RESET;
        for (int col = 0; col < w; ++col) {
            switch (colorMap[row][col]) {
                case 1:  std::cout << clr::BOLD_BLUE   << grid[row][col] << clr::RESET; break;
                case 2:  std::cout << clr::BOLD_GREEN  << grid[row][col] << clr::RESET; break;
                case 3:  std::cout << clr::BOLD_RED    << grid[row][col] << clr::RESET; break;
                case 4:  std::cout << clr::BOLD_YELLOW << grid[row][col] << clr::RESET; break;
                default: std::cout << clr::DIM         << grid[row][col] << clr::RESET; break;
            }
        }
        std::cout << clr::DIM << "|" << clr::RESET << "\n";
    }
    std::cout << "  " << clr::DIM << "+" << std::string(w, '-') << "+" << clr::RESET << "\n";

    // Legend
    std::cout << "  " << clr::BOLD_GREEN  << "* Safe  " << clr::RESET
              << clr::BOLD_RED    << "* Danger  " << clr::RESET
              << clr::BOLD_YELLOW << "* Caution  " << clr::RESET
              << clr::BOLD_BLUE   << "# Weather Zone" << clr::RESET << "\n";
}
