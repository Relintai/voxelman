#ifndef SUB_VOXEL_FACE_POINTS_HELPER_H
#define SUB_VOXEL_FACE_POINTS_HELPER_H

#include "core/reference.h"
#include "../../math/vector3i.h"
#include "core/math/vector2.h"
#include "core/vector.h"

class VoxelCubePoints;

class SubVoxelFacePointsHelper : public Reference {
	GDCLASS(SubVoxelFacePointsHelper, Reference);

public:
	Vector2i gettlv2();
	Vector2i gettrv2();
	Vector2i getblv2();
	Vector2i getbrv2();

	Vector2 gettlv2_bind();
	Vector2 gettrv2_bind();
	Vector2 getblv2_bind();
	Vector2 getbrv2_bind();

	int gettl_depth();
	int gettr_depth();
	int getbl_depth();
	int getbr_depth();

	void set_sub_voxel_points(int face, Ref<VoxelCubePoints> points);
	bool is_face_fully_covered();
	bool is_face_near_the_edges();
	bool is_face_visible_against(Ref<SubVoxelFacePointsHelper> other);
	//bool is_face_visible_against(Ref<SubVoxelFacePointsHelper> other);
	int get_depth(int face, Vector3i v3);
	int get_depth_bind(int face, Vector3 v3);
	Vector2i transform_to_vector2i(int face, Vector3i v3);
	Vector2 transform_to_vector2(int face, Vector3 v3);

	bool equals(Ref<SubVoxelFacePointsHelper> other);

	int get_hash_code();

	SubVoxelFacePointsHelper();
	~SubVoxelFacePointsHelper();

protected:
	static void _bind_methods();

private:
	int _face;

	Vector3i _tl;
	Vector3i _tr;
	Vector3i _bl;
	Vector3i _br;
};

#endif
