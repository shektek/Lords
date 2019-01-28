#ifndef LOADINGSCREENGUISHEET_H
#define LOADINGSCREENGUISHEET_H

#include "AbstractGUISheet.h"
#include "GUIStates.h"
#include <ITexture.h>

namespace Steel
{
	class LoadingScreenGUISheet : public AbstractGUISheet
	{
		private:

		public:
			LoadingScreenGUISheet(irr::video::ITexture *backgroundImage, GUIState previousState);
			virtual ~LoadingScreenGUISheet();

			void Init(int windowWidth, int windowHeight, GameMaster *game);
			void Draw(irr::gui::IGUIEnvironment *irrGUI);
	};
}

#endif