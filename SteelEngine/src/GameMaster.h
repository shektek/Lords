#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include <cstdio>
//#include "Terrain.h"
#include "TerritoryFactory.h"
#include "TerrainFactory.h"
#include "TerrainGenerator.h"
#include "Level.h"
#include "Season.h"

/* TODO: This needs to have a few more functions to operate the game state
 */

namespace Steel
{
	class GameMaster
	{
		public:
			//ConfigFile		m_config; //doesn't exist yet
			PeasantFactory		*peasantFactory;
			TerritoryFactory	*territoryFactory;
			ResourceFactory		*resourceFactory;
			TerrainGenerator	*terrainGenerator;
			TerrainFactory		*terrainFactory;

			std::vector<Level*>		levels;	//levels contain territory and terrain!
			Level				*currentLevel;
			Season				m_currentSeason;

		public:
			GameMaster();
			~GameMaster();

			bool				SetCurrentLevel(const std::string &levelFilename);
			Level				*GetCurrentLevel() { return currentLevel; }
			Level				*GetLevelAtIndex(const int i) { if (i >= 0 && i < GetLevelCount()) return levels[i]; else return nullptr; }
			Level				*GetLevelByName(const std::string &name);
			int					GetLevelCount() const { return levels.size(); }

			bool				AddLevel(const std::string &levelFilename);
			bool				NextLevel();
			void				PassSeason(); //pass the season for all territories
			Season				GetCurrentSeason() const { return m_currentSeason; }
	};
}

#endif