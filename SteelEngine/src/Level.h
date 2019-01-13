#ifndef LEVEL_H

#include <string>
#include <vector>
#include "Territory.h"
#include "Terrain.h"

namespace Steel
{
	class Level
	{
	private:
		std::string				m_levelName;
		std::string				m_levelFilename;

	public:
		//it'd be better to have these hidden tbh
		Terrain					*terrain;
		std::vector<Territory*>	territories;

		//these either shouldn't be public or should be put elsewhere
		double					relWidth;	//terrain->scale * terrain->width
		double					relHeight;	//terrain->scale * terrain->height

		Level();
		Level(std::string levelName, Terrain *ter, std::vector<Territory*> terrs, std::string levelFilename);
		Level(const Level &other);
		Level(Level *other);
		~Level();

		inline std::string		GetLevelName() const { return m_levelName; }
		inline std::string		GetLevelFilename() const { return m_levelFilename; }

		inline void				SetLevelName(std::string levelName) { m_levelName = levelName; }
		inline void				SetLevelFilename(std::string levelFilename){ m_levelFilename = levelFilename; }
	};
}

#endif