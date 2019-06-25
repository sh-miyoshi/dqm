#include "include.h"
#include "mapMoveChar.h"

namespace {
	const int PATTERN_MAX = 4;
	const int ANIM_TABLE[PATTERN_MAX] = { 1,0,1,2 };
	const int PLAYER_WALK_SPEED = 2;
};

MapMoveChar::MapMoveChar(std::string fileName, int xnum, int ynum)
	:xnum(xnum), ynum(ynum) {
	image = new int[xnum*ynum];
	LoadDivGraphWithCheckError(image, fileName, "MapMovePlayer::MapMovePlayer", xnum, ynum, def::CHAR_IMAGE_RECT_SIZE, def::CHAR_IMAGE_RECT_SIZE);
}

MapMoveChar::~MapMoveChar() {
	for( int i = 0; i < xnum*ynum; i++ )
		DeleteGraph(image[i]);
	delete[] image;
}

void MapMoveChar::Draw(CPoint<int> pos, int muki, int animOfs, bool isWalk) {
	ASSERT(muki < def::eMUKI_MAX, "�����Ȍ���: " + ToString<int>(muki));
	animOfs %= PATTERN_MAX;
	int no = isWalk ? ANIM_TABLE[animOfs] : 1;
	DrawGraph(pos.x, pos.y, image[muki*xnum + no], TRUE);
}

//	�}�b�v�O�ɏo�Ă��Ȃ����`�F�b�N(pos = �}�b�v�O: false, �}�b�v��: true)
bool MapMovePlayer::CheckMapSize(int pos, int charSize, int mapSize) {
	if( pos < 0 )
		return false;
	if( ( pos + charSize ) > mapSize )
		return false;

	return true;
}

bool MapMovePlayer::CheckMove(CPoint<int> pos, int muki, MapField fieldData) {
	CPoint<int> tmp = MoveMuki(pos, CPoint<int>(def::CHAR_IMAGE_RECT_SIZE, def::CHAR_IMAGE_RECT_SIZE), muki);// ���Ɉ��������

	CPoint<int> mapSize = fieldData.GetMapPixelSize();
	if( !CheckMapSize(tmp.x, def::CHAR_IMAGE_RECT_SIZE, mapSize.x) )// x����
		return false;
	if( !CheckMapSize(tmp.y, def::CHAR_IMAGE_RECT_SIZE, mapSize.y) )// y����
		return false;

	if( fieldData.HitJudge(tmp) )// �����ɓ������Ă�����
		return false;

	return true;// ������
}

CPoint<int> MapMovePlayer::MoveMuki(CPoint<int> pos, CPoint<int> spd, int muki) {
	CPoint<int> result = pos;
	switch( muki ) {
	case def::eMUKI_UP:   result.y -= spd.y; break;
	case def::eMUKI_DOWN: result.y += spd.y; break;
	case def::eMUKI_RIGHT:result.x += spd.x; break;
	case def::eMUKI_LEFT: result.x -= spd.x; break;
	}
	return result;
}

MapMovePlayer::MapMovePlayer()
	:MapMoveChar(def::IMAGE_FILE_PATH + "mapPlayer.png", 3, 4), count(0),
	muki(def::eMUKI_DOWN), isMove(false) {
	// debug(�����L�����ʒu�̓C�x���g�ԍ��Ō��߂�ׂ�)
	pos = CPoint<int>(40 * def::CHAR_IMAGE_RECT_SIZE, 38 * def::CHAR_IMAGE_RECT_SIZE);
}

MapMovePlayer::~MapMovePlayer() {
}

void MapMovePlayer::Draw(CPoint<int> ofsPlayerPos) {
	MapMoveChar::Draw(ofsPlayerPos, muki, count >> 4, isMove);
}

MapMovePlayer::RtnCode MapMovePlayer::Process(MapField fieldData) {
	count++;

	CPoint<int> tpos = this->pos;

	if( pos.x%def::CHAR_IMAGE_RECT_SIZE == 0 && pos.y%def::CHAR_IMAGE_RECT_SIZE == 0 ) {
		if( isMove ) {
			isMove = false;
			// TODO(event����)
		}
	}

	if( !isMove ) {
		if( CKey::GetInst()->CheckKey(eKEY_UP) > 0 ) {
			muki = def::eMUKI_UP;
			isMove = true;
		} else if( CKey::GetInst()->CheckKey(eKEY_DOWN) > 0 ) {
			muki = def::eMUKI_DOWN;
			isMove = true;
		} else if( CKey::GetInst()->CheckKey(eKEY_LEFT) > 0 ) {
			muki = def::eMUKI_LEFT;
			isMove = true;
		} else if( CKey::GetInst()->CheckKey(eKEY_RIGHT) > 0 ) {
			muki = def::eMUKI_RIGHT;
			isMove = true;
		}

		if( isMove )
			if( !CheckMove(tpos, muki, fieldData) )
				isMove = false;
	}

	if( isMove )
		this->pos = MoveMuki(tpos, CPoint<int>(PLAYER_WALK_SPEED, PLAYER_WALK_SPEED), muki);

	// �L�[���͏���(���j���[,�C�x���g)
	if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 ) {
		// TODO(�G�x���g�����݂����Ƃ��̂݃C�x���g�����Ɉڍs)
		return RTN_EVENT;
	} else if( CheckHitKey(KEY_INPUT_B) == 1 )// debug(�퓬�ڍs�̓C�x���g�Ŕ���)
		return RTN_ENCOUNT;

	return RTN_CONTINUE;
}
