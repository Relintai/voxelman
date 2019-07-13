#include "sub_voxel_points_helper.h"

#if MEM_TOOLS
int SubVoxelFacePointsHelper::allocs = 0;
#endif

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

SubVoxelFacePointsHelper::SubVoxelFacePointsHelper(int face, SubVoxelPoints *points) {
	_size = 255;
	set_sub_voxel_points(face, points);

#if MEM_TOOLS
	SubVoxelFacePointsHelper::allocs++;
	print_error("SubVoxelFacePointsHelper alloc " + String::num(SubVoxelFacePointsHelper::allocs));
#endif
}

SubVoxelFacePointsHelper::~SubVoxelFacePointsHelper() {
#if MEM_TOOLS
	SubVoxelFacePointsHelper::allocs--;
	print_error("SubVoxelFacePointsHelper dealloc " + String::num(SubVoxelFacePointsHelper::allocs));
#endif
}

void SubVoxelFacePointsHelper::set_sub_voxel_points(int face, SubVoxelPoints *points) {
	_face = face;
	_tl = Vector3i(SubVoxelPoints::get_top_left_point(face, points));
	_tr = Vector3i(SubVoxelPoints::get_top_right_point(face, points));
	_bl = Vector3i(SubVoxelPoints::get_bottom_left_point(face, points));
	_br = Vector3i(SubVoxelPoints::get_bottom_right_point(face, points));
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
	if (_face == VoxelFace::VOXEL_FACE_FRONT) {
		if (gettl_depth() == 0 && gettr_depth() == 0 && getbl_depth() == 0 && getbr_depth() == 0) {
			return true;
		}
	} else if (_face == VoxelFace::VOXEL_FACE_BACK) {
		if (gettl_depth() == 255 && gettr_depth() == 255 && getbl_depth() == 255 && getbr_depth() == 255) {
			return true;
		}
	} else if (_face == VoxelFace::VOXEL_FACE_RIGHT) {
		if (gettl_depth() == 255 && gettr_depth() == 255 && getbl_depth() == 255 && getbr_depth() == 255) {
			return true;
		}
	} else if (_face == VoxelFace::VOXEL_FACE_LEFT) {
		if (gettl_depth() == 0 && gettr_depth() == 0 && getbl_depth() == 0 && getbr_depth() == 0) {
			return true;
		}
	} else if (_face == VoxelFace::VOXEL_FACE_TOP) {
		if (gettl_depth() == 255 && gettr_depth() == 255 && getbl_depth() == 255 && getbr_depth() == 255) {
			return true;
		}
	} else if (_face == VoxelFace::VOXEL_FACE_DOWN) {
		if (gettl_depth() == 0 && gettr_depth() == 0 && getbl_depth() == 0 && getbr_depth() == 0) {
			return true;
		}
	}

	return false;
}

bool SubVoxelFacePointsHelper::is_face_visible_against(SubVoxelFacePointsHelper *other) {
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

bool SubVoxelFacePointsHelper::is_face_visible_against(SubVoxelFacePointsHelper other) {
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

int SubVoxelFacePointsHelper::get_depth(int face, Vector3i v3) {
	if (face == VoxelFace::VOXEL_FACE_BACK) {
		return v3.z;
	}
	if (face == VoxelFace::VOXEL_FACE_FRONT) {
		return v3.z;
	}
	if (face == VoxelFace::VOXEL_FACE_RIGHT) {
		return v3.x;
	}
	if (face == VoxelFace::VOXEL_FACE_LEFT) {
		return v3.x;
	}
	if (face == VoxelFace::VOXEL_FACE_TOP) {
		return v3.y;
	}
	if (face == VoxelFace::VOXEL_FACE_DOWN) {
		return v3.y;
	}
	return 0;
}

Vector2i SubVoxelFacePointsHelper::transform_to_vector2i(int face, Vector3i v3) {
	Vector2i result;
	if (face == VoxelFace::VOXEL_FACE_BACK) {
		result = Vector2i(v3.x, v3.y);
	} else if (face == VoxelFace::VOXEL_FACE_FRONT) {
		result = Vector2i(v3.x, v3.y);
	} else if (face == VoxelFace::VOXEL_FACE_RIGHT) {
		result = Vector2i(v3.y, v3.z);
	} else if (face == VoxelFace::VOXEL_FACE_LEFT) {
		result = Vector2i(v3.y, v3.z);
	} else if (face == VoxelFace::VOXEL_FACE_TOP) {
		result = Vector2i(v3.x, v3.z);
	} else if (face == VoxelFace::VOXEL_FACE_DOWN) {
		result = Vector2i(v3.x, v3.z);
	}

	return result;
}

bool SubVoxelFacePointsHelper::equals(SubVoxelFacePointsHelper *other) {
	return ((((_tl == other->_tl) && (_tr == other->_tr)) && (_bl == other->_bl)) && (_br == other->_br)) && (_face == other->_face);
}

int SubVoxelFacePointsHelper::get_hash_code() {
	//return ((((_tl->get_hash_code() / 100000) + (_tr->get_hash_code() / 100000)) + (_bl->get_hash_code() / 100000)) + (_br->get_hash_code() / 100000)) + _face;

	return 123;
}
