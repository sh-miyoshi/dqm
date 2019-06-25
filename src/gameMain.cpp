#include "include.h"
#include "gameMain.h"

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

	DrawString(def::FMX/2-40,def::FMY/2-20,"戦闘前処理",RED);
	DrawString(def::FMX/2-40,def::FMY/2+40,"5秒後 戦闘",RED);
}

void GameMain::StateBattleIn::Process(){
	if (count >= 5 * def::FPS) {
		obj->stateMgr.PushState(new StateBattle(obj));
	}
	count++;
}

//--------------------------------------
// 戦闘処理
//--------------------------------------
GameMain::StateBattle::StateBattle(GameMain *obj):obj(obj){
}

GameMain::StateBattle::~StateBattle(){
}

void GameMain::StateBattle::Draw(){
	DrawString(def::FMX/2-40,def::FMY/2-20,"戦闘中",WHITE);
	DrawString(def::FMX/2-40,def::FMY/2+40,"決定: 勝利",WHITE);
	DrawString(def::FMX/2-40,def::FMY/2+60,"キャンセル: 敗北",WHITE);
}

void GameMain::StateBattle::Process(){
	if(CKey::GetInst()->CheckKey(eKEY_ENTER)==1){
		obj->stateMgr.BackToMark();
	} else if(CKey::GetInst()->CheckKey(eKEY_CANCEL)==1){
		obj->result=GameMain::RTN_GAMEOVER;
	}
}
