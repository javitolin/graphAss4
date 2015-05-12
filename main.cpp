/*
 * main.cpp
 *
 *  Created on: May 12, 2015
 *      Author: jdorfsman
 */
#include <vector>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>
#include "GL/gl.h"
#include "GL/glut.h"
#include "Vector3f.h"
using namespace std;

vector<float> extractNumbers(char *line, char separator) {
	vector<float> a;
	string s = line;
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
vector<string> extractString(char *line, char separator) {
	vector<string> a;
	string s = line;
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
	string forNow = "";
	for (unsigned int i = 0; i < line.length(); i++) {
		if (line.at(i) == '/') {
			stringstream ss;
			ss << forNow;
			float f;
			ss >> f;
			ans.push_back(f);
			i++;//Skipping next /
			forNow = "";
		}
		else{
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

int main(int argc, char* argv[]) {
	ifstream sceneFile((argc > 1) ? argv[1] : "simple.obj");
	char output[512];
	float gGroup;
	vector<Vector3f> vertices;
	vector<pair<int,int> > verticesPairs;
	while (!sceneFile.eof()) {
		sceneFile >> output;
		vector<string> currLine = extractString(output, ' ');
	}
}
