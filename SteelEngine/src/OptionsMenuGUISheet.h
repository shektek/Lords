#ifndef OPTIONSMENUGUISHEET_H
#define OPTIONSMENUGUISHEET_H

#include "AbstractGUISheet.h"
#include "GUIStates.h"
#include <ITexture.h>

namespace Steel
{
	class OptionsMenuGUISheet : public AbstractGUISheet
	{
	private:

	public:
		OptionsMenuGUISheet(irr::video::ITexture *backgroundImage, GUIState previousState);
		virtual ~OptionsMenuGUISheet();

		void Init(int windowWidth, int windowHeight, GameMaster *game);
		void Draw(irr::gui::IGUIEnvironment *irrGUI);
	};
}

#endif