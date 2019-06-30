#include "include.h"
#include "itemMgr.h"
#include "csv.h"

void ItemMgr::LoadData() {
	const std::string FILE_PATH = def::TEXT_FILE_PATH + "/itemData.csv";
	CSV csv(FILE_PATH,CSV::MODE_READ);

	for( int i = 0; i < ID_MAX; i++ ) {
		std::string buf[7];
		int num = csv.getData(buf,7);
		ASSERT(num == 7, "itemData.csv maybe broken");

		// Set Item Data
		itemData[i].id = atoi(buf[0].c_str());
		itemData[i].name = buf[1];
		itemData[i].description = buf[2];
		itemData[i].power = atoi(buf[3].c_str());
		itemData[i].isTargetSelect = ( buf[4] == "true" );
		itemData[i].useInFieldOK = ( buf[5] == "true" );
		itemData[i].useInBattleOK = ( buf[6] == "true" );
	}

	isLoaded = true;
}

Item_t ItemMgr::GetInfo(int id) {
	ASSERT(isLoaded,"ItemData‚ª‚Ü‚¾ƒ[ƒh‚³‚ê‚Ä‚Ü‚¹‚ñ");
	if( !(0 <= id && id < ID_MAX) ) {
		AppLogger::Warn(ToString("ID(%d) of ItemData is out of range",id));
		return itemData[ID_NULL];
	}
	return itemData[id];
}
