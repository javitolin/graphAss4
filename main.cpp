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
Vector3f camPos = Vector3f(0, 0, -100);
float modelMatrix[16];
vector<float*> modelMatrixes;
int screenWidth = 512;
int screenHeight = 512;

vector<float> extractNumbers(string s, char separator) {
	vector<float> a;
	string forNow = "";
	for (unsigned int i = 0; i < s.size(); i++) {
		if (s[i] == separator) {
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
	std::stringstream ss;
	ss << forNow;
	float f;
	ss >> f;
	a.push_back(f);
	forNow = "";
	return a;
}
vector<string> extractString(string s, char separator) {
	vector<string> a;
	string forNow = "";
	for (unsigned int i = 0; i < s.size(); i++) {
		if (s[i] == separator) {
			stringstream ss;
			ss << forNow;
			a.push_back(ss.str());
			forNow = "";
		} else {
			forNow += s[i];
		}
	}
	std::stringstream ss;
	ss << forNow;
	a.push_back(ss.str());
	forNow = "";
	return a;
}
vector<int> extractF(string line) {
	vector<int> ans;
	if (line == "")
		return ans;
	string forNow = "";
	for (unsigned int i = 0; i < line.length(); i++) {
		if (line.at(i) == '/') {
			stringstream ss;
			ss << forNow;
			float f;
			ss >> f;
			ans.push_back(f);
			i++; //Skipping next /
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
	for (unsigned int i = 0; i < v.size(); i++) {
		//cout << v[i] << endl;
	}
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
void init() {
	glInitNames();
	gluPerspective(60, 1, 2, 200);
	glTranslatef(camPos.x, camPos.y, camPos.z);
	glClearColor(0, 0, 0, 1);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glViewport(0.0, 0.0, screenWidth, screenHeight);

	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow("AMAZING 3D MODELING");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glutDisplayFunc(displayFunc);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}

void displayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);

	glColor3f(0, 0, 1);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);

	glColor3f(0, 1, 0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();
	glClearColor(0, 0, 0, 0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glMaterialfv(GL_FRONT, GL_AMBIENT, object_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, object_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, object_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, object_shine);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
	for (unsigned int i = 0; i < objects.size(); i++) {
		objectItem currObject = objects[i];
		glPushMatrix();
		float objectMatrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, objectMatrix);
		modelMatrixes.push_back(objectMatrix);
		glLoadName(i);
		for (unsigned int j = 0; j < currObject.getGroups().size(); j++) {
			group currG = currObject.getGroups().at(j);
			glBegin(GL_POLYGON);
			for (unsigned int k = 0; k < currG.getFs().size(); k++) {
				vector<pair<int, int> > currF = currG.getFs()[k];
				for (unsigned int kk = 0; kk < currF.size(); kk++) {
					Vector3f currVertex = vertexes.at(
							(currF.at(kk).first - 1 >= 0) ?
									currF.at(kk).first - 1 : 0);

					Vector3f currNormal = vertexesNormal.at(
							(currF.at(kk).second - 1 >= 0) ?
									currF.at(kk).second - 1 : 0);

					glNormal3f(currNormal.x, currNormal.y, currNormal.z);
					glVertex3f(currVertex.x / 100, currVertex.y / 100,
							currVertex.z / 100);
				}
			}
			glEnd();
			glPopMatrix();
		}
	}
	glPopMatrix();
	glFlush();
}
bool sceneMode = false;
int prevX, prevY;
int mousePressed = -1;
void mouse(int button, int state, int x, int y) {
	//cout << "Mouse Click: " << button << " " << state << " " << x << " " << y << endl;
	if (state == 0) {
		prevX = x;
		prevY = y;
		mousePressed = button;
	}
	if (button == 0) {
		//left button

	} else if (button == 1) {
		//middle button

	} else {
		//Right button

	}
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
					angle = -0.01;
				else
					angle = 0.01;
				glTranslatef(0, angle, 0);
			} else {
				//moved on Y
				if (changedX > 0)
					angle = 0.01;
				else
					angle = -0.01;
				glTranslatef(angle, 0, 0);
			}
		}
		glPopMatrix();
	} else {
		//TODO
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
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
					angle = -0.01;
				else
					angle = 0.01;
				glTranslatef(0, angle, 0);
			} else {
				//moved on Y
				if (changedX > 0)
					angle = 0.01;
				else
					angle = -0.01;
				glTranslatef(angle, 0, 0);
			}
		}
		glPopMatrix();
	}
	glMatrixMode(GL_PROJECTION);
	glutPostRedisplay();
}
void zoom(int zoomFactor) {
	glMatrixMode(GL_PROJECTION);
	//glTranslatef(camPos.x, camPos.y, camPos.z*zoomFactor);
	//gluPerspective (zoomFactor, (float)screenWidth/(float)screenHeight, near, far);
	//glScalef(zoomFactor,zoomFactor,1);
	glutPostRedisplay();
}

void specialKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F2:
		zoom(+0.05);
		break;
	case GLUT_KEY_F3:
		zoom(-0.05);
		break;
	default:
		cout << "Not recognized" << endl;
		break;
	}
}

void keyboard(unsigned char key, int x, int y) {
	if (key == ' ') {
		sceneMode = !sceneMode;
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
					//save previous object
					objects.push_back(*currentObject);
				}
				//Starting a new group
				if (currLine.size() > 1) {
					//Has group number;
					int objIndex = findObject(atof(currLine[1].c_str()));
					if (objIndex > 0) {
						currentObject = &objects.at(objIndex);
					}
					currentObject = new objectItem(atof(currLine[1].c_str()));
				} else {
					currentObject = new objectItem();
				}
				printVector(currLine);
			} else if (currLine[0] == "f") {
				vector<pair<int, int> > currF;
				for (unsigned int i = 1; i < currLine.size(); i++) {
					vector<int> currFi = extractF(currLine[i]);
					if (currFi.size() == 2) {
							currF.push_back(
									pair<int, int>(currFi[0], currFi[1]));
					}
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
		/*if (objects[i].getGroups().size() > 0) {
		 for (unsigned int j = 0; j < objects[i].getGroups().size(); j++) {
		 cout << "Group with name: "
		 << objects[i].getGroups()[j].getName() << endl;
		 cout << "With fs: " << endl;
		 printF(objects[i].getGroups()[j].getFs());

		 }
		 }*/
	}
	glutInit(&argc, argv);
	init();

}
