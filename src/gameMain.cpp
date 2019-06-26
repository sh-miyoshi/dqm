#include "include.h"
#include "gameMain.h"

#define DEBUG_SKIP_BATTLE_IN 1 // debug(set 0 in production)

GameMain::GameMain():result(RTN_CONTINUE){
	stateMgr.PushState(new StateMapMove(this));// debug(とりあえず最初はマップ移動で)
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
// マップ移動に関する処理
//--------------------------------------
GameMain::StateMapMove::StateMapMove(GameMain *obj):obj(obj){
	MapMgr::GetInst()->LoadData("マルタ");// debug
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
// イベントに関する処理
//--------------------------------------
GameMain::StateEvent::StateEvent(GameMain *obj):obj(obj){
}

GameMain::StateEvent::~StateEvent(){
}

void GameMain::StateEvent::Draw(){
	DrawString(def::FMX/2-40,def::FMY/2-20,"イベント処理中",WHITE);
	DrawString(def::FMX/2-40,def::FMY/2+20,"B: 戦闘",WHITE);
	DrawString(def::FMX/2-40,def::FMY/2+40,"S: ショップへ",WHITE);
	DrawString(def::FMX/2-40,def::FMY/2+60,"キャンセル: マップ移動",WHITE);
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
// ショップに関する処理
//--------------------------------------
GameMain::StateShop::StateShop(GameMain *obj):obj(obj){
}

GameMain::StateShop::~StateShop(){
}

void GameMain::StateShop::Draw(){
	DrawString(def::FMX/2-40,def::FMY/2-20,"ショップで買い物中",WHITE);
	DrawString(def::FMX/2-40,def::FMY/2+40,"キャンセル: マップ移動",WHITE);
}

void GameMain::StateShop::Process(){
	if(CKey::GetInst()->CheckKey(eKEY_CANCEL)==1){
		obj->stateMgr.PopState();
	}
}

//--------------------------------------
// メニューに関する処理
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
// 戦闘前処理
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
// 戦闘処理
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
