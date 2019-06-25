#pragma once

#include <string>
#include "fmfFile.h"
#include "point.h"

class MapField{
	enum {
		eMAP_LAYER_CHIP,
		eMAP_LAYER_HIT,
		eMAP_LAYER_EVENT,

		eMAP_LAYER_MAX
	};
	static const int MAP_IMAGE_RECT_SIZE = 16;

	FmfFile mapData;
	int image;
public:
	MapField();
	~MapField();

	void LoadData(std::string mapName);
	void DeleteData();
	void Draw(CPoint<int> centerPos);
	FMF_HEADER GetData();
	bool HitJudge(CPoint<int> pos);
	CPoint<int> GetMapPixelSize();
};