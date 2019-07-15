#include "sub_voxel_side_points.h"


SubVoxelSidePoints::SubVoxelSidePoints() {
}

SubVoxelSidePoints::~SubVoxelSidePoints() {
}

bool SubVoxelSidePoints::is_sub_voxel_point_vec(Vector3i point) {
	for (int i = 0; i < 4; ++i) {
		if (_points[i] == point) {
			return true;
		}
	}
	return false;
}


bool SubVoxelSidePoints::is_sub_voxel_point(int x, int y, int z) {
	for (int i = 0; i < 4; i += 1) {
		if (_points[i].x == x && _points[i].y == y && _points[i].z == z) {
			return true;
		}
	}
	return false;
}

Vector3 SubVoxelSidePoints::get_vector3_for_point(int index) {
	Vector3i a = _points[index];

	Vector3 b(a.x, a.y, a.z);

	return b;
}

Vector3 SubVoxelSidePoints::get_vertex_vector3_for_point(int index) {
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

void SubVoxelSidePoints::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_point", "index"), &SubVoxelSidePoints::get_point_bind);
	ClassDB::bind_method(D_METHOD("set_point", "index", "point"), &SubVoxelSidePoints::set_point_bind);
	ClassDB::bind_method(D_METHOD("get_point_count"), &SubVoxelSidePoints::get_point_count);

	ClassDB::bind_method(D_METHOD("is_sub_voxel_point", "x", "y", "z"), &SubVoxelSidePoints::is_sub_voxel_point);
	ClassDB::bind_method(D_METHOD("get_vector3_for_point", "index"), &SubVoxelSidePoints::get_vector3_for_point);
	ClassDB::bind_method(D_METHOD("get_vertex_vector3_for_point", "index"), &SubVoxelSidePoints::get_vertex_vector3_for_point);
}
