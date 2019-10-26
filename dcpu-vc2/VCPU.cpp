#include "VCPU.h"
#include "CClock.h"
#include <thread>

VCPU::VCPU(CVehicle* _vehicle, const std::string filename) : CPU(filename) {
	vehicle = _vehicle;
}

VCPU::VCPU(const std::string filename) : CPU(filename) {

}

void VCPU::sync() {

}

void VCPU::haltMachine() {
	halt = true;
}

bool VCPU::running() {
	return !halt;
}

void VCPU::updateVehicleData() {
	auto o = 0x6000;
	addr(o + 0) = (word)vehicle->m_fHealth;
	addr(o + 2) = (word)(vehicle->m_vecMoveSpeed.Magnitude() * 100.0f);
	addr(o + 4) = (word)vehicle->m_nNumPassengers;
	addr(o + 6) = (word)vehicle->m_nLockStatus;
	addr(o + 8) = (word)vehicle->m_fGasPedal;
	addr(o + 10) = (word)vehicle->m_fBreakPedal;
	addr(o + 12) = vehicle->IsUpsideDown() ? 0x0001 : 0x0000;
	addr(o + 14) = vehicle->m_wBombTimer;
	addr(o + 15) = vehicle->m_nPrimaryColor;
	addr(o + 16) = vehicle->m_nSecondaryColor;
	
	// Все двери можно уместить в один байт. То же самое с фонарями
	// addr(0xXXXX) = vehicle->IsDoorClosed(eDoors::DOOR_FRONT_LEFT) ? 0x0001 : 0x0000;
}

void VCPU::drawMemoryTable() {
	system("cls");
	for (word y = 0; y < 16 * 16; y += 16) {
		auto off = tableOffset + y;

		printf("%04X:", off);
		for (word x = 0; x < 16; x += 2) {
			auto a = off + x;
			auto v = addr(0x0000);

			if (a == (*ctx)[PC]) {
				printf("[%04X]", v);
			} else {
				printf(" %04X ", v);
			}
		}
		printf("\n");
	}
}

Memory* VCPU::mem() {
	return memory;
}

typedef struct MyData {
	int val1;
	int val2;
} MYDATA, * PMYDATA;
