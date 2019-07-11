#include "TerritoryImage.h"

namespace Steel
{
	BitmapColour TerritoryImage::CastleColour = BitmapColour(127, 127, 127, 0);

	BitmapColour TerritoryImage::TownColour = BitmapColour(255, 127, 63, 0);

	//for now assume that each territory will have a town and castle location
	std::vector<Territory*> TerritoryImage::ParseImage(std::string filename, PeasantFactory *peasantFactory, TerritoryFactory *territoryFactory)
	{
		std::vector<Territory*> result;

		BitmapFile* bmp = new BitmapFile(filename);

		std::vector<BitmapColour> territoryColours;
		std::vector<Vector2d> territoryBounds;
		std::vector<Vector2d> castleLocations;
		std::vector<Vector2d> townLocations;
		BitmapColour* currentTerritory = nullptr;

		int i, j;
		BitmapColour lastPixel(0, 0, 0, 0);
		//bottom left is 0,0
		for (i = 0; i < bmp->GetWidth(); i++)
		{
			for (j = 0; j < bmp->GetHeight(); j++)
			{
				auto pixel = bmp->GetPixelAt(i, j);

				if (pixel.RGBA.R != lastPixel.RGBA.R
					|| pixel.RGBA.G != lastPixel.RGBA.G
					|| pixel.RGBA.B != lastPixel.RGBA.B)
				{
					//change in colour marks a change in territory, hence a territorial boundary should be placed
					territoryBounds.push_back(Vector2d(i, j));
				}

				lastPixel = pixel;

				if (pixel.Value == TerritoryImage::CastleColour.Value)
				{
					castleLocations.push_back(Vector2d(i, j));
				}
				else if (pixel.Value == TerritoryImage::TownColour.Value)
				{
					townLocations.push_back(Vector2d(i, j));
				}
				else
				{
					bool found = false;
					for (auto colour : territoryColours)
					{
						if (pixel.Value == colour.Value)
						{
							currentTerritory = &pixel;
							found = true;
						}
					}

					if (!found)
					{
						territoryColours.push_back(pixel);
					}
				}
			}
		}

		//now we should have collections of territory boundaries, castle locations, and town locations
		//the size of each should be the same
		if (territoryColours.size() == castleLocations.size() && castleLocations.size() == townLocations.size())
		{
			for (int i = 0; i < territoryColours.size(); i++)
			{
				char d[2] = { '0' + i };
				Pos3 castlePos(castleLocations[i].X, castleLocations[i].Y, 0.0);
				Pos3 townPos(townLocations[i].X, townLocations[i].Y, 0.0);
				std::string name = territoryFactory == nullptr ? ("territory " + std::string(d)) : territoryFactory->CreateRandomTerritoryName();
				Territory* t = new Territory(name, castlePos, townPos, peasantFactory);
				result.push_back(t);
			}
		}

		return result;
	}
}