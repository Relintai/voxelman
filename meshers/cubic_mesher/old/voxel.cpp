#include "voxel.h"


int Voxel::get_neighbours() {
	return _neighbours;
}

void Voxel::set_neighbours(int value) {
	_neighbours = value;
}

bool Voxel::get_has_sub_voxel_points() {
	return _sub_voxel_points != NULL;
}

SubVoxelPoints *Voxel::get_sub_voxel_points() {
	return _sub_voxel_points;
}

Voxel::Voxel() {
	_sub_voxel_points = NULL;

}

void Voxel::set_voxel_datas(Ref<VoxelData> VP000, Ref<VoxelData> VP100, Ref<VoxelData> VP010, Ref<VoxelData> VP001, Ref<VoxelData> VP110, Ref<VoxelData> VP011, Ref<VoxelData> VP101, Ref<VoxelData> VP111) {
	if (_sub_voxel_points == NULL) {
		_sub_voxel_points = memnew(SubVoxelPoints(this));
	}

	_sub_voxel_points->set_voxel_datas(VP000, VP100, VP010, VP001, VP110, VP011, VP101, VP111);
}

void Voxel::refresh_sub_voxel_points() {
	get_sub_voxel_points()->refresh_points();
}
