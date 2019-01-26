#ifndef MAINMENUGUISHEET_H
#define MAINMENUGUISHEET_H

#include "AbstractGUISheet.h"
#include <IGUIImage.h>
#include <IGUIButton.h>
#include <ITexture.h>

namespace Steel
{
	class MainMenuGUISheet : public AbstractGUISheet
	{
		private:			
			irr::video::ITexture *m_logoTexture;
			irr::gui::IGUIImage	 *m_logoImage;
			irr::gui::IGUIButton *m_newGameButton;
			irr::gui::IGUIButton *m_loadGameButton;
			irr::gui::IGUIButton *m_optionsButton;
			irr::gui::IGUIButton *m_quitButton;

		public:
			MainMenuGUISheet(irr::video::ITexture *logoTexture);
			virtual ~MainMenuGUISheet();

			void				Init(int windowWidth, int windowHeight, GameMaster *game);
			void				Draw(irr::gui::IGUIEnvironment *irrGUI);
	};
}

#endif