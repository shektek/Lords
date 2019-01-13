#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <cstdlib>
#include <cstring>
#include <string>

/* TODO: This class is not complete enough for use
 *	This is here because it'd be nice to have random map generation - something that Lords2
 *	didn't have. That would mean generating rules for terrain, walkability, resource placement,
 *	castle placement, etc. A lot of stuff that isn't really viable for a prototype to be doing.
 */

//Terrain is the physical component of a game level
//this is really just a factory... it will be necessary to save the hm to a bmp for savegames
namespace Steel
{
	class TerrainGenerator
	{
		private:


		public:
			unsigned char *Generate(const int width, const int height);
	};
}

#endif