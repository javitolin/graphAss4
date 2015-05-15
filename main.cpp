/*
 * main.cpp
 *
 *  Created on: May 12, 2015
 *      Author: jdorfsman
 */

#include <GL/gl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include "group.h"
#include "Vector3f.h"
#include <stdlib.h>

using namespace std;

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
	std::stringstream ss;
	ss << forNow;
	float f;
	ss >> f;
	ans.push_back(f);
	forNow = "";
	return ans;
}

void printVector(vector<string> v) {
	for (unsigned int i = 0; i < v.size(); i++) {
		//cout << v[i] << endl;
	}
}
void printF(vector<vector<pair<int, int> > > vp) {
	for (unsigned int i = 0; i < vp.size(); i++) {
		for(unsigned int j = 0; j < vp[i].size(); j++){
			cout << "v " << vp[i][j].first << " vn " << vp[i][j].second << endl;
		}
		cout << "end of curr j" << endl;
	}
}

GLint vertexCounter = 0;
GLint vertexNormalCounter = 0;
vector<objectItem> objects;
vector<Vector3f> vertexes;
vector<Vector3f> vertexesNormal;
group *currentGroup;
objectItem *currentObject;

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
					currentObject = new objectItem(atof(currLine[1].c_str()));
				} else {
					currentObject = new objectItem();
				}
				printVector(currLine);
			} else if (currLine[0] == "f") {
				vector<pair<int,int> > currF;
				for (unsigned int i = 1; i < currLine.size(); i++) {
					vector<int> currFi = extractF(currLine[i]);
					if (currFi.size() > 0) {
						currF.push_back(pair<int,int>(currFi[0],currFi[1]));
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
		cout << "In object " << (i+1) << " There are "
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


}
