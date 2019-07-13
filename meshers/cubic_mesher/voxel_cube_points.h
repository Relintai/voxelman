#ifndef SUB_VOXEL_POINTS_H
#define SUB_VOXEL_POINTS_H

#include "../collections/vector3i.h"
#include "core/vector.h"
#include "sub_voxel_points_helper.h"
#include "sub_voxel_side_points.h"
#include "voxel.h"
#include "voxel_data.h"

using namespace voxelman;

namespace voxelman {

class Voxel;
class VoxelData;

class SubVoxelPoints {
public:
	enum SidePoints {
		P000 = 0,
		P100 = 1,
		P010 = 2,
		P001 = 3,
		P110 = 4,
		P011 = 5,
		P101 = 6,
		P111 = 7,
	};

	enum Points {
		POINT_COUNT = 8,
	};

	Vector3i get_point(int index);
	SubVoxelPoints(Voxel *voxel);
	~SubVoxelPoints();

	void refresh_points();
	void set_voxel_datas(Ref<VoxelData> VP000, Ref<VoxelData> VP100, Ref<VoxelData> VP010, Ref<VoxelData> VP001, Ref<VoxelData> VP110, Ref<VoxelData> VP011, Ref<VoxelData> VP101, Ref<VoxelData> VP111);
	SubVoxelSidePoints get_points_for_face(int face);
	bool face_fully_covered(int face);
	bool face_should_be_visible_against_full(int face);
	bool face_should_be_visible_against(int face, SubVoxelPoints *other);
	bool is_sub_voxel_point(Vector3i point);
	bool is_sub_voxel_point(int x, int y, int z);
	void set_point(int point, int x, int y, int z);
	int get_point_id(Vector3i point);
	int get_point_id(int x, int y, int z);
	void copy_values_from(SubVoxelPoints *other);
	static Vector3i get_top_left_point(int face, SubVoxelPoints *points);
	static Vector3i get_top_right_point(int face, SubVoxelPoints *points);
	static Vector3i get_bottom_left_point(int face, SubVoxelPoints *points);
	static Vector3i get_bottom_right_point(int face, SubVoxelPoints *points);
	static bool validate_voxel_datas(Ref<VoxelData> VP000, Ref<VoxelData> VP100, Ref<VoxelData> VP010, Ref<VoxelData> VP001, Ref<VoxelData> VP110, Ref<VoxelData> VP011, Ref<VoxelData> VP101, Ref<VoxelData> VP111);
	int get_opposite_face(int face);

private:
	Vector3i _points[POINT_COUNT];
	Ref<VoxelData> *_voxel_datas;
	Voxel *_voxel;
	SubVoxelSidePoints _face_helper;
	void refresh_point(int index, int vectx, int vecty, int vectz, int axisx, int axis2x, int axis3x, int axis4notx, int axisy, int axis2y, int axis3y, int axis4y, int axisz, int axis2z, int axis3z, int axis4notz);
	void refresh_pointo(int index, int vectx, int vecty, int vectz, int axisx, int axis2x, int axis3x, int axis4notx, int axisy, int axis2y, int axis3y, int axis4y, int axisz, int axis2z, int axis3z, int axis4notz);
	void refresh_point_full(int index, int vectx, int vecty, int vectz, int axisx, int axis2x, int axis3x, int axisy, int axis2y, int axis3y, int axis4y, int axisz, int axis2z, int axis3z);

#if MEM_TOOLS
	static int allocs;
#endif
};

} // namespace voxelman

#endif
