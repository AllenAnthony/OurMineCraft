#include "model.h"

Model::Model() {
	
}

//һ��key�Ƿ���һ��map��
bool inMap(const Vector3& key, const _Block& map) {
	if (map.find(key) != map.end())
		return true;
	return false;
}

//��vector��ɾ��һ��ֵ
void deleteFromVector(Vector3& value, vector<Vector3>& vect) {
	for (vector<Vector3>::iterator i = vect.begin(); i != vect.end(); i++) {
		if ((*i) == value) {
			vect.erase(i);
			break;
		}	
	}
}

//�ж�һ��λ����û�з���
bool Model::hasBlock(Vector3 position) {
	if (inMap(position, world))
		return true;
	return false;
}

//�õ�һ��������¶�����Ϣ
unsigned char Model::get_exposed(Vector3 position) {
	unsigned char exp = 0;

	for (int i = 0; i < 6; i++) {
		Vector3 pos = Vector3(position.x + FACES[i][0], position.y + FACES[i][1], position.z + FACES[i][2]);

		if (!hasBlock(pos)) {
			//cout << "����û�з���" << i << endl;
			exp |= (1 << (6 - i - 1));
		}
	}

	//cout << (unsigned int)exp << endl;
	return exp;
}

//���ص�һ���з����λ�ã���һ��λ��
HitKey Model::hit_test(Vector3f position, Vector3f direction, int max_distance) {
	int m = 8;
	//Vector3 tmp = ROUND(position);
	//Float p[3] = { tmp.x, tmp.y, tmp.z };
	Float p[3] = { position.x, position.y, position.z };

	Vector3 previous = NONE;
	Vector3f hit;
	for (int i = 0; i < max_distance * m; i++) {
		hit = Vector3f(p);
		Vector3 Hit = ROUND(hit); 	//������ҪС�ģ���ע��hit��Hit

		if (Hit != previous && hasBlock(Hit)) {
			return HitKey(Hit, previous);
		}
		previous = Hit;

		p[0] += (direction.x / m);		//�Ǹ���������
		p[1] += (direction.y / m);
		p[2] += (direction.z / m);
	}

	return HitKey(NONE, NONE);
}


//��һ��λ�����һ��ָ�����ʵķ���
void Model::add_block(Vector3 position, Block block, bool immediate) {
	if (hasBlock(position)) {	//����Ѿ����ڷ��飬���Ƴ�
		remove_block(position);
	}

	//world[position] = block;

	//�����м���
	//Vector3 secp = SECTORIZE(Vector3f(position.x, position.y, position.z));
	//sectors[secp].push_back(position);

	//����¼���ķ�������ʾ������������ʾ�б�
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


//ȥ��һ������
void Model::remove_block(Vector3 position, bool immediate) {
	Block block = world[position];		//��֤һ�����п��������
	//if (block.type == STONE_BLOCK)		//ʯͷ�����Ƴ�
		//return;

	world.erase(position);

	//������ɾ��
	//Vector3 secp = SECTORIZE(Vector3f(position.x, position.y, position.z));
	//deleteFromVector(position, sectors[secp]);

	//ȥ���ķ���һ����֮ǰ�ܿ����ģ���������ʾ�б���ɾ��
	/*if (inMap(position, shown)) {
		hide_block(position);
	}*/

	
	if (immediate) {
		check_neighbors(position);
	}
}


//һ�����鱻ɾ��������6���涼�����أ����ط���
void Model::hide_block(Vector3 position) {
	shown.erase(position);
}

//�������汩¶���⣬������ʾ����
void Model::show_block(Vector3 position, Block block) {
	shown[position] = block;
}


//��һ���������Χ���м�飬���Ƿ��б�¶���Ǳ����صĿ�
void Model::check_neighbors(Vector3 position) {

	for (int i = 0; i < 6; i++) {
		Vector3 key(position.x + FACES[i][0], position.y + FACES[i][1], position.z + FACES[i][2]);
		if (!hasBlock(key))
			continue;

		/*if (!inMap(key, shown)) {
			if (exposed(key))	//֮ǰû������ʾ�б��У����ڱ�¶�����ˣ�������ʾ�б�
				show_block(key, world[key]);
		}
		else {
			if (!exposed(key))	//֮ǰ����ʾ�б��У����ڱ������ˣ�����ʾ�б���ɾ��
				hide_block(key);
		}*/

		world[key].exposed = get_exposed(key);	//���±�¶��
	}
}


/*
* ������������
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


