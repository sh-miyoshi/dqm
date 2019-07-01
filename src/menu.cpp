#include "include.h"
#include "menu.h"
#include "party.h"
#include "itemMgr.h"

// TODO(stateMgr.Push時にneedUpdateをtrueにして置き、needUpdateがtrueならactive windowを変更する)

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
		obj->stateMgr.Mark();
		obj->stateMgr.PushState(new MenuItemSelect(obj));
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

//-------------------------------------------------------
// 道具の使用に関するクラス(道具の選択)
//-------------------------------------------------------
Menu::MenuItemSelect::MenuItemSelect(Menu* obj)
	:obj(obj), itemWinNo(-1), pageNo(0), descWinNo(-1) {
	UpdateItemDesc(0);

	itemWinNo = obj->winMgr.New(130, 50, ITEM_DRAW_MAX, 7);
	obj->winMgr.SetPageData(Party::GetInst()->GetItemIDs().size(), ITEM_DRAW_MAX);
	UpdateItemList();
}

Menu::MenuItemSelect::~MenuItemSelect() {
	obj->winMgr.Delete(itemWinNo);
	obj->winMgr.Delete(descWinNo);
}

void Menu::MenuItemSelect::Process() {
	int prevCursor = obj->winMgr.GetCursor();
	int selectItemNo = obj->winMgr.MoveCursor();
	int nowCursor = obj->winMgr.GetCursor();

	if( selectItemNo != -1 ) {// 決定キーが押されたら
		// 道具の使い道の選択へ
		obj->stateMgr.PushState(new MenuItemUseOrThrow(obj, selectItemNo));
	} else if( obj->winMgr.PageProcess() ) {// ページ移動だったら
		UpdateItemList();
	} else if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 ) {
		obj->stateMgr.PopState();
	} else if( nowCursor != prevCursor ) {
		UpdateItemDesc(nowCursor);
	}
}

void Menu::MenuItemSelect::UpdateItemList() {
	std::list<std::string> lstr;
	auto partyItemIDs = Party::GetInst()->GetItemIDs();
	for( int i = 0; i < ITEM_DRAW_MAX; i++ ) {
		int itemNo = obj->winMgr.GetNowPage() * ITEM_DRAW_MAX + i;
		if( itemNo >= (int)partyItemIDs.size() ) {
			break;
		}
		lstr.push_back(ItemMgr::GetInst()->GetInfo(partyItemIDs[itemNo]).name);
	}
	obj->winMgr.SetList(lstr);
}

void Menu::MenuItemSelect::UpdateItemDesc(int itemNo) {
	static const int MSG_NUM = 7;
	static const int WIN_DRAW_MAX = 3;

	auto partyItemIDs = Party::GetInst()->GetItemIDs();
	if( 0 <= itemNo && itemNo < (int)partyItemIDs.size() ) {
		int active = obj->winMgr.GetActive();
		if( !obj->winMgr.SetActive(descWinNo) ) {
			descWinNo = obj->winMgr.New(340, 70, WIN_DRAW_MAX, MSG_NUM);
		}

		std::list<std::string> lstr;
		lstr.push_back("説明:");
		auto item = ItemMgr::GetInst()->GetInfo(partyItemIDs[itemNo]);
		ASSERT(item.description.size() < MSG_NUM * 2 * 2, "Item Description is too long");// 折り返しても2行まで
		lstr.push_back(" " + item.description);

		obj->winMgr.SetList(lstr);
		obj->winMgr.SetListData(false, true);
		obj->winMgr.SetActive(active);
	}
}

//-------------------------------------------------------
// 道具の使用に関するクラス(使い道の選択)
//-------------------------------------------------------
Menu::MenuItemUseOrThrow::MenuItemUseOrThrow(Menu* obj, int partyItemNo)
	:obj(obj), winNo(-1) , partyItemNo(partyItemNo)
{
	winNo = obj->winMgr.New(270, 110, 2, 4);
	const std::string str[] = { "使う","捨てる" };
	obj->winMgr.SetList(str, 2);
}

Menu::MenuItemUseOrThrow::~MenuItemUseOrThrow() {
	obj->winMgr.Delete(winNo);
}

void Menu::MenuItemUseOrThrow::Process() {
	int c = obj->winMgr.MoveCursor();
	if( c == 0 ) {// 使う
		auto item = ItemMgr::GetInst()->GetInfo(Party::GetInst()->GetItemIDs()[partyItemNo]);
		if( item.useInFieldOK && item.isTargetSelect ) {// キャラクタ単体なら選択画面へ
			obj->stateMgr.PushState(new MenuItemSelectTarget(obj, partyItemNo));
		} else {
			// それ以外なら結果クラスですべて処理する
			obj->stateMgr.PushState(new MenuItemResult(obj,true, partyItemNo,-1));
		}
	} else if( c == 1 )// 捨てる
		obj->stateMgr.PushState(new MenuItemResult(obj,false, partyItemNo,-1));
	else if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 )
		obj->stateMgr.PopState();
}


//-------------------------------------------------------
// 道具の使用に関するクラス(対象の選択)
//-------------------------------------------------------
Menu::MenuItemSelectTarget::MenuItemSelectTarget(Menu* obj,int partyItemNo)
	:obj(obj), selectWinNo(-1),infoWinNo(-1), partyItemNo(partyItemNo)
{
	UpdateCharInfo(0);

	selectWinNo = obj->winMgr.New(270, 100, Party::PARTY_MAX_NUM, 5);
	std::list<std::string> lstr;
	for( auto p : Party::GetInst()->GetParty() ) {
		lstr.push_back(p.nickName);
	}
	obj->winMgr.SetList(lstr);
}

Menu::MenuItemSelectTarget::~MenuItemSelectTarget() {
	obj->winMgr.Delete(selectWinNo);
	obj->winMgr.Delete(infoWinNo);
}

void Menu::MenuItemSelectTarget::Process() {
	int prevCursor = obj->winMgr.GetCursor();
	int selectCharNo = obj->winMgr.MoveCursor();
	int nowCursor = obj->winMgr.GetCursor();

	if( selectCharNo != -1 ) {// 決定キーが押されたら
		// 結果画面へ
		obj->stateMgr.PushState(new MenuItemResult(obj,true,partyItemNo, selectCharNo));
	} else if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 ) {
		obj->stateMgr.PopState();
	} else if( nowCursor != prevCursor ) {
		UpdateCharInfo(nowCursor);
	}
}

void Menu::MenuItemSelectTarget::UpdateCharInfo(int charNo) {
	int active = obj->winMgr.GetActive();

	if( !obj->winMgr.SetActive(infoWinNo) )
		infoWinNo = obj->winMgr.New(420, 140, 4, 6);
	std::string str[4];
	auto partyInfo = Party::GetInst()->GetParty();
	str[0] = partyInfo[charNo].nickName;
	str[1] = ToString("Level    %3d", partyInfo[charNo].lv);
	str[2] = ToString("HP   %3d/%3d", partyInfo[charNo].hp, partyInfo[charNo].hpMax);
	str[3] = ToString("MP   %3d/%3d", partyInfo[charNo].mp, partyInfo[charNo].mpMax);
	obj->winMgr.SetList(str, 4);
	obj->winMgr.SetListData(false, false);

	obj->winMgr.SetActive(active);
}

//-------------------------------------------------------
// 道具の使用に関するクラス(結果)
//-------------------------------------------------------
Menu::MenuItemResult::MenuItemResult(Menu* obj, bool isUse, int partyItemNo, int targetNo):obj(obj),winNo(-1) {
	winNo = obj->winMgr.New(320, 110, 9, 12);

	std::string msg;
	if( isUse ) {// 選択した道具を使用する
		auto item = ItemMgr::GetInst()->GetInfo(Party::GetInst()->GetItemIDs()[partyItemNo]);

		if( !item.useInFieldOK )
			msg = "その道具は移動中使えません。";
		else {
			msg = Party::GetInst()->GetPlayerName() + "は" + item.name + "を使った/";
			int power = item.power;
			switch( Party::GetInst()->GetItemIDs()[partyItemNo] ) {
			case ItemMgr::ID_薬草:
				ASSERT(targetNo >= 0, "Target is not selected");// 対象が選択されているか
				if( Party::GetInst()->Recover(targetNo, true, power, power / 7) ) {
					Party::GetInst()->DeleteItem(partyItemNo);
					msg += Party::GetInst()->GetParty()[targetNo].name + "はHPを" + ToString(power) + "回復した!";
				} else
					msg += "しかし、失敗した";
				break;
			case ItemMgr::ID_魔法の聖水:
				ASSERT(targetNo >= 0, "Target is not selected");// 対象が選択されているか
				if( Party::GetInst()->Recover(targetNo, false, power, power / 7) ) {
					Party::GetInst()->DeleteItem(partyItemNo);
					msg += Party::GetInst()->GetParty()[targetNo].name + "はMPを" + ToString(power) + "回復した!";
				} else
					msg += "しかし、失敗した";
				break;
			default:
				ASSERT(0, "そんな道具は存在しません");// その道具は存在しないか未実装
				break;
			}
		}
	} else {// 選択した道具を捨てる
		if( Party::GetInst()->DeleteItem(partyItemNo) ) {
			auto item = ItemMgr::GetInst()->GetInfo(Party::GetInst()->GetItemIDs()[partyItemNo]);
			msg = Party::GetInst()->GetPlayerName() + "は" + item.name + "を捨てた。";
		} else {
			msg = "その道具は捨てられません!";
		}
	}

	obj->winMgr.SetMsg(msg);
}

Menu::MenuItemResult::~MenuItemResult() {
	obj->winMgr.Delete(winNo);
}

void Menu::MenuItemResult::Process() {
	if( obj->winMgr.UpdateMsg(g_MessageSpeed) )
		obj->stateMgr.BackToMark();
}
