#include "include.h"
#include "mapData.h"

void MapDefine::LoadData() {
	MapDefineData_t data = {
		"マルタ", // mapDefName
		"DQ_maruta.fmf", // mapFileName
		"DQ_maruta.png", // imageFile
	};
	mapData.push_back(data);

	data = {
		"井戸", // mapDefName
		"DQ_maruta_ido.fmf", // mapFileName
		"DQ_maruta_ido.png", // imageFile
	};
	mapData.push_back(data);

	data = {
		"オアシス", // mapDefName
		"DQ_oasis.fmf", // mapFileName
		"DQ_oasis.png", // imageFile
	};
	mapData.push_back(data);

	data = {
		"祠", // mapDefName
		"DQ_shrine.fmf", // mapFileName
		"DQ_shrine.png", // imageFile
	};
	mapData.push_back(data);
}

MapDefineData_t MapDefine::GetData(std::string mapDefName) {
	for( auto data : mapData ) {
		if( data.mapDefName == mapDefName ) {
			return data;
		}
	}
	// そんなマップは存在しない
	ASSERT(0, std::string("No such map: ") + mapDefName);
	return MapDefineData_t();
}
