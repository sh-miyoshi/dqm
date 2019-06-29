#include "include.h"
#include "menu.h"
#include "party.h"

//-------------------------------------------------------
// 全体処理
//-------------------------------------------------------
Menu::Menu():rtnCode(RTN_CONTINUE) {
	stateMgr.PushState(new MenuTop(this));
}

Menu::~Menu() {
}

void Menu::Draw() {
	winMgr.Draw();
	stateMgr.Draw();
}

Menu::RtnCode Menu::Process() {
	stateMgr.Process();
	return rtnCode;
}

void Menu::UpdateParamWindow() {
	std::vector<PartyData> party = Party::GetInst()->GetParty();

	// ウィンドウの容量を作成
	if( charParamWinNo.size() != party.size() ) {
		for( auto winNo : charParamWinNo ) {
			winMgr.Delete(winNo);
		}
		charParamWinNo.clear();// いったん全削除
		for( unsigned i = 0; i < party.size(); i++ )
			charParamWinNo.push_back(-1);
	}

	for( unsigned i = 0; i < party.size(); i++ ) {
		if( !winMgr.SetActive(charParamWinNo[i]) )
			charParamWinNo[i] = winMgr.New(50 + i * 180, 300, 4, 5);
		std::list<std::string> lstr;
		lstr.push_back(party[i].nickName);
		lstr.push_back(ToString(" HP %3d", party[i].hp));
		lstr.push_back(ToString(" MP %3d", party[i].mp));
		lstr.push_back(ToString(" Lv %3d", party[i].lv));
		winMgr.SetList(lstr);
		winMgr.SetListData(false, false);
	}
}

//-------------------------------------------------------
// トップ
//-------------------------------------------------------
Menu::MenuTop::MenuTop(Menu* obj):obj(obj), selWinNo(-1), moneyWinNo(-1) {
	const std::string str[] = { "つよさ","特技","道具","作戦" };
	selWinNo = obj->winMgr.New(75, 40, sizeof(str) / sizeof(str[0]), 5);
	obj->winMgr.SetList(str, sizeof(str) / sizeof(str[0]));

	moneyWinNo = obj->winMgr.New(395, 55, 1, 6);
	obj->winMgr.SetMsg(ToString("お金 %5d G", Party::GetInst()->GetMoney()));
	obj->winMgr.UpdateMsg();

	obj->UpdateParamWindow();

	obj->winMgr.SetActive(selWinNo);
}

Menu::MenuTop::~MenuTop() {
}

void Menu::MenuTop::Process() {
	// TODO(Menuが選択された時の処理を追加)
	switch( obj->winMgr.MoveCursor() ) {
	case 0:// つよさ
		obj->stateMgr.PushState(new MenuPower(obj));
		break;
	case 1:// 特技
		break;
	case 2:// 道具
		break;
	case 3:// 作戦
		break;
	}

	if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 )
		obj->rtnCode = Menu::RTN_END;
}

//-------------------------------------------------------
// つよさ
//-------------------------------------------------------
Menu::MenuPower::MenuPower(Menu* obj):obj(obj), infoWinNo(-1), selWinNo(-1) {
	ASSERT(Party::GetInst()->GetParty().size() > 0, "partyがいません");

	infoWinNo = obj->winMgr.New(310, 50, 9, 10);
	UpdateInfo(0);

	selWinNo = obj->winMgr.New(120, 60, Party::PARTY_MAX_NUM, 5);
	std::list<std::string> lstr;
	for( auto c : Party::GetInst()->GetParty() )
		lstr.push_back(c.nickName);
	obj->winMgr.SetList(lstr);
}

Menu::MenuPower::~MenuPower() {
	obj->winMgr.Delete(infoWinNo);
	obj->winMgr.Delete(selWinNo);
}

void Menu::MenuPower::Process() {
	int prevCursor = obj->winMgr.GetCursor();

	obj->winMgr.MoveCursor();
	int nowCursor = obj->winMgr.GetCursor();
	if( nowCursor != prevCursor ) {
		UpdateInfo(nowCursor);
	} else if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 )
		obj->stateMgr.PopState();
}

void Menu::MenuPower::UpdateInfo(int charNo) {
	auto party = Party::GetInst()->GetParty();

	ASSERT(0 <= charNo && charNo < ( int ) party.size(), "charNoが異常値です");

	int active = obj->winMgr.GetActive();
	obj->winMgr.SetActive(infoWinNo);
	std::list<std::string> lstr;
	lstr.push_back("Level  " + ToString<int>(party[charNo].lv));
	lstr.push_back("攻撃   " + ToString<int>(party[charNo].atk));
	lstr.push_back("防御   " + ToString<int>(party[charNo].def));
	lstr.push_back("素早さ " + ToString<int>(party[charNo].spd));
	lstr.push_back("経験値 " + ToString<int>(party[charNo].exp));
	lstr.push_back("");
	lstr.push_back("");
	lstr.push_back(ToString("HP     %3d/%3d", party[charNo].hp, party[charNo].hpMax));
	lstr.push_back(ToString("MP     %3d/%3d", party[charNo].mp, party[charNo].mpMax));

	obj->winMgr.SetList(lstr);
	obj->winMgr.SetListData(false, false);
	obj->winMgr.SetActive(active);
}
