#include <time.h>
#include "include.h"
#include "fps.h"
#include "state.h"
#include "gameMain.h"
#include "mapData.h"
#include "party.h"
#include "charData.h"
#include "itemMgr.h"

bool g_exitFlag = false;
unsigned long long g_gameCount = 0;
int g_MessageSpeed = 3;// TODO(set in global config file)

class Main{
	class StateTitle:public StateBase{
		Main *obj;
	public:
		StateTitle(Main *obj);
		~StateTitle();

		void Draw();
		void Process();
	};

	class StateGameMain:public StateBase{
		Main *obj;
		GameMain gameMain;
	public:
		StateGameMain(Main *obj);
		~StateGameMain();

		void Draw();
		void Process();
	};

	class StateGameOver:public StateBase{
		Main *obj;
	public:
		StateGameOver(Main *obj);
		~StateGameOver();

		void Draw();
		void Process();
	};

	Fps fps;
	StateMgr stateMgr;
public:
	Main();
	~Main();

	void Process();
};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow){
	// �V�X�e���֌W�̏�����
	srand(( unsigned int ) time(NULL));
	AppLogger::CleanupLogFile();

	// TODO(Add font data)
//	std::string fname = def::TEXT_FILE_PATH + "font.ttf";
//	if( AddFontResourceEx(fname.c_str(), FR_PRIVATE, NULL) <= 0 ){
//		// �t�H���g�ǂݍ��ݎ��s
//		AppLogger::Error("failed to read font file: " + fname);
//	}

	// DxLib�֌W�̏�����
	ChangeWindowMode(TRUE);//�E�B���h�E���[�h
	SetGraphMode(def::FMX, def::FMY, 16);//�E�B���h�E�T�C�Y�ύX
	SetOutApplicationLogValidFlag(TRUE);//log��\������
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	// �A�v���P�[�V�����֌W�̏�����
	MapDefine::GetInst()->LoadData();
	CharDataMgr::GetInst()->LoadData();
	ItemMgr::GetInst()->LoadData();
	Party::GetInst()->InitData();// debug(�^�C�g����ʂ���I������)

	// ���C������
	Main main;
	main.Process();

	// �A�v���P�[�V�����֌W�̏I������
	// now nothing to do

	DxLib_End();
	return 0;
}

Main::Main(void){
	stateMgr.PushState(new StateGameMain(this));// debug(�������)
//	stateMgr.PushState(new StateTitle(this));
}

Main::~Main(void){
}

void Main::Process(){
	while( ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 ){
		// ���C������
		stateMgr.Process();
		stateMgr.Draw();

		CKey::GetInst()->Update();
		fps.Process();// fps����
		fps.Draw(def::FMX - 60, 10);// debug(FPS�̕`��)

		if( g_exitFlag )
			break;

		// debug(�����I��)
		if( CKey::GetInst()->CheckKey(eKEY_START) == 1 )
			break;

		g_gameCount++;
	}
}

Main::StateTitle::StateTitle(Main *obj):obj(obj){
}

Main::StateTitle::~StateTitle(){
}

void Main::StateTitle::Draw(){
	DrawString(def::FMX/2-40,def::FMY/2,"Title",WHITE);
}

void Main::StateTitle::Process(){
	if(CKey::GetInst()->CheckKey(eKEY_ENTER)==1){
		obj->stateMgr.PushState(new StateGameMain(obj));
	}
}

Main::StateGameMain::StateGameMain(Main *obj):obj(obj){
}

Main::StateGameMain::~StateGameMain(){
}

void Main::StateGameMain::Draw(){
	gameMain.Draw();
}

void Main::StateGameMain::Process(){
	switch(gameMain.Process()){
	case GameMain::RTN_GAMEOVER:
		// �Q�[���I�[�o�[�֑J��
		obj->stateMgr.PushState(new StateGameOver(obj));
		break;
	default:
		break;
	}
}

Main::StateGameOver::StateGameOver(Main *obj):obj(obj){
}

Main::StateGameOver::~StateGameOver(){
}

void Main::StateGameOver::Draw(){
	DrawString(def::FMX/2-40,def::FMY/2,"GameOver",WHITE);
}

void Main::StateGameOver::Process(){
	if(CKey::GetInst()->CheckKey(eKEY_ENTER)==1){
		obj->stateMgr.PushState(new StateTitle(obj));
	}
}
