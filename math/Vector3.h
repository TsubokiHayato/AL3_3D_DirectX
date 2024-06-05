#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	 // ベクトルの加算
	Vector3 operator+(const Vector3& rhs) const { return {x + rhs.x, y + rhs.y, z + rhs.z}; }

	// ベクトルの減算
	Vector3 operator-(const Vector3& rhs) const { return {x - rhs.x, y - rhs.y, z - rhs.z}; }

	// ベクトルの乗算（要素ごとの乗算）
	Vector3 operator*(const Vector3& rhs) const { return {x * rhs.x, y * rhs.y, z * rhs.z}; }

	// ベクトルの除算（要素ごとの除算）
	Vector3 operator/(const Vector3& rhs) const { return {x / rhs.x, y / rhs.y, z / rhs.z}; }

	// ベクトルの加算代入
	Vector3& operator+=(const Vector3& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	// ベクトルの減算代入
	Vector3& operator-=(const Vector3& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	// ベクトルの乗算代入
	Vector3& operator*=(const Vector3& rhs) {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	// ベクトルの除算代入
	Vector3& operator/=(const Vector3& rhs) {
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}
};
