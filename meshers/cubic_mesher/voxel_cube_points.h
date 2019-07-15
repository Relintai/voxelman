#ifndef SUB_VOXEL_POINTS_H
#define SUB_VOXEL_POINTS_H

#include "../../math/vector3i.h"
#include "../../world/voxel_buffer.h"
#include "core/reference.h"
#include "core/vector.h"

class SubVoxelFacePointsHelper;

class VoxelCubePoints : public Reference {
	GDCLASS(VoxelCubePoints, Reference);

public:
	enum Points {
		P000 = 0,
		P100 = 1,
		P010 = 2,
		P001 = 3,

		P110 = 4,
		P011 = 5,
		P101 = 6,
		P111 = 7,

		POINT_COUNT = 8,
	};

	enum VoxelFaces {
		VOXEL_FACE_FRONT = 0,
		VOXEL_FACE_RIGHT = 1,
		VOXEL_FACE_BACK = 2,
		VOXEL_FACE_LEFT = 3,
		VOXEL_FACE_TOP = 4,
		VOXEL_FACE_BOTTOM = 5,
		VOXEL_FACE_COUNT = 6,
	};

	enum VoxelNeighbours {
		VOXEL_NEIGHBOUR_NONE = 0,

		VOXEL_NEIGHBOUR_LEFT = 1 << 0,
		VOXEL_NEIGHBOUR_RIGHT = 1 << 1,
		VOXEL_NEIGHBOUR_FRONT = 1 << 2,
		VOXEL_NEIGHBOUR_BACK = 1 << 3,
		VOXEL_NEIGHBOUR_TOP = 1 << 4,
		VOXEL_NEIGHBOUR_BOTTOM = 1 << 5,

		VOXEL_NEIGHBOUR_LEFT_FRONT = 1 << 6,
		VOXEL_NEIGHBOUR_LEFT_BACK = 1 << 7,
		VOXEL_NEIGHBOUR_RIGHT_FRONT = 1 << 8,
		VOXEL_NEIGHBOUR_RIGHT_BACK = 1 << 9,

		VOXEL_NEIGHBOUR_TOP_LEFT = 1 << 10,
		VOXEL_NEIGHBOUR_TOP_RIGHT = 1 << 11,
		VOXEL_NEIGHBOUR_TOP_FRONT = 1 << 12,
		VOXEL_NEIGHBOUR_TOP_BACK = 1 << 13,

		VOXEL_NEIGHBOUR_BOTTOM_LEFT = 1 << 14,
		VOXEL_NEIGHBOUR_BOTTOM_RIGHT = 1 << 15,
		VOXEL_NEIGHBOUR_BOTTOM_FRONT = 1 << 16,
		VOXEL_NEIGHBOUR_BOTTOM_BACK = 1 << 17,

		VOXEL_NEIGHBOUR_BOTTOM_LEFT_FRONT = 1 << 18,
		VOXEL_NEIGHBOUR_BOTTOM_LEFT_BACK = 1 << 19,
		VOXEL_NEIGHBOUR_BOTTOM_RIGHT_FRONT = 1 << 20,
		VOXEL_NEIGHBOUR_BOTTOM_RIGHT_BACK = 1 << 21,

		VOXEL_NEIGHBOUR_TOP_LEFT_FRONT = 1 << 22,
		VOXEL_NEIGHBOUR_TOP_LEFT_BACK = 1 << 23,
		VOXEL_NEIGHBOUR_TOP_RIGHT_FRONT = 1 << 24,
		VOXEL_NEIGHBOUR_TOP_RIGHT_BACK = 1 << 25,

		VOXEL_FULL_NEIGHBOURS_CROSS = VOXEL_NEIGHBOUR_LEFT | VOXEL_NEIGHBOUR_RIGHT | VOXEL_NEIGHBOUR_BACK | VOXEL_NEIGHBOUR_FRONT | VOXEL_NEIGHBOUR_TOP | VOXEL_NEIGHBOUR_BOTTOM, // | VOXEL_NEIGHBOUR_LEFT_BACK | VOXEL_NEIGHBOUR_LEFT_FRONT | VOXEL_NEIGHBOUR_RIGHT_BACK | VOXEL_NEIGHBOUR_RIGHT_FRONT;
		VOXEL_FULL_SIDE_NEIGHBOURS = VOXEL_NEIGHBOUR_LEFT | VOXEL_NEIGHBOUR_RIGHT | VOXEL_NEIGHBOUR_BACK | VOXEL_NEIGHBOUR_FRONT,
		VOXEL_FULL_SIDE_NEIGHBOURS_TOP = VOXEL_NEIGHBOUR_LEFT | VOXEL_NEIGHBOUR_RIGHT | VOXEL_NEIGHBOUR_BACK | VOXEL_NEIGHBOUR_FRONT | VOXEL_NEIGHBOUR_TOP,
		VOXEL_FULL_SIDE_NEIGHBOURS_DOWN = VOXEL_NEIGHBOUR_LEFT | VOXEL_NEIGHBOUR_RIGHT | VOXEL_NEIGHBOUR_BACK | VOXEL_NEIGHBOUR_FRONT | VOXEL_NEIGHBOUR_BOTTOM,
	};

	int get_x();
	void set_x(int value);

	int get_y();
	void set_y(int value);

	int get_z();
	void set_z(int value);

	int get_size();
	void set_size(int value);

	void refresh_points();
	void recalculate_point(int point);
	void refresh_neighbours(const Ref<VoxelBuffer> buffer);
	void setup(const Ref<VoxelBuffer> buffer, int x, int y, int z, int size = 1);

	void reset();

	int get_point_index(int face, int index);
	Vector3 get_points_for_face(int face, int index);
	bool is_face_visible(int face);
	bool is_sub_voxel_point_vec(Vector3i point);
	bool is_sub_voxel_point(int x, int y, int z);
	void set_point(int point, int x, int y, int z);
	int get_point_id_vec(Vector3i point);
	int get_point_id(int x, int y, int z);

	Vector3 get_point_for_face(int face, int pointIndex);
	Vector3 get_vertex_vector3_for_point(int face, int pointIndex);

	Vector3 get_point(int index);
	Vector3 get_top_left_point(int face);
	Vector3 get_top_right_point(int face);
	Vector3 get_bottom_left_point(int face);
	Vector3 get_bottom_right_point(int face);

	bool has_points();
	int get_opposite_face(int face);

	VoxelCubePoints();
	~VoxelCubePoints();

protected:
	static void _bind_methods();

	void refresh_point(int index, int vectx, int vecty, int vectz, int axisx, int axis2x, int axis3x, int axis4notx, int axisy, int axis2y, int axis3y, int axis4y, int axisz, int axis2z, int axis3z, int axis4notz);

private:
	static const unsigned int index_table[6][4];
	static const unsigned int visibility_check_table[6];
	static const float point_direction_table[8][3];
	static const unsigned int point_direction_neighbour_table[8][3];

	Vector3 _points[POINT_COUNT];

	uint8_t _point_types[POINT_COUNT];
	uint8_t _point_fills[POINT_COUNT];
	unsigned int _point_neighbours[POINT_COUNT];

	int _size;
	int _x;
	int _y;
	int _z;
};

VARIANT_ENUM_CAST(VoxelCubePoints::Points);
VARIANT_ENUM_CAST(VoxelCubePoints::VoxelFaces);
VARIANT_ENUM_CAST(VoxelCubePoints::VoxelNeighbours);

#endif
