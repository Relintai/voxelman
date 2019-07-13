#ifndef VOXEL_H
#define VOXEL_H

#include "core/reference.h"
#include "../collections/vector3i.h"
#include "core/math/quat.h"
#include "core/resource.h"
#include "core/vector.h"
#include "sub_voxel_points.h"
#include "voxel_data.h"
#include "voxel_enums.h"
#include "voxel_face.h"

#include "../library/voxel_surface_definition.h"

using namespace voxelman;

namespace voxelman {

class SubVoxelPoints;

class Voxel : public Reference {
	GDCLASS(Voxel, Reference);

public:
	enum VoxelNeighbours {
		NEIGHBOUR_NONE = 0,

		NEIGHBOUR_LEFT = 1 << 0,
		NEIGHBOUR_RIGHT = 1 << 1,
		NEIGHBOUR_FRONT = 1 << 2,
		NEIGHBOUR_BACK = 1 << 3,
		NEIGHBOUR_TOP = 1 << 4,
		NEIGHBOUR_DOWN = 1 << 5,

		NEIGHBOUR_LEFT_FRONT = 1 << 6,
		NEIGHBOUR_LEFT_BACK = 1 << 7,
		NEIGHBOUR_RIGHT_FRONT = 1 << 8,
		NEIGHBOUR_RIGHT_BACK = 1 << 9,

		NEIGHBOUR_TOP_LEFT = 1 << 10,
		NEIGHBOUR_TOP_RIGHT = 1 << 11,
		NEIGHBOUR_TOP_FRONT = 1 << 12,
		NEIGHBOUR_TOP_BACK = 1 << 13,

		NEIGHBOUR_BOTTOM_LEFT = 1 << 14,
		NEIGHBOUR_BOTTOM_RIGHT = 1 << 15,
		NEIGHBOUR_BOTTOM_FRONT = 1 << 16,
		NEIGHBOUR_BOTTOM_BACK = 1 << 17,

		NEIGHBOUR_BOTTOM_LEFT_FRONT = 1 << 18,
		NEIGHBOUR_BOTTOM_LEFT_BACK = 1 << 19,
		NEIGHBOUR_BOTTOM_RIGHT_FRONT = 1 << 20,
		NEIGHBOUR_BOTTOM_RIGHT_BACK = 1 << 21,

		NEIGHBOUR_TOP_LEFT_FRONT = 1 << 22,
		NEIGHBOUR_TOP_LEFT_BACK = 1 << 23,
		NEIGHBOUR_TOP_RIGHT_FRONT = 1 << 24,
		NEIGHBOUR_TOP_RIGHT_BACK = 1 << 25,

		
		FULL_NEIGHBOURS_CROSS = NEIGHBOUR_LEFT | NEIGHBOUR_RIGHT | NEIGHBOUR_BACK | NEIGHBOUR_FRONT | NEIGHBOUR_TOP | NEIGHBOUR_DOWN, // | NEIGHBOUR_LEFT_BACK | NEIGHBOUR_LEFT_FRONT | NEIGHBOUR_RIGHT_BACK | NEIGHBOUR_RIGHT_FRONT;
		FULL_SIDE_NEIGHBOURS = NEIGHBOUR_LEFT | NEIGHBOUR_RIGHT | NEIGHBOUR_BACK | NEIGHBOUR_FRONT,
		FULL_SIDE_NEIGHBOURS_TOP = NEIGHBOUR_LEFT | NEIGHBOUR_RIGHT | NEIGHBOUR_BACK | NEIGHBOUR_FRONT | NEIGHBOUR_TOP,
		FULL_SIDE_NEIGHBOURS_DOWN = NEIGHBOUR_LEFT | NEIGHBOUR_RIGHT | NEIGHBOUR_BACK | NEIGHBOUR_FRONT | NEIGHBOUR_DOWN,
	};

	static const unsigned int FACE_COUNT = 6;

	int get_neighbours();
	void set_neighbours(int value);

	bool get_has_sub_voxel_points();
	SubVoxelPoints *get_sub_voxel_points();

	void set_voxel_datas(Ref<VoxelData> VP000, Ref<VoxelData> VP100, Ref<VoxelData> VP010, Ref<VoxelData> VP001, Ref<VoxelData> VP110, Ref<VoxelData> VP011, Ref<VoxelData> VP101, Ref<VoxelData> VP111);
	void refresh_sub_voxel_points();

protected:
	static void _bind_methods() {}

private:
	int _neighbours;
	SubVoxelPoints *_sub_voxel_points;
};

} // namespace voxelman

#endif
