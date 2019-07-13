#include "sub_voxel_points.h"

namespace voxelman {

#if MEM_TOOLS
int SubVoxelPoints::allocs = 0;
#endif

Vector3i SubVoxelPoints::get_point(int index) {
	return _points[index];
}

SubVoxelPoints::SubVoxelPoints(Voxel *voxel) {
	_voxel = voxel;

	_voxel_datas = memnew_arr(Ref<VoxelData>, POINT_COUNT);

	//for (int i = 0; i < POINT_COUNT; i++) {
	//	_voxel_datas[i] = Ref<VoxelData>(NULL);
	//}

	//_voxel_datas = new Vector<VoxelData>(8);
	//points = new Vector<Vector3i>(8);

	//for (int i = 0; i < POINT_COUNT; ++i) {
	//	points.SetData(i, );
	//}

	//faceHelper = new SubVoxelSidePoints();

#if MEM_TOOLS
	SubVoxelPoints::allocs++;
	print_error("SubVoxelPoints alloc " + String::num(SubVoxelPoints::allocs));
#endif
}

SubVoxelPoints::~SubVoxelPoints() {
	for (int i = 0; i < POINT_COUNT; i++) {
		_voxel_datas[i] = Ref<VoxelData>(NULL);
	}

	memdelete_arr(_voxel_datas);

#if MEM_TOOLS
	SubVoxelPoints::allocs--;
	print_error("SubVoxelPoints dealloc " + String::num(SubVoxelPoints::allocs));
#endif
}

void SubVoxelPoints::refresh_points() {
	//Front
	//Bottom Left
	refresh_point(P000, 0, 0, 0,
			Voxel::NEIGHBOUR_LEFT, Voxel::NEIGHBOUR_BOTTOM_LEFT, Voxel::NEIGHBOUR_LEFT, Voxel::NEIGHBOUR_FRONT,
			Voxel::NEIGHBOUR_DOWN, Voxel::NEIGHBOUR_BOTTOM_LEFT, Voxel::NEIGHBOUR_BOTTOM_FRONT, Voxel::NEIGHBOUR_BOTTOM_LEFT_FRONT,
			Voxel::NEIGHBOUR_FRONT, Voxel::NEIGHBOUR_BOTTOM_FRONT, Voxel::NEIGHBOUR_LEFT, Voxel::NEIGHBOUR_FRONT);

	//Bottom Right
	refresh_point(P100, 255, 0, 0,
			Voxel::NEIGHBOUR_RIGHT, Voxel::NEIGHBOUR_BOTTOM_RIGHT, Voxel::NEIGHBOUR_RIGHT, Voxel::NEIGHBOUR_FRONT,
			Voxel::NEIGHBOUR_DOWN, Voxel::NEIGHBOUR_BOTTOM_RIGHT, Voxel::NEIGHBOUR_BOTTOM_FRONT, Voxel::NEIGHBOUR_BOTTOM_RIGHT_FRONT,
			Voxel::NEIGHBOUR_FRONT, Voxel::NEIGHBOUR_BOTTOM_FRONT, Voxel::NEIGHBOUR_RIGHT, Voxel::NEIGHBOUR_FRONT);

	//Top Left
	refresh_point(P010, 0, 255, 0,
			Voxel::NEIGHBOUR_LEFT, Voxel::NEIGHBOUR_TOP_LEFT, Voxel::NEIGHBOUR_LEFT, Voxel::NEIGHBOUR_FRONT,
			Voxel::NEIGHBOUR_TOP, Voxel::NEIGHBOUR_TOP_LEFT, Voxel::NEIGHBOUR_TOP_FRONT, Voxel::NEIGHBOUR_TOP_LEFT_FRONT,
			Voxel::NEIGHBOUR_FRONT, Voxel::NEIGHBOUR_TOP_FRONT, Voxel::NEIGHBOUR_LEFT, Voxel::NEIGHBOUR_FRONT);

	//Top Right
	refresh_point(P110, 255, 255, 0,
			Voxel::NEIGHBOUR_RIGHT, Voxel::NEIGHBOUR_TOP_RIGHT, Voxel::NEIGHBOUR_RIGHT, Voxel::NEIGHBOUR_FRONT,
			Voxel::NEIGHBOUR_TOP, Voxel::NEIGHBOUR_TOP_RIGHT, Voxel::NEIGHBOUR_TOP_FRONT, Voxel::NEIGHBOUR_TOP_RIGHT_FRONT,
			Voxel::NEIGHBOUR_FRONT, Voxel::NEIGHBOUR_TOP_FRONT, Voxel::NEIGHBOUR_RIGHT, Voxel::NEIGHBOUR_FRONT);

	//Back
	//Bottom Left
	refresh_point(P001, 0, 0, 255,
			Voxel::NEIGHBOUR_LEFT, Voxel::NEIGHBOUR_BOTTOM_LEFT, Voxel::NEIGHBOUR_LEFT, Voxel::NEIGHBOUR_BACK,
			Voxel::NEIGHBOUR_DOWN, Voxel::NEIGHBOUR_BOTTOM_LEFT, Voxel::NEIGHBOUR_BOTTOM_BACK, Voxel::NEIGHBOUR_BOTTOM_LEFT_BACK,
			Voxel::NEIGHBOUR_BACK, Voxel::NEIGHBOUR_BOTTOM_BACK, Voxel::NEIGHBOUR_LEFT, Voxel::NEIGHBOUR_BACK);

	//Bottom Right
	refresh_point(P101, 255, 0, 255,
			Voxel::NEIGHBOUR_RIGHT, Voxel::NEIGHBOUR_BOTTOM_RIGHT, Voxel::NEIGHBOUR_RIGHT, Voxel::NEIGHBOUR_BACK,
			Voxel::NEIGHBOUR_DOWN, Voxel::NEIGHBOUR_BOTTOM_RIGHT, Voxel::NEIGHBOUR_BOTTOM_BACK, Voxel::NEIGHBOUR_BOTTOM_RIGHT_BACK,
			Voxel::NEIGHBOUR_BACK, Voxel::NEIGHBOUR_BOTTOM_BACK, Voxel::NEIGHBOUR_RIGHT, Voxel::NEIGHBOUR_BACK);

	//Top Left
	refresh_point(P011, 0, 255, 255,
			Voxel::NEIGHBOUR_LEFT, Voxel::NEIGHBOUR_TOP_LEFT, Voxel::NEIGHBOUR_LEFT, Voxel::NEIGHBOUR_BACK,
			Voxel::NEIGHBOUR_TOP, Voxel::NEIGHBOUR_TOP_LEFT, Voxel::NEIGHBOUR_TOP_BACK, Voxel::NEIGHBOUR_TOP_LEFT_BACK,
			Voxel::NEIGHBOUR_BACK, Voxel::NEIGHBOUR_TOP_BACK, Voxel::NEIGHBOUR_LEFT, Voxel::NEIGHBOUR_BACK);

	//Top Right
	refresh_point(P111, 255, 255, 255,
			Voxel::NEIGHBOUR_RIGHT, Voxel::NEIGHBOUR_TOP_RIGHT, Voxel::NEIGHBOUR_RIGHT, Voxel::NEIGHBOUR_BACK,
			Voxel::NEIGHBOUR_TOP, Voxel::NEIGHBOUR_TOP_RIGHT, Voxel::NEIGHBOUR_TOP_BACK, Voxel::NEIGHBOUR_TOP_RIGHT_BACK,
			Voxel::NEIGHBOUR_BACK, Voxel::NEIGHBOUR_TOP_BACK, Voxel::NEIGHBOUR_RIGHT, Voxel::NEIGHBOUR_BACK);
}

void SubVoxelPoints::refresh_point(int index, int vectx, int vecty, int vectz, int axisx, int axis2x, int axis3x, int axis4notx, int axisy, int axis2y, int axis3y, int axis4y, int axisz, int axis2z, int axis3z, int axis4notz) {
	Ref<VoxelData> tVoxelData = _voxel_datas[index];
	char fill = (char)128;

	if (tVoxelData != NULL) {
		fill = tVoxelData->get_fill();
	}

	Vector3i vector3i = Vector3i(vectx, vecty, vectz);
	if ((tVoxelData->get_neighbours() & axisx) != axisx) {
		if (vectx == 0) {
			vector3i.x = (int)(128 - fill);
		} else {
			vector3i.x = (int)(fill + 128);
		}
	}

	if ((tVoxelData->get_neighbours() & axisy) != axisy) {
		if (vecty == 0) {
			vector3i.y = (int)(128 - fill);
		} else {
			vector3i.y = (int)(fill + 128);
		}
	}

	if ((tVoxelData->get_neighbours() & axisz) != axisz) {
		if (vectz == 0) {
			vector3i.z = (int)(128 - fill);
		} else {
			vector3i.z = (int)(fill + 128);
		}
	}

	_points[index] = vector3i;
}

void SubVoxelPoints::refresh_pointo(int index, int vectx, int vecty, int vectz, int axisx, int axis2x, int axis3x, int axis4notx, int axisy, int axis2y, int axis3y, int axis4y, int axisz, int axis2z, int axis3z, int axis4notz) {
	Ref<VoxelData> tVoxelData = _voxel_datas[index];
	char b = (char)128;
	if (tVoxelData != NULL) {
		b = tVoxelData->get_fill();
	}
	Vector3i vector3i = Vector3i(vectx, vecty, vectz);
	if (((((tVoxelData->get_neighbours() & axisx) != axisx) && ((tVoxelData->get_neighbours() & axis2x) != axis2x)) && ((tVoxelData->get_neighbours() & axis3x) != axis3x)) && ((tVoxelData->get_neighbours() & axis4notx) != axis4notx)) {
		if (vectx == 0) {
			vector3i.x = (int)(128 - b);
		} else {
			vector3i.x = (int)(b + 128);
		}
	}
	if (((((tVoxelData->get_neighbours() & axisy) != axisy) && ((tVoxelData->get_neighbours() & axis2y) != axis2y)) && ((tVoxelData->get_neighbours() & axis3y) != axis3y)) && ((tVoxelData->get_neighbours() & axis4y) != axis4y)) {
		if (vecty == 0) {
			vector3i.y = (int)(128 - b);
		} else {
			vector3i.y = (int)(b + 128);
		}
	}
	if (((((tVoxelData->get_neighbours() & axisz) != axisz) && ((tVoxelData->get_neighbours() & axis2z) != axis2z)) && ((tVoxelData->get_neighbours() & axis3z) != axis3z)) && ((tVoxelData->get_neighbours() & axis4notz) != axis4notz)) {
		if (vectz == 0) {
			vector3i.z = (int)(128 - b);
		} else {
			vector3i.z = (int)(b + 128);
		}
	}

	_points[index] = vector3i;
}

void SubVoxelPoints::refresh_point_full(int index, int vectx, int vecty, int vectz, int axisx, int axis2x, int axis3x, int axisy, int axis2y, int axis3y, int axis4y, int axisz, int axis2z, int axis3z) {
	Vector3i vector3i = Vector3i(vectx, vecty, vectz);
	_points[index] = vector3i;
}

void SubVoxelPoints::set_voxel_datas(Ref<VoxelData> VP000, Ref<VoxelData> VP100, Ref<VoxelData> VP010, Ref<VoxelData> VP001, Ref<VoxelData> VP110, Ref<VoxelData> VP011, Ref<VoxelData> VP101, Ref<VoxelData> VP111) {
	if (VP000 != NULL && VP000->get_surface()->get_prefab_id() == 0 && VP000->get_surface()->get_mesh_id() == 0) {
		_voxel_datas[P000] = VP000;
	}
	if (VP100 != NULL && VP100->get_surface()->get_prefab_id() == 0 && VP100->get_surface()->get_mesh_id() == 0) {
		_voxel_datas[P100] = VP100;
	}
	if (VP010 != NULL && VP010->get_surface()->get_prefab_id() == 0 && VP010->get_surface()->get_mesh_id() == 0) {
		_voxel_datas[P010] = VP010;
	}
	if (VP001 != NULL && VP001->get_surface()->get_prefab_id() == 0 && VP001->get_surface()->get_mesh_id() == 0) {
		_voxel_datas[P001] = VP001;
	}
	if (VP110 != NULL && VP110->get_surface()->get_prefab_id() == 0 && VP110->get_surface()->get_mesh_id() == 0) {
		_voxel_datas[P110] = VP110;
	}
	if (VP011 != NULL && VP011->get_surface()->get_prefab_id() == 0 && VP011->get_surface()->get_mesh_id() == 0) {
		_voxel_datas[P011] = VP011;
	}
	if (VP101 != NULL && VP101->get_surface()->get_prefab_id() == 0 && VP101->get_surface()->get_mesh_id() == 0) {
		_voxel_datas[P101] = VP101;
	}
	if (VP111 != NULL && VP111->get_surface()->get_prefab_id() == 0 && VP111->get_surface()->get_mesh_id() == 0) {
		_voxel_datas[P111] = VP111;
	}
}

SubVoxelSidePoints SubVoxelPoints::get_points_for_face(int face) {

	if (face == VoxelFace::VOXEL_FACE_FRONT) {
		_face_helper.set_point(0, get_point(P000));
		_face_helper.set_point(1, get_point(P010));
		_face_helper.set_point(2, get_point(P110));
		_face_helper.set_point(3, get_point(P100));
	} else if (face == VoxelFace::VOXEL_FACE_BACK) {
		_face_helper.set_point(0, get_point(P101));
		_face_helper.set_point(1, get_point(P111));
		_face_helper.set_point(2, get_point(P011));
		_face_helper.set_point(3, get_point(P001));
	} else if (face == VoxelFace::VOXEL_FACE_RIGHT) {
		_face_helper.set_point(0, get_point(P100));
		_face_helper.set_point(1, get_point(P110));
		_face_helper.set_point(2, get_point(P111));
		_face_helper.set_point(3, get_point(P101));
	} else if (face == VoxelFace::VOXEL_FACE_LEFT) {
		_face_helper.set_point(0, get_point(P001));
		_face_helper.set_point(1, get_point(P011));
		_face_helper.set_point(2, get_point(P010));
		_face_helper.set_point(3, get_point(P000));
	} else if (face == VoxelFace::VOXEL_FACE_TOP) {
		_face_helper.set_point(0, get_point(P111));
		_face_helper.set_point(1, get_point(P110));
		_face_helper.set_point(2, get_point(P010));
		_face_helper.set_point(3, get_point(P011));
	} else if (face == VoxelFace::VOXEL_FACE_DOWN) {
		_face_helper.set_point(0, get_point(P001));
		_face_helper.set_point(1, get_point(P000));
		_face_helper.set_point(2, get_point(P100));
		_face_helper.set_point(3, get_point(P101));
	}

	return _face_helper;
}

bool SubVoxelPoints::face_fully_covered(int face) {
	SubVoxelFacePointsHelper avp(face, this);

	return avp.is_face_fully_covered();
}

bool SubVoxelPoints::face_should_be_visible_against_full(int face) {

	SubVoxelFacePointsHelper avp(face, this);

	return !avp.is_face_near_the_edges();
}

bool SubVoxelPoints::face_should_be_visible_against(int face, SubVoxelPoints *other) {
	SubVoxelFacePointsHelper avp(face, this);

	SubVoxelFacePointsHelper other2(get_opposite_face(face), other);

	return avp.is_face_visible_against(other2);
}

bool SubVoxelPoints::is_sub_voxel_point(Vector3i point) {
	for (int i = 0; i < POINT_COUNT; i += 1) {
		if (get_point(i) == (point)) {
			return true;
		}
	}
	return false;
}

bool SubVoxelPoints::is_sub_voxel_point(int x, int y, int z) {
	for (int i = 0; i < POINT_COUNT; i += 1) {
		if (get_point(i) == Vector3i(x, y, z)) {
			return true;
		}
	}
	return false;
}

void SubVoxelPoints::set_point(int point, int x, int y, int z) {
	_points[point] = Vector3i(x, y, z);
}

int SubVoxelPoints::get_point_id(Vector3i point) {
	for (int i = 0; i < POINT_COUNT; ++i) {
		if (get_point(i) == point) {
			return i;
		}
	}
	return 0;
}

int SubVoxelPoints::get_point_id(int x, int y, int z) {
	for (int i = 0; i < POINT_COUNT; ++i) {
		if (get_point(i) == Vector3i(x, y, z)) {
			return i;
		}
	}
	return 0;
}

void SubVoxelPoints::copy_values_from(SubVoxelPoints *other) {
	for (int i = 0; i < POINT_COUNT; ++i) {
		_points[i] = other->get_point(i);
	}
}

Vector3i SubVoxelPoints::get_top_left_point(int face, SubVoxelPoints *points) {
	if (face == VoxelFace::VOXEL_FACE_BACK) {
		return points->get_point(P011);
	}
	if (face == VoxelFace::VOXEL_FACE_FRONT) {
		return points->get_point(P010);
	}
	if (face == VoxelFace::VOXEL_FACE_RIGHT) {
		return points->get_point(P111);
	}
	if (face == VoxelFace::VOXEL_FACE_LEFT) {
		return points->get_point(P010);
	}
	if (face == VoxelFace::VOXEL_FACE_TOP) {
		return points->get_point(P010);
	}
	if (face == VoxelFace::VOXEL_FACE_DOWN) {
		return points->get_point(P000);
	}
	return points->get_point(0);
}

Vector3i SubVoxelPoints::get_top_right_point(int face, SubVoxelPoints *points) {
	if (face == VoxelFace::VOXEL_FACE_BACK) {
		return points->get_point(P111);
	}
	if (face == VoxelFace::VOXEL_FACE_FRONT) {
		return points->get_point(P110);
	}
	if (face == VoxelFace::VOXEL_FACE_RIGHT) {
		return points->get_point(P110);
	}
	if (face == VoxelFace::VOXEL_FACE_LEFT) {
		return points->get_point(P011);
	}
	if (face == VoxelFace::VOXEL_FACE_TOP) {
		return points->get_point(P110);
	}
	if (face == VoxelFace::VOXEL_FACE_DOWN) {
		return points->get_point(P100);
	}
	return points->get_point(0);
}

Vector3i SubVoxelPoints::get_bottom_left_point(int face, SubVoxelPoints *points) {
	if (face == VoxelFace::VOXEL_FACE_BACK) {
		return points->get_point(P001);
	}
	if (face == VoxelFace::VOXEL_FACE_FRONT) {
		return points->get_point(P000);
	}
	if (face == VoxelFace::VOXEL_FACE_RIGHT) {
		return points->get_point(P101);
	}
	if (face == VoxelFace::VOXEL_FACE_LEFT) {
		return points->get_point(P001);
	}
	if (face == VoxelFace::VOXEL_FACE_TOP) {
		return points->get_point(P011);
	}
	if (face == VoxelFace::VOXEL_FACE_DOWN) {
		return points->get_point(P001);
	}
	return points->get_point(0);
}

Vector3i SubVoxelPoints::get_bottom_right_point(int face, SubVoxelPoints *points) {
	if (face == VoxelFace::VOXEL_FACE_BACK) {
		return points->get_point(P101);
	}
	if (face == VoxelFace::VOXEL_FACE_FRONT) {
		return points->get_point(P100);
	}
	if (face == VoxelFace::VOXEL_FACE_RIGHT) {
		return points->get_point(P100);
	}
	if (face == VoxelFace::VOXEL_FACE_LEFT) {
		return points->get_point(P001);
	}
	if (face == VoxelFace::VOXEL_FACE_TOP) {
		return points->get_point(P111);
	}
	if (face == VoxelFace::VOXEL_FACE_DOWN) {
		return points->get_point(P101);
	}
	return points->get_point(0);
}

bool SubVoxelPoints::validate_voxel_datas(Ref<VoxelData> VP000, Ref<VoxelData> VP100, Ref<VoxelData> VP010, Ref<VoxelData> VP001, Ref<VoxelData> VP110, Ref<VoxelData> VP011, Ref<VoxelData> VP101, Ref<VoxelData> VP111) {
	return (VP000 != NULL && VP100 != NULL && VP010 != NULL && VP001 != NULL &&
			VP110 != NULL && VP011 != NULL && VP101 != NULL && VP111 != NULL &&
			VP000->get_surface()->get_prefab_id() == 0 &&
			VP000->get_surface()->get_mesh_id() == 0 &&
			VP100->get_surface()->get_prefab_id() == 0 &&
			VP100->get_surface()->get_mesh_id() == 0 &&
			VP010->get_surface()->get_prefab_id() == 0 &&
			VP010->get_surface()->get_mesh_id() == 0 &&
			VP001->get_surface()->get_prefab_id() == 0 &&
			VP001->get_surface()->get_mesh_id() == 0 &&
			VP110->get_surface()->get_prefab_id() == 0 &&
			VP110->get_surface()->get_mesh_id() == 0 &&
			VP011->get_surface()->get_prefab_id() == 0 &&
			VP011->get_surface()->get_mesh_id() == 0 &&
			VP101->get_surface()->get_prefab_id() == 0 &&
			VP101->get_surface()->get_mesh_id() == 0 &&
			VP111->get_surface()->get_prefab_id() == 0 &&
			VP111->get_surface()->get_mesh_id() == 0);
}

int SubVoxelPoints::get_opposite_face(int face) {
	if (face == VoxelFace::VOXEL_FACE_FRONT) {
		return VoxelFace::VOXEL_FACE_BACK;
	}
	if (face == VoxelFace::VOXEL_FACE_BACK) {
		return VoxelFace::VOXEL_FACE_FRONT;
	}
	if (face == VoxelFace::VOXEL_FACE_LEFT) {
		return VoxelFace::VOXEL_FACE_RIGHT;
	}
	if (face == VoxelFace::VOXEL_FACE_RIGHT) {
		return VoxelFace::VOXEL_FACE_LEFT;
	}
	if (face == VoxelFace::VOXEL_FACE_TOP) {
		return VoxelFace::VOXEL_FACE_DOWN;
	}
	//int arg_4C_0 = VoxelFace::VOXEL_FACE_DOWN;
	return VoxelFace::VOXEL_FACE_DOWN;
}

} // namespace voxelman
