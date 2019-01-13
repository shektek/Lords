#ifndef GUIMENU_H
#define GUIMENU_H

namespace Steel
{
	enum MainMenuID
	{
		M_MM_NONE = 100,
		M_MM_NEWGAME_ID,
		M_MM_LOADGAME_ID,
		M_MM_OPTIONS_ID,
		M_MM_QUIT_ID,
		M_MM_MAX
	};

	enum NewGameMenuID
	{
		M_NG_NONE = MainMenuID::M_MM_MAX,
		M_NG_STARTGAME_ID, 
		M_NG_BACK_ID,
		M_NG_GAMETAB_ID,
		M_NG_CHECKCAMPAIGN_ID,
		M_NG_CHECKSCENARIO_ID,
		M_NG_REGENRANDOM_ID,
		M_NG_MINTERRITORIES_ID,
		M_NG_MAXTERRITORIES_ID,
		M_NG_MAX
	};

	enum LoadGameMenuID
	{
		M_LG_NONE = NewGameMenuID::M_NG_MAX,
		M_LG_SAVELIST_ID,
		M_LG_LOADGAME_ID,
		M_LG_BACK_ID,
		M_LG_MAX
	};

	enum OptionsMenuID
	{
		M_OM_NONE = LoadGameMenuID::M_LG_MAX,
		//...
		M_OM_APPLY_ID,
		M_OM_BACK,
		M_OM_MAX
	};
}
#endif