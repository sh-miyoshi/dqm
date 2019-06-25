#include <fstream>
#include "fmfFile.h"

FmfFile::FmfFile() {
}

FmfFile::~FmfFile() {
}

void FmfFile::LoadData(std::string fileName) {
	std::ifstream ifs(fileName, std::ios::binary);
	ifs.read(( char * ) &fmfHeader, sizeof(FMF_HEADER));

	BYTE *pLayer = new BYTE[fmfHeader.dwSize];
	ifs.read(( char * ) pLayer, fmfHeader.dwSize);
	for( DWORD i = 0; i < fmfHeader.dwSize; i++ ) {
		data.push_back(pLayer[i]);
	}
	delete[] pLayer;
}

FMF_HEADER FmfFile::GetInfo() {
	return fmfHeader;
}

BYTE FmfFile::GetValue(BYTE layer, DWORD x, DWORD y) {
	// レイヤーが存在しないか範囲外アクセスなら
	if( layer >= fmfHeader.byLayerCount || x >= fmfHeader.dwWidth || y >= fmfHeader.dwHeight )
		return -1;
	int layerSize = fmfHeader.dwSize / fmfHeader.byLayerCount;
	return data[layerSize * layer + fmfHeader.dwWidth * y + x];
}
