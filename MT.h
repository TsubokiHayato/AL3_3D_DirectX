#pragma once
#include <Vector3.h>
#include <Matrix4x4.h>

 inline Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
	    (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0]),
	    (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1]),
	    (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]),
	};

	return result;
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
	float len = Length(v);
	Vector3 v2 = {};
	v2.x = v.x / len;
	v2.y = v.y / len;
	v2.z = v.z / len;

	return v2;
}