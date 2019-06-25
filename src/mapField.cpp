#include <map>
#include "include.h"
#include "mapField.h"
#include "mapData.h"

MapField::MapField():image(-1){
}

MapField::~MapField() {
}

void MapField::LoadData(std::string mapName) {
	DeleteData();// delete previous data

	MapDefineData_t data = MapDefine::GetInst()->GetData(mapName);
	mapData.LoadData(def::MAP_DATA_FILE_PATH + data.mapFileName);
	image = LoadGraphWithCheckError(def::MAP_DATA_FILE_PATH + data.imageFile, "MapField::MapField");
}

void MapField::DeleteData() {
	if( image != -1 ) {
		DeleteGraph(image);
		image = -1;
	}
}

void MapField::Draw(CPoint<int> centerPos) {
	FMF_HEADER mapInfo = mapData.GetInfo();
	int chipW = mapInfo.byChipWidth;
	int chipH = mapInfo.byChipHeight;
	int chipsX = def::FMX / chipW + 1;
	int chipsY = def::FMY / chipH + 1;
	int ctopX = centerPos.x / chipW;
	int ctopY = centerPos.y / chipH;
	int cofX = centerPos.x%chipW;
	int cofY = centerPos.y%chipH;

	// マップ描画
	for( int x = 0; x < chipsX; x++ ) {
		for( int y = 0; y < chipsY; y++ ) {
			DWORD cx = x + ctopX;
			DWORD cy = y + ctopY;
			if( ( cx < mapInfo.dwWidth ) && ( cy < mapInfo.dwHeight ) ) {
				DWORD dstX = ( x*chipW ) - cofX;
				DWORD dstY = ( y*chipH ) - cofY;

				int pno = mapData.GetValue(eMAP_LAYER_CHIP, cx, cy);
				DWORD srcX = ( pno%MAP_IMAGE_RECT_SIZE )*chipW;
				DWORD srcY = ( pno / MAP_IMAGE_RECT_SIZE )*chipH;

				DrawRectGraph(dstX, dstY, srcX, srcY, chipW, chipH, image, TRUE, FALSE);
			}
		}
	}
}

FMF_HEADER MapField::GetData() {
	return mapData.GetInfo();
}

bool MapField::HitJudge(CPoint<int> pos) {
	int chipX = pos.x / mapData.GetInfo().byChipWidth;
	int chipY = pos.y / mapData.GetInfo().byChipHeight;

	if( mapData.GetValue(eMAP_LAYER_HIT, chipX, chipY) == 0 )// レイヤーが存在してヒットチップなら
		return true;// 進めない場所に当たった

	return false;// 当たっていない
}

CPoint<int> MapField::GetMapPixelSize() {
	int x = GetData().dwWidth*GetData().byChipWidth;
	int y = GetData().dwHeight*GetData().byChipHeight;
	return CPoint<int>(x, y);
}
