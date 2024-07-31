#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x, y, z;

	// 内積を計算するメンバー関数
	float dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }

	// スカラー倍を行うメンバー関数
	Vector3 scale(float scalar) const { return {x * scalar, y * scalar, z * scalar}; }

	// 演算子オーバーロード
	Vector3 operator+(const Vector3& other) const 
	{ return {x + other.x, y + other.y, z + other.z}; }

	Vector3 operator-(const Vector3& other) const { return {x - other.x, y - other.y, z - other.z}; }

	Vector3 operator/(float scalar) const { return {x / scalar, y / scalar, z / scalar}; }
	Vector3 operator*(float scalar) const { return {x * scalar, y * scalar, z * scalar}; }
	Vector3 operator-(float scalar) const { return {x - scalar, y - scalar, z - scalar}; }
	Vector3 operator+(float scalar) const { return {x + scalar, y + scalar, z + scalar}; }
	// ベクトルの長さの二乗を計算するメンバー関数
	float lengthSquared() const { return x * x + y * y + z * z; }

	// += 演算子のオーバーロード
	Vector3& operator+=(const Vector3& other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}

	void sanitize() {
		if (std::isnan(x))
			x = 0.0f;
		if (std::isnan(y))
			y = 0.0f;
		if (std::isnan(z))
			z = 0.0f;
	}
};

// inlineで関数を定義
inline Vector3 operator*(float scalar, const Vector3& vec) { return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar); }

// 加算
inline Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 v3 = {};
	v3.x = v1.x + v2.x;
	v3.y = v1.y + v2.y;
	v3.z = v1.z + v2.z;
	return v3;
}
// 減算
inline Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 v3 = {};
	v3.x = v1.x - v2.x;
	v3.y = v1.y - v2.y;
	v3.z = v1.z - v2.z;
	return v3;
}
// スカラー
inline Vector3 Multiply(const Vector3& v1, const Vector3& v2) {
	Vector3 v3 = {};
	v3.x = v1.x * v2.x;
	v3.y = v1.y * v2.y;
	v3.z = v1.z * v2.z;
	return v3;
}

// スカラー
inline Vector3 Multiply(float k, const Vector3& v) {
	Vector3 v2 = {};
	v2.x = v.x * k;
	v2.y = v.y * k;
	v2.z = v.z * k;
	return v2;
}
// 内積
inline float Dot(const Vector3& v1, const Vector3& v2) {
	float dot = {};
	dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return dot;
}
// 長さ
inline float Length(const Vector3& v) {
	float len = {};
	len = sqrtf(Dot(v, v));
	return len;
}

// 正規化
inline Vector3 Normalize(const Vector3& v) {
float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
if (length > 0.0f) {
	return Vector3(v.x / length, v.y / length, v.z / length);
} else {
	return Vector3(0.0f, 0.0f, 0.0f); // Return zero vector if length is zero
}
}