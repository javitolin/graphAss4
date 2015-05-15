/*
 * group.h
 *
 *  Created on: May 15, 2015
 *      Author: jdorfsman
 */

#ifndef GROUP_H_
#define GROUP_H_
#include <vector>
using namespace std;
class group {
public:
	group() {
	}
	group(vector<vector<pair<int, int> > > f) {
		fs = f;
		groupNumber = -1;
	}
	group(vector<vector<pair<int, int> > >f, int gNum) {
		fs = f;
		groupNumber = gNum;
	}
	group(int gNum) {
		groupNumber = gNum;
	}
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
};

class objectItem {
public:
	objectItem() {
	}
	objectItem(vector<group> g) {
		groups = g;
	}
	objectItem(int n) {
		name = n;
	}
	void addGroup(group g) {
		groups.push_back(g);
	}
	int getName() {
		return name;
	}
	vector<group> getGroups() {
		return groups;
	}

private:
	vector<group> groups;
	int name;
};

#endif /* GROUP_H_ */
