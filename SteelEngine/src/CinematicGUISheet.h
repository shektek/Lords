#ifndef CINEMATICGUISHEET_H
#define CINEMATICGUISHEET_H

#include "AbstractGUISheet.h"
#include "GUIStates.h"
#include <ITexture.h>

namespace Steel
{
	class CinematicGUISheet : public AbstractGUISheet
	{
	private:

	public:
		CinematicGUISheet(irr::video::ITexture *backgroundImage, GUIState previousState);
		virtual ~CinematicGUISheet();

		void Init(int windowWidth, int windowHeight, GameMaster *game);
		void Draw(irr::gui::IGUIEnvironment *irrGUI);
	};
}

#endif