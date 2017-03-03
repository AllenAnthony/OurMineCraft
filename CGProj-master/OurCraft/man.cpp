#include "man.h"

Man::Man() {
	man_height = 2;
	height = HEIGHT;
	width = WIDTH;
	current_type = GRASS_BLOCK;
	exclusive = false;
	flying = false;
	motion.z = motion.x = motion.y = 0;
	rotation.PHI = rotation.THETA = 0;
	position = Vector3f(0, 1, 0);
	model = Model();
}



//��õ�ǰ�ӽǷ���λ����
Vector3f Man::get_sight_vector() {
	Float dx = sin(RADIUS(rotation.PHI)) * cos(RADIUS(rotation.THETA));
	Float dy = sin(RADIUS(rotation.THETA));
	Float dz = -cos(RADIUS(rotation.PHI)) * cos(RADIUS(rotation.THETA));

	return Vector3f(dx, dy, dz);
}


/*
* �����һ��״̬��λ������
*/
Vector3f Man::get_motion_vector() {
	if ((!motion.x) && (!motion.z) && (!motion.y)) return Vector3f(0, 0, 0);
	
	Float offset = DEGREE(atan2(motion.z, motion.x));
	Float m, dy, dx, dz;

	if (flying) {	//������ڷɵ�״̬
		dy = motion.y;
		if (!motion.z && !motion.x) {
			dx = dz = 0;
		}
		else {
			dx = cos(RADIUS(rotation.PHI + offset));
			dz = sin(RADIUS(rotation.PHI + offset));
		}
	}
	else {
		dy = 0;
		dx = cos(RADIUS(rotation.PHI + offset));
		dz = sin(RADIUS(rotation.PHI + offset));
	}

	//cout << "dx: " << dx << "  dy: " << dy << "  dz: " << dz << endl;

	return Vector3f(dx, dy, dz);
}


Vector3f Man::collide(Vector3f position) {
	//��������Ϊһ��ϸ�����壬padding��ʾ�����׼�����ƫ����padding >= 0.5�����ǿ�� <= 0��Ҳ���ǿ��Դ����κ������ˡ�
	Float padding = 0.15;
	Float p[3] = { position.x, position.y, position.z };
	Vector3 t = ROUND(position);
	const int np[3] = {t.x, t.y, t.z};		//��λ������ȡ�����ͷ���λ�ö�Ӧ

	int op[3];

	int i, j, k;
	for (i = 0; i < 6; i++) {		//ǰ���������6������
		for (j = 0; j < 3; j++) {
			if (FACES[i][j] == 0) continue;	//6�������xyzֻ��һ���Ƿ�0��

			Float d = (p[j] - np[j]) * FACES[i][j];
			if (d < padding) {
				//cout << "d < padding   " <<d<< endl;
				continue;
			}

			for (k = 0; k < man_height; k++) {
				op[0] = np[0];
				op[1] = np[1];
				op[2] = np[2];

				op[1] += k;		//y������ϸ߶�
				op[j] += (FACES[i][j]);

				//cout << "opx: " << op[0] << "  opy: " << op[1] << "  opz: " << op[2] << endl;

				if (!model.hasBlock(Vector3(op))) continue;

				p[j] -= ((d - padding) * FACES[i][j]);

				if (i == 0 || i == 1) {		//y����
					this->dy = 0;
					//cout << "ͷ��������з���" << endl;
				}
				else {
					//cout << "�����з���" << endl;
				}
				break;
			}
		}
	}

	//cout << "dx: " << p[0] << "  dy: " << p[1] << "  dz: " << p[2] << endl;
	//system("pause");
	return Vector3f(p);
}


void Man::update() {
	speed = flying ? 12 : 5;

	Vector3f d = get_motion_vector() * TIME_INTERVAL * speed;

	//cout << "dx: " << d.x << "  dy: " << d.y << "  dz: " << d.z << endl;

	if (!flying) {
		dy -= 0.0096;
		dy = MAX(dy, -0.48);
		d.y += dy;
		//cout << dy << endl;
	}

	position = collide(position + d);

	//cout << "x: " << position.x << "  y: " << position.y << "  z: " << position.z << endl;
}


void Man::set_3d() {
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, width / height, 0.1, 5000.0);
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	
	glPushMatrix();
	glRotatef(rotation.PHI, 0, 1, 0);
	glRotatef(-rotation.THETA, cos(RADIUS(rotation.PHI)), 0, sin(RADIUS(rotation.PHI)));
	glTranslatef(-position.x, -(position.y + man_height - 1), -position.z);
}

void Man::set_2d()
{
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

void Man::set_current_block(Block_type type) {
	current_type = type;
}