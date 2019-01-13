#include "Level.h"

namespace Steel
{
	Level::Level()
	{
		m_levelName.clear();
		m_levelFilename.clear();
		terrain = nullptr;
		territories.clear();
	}

	Level::Level(std::string levelName, Terrain *ter, std::vector<Territory*> terrs, std::string levelFilename)
	{
		m_levelName = levelName;
		m_levelFilename = levelFilename;
		terrain = ter;	//shallow copying on purpose here
		for (int i = 0; i < terrs.size(); i++)
		{
			territories.push_back(terrs[i]);	//shallow copying the territory list, as it is the Levels responsibility to delete
		}
	}

	Level::Level(const Level &other)
	{
		m_levelName = other.m_levelName;
		m_levelFilename = other.m_levelFilename;
		terrain = other.terrain;
		territories = other.territories;
	}

	Level::Level(Level *other)
	{
		m_levelName = other->m_levelName;
		m_levelFilename = other->m_levelFilename;
		terrain = other->terrain;
		territories = other->territories;
	}

	Level::~Level()
	{
		m_levelName.clear();
		m_levelFilename.clear();
		//it is the levels responsibility to delete the terrain and individual territories
		delete terrain;
		for (int i = 0; i < territories.size(); i++)
		{
			delete territories[i];
		}

		territories.clear();
	}
}