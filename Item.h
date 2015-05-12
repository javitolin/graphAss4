/*
 * Item.h
 *
 *  Created on: May 12, 2015
 *      Author: jdorfsman
 */

#ifndef ITEM_H_
#define ITEM_H_
#include "Vector3f.h"
#include <vector>
using namespace std;
class Item {
public:
	Item(int groupName, int oName, vector<Vector3f> vert, vector<Vector3f> vertNormal, vector<pair<int,int> > pairs);

	int getGroupName() const {
		return _groupName;
	}

	void setGroupName(int groupName) {
		_groupName = groupName;
	}

	int getName() const {
		return _oName;
	}

	void setName(int name) {
		_oName = name;
	}

	const vector<pair<int, int> >& getVertexNormalPair() const {
		return _vertexNormalPair;
	}

	void setVertexNormalPair(const vector<pair<int, int> >& vertexNormalPair) {
		_vertexNormalPair = vertexNormalPair;
	}

	const vector<Vector3f>& getVertices() const {
		return _vertices;
	}

	void setVertices(const vector<Vector3f>& vertices) {
		_vertices = vertices;
	}

	const vector<Vector3f>& getVerticesNormal() const {
		return _verticesNormal;
	}

	void setVerticesNormal(const vector<Vector3f>& verticesNormal) {
		_verticesNormal = verticesNormal;
	}

private:
	int _groupName;
	int _oName;
	vector<Vector3f> _vertices;
	vector<Vector3f> _verticesNormal;
	vector<pair<int,int> > _vertexNormalPair;
};

#endif /* ITEM_H_ */
