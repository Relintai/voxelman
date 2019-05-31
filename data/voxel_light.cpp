#include "voxel_light.h"

VoxelLight::VoxelLight() {
}

VoxelLight::VoxelLight(Vector3i position, Color color, float strength, Vector3 WorldPosition, Vector3 offset) {

	_world_position = WorldPosition + offset;
	_local_position = position;
	_offset = offset;
	_color = color;
	_strength = strength;
}

VoxelLight::~VoxelLight() {
}
