#pragma once

#include <vector>
#include "winMgr.h"
#include "state.h"

class Menu {
public:
	enum RtnCode {
		RTN_CONTINUE,
		RTN_END
	};
private:
	class MenuTop:public StateBase {
		int selWinNo;
		int moneyWinNo;
		Menu *obj;
	public:
		MenuTop(Menu *obj);
		~MenuTop();

		void Process();
	};

	// TODO(‚Â‚æ‚³‰æ–Ê‚Ì•\Ž¦ƒNƒ‰ƒX‚ð’Ç‰Á)
	class MenuPower:public StateBase {
		int selWinNo;
		int infoWinNo;
		Menu *obj;
	public:
		MenuPower(Menu *obj);
		~MenuPower();

		void Process();
	};

	std::vector<int> charParamWinNo;
	RtnCode rtnCode;
	WinMgr winMgr;
	StateMgr stateMgr;

	void UpdateParamWindow();
public:
	Menu();
	~Menu();

	void Draw();
	RtnCode Process();
};