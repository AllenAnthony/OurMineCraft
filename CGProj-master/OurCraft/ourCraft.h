#pragma once

#include "util.h"
#include "block.h"
#include "OBJLoader.hpp"
#include "soli/SOIL.h"
#include "objloader.hpp"
#include "ObjUtils.hpp"

#define GLOBAL_LIGHT 0
#define TEXT_WIDTH 64
#define TEXT_HEIGHT 64
#define TEXTURE_NUM 1

GLuint texture[TEXTURE_NUM];

OBJLoader *treeObj;

Man man = Man();

int oldX = WIDTH / 2, oldY = HEIGHT / 2;

GLfloat blockEmission[] = { 0.5, 0.5, 0.5, 1.0 };
void initMaterial() {
	int width, height;
	unsigned char *image;

	FILE* file = fopen("res/textures/texture.png", "r");
	if (file == NULL) {
		cout << "cannot open texture file!" << endl;
		return;
	}
	fclose(file);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glGenTextures(TEXTURE_NUM, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, 1);
	image = SOIL_load_image("res/textures/texture.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SOIL_free_image_data(image);

	glMaterialfv(GL_FRONT, GL_EMISSION, blockEmission);
}

GLfloat lightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightDiffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat lightSpcular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 0.0f, 50.0f, 0.0f, 0.0f };


void initLight() {
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpcular);
	
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.09);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.032);
}

Group *group;
void initObjLoader() {
	string cd = "";
	//treeObj = new OBJLoader("res/objs/lft.obj");
	group = new Group(cd, "sphere.obj");
	cout << cd << endl;
}

void saveTheWorld() {
	fstream fout("world.txt", ios_base::out);
	for (map<Vector3, Block>::iterator i = man.model.world.begin(); i != man.model.world.end(); i++) {
		Vector3 vector3 = i->first;
		Block block = i->second;
		fout << vector3.x << " " << vector3.y << " " << vector3.z << " " << (int)block.type << endl;
	}
	fout.close();
	cout << "save successfully~" << endl;
}
void loadTheWorld() {
	//man.model.world.clear();
	fstream fin("world.txt", ios_base::in);
	while (!fin.eof()) {
		Vector3 vector3;
		int type;
		fin >> vector3.x >> vector3.y >> vector3.z >> type;
		Block block((Block_type)type);
		man.model.add_block(vector3, block, false);
	}
	_Block::iterator i = man.model.world.begin();
	for (; i != man.model.world.end(); i++) {
		Vector3 pos = i->first;
		i->second.exposed = man.model.get_exposed(pos);
	}
	cout << "load successfully~" << endl;
}


void initWorld() {
	int size = 30;
	/*
	* 创建草地
	*/
	for (int i = -size; i < size; i++) {
		for (int j = -size; j < size; j++) {
			int s;
			do {
				s = rand() % BLOCK_TYPE_NUM;
			} while ((Block_type)s == DEFAULT_BLOCK);

			man.model.add_block(Vector3(i, 0, j), Block(GRASS_BLOCK), false);
			man.model.add_block(Vector3(i, -1, j), Block(GRASS_BLOCK), false);
			man.model.add_block(Vector3(i, -2, j), Block(GRASS_BLOCK), false);
			man.model.add_block(Vector3(i, -3, j), Block(SAND_BLOCK), false);
			man.model.add_block(Vector3(i, -4, j), Block(SAND_BLOCK), false);
		}
	}
	/* 创建围墙
	*/

	for (int i = -size; i < size; i++) {
		for (int j = -size; j < size; j++) {
			if (i != -size && i != size - 1 && j != -size && j != size - 1) {
				man.model.add_block(Vector3(i, -5, j), Block(STONE_BLOCK), false);
			}
			else {
				for (int k = 0; k < 7; k++) {
					man.model.add_block(Vector3(i, k - 4, j), Block(STONE_BLOCK), false);
				}
			}
		}
	}

	_Block::iterator i = man.model.world.begin();
	for (; i != man.model.world.end(); i++) {
		Vector3 pos = i->first;
		i->second.exposed = man.model.get_exposed(pos);
	}
}

void init(void) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);   
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHT0);

	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH, GL_NICEST);
	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH, GL_FASTEST);
	glShadeModel(GL_FLAT);
	

	glClearColor(0.5, 0.69, 1.0, 1.0);
	glClearDepth(1000.0);

	/**
	* 雾渲染
	*/
	glEnable(GL_FOG);
	GLfloat fogfv[] = { 0,0,0,1 };
	glFogfv(GL_FOG_COLOR, fogfv);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 2.0);
	glFogf(GL_FOG_END, 6.0);

	/**
	* 纹理设置
	*/
	initMaterial();

	initWorld();


	/**
	* 光线设置
	*/
	initLight();

	/**
	* 模型设置
	*/
	initObjLoader();
}


FSOUND_STREAM *bgm, *grass_on, *grass_off, *sand_on, *sand_off,
*brick_on, *brick_off, *stone_on, *stone_off;