#include "include.h"
#include "menu.h"
#include "party.h"

//-------------------------------------------------------
// �S�̏���
//-------------------------------------------------------
Menu::Menu():rtnCode(RTN_CONTINUE){
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

	for( unsigned i = 0; i<party.size(); i++ ) {
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
Menu::MenuTop::MenuTop(Menu * obj):obj(obj), selWinNo(-1), moneyWinNo(-1){
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
		break;
	case 1:// ���Z
		break;
	case 2:// ����
		break;
	case 3:// ���
		break;
	}

	if( CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 )
		obj->rtnCode = Menu::RTN_END;
}
