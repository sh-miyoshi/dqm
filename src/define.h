#pragma once

#include <string>

namespace def{
	const int FMX = 640;
	const int FMY = 480;
	const int CHAR_IMAGE_RECT_SIZE = 32;
	const int FPS = 60;

	enum Muki{
		eMUKI_UP,
		eMUKI_RIGHT,
		eMUKI_DOWN,
		eMUKI_LEFT,

		eMUKI_MAX
	};

	const std::string IMAGE_FILE_PATH = "data/images/";
	const std::string MAP_DATA_FILE_PATH = "data/map/";
	const std::string TEXT_FILE_PATH = "data/text/";
	const std::string CHAR_IMAGE_PATH = IMAGE_FILE_PATH + "モンスター/";
//	const std::string BGM_FILE_PATH = "data/bgm/";// TODO(Add BGM Path)
}

// 色の定義
#define WHITE		(0xFFFFFF)
#define BLACK		(0x000000)
#define YELLOW		(0xFFFF00)
#define RED			(0xFF0000)
#define LIGHTGREEN	(0x00FF60)