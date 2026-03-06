#include "SimulationEngine.h"
#include <iostream>
#include <thread>
#include <chrono>

// ─── Constructor (from dimensions) ──────────────────────────────────────────
SimulationEngine::SimulationEngine(int width, int height, int maxSteps, double safeDistance)
    : airspace(width, height),
      controller(safeDistance),
      maxSteps(maxSteps),
      currentStep(0) {}

// ─── Constructor (from pre-built objects) ───────────────────────────────────
// Accepts an Airspace that already has aircraft added, plus Radar & Controller.
SimulationEngine::SimulationEngine(Airspace airspace, Radar radar, Controller controller, int maxSteps)
    : airspace(airspace),
      radar(radar),
      controller(controller),
      maxSteps(maxSteps),
      currentStep(0) {}

// ─── Add Aircraft ───────────────────────────────────────────────────────────
void SimulationEngine::addAircraft(const Aircraft& aircraft) {
    airspace.addAircraft(aircraft);
}

// ─── Run Full Simulation ────────────────────────────────────────────────────
// Main entry point.  Runs the loop for maxSteps iterations.
// Each step executes:  update positions → detect collisions → display radar.
// A 500 ms delay is inserted between steps for console readability.
void SimulationEngine::run() {
    printBanner();

    std::cout << "\n  Airspace: " << airspace.getWidth() << " x " << airspace.getHeight()
              << "  |  Aircraft: " << airspace.getAircraftCount()
              << "  |  Steps: " << maxSteps
              << "  |  Safe Distance: " << controller.getMinSafeDistance()
              << "\n";

    // Show initial state before the loop begins
    std::cout << "\n  ─── INITIAL STATE ───────────────────────────────────\n";
    radar.displayAirspace(airspace);
    radar.printGrid(airspace);

    // ─── Main simulation loop (20 steps by default) ─────────────────────
    // For each step:
    //   1. Airspace::updateAircraftPositions()  — move all aircraft
    //   2. Controller::checkCollisions()        — detect proximity violations
    //   3. Radar::displayAirspace()             — print positions & grid
    for (currentStep = 1; currentStep <= maxSteps; ++currentStep) {
        step();

        // Delay between steps for readability (500 ms)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    printSummary();
}

// ─── Single Step ────────────────────────────────────────────────────────────
// Executes one simulation time-step in three phases:
//   1. updateAircraftPositions() — move every aircraft by its speed/direction
//   2. resolveCollisions()       — detect AND auto-reroute if too close
//   3. displayAirspace()         — print status table + ASCII grid
void SimulationEngine::step() {
    std::cout << "\n  ═══ STEP " << currentStep << " / " << maxSteps
              << " ═══════════════════════════════════════════\n";

    // 1. Update all aircraft positions
    airspace.updateAircraftPositions();

    // 2. Detect collisions AND auto-reroute if needed
    int avoidances = controller.resolveCollisions(airspace);
    if (avoidances == 0) {
        std::cout << "  ✓  All aircraft safe — no avoidance needed.\n";
    }

    // 3. Display radar
    radar.displayAirspace(airspace);
    radar.printGrid(airspace);
}

// ─── Banner ─────────────────────────────────────────────────────────────────
void SimulationEngine::printBanner() const {
    std::cout << "\n";
    std::cout << "  ╔══════════════════════════════════════════════════════╗\n";
    std::cout << "  ║                                                      ║\n";
    std::cout << "  ║       AIR TRAFFIC CONTROL SIMULATOR  v1.0            ║\n";
    std::cout << "  ║       ─────────────────────────────────              ║\n";
    std::cout << "  ║       C++ OOP Simulation Engine                      ║\n";
    std::cout << "  ║                                                      ║\n";
    std::cout << "  ╚══════════════════════════════════════════════════════╝\n";
}

// ─── Summary ────────────────────────────────────────────────────────────────
void SimulationEngine::printSummary() {
    std::cout << "\n  ╔══════════════════════════════════════════════════════╗\n";
    std::cout << "  ║              SIMULATION COMPLETE                     ║\n";
    std::cout << "  ╠══════════════════════════════════════════════════════╣\n";
    int avoided = controller.getTotalAvoidances();
    std::cout << "  ║  Total Steps Run:    " << maxSteps << std::string(32 - std::to_string(maxSteps).length(), ' ') << "║\n";
    std::cout << "  ║  Aircraft Tracked:   " << airspace.getAircraftCount() << std::string(32 - std::to_string(airspace.getAircraftCount()).length(), ' ') << "║\n";
    std::cout << "  ║  Collisions Avoided: " << avoided << std::string(32 - std::to_string(avoided).length(), ' ') << "║\n";
    std::cout << "  ╚══════════════════════════════════════════════════════╝\n\n";
}
