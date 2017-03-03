#pragma once

#include "util.h"
#include "model.h"
#include "block.h"

typedef struct {
	int z;	//z方向
	int x;	//x方向
	int y;	//y方向
} Motion;

typedef struct {
	Float PHI;
	Float THETA;
} Rotation;

class Man {
public:
	Block_type current_type;

	bool exclusive;

	bool flying;
	Motion motion;
	Rotation rotation;
	Float dy;
	Vector3f position;
	Float speed;
	
	Float height, width;

	int man_height;

	Model model;

public:
	Man();
	void update();
	void set_3d();
	void set_2d();
	Vector3f get_sight_vector();
	void set_current_block(Block_type);  //设置当前方块类型
private:
	Vector3f get_motion_vector();
	Vector3f collide(Vector3f);
};