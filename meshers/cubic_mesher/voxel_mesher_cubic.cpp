#include "voxel_mesher_cubic.h"

float VoxelMesherCubic::get_ao_strength() const {
	return _ao_strength;
}
void VoxelMesherCubic::set_ao_strength(float value) {
	_ao_strength = value;
}

float VoxelMesherCubic::get_base_light_value() const {
	return _base_light_value;
}
void VoxelMesherCubic::set_base_light_value(float value) {
	_base_light_value = value;
}


void VoxelMesherCubic::_add_buffer(Ref<VoxelBuffer> buffer) {
	buffer->generate_ao();

	Vector3i bfs = buffer->get_size();
	int x_size = bfs.x - 1;
	int y_size = bfs.y - 1;
	int z_size = bfs.z - 1;

	float lod_size = get_lod_size();
	float voxel_size = get_lod_size();
	float voxel_scale = get_voxel_scale();

	Ref<VoxelCubePoints> cube_points;
	cube_points.instance();

	float tile_uv_size = 1 / 4.0;
	Color base_light(_base_light_value, _base_light_value, _base_light_value);

	for (int y = lod_size; y < y_size - lod_size; y += lod_size) {
		for (int z = lod_size; z < z_size - lod_size; z += lod_size) {
			for (int x = lod_size; x < x_size - lod_size; x += lod_size) {

				cube_points->setup(buffer, x, y, z, lod_size);

				if (!cube_points->has_points())
					continue;

				for (int face = 0; face < VoxelCubePoints::VOXEL_FACE_COUNT; ++face) {
					if (!cube_points->is_face_visible(face))
						continue;

					add_indices(get_vertex_count() + 2);
					add_indices(get_vertex_count() + 1);
					add_indices(get_vertex_count() + 0);
					add_indices(get_vertex_count() + 3);
					add_indices(get_vertex_count() + 2);
					add_indices(get_vertex_count() + 0);

					Vector3 vertices[4] = {
						cube_points->get_point_for_face(face, 0),
						cube_points->get_point_for_face(face, 1),
						cube_points->get_point_for_face(face, 2),
						cube_points->get_point_for_face(face, 3)
					};

					Vector3 normals[4] = {
						(vertices[3] - vertices[0]).cross(vertices[1] - vertices[0]),
						(vertices[0] - vertices[1]).cross(vertices[2] - vertices[1]),
						(vertices[1] - vertices[2]).cross(vertices[3] - vertices[2]),
						(vertices[2] - vertices[3]).cross(vertices[0] - vertices[3])
					};

					Vector3 face_light_direction = cube_points->get_face_light_direction(face);

					for (int i = 0; i < 4; ++i) {
						Color light = cube_points->get_face_point_light_color(face, i);
						light += base_light;

						float NdotL = CLAMP(normals[i].dot(face_light_direction), 0, 1.0);

						light *= NdotL;

						light -= cube_points->get_face_point_ao_color(face, i) * _ao_strength;
						add_color(light);

						light.r = CLAMP(light.r, 0, 1.0);
						light.g = CLAMP(light.g, 0, 1.0);
						light.b = CLAMP(light.b, 0, 1.0);

						add_uv((cube_points->get_point_uv_direction(face, i) + Vector2(0.5, 0.5)) * Vector2(tile_uv_size, tile_uv_size));

						add_vertex((vertices[i] * voxel_size + Vector3(x, y, z)) * voxel_scale);
					}
				}
			}
		}
	}
}

VoxelMesherCubic::VoxelMesherCubic() {
	_ao_strength = 0.25;
	_base_light_value = 0.5;
}

VoxelMesherCubic::~VoxelMesherCubic() {
}

void VoxelMesherCubic::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_add_buffer", "buffer"), &VoxelMesherCubic::_add_buffer);

	ClassDB::bind_method(D_METHOD("get_ao_strength"), &VoxelMesherCubic::get_ao_strength);
	ClassDB::bind_method(D_METHOD("set_ao_strength", "value"), &VoxelMesherCubic::set_ao_strength);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ao_strength"), "set_ao_strength", "get_ao_strength");

	ClassDB::bind_method(D_METHOD("get_base_light_value"), &VoxelMesherCubic::get_base_light_value);
	ClassDB::bind_method(D_METHOD("set_base_light_value", "value"), &VoxelMesherCubic::set_base_light_value);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "base_light_value"), "set_base_light_value", "get_base_light_value");
}
