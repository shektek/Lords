#include "TerritoryImage.h"

namespace Steel
{
	BitmapColour TerritoryImage::CastleColour = BitmapColour(127, 127, 127, 0);

	BitmapColour TerritoryImage::TownColour = BitmapColour(255, 127, 63, 0);

	//for now assume that each territory will have a town and castle location
	std::vector<Territory*> TerritoryImage::ParseImage(std::string filename, PeasantFactory* peasantFactory, TerritoryFactory* territoryFactory)
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
						result.push_back(new Territory());
					}
				}

				//now that the pixel has been processed, determine whether this indicates a change in territory
				if (pixel.Value != TerritoryImage::CastleColour.Value && pixel.Value != TerritoryImage::TownColour.Value)
				{
					if (pixel.RGBA.R != lastPixel.RGBA.R
						|| pixel.RGBA.G != lastPixel.RGBA.G
						|| pixel.RGBA.B != lastPixel.RGBA.B)
					{
						//change in colour marks a change in territory, so a territorial boundary is added to both sides
						territoryBounds.push_back(Vector2d(i, j));

						//get a pointer to each in the results array
						Territory* last = nullptr, *current = nullptr;
						for (int colourIndex = 0; colourIndex < territoryColours.size(); colourIndex++)
						{
							if (territoryColours[colourIndex].Value == pixel.Value)
							{
								current = result[colourIndex];
							}

							if (territoryColours[colourIndex].Value == lastPixel.Value)
							{
								last = result[colourIndex];
							}
						}

						//normalise and add
						if (last != nullptr)
						{
							double normX, normZ;
							normX = ((double)bmp->GetWidth() / 100.0) * i;
							normZ = ((double)bmp->GetHeight() / 100.0) * j;

							last->AddLocalBoundary(Pos3(normX, 0, normZ));
						}

						if (current != nullptr)
						{
							double normX, normZ;
							normX = ((double)bmp->GetWidth() / 100.0) * i;
							normZ = ((double)bmp->GetHeight() / 100.0) * j;

							current->AddLocalBoundary(Pos3(normX, 0, normZ));
						}
					}

					lastPixel = pixel;
				}
			}
		}

		//now we should have collections of territory boundaries, castle locations, and town locations
		//the size of each should be the same
		if (result.size() == castleLocations.size() && castleLocations.size() == townLocations.size())
		{
			for (int i = 0; i < result.size(); i++)
			{
				char d[2] = { '0' + i };
				Pos3 castlePos(((double)bmp->GetWidth() / 100.0) * castleLocations[i].X, 0.0, ((double)bmp->GetHeight() / 100.0) * castleLocations[i].Y);
				Pos3 townPos(((double)bmp->GetWidth() / 100.0) * townLocations[i].X, 0.0, ((double)bmp->GetHeight() / 100.0) * townLocations[i].Y);
				std::string name = territoryFactory == nullptr ? ("territory " + std::string(d)) : territoryFactory->CreateRandomTerritoryName();
				result[i]->SetName(name);
				result[i]->SetCastlePosition(castlePos);
				result[i]->SetTownPosition(townPos);
				result[i]->SetPeasantFactory(peasantFactory);
			}
		}



		return result;
	}
}