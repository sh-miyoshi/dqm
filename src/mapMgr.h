#pragma once

#include <string>
#include "mapField.h"
#include "mapMoveChar.h"

class MapMgr{
	MapField fieldData;
	MapMovePlayer playerMove;

	MapMgr(){}
	~MapMgr(){}

	int MapAdjust(int pos, int fieldSize, int mapSize);
public:
	enum RtnCode{
		RTN_CONTINUE,
		RTN_EVENT,
		RTN_ENCOUNT,
		RTN_MENU,
	};

	static MapMgr *GetInst(){
		static MapMgr inst;
		return &inst;
	}

	void LoadData(std::string mapName);
	void DeleteData();

	void Draw();
	RtnCode Process();
};