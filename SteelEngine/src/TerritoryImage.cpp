#include "TerritoryImage.h"

namespace Steel
{
	BitmapColour TerritoryImage::CastleColour = BitmapColour(127, 127, 127, 0);

	BitmapColour TerritoryImage::TownColour = BitmapColour(255, 127, 63, 0);

	std::vector<Territory*> TerritoryImage::ParseImage(std::string filename)
	{
		std::vector<Territory*> result;

		BitmapFile *bmp = new BitmapFile(filename);

		std::vector<BitmapColour> territoryColours;
		std::vector<std::vector<Vector2d>> territoryBounds;
		std::vector<Vector2d> castleLocations;
		std::vector<Vector2d> townLocations;
		BitmapColour *currentTerritory = nullptr;
		Vector2d *awaitingCastle = nullptr;
		Vector2d *awaitingTown = nullptr;

		//assume top right is 0,0
		for (int i = 0; i < bmp->GetWidth(); i++)
		{
			for (int j = 0; j < bmp->GetHeight(); j++)
			{
				auto pixel = bmp->GetPixelAt(i, j);
				if (pixel.Value == TerritoryImage::CastleColour.Value)
				{
					//if the first pixel read is a town or castle, it belongs to the first territory
					if(awaitingCastle == nullptr)
						awaitingCastle = new Vector2d(i, j);
					else
					{
						//add it to the current territory
						castleLocations.push_back(Vector2d(i, j));
					}
				}
				else if (pixel.Value == TerritoryImage::TownColour.Value)
				{
					if (awaitingTown == nullptr)
						awaitingTown = new Vector2d(i, j);
					else
					{
						townLocations.push_back(Vector2d(i, j));
					}
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
						if (territoryColours.size() == 1)
						{
							if (awaitingCastle != nullptr)
								castleLocations.push_back(Vector2d(awaitingCastle->X, awaitingCastle->Y));

							if (awaitingTown != nullptr)
								townLocations.push_back(Vector2d(awaitingTown->X, awaitingTown->Y));
						}
					}
				}
			}
		}

		delete awaitingCastle;
		delete awaitingTown;

		return result;
	}
}