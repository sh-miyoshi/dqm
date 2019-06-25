#pragma once

#include <windows.h>
#include <string>
#include <vector>

// FMFファイルヘッダ (20 bytes)
typedef struct tag_FMFHeader{
	DWORD dwIdentifier;// ファイル識別子 'FMF_'
	DWORD dwSize;      // ヘッダを除いたデータサイズ
	DWORD dwWidth;     // マップの横幅
	DWORD dwHeight;    // マップの高さ
	BYTE  byChipWidth; // マップチップ1つの幅(pixel)
	BYTE  byChipHeight;// マップチップ１つの高さ(pixel)
	BYTE  byLayerCount;// レイヤーの数
	BYTE  byBitCount;  // レイヤデータのビットカウント
}FMF_HEADER;

class FmfFile{
	FMF_HEADER fmfHeader;// Fileメタ情報
	std::vector<BYTE> data;// レイヤーデータ
public:
	FmfFile();
	~FmfFile();

	void LoadData(std::string fileName);

	FMF_HEADER GetInfo();
	BYTE GetValue(BYTE layer, DWORD x, DWORD y);
};