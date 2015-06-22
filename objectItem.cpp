/*
 * objectItem.cpp
 *
 *  Created on: Jun 15, 2015
 *      Author: jdorfsman
 */

#include "group.h"

int objectItem::allNames = 0;
objectItem::objectItem() {
	name = allNames++;
	moveType = -1;
}

objectItem::objectItem(vector<group> g) {
	groups = g;
	name = allNames++;
	moveType = -1;
}
objectItem::objectItem(int n) {
	name = n;
	//name =
	while(allNames <= n)
		allNames++;
	moveType = -1;
}
