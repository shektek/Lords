#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include <cstdio>
//#include "Terrain.h"
#include "TerritoryFactory.h"
#include "TerrainFactory.h"
#include "TerrainGenerator.h"
#include "Level.h"

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

		public:
			GameMaster();
			~GameMaster();

			bool				SetCurrentLevel(const std::string &levelFilename);
			Level				*GetCurrentLevel() { return currentLevel; }

			bool				AddLevel(const std::string &levelFilename);
			bool				NextLevel();
			void				PassSeason(); //pass the season for all territories
	};
}

#endif