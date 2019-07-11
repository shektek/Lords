#ifndef TERRITORYIMAGE_H
#define TERRITORYIMAGE_H

#include <string>
#include <vector>
#include "Territory.h"
#include "BitmapLoader.h"
#include "TerritoryFactory.h"

namespace Steel
{
	static class TerritoryImage
	{
		private:
			static BitmapColour		CastleColour;
			static BitmapColour		TownColour;

		public:
			static std::vector<Territory*>	ParseImage(std::string filename, PeasantFactory *peasantFactory = nullptr, TerritoryFactory *territoryFactory = nullptr);
	};
}

#endif