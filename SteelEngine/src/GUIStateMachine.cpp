#include "GUIStateMachine.h"

namespace Steel
{
	GUIStateMachine::GUIStateMachine()
	{
		StateInfo cur;
		cur.m_currentState = GUIState::G_SPLASH;
		cur.m_nextState = GUIState::G_NONE;
		cur.m_previousState = GUIState::G_NONE;

		m_stack.push(cur);

		m_popNext = false;
		m_stateChanged = true;
	}

	GUIStateMachine::GUIStateMachine(GUIState startingMenu)
	{
		StateInfo cur;
		cur.m_currentState = startingMenu;
		cur.m_nextState = GUIState::G_NONE;
		cur.m_previousState = GUIState::G_NONE;

		m_popNext = false;
		m_stateChanged = true;
		m_stack.push(cur);
	}

	GUIStateMachine::~GUIStateMachine()
	{
		while (m_stack.empty() == false)
			m_stack.pop();
	}

	void GUIStateMachine::ChangeState(GUIState next)
	{
		m_stack.top().m_nextState = next;
		m_stateChanged = true;
	}

	void GUIStateMachine::Back()
	{
		m_popNext = true;
		m_stateChanged = true;
	}

	void GUIStateMachine::Tick()
	{
		m_stateChanged = false;
		if (m_popNext)
		{
			m_stack.pop();
			if (!m_stack.empty())
			{
				m_stack.top().m_nextState = GUIState::G_NONE;
			}
			else
			{
				//what did you do...
				StateInfo wtf;
				wtf.m_currentState = GUIState::G_MAINMENU;
				wtf.m_nextState = GUIState::G_NONE;
				wtf.m_previousState = GUIState::G_NONE;
			}
			m_popNext = false;
		}
		else if (m_stack.top().m_nextState != GUIState::G_NONE)
		{
			StateInfo next;
			next.m_previousState = m_stack.top().m_currentState;
			next.m_currentState = m_stack.top().m_nextState;
			next.m_nextState = G_NONE;

			m_stack.push(next);
		}
	}
}