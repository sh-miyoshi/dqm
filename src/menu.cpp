#include "include.h"
#include "menu.h"
#include "party.h"
#include "itemMgr.h"

//-------------------------------------------------------
// �S�̏���
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

	// �E�B���h�E�̗e�ʂ��쐬
	if( charParamWinNo.size() != party.size() ) {
		for( auto winNo : charParamWinNo ) {
			winMgr.Delete(winNo);
		}
		charParamWinNo.clear();// ��������S�폜
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
// �g�b�v
//-------------------------------------------------------
Menu::MenuTop::MenuTop(Menu* obj):obj(obj), selWinNo(-1), moneyWinNo(-1) {
	const std::string str[] = { "�悳","���Z","����","���" };
	selWinNo = obj->winMgr.New(75, 40, sizeof(str) / sizeof(str[0]), 5);
	obj->winMgr.SetList(str, sizeof(str) / sizeof(str[0]));

	moneyWinNo = obj->winMgr.New(395, 55, 1, 6);
	obj->winMgr.SetMsg(ToString("���� %5d G", Party::GetInst()->GetMoney()));
	obj->winMgr.UpdateMsg();

	obj->UpdateParamWindow();

	obj->winMgr.SetActive(selWinNo);
}

Menu::MenuTop::~MenuTop() {
}

void Menu::MenuTop::Process() {
	// TODO(Menu���I�����ꂽ���̏�����ǉ�)
	switch( obj->winMgr.MoveCursor() ) {
	case 0:// �悳
		obj->stateMgr.PushState(new MenuPower(obj));
		break;
	case 1:// ���Z
		break;
	case 2:// ����
		obj->stateMgr.Mark();
		obj->stateMgr.PushState(new MenuItemSelect(obj));
		break;
	case 3:// ���
		break;
	}

	if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 )
		obj->rtnCode = Menu::RTN_END;
}

//-------------------------------------------------------
// �悳
//-------------------------------------------------------
Menu::MenuPower::MenuPower(Menu* obj):obj(obj), infoWinNo(-1), selWinNo(-1) {
	ASSERT(Party::GetInst()->GetParty().size() > 0, "party�����܂���");

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

	ASSERT(0 <= charNo && charNo < ( int ) party.size(), "charNo���ُ�l�ł�");

	int active = obj->winMgr.GetActive();
	obj->winMgr.SetActive(infoWinNo);
	std::list<std::string> lstr;
	lstr.push_back("Level  " + ToString<int>(party[charNo].lv));
	lstr.push_back("�U��   " + ToString<int>(party[charNo].atk));
	lstr.push_back("�h��   " + ToString<int>(party[charNo].def));
	lstr.push_back("�f���� " + ToString<int>(party[charNo].spd));
	lstr.push_back("�o���l " + ToString<int>(party[charNo].exp));
	lstr.push_back("");
	lstr.push_back("");
	lstr.push_back(ToString("HP     %3d/%3d", party[charNo].hp, party[charNo].hpMax));
	lstr.push_back(ToString("MP     %3d/%3d", party[charNo].mp, party[charNo].mpMax));

	obj->winMgr.SetList(lstr);
	obj->winMgr.SetListData(false, false);
	obj->winMgr.SetActive(active);
}

//-------------------------------------------------------
// ����̎g�p�Ɋւ���N���X(����̑I��)
//-------------------------------------------------------
Menu::MenuItemSelect::MenuItemSelect(Menu* obj)
	:obj(obj), itemWinNo(-1), pageNo(0),descWinNo(-1)
{
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

	if( selectItemNo != -1 ) {// ����L�[�������ꂽ��
		// TODO(����̎g�����̑I����)
	} else if( obj->winMgr.PageProcess() ) {// �y�[�W�ړ���������
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
		if( itemNo >= partyItemIDs.size() ) {
			break;
		}
		lstr.push_back(ItemMgr::GetInst()->GetInfo(partyItemIDs[itemNo]).name);
	}
	obj->winMgr.SetList(lstr);
}

void Menu::MenuItemSelect::UpdateItemDesc(int itemNo) {
	// TODO(������)
}

//-------------------------------------------------------
// ����̎g�p�Ɋւ���N���X(�g�����̑I��)
//-------------------------------------------------------
Menu::MenuItemUseOrThrow::MenuItemUseOrThrow(Menu* obj) {
}

Menu::MenuItemUseOrThrow::~MenuItemUseOrThrow() {
}

void Menu::MenuItemUseOrThrow::Process() {
}

//-------------------------------------------------------
// ����̎g�p�Ɋւ���N���X(�Ώۂ̑I��)
//-------------------------------------------------------
Menu::MenuItemSelectTarget::MenuItemSelectTarget(Menu* obj) {
}

Menu::MenuItemSelectTarget::~MenuItemSelectTarget() {
}

void Menu::MenuItemSelectTarget::Process() {
}

//-------------------------------------------------------
// ����̎g�p�Ɋւ���N���X(����)
//-------------------------------------------------------
Menu::MenuItemResult::MenuItemResult(Menu* obj) {
}

Menu::MenuItemResult::~MenuItemResult() {
}

void Menu::MenuItemResult::Process() {
}
