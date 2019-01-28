#ifndef LOADGAMEGUISHEET_H
#define LOADGAMEGUISHEET_H

#include "AbstractGUISheet.h"
#include "GUIStates.h"
#include <ITexture.h>

namespace Steel
{
	class LoadGameGUISheet : public AbstractGUISheet
	{
	private:

	public:
		LoadGameGUISheet(irr::video::ITexture *backgroundImage, GUIState previousState);
		virtual ~LoadGameGUISheet();

		void Init(int windowWidth, int windowHeight, GameMaster *game);
		void Draw(irr::gui::IGUIEnvironment *irrGUI);
	};
}

#endif