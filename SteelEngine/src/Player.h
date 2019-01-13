#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "Territory.h"

class Player
{
	private:

	public:
		std::vector<Territory*>		m_ownedTerritories;
};

#endif