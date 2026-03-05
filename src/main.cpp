#include "Aircraft.h"
#include "Airspace.h"
#include "Radar.h"
#include "Controller.h"
#include "SimulationEngine.h"
#include <iostream>

int main() {
    // ─── Configuration ──────────────────────────────────────────────────
    const int AIRSPACE_WIDTH   = 40;
    const int AIRSPACE_HEIGHT  = 40;
    const int MAX_STEPS        = 20;
    const double SAFE_DISTANCE = 5.0;

    std::cout << "\n  [main] Integrating all ATC classes...\n";

    // ─── Step 1: Create Airspace ────────────────────────────────────────
    Airspace airspace(AIRSPACE_WIDTH, AIRSPACE_HEIGHT);
    std::cout << "  [main] Airspace created (" << AIRSPACE_WIDTH
              << " x " << AIRSPACE_HEIGHT << ")\n";

    // ─── Add Restricted Weather Zones ───────────────────────────────────
    // Storm zone: 5×5 block at (14,8) — sits on y=10 row, blocks A1/A2 path
    airspace.addWeatherZone({"Storm", 14, 8, 5, 5});
    // Fog zone: 4×4 block at (3,33) — sits in A3's northbound path
    airspace.addWeatherZone({"Fog",    3, 33, 4, 4});

    // ─── Step 2: Create Aircraft objects ────────────────────────────────
    Aircraft a1("A1", 10, 10, 1, "E");   // moving East
    Aircraft a2("A2", 20, 10, 1, "W");   // moving West  → heads toward A1!
    Aircraft a3("A3",  5, 30, 1, "N");   // moving North

    std::cout << "  [main] Aircraft created:\n";
    a1.printInfo();
    a2.printInfo();
    a3.printInfo();

    // ─── Step 3: Add Aircraft to Airspace ───────────────────────────────
    airspace.addAircraft(a1);
    airspace.addAircraft(a2);
    airspace.addAircraft(a3);

    // ─── Step 4: Create Radar and Controller ────────────────────────────
    Radar radar;
    Controller controller(SAFE_DISTANCE);
    std::cout << "  [main] Radar created\n";
    std::cout << "  [main] Controller created (safe distance: "
              << SAFE_DISTANCE << ")\n";

    // ─── Step 5: Start SimulationEngine ─────────────────────────────────
    SimulationEngine engine(airspace, radar, controller, MAX_STEPS);
    std::cout << "  [main] SimulationEngine initialised — starting simulation...\n";
    engine.run();

    return 0;
}
