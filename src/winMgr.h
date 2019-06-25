#pragma once

#include <string>
#include <vector>
#include <list>

class WinMgr {
	static const int COLUMN_MAX = 20;
	const int space;

	typedef struct {
		bool aliveFlag;
		bool modeMsg;

		int x, y;
		int colNum, rowNum;

		std::string msg;
		std::string outMsg[COLUMN_MAX];
		int nowColumn;

		int fontSize;
		int strColor;
		bool frameFlag;

		std::list<std::string> list;
		bool cursorFlag;
		int cursor;
		bool pageFlag;
		int nowPage;
		int pageMax;
	}WinData_t;

	int active;
	int updateTime;
	std::vector<WinData_t> data;

	bool UpdateChar();
	int Select(int &cursor, unsigned max);

	void DrawScrollMark(int no, int x, int y, int sizeX, int sizeY);
	void DrawFrame(int no);
	void DrawMessage(int no);
	void DrawList(int no);
	void DrawCursor(int no);
	void DrawPage(int no);
public:
	static void GetWinInfo(int &blendParam, int &color, int &frameColor);

	WinMgr(int space = 5);
	~WinMgr();

	int New(int x, int y, int colNum, int rowNum);
	void Delete(int &no);
	void AllDelete() {
		data.clear();
		active = -1;
	}

	int GetActive()const { return active; }

	bool SetActive(int active);
	void SetMsg(std::string msg);
	void SetList(std::list<std::string> list);
	void SetList(const std::string *data, int size);
	void SetListData(bool cursorFlag, bool turnFlag);
	void SetData(int fontSize = -1, int color = -1, bool frameFlag = true);

	void SetPageData(unsigned num, unsigned drawMax);
	void DeletePage(int winNo);
	int GetNowPage()const;
	bool PageProcess();

	void Draw();
	bool UpdateMsg(int speed = 0);
	int GetCursor()const;
	int MoveCursor();
};