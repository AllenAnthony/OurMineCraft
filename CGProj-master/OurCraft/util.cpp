#include "util.h"

Vector3 ROUND(Vector3f vec) {
	int x = round(vec.x);
	int y = round(vec.y);
	int z = round(vec.z);

	return Vector3(x, y, z);
}

//归一化
Vector3f NORMALIZE(Vector3f vec) {
	Float length = pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2);

	return Vector3f(vec.x / length, vec.y / length, vec.z / length);
}

//区域化
Vector3 SECTORIZE(Vector3f position) {
	Vector3 tmp = ROUND(position);
	int x = tmp.x / SECTOR_SIZE;
	int z = tmp.z / SECTOR_SIZE;
	return Vector3(x, 0, z);	//都是整数
}


Float distance(Vector3f p1, Vector3f p2) {
	return pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2);
}

//判断一个位置是否在面前的空间内
bool atFront(Vector3f target, Vector3f source, Vector3f direction) {
	if ((target - source) * direction >= 0 || distance(target, source) <= 2) return true;
	return false;
}