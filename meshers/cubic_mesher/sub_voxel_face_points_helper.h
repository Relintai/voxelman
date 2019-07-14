#ifndef SUB_VOXEL_FACE_POINTS_HELPER_H
#define SUB_VOXEL_FACE_POINTS_HELPER_H

#include "../../math/vector3i.h"
#include "core/math/vector2.h"
#include "core/vector.h"
#include "voxel_cube_points.h"


class VoxelCubePoints;

class SubVoxelFacePointsHelper {

public:
	Vector2i gettlv2();
	Vector2i gettrv2();
	Vector2i getblv2();
	Vector2i getbrv2();

	int gettl_depth();
	int gettr_depth();
	int getbl_depth();
	int getbr_depth();

	SubVoxelFacePointsHelper(int face, VoxelCubePoints *points);
	~SubVoxelFacePointsHelper();

	void set_sub_voxel_points(int face, VoxelCubePoints *points);
	bool is_face_fully_covered();
	bool is_face_near_the_edges();
	bool is_face_visible_against(SubVoxelFacePointsHelper *other);
	bool is_face_visible_against(SubVoxelFacePointsHelper other);
	static int get_depth(int face, Vector3i v3);
	static Vector2i transform_to_vector2i(int face, Vector3i v3);

	bool equals(SubVoxelFacePointsHelper *other);

	int get_hash_code();

private:
	int _size;
	int _face;

	Vector3i _tl;
	Vector3i _tr;
	Vector3i _bl;
	Vector3i _br;
};

#endif
