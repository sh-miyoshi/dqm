#pragma once

#include <string>
#include <vector>

typedef struct {
	std::string mapDefName;
	std::string mapFileName;
	std::string imageFile;
}MapDefineData_t;

class MapDefine {
	std::vector<MapDefineData_t> mapData;

	MapDefine() {}
	~MapDefine() {}
public:
	static MapDefine *GetInst() {
		static MapDefine inst;
		return &inst;
	}

	void LoadData();
	MapDefineData_t GetData(std::string mapDefName);
};