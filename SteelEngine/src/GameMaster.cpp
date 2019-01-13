#include <cstring>

#include "GameMaster.h"
#include "StringTrim.h"

namespace Steel
{
	GameMaster::GameMaster()
	{
		peasantFactory = nullptr;
		territoryFactory = nullptr;
		resourceFactory = nullptr;
		terrainGenerator = nullptr;
		terrainFactory = nullptr;
		levels.clear();
	}

	GameMaster::~GameMaster()
	{
		delete peasantFactory;
		delete territoryFactory;
		delete resourceFactory;
		delete terrainGenerator;
		delete terrainFactory;
		for (Level *l : levels)
			delete l;
		levels.clear();
	}

	bool GameMaster::AddLevel(const std::string &levelFilename)
	{
		bool ret = true;
		int state = 0;
		bool terrainFound = false;
		bool territoryFound = false;

		FILE *f = nullptr;
		fopen_s(&f, levelFilename.c_str(), "r");

		if (f)
		{
			levels.push_back(new Level());
			levels.back()->SetLevelFilename(levelFilename);
			char line[1024] = { 0 };

			int tInd = 0;
			std::vector<Pos3> coords;

			while (fgets(line, 1023, f))
			{
				if (state == 0)
				{
					//don't even bother with comment lines
					if (strlen(line) > 1)
					if (line[0] == '/' && line[1] == '/')
						continue;

					char *epos = strchr(line, '=');

					if (epos != nullptr)
					{
						epos++;

						if (strstr(line, "terrain"))
						{
							levels.back()->terrain = terrainFactory->CreateNewTerrain(trim(std::string(epos)));
							//set the relWidth and relHeight
							levels.back()->relWidth = levels.back()->terrain->GetScale() * (levels.back()->terrain->GetWidth() - 1);
							levels.back()->relHeight = levels.back()->terrain->GetScale() * (levels.back()->terrain->GetHeight() - 1);
							terrainFound = true;
						}

						if (strstr(line, "levelname"))
						{
							levels.back()->SetLevelName(trim(std::string(epos)));
						}

						if (strstr(line, "territorydeclare"))
						{
							char *cpos = strchr(epos, ',');
							if (cpos)
							{
								char territoryInt[3] = { 0 };	//max 999 territories, big enough for Europe/Middle Earth sim
								int dig = 0;
								char *tpos = epos;
								while (tpos != cpos && dig < 3)
								{
									territoryInt[dig] = *tpos;
									dig++;
									tpos++;
								}

								int tnum = atoi(territoryInt);
								tpos++;	//get past the comma
								cpos = strchr(tpos, ',');
								//now look for the territory name to be displayed in-game
								dig = 0;
								char tname[64] = { 0 };
								while (tpos != cpos && dig < 64)
								{
									tname[dig] = *tpos;
									dig++;
									tpos++;
								}

								std::string tnamestr = trim(std::string(tname));

								//now there should be a list of normalised coordinates...
								char *bpos = strchr(tpos, '{');
								if (bpos)
								{
									sprintf(line, "%s", bpos);
									state = 1;
								}

								printf("Territory %d name = %s\n", tnum, tnamestr.c_str());
								tInd = tnum;
								if (levels.back()->territories.size() < tnum + 1)
								{
									levels.back()->territories.resize(tnum + 1);
									levels.back()->territories[tInd] = new Territory(tnamestr, peasantFactory);
								}
							}
							else ret = false;
						}
					}
				}

				//read coordinates
				if (state == 1)
				{
					//tokenise the string on the ; character
					char *token = strtok(line, ";");
					while (token != nullptr)
					{
						//this will indicate the final token
						if (strchr(token, '}')) state = 2;

						//look for a pair of comma delimited values
						char *cpos = strchr(token, ',');
						char *tpos = token;
						if (cpos)
						{
							int dig = 0;
							char xstr[64] = { 0 };	//some precision will be required for large maps
							while (tpos != cpos && dig < 64)
							{
								xstr[dig] = *tpos;
								dig++;
								tpos++;
							}

							tpos++;

							double x = atof(xstr);
							double y = 0.0;

							dig = 0;
							char ystr[64] = { 0 };
							//unfortunately, strtok pulls out all the delimiters
							while (*tpos != ';' && *tpos != '\n' && dig < 64)	//this is a bit dirty but it works...
							{
								ystr[dig] = *tpos;
								dig++;
								tpos++;
							}

							y = atof(ystr);
							printf("x = %.8f y = %.8f\n", x, y);
							coords.push_back(Pos3{ x, 0.0, y });
						}

						//next token
						token = strtok(nullptr, ";");
					}
				}

				//finished parsing a territory
				if (state == 2)
				{
					levels.back()->territories[tInd]->SetLocalBounds(coords);
					coords.clear();
					territoryFound = true;
					state = 0;
				}
			}

			fclose(f);
		}

		if (territoryFound && terrainFound)
		{
			for (Territory *t : levels.back()->territories)
			{
				std::vector<Pos3> mapBounds;
				std::vector<Pos3> bounds = t->GetLocalBounds();

				for (int i = 0; i < bounds.size(); i++)
				{
					Pos3 realBounds;

					realBounds.x = bounds[i].x * levels.back()->relWidth;
					realBounds.y = 0;
					realBounds.z = bounds[i].z * levels.back()->relHeight;

					mapBounds.push_back(realBounds);
				}

				t->SetMapBounds(mapBounds);
			}
		}

		if (ret)
			currentLevel = levels.front();

		return ret;
	}

	bool GameMaster::SetCurrentLevel(const std::string &levelFilename)
	{
		bool result = false;

		for (Level *l : levels)
		{
			if (l->GetLevelFilename() == levelFilename)
			{
				currentLevel = l;
				result = true;
				break;
			}
		}

		return result;
	}

	bool GameMaster::NextLevel()
	{
		bool result = false;
		bool nextOne = false;
		for (Level *l : levels)
		{
			if (nextOne)
			{
				currentLevel = l;
				result = true;
				break;
			}
			
			if (l == currentLevel)
			{
				nextOne = true;
			}
		}

		return result;
	}

	void GameMaster::PassSeason()
	{
		for (Territory *t : currentLevel->territories)
		{
			t->PassSeason();
		}
	}
}
