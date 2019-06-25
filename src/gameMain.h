#pragma once

#include "state.h"
#include "mapMgr.h"
#include "menu.h"

class GameMain{
public:
	enum RtnCode{
		RTN_CONTINUE,
		RTN_GAMEOVER
	};
private:
	class StateMapMove:public StateBase{
		GameMain *obj;
	public:
		StateMapMove(GameMain *obj);
		~StateMapMove();

		void Draw();
		void Process();
	};

	class StateEvent:public StateBase{
		GameMain *obj;
	public:
		StateEvent(GameMain *obj);
		~StateEvent();

		void Draw();
		void Process();
	};

	class StateShop:public StateBase{
		GameMain *obj;
	public:
		StateShop(GameMain *obj);
		~StateShop();

		void Draw();
		void Process();
	};

	class StateMenu:public StateBase{
		Menu menu;
		GameMain *obj;
	public:
		StateMenu(GameMain *obj);
		~StateMenu();

		void Draw();
		void Process();
	};

	class StateBattleIn:public StateBase{
		int count;
		GameMain *obj;
	public:
		StateBattleIn(GameMain *obj);
		~StateBattleIn();

		void Draw();
		void Process();
	};

	class StateBattle:public StateBase{
		GameMain *obj;
	public:
		StateBattle(GameMain *obj);
		~StateBattle();

		void Draw();
		void Process();
	};

	StateMgr stateMgr;
	RtnCode result;
public:
	GameMain();
	~GameMain();

	void Draw();
	RtnCode Process();
};