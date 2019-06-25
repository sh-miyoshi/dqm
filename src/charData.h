#pragma once

#include <string>
#include <vector>
#include "point.h"

class CharData {
public:
	static const int SKILL_MAX = 8;

	std::string name;
	int hp, hpMax;
	int mp, mpMax;
	int atk, def;
	int lv;
	int spd;
	int image;
	std::string charType;

	int exp;
	int expPtn;
//	std::vector<int> skillData;// TODO(Add 特技情報)

	CharData();
	~CharData();

	void LoadImage();
	void DeleteImage();

	void Draw(CPoint<int> pos);
};

class PartyData:public CharData {
public:
	std::string nickName;
//	std::vector<int> nowSkill;// TODO(Add 特技情報)

	PartyData(
		std::string nickName,
		int hp,
		int mp,
		int atk,
		int def,
		int lv,
		int spd,
		int exp,
		int charID
	);
	~PartyData();

	// todo(Save関数)
};

class CharDataMgr {
public:
	// charData.csvファイルと合わせる必要あり
	enum CharID {
		ID_スライム,
		ID_ぶちスライム,
		ID_サボテンボール,
		ID_とさかへび,
		ID_テールイーター,

		ID_MAX
	};
private:
	bool isLoad;
	CharData charData[ID_MAX];

	CharDataMgr():isLoad(false) {}
	~CharDataMgr() {}
public:
	static CharDataMgr *GetInst() {
		static CharDataMgr inst;
		return &inst;
	}

	void LoadData();
	void DeleteData();

	CharData GetData(int charID);
};