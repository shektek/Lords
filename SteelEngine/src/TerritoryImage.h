#ifndef TERRITORYIMAGE_H
#define TERRITORYIMAGE_H

#include <string>
#include <vector>
#include "Territory.h"
#include "BitmapLoader.h"

namespace Steel
{
	static class TerritoryImage
	{
		private:
			static BitmapColour		CastleColour;
			static BitmapColour		TownColour;

		public:
			static std::vector<Territory*>	ParseImage(std::string filename);
	};
}

#endif