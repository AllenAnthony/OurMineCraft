#pragma once

#include "util.h"

#define MATERIAL_NUM 7

//贴图材料类型，每一个对应一种贴图， 第0个为默认无贴图
enum Material {
	DEFAULT, GRASS_TOP, GRASS_AROUND, GRASS_BOTTOM, SAND, BRICK, STONE
};

//方块类型，依次为无、草、沙、砖、石
enum Block_type {
	DEFAULT_BLOCK, GRASS_BLOCK, SAND_BLOCK, BRICK_BLOCK, STONE_BLOCK
};

//Material 类型：
//	DEFAULT, GRASS_TOP, GRASS_AROUND, GRASS_BOTTOM, SAND, BRICK, STONE
//

const GLfloat MATERIAL_POSITION[MATERIAL_NUM][4] = {
	{ 0.75f, 1.0f, 0.75f, 1.0f }, //DEFAULT
	{ 0.25f, 0.5f, 0.75f, 1.0f }, //GRASS_TOP
	{ 0.0f, 0.25f, 0.75f, 1.0f }, //GRASS_AROUND
	{ 0.0f, 0.25f, 0.5f, 0.75f }, //GRASS_BOTTOM
	{ 0.25f, 0.5f, 0.5f, 0.75f }, //SAND
	{ 0.5f, 0.75f, 0.75f, 1.0f }, //BRICK
	{ 0.5f, 0.75f, 0.5f, 0.75f }  //STONE
};

//方块类定义
class Block {
public:
	Block_type type;
	Material top;
	Material around;
	Material bottom;

	unsigned char exposed;

	Block() {
		exposed = TOP | BOTTOM | LEFT | RIGHT | FRONT | BACK;
	}
	Block(Block_type type);
	Block(Material top, Material around, Material bottom);
	Block(const Block& b);

	void draw();
	void drawFace(int face);
};

