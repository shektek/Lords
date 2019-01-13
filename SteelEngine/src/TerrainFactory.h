#ifndef TERRAIN_FACTORY_H
#define TERRAIN_FACTORY_H

#include "Terrain.h"

namespace Steel
{
	//really should be static
	class TerrainFactory
	{
		private:
			unsigned int	m_randCount;

		public:
			TerrainFactory();
			~TerrainFactory();

			//pre-created levels
			Terrain			*CreateNewTerrain(const std::string &filename);
			//random maps
			Terrain			*CreateNewTerrain(const std::string &name, const unsigned int width, const unsigned int height, unsigned char *data, const int scale);
	};
}

#endif