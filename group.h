/*
 * group.h
 *
 *  Created on: May 15, 2015
 *      Author: jdorfsman
 */

#ifndef GROUP_H_
#define GROUP_H_
#include <vector>
#include "GL/gl.h"
using namespace std;
class group {
public:
	group();
	group(vector<vector<pair<int, int> > > f);
	group(vector<vector<pair<int, int> > >f, int gNum);
	group(int gNum);
	const vector<vector<pair<int, int> > >& getFs() const {
		return fs;
	}
	void setFs(const vector<vector<pair<int, int> > >& fs) {
		this->fs = fs;
	}
	void addToFs(vector<pair<int,int> > currF) {
		fs.push_back(currF);
	}
	int getName(){
		return groupNumber;
	}

private:
	vector<vector<pair<int, int> > > fs;
	int groupNumber;
	static int allGroupsName;
};

class objectItem {
public:
	objectItem();
	objectItem(vector<group> g);
	objectItem(int n);
	void addGroup(group g) {
		groups.push_back(g);
	}
	int getName() {
		return name;
	}
	vector<group> getGroups() {
		return groups;
	}
	int getMoveType() {return moveType;};
	GLfloat* getMoves(){ return movements;}
	GLfloat getAngle() {return angle;}
	GLfloat* getScale() {return scale;}
	char getSideToRotate() {return sideToRotate;}

private:
	vector<group> groups;
	int name;
	int moveType; //0 - translation, 1 - rotation, 2 - scale
	GLfloat movements[3];
	char sideToRotate;
	GLfloat angle;
	GLfloat scale[3];
	static int allNames;

};

#endif /* GROUP_H_ */
