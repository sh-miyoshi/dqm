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
		Menu* obj;
	public:
		MenuTop(Menu* obj);
		~MenuTop();

		void Process();
	};

	// �悳��ʂ̕\���N���X
	class MenuPower:public StateBase {
		int selWinNo;
		int infoWinNo;
		Menu* obj;

		void UpdateInfo(int charNo);
	public:
		MenuPower(Menu* obj);
		~MenuPower();

		void Process();
	};

	// ����̎g�p�Ɋւ���N���X(����̑I��)
	class MenuItemSelect:public StateBase {
		static const int ITEM_DRAW_MAX = 10;

		Menu* obj;

		int itemWinNo;
		int descWinNo;
		int pageNo;

		void UpdateItemList();
		void UpdateItemDesc(int itemNo);
	public:
		MenuItemSelect(Menu* obj);
		~MenuItemSelect();

		void Process();
	};

	// ����̎g�p�Ɋւ���N���X(�g�����̑I��)
	class MenuItemUseOrThrow:public StateBase {
		int partyItemNo;
		int winNo;
		Menu* obj;
	public:
		MenuItemUseOrThrow(Menu* obj, int partyItemNo);
		~MenuItemUseOrThrow();

		void Process();
	};

	// ����̎g�p�Ɋւ���N���X(�Ώۂ̑I��)
	class MenuItemSelectTarget:public StateBase {
		Menu* obj;
		int selectWinNo;
		int infoWinNo;
		int partyItemNo;

		void UpdateCharInfo(int charNo);
	public:
		MenuItemSelectTarget(Menu* obj,int partyItemNo);
		~MenuItemSelectTarget();

		void Process();
	};

	// ����̎g�p�Ɋւ���N���X(����)
	class MenuItemResult:public StateBase {
		int winNo;
		Menu* obj;
	public:
		MenuItemResult(Menu* obj, bool isUse, int partyItemNo, int targetNo);
		~MenuItemResult();

		void Process();
	};

	std::vector<int> charParamWinNo;
	bool requireActivateTopWin;
	RtnCode rtnCode;
	WinMgr winMgr;
	StateMgr stateMgr;

	void UpdateParamWindow();
	void PushState(StateBase *state);
	void PopState();
public:
	Menu();
	~Menu();

	void Draw();
	RtnCode Process();
};