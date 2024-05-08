#include "MapChipField.h"

#include <map>

#include <fstream>
#include <sstream>

#include <cassert>

namespace {

std::map<std::string, MapChipType> MapChipTable = {
	//0のときは空白にする
    {"0", MapChipType::kBlank},
	//1のときはブロック
    {"1", MapChipType::kBlock},
};

}

void MapChipField::ResetMapChipData() {

	// mapChipDataをリセット
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {

	// MapChipData is Reset
	ResetMapChipData();

	// File Open
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// MapChipCSV
	std::stringstream mapChipCSV;
	// ファイルの内容を文字列ストリームにコピー
	mapChipCSV << file.rdbuf();
	// File is Close
	file.close();

	// CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) {

		// line作成
		std::string line;

		// lineに文字列ストリーム
		getline(mapChipCSV, line);

		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			std::string word;
			getline(line_stream, word, ',');

			if (MapChipTable.contains(word)) {
				mapChipData_.data[i][j] = MapChipTable[word];
			}
		}
	}
}
MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {

	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}

	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex];
}
Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { 

	return Vector3(
		kBlockWidth * xIndex,
		kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 
		0
	); 
}
//Blockの数を取得する関数
uint32_t MapChipField::GetNumBlockHorizontal() { return kNumBlockHorizontal; }
uint32_t MapChipField::GetNumBlockVirtical() { return kNumBlockVirtical; }