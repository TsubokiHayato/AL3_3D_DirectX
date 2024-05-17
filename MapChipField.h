#pragma once

#include <Vector3.h>
#include <string>
#include <vector>

enum class MapChipType {
	kBlank,
	kBlock,
};

struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {

public:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;

	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 20;

	MapChipData mapChipData_;

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical();
	uint32_t GetNumBlockHorizontal();

	

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	struct Rect {
	
		float left;
		float right;
		float top;
		float bottom;
	
	};

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
		Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);
		Rect rect;

		rect.left = center.x - kBlockWidth / 2.0f;
		rect.right = center.x + kBlockWidth / 2.0f;

		rect.bottom = center.y - kBlockHeight / 2.0f;
		rect.top = center.y + kBlockHeight / 2.0f;

		return rect;
	
	}
};
