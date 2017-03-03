#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP
#include <fstream>
#include "util.h"
#include "GL\freeglut.h"

#include <vector>
#include <string>

using namespace std;



class OBJLoader {
	struct vertex {
		double x, y, z;
	};
	struct texture {
		double z, w;
	};
	struct normal {
		double x, y, z;
	};
	struct face {
		unsigned int v0, v1, v2;
		unsigned int vn0, vn1, vn2;
		unsigned int vt0, vt1, vt2;
	};
	vector<vertex> vertexes;
	vector<texture> textures;
	vector<normal> normals;
	vector<face> faces;
public:
	void parse(const char *filename);

	void parse(string filename);

	void draw();

	OBJLoader(const char *filename);

	OBJLoader(string filename);

	~OBJLoader();
};


void OBJLoader::parse(const char *filename) {
	char type[100];
	FILE *file = fopen(filename, "r");
	if (!file) {
		cout << "cant not open obj file:" << filename << endl;
		return;
	}
	while (true) {
		fscanf(file, "%s", type);
		if (feof(file)) break;
		if (!strcmp(type, "v")) {
			vertex v;
			fscanf(file, "%lf %lf %lf", &v.x, &v.y, &v.z);
			this->vertexes.push_back(v);
		}
		else if (!strcmp(type, "vt")) {
			texture t;
			fscanf(file, "%lf %lf", &t.z, &t.w);
			this->textures.push_back(t);
		}
		else if (!strcmp(type, "vn")) {
			normal n;
			fscanf(file, "%lf %lf %lf", &n.x, &n.y, &n.z);
			this->normals.push_back(n);
		}
		else if (!strcmp(type, "f")) {
			face f;
			fscanf(file, "%u/%u/%u %u/%u/%u %u/%u/%u", &f.v0, &f.vt0, &f.vn0, &f.v1, &f.vt1, &f.vn1, &f.v2,
				&f.vt2, &f.vn2);
			this->faces.push_back(f);
		}
		else {
			printf("%s", type);
		}
	};
	cout << "vertex:" << vertexes.size() << endl;
	cout << "texture:" << textures.size() << endl;
	cout << "normal:" << normals.size() << endl;
	cout << "face:" << faces.size() << endl;
}


void OBJLoader::draw() {
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < faces.size(); i++) {
		//索引要减去1
		vertex v0 = vertexes[faces[i].v0 - 1];
		vertex v1 = vertexes[faces[i].v1 - 1];
		vertex v2 = vertexes[faces[i].v2 - 1];
		texture t0 = textures[faces[i].vt0 - 1];
		texture t1 = textures[faces[i].vt1 - 1];
		texture t2 = textures[faces[i].vt2 - 1];
		glEvalCoord2d(t0.z, t0.w);
		glVertex3d(v0.x, v0.y, v0.z);
		glEvalCoord2d(t1.z, t1.w);
		glVertex3d(v1.x, v1.y, v1.z);
		glEvalCoord2d(t2.z, t2.w);
		glVertex3d(v2.x, v2.y, v2.z);
	}
	glEnd();
}


OBJLoader::OBJLoader(const char *filename) {
	parse(filename);
}

OBJLoader::~OBJLoader() {

}

void OBJLoader::parse(string filename) {
	parse(filename.c_str());
}

OBJLoader::OBJLoader(string filename) {
	parse(filename);
}

#endif // !OBJLOADER_HPP


