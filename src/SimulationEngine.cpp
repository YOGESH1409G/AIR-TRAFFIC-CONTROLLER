#include "SimulationEngine.h"
#include "AnsiColors.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <set>
#include <string>
#include <cstdlib>
#include <cmath>

// Constructor (from dimensions)
SimulationEngine::SimulationEngine(int width, int height, int maxSteps, double safeDistance)
    : airspace(width, height),
      controller(safeDistance),
      maxSteps(maxSteps),
      currentStep(0) {}

// Constructor (from pre-built objects)
SimulationEngine::SimulationEngine(Airspace airspace, Radar radar, Controller controller, int maxSteps)
    : airspace(airspace),
      radar(radar),
      controller(controller),
      maxSteps(maxSteps),
      currentStep(0) {}

// Add Aircraft
void SimulationEngine::addAircraft(const Aircraft& aircraft) {
    airspace.addAircraft(aircraft);
}

// Build danger set: IDs of all aircraft within minSafeDistance of another
static std::set<std::string> buildDangerSet(const Airspace& airspace, double safeDist) {
    std::set<std::string> ids;
    const auto& list = airspace.getAircraftList();
    for (size_t i = 0; i < list.size(); ++i) {
        for (size_t j = i + 1; j < list.size(); ++j) {
            double dx = list[i].getX() - list[j].getX();
            double dy = list[i].getY() - list[j].getY();
            double dist = std::sqrt(dx * dx + dy * dy);
            if (dist < safeDist) {
                ids.insert(list[i].getId());
                ids.insert(list[j].getId());
            }
        }
    }
    return ids;
}

// Run full simulation
void SimulationEngine::run() {
    printBanner();

    std::cout << "\n  " << clr::DIM << "Airspace: " << airspace.getWidth() << " x " << airspace.getHeight()
              << "  |  Aircraft: " << airspace.getAircraftCount()
              << "  |  Steps: " << maxSteps
              << "  |  Safe Distance: " << controller.getMinSafeDistance()
              << clr::RESET << "\n";

    // Initial state
    std::cout << "\n  " << clr::BOLD_WHITE << "--- INITIAL STATE ---" << clr::RESET << "\n";
    radar.displayAirspace(airspace);
    radar.printGrid(airspace);

    std::cout << "\n  " << clr::DIM << "Starting simulation in 2 seconds..." << clr::RESET << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Main simulation loop
    for (currentStep = 1; currentStep <= maxSteps; ++currentStep) {
        step();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    printSummary();
}

// Single step
void SimulationEngine::step() {
    // Clear screen for animation
    std::system("clear");

    std::cout << "\n";
    printBanner();

    // Progress bar
    int barWidth = 40;
    int filled = (currentStep * barWidth) / maxSteps;
    int pct = (currentStep * 100) / maxSteps;
    std::cout << "\n  " << clr::BOLD_CYAN << "Step " << currentStep << " / " << maxSteps
              << "  [";
    for (int i = 0; i < barWidth; ++i) {
        if (i < filled) std::cout << "#";
        else            std::cout << clr::DIM << "-" << clr::BOLD_CYAN;
    }
    std::cout << "]  " << pct << "%" << clr::RESET << "\n";

    // 1. Update positions
    airspace.updateAircraftPositions();

    // 2. Collision avoidance
    int avoidances = controller.resolveCollisions(airspace);
    if (avoidances == 0) {
        std::cout << "  " << clr::GREEN << "[OK] All aircraft safe - no avoidance needed." << clr::RESET << "\n";
    }

    // 3. Build danger set and display
    std::set<std::string> dangerIds = buildDangerSet(airspace, controller.getMinSafeDistance());
    radar.displayAirspace(airspace, dangerIds);
    radar.printGrid(airspace, dangerIds);
}

// Banner
void SimulationEngine::printBanner() const {
    std::cout << "\n";
    std::cout << "  " << clr::BOLD_CYAN << "+======================================================+" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET << "                                                      " << clr::BOLD_CYAN << "|" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET << "       " << clr::BOLD_WHITE << "AIR TRAFFIC CONTROL SIMULATOR  v2.0" << clr::RESET << "            " << clr::BOLD_CYAN << "|" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET << "       " << clr::DIM << "-----------------------------------" << clr::RESET << "            " << clr::BOLD_CYAN << "|" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET << "       " << clr::GREEN << "C++ OOP Simulation Engine" << clr::RESET << "                      " << clr::BOLD_CYAN << "|" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET << "                                                      " << clr::BOLD_CYAN << "|" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN << "+======================================================+" << clr::RESET << "\n";
}

// Summary dashboard
void SimulationEngine::printSummary() {
    std::system("clear");

    int avoided  = controller.getTotalAvoidances();
    int acCount  = airspace.getAircraftCount();
    int zones    = static_cast<int>(airspace.getWeatherZones().size());

    std::cout << "\n\n";
    std::cout << "  " << clr::BOLD_CYAN << "+=========================================================+" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET << "                                                         " << clr::BOLD_CYAN << "|" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET << "          " << clr::BOLD_WHITE << "S I M U L A T I O N   C O M P L E T E" << clr::RESET << "          " << clr::BOLD_CYAN << "|" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET << "                                                         " << clr::BOLD_CYAN << "|" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN << "+=========================================================+" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET << "                                                         " << clr::BOLD_CYAN << "|" << clr::RESET << "\n";

    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET
              << "   " << clr::BOLD_WHITE << "Total Steps Run:      " << clr::BOLD_CYAN << maxSteps << clr::RESET
              << std::string(36 - std::to_string(maxSteps).length(), ' ') << clr::BOLD_CYAN << "|" << clr::RESET << "\n";

    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET
              << "   " << clr::BOLD_WHITE << "Aircraft Tracked:     " << clr::BOLD_GREEN << acCount << clr::RESET
              << std::string(36 - std::to_string(acCount).length(), ' ') << clr::BOLD_CYAN << "|" << clr::RESET << "\n";

    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET
              << "   " << clr::BOLD_WHITE << "Weather Zones:        " << clr::BOLD_BLUE << zones << clr::RESET
              << std::string(36 - std::to_string(zones).length(), ' ') << clr::BOLD_CYAN << "|" << clr::RESET << "\n";

    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET
              << "   " << clr::BOLD_WHITE << "Collisions Avoided:   ";
    if (avoided > 0)
        std::cout << clr::BOLD_RED << avoided << clr::RESET;
    else
        std::cout << clr::BOLD_GREEN << avoided << clr::RESET;
    std::cout << std::string(36 - std::to_string(avoided).length(), ' ') << clr::BOLD_CYAN << "|" << clr::RESET << "\n";

    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET << "                                                         " << clr::BOLD_CYAN << "|" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN << "+=========================================================+" << clr::RESET << "\n";

    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET
              << "   " << clr::BOLD_GREEN  << "* Safe    " << clr::RESET
              << clr::BOLD_RED    << "* Danger    " << clr::RESET
              << clr::BOLD_YELLOW << "* Caution    " << clr::RESET
              << clr::BOLD_BLUE   << "# Zone" << clr::RESET
              << "        " << clr::BOLD_CYAN << "|" << clr::RESET << "\n";

    std::cout << "  " << clr::BOLD_CYAN << "|" << clr::RESET << "                                                         " << clr::BOLD_CYAN << "|" << clr::RESET << "\n";
    std::cout << "  " << clr::BOLD_CYAN << "+=========================================================+" << clr::RESET << "\n\n";
}
