/*
 * Item.cpp
 *
 *  Created on: May 12, 2015
 *      Author: jdorfsman
 */

#include "Item.h"

Item::Item(int groupName, int oName, vector<Vector3f> vert,
		vector<Vector3f> vertNormal, vector<pair<int, int> > pairs) {
	_groupName = groupName;
	_oName = oName;
	_vertices = vert;
	_verticesNormal = vertNormal;
	_vertexNormalPair = pairs;
}
