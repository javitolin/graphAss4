/*
 * main.cpp
 *
 *  Created on: May 12, 2015
 *      Author: jdorfsman
 */

#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include "group.h"
#include "Vector3f.h"
#include <stdlib.h>
#define SPACEBAR 32
#define far 20.0
#define near 2.0
#define bufSize 512
using namespace std;

vector<objectItem> objects;
vector<Vector3f> vertexes;
vector<Vector3f> vertexesNormal;
group *currentGroup;
objectItem *currentObject;
GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_diffuse[] = { 0.0, 0.5, 0.5, 1.0 };
GLfloat light_specular[] = { 0.0, 0.0, 0.5, 1.0 };
GLfloat light_directional[] = { 0.0, -1.0, -1.0 };
GLfloat light_position[] = { -1.0, 1.0, -1.0, 0.0 };

GLfloat object_ambient[] = { 0.3, 0.4, 0.5, 1.0 };
GLfloat object_diffuse[] = { 0.0, 0.6, 0.7, 1.0 };
GLfloat object_specular[] = { 0.0, 0.0, 0.8, 1.0 };
GLfloat object_shine[] = { 5 };
Vector3f camera = Vector3f(0, 0, -100);
float modelMatrix[16];
vector<float*> modelMatrixes;
int screenWidth = 512;
int screenHeight = 512;
float fovAngle = 60.0f;
bool sceneMode = false;
bool changeFOV = false;
double fovScale;

float zValue[4];
GLint hits;
int ii = 0;
int jj = 0;
bool isPick = false;
int xx, yy;

vector<float> extractNumbers(string s, char separator) {
	vector<float> a;
	string forNow = "";
	for (unsigned int i = 0; i < s.size(); i++) {
		if (s[i] == separator) {
			if (forNow == "")
				continue;
			stringstream ss;
			ss << forNow;
			float f;
			ss >> f;
			a.push_back(f);
			forNow = "";
		} else {
			forNow += s[i];
		}
	}
	if (forNow != "") {
		std::stringstream ss;
		ss << forNow;
		float f;
		ss >> f;
		a.push_back(f);
	}
	return a;
}
vector<string> extractString(string s, char separator) {
	vector<string> a;
	string forNow = "";
	for (unsigned int i = 0; i < s.size(); i++) {
		if (s[i] == separator) {
			if (forNow == "")
				continue;
			stringstream ss;
			ss << forNow;
			a.push_back(ss.str());
			forNow = "";
		} else {
			forNow += s[i];
		}
	}
	if (forNow != "") {
		std::stringstream ss;
		ss << forNow;
		a.push_back(ss.str());
	}
	return a;
}
vector<int> extractF(string line) {
	vector<int> ans;
	if (line == "")
		return ans;
	string forNow = "";
	for (unsigned int i = 0; i < line.length(); i++) {
		if (line.at(i) == '/') {
			if (forNow == "")
				continue;
			stringstream ss;
			ss << forNow;
			float f;
			ss >> f;
			ans.push_back(f);
			forNow = "";
		} else {
			forNow += line.at(i);
		}
	}
	if (forNow != "") {
		std::stringstream ss;
		ss << forNow;
		float f;
		ss >> f;
		ans.push_back(f);
	}
	return ans;
}

void printVector(vector<string> v) {
	/*for (unsigned int i = 0; i < v.size(); i++) {
	 cout << v[i] << endl;
	 }*/
}
void printF(vector<vector<pair<int, int> > > vp) {
	for (unsigned int i = 0; i < vp.size(); i++) {
		for (unsigned int j = 0; j < vp[i].size(); j++) {
			cout << "v " << vp[i][j].first << " vn " << vp[i][j].second << endl;
		}
		cout << "end of curr j" << endl;
	}
}

int findObject(int name) {
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (objects.at(i).getName() == name)
			return i;
	}
	return -1;
}
void displayFunc(void);
void mouse(int, int, int, int);
void motion(int, int);
void specialKey(int, int, int);
void keyboard(unsigned char, int, int);
void idle(int);
void setTransformations() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovAngle, screenWidth / screenHeight, 2, 200);
	glTranslatef(camera.x, camera.y, camera.z);
	glMatrixMode(GL_MODELVIEW);
}

void startPicking(GLuint *selectionBuf) {
	glSelectBuffer(bufSize, selectionBuf); //declare buffer for input in selection mode
	glRenderMode(GL_SELECT); //change to selecting mode
	glInitNames();			//initialize names stack
	glPushName(-1);			//push name
}
void processHits(GLint hits, GLuint *buffer) {
	float z1, z2;
	for (int i = 0; buffer[i] > 0; i += 5) {
		z1 = buffer[i + 1] / 4294967295.0;
		z2 = buffer[i + 2] / 4294967295.0;

		printf("z1 = %f ,z2 = %f zValue = %f\n", z1, z2, zValue[0]);
		if ((zValue[0] <= z1 + 0.0001 && zValue[0] >= z2 - 0.0001)
				|| (zValue[0] >= z1 - 0.0001 && zValue[0] <= z2 + 0.0001)) {//try to locate which name is correlated with the pressed pixel according to z value
			ii = buffer[i + 3];
			jj = buffer[i + 4];
		}

	}
}

void init() {
	string windowName = "AMAZING 3D MODELING - ";
	windowName += (sceneMode ? "Scene" : "Camera");
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow(windowName.c_str());
	glViewport(0.0, 0.0, screenWidth, screenHeight);
	setTransformations();
	glInitNames();
	glPushName(0); //push 0 on stack
	glClearColor(0, 0, 0, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glMaterialfv(GL_FRONT, GL_AMBIENT, object_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, object_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, object_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, object_shine);
	glutDisplayFunc(displayFunc);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(2, idle, 1);
	glutMainLoop();
}
void idle(int v) {
	glutPostRedisplay();
	glutTimerFunc(1, idle, 0);
}
void drawImage(GLenum type) {
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, 0.0);

	glColor3f(0, 0, 1);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 100.0, 0.0);

	glColor3f(0, 1, 0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 100.0);
	glEnd();

	glColor4f(1.0, 1.0, 1.0, 1.0);

	if (changeFOV) {
		fovAngle += fovScale;
		changeFOV = false;
		setTransformations();
	}
	glPushMatrix();
	for (unsigned int i = 0; i < objects.size(); i++) {
		objectItem currObject = objects[i];
		if (type == GL_SELECT)
			glLoadName(currObject.getName());
		for (unsigned int j = 0; j < currObject.getGroups().size(); j++) {
			if (currentObject->getMoveType() > -1) {
				//object has moved
				glPushMatrix();
				switch (currentObject->getMoveType()) {
				case 0: //Translation
					glTranslatef(currentObject->getMoves()[0],
							currentObject->getMoves()[1],
							currentObject->getMoves()[2]);
					break;
				case 1: //Rotation
					if (currentObject->getSideToRotate() == 'u') {
						glRotatef(currentObject->getSideToRotate(), 0,1,0);
					} else if (currentObject->getSideToRotate() == 'r') {
						glRotatef(currentObject->getSideToRotate(), 0,0,1);
					} else if (currentObject->getSideToRotate() == 'l') {
						glRotatef(currentObject->getSideToRotate(), 0,0,-1);
					} else{ //down
						glRotatef(currentObject->getSideToRotate(), 0,-1,0);
					}
					break;
				case 2: //Scale
					glScalef(currentObject->getScale()[0],currentObject->getScale()[1],currentObject->getScale()[2]);
					break;
				}
				glPopMatrix();
			}
			group currGroup = currObject.getGroups().at(j);
			for (unsigned int k = 0; k < currGroup.getFs().size(); k++) {
				vector<pair<int, int> > currF = currGroup.getFs()[k];
				glBegin(GL_POLYGON);
				for (unsigned int kk = 0; kk < currF.size(); kk++) {
					Vector3f currVertex = vertexes.at(
							(currF.at(kk).first - 1 >= 0) ?
									currF.at(kk).first - 1 : 0);

					Vector3f currNormal = vertexesNormal.at(
							(currF.at(kk).second - 1 >= 0) ?
									currF.at(kk).second - 1 : 0);

					glNormal3f(currNormal.x, currNormal.y, currNormal.z);
					glVertex3f(currVertex.x / 1, currVertex.y / 1,
							currVertex.z / 1);
				}
				glEnd();
			}
		}
	}
	glPopMatrix();
}
void displayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawImage(GL_RENDER);
	glFlush();
}
void processHits2(GLint hits, GLuint buffer[]) {
	int i, j;
	GLuint names, *ptr, minZ, *ptrNames, numberOfNames;

	printf("hits = %d\n", hits);
	ptr = (GLuint *) buffer;
	minZ = 0xffffffff;
	for (i = 0; i < hits; i++) {
		names = *ptr;
		ptr++;
		if (*ptr < minZ) {
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr + 2;
		}

		ptr += names + 2;
	}
	printf("The closest hit names are ");
	ptr = ptrNames;
	for (j = 0; j < numberOfNames; j++, ptr++) {
		printf("%d ", *ptr);
	}
	printf("\n");

}
int prevX, prevY;
int mousePressed = -1;
void mouse(int button, int state, int x, int y) {
	if (state == 0) {
		prevX = x;
		prevY = y;
		mousePressed = button;
	}

	//GLint viewport[5];
	GLint viewport[4];
	GLuint selectionBuf[bufSize];
	GLuint buffer[bufSize];
	float pix[4];
	for (unsigned int i = 0; i < bufSize; i++)
		selectionBuf[i] = 0;
	if (state == 0) {
		glGetIntegerv(GL_VIEWPORT, viewport); //reading viewport parameters
		glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_FLOAT, pix);

		//	  printf("depth = %f, x = %d, y = %d\n",pixels[(viewport[3]-y)*512+x],x,viewport[3]-y);

		glMatrixMode(GL_PROJECTION);

		glReadPixels((GLdouble) x, (GLdouble) viewport[3] - y, 2, 2,
		GL_DEPTH_COMPONENT, GL_FLOAT, zValue);

		glPushMatrix();	//saves current projection matrix
		glLoadIdentity();

		startPicking(selectionBuf); //preper selection mode

		gluPickMatrix((GLdouble) x, (GLdouble) viewport[3] - y, 1, 1, viewport); //change matrices so only the area of the picking pixel can be seen.
		gluPerspective(fovAngle, 1, near, far); //return to perspective state

		glMatrixMode(GL_MODELVIEW);
		drawImage(GL_SELECT); //draws board on background

		hits = glRenderMode(GL_RENDER); //gets hits number
		printf("hits: %d\n", hits);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix(); //restores projection matrix

		glMatrixMode(GL_MODELVIEW);
		processHits(hits, selectionBuf); //check hits

		//  printf("depth %f hits: %d\n\n",pixels[(viewport[3]-y)*512+x], hits);
		if (zValue[0] < 1.0) {
			isPick = true;
			xx = x;
			yy = y;
		}

	} else
		isPick = false;
}
float angle = 0;
void motion(int x, int y) {
	int changedX = x - prevX;
	int changedY = y - prevY;
	if (sceneMode) {
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		if (mousePressed == 0) {
			//Left Pressed
			if (abs(changedY) > abs(changedX)) {
				//moved on X
				if (changedY > 0)
					angle = -1;
				else
					angle = 1;
				glRotatef(angle, 1.0f, 0.0f, 0.0f);
			} else {
				//moved on Y
				if (changedX > 0)
					angle = 1;
				else
					angle = -1;
				glRotatef(angle, 0.0f, 0.0f, 1.0f);
			}
		} else if (mousePressed == 1) {
			if (changedY < 0) {
				glTranslatef(0, 0, 0.01);
			} else {
				glTranslatef(0, 0, -0.01);
			}
		} else if (mousePressed == 2) {
			if (abs(changedY) > abs(changedX)) {
				//moved on Y
				if (changedY > 0)
					angle = -1;
				else
					angle = 1;
				glTranslatef(0, angle, 0);
			} else {
				//moved on Y
				if (changedX > 0)
					angle = 1;
				else
					angle = -1;
				glTranslatef(angle, 0, 0);
			}
		}
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	} else {
		if (mousePressed == 0) {
			//Left Pressed
			if (abs(changedY) > abs(changedX)) {
				//moved on X
				if (changedY > 0)
					angle = -1;
				else
					angle = 1;
				glRotatef(angle, 1, 0, 0);
			} else {
				//moved on Y
				if (changedX > 0)
					angle = 1;
				else
					angle = -1;
				glRotatef(angle, 0, 1, 0);
			}
		} else if (mousePressed == 1) {
			if (changedY < 0) {
				angle = 1.05;
			} else {
				angle = 0.95;
			}
			glScalef(angle, angle, angle);
		} else if (mousePressed == 2) {
			if (abs(changedY) > abs(changedX)) {
				if (changedY > 0)
					angle = +0.1;
				else
					angle = -0.1;
				glTranslatef(0, angle, 0);
			} else {
				//moved on Y
				if (changedX > 0)
					angle = -0.1;
				else
					angle = +0.1;
				glTranslatef(angle, 0, 0);
			}
		}
	}
}
void zoom(double scale) {
	glMatrixMode(GL_MODELVIEW);
	glScalef(scale, scale, 1);
	glMatrixMode(GL_PROJECTION);
}

void specialKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F2:
		changeFOV = true;
		fovScale = 0.05;
		break;
	case GLUT_KEY_F3:
		changeFOV = true;
		fovScale = -0.05;
		break;
	case GLUT_KEY_UP:
		zoom(1.05);
		break;
	case GLUT_KEY_DOWN:
		zoom(0.95);
		break;
	default:
		cout << "Not recognized" << endl;
		break;
	}
}

void keyboard(unsigned char key, int x, int y) {
	if (key == ' ') {
		sceneMode = !sceneMode;
		string windowName = "AMAZING 3D MODELING - ";
		windowName += (sceneMode ? "Scene" : "Camera");
		glutSetWindowTitle(windowName.c_str());
	}
}
int main(int argc, char* argv[]) {

	currentObject = new objectItem(-1); //Creating default object
	ifstream sceneFile((argc > 1) ? argv[1] : "simple.obj");
	vector<string> currLine;
	string line;
	while (std::getline(sceneFile, line)) {
		std::istringstream iss(line);
		if (line[0] != '#') {
			currLine = extractString(line, ' ');
			if (currLine[0] == "g") {
				if (currentGroup != 0) {
					currentObject->addGroup(*currentGroup);
				}
				if (currLine.size() > 1) {
					//Has group number;
					currentGroup = new group(atof(currLine[1].c_str()));
				} else {
					currentGroup = new group();
				}
				printVector(currLine);
			} else if (currLine[0] == "v") {
				Vector3f currV = Vector3f(atof(currLine[1].c_str()),
						atof(currLine[2].c_str()), atof(currLine[3].c_str()));
				vertexes.push_back(currV);
				printVector(currLine);
			} else if (currLine[0] == "vn") {
				Vector3f currNV = Vector3f(atof(currLine[1].c_str()),
						atof(currLine[2].c_str()), atof(currLine[3].c_str()));
				vertexesNormal.push_back(currNV);
				printVector(currLine);
			} else if (currLine[0] == "o") {
				if (currentObject != 0
						&& (currentObject->getName() != -1
								|| currentObject->getGroups().size() > 0)) {
					currentObject->addGroup(*currentGroup);
					//save previous object
					objects.push_back(*currentObject);
				}
				//Starting a new group
				if (currLine.size() > 1) {
					//Has group number;
					int objIndex = findObject(atof(currLine[1].c_str()));
					if (objIndex > 0) {
						currentObject = &objects.at(objIndex);
					} else {
						currentObject = new objectItem(
								atof(currLine[1].c_str()));
					}
				} else {
					currentObject = new objectItem();
				}
				printVector(currLine);
			} else if (currLine[0] == "f") {
				vector<pair<int, int> > currF;
				for (unsigned int i = 1; i < currLine.size(); i++) {
					vector<int> currFi = extractF(currLine[i]);
					if (currFi.size() == 2) {
						currF.push_back(pair<int, int>(currFi[0], currFi[1]));
					}
				}
				if (currentGroup == 0) {
					currentGroup = new group();
				}
				currentGroup->addToFs(currF);
				printVector(currLine);
			} else {
				printVector(currLine);
			}
		}
	}
	currentObject->addGroup(*currentGroup);
	objects.push_back(*currentObject);
	cout << "Finished loading file!" << endl;
	cout << "Total of " << vertexes.size() << " Vertexes" << endl;
	cout << "Total of " << vertexesNormal.size() << " Vertexes Normals" << endl;
	cout << "There are " << objects.size() << " Objects" << endl;
	for (unsigned int i = 0; i < objects.size(); i++) {
		cout << "In object " << (i + 1) << " There are "
				<< objects[i].getGroups().size() << " groups" << endl;
	}
	glutInit(&argc, argv);
	init();

}
