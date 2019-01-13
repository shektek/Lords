#ifndef PEASANTFACTORY_H
#define PEASANTFACTORY_H

#include <cstdio>
#include <cmath>
#include <ctime>
#include <vector>
#include <string>
#include "Peasant.h"
#include "IdGenerator.h"

class PeasantFactory
{
	private:
		std::vector<std::string>	m_names;
		std::vector<std::string>	m_surnames;

	public:
		PeasantFactory(const std::string &nameFile);

		Peasant		*CreateNewRandomPeasant();	//no parameters, random peasant
};

#endif