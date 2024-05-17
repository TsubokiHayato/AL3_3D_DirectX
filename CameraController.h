#include "ViewProjection.h"

    // 前方宣言
    class Player;

	struct Rect {
	    float left = 0.0f;
	    float right = 1.0f;
	    float bottom = 0.0f;
	    float top = 1.0f;
	};

class CameraController {
public:
	void Initialize(ViewProjection* viewProjection_);
	void Update();
	void Reset();

	// Player クラスのポインタを受け取るための関数
	void SetTarget(Player* target) { target_ = target; }

	void SetMovableArea(Rect area) { movableArea_ = area; }


private:
	// Player クラスのポインタ
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -20.0f};
	ViewProjection* viewProjection = {};

	Rect movableArea_ = {0, 100, 0, 100};
	//カメラの目標座標
	Vector3 goalPos = {};
	//座標補完割合
	static inline const float kInterpolationRate = 0.4f;
	
	//速度制限掛率
	static inline const float kVelocityBias = 0.8f;

	//margin
	static inline const Rect margin = {};
};