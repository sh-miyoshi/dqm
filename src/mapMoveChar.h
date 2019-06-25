#pragma once

#include <string>
#include "point.h"
#include "mapField.h"

class MapMoveChar {
	int *image;
	int xnum, ynum;

	MapMoveChar(const MapMoveChar& data);// とりあえず使用禁止
public:
	MapMoveChar(std::string fileName,int xnum, int ynum);
	~MapMoveChar();

	void Draw(CPoint<int> pos, int muki, int animOfs, bool isWalk);
};

class MapMovePlayer:public MapMoveChar{
	CPoint<int> pos;
	int muki;
	bool isMove;
	int count;

	bool CheckMapSize(int pos, int charSize, int mapSize);
	bool CheckMove(CPoint<int> pos, int muki, MapField fieldData);
	CPoint<int> MoveMuki(CPoint<int> pos, CPoint<int> spd, int muki);
public:
	enum RtnCode{
		RTN_CONTINUE,
		RTN_ENCOUNT,
		RTN_EVENT
	};

	MapMovePlayer();
	~MapMovePlayer();

	void Draw(CPoint<int> ofsPlayerPos);// ofsPlayerPos: マップ補正後のキャラの位置
	RtnCode Process(MapField fieldData);

	CPoint<int> GetPos()const { return pos; }
};