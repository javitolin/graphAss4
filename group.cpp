/*
 * group.cpp
 *
 *  Created on: Jun 15, 2015
 *      Author: jdorfsman
 */
#include "group.h"
int group::allGroupsName = 0;
group::group(){
	groupNumber = allGroupsName++;
}
group::group(int gNum) {
	groupNumber = gNum;
	while(allGroupsName <= gNum)
		allGroupsName++;
}
group::group(vector<vector<pair<int, int> > > f) {
	fs = f;
	groupNumber = allGroupsName++;
}
group::group(vector<vector<pair<int, int> > >f, int gNum) {
	fs = f;
	groupNumber = gNum;
	while(allGroupsName <= gNum)
		allGroupsName++;
}


