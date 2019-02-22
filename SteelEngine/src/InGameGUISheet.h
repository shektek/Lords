#ifndef INGAMEGUISHEET_H
#define INGAMEGUISHEET_H

#include "AbstractGUISheet.h"

namespace Steel
{
	class InGameGUISheet : public AbstractGUISheet
	{
		private:
			irr::gui::IGUIButton	*m_endTurnButton;

		public:
			InGameGUISheet();
			virtual ~InGameGUISheet();

			void Init(int windowWidth, int windowHeight, GameMaster *game);
			void Draw(irr::gui::IGUIEnvironment *irrGUI);

	};
}
#endif