#ifndef TERRITORYFACTORY_H
#define TERRITORYFACTORY_H

#include <cstdio>
#include <cmath>
#include <ctime>
#include <vector>
#include <string>
#include "IdGenerator.h"
#include "Territory.h"
#include "PeasantFactory.h"

class TerritoryFactory
{
	private:
		std::vector<std::string>	m_names;
		std::vector<std::string>	m_surnames;
		PeasantFactory				*m_peasantFactory;

	public:
		TerritoryFactory(const std::string &nameFile, PeasantFactory *peasantFactory);
		~TerritoryFactory();

		Territory	*CreateNewRandomTerritory();	//no parameters, random territory
};

#endif