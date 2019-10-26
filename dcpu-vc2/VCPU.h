#include "cpu.h"
#include "CVehicle.h"

#pragma once
class VCPU : public CPU
{
public:
	CVehicle* vehicle;

	word tableOffset = 0x0000;

	VCPU(CVehicle* vehicle, const std::string filename);
	VCPU(const std::string filename);

	void haltMachine();
	void sync();
	bool running();
	Memory* mem();
	void updateVehicleData();
	void drawMemoryTable();
};

