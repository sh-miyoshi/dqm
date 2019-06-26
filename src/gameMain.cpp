#include "include.h"
#include "gameMain.h"

#define DEBUG_SKIP_BATTLE_IN 1 // debug(set 0 in production)

GameMain::GameMain():result(RTN_CONTINUE){
	stateMgr.PushState(new StateMapMove(this));// debug(�Ƃ肠�����ŏ��̓}�b�v�ړ���)
}

GameMain::~GameMain(){
}

void GameMain::Draw(){
	stateMgr.Draw();
}

GameMain::RtnCode GameMain::Process(){
	stateMgr.Process();
	return result;
}

//--------------------------------------
// �}�b�v�ړ��Ɋւ��鏈��
//--------------------------------------
GameMain::StateMapMove::StateMapMove(GameMain *obj):obj(obj){
	MapMgr::GetInst()->LoadData("�}���^");// debug
}

GameMain::StateMapMove::~StateMapMove(){
}

void GameMain::StateMapMove::Draw(){
	MapMgr::GetInst()->Draw();
}

void GameMain::StateMapMove::Process(){
	switch( MapMgr::GetInst()->Process() ) {
	case MapMgr::RTN_ENCOUNT:
		obj->stateMgr.Mark();
		obj->stateMgr.PushState(new StateBattleIn(obj));
		break;
	case MapMgr::RTN_EVENT:
		obj->stateMgr.PushState(new StateEvent(obj));
		break;
	case MapMgr::RTN_MENU:
		obj->stateMgr.PushState(new StateMenu(obj));
		break;
	}
}

//--------------------------------------
// �C�x���g�Ɋւ��鏈��
//--------------------------------------
GameMain::StateEvent::StateEvent(GameMain *obj):obj(obj){
}

GameMain::StateEvent::~StateEvent(){
}

void GameMain::StateEvent::Draw(){
	DrawString(def::FMX/2-40,def::FMY/2-20,"�C�x���g������",WHITE);
	DrawString(def::FMX/2-40,def::FMY/2+20,"B: �퓬",WHITE);
	DrawString(def::FMX/2-40,def::FMY/2+40,"S: �V���b�v��",WHITE);
	DrawString(def::FMX/2-40,def::FMY/2+60,"�L�����Z��: �}�b�v�ړ�",WHITE);
}

void GameMain::StateEvent::Process(){
	if(CKey::GetInst()->CheckKey(eKEY_DEBUG_B)==1){
		obj->stateMgr.Mark();
		obj->stateMgr.PushState(new StateBattleIn(obj));
	} else if(CKey::GetInst()->CheckKey(eKEY_DEBUG_S)==1){
		obj->stateMgr.PushState(new StateShop(obj));
	} else if(CKey::GetInst()->CheckKey(eKEY_CANCEL)==1){
		obj->stateMgr.PushState(new StateMapMove(obj));
	}
}

//--------------------------------------
// �V���b�v�Ɋւ��鏈��
//--------------------------------------
GameMain::StateShop::StateShop(GameMain *obj):obj(obj){
}

GameMain::StateShop::~StateShop(){
}

void GameMain::StateShop::Draw(){
	DrawString(def::FMX/2-40,def::FMY/2-20,"�V���b�v�Ŕ�������",WHITE);
	DrawString(def::FMX/2-40,def::FMY/2+40,"�L�����Z��: �}�b�v�ړ�",WHITE);
}

void GameMain::StateShop::Process(){
	if(CKey::GetInst()->CheckKey(eKEY_CANCEL)==1){
		obj->stateMgr.PopState();
	}
}

//--------------------------------------
// ���j���[�Ɋւ��鏈��
//--------------------------------------
GameMain::StateMenu::StateMenu(GameMain *obj):obj(obj){
}

GameMain::StateMenu::~StateMenu(){
}

void GameMain::StateMenu::Draw(){
	MapMgr::GetInst()->Draw();

	menu.Draw();
}

void GameMain::StateMenu::Process(){
	switch( menu.Process() ) {
	case Menu::RTN_END:
		obj->stateMgr.PopState();
		break;
	}
}

//--------------------------------------
// �퓬�O����
//--------------------------------------
GameMain::StateBattleIn::StateBattleIn(GameMain *obj):obj(obj),count(0){
}

GameMain::StateBattleIn::~StateBattleIn(){
}

void GameMain::StateBattleIn::Draw(){
	MapMgr::GetInst()->Draw();

	DrawBox(0, 0, def::FMX, count, GetColor(0, 0, 0), TRUE);
	DrawBox(0, 0, count, def::FMY, GetColor(0, 0, 0), TRUE);
	DrawBox(0, def::FMY, def::FMX, def::FMY - count, GetColor(0, 0, 0), TRUE);
	DrawBox(def::FMX, 0, def::FMX - count, def::FMY, GetColor(0, 0, 0), TRUE);
}

void GameMain::StateBattleIn::Process(){
#if DEBUG_SKIP_BATTLE_IN
	obj->stateMgr.PushState(new StateBattle(obj));
#else
	if (count >= def::FMY / 2) {
		obj->stateMgr.PushState(new StateBattle(obj));
	}
	count+=5;
#endif
}

//--------------------------------------
// �퓬����
//--------------------------------------
GameMain::StateBattle::StateBattle(GameMain *obj):obj(obj){
}

GameMain::StateBattle::~StateBattle(){
}

void GameMain::StateBattle::Draw(){
	battle.Draw();
}

void GameMain::StateBattle::Process(){
	switch (battle.Process()) {
	case Battle::eRTN_WIN:
		// TODO(Set Party Data)
		obj->stateMgr.BackToMark();
		break;
	case Battle::eRTN_LOSE:
		obj->result = GameMain::RTN_GAMEOVER;
		break;
	}
}
