#include "include.h"
#include "charData.h"
#include "csv.h"

CharData::CharData()
	:mp(0), mpMax(0), hp(0), hpMax(0), atk(0), def(0), lv(0), spd(0), image(-1), exp(0)
	, expPtn(0) {
}

CharData::~CharData() {
}

void CharData::LoadImage() {
	std::string fname = def::CHAR_IMAGE_PATH + name + ".png";
	image = LoadGraphWithCheckError(fname, "CharData::LoadImage");
}

void CharData::DeleteImage() {
	DeleteGraph(image);
	image = -1;
}

void CharData::Draw(CPoint<int> pos) {
	if( image != -1 ) {
		DrawGraph(pos.x, pos.y, image, TRUE);
	}
}

bool CharData::Recover(bool targetIsHP, int point, int rndMax) {
	if( hp <= 0 )// ‚»‚ÌƒLƒƒƒ‰‚ªŽ€‚ñ‚Å‚¢‚½‚çŽ¸”s(AliveŠÖ”‚Å‘Î‰ž)
		return false;
	int& target = targetIsHP ? hp : mp;
	int& max = targetIsHP ? hpMax : mpMax;
	if( target < max ) {
		unsigned recover = point;
		if( rndMax > 0 )// —”‚ð‚Â‚¯‚é‚È‚ç
			recover += GetRand(rndMax - 1);// ‚»‚Ì•ª‰ÁŽZ
		target += recover;
		if( target > max )
			target = max;
		return true;
	}
	return false;
}

void CharDataMgr::LoadData() {
	const std::string inputFile = def::TEXT_FILE_PATH + "charData.csv";
	CSV data(inputFile.c_str(), CSV::MODE_READ);
	std::string temp[256];
	for( int i = 0, num = 0; ( num = data.getData(temp) ) > 0; i++ ) {
		charData[i].name = temp[0];
		charData[i].hp = atoi(temp[1].c_str());
		charData[i].hpMax = charData[i].hp;
		charData[i].mp = atoi(temp[2].c_str());
		charData[i].mpMax = charData[i].mp;
		charData[i].atk = atoi(temp[3].c_str());
		charData[i].def = atoi(temp[4].c_str());
		charData[i].lv = atoi(temp[5].c_str());
		charData[i].exp = atoi(temp[6].c_str());
		charData[i].expPtn = atoi(temp[7].c_str());
		charData[i].charType = temp[8];
	}
	isLoad = true;
}

void CharDataMgr::DeleteData() {
	for( int i = 0; i < ID_MAX; i++ )
		charData[i].DeleteImage();
	isLoad = false;
}

CharData CharDataMgr::GetData(int charID) {
	ASSERT(isLoad, "CharDataMgr::GetData not load data");
	ASSERT(0 <= charID&&charID < ID_MAX, ToString("CharDataMgr::GetData charID(%d) is out of range", charID));
	return charData[charID];
}

PartyData::PartyData(std::string nickName, int hp, int mp, int atk, int def, int lv, int spd, int exp, int charID) {
	CharData temp = CharDataMgr::GetInst()->GetData(charID);

	this->name = temp.name;
	this->image = temp.image;
	this->expPtn = temp.expPtn;
	this->charType = temp.charType;

	this->hp = this->hpMax = hp;
	this->mp = this->mpMax = mp;
	this->atk = atk;
	this->def = def;
	this->lv = lv;
	this->spd = spd;
	this->exp = exp;
	this->nickName = nickName;
}

PartyData::~PartyData() {
}
