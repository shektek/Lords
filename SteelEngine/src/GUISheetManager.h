#ifndef GUISHEETMANAGER_H
#define GUISHEETMANAGER_H

#include <map>
#include "AbstractGUISheet.h"
#include "GUIStates.h"

namespace Steel
{
	class GUISheetManager
	{
		private:
			std::map<GUIState, AbstractGUISheet*> m_sheets;

		public:
			GUISheetManager();
			virtual ~GUISheetManager();

			void				InitSheet(GUIState state, int windowWidth, int windowHeight, GameMaster *game);
			AbstractGUISheet	*GetSheet(GUIState state);
			void				AddSheet(GUIState state, AbstractGUISheet* newSheet);
			void				RemoveSheet(GUIState state);
			void				DrawSheet(GUIState state, irr::gui::IGUIEnvironment *irrGUI);
	};
}

#endif