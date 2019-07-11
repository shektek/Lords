#include "TerritoryFactory.h"

TerritoryFactory::TerritoryFactory(const std::string &nameFile, PeasantFactory *peasantFactory)
{
	m_peasantFactory = peasantFactory;

	srand((unsigned int)time(0));

	FILE *f = nullptr;
	fopen_s(&f, nameFile.c_str(), "r");

	if (f)
	{
		char line[256] = { 0 };
		while (fgets(line, 255, f) != nullptr)
		{
			char *lp = line;
			lp += 2;

			char *end = lp;
			while (end != NULL && *end != '\n')
				end++;

			if (line[0] == 'N')
			{
				m_names.push_back(std::string(lp, end));
			}
			else if (line[0] == 'S')
			{
				m_surnames.push_back(std::string(lp, end));
			}
		}

		fclose(f);
	}
}

TerritoryFactory::~TerritoryFactory()
{
	m_names.clear();
	m_surnames.clear();
}

Territory *TerritoryFactory::CreateNewRandomTerritory()
{
	return new Territory(CreateRandomTerritoryName(), /*TODO*/ Pos3(0,0,0), /*TODO*/ Pos3(0,0,0), m_peasantFactory);
}

std::string TerritoryFactory::CreateRandomTerritoryName()
{
	int rnn = rand() % m_names.size();
	int rsn = rand() % m_surnames.size();

	std::string name = m_names[rnn] + " " + m_surnames[rsn];

	return name;
}