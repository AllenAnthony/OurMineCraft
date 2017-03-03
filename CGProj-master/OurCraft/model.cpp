#include "model.h"

Model::Model() {
	
}

//一个key是否在一个map中
bool inMap(const Vector3& key, const _Block& map) {
	if (map.find(key) != map.end())
		return true;
	return false;
}

//从vector中删除一个值
void deleteFromVector(Vector3& value, vector<Vector3>& vect) {
	for (vector<Vector3>::iterator i = vect.begin(); i != vect.end(); i++) {
		if ((*i) == value) {
			vect.erase(i);
			break;
		}	
	}
}

//判断一个位置有没有方块
bool Model::hasBlock(Vector3 position) {
	if (inMap(position, world))
		return true;
	return false;
}

//得到一个方块裸露面的信息
unsigned char Model::get_exposed(Vector3 position) {
	unsigned char exp = 0;

	for (int i = 0; i < 6; i++) {
		Vector3 pos = Vector3(position.x + FACES[i][0], position.y + FACES[i][1], position.z + FACES[i][2]);

		if (!hasBlock(pos)) {
			//cout << "这里没有方块" << i << endl;
			exp |= (1 << (6 - i - 1));
		}
	}

	//cout << (unsigned int)exp << endl;
	return exp;
}

//返回第一个有方块的位置，上一个位置
HitKey Model::hit_test(Vector3f position, Vector3f direction, int max_distance) {
	int m = 8;
	//Vector3 tmp = ROUND(position);
	//Float p[3] = { tmp.x, tmp.y, tmp.z };
	Float p[3] = { position.x, position.y, position.z };

	Vector3 previous = NONE;
	Vector3f hit;
	for (int i = 0; i < max_distance * m; i++) {
		hit = Vector3f(p);
		Vector3 Hit = ROUND(hit); 	//这里需要小心！！注意hit和Hit

		if (Hit != previous && hasBlock(Hit)) {
			return HitKey(Hit, previous);
		}
		previous = Hit;

		p[0] += (direction.x / m);		//是浮点数！！
		p[1] += (direction.y / m);
		p[2] += (direction.z / m);
	}

	return HitKey(NONE, NONE);
}


//往一个位置添加一个指定材质的方块
void Model::add_block(Vector3 position, Block block, bool immediate) {
	if (hasBlock(position)) {	//如果已经存在方块，先移除
		remove_block(position);
	}

	//world[position] = block;

	//区域中加入
	//Vector3 secp = SECTORIZE(Vector3f(position.x, position.y, position.z));
	//sectors[secp].push_back(position);

	//如果新加入的方块能显示，把它加入显示列表
	/*if (get_exposed(position)) {
		show_block(position, block);
	}*/

	if (immediate) {
		block.exposed = get_exposed(position);

		world[position] = block;

		check_neighbors(position);
	}
	else {
		world[position] = block;
	}
}


//去除一个方块
void Model::remove_block(Vector3 position, bool immediate) {
	Block block = world[position];		//保证一定是有块在那里的
	//if (block.type == STONE_BLOCK)		//石头不能移除
		//return;

	world.erase(position);

	//区域中删除
	//Vector3 secp = SECTORIZE(Vector3f(position.x, position.y, position.z));
	//deleteFromVector(position, sectors[secp]);

	//去掉的方块一定是之前能看到的，把它从显示列表中删除
	/*if (inMap(position, shown)) {
		hide_block(position);
	}*/

	
	if (immediate) {
		check_neighbors(position);
	}
}


//一个方块被删除，或是6个面都被隐藏，隐藏方块
void Model::hide_block(Vector3 position) {
	shown.erase(position);
}

//方块有面暴露在外，把它显示出来
void Model::show_block(Vector3 position, Block block) {
	shown[position] = block;
}


//对一个方块的周围进行检查，看是否有暴露或是被隐藏的块
void Model::check_neighbors(Vector3 position) {

	for (int i = 0; i < 6; i++) {
		Vector3 key(position.x + FACES[i][0], position.y + FACES[i][1], position.z + FACES[i][2]);
		if (!hasBlock(key))
			continue;

		/*if (!inMap(key, shown)) {
			if (exposed(key))	//之前没有在显示列表中，现在暴露出来了，加入显示列表
				show_block(key, world[key]);
		}
		else {
			if (!exposed(key))	//之前在显示列表中，现在被遮掩了，从显示列表中删除
				hide_block(key);
		}*/

		world[key].exposed = get_exposed(key);	//更新暴露面
	}
}


/*
* 绘制整个世界
*/
void Model::draw(Vector3f source, Vector3f direction) {
	_Block::iterator i = world.begin();
	for (; i != world.end(); i++) {
		Vector3 position = i->first;

		if (!atFront(Vector3f(position.x, position.y, position.z), source, direction))
			continue;

		Block block = i->second;

		glPushMatrix();
		glTranslated(position.x, position.y, position.z);
		block.draw();
		glPopMatrix();
	}
}


