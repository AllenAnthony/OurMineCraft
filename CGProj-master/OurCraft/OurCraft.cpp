#include "util.h"
#include "man.h"
#include "model.h"
#include "ourCraft.h"

GLfloat const mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat const mat_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat const mat_shininess[] = { 20.0 };

//GLfloat const lightPosition[] = { 700, 700, 700, 1.0 };
//GLfloat const ambientLight[] = { 1.0, 1.0, 1.0, 0.5 };
//GLfloat const diffuseLight[] = { 0.8, 0.8, 0.8, 0.8 };
//GLfloat const specularLight[] = { 1.0, 1.0, 1.0, 1.0 };


/*
void init() {			//初始化函数 
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	//设置光源位置
														//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
}
*/

void reshape(int w, int h) {	//大小改变回调函数 
	//oldX = w / 2;
	//oldY = h / 2;

	man.width = w;
	man.height = h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluOrtho2D(0, w, 0, h);

	//gluPerspective(75.0, w / (Float)h, 0.1, 60.0);
	//gluLookAt(0.0, 1.7, 0.0, 0.0, 1.7, -1.0, 0.0, 1.0, 0.0);
	//gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//键盘事件
void keyboardEvent(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'w':
		man.motion.z = -1;
		break;

	case 's':
		man.motion.z = 1;
		break;

	case 'a':
		man.motion.x = -1;
		break;

	case 'd':
		man.motion.x = 1;
		break;
	case 32:	//空格
		if (!man.flying) {		//跳
			if (man.dy == 0) man.dy = JUMP_SPEED * TIME_INTERVAL;
			break;
		}
		else {		//向上飞
			man.motion.y = 1;
		}
		break;

	

	case 9:		//TAB
		man.flying = !man.flying;
		break;

	default:
		break;
	}
}


//键盘事件
void specialKeyboardEvent(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_SHIFT_L:	//SHIFT
		if (man.flying) {
			man.motion.y = -1;
		}
		break;
	default:
		break;
	}
}

void specialKeyboardUpEvent(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_F5:
		saveTheWorld();
		break;
	case GLUT_KEY_F7:
		loadTheWorld();
		break;
	case GLUT_KEY_SHIFT_L:	//SHIFT
		if (man.flying) {
			man.motion.y = 0;
		}
		break;
	default: break;
	}
}

bool lightOn = false;

//键盘释放事件
void keyboardUpEvent(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'w':
		man.motion.z = 0;
		break;

	case 's':
		man.motion.z = 0;
		break;

	case 'a':
		man.motion.x = 0;
		break;

	case 'd':
		man.motion.x = 0;
		break;
	case 'l':
		if (lightOn) {
			glDisable(GL_LIGHT0);
			lightOn = false;
		}
		else {
			glEnable(GL_LIGHT0);
			lightOn = true;
		}
		break;
	case 'e':
		man.current_type = (Block_type)((man.current_type + 1) % BLOCK_TYPE_NUM);
		if (man.current_type == DEFAULT_BLOCK)
			man.current_type = (Block_type)((man.current_type + 1) % BLOCK_TYPE_NUM);
		break;

	case 'q':
		man.current_type = (Block_type)((man.current_type - 1) % BLOCK_TYPE_NUM);
		if (man.current_type == DEFAULT_BLOCK) {
			man.current_type = (Block_type)(man.current_type - 1 + BLOCK_TYPE_NUM);
		}	
		break;
	//数字键选择方块类型
	case '1':  //草
		man.set_current_block(GRASS_BLOCK);
		break;
	case '2': //沙
		man.set_current_block(SAND_BLOCK);
		break;
	case '3':  //砖
		man.set_current_block(BRICK_BLOCK);
		break;
	case '4':  //石
		man.set_current_block(STONE_BLOCK);
		break;

	case 32:
		if (man.flying) {
			man.motion.y = 0;
		}
		break;

	case GLUT_KEY_SHIFT_L:	//shift
		if (man.flying) {
			man.motion.y = 0;
		}
		break;

	case 27: //ESC
		exit(0); //离开游戏
		break;

	default:
		break;
	}
}


//鼠标按键事件
void mouseEvent(int button, int state, int x, int y) {
	Vector3f direction = man.get_sight_vector();
	HitKey hitkey = man.model.hit_test(man.position + Vector3f(0, man.man_height - 1, 0), direction);
	Vector3 hit = hitkey.hit;
	Vector3 previous = hitkey.previous;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (hit != NONE) {
			Block block = man.model.world[hit];
			man.model.remove_block(hit);

			switch (block.type)
			{
			case GRASS_BLOCK:
				FSOUND_Stream_Play(FSOUND_FREE, grass_off);
				break;

			case SAND_BLOCK:
				FSOUND_Stream_Play(FSOUND_FREE, sand_off);
				break;

			case BRICK_BLOCK:
				FSOUND_Stream_Play(FSOUND_FREE, brick_off);
				break;

			case STONE_BLOCK:
				FSOUND_Stream_Play(FSOUND_FREE, stone_off);
				break;

			default:
				break;
			}
		}

		//oldX = x, oldY = y;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (previous != NONE) {
			Block block(man.current_type);
			man.model.add_block(previous, block);

			switch (man.current_type)
			{
			case GRASS_BLOCK:
				FSOUND_Stream_Play(FSOUND_FREE, grass_on);
				break;

			case SAND_BLOCK:
				FSOUND_Stream_Play(FSOUND_FREE, sand_on);
				break;

			case BRICK_BLOCK:
				FSOUND_Stream_Play(FSOUND_FREE, brick_on);
				break;

			case STONE_BLOCK:
				FSOUND_Stream_Play(FSOUND_FREE, stone_on);
				break;

			default:
				break;
			}
		}
	}
}


int mouse_lock = 0;

#define minScreenWidth (man.width / 8)
#define maxScreenWidth (man.width / 8 * 7)
#define minScreenHeight (man.height / 8)
#define maxScreenHeight (man.height / 8 * 7)

void mouseFix(int& x, int& y) {
	if (x > maxScreenWidth) {
		x = minScreenWidth;
		SetCursorPos(x, y);
	}
	else if (x < minScreenWidth) {
		x = maxScreenWidth;
		SetCursorPos(x, y);
	}
	if (y > maxScreenHeight) {
		y = minScreenHeight;
		SetCursorPos(x, y);
	}
	else if (y < minScreenHeight) {
		y = maxScreenHeight;
		SetCursorPos(x, y);
	}
}

//鼠标移动事件
void mouseMoveEvent(int x, int y) {

	int dx = x - oldX, dy = oldY - y;

	mouseFix(x, y);

	Float m = 0.15;
	Float phi = man.rotation.PHI;
	Float theta = man.rotation.THETA;

	phi += dx * m;
	theta += dy * m;

	theta = MAX(-90, MIN(90, theta));

	man.rotation.PHI = phi;
	man.rotation.THETA = theta;

	oldX = x;
	oldY = y;

	//cout<<"rotation: "<<"phi: "<<man.rotation.PHI<<"  theta: "<<man.rotation.THETA<<endl;
}



/**********************萌萌哒分界线***********************/
void draw_cross() {
	glBegin(GL_LINES);

		glVertex2f(man.width / 2 - 10.0f, man.height / 2);
		glVertex2f(man.width / 2 + 10.0f, man.height / 2);

		glVertex2f(man.width / 2, man.height / 2 - 10.5f);
		glVertex2f(man.width / 2, man.height / 2 + 10.5f);

	glEnd();
}

void draw_focused_block(Vector3f direction) {
	
	HitKey hitkey = man.model.hit_test(man.position + Vector3f(0, man.man_height - 1, 0), direction);
	Vector3 hit = hitkey.hit;

	if (hit == NONE) return;
	Float x = hit.x, y = hit.y, z = hit.z;

	glColor3d(1.0, 1.0, 1.0);

	glBegin(GL_LINES);
	
	glVertex3f(x - 0.51, y + 0.51, z + 0.51);
	glVertex3f(x + 0.51, y + 0.51, z + 0.51);

	glVertex3f(x - 0.51, y + 0.51, z - 0.51);
	glVertex3f(x + 0.51, y + 0.51, z - 0.51);

	glVertex3f(x - 0.51, y - 0.51, z + 0.51);
	glVertex3f(x + 0.51, y - 0.51, z + 0.51);

	glVertex3f(x - 0.51, y - 0.51, z - 0.51);
	glVertex3f(x + 0.51, y - 0.51, z - 0.51);


	glVertex3f(x - 0.51, y - 0.51, z + 0.51);
	glVertex3f(x - 0.51, y - 0.51, z - 0.51);

	glVertex3f(x + 0.51, y - 0.51, z + 0.51);
	glVertex3f(x + 0.51, y - 0.51, z - 0.51);

	glVertex3f(x - 0.51, y + 0.51, z + 0.51);
	glVertex3f(x - 0.51, y + 0.51, z - 0.51);

	glVertex3f(x + 0.51, y + 0.51, z + 0.51);
	glVertex3f(x + 0.51, y + 0.51, z - 0.51);


	glVertex3f(x - 0.51, y + 0.51, z - 0.51);
	glVertex3f(x - 0.51, y - 0.51, z - 0.51);

	glVertex3f(x - 0.51, y + 0.51, z + 0.51);
	glVertex3f(x - 0.51, y - 0.51, z + 0.51);

	glVertex3f(x + 0.51, y + 0.51, z - 0.51);
	glVertex3f(x + 0.51, y - 0.51, z - 0.51);

	glVertex3f(x + 0.51, y + 0.51, z + 0.51);
	glVertex3f(x + 0.51, y - 0.51, z + 0.51);

	glEnd();
}

#define CURRENT_BLOCK_SHOW_X 100
#define CURRENT_BLOCK_SHOW_Y 100

void drawCurrentBlock() {
	Block_type block_type = man.current_type;
	Material material;
	switch (block_type) {
	case GRASS_BLOCK: 
		material = GRASS_AROUND;
		break;
	case SAND_BLOCK:
		material = SAND;
		break;
	case BRICK_BLOCK:
		material = BRICK;
		break;
	case STONE_BLOCK:
		material = STONE;
		break;
	default: 
		material = DEFAULT;
	}
	glBegin(GL_POLYGON);
	glTexCoord2d(MATERIAL_POSITION[material][0], MATERIAL_POSITION[material][2]);  glVertex2f(CURRENT_BLOCK_SHOW_X+64.0f, CURRENT_BLOCK_SHOW_Y+64.0f);
	glTexCoord2d(MATERIAL_POSITION[material][1], MATERIAL_POSITION[material][2]);  glVertex2f(CURRENT_BLOCK_SHOW_X+0.0f, CURRENT_BLOCK_SHOW_Y+64.0f);
	glTexCoord2d(MATERIAL_POSITION[material][1], MATERIAL_POSITION[material][3]);  glVertex2f(CURRENT_BLOCK_SHOW_X+0.0f, CURRENT_BLOCK_SHOW_Y+0.0f);
	glTexCoord2d(MATERIAL_POSITION[material][0], MATERIAL_POSITION[material][3]);  glVertex2f(CURRENT_BLOCK_SHOW_X+64.0f, CURRENT_BLOCK_SHOW_Y+0.0f);
	glEnd();
}
GLfloat sunColor[] = { 1.0, 0.8, 0.0, 0.0 };
float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float scale[] = { 50,50,50 };
float transf[] = { 0, 50, 0 };
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	man.set_3d();
	Vector3f direction = man.get_sight_vector();
	glBindTexture(GL_TEXTURE_2D, 1);
	man.model.draw(man.position + Vector3f(0, man.man_height - 1, 0), direction);
	draw_focused_block(direction);
	
	glPushMatrix();
	
	glMaterialfv(GL_FRONT, GL_EMISSION, sunColor);
	//glTranslated(0, 30, 0);
	//glScaled(10, 10, 10);
	group->Draw(color, scale, transf);
	
	glMaterialfv(GL_FRONT, GL_EMISSION, blockEmission);
	glPopMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//treeObj->draw();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 1);
	man.set_2d();
	draw_cross();
	drawCurrentBlock();

	glutSwapBuffers();
}


//定时更新
void myUpdate(int value) {
	man.update();
	glutPostRedisplay();
	glutTimerFunc(20, myUpdate, 1);
}


GLvoid initFMOD(void)
{
	if (FSOUND_Init(44100, 32, 0)) {
		bgm = FSOUND_Stream_OpenFile("res/music/backmusic.mp3", FSOUND_LOOP_NORMAL, 0);
		grass_on = FSOUND_Stream_OpenFile("res/music/effects/grass_on.ogg", FSOUND_LOOP_OFF, 0);
		grass_off = FSOUND_Stream_OpenFile("res/music/effects/grass_off.ogg", FSOUND_LOOP_OFF, 0);
		sand_on = FSOUND_Stream_OpenFile("res/music/effects/sand_on.ogg", FSOUND_LOOP_OFF, 0);
		sand_off = FSOUND_Stream_OpenFile("res/music/effects/sand_off.ogg", FSOUND_LOOP_OFF, 0);
		brick_on = FSOUND_Stream_OpenFile("res/music/effects/brick_on.ogg", FSOUND_LOOP_OFF, 0);
		brick_off = FSOUND_Stream_OpenFile("res/music/effects/brick_off.ogg", FSOUND_LOOP_OFF, 0);
		stone_on = FSOUND_Stream_OpenFile("res/music/effects/stone_on.ogg", FSOUND_LOOP_OFF, 0);
		stone_off = FSOUND_Stream_OpenFile("res/music/effects/stone_off.ogg", FSOUND_LOOP_OFF, 0);
	}
}

GLvoid freeFMOD(void)
{
	if (bgm != NULL)
	{
		FSOUND_Stream_Close(bgm);
		FSOUND_Stream_Close(grass_on);
		FSOUND_Stream_Close(grass_off);
		FSOUND_Stream_Close(sand_on);
		FSOUND_Stream_Close(sand_off);
		FSOUND_Stream_Close(brick_on);
		FSOUND_Stream_Close(brick_off);
		FSOUND_Stream_Close(stone_on);
		FSOUND_Stream_Close(stone_off);
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	//glutFullScreen();
	//glutInitWindowPosition(500, 500);
	glutInitWindowPosition(500, 500);
	glutCreateWindow("OurCraft");

	init(); 
	initFMOD();
	//register callback functions
	glutDisplayFunc(display);
	//glutIdleFunc(glutPostRedisplay);
	glutMouseFunc(mouseEvent);
	glutKeyboardFunc(keyboardEvent);
	glutKeyboardUpFunc(keyboardUpEvent);
	glutSpecialUpFunc(specialKeyboardUpEvent);
	glutSpecialFunc(specialKeyboardEvent);
	glutPassiveMotionFunc(mouseMoveEvent);
	glutReshapeFunc(reshape);
	glutSetCursor(GLUT_CURSOR_NONE);

	glutFullScreen();  //全屏
	//SetCursorPos(screenWidth / 2, screenHeight / 2);  //将鼠标移至视野中心

	glutTimerFunc(20, myUpdate, 1);
	cout << glGetString(GL_VENDOR) << endl;

	FSOUND_Stream_Play(FSOUND_FREE, bgm);	//开始播放背景音乐

	glutMainLoop();
	
	freeFMOD();
	return 0;
}

