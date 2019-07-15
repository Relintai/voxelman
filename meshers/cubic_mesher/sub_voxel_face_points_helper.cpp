#include "sub_voxel_face_points_helper.h"

#include "voxel_cube_points.h"

Vector2i SubVoxelFacePointsHelper::gettlv2() {
	return SubVoxelFacePointsHelper::transform_to_vector2i(_face, _tl);
}
Vector2i SubVoxelFacePointsHelper::gettrv2() {
	return SubVoxelFacePointsHelper::transform_to_vector2i(_face, _tr);
}
Vector2i SubVoxelFacePointsHelper::getblv2() {
	return SubVoxelFacePointsHelper::transform_to_vector2i(_face, _bl);
}
Vector2i SubVoxelFacePointsHelper::getbrv2() {
	return SubVoxelFacePointsHelper::transform_to_vector2i(_face, _br);
}


Vector2 SubVoxelFacePointsHelper::gettlv2_bind() {
	return SubVoxelFacePointsHelper::transform_to_vector2(_face, _tl.to_vec3());
}
Vector2 SubVoxelFacePointsHelper::gettrv2_bind() {
	return SubVoxelFacePointsHelper::transform_to_vector2(_face, _tr.to_vec3());
}
Vector2 SubVoxelFacePointsHelper::getblv2_bind() {
	return SubVoxelFacePointsHelper::transform_to_vector2(_face, _bl.to_vec3());
}
Vector2 SubVoxelFacePointsHelper::getbrv2_bind() {
	return SubVoxelFacePointsHelper::transform_to_vector2(_face, _br.to_vec3());
}


int SubVoxelFacePointsHelper::gettl_depth() {
	return SubVoxelFacePointsHelper::get_depth(_face, _tl);
}

int SubVoxelFacePointsHelper::gettr_depth() {
	return SubVoxelFacePointsHelper::get_depth(_face, _tr);
}

int SubVoxelFacePointsHelper::getbl_depth() {
	return SubVoxelFacePointsHelper::get_depth(_face, _bl);
}

int SubVoxelFacePointsHelper::getbr_depth() {
	return SubVoxelFacePointsHelper::get_depth(_face, _br);
}

SubVoxelFacePointsHelper::SubVoxelFacePointsHelper() {
}

SubVoxelFacePointsHelper::~SubVoxelFacePointsHelper() {
}

void SubVoxelFacePointsHelper::set_sub_voxel_points(int face, Ref<VoxelCubePoints> points) {
	_face = face;
	_tl = Vector3i(points->get_top_left_point(face));
	_tr = Vector3i(points->get_top_right_point(face));
	_bl = Vector3i(points->get_bottom_left_point(face));
	_br = Vector3i(points->get_bottom_right_point(face));
}

bool SubVoxelFacePointsHelper::is_face_fully_covered() {
	if (!is_face_near_the_edges()) {
		return false;
	}

	Vector2i tlv = gettlv2();
	if ((tlv.x != 0) || (tlv.y != 255)) {
		return false;
	}

	Vector2i trv = gettrv2();
	if ((trv.x != 255) || (trv.y != 255)) {
		return false;
	}

	Vector2i blv = getblv2();
	if ((blv.x != 0) || (blv.y != 0)) {
		return false;
	}

	Vector2i brv = getbrv2();
	return (brv.x == 255) && (brv.y == 0);
}

bool SubVoxelFacePointsHelper::is_face_near_the_edges() {
	if (_face == VoxelCubePoints::VOXEL_FACE_FRONT) {
		if (gettl_depth() == 0 && gettr_depth() == 0 && getbl_depth() == 0 && getbr_depth() == 0) {
			return true;
		}
	} else if (_face == VoxelCubePoints::VOXEL_FACE_BACK) {
		if (gettl_depth() == 255 && gettr_depth() == 255 && getbl_depth() == 255 && getbr_depth() == 255) {
			return true;
		}
	} else if (_face == VoxelCubePoints::VOXEL_FACE_RIGHT) {
		if (gettl_depth() == 255 && gettr_depth() == 255 && getbl_depth() == 255 && getbr_depth() == 255) {
			return true;
		}
	} else if (_face == VoxelCubePoints::VOXEL_FACE_LEFT) {
		if (gettl_depth() == 0 && gettr_depth() == 0 && getbl_depth() == 0 && getbr_depth() == 0) {
			return true;
		}
	} else if (_face == VoxelCubePoints::VOXEL_FACE_TOP) {
		if (gettl_depth() == 255 && gettr_depth() == 255 && getbl_depth() == 255 && getbr_depth() == 255) {
			return true;
		}
	} else if (_face == VoxelCubePoints::VOXEL_FACE_BOTTOM) {
		if (gettl_depth() == 0 && gettr_depth() == 0 && getbl_depth() == 0 && getbr_depth() == 0) {
			return true;
		}
	}

	return false;
}

bool SubVoxelFacePointsHelper::is_face_visible_against(Ref<SubVoxelFacePointsHelper> other) {
	if (!is_face_near_the_edges() || !other->is_face_near_the_edges()) {
		return true;
	}

	Vector2i tlv = gettlv2();
	Vector2i tlv2 = other->gettlv2();

	if ((tlv.x < tlv2.x) || (tlv.y > tlv2.y)) {
		return true;
	}

	Vector2i trv = gettrv2();
	Vector2i trv2 = other->gettrv2();

	if ((trv.x > trv2.x) || (trv.y > trv2.y)) {
		return true;
	}

	Vector2i blv = getblv2();
	Vector2i blv2 = other->getblv2();

	if ((blv.x < blv2.x) || (blv.y < blv2.y)) {
		return true;
	}

	Vector2i brv = getbrv2();
	Vector2i brv2 = other->getbrv2();

	return (brv.x > brv2.x) || (brv.y < brv2.y);
}
/*
bool SubVoxelFacePointsHelper::is_face_visible_against(Ref<SubVoxelFacePointsHelper> other) {
	if (!is_face_near_the_edges() || !other.is_face_near_the_edges()) {
		return true;
	}

	Vector2i tlv = gettlv2();
	Vector2i tlv2 = other.gettlv2();

	if ((tlv.x < tlv2.x) || (tlv.y > tlv2.y)) {
		return true;
	}

	Vector2i trv = gettrv2();
	Vector2i trv2 = other.gettrv2();

	if ((trv.x > trv2.x) || (trv.y > trv2.y)) {
		return true;
	}

	Vector2i blv = getblv2();
	Vector2i blv2 = other.getblv2();

	if ((blv.x < blv2.x) || (blv.y < blv2.y)) {
		return true;
	}

	Vector2i brv = getbrv2();
	Vector2i brv2 = other.getbrv2();

	return (brv.x > brv2.x) || (brv.y < brv2.y);
}
*/
int SubVoxelFacePointsHelper::get_depth(int face, Vector3i v3) {
	if (face == VoxelCubePoints::VOXEL_FACE_BACK) {
		return v3.z;
	}
	if (face == VoxelCubePoints::VOXEL_FACE_FRONT) {
		return v3.z;
	}
	if (face == VoxelCubePoints::VOXEL_FACE_RIGHT) {
		return v3.x;
	}
	if (face == VoxelCubePoints::VOXEL_FACE_LEFT) {
		return v3.x;
	}
	if (face == VoxelCubePoints::VOXEL_FACE_TOP) {
		return v3.y;
	}
	if (face == VoxelCubePoints::VOXEL_FACE_BOTTOM) {
		return v3.y;
	}
	return 0;
}

int SubVoxelFacePointsHelper::get_depth_bind(int face, Vector3 v3) {
	if (face == VoxelCubePoints::VOXEL_FACE_BACK) {
		return v3.z;
	}
	if (face == VoxelCubePoints::VOXEL_FACE_FRONT) {
		return v3.z;
	}
	if (face == VoxelCubePoints::VOXEL_FACE_RIGHT) {
		return v3.x;
	}
	if (face == VoxelCubePoints::VOXEL_FACE_LEFT) {
		return v3.x;
	}
	if (face == VoxelCubePoints::VOXEL_FACE_TOP) {
		return v3.y;
	}
	if (face == VoxelCubePoints::VOXEL_FACE_BOTTOM) {
		return v3.y;
	}
	return 0;
}

Vector2i SubVoxelFacePointsHelper::transform_to_vector2i(int face, Vector3i v3) {
	Vector2i result;
	if (face == VoxelCubePoints::VOXEL_FACE_BACK) {
		result = Vector2i(v3.x, v3.y);
	} else if (face == VoxelCubePoints::VOXEL_FACE_FRONT) {
		result = Vector2i(v3.x, v3.y);
	} else if (face == VoxelCubePoints::VOXEL_FACE_RIGHT) {
		result = Vector2i(v3.y, v3.z);
	} else if (face == VoxelCubePoints::VOXEL_FACE_LEFT) {
		result = Vector2i(v3.y, v3.z);
	} else if (face == VoxelCubePoints::VOXEL_FACE_TOP) {
		result = Vector2i(v3.x, v3.z);
	} else if (face == VoxelCubePoints::VOXEL_FACE_BOTTOM) {
		result = Vector2i(v3.x, v3.z);
	}

	return result;
}

Vector2 SubVoxelFacePointsHelper::transform_to_vector2(int face, Vector3 v3) {
	Vector2 result;
	if (face == VoxelCubePoints::VOXEL_FACE_BACK) {
		result = Vector2(v3.x, v3.y);
	} else if (face == VoxelCubePoints::VOXEL_FACE_FRONT) {
		result = Vector2(v3.x, v3.y);
	} else if (face == VoxelCubePoints::VOXEL_FACE_RIGHT) {
		result = Vector2(v3.y, v3.z);
	} else if (face == VoxelCubePoints::VOXEL_FACE_LEFT) {
		result = Vector2(v3.y, v3.z);
	} else if (face == VoxelCubePoints::VOXEL_FACE_TOP) {
		result = Vector2(v3.x, v3.z);
	} else if (face == VoxelCubePoints::VOXEL_FACE_BOTTOM) {
		result = Vector2(v3.x, v3.z);
	}

	return result;
}

bool SubVoxelFacePointsHelper::equals(Ref<SubVoxelFacePointsHelper> other) {
	return ((((_tl == other->_tl) && (_tr == other->_tr)) && (_bl == other->_bl)) && (_br == other->_br)) && (_face == other->_face);
}

int SubVoxelFacePointsHelper::get_hash_code() {
	//return ((((_tl->get_hash_code() / 100000) + (_tr->get_hash_code() / 100000)) + (_bl->get_hash_code() / 100000)) + (_br->get_hash_code() / 100000)) + _face;

	return 123;
}

void SubVoxelFacePointsHelper::_bind_methods() {
	ClassDB::bind_method(D_METHOD("gettlv2"), &SubVoxelFacePointsHelper::gettlv2_bind);
	ClassDB::bind_method(D_METHOD("gettrv2"), &SubVoxelFacePointsHelper::gettrv2_bind);
	ClassDB::bind_method(D_METHOD("getblv2"), &SubVoxelFacePointsHelper::getblv2_bind);
	ClassDB::bind_method(D_METHOD("getbrv2"), &SubVoxelFacePointsHelper::getbrv2_bind);

	ClassDB::bind_method(D_METHOD("gettl_depth"), &SubVoxelFacePointsHelper::gettl_depth);
	ClassDB::bind_method(D_METHOD("gettr_depth"), &SubVoxelFacePointsHelper::gettr_depth);
	ClassDB::bind_method(D_METHOD("getbl_depth"), &SubVoxelFacePointsHelper::getbl_depth);
	ClassDB::bind_method(D_METHOD("getbr_depth"), &SubVoxelFacePointsHelper::getbr_depth);

	ClassDB::bind_method(D_METHOD("set_sub_voxel_points", "face", "points"), &SubVoxelFacePointsHelper::set_sub_voxel_points);
	ClassDB::bind_method(D_METHOD("is_face_fully_covered"), &SubVoxelFacePointsHelper::is_face_fully_covered);

	ClassDB::bind_method(D_METHOD("is_face_near_the_edges"), &SubVoxelFacePointsHelper::is_face_near_the_edges);
	ClassDB::bind_method(D_METHOD("is_face_visible_against", "other"), &SubVoxelFacePointsHelper::is_face_visible_against);

	ClassDB::bind_method(D_METHOD("get_depth", "face", "v3"), &SubVoxelFacePointsHelper::get_depth_bind);
	ClassDB::bind_method(D_METHOD("transform_to_vector2"), &SubVoxelFacePointsHelper::transform_to_vector2);

}
