#pragma once
#include "../DS/Queues/DEndedQueue.h"

#include "../Units/EarthGunnery.h"
#include "../Units/EarthTank.h"
#include "../Units/EarthSoldier.h"
#include "Army.h"

class EarthArmy : public Army
{

public:
	EarthArmy();
	void Attack();
};

