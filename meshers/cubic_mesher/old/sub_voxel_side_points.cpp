#include "sub_voxel_side_points.h"

using namespace voxelman;

#if MEM_TOOLS
int SubVoxelSidePoints::allocs = 0;
#endif

SubVoxelSidePoints::SubVoxelSidePoints() {
	//_points = new Vector<Vector3i>(4);
	for (int i = 0; i < 4; ++i) {
		Vector3i a;

		_points.push_back(a);
	}

	#if MEM_TOOLS
	SubVoxelSidePoints::allocs++;
	print_error("SubVoxelSidePoints alloc " + String::num(SubVoxelSidePoints::allocs));
	#endif
}

SubVoxelSidePoints::~SubVoxelSidePoints() {
	#if MEM_TOOLS
	SubVoxelSidePoints::allocs--;
	print_error("SubVoxelSidePoints dealloc " + String::num(SubVoxelSidePoints::allocs));
	#endif
}

bool SubVoxelSidePoints::is_sub_voxel_point(Vector3i point) {
	for (int i = 0; i < _points.size(); ++i) {
		if (_points.get(i) == point) {
			return true;
		}
	}
	return false;
}


bool SubVoxelSidePoints::is_sub_voxel_point(int x, int y, int z) {
	return is_sub_voxel_point(Vector3i(x, y, z));
	/*
	for (int i = 0; i < points->Length; i += 1) {
		if (points.get(i).Equals(x, y, z)) {
			return true;
		}
	}*/
	//return false;
}

Vector3 SubVoxelSidePoints::get_vector3_for_point(int index) {
	//return Vector3((float)(getPoints()->GetData(pointIndex)->x), (float)(getPoints()->GetData(pointIndex)->y), (float)(getPoints()->GetData(pointIndex)->z));
	Vector3i a = _points[index];

	Vector3 b(a.x, a.y, a.z);

	return b;
}

Vector3 SubVoxelSidePoints::get_vertex_vector3_for_point(int index) {
	//Vector3 vector3((float)(getPoints()->GetData(index)->x), (float)(getPoints()->GetData(index)->y), (float)(getPoints()->GetData(index)->z));
	Vector3i a = _points[index];

	Vector3 vector(a.x, a.y, a.z);

	float num = (float)255;
	float num2 = num / (float)2;
	vector.x -= num2;
	vector.y -= num2;
	vector.z -= num2;
	vector /= num;
	return vector;
}
