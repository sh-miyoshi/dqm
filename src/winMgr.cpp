#include "include.h"
#include "winMgr.h"

namespace {
	const int BLEND_PARAM = 200;
	const int DEFAULT_WINDOW_FONT_SIZE = 18;
	const int COLUMN_SPACE = 5;
	const int MESSAGE_START_SPACE_X = 7;
	const int LIST_CURSOR_SIZE_SPACE_X = 13;
	const int WIN_COLOR = BLACK;
	const int FRAME_AND_STRING_COLOR = WHITE;
};

void WinMgr::GetWinInfo(int &blendParam, int &color, int &frameColor) {
	blendParam = BLEND_PARAM;
	color = WIN_COLOR;
	frameColor = FRAME_AND_STRING_COLOR;
}

WinMgr::WinMgr(int space):space(space), active(-1), updateTime(0) {
}

WinMgr::~WinMgr() {
}

int WinMgr::New(int x, int y, int colNum, int rowNum) {
	// ���Ƀ��b�Z�[�W�̂Ȃ����b�Z�[�W�E�B���h�E�Ƃ��č쐬
	WinData_t temp;
	temp.aliveFlag = true;
	temp.modeMsg = true;
	temp.x = x;
	temp.y = y;
	temp.colNum = colNum;
	temp.rowNum = rowNum;
	temp.fontSize = DEFAULT_WINDOW_FONT_SIZE;
	temp.nowColumn = 0;
	temp.strColor = FRAME_AND_STRING_COLOR;
	temp.frameFlag = true;
	temp.pageFlag = false;
	temp.nowPage = 0;
	temp.pageMax = 0;
	temp.cursor = 0;
	data.push_back(temp);
	active = data.size() - 1;
	return active;
}

void WinMgr::Delete(int &no) {
	if( no != -1 ) {
		std::string err_msg = "WinMgr::Delete illegal winNo no: " + ToString<int>(no) + " winNum: " + ToString<unsigned>(data.size());
		ASSERT(no < ( int ) data.size(), err_msg);
		if( data[no].aliveFlag ) {
			data[no].aliveFlag = false;// ���̃E�B���h�E���E��
			if( no == active )
				active = data.size() - 1;
			no = -1;
		}
	}
}

bool WinMgr::SetActive(int active) {
	if( active == -1 || !data[active].aliveFlag )
		return false;
	this->active = active;
	return true;
}

void WinMgr::SetMsg(std::string msg) {
	if( active != -1 && data[active].aliveFlag ) {
		// �E�B���h�E���Ɏ��܂�悤��/��}��
		int num = 0;
		while( !msg.empty() ) {
			int s = GetStringSize(msg[0]);
			if( s == 1 && ( msg[0] == '/' || msg[0] == '*' ) )// ���ꕶ���Ȃ��
				num = 0;
			else
				num += s;
			if( num > data[active].rowNum * 2 ) {
				data[active].msg += "/";
				num = 0;
			}
			for( int i = 0; i < s; i++ ) {
				data[active].msg += msg[0];
				msg.erase(msg.begin());
			}
		}
		data[active].modeMsg = true;
		data[active].nowColumn = 0;
		for( int i = 0; i < COLUMN_MAX; i++ )
			data[active].outMsg[i].clear();
	}
}

void WinMgr::SetList(std::list<std::string> list) {
	if( active != -1 && data[active].aliveFlag ) {
		data[active].modeMsg = false;
		data[active].list = list;
		data[active].cursor = 0;
		data[active].cursorFlag = true;
	}
}

void WinMgr::SetList(const std::string *data, int size) {
	std::list<std::string> lstr;
	for( int i = 0; i < size; i++ )
		lstr.push_back(data[i]);
	SetList(lstr);
}

void WinMgr::SetListData(bool cursorFlag, bool turnFlag) {
	if( active != -1 && data[active].aliveFlag && !data[active].modeMsg ) {
		data[active].cursorFlag = cursorFlag;
		if( turnFlag ) {// �f�[�^�T�C�Y���E�B���h�E�T�C�Y�𒴂��Ă����ꍇ�܂�Ԃ�
			using namespace std;
			list<string> lstr;
			for( list<string>::iterator it = data[active].list.begin(); it != data[active].list.end(); it++ ) {
				string d = *it, ret;
				for( int i = 0; !d.empty();) {
					int size = GetStringSize(d[0]);
					for( int j = 0; j < size; j++ ) {
						ret += d[0];
						d.erase(d.begin());
						i++;
					}

					if( i > (data[active].rowNum - 1) * 2 ) {
						lstr.push_back(ret);
						ret.clear();
						i = 0;
					}
				}
				if( !ret.empty() )
					lstr.push_back(ret);
			}

			// �f�[�^���X�V
			data[active].list = lstr;
		}
	}
}

void WinMgr::SetData(int fontSize, int color, bool frameFlag) {
	if( active != -1 && data[active].aliveFlag ) {
		if( fontSize == -1 )
			fontSize = DEFAULT_WINDOW_FONT_SIZE;
		if( color == -1 )
			color = FRAME_AND_STRING_COLOR;
		data[active].fontSize = fontSize;
		data[active].strColor = color;
		data[active].frameFlag = frameFlag;
	}
}

void WinMgr::SetPageData(unsigned num, unsigned drawMax) {
	if( active != -1 && data[active].aliveFlag ) {
		data[active].pageFlag = true;
		data[active].nowPage = 0;
		if( drawMax == 1 && num > 0 )
			data[active].pageMax = num - 1;
		else
			data[active].pageMax = num / ( drawMax + 1 );
	}
}

void WinMgr::DeletePage(int winNo) {
	if( winNo != -1 && data[winNo].aliveFlag&&data[winNo].pageFlag )
		data[winNo].pageFlag = false;
}

int WinMgr::GetNowPage()const {
	if( active != -1 && data[active].aliveFlag&&data[active].pageFlag )
		return data[active].nowPage;
	return -1;
}

bool WinMgr::PageProcess() {
	if( active != -1 && data[active].aliveFlag&&data[active].pageFlag ) {
		if( CKey::GetInst()->CheckKey(eKEY_LEFT) == 1 && data[active].nowPage != 0 ) {
			data[active].nowPage--;
			return true;
		}
		if( CKey::GetInst()->CheckKey(eKEY_RIGHT) == 1 && data[active].nowPage < data[active].pageMax ) {
			data[active].nowPage++;
			return true;
		}
	}
	return false;
}

void WinMgr::Draw() {
	for( unsigned i = 0; i < data.size(); i++ ) {
		if( data[i].aliveFlag ) {
			if( data[i].frameFlag )
				DrawFrame(i);
			if( data[i].modeMsg )
				DrawMessage(i);
			else {
				DrawList(i);
				if( !data[i].list.empty() && data[i].cursorFlag )
					DrawCursor(i);
			}
		}
	}
}

bool WinMgr::UpdateMsg(int speed) {
	int state = 0;// �\���𑱂���
	if( data[active].msg.empty() )// �I��
		state = 1;
	else if( data[active].nowColumn >= data[active].colNum )
		state = 2;

	if( state == 0 ) {
		if( speed == 0 ) {// TODO(�S�\���ŃX�N���[���������ɂ̓G���[��Ԃ� or �K�؂ȏ���)
			data[active].nowColumn = Split(data[active].outMsg, COLUMN_MAX, data[active].msg, "/");
			data[active].nowColumn--;
			data[active].msg.clear();
		} else if( updateTime%speed == 0 ) {
			UpdateChar();
		}
	}
	updateTime++;

	// �L�[���͂���������
	if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 || CKey::GetInst()->CheckKey(eKEY_CANCEL) == 1 ) {
		switch( state ) {
		case 0:// �X�N���[������Ȃ����肷�ׂĕ\��
			while( UpdateChar() );
			break;
		case 1:// �I��
			updateTime = 0;
			return true;
		case 2:
			data[active].nowColumn = 0;
			for( int i = 0; i < COLUMN_MAX; i++ )
				data[active].outMsg[i].clear();
			break;
		}
	}
	return false;
}

int WinMgr::GetCursor()const {
	ASSERT(active != -1, "WinMgr::GetCursor no active window");
	return data[active].cursor;
}

int WinMgr::MoveCursor() {
	ASSERT(active != -1, "WinMgr::MoveCursor no active window");
	if( !data[active].modeMsg && !data[active].list.empty() )
		return Select(data[active].cursor, data[active].list.size());
	return -1;
}

bool WinMgr::UpdateChar() {
	if( data[active].msg.empty() )
		return false;
	else if( data[active].nowColumn >= data[active].colNum )
		return false;
	else {
		char c = data[active].msg[0];
		data[active].msg.erase(data[active].msg.begin());
		if( c == '/' )
			data[active].nowColumn++;
		else if( c == '*' )
			data[active].nowColumn = data[active].colNum;
		else {
			int s = GetStringSize(c);
			data[active].outMsg[data[active].nowColumn] += c;
			if( s == 2 ) {
				c = data[active].msg[0];
				data[active].msg.erase(data[active].msg.begin());
				data[active].outMsg[data[active].nowColumn] += c;
			}
		}
		return true;
	}
}

int WinMgr::Select(int &cursor, unsigned max) {
	if( cursor != 0 && CKey::GetInst()->CheckKey(eKEY_UP) == 1 )
		cursor--;
	if( cursor != ( max - 1 ) && CKey::GetInst()->CheckKey(eKEY_DOWN) == 1 )
		cursor++;
	if( CKey::GetInst()->CheckKey(eKEY_ENTER) == 1 )
		return cursor;

	return -1;
}

void WinMgr::DrawScrollMark(int no, int x, int y, int sizeX, int sizeY) {
	if( data[no].nowColumn >= data[no].colNum ) {
		const int INTERVAL = 96;
		if( updateTime%INTERVAL < ( INTERVAL / 2 ) ) {
			int tx = x + ( sizeX / 2 );
			int ty = y + sizeY;
			DrawTriangle(tx - 12, ty - 5, tx + 12, ty - 5, tx, ty + 5, FRAME_AND_STRING_COLOR, TRUE);
		}
	}
}

void WinMgr::DrawFrame(int no) {
	// �t���[���`��
	int x = data[no].x;
	int y = data[no].y;
	int sizeX = ( data[no].rowNum + 1 )*data[no].fontSize + space * 2 + MESSAGE_START_SPACE_X * 2;
	if( !data[no].modeMsg && !data[no].cursorFlag )// ���X�g���[�h�ŃJ�[�\���`�悵�Ȃ����
		sizeX -= LIST_CURSOR_SIZE_SPACE_X;
	int sizeY = data[no].colNum*( data[no].fontSize + COLUMN_SPACE ) + space * 2 + COLUMN_SPACE;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BLEND_PARAM);// �A���t�@�u�����h
	DrawBox(x + 1, y + 1, x + sizeX - 1, y + sizeY - 1, WIN_COLOR, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);// �m�[�}���u�����h

	// �t���[��
	DrawBox(x, y, x + 1, y + sizeY, data[no].strColor, TRUE);
	DrawBox(x, y, x + sizeX, y + 1, data[no].strColor, TRUE);
	DrawBox(x, y + sizeY - 1, x + sizeX, y + sizeY, data[no].strColor, TRUE);
	DrawBox(x + sizeX - 1, y, x + sizeX, y + sizeY, data[no].strColor, TRUE);

	// �A�N�e�B�u�ȃE�B���h�E�ŃX�N���[���K�v���̂ݕ\��
	if( no == active )
		DrawScrollMark(no, x, y, sizeX, sizeY);
}

void WinMgr::DrawMessage(int no) {
	// ���b�Z�[�W�`��
	int px = data[no].x + space + MESSAGE_START_SPACE_X;
	int py = data[no].y + space + 5;
	int fsize = data[no].fontSize;
	fsize += COLUMN_SPACE;// �X�y�[�X���󂯂�

	SetFontSize(data[no].fontSize);
	for( int i = 0; i <= data[no].nowColumn; i++ )
		DrawString(px, py + i*fsize, data[no].outMsg[i].c_str(), data[no].strColor);
}

void WinMgr::DrawList(int no) {
	int topx = data[no].x + space + 5;
	if( data[no].cursorFlag )
		topx += LIST_CURSOR_SIZE_SPACE_X;
	int topy = data[no].y + space + COLUMN_SPACE;

	int i = 0;
	SetFontSize(data[no].fontSize);
	for( std::list<std::string>::iterator it = data[no].list.begin(); it != data[no].list.end(); it++, i++ )
		DrawString(topx, topy + i*( data[no].fontSize + COLUMN_SPACE ), it->c_str(), data[no].strColor);

	if( data[no].pageFlag )
		DrawPage(no);
}

void WinMgr::DrawCursor(int no) {
	int x = data[no].x + space + 5;
	int y = data[no].y + data[no].cursor*( data[no].fontSize + COLUMN_SPACE ) + data[no].fontSize / 2 + space + COLUMN_SPACE;
	int sizeY = data[no].fontSize / 2;

	DrawTriangle(x, y - sizeY, x, y + sizeY, x + 10, y, data[no].strColor, TRUE);
}

void WinMgr::DrawPage(int no) {
	int sizeX = ( data[no].rowNum + 1 )*data[no].fontSize + space * 2 + MESSAGE_START_SPACE_X * 2;
	int sizeY = data[no].colNum*( data[no].fontSize + COLUMN_SPACE ) + space * 2 + COLUMN_SPACE;
	int x = data[no].x + sizeX - 40;
	int y = data[no].y + sizeY - 25;

	DrawFormatString(x, y, data[no].strColor, "%d/%d", data[no].nowPage + 1, data[no].pageMax + 1);
}
