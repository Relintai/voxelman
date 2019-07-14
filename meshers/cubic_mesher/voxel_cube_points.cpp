#include "voxel_cube_points.h"

Vector3i VoxelCubePoints::get_point(int index) {
	return _points[index];
}

VoxelCubePoints::VoxelCubePoints() {
	_face_helper.instance();
}

VoxelCubePoints::~VoxelCubePoints() {
	_face_helper.unref();
}

void VoxelCubePoints::refresh_points() {
	//Front
	//Bottom Left
	refresh_point(P000, 0, 0, 0,
			VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_BOTTOM_LEFT, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_FRONT,
			VOXEL_NEIGHBOUR_BOTTOM, VOXEL_NEIGHBOUR_BOTTOM_LEFT, VOXEL_NEIGHBOUR_BOTTOM_FRONT, VOXEL_NEIGHBOUR_BOTTOM_LEFT_FRONT,
			VOXEL_NEIGHBOUR_FRONT, VOXEL_NEIGHBOUR_BOTTOM_FRONT, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_FRONT);

	//Bottom Right
	refresh_point(P100, 255, 0, 0,
			VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_BOTTOM_RIGHT, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_FRONT,
			VOXEL_NEIGHBOUR_BOTTOM, VOXEL_NEIGHBOUR_BOTTOM_RIGHT, VOXEL_NEIGHBOUR_BOTTOM_FRONT, VOXEL_NEIGHBOUR_BOTTOM_RIGHT_FRONT,
			VOXEL_NEIGHBOUR_FRONT, VOXEL_NEIGHBOUR_BOTTOM_FRONT, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_FRONT);

	//Top Left
	refresh_point(P010, 0, 255, 0,
			VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_TOP_LEFT, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_FRONT,
			VOXEL_NEIGHBOUR_TOP, VOXEL_NEIGHBOUR_TOP_LEFT, VOXEL_NEIGHBOUR_TOP_FRONT, VOXEL_NEIGHBOUR_TOP_LEFT_FRONT,
			VOXEL_NEIGHBOUR_FRONT, VOXEL_NEIGHBOUR_TOP_FRONT, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_FRONT);

	//Top Right
	refresh_point(P110, 255, 255, 0,
			VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_TOP_RIGHT, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_FRONT,
			VOXEL_NEIGHBOUR_TOP, VOXEL_NEIGHBOUR_TOP_RIGHT, VOXEL_NEIGHBOUR_TOP_FRONT, VOXEL_NEIGHBOUR_TOP_RIGHT_FRONT,
			VOXEL_NEIGHBOUR_FRONT, VOXEL_NEIGHBOUR_TOP_FRONT, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_FRONT);

	//Back
	//Bottom Left
	refresh_point(P001, 0, 0, 255,
			VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_BOTTOM_LEFT, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_BACK,
			VOXEL_NEIGHBOUR_BOTTOM, VOXEL_NEIGHBOUR_BOTTOM_LEFT, VOXEL_NEIGHBOUR_BOTTOM_BACK, VOXEL_NEIGHBOUR_BOTTOM_LEFT_BACK,
			VOXEL_NEIGHBOUR_BACK, VOXEL_NEIGHBOUR_BOTTOM_BACK, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_BACK);

	//Bottom Right
	refresh_point(P101, 255, 0, 255,
			VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_BOTTOM_RIGHT, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_BACK,
			VOXEL_NEIGHBOUR_BOTTOM, VOXEL_NEIGHBOUR_BOTTOM_RIGHT, VOXEL_NEIGHBOUR_BOTTOM_BACK, VOXEL_NEIGHBOUR_BOTTOM_RIGHT_BACK,
			VOXEL_NEIGHBOUR_BACK, VOXEL_NEIGHBOUR_BOTTOM_BACK, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_BACK);

	//Top Left
	refresh_point(P011, 0, 255, 255,
			VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_TOP_LEFT, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_BACK,
			VOXEL_NEIGHBOUR_TOP, VOXEL_NEIGHBOUR_TOP_LEFT, VOXEL_NEIGHBOUR_TOP_BACK, VOXEL_NEIGHBOUR_TOP_LEFT_BACK,
			VOXEL_NEIGHBOUR_BACK, VOXEL_NEIGHBOUR_TOP_BACK, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_BACK);

	//Top Right
	refresh_point(P111, 255, 255, 255,
			VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_TOP_RIGHT, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_BACK,
			VOXEL_NEIGHBOUR_TOP, VOXEL_NEIGHBOUR_TOP_RIGHT, VOXEL_NEIGHBOUR_TOP_BACK, VOXEL_NEIGHBOUR_TOP_RIGHT_BACK,
			VOXEL_NEIGHBOUR_BACK, VOXEL_NEIGHBOUR_TOP_BACK, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_BACK);
}

void VoxelCubePoints::refresh_point(int index, int vectx, int vecty, int vectz, int axisx, int axis2x, int axis3x, int axis4notx, int axisy, int axis2y, int axis3y, int axis4y, int axisz, int axis2z, int axis3z, int axis4notz) {

	char fill = _point_fills[index];
	unsigned int neighbours = _point_neighbours[index];

	Vector3i vector3i = Vector3i(vectx, vecty, vectz);
	if ((neighbours & axisx) != axisx) {
		if (vectx == 0) {
			vector3i.x = (int)(128 - fill);
		} else {
			vector3i.x = (int)(fill + 128);
		}
	}

	if ((neighbours & axisy) != axisy) {
		if (vecty == 0) {
			vector3i.y = (int)(128 - fill);
		} else {
			vector3i.y = (int)(fill + 128);
		}
	}

	if ((neighbours & axisz) != axisz) {
		if (vectz == 0) {
			vector3i.z = (int)(128 - fill);
		} else {
			vector3i.z = (int)(fill + 128);
		}
	}

	_points[index] = vector3i;
}

void VoxelCubePoints::refresh_point_full(int index, int vectx, int vecty, int vectz, int axisx, int axis2x, int axis3x, int axisy, int axis2y, int axis3y, int axis4y, int axisz, int axis2z, int axis3z) {
	Vector3i vector3i = Vector3i(vectx, vecty, vectz);
	_points[index] = vector3i;
}

void VoxelCubePoints::setup(const Ref<VoxelBuffer> buffer, int x, int y, int z, int size) {
	ERR_FAIL_COND(size <= 0);

	_point_types[P000] = buffer->get_voxel(x, y, z, VoxelBuffer::CHANNEL_TYPE);
	_point_types[P100] = buffer->get_voxel(x + size, y, z, VoxelBuffer::CHANNEL_TYPE);
	_point_types[P010] = buffer->get_voxel(x, y + size, z, VoxelBuffer::CHANNEL_TYPE);
	_point_types[P001] = buffer->get_voxel(x, y, z + size, VoxelBuffer::CHANNEL_TYPE);
	_point_types[P110] = buffer->get_voxel(x + size, y + size, z, VoxelBuffer::CHANNEL_TYPE);
	_point_types[P011] = buffer->get_voxel(x, y + size, z + size, VoxelBuffer::CHANNEL_TYPE);
	_point_types[P101] = buffer->get_voxel(x + size, y, z + size, VoxelBuffer::CHANNEL_TYPE);
	_point_types[P111] = buffer->get_voxel(x + size, y + size, z + size, VoxelBuffer::CHANNEL_TYPE);

	if (!has_points())
		return;

	_point_fills[P000] = buffer->get_voxel(x, y, z, VoxelBuffer::CHANNEL_ISOLEVEL);
	_point_fills[P100] = buffer->get_voxel(x + size, y, z, VoxelBuffer::CHANNEL_ISOLEVEL);
	_point_fills[P010] = buffer->get_voxel(x, y + size, z, VoxelBuffer::CHANNEL_ISOLEVEL);
	_point_fills[P001] = buffer->get_voxel(x, y, z + size, VoxelBuffer::CHANNEL_ISOLEVEL);
	_point_fills[P110] = buffer->get_voxel(x + size, y + size, z, VoxelBuffer::CHANNEL_ISOLEVEL);
	_point_fills[P011] = buffer->get_voxel(x, y + size, z + size, VoxelBuffer::CHANNEL_ISOLEVEL);
	_point_fills[P101] = buffer->get_voxel(x + size, y, z + size, VoxelBuffer::CHANNEL_ISOLEVEL);
	_point_fills[P111] = buffer->get_voxel(x + size, y + size, z + size, VoxelBuffer::CHANNEL_ISOLEVEL);



}


Ref<SubVoxelSidePoints> VoxelCubePoints::get_points_for_face(int face) {

	if (face == VOXEL_FACE_FRONT) {
		_face_helper->set_point(0, get_point(P000));
		_face_helper->set_point(1, get_point(P010));
		_face_helper->set_point(2, get_point(P110));
		_face_helper->set_point(3, get_point(P100));
	} else if (face == VOXEL_FACE_BACK) {
		_face_helper->set_point(0, get_point(P101));
		_face_helper->set_point(1, get_point(P111));
		_face_helper->set_point(2, get_point(P011));
		_face_helper->set_point(3, get_point(P001));
	} else if (face == VOXEL_FACE_RIGHT) {
		_face_helper->set_point(0, get_point(P100));
		_face_helper->set_point(1, get_point(P110));
		_face_helper->set_point(2, get_point(P111));
		_face_helper->set_point(3, get_point(P101));
	} else if (face == VOXEL_FACE_LEFT) {
		_face_helper->set_point(0, get_point(P001));
		_face_helper->set_point(1, get_point(P011));
		_face_helper->set_point(2, get_point(P010));
		_face_helper->set_point(3, get_point(P000));
	} else if (face == VOXEL_FACE_TOP) {
		_face_helper->set_point(0, get_point(P111));
		_face_helper->set_point(1, get_point(P110));
		_face_helper->set_point(2, get_point(P010));
		_face_helper->set_point(3, get_point(P011));
	} else if (face == VOXEL_FACE_BOTTOM) {
		_face_helper->set_point(0, get_point(P001));
		_face_helper->set_point(1, get_point(P000));
		_face_helper->set_point(2, get_point(P100));
		_face_helper->set_point(3, get_point(P101));
	}

	return _face_helper;
}

bool VoxelCubePoints::face_fully_covered(int face) {
	SubVoxelFacePointsHelper avp(face, this);

	return avp.is_face_fully_covered();
}

bool VoxelCubePoints::face_should_be_visible_against_full(int face) {

	SubVoxelFacePointsHelper avp(face, this);

	return !avp.is_face_near_the_edges();
}

bool VoxelCubePoints::face_should_be_visible_against(int face, Ref<VoxelCubePoints> other) {
	SubVoxelFacePointsHelper avp(face, this);

	SubVoxelFacePointsHelper other2(get_opposite_face(face), other);

	return avp.is_face_visible_against(other2);
}

bool VoxelCubePoints::is_sub_voxel_point(Vector3i point) {
	for (int i = 0; i < POINT_COUNT; i += 1) {
		if (get_point(i) == (point)) {
			return true;
		}
	}
	return false;
}

bool VoxelCubePoints::is_sub_voxel_point(int x, int y, int z) {
	for (int i = 0; i < POINT_COUNT; i += 1) {
		if (get_point(i) == Vector3i(x, y, z)) {
			return true;
		}
	}
	return false;
}

void VoxelCubePoints::set_point(int point, int x, int y, int z) {
	_points[point] = Vector3i(x, y, z);
}

int VoxelCubePoints::get_point_id(Vector3i point) {
	for (int i = 0; i < POINT_COUNT; ++i) {
		if (get_point(i) == point) {
			return i;
		}
	}
	return 0;
}

int VoxelCubePoints::get_point_id(int x, int y, int z) {
	for (int i = 0; i < POINT_COUNT; ++i) {
		if (get_point(i) == Vector3i(x, y, z)) {
			return i;
		}
	}
	return 0;
}

Vector3i VoxelCubePoints::get_top_left_point(int face) {
	if (face == VOXEL_FACE_BACK) {
		return _points[P011];
	}
	if (face == VOXEL_FACE_FRONT) {
		return _points[P010];
	}
	if (face == VOXEL_FACE_RIGHT) {
		return _points[P111];
	}
	if (face == VOXEL_FACE_LEFT) {
		return _points[P010];
	}
	if (face == VOXEL_FACE_TOP) {
		return _points[P010];
	}
	if (face == VOXEL_FACE_BOTTOM) {
		return _points[P000];
	}
	return _points[0];
}

Vector3i VoxelCubePoints::get_top_right_point(int face) {
	if (face == VOXEL_FACE_BACK) {
		return _points[P111];
	}
	if (face == VOXEL_FACE_FRONT) {
		return _points[P110];
	}
	if (face == VOXEL_FACE_RIGHT) {
		return _points[P110];
	}
	if (face == VOXEL_FACE_LEFT) {
		return _points[P011];
	}
	if (face == VOXEL_FACE_TOP) {
		return _points[P110];
	}
	if (face == VOXEL_FACE_BOTTOM) {
		return _points[P100];
	}
	return _points[0];
}

Vector3i VoxelCubePoints::get_bottom_left_point(int face) {
	if (face == VOXEL_FACE_BACK) {
		return _points[P001];
	}
	if (face == VOXEL_FACE_FRONT) {
		return _points[P000];
	}
	if (face == VOXEL_FACE_RIGHT) {
		return _points[P101];
	}
	if (face == VOXEL_FACE_LEFT) {
		return _points[P001];
	}
	if (face == VOXEL_FACE_TOP) {
		return _points[P011];
	}
	if (face == VOXEL_FACE_BOTTOM) {
		return _points[P001];
	}
	return _points[0];
}

Vector3i VoxelCubePoints::get_bottom_right_point(int face) {
	if (face == VOXEL_FACE_BACK) {
		return _points[P101];
	}
	if (face == VOXEL_FACE_FRONT) {
		return _points[P100];
	}
	if (face == VOXEL_FACE_RIGHT) {
		return _points[P100];
	}
	if (face == VOXEL_FACE_LEFT) {
		return _points[P001];
	}
	if (face == VOXEL_FACE_TOP) {
		return _points[P111];
	}
	if (face == VOXEL_FACE_BOTTOM) {
		return _points[P101];
	}
	return _points[P000];
}

bool VoxelCubePoints::has_points() {
	return (_point_types[P000] != 0 && _point_types[P100] != 0 && _point_types[P010] != 0 && _point_types[P001] != 0 &&
			_point_types[P110] != 0 && _point_types[P011] != 0 && _point_types[P101] != 0 && _point_types[P111] != 0);
}

int VoxelCubePoints::get_opposite_face(int face) {
	if (face == VOXEL_FACE_FRONT) {
		return VOXEL_FACE_BACK;
	}
	if (face == VOXEL_FACE_BACK) {
		return VOXEL_FACE_FRONT;
	}
	if (face == VOXEL_FACE_LEFT) {
		return VOXEL_FACE_RIGHT;
	}
	if (face == VOXEL_FACE_RIGHT) {
		return VOXEL_FACE_LEFT;
	}
	if (face == VOXEL_FACE_TOP) {
		return VOXEL_FACE_BOTTOM;
	}

	return VOXEL_FACE_BOTTOM;
}

void VoxelCubePoints::_bind_methods() {

	BIND_ENUM_CONSTANT(P000);
	BIND_ENUM_CONSTANT(P100);
	BIND_ENUM_CONSTANT(P010);
	BIND_ENUM_CONSTANT(P001);
	BIND_ENUM_CONSTANT(P110);
	BIND_ENUM_CONSTANT(P011);
	BIND_ENUM_CONSTANT(P101);
	BIND_ENUM_CONSTANT(P111);
	BIND_ENUM_CONSTANT(POINT_COUNT);

	BIND_ENUM_CONSTANT(VOXEL_FACE_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_FACE_RIGHT);
	BIND_ENUM_CONSTANT(VOXEL_FACE_BACK);
	BIND_ENUM_CONSTANT(VOXEL_FACE_LEFT);
	BIND_ENUM_CONSTANT(VOXEL_FACE_TOP);
	BIND_ENUM_CONSTANT(VOXEL_FACE_BOTTOM);
	BIND_ENUM_CONSTANT(VOXEL_FACE_COUNT);

	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_NONE);

	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_LEFT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_RIGHT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BACK);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM);

	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_LEFT_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_LEFT_BACK);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_RIGHT_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_RIGHT_BACK);

	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_LEFT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_RIGHT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_BACK);

	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_LEFT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_RIGHT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_BACK);

	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_LEFT_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_LEFT_BACK);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_RIGHT_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_RIGHT_BACK);

	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_LEFT_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_LEFT_BACK);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_RIGHT_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_RIGHT_BACK);

	BIND_ENUM_CONSTANT(VOXEL_FULL_NEIGHBOURS_CROSS);
	BIND_ENUM_CONSTANT(VOXEL_FULL_SIDE_NEIGHBOURS);
	BIND_ENUM_CONSTANT(VOXEL_FULL_SIDE_NEIGHBOURS_TOP);
	BIND_ENUM_CONSTANT(VOXEL_FULL_SIDE_NEIGHBOURS_DOWN);
}
