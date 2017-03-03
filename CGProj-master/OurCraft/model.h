#pragma once
#include "util.h"
#include "block.h"
#include <map>
#include <vector>


typedef map<Vector3, Block> _Block;

typedef map<Vector3, vector<Vector3>> _Sector;

typedef struct HitKey {
public:
	Vector3 hit;
	Vector3 previous;
public:
	HitKey(Vector3 a, Vector3 b) {
		hit = a;
		previous = b;
	}
} HitKey;

class Model {
public:
	_Block world;
	_Block shown;

	_Sector sectors;

	
public:
	Model();
	bool hasBlock(Vector3 position);
	unsigned char get_exposed(Vector3 position);
	HitKey hit_test(Vector3f position, Vector3f direction, int max_distance = 5);
	void add_block(Vector3 position, Block block, bool immediate = true);
	void remove_block(Vector3 position, bool immediate = true);

	void hide_block(Vector3 position);
	void show_block(Vector3 position, Block block);
	void check_neighbors(Vector3 position);

	void draw(Vector3f source, Vector3f direction);
};