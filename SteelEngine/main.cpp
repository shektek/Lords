#include <cstdio>

#include "TerritoryFactory.h"


int main(int argc, char **argv)
{
	//I don't really like the way this looks, but it will be hidden in the GameMaster class
	PeasantFactory *peasantFactory = new PeasantFactory("C:/dev/Lords/peasant.txt");
	TerritoryFactory *tFac = new TerritoryFactory("C:/dev/Lords/territory.txt");
	ResourceFactory *rFac = new ResourceFactory();
		
	std::vector<Territory*> testTerritory;
	testTerritory.push_back(tFac->CreateNewRandomTerritory());
	
	testTerritory[0]->m_peasantFactory = peasantFactory;

	bool run = true;
	int totalpeasants = 0;

	while (run)
	{
		printf("\nNext command: ");

		char c = 0;
		int index = 0;

		while (c != 'q' && c != 'n')
		{
			c = getchar();

			switch (c)
			{
			case 'q':
				run = false;
				break;

			case 'a':
				totalpeasants += testTerritory[index]->AddPeasants(5);
				break;

			case 'l':
				testTerritory[index]->RandomlyDistributeLabour();
				break;

			case 'r':
				testTerritory[index]->AddResource(rFac->CreateNewRandomResource());
				break;

			case 't':
				testTerritory.push_back(tFac->CreateNewRandomTerritory());
				testTerritory.back()->m_peasantFactory = peasantFactory;
				break;

			case ']':
				index++;

				if (index >= testTerritory.size())
					index = 0;

				printf("\n%s", testTerritory[index]->GetName().c_str());

				break;

			case '[':
				index--;

				if (index <= 0)
					index = testTerritory.size()-1;

				printf("\n%s", testTerritory[index]->GetName().c_str());

				break;

			default:
				break;
			}

			fflush(stdin);
		}

		//this will most likely be in the GameWorld class
		testTerritory[index]->PassSeason();
	}

	delete peasantFactory;
	delete tFac;
	delete rFac;

	return 0;
}