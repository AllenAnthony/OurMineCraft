#include "block.h"

Block::Block(Block_type type) {
	this->type = type;
	if (type == GRASS_BLOCK) {
		this->top = GRASS_TOP;
		this->around = GRASS_AROUND;
		this->bottom = GRASS_BOTTOM;
	}
	else if (type == SAND_BLOCK) {
		this->top = SAND;
		this->around = SAND;
		this->bottom = SAND;
	}
	else if (type == BRICK_BLOCK) {
		this->top = BRICK;
		this->around = BRICK;
		this->bottom = BRICK;
	}
	else if (type == STONE_BLOCK) {
		this->top = STONE;
		this->around = STONE;
		this->bottom = STONE;
	}
	else {
		std::cout << "unknown block type:" << type << std::endl;
	}
}




/*
* 根据方块各个面的材料定义方块
*/
Block::Block(Material top, Material around, Material bottom) {
	this->top = top;
	this->around = around;
	this->bottom = bottom;
	this->type = DEFAULT_BLOCK;
}

/*
* 用于方块的复制
*/
Block::Block(const Block& b) {
	top = b.top;
	around = b.around;
	bottom = b.bottom;
	type = b.type;
	exposed = b.exposed;
}

void Block::drawFace(int face) {
	if (face == TOP) {
		glBegin(GL_POLYGON);
		glTexCoord2d(MATERIAL_POSITION[top][0], MATERIAL_POSITION[top][2]); glVertex3f(0.0f, 1.0f, 0.0f);
		glTexCoord2d(MATERIAL_POSITION[top][1], MATERIAL_POSITION[top][2]); glVertex3f(0.0f, 1.0f, 1.0f);
		glTexCoord2d(MATERIAL_POSITION[top][1], MATERIAL_POSITION[top][3]); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2d(MATERIAL_POSITION[top][0], MATERIAL_POSITION[top][3]); glVertex3f(1.0f, 1.0f, 0.0f);
		glEnd();
	}
	else if (face == BOTTOM) {
		glBegin(GL_POLYGON);
		glTexCoord2d(MATERIAL_POSITION[bottom][0], MATERIAL_POSITION[bottom][2]); glVertex3f(0.0, 0.0, 1.0);
		glTexCoord2d(MATERIAL_POSITION[bottom][1], MATERIAL_POSITION[bottom][2]); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2d(MATERIAL_POSITION[bottom][1], MATERIAL_POSITION[bottom][3]); glVertex3f(1.0, 0.0, 0.0);
		glTexCoord2d(MATERIAL_POSITION[bottom][0], MATERIAL_POSITION[bottom][3]); glVertex3f(1.0, 0.0, 1.0);
		glEnd();
	}
	else if(face == LEFT){
		glBegin(GL_POLYGON);
		glTexCoord2d(MATERIAL_POSITION[around][0], MATERIAL_POSITION[around][2]); glVertex3f(0.0, 1.0, 1.0);
		glTexCoord2d(MATERIAL_POSITION[around][1], MATERIAL_POSITION[around][2]); glVertex3f(0.0, 1.0, 0.0);
		glTexCoord2d(MATERIAL_POSITION[around][1], MATERIAL_POSITION[around][3]); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2d(MATERIAL_POSITION[around][0], MATERIAL_POSITION[around][3]); glVertex3f(0.0, 0.0, 1.0);
		glEnd();
	}
	else if (face == RIGHT) {
		glBegin(GL_POLYGON);
		glTexCoord2d(MATERIAL_POSITION[around][0], MATERIAL_POSITION[around][2]); glVertex3f(1.0, 1.0, 0.0);
		glTexCoord2d(MATERIAL_POSITION[around][1], MATERIAL_POSITION[around][2]); glVertex3f(1.0, 1.0, 1.0);
		glTexCoord2d(MATERIAL_POSITION[around][1], MATERIAL_POSITION[around][3]); glVertex3f(1.0, 0.0, 1.0);
		glTexCoord2d(MATERIAL_POSITION[around][0], MATERIAL_POSITION[around][3]); glVertex3f(1.0, 0.0, 0.0);
		glEnd();
	}
	else if (face == FRONT) {
		glBegin(GL_POLYGON);
		glTexCoord2d(MATERIAL_POSITION[around][0], MATERIAL_POSITION[around][2]); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2d(MATERIAL_POSITION[around][1], MATERIAL_POSITION[around][2]); glVertex3f(0.0f, 1.0f, 1.0f);
		glTexCoord2d(MATERIAL_POSITION[around][1], MATERIAL_POSITION[around][3]); glVertex3f(0.0f, 0.0f, 1.0f);
		glTexCoord2d(MATERIAL_POSITION[around][0], MATERIAL_POSITION[around][3]); glVertex3f(1.0f, 0.0f, 1.0f);
		glEnd();
	}
	else if (face == BACK) {
		glBegin(GL_POLYGON);
		glTexCoord2d(MATERIAL_POSITION[around][0], MATERIAL_POSITION[around][2]); glVertex3f(0.0f, 1.0f, 0.0f);
		glTexCoord2d(MATERIAL_POSITION[around][1], MATERIAL_POSITION[around][2]); glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord2d(MATERIAL_POSITION[around][1], MATERIAL_POSITION[around][3]); glVertex3f(1.0f, 0.0f, 0.0f);
		glTexCoord2d(MATERIAL_POSITION[around][0], MATERIAL_POSITION[around][3]); glVertex3f(0.0f, 0.0f, 0.0f);
		glEnd();
	}
	else {
		std::cout << "Unknown face type:" << face << std::endl;
	}
}

/*
* 绘制方块
*/
void Block::draw() {

	//glPushMatrix();
	glTranslatef(-0.5, -0.5, -0.5);

	if (!exposed) return;

	if (exposed & TOP) {
		//top
		drawFace(TOP);
	}

	if (exposed & BOTTOM) {
		//bottom
		drawFace(BOTTOM);
	}

	if (exposed & LEFT) {
		//left
		drawFace(LEFT);
	}

	if (exposed & RIGHT) {
		//right
		drawFace(RIGHT);
	}

	if (exposed & FRONT) {
		//front
		drawFace(FRONT);
	}

	if (exposed & BACK) {
		//back
		drawFace(BACK);
	}
	//glPopMatrix();
}