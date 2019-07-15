#ifndef SUB_VOXEL_SIDE_POINTS_H
#define SUB_VOXEL_SIDE_POINTS_H

#include "../../math/vector3i.h"
#include "core/reference.h"
#include "core/vector.h"

class SubVoxelSidePoints : public Reference {
	GDCLASS(SubVoxelSidePoints, Reference);

public:
	Vector3i get_point(int index) { return _points[index]; }
	Vector3 get_point_bind(int index) { return _points[index].to_vec3(); }
	void set_point(int index, Vector3i point) { _points[index] = point; }
	void set_point_bind(int index, Vector3 point) { _points[index] = point; }
	int get_point_count() { return 4; }

	bool is_sub_voxel_point_vec(Vector3i point);
	bool is_sub_voxel_point(int x, int y, int z);
	Vector3 get_vector3_for_point(int pointIndex);
	Vector3 get_vertex_vector3_for_point(int pointIndex);

	SubVoxelSidePoints();
	~SubVoxelSidePoints();

protected:
	static void _bind_methods();

private:
	
	Vector3i _points[4];
};

#endif
