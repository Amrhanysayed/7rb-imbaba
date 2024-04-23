#include "Game.h"

Game::Game(char _mode)
{
	earthArmy = new EarthArmy();
	alienArmy = new AlienArmy();
	randGenerator = new randGen(this);
	mode = _mode;
	timeStep = 0;
}

void Game::NextTimeStep()
{
	timeStep++;
	CheckingUML();
	randGenerator->execute();
	earthArmy->Print(); 
	alienArmy->Print();

}

void Game::NextTimeStepTest()
{ 
	// for phase 1.2 only

	timeStep++;
	
	randGenerator->execute();

	int x = rand() % 100 + 1;
	Unit* unit = nullptr;
	cout << endl << "X = " << x << endl;
	if (x < 10) {
		unit = earthArmy->RemoveUnit(Unit::ES);
		if(unit)
			earthArmy->AddUnit(unit);
	}
	else if (x < 20) {
		unit = earthArmy->RemoveUnit(Unit::ET);
		if (unit)
			AddToKilledList(unit);
	}
	else if (x < 30) {
		unit = earthArmy->RemoveUnit(Unit::EG);
		if (unit) {
			unit->TakeDamage(unit->GetHealth() / 2);
			earthArmy->AddUnit(unit);
		}
	}
	else if (x < 40) {
		LinkedQueue<Unit*> Temp;
		for (int i = 0; i < 5; ++i) {
			unit = alienArmy->RemoveUnit(Unit::AS);
			if (unit) {
				unit->TakeDamage(unit->GetHealth() / 2);
				Temp.enqueue(unit);
			}
		}
		while (!Temp.isEmpty()) {
			Temp.dequeue(unit);
			alienArmy->AddUnit(unit);
		}

	}
	else if (x < 50) {
		for (int i = 0; i < 5; ++i) {
			unit = alienArmy->RemoveUnit(Unit::AM);
			if (unit) {
				alienArmy->AddUnit(unit);
			}
		}
	}
	else if (x < 60) {
		for (int i = 0; i < 3; ++i) {
			unit = alienArmy->RemoveUnit(Unit::AD);
			if (unit) {
				AddToKilledList(unit);
			}

			unit = alienArmy->RemoveUnit(Unit::AD, true);
			if (unit) {
				AddToKilledList(unit);
			}
		}
	}
	earthArmy->Print(); 
	alienArmy->Print();
	PrintKilledList();
}

int Game::GetCurrentTimeStep() const
{
	return timeStep;
}


int Game::getNextUnitId(char army)
{
	if (army == 'a')
		return alienArmy->getNextUnitId();
	return earthArmy->getNextUnitId();
}

int Game::getTj()
{
	return timeStep;
}

Game* Game::getgameptr()
{
	
	return this;
}

void Game::addUnit(Unit* unit)
{
	if (!unit) return;
	if(unit->getType() == Unit::EG || unit->getType() == Unit::ES || unit->getType() == Unit::ET|| unit->getType() == Unit::HU)
		earthArmy->AddUnit(unit);
	alienArmy->AddUnit(unit);
}



void Game::AddToKilledList(Unit* unit)
{
	Killed.enqueue(unit);
}

Unit* Game::GetFromUML()
{
	Unit* p;
	int x;
	UML.dequeue(p, x);
	return p;
}

void Game::AddToUML(Unit* unit)
{
	//// use for tanks and earth solider only //////////
	if (!unit) return;
	float pri = 0;
	if (unit->getType()==Unit::ES) {
		pri = 100 - unit->GetHealth();
		unit->SetHealtime(this->GetCurrentTimeStep());
		UML.enqueue(unit, pri);
	}
	if (unit->getType() == Unit::ET) {
		unit->SetHealtime(this->GetCurrentTimeStep());
		UML.enqueue(unit, pri);
	}
}

void Game::CheckingUML()
{
	//// checking for the 10 time step ////////////////
	/// used when you next time step ///////////////
	priQueue<Unit*>tempcheck;
	while (!UML.isEmpty())
	{
		int p;
		Unit* ptr;
		UML.dequeue(ptr, p);
		if (abs(ptr->GetHealtime() - this->GetCurrentTimeStep()) >= 10) {
			AddToKilledList(ptr);
		}
		else {
			tempcheck.enqueue(ptr, p);
		}
	}

	while (!tempcheck.isEmpty())
	{
		int p;
		Unit* ptr;
		tempcheck.dequeue(ptr, p);
		UML.enqueue(ptr, p);
	}
}


void Game::PrintKilledList() {
	cout << "============================== Killed/Destructed Units ==============================" << endl;
	cout << Killed.GetCount() << " units [";
	Killed.Print();
	cout << "] " << endl << endl;
}


void Game::loadFile(int& N, int& Prob, EarthArmyConfig* eParams, AlienArmyConfig* aParams)
{
	cout << "Please, Enter the Name of the file: ";
	cin >> file;
	file += ".txt";
	cin.ignore();
	ifstream inFile(file);
	while (!inFile.is_open()) {
		cout << "Couldn't Find it\nPlease Enter Your File name Correctly and Without .txt: ";
		cin >> file;
		file += ".txt";
		cin.ignore();
		inFile.open(file);
	}
		inFile >> N
			>> eParams->ES >> eParams->EG >> eParams->ET >> eParams->HU
			>> aParams->AS >> aParams->AD >> aParams->AM
			>> Prob
			>> eParams->ePowCeil >> eParams->ePowFloor 
			>> eParams->eHealCeil >> eParams->eHealFloor 
			>> eParams->eCapCeil >> eParams->eCapFloor
			>> aParams->aPowCeil >> aParams->aPowFloor 
			>> aParams->aHealCeil >> aParams->aHealFloor 
			>> aParams->aCapCeil >> aParams->aCapFloor;
		eParams->ePowFloor *= -1;
		eParams->eHealFloor *= -1;
		eParams->eCapFloor *= -1;
		aParams->aPowFloor *= -1;
		aParams->aHealFloor *= -1;
		aParams->aCapFloor *= -1;
	inFile.close();
}

Game::~Game()
{
	delete earthArmy;
	delete alienArmy;
	delete randGenerator;
}


