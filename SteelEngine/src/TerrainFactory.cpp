#include "TerrainFactory.h"
#include "StringTrim.h"

namespace Steel
{
	TerrainFactory::TerrainFactory()
	{
		m_randCount = 0;
	}

	TerrainFactory::~TerrainFactory()
	{
		m_randCount = 0;
	}

	Terrain *TerrainFactory::CreateNewTerrain(const std::string &filename)
	{
		std::string name;
		unsigned int width;
		unsigned int height;
		unsigned int scale;
		unsigned char *data = nullptr;	//comes from a corresponding filename.bmp
		std::string hmname;
		std::string diffname;
		std::string detname;

		FILE *f = nullptr;
		fopen_s(&f, filename.c_str(), "r");

		if (f)
		{
			char line[1024] = { 0 };

			while (fgets(line, 1024, f))
			{
				char *pos = strchr(line, '=');
				pos++;

				if (strstr(line, "TerrainName") != nullptr) name = trim(std::string(pos));
				else if (strstr(line, "TerrainWidth") != nullptr) width = atoi(pos);
				else if (strstr(line, "TerrainHeight") != nullptr) height = atoi(pos);
				else if (strstr(line, "TerrainScale") != nullptr) scale = atoi(pos);
				else if (strstr(line, "HeightmapFile") != nullptr) hmname = trim(std::string(pos));
				else if (strstr(line, "DiffusemapFile") != nullptr) diffname = trim(std::string(pos));
				else if (strstr(line, "DetailmapFile") != nullptr) detname = trim(std::string(pos));
			}

			fclose(f);

			if (hmname.size() > 0 && hmname != "random" && hmname.find(".raw") != hmname.npos)
			{
				FILE *hm = nullptr;
				fopen_s(&hm, hmname.c_str(), "rb");
				if (hm)
				{
					unsigned int size = 0;
					fseek(hm, 0, SEEK_END);
					size = ftell(hm);
					rewind(hm);

					int nsize = size;
					if (size != width * height)
					{
						printf("TERRAIN LOADER: Filesize mismatch! Using smaller value...");

						nsize = (size < width * height ? size : (width * height));
					}

					data = new unsigned char[nsize];
					fread_s(data, nsize, 1, nsize, hm);

					fclose(hm);
				}
			}
			else if (hmname == "random")
			{
				//TODO
				m_randCount++;
			}
		}

		return new Terrain(name, data, width, height, scale, hmname, diffname, detname);
	}

	//TODO
	Terrain *TerrainFactory::CreateNewTerrain(const std::string &name, const unsigned int width, const unsigned int height, unsigned char *data, const int scale)
	{
		return nullptr;
	}
}