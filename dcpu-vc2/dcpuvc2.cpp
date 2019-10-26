#include "plugin.h"
#include "CStreaming.h"
#include "CModelInfo.h"
#include "CGame.h"
#include "CHeli.h"
#include "CTimer.h"
#include "CMessages.h"
#include "plugin.h"
#include "common.h"
#include "CPools.h"
#include "CWorld.h"
#include "VCPU.h"
#include "CTheScripts.h"
#include "extensions/ScriptCommands.h"

class dcpuvc2 {
public:
	VCPU* vcpu;
	FILE* conOut;

	unsigned int m_nLastCheckTime;

	dcpuvc2() {
		AllocConsole();
		
		// GetConsoleWindow();

		freopen_s(&conOut, "CONOUT$", "w", stdout);

		plugin::Events::gameProcessEvent += [this] {
			CPed* playa = FindPlayerPed();

			if (!playa) {
				return;
			}

			if (!playa->m_pVehicle) {
				return;
			}

			if (plugin::KeyPressed(VK_TAB) && !vcpu) {
				vcpu = new VCPU(playa->m_pVehicle, "example.dasm");
				vcpu->boot();
			}

			if (plugin::KeyPressed(VK_NEXT)) { // Page down
				if (vcpu->tableOffset < 0x10000 - 16) {
					vcpu->tableOffset += 16;
				}
			}

			if (plugin::KeyPressed(VK_PRIOR)) { // Page up
				if (vcpu->tableOffset > 0) {
					vcpu->tableOffset -= 16;
				}
			}
		};

		plugin::Events::gameProcessEvent += [this] {
			if (vcpu) {
				vcpu->reloadIfChanged();
				vcpu->step();
			}
		};

		plugin::Events::gameProcessEvent += [this] {
			if (CTimer::m_snTimeInMilliseconds > (m_nLastCheckTime + 1000)) {
				CPed* playa = FindPlayerPed();

				if (vcpu) {
					vcpu->updateVehicleData();
					vcpu->drawMemoryTable();
				}

				m_nLastCheckTime = CTimer::m_snTimeInMilliseconds;
			}
		};
	}
} _dcpuvc2;
