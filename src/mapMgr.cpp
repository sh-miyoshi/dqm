#include "include.h"
#include "mapMgr.h"

void MapMgr::LoadData(std::string mapName) {
	fieldData.LoadData(mapName);
}

void MapMgr::DeleteData() {
	fieldData.DeleteData();
}

void MapMgr::Draw() {

	//	�}�b�v�̕\���ʒu����l���L�����̈ʒu����v�Z����B
	//	��{�͉�ʂ̒��S�Ɏ�l���L������\�����邪�A�}�b�v�O���`�悳��Ȃ��l��
	//	�}�b�v�̒[�̏ꍇ�͎�l���L���������S����O���l�ɂ���B
	CPoint<int> playerPos(( def::FMX - def::CHAR_IMAGE_RECT_SIZE ) / 2, ( def::FMY - def::CHAR_IMAGE_RECT_SIZE ) / 2);
	CPoint<int> mapPos = playerMove.GetPos() - playerPos;
	CPoint<int> ofs;
	ofs.x = MapAdjust(mapPos.x, def::FMX, fieldData.GetMapPixelSize().x);
	ofs.y = MapAdjust(mapPos.y, def::FMY, fieldData.GetMapPixelSize().y);

	fieldData.Draw(mapPos + ofs);
	playerMove.Draw(playerPos - ofs);
}

MapMgr::RtnCode MapMgr::Process() {
	MapMovePlayer::RtnCode rtnCode = playerMove.Process(fieldData);
	if( rtnCode != MapMovePlayer::RTN_CONTINUE ) {
		switch( rtnCode ) {
		case MapMovePlayer::RTN_ENCOUNT:
			return RTN_ENCOUNT;
		case MapMovePlayer::RTN_EVENT:
			return RTN_EVENT;
		}
	} else if( CKey::GetInst()->CheckKey(eKEY_MENU) == 1 ) {
		return RTN_MENU;
	}

	return RTN_CONTINUE;
}

//	�}�b�v�O���`�悳��Ȃ��l�ɕ␳���|����
int MapMgr::MapAdjust(int pos, int fieldSize, int mapSize) {
	int ofs = 0;

	if( pos < 0 )
		ofs = -pos;
	if( ( pos + fieldSize ) >= mapSize )
		ofs = mapSize - ( pos + fieldSize );

	return ofs;
}