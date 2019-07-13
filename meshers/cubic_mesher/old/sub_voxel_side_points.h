#ifndef SUB_VOXEL_SIDE_POINTS_H
#define SUB_VOXEL_SIDE_POINTS_H

#include "core/object.h"
#include "../collections/vector3i.h"
#include "core/vector.h"

namespace voxelman {

class SubVoxelSidePoints {

public:
	Vector3i get_point(int index) { return _points[index]; }
	void set_point(int index, Vector3i point) { _points.set(index, point); }
	int get_point_count() { return _points.size(); }

	bool is_sub_voxel_point(Vector3i point);
	bool is_sub_voxel_point(int x, int y, int z);
	Vector3 get_vector3_for_point(int pointIndex);
	Vector3 get_vertex_vector3_for_point(int pointIndex);
	SubVoxelSidePoints();
	~SubVoxelSidePoints();

private:
	Vector<Vector3i> _points;

	#if MEM_TOOLS
	static int allocs;
	#endif
};

} // namespace voxelman

#endif
