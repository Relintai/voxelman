/*
Copyright (c) 2019-2020 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "voxel_mesher_blocky.h"

#include "../../world/default/voxel_chunk_default.h"

void VoxelMesherBlocky::_add_chunk(Ref<VoxelChunk> p_chunk) {
	Ref<VoxelChunkDefault> chunk = p_chunk;

	ERR_FAIL_COND(!chunk.is_valid());

	if ((get_build_flags() & VoxelChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
		chunk->generate_ao();

	int x_size = chunk->get_size_x();
	int y_size = chunk->get_size_y();
	int z_size = chunk->get_size_z();

	float voxel_scale = get_voxel_scale();

	Color base_light(_base_light_value, _base_light_value, _base_light_value);
	Color light;
	bool use_lighting = (get_build_flags() & VoxelChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0;
	bool use_ao = (get_build_flags() & VoxelChunkDefault::BUILD_FLAG_USE_AO) != 0;
	bool use_rao = (get_build_flags() & VoxelChunkDefault::BUILD_FLAG_USE_RAO) != 0;

	for (int y = 0; y < y_size; ++y) {
		for (int z = 0; z < z_size; ++z) {
			for (int x = 0; x < x_size; ++x) {

				uint8_t type = chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_TYPE);

				if (type == 0)
					continue;

				Ref<VoxelSurface> surface = _library->get_voxel_surface(type);

				if (!surface.is_valid())
					continue;

				uint8_t neighbours[] = {
					chunk->get_voxel(x + 1, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_TYPE),
					chunk->get_voxel(x - 1, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_TYPE),
					chunk->get_voxel(x, y + 1, z, VoxelChunkDefault::DEFAULT_CHANNEL_TYPE),
					chunk->get_voxel(x, y - 1, z, VoxelChunkDefault::DEFAULT_CHANNEL_TYPE),
					chunk->get_voxel(x, y, z + 1, VoxelChunkDefault::DEFAULT_CHANNEL_TYPE),
					chunk->get_voxel(x, y, z - 1, VoxelChunkDefault::DEFAULT_CHANNEL_TYPE),
				};

				//x + 1
				if (neighbours[0] == 0) {
					if (use_lighting) {
						light = Color(chunk->get_voxel(x + 1, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
								chunk->get_voxel(x + 1, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
								chunk->get_voxel(x + 1, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

						float ao = 0;

						if (use_ao)
							ao = chunk->get_voxel(x + 1, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_AO) / 255.0;

						if (use_rao) {
							float rao = chunk->get_voxel(x + 1, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;
							ao += rao;
						}

						light += base_light;

						if (ao > 0)
							light -= Color(ao, ao, ao) * _ao_strength;

						light.r = CLAMP(light.r, 0, 1.0);
						light.g = CLAMP(light.g, 0, 1.0);
						light.b = CLAMP(light.b, 0, 1.0);
					}

					int vc = get_vertex_count();
					add_indices(vc + 2);
					add_indices(vc + 1);
					add_indices(vc + 0);
					add_indices(vc + 3);
					add_indices(vc + 2);
					add_indices(vc + 0);

					Vector2 uvs[] = {
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(0, 1)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(0, 0)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(1, 0)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(1, 1))
					};

					Vector3 verts[] = {
						Vector3(0.5, -0.5, -0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(0.5, 0.5, -0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(0.5, 0.5, 0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(0.5, -0.5, 0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale
					};

					for (int i = 0; i < 4; ++i) {
						add_normal(Vector3(1, 0, 0));

						if (use_lighting)
							add_color(light);

						add_uv(uvs[i]);
						add_vertex(verts[i]);
					}
				}

				//x - 1
				if (neighbours[1] == 0) {
					if (use_lighting) {
						light = Color(chunk->get_voxel(x - 1, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
								chunk->get_voxel(x - 1, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
								chunk->get_voxel(x - 1, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

						float ao = 0;

						if (use_ao)
							ao = chunk->get_voxel(x - 1, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_AO) / 255.0;

						if (use_rao) {
							float rao = chunk->get_voxel(x - 1, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;
							ao += rao;
						}

						light += base_light;

						if (ao > 0)
							light -= Color(ao, ao, ao) * _ao_strength;

						light.r = CLAMP(light.r, 0, 1.0);
						light.g = CLAMP(light.g, 0, 1.0);
						light.b = CLAMP(light.b, 0, 1.0);
					}

					int vc = get_vertex_count();
					add_indices(vc + 0);
					add_indices(vc + 1);
					add_indices(vc + 2);

					add_indices(vc + 0);
					add_indices(vc + 2);
					add_indices(vc + 3);

					Vector2 uvs[] = {
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(0, 1)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(0, 0)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(1, 0)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(1, 1))
					};

					Vector3 verts[] = {
						Vector3(-0.5, -0.5, -0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(-0.5, 0.5, -0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(-0.5, 0.5, 0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(-0.5, -0.5, 0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale
					};

					for (int i = 0; i < 4; ++i) {
						add_normal(Vector3(-1, 0, 0));

						if (use_lighting)
							add_color(light);

						add_uv(uvs[i]);
						add_vertex(verts[i]);
					}
				}

				//y + 1
				if (neighbours[2] == 0) {
					if (use_lighting) {
						light = Color(chunk->get_voxel(x, y + 1, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
								chunk->get_voxel(x, y + 1, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
								chunk->get_voxel(x, y + 1, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

						float ao = 0;

						if (use_ao)
							ao = chunk->get_voxel(x, y + 1, z, VoxelChunkDefault::DEFAULT_CHANNEL_AO) / 255.0;

						if (use_rao) {
							float rao = chunk->get_voxel(x, y + 1, z, VoxelChunkDefault::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;
							ao += rao;
						}

						light += base_light;

						if (ao > 0)
							light -= Color(ao, ao, ao) * _ao_strength;
					}

					light.r = CLAMP(light.r, 0, 1.0);
					light.g = CLAMP(light.g, 0, 1.0);
					light.b = CLAMP(light.b, 0, 1.0);

					int vc = get_vertex_count();
					add_indices(vc + 2);
					add_indices(vc + 1);
					add_indices(vc + 0);
					add_indices(vc + 3);
					add_indices(vc + 2);
					add_indices(vc + 0);

					Vector2 uvs[] = {
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_TOP, Vector2(0, 1)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_TOP, Vector2(0, 0)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_TOP, Vector2(1, 0)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_TOP, Vector2(1, 1))
					};

					Vector3 verts[] = {
						Vector3(0.5, 0.5, -0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(-0.5, 0.5, -0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(-0.5, 0.5, 0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(0.5, 0.5, 0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale
					};

					for (int i = 0; i < 4; ++i) {
						add_normal(Vector3(0, 1, 0));

						if (use_lighting)
							add_color(light);

						add_uv(uvs[i]);
						add_vertex(verts[i]);
					}
				}

				//y - 1
				if (neighbours[3] == 0) {
					if (use_lighting) {
						light = Color(chunk->get_voxel(x, y - 1, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
								chunk->get_voxel(x, y - 1, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
								chunk->get_voxel(x, y - 1, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

						float ao = 0;

						if (use_ao)
							ao = chunk->get_voxel(x, y - 1, z, VoxelChunkDefault::DEFAULT_CHANNEL_AO) / 255.0;

						if (use_rao) {
							float rao = chunk->get_voxel(x, y - 1, z, VoxelChunkDefault::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;
							ao += rao;
						}

						light += base_light;

						if (ao > 0)
							light -= Color(ao, ao, ao) * _ao_strength;
					}

					light.r = CLAMP(light.r, 0, 1.0);
					light.g = CLAMP(light.g, 0, 1.0);
					light.b = CLAMP(light.b, 0, 1.0);

					int vc = get_vertex_count();
					add_indices(vc + 0);
					add_indices(vc + 1);
					add_indices(vc + 2);

					add_indices(vc + 0);
					add_indices(vc + 2);
					add_indices(vc + 3);

					Vector2 uvs[] = {
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_BOTTOM, Vector2(0, 1)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_BOTTOM, Vector2(0, 0)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_BOTTOM, Vector2(1, 0)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_BOTTOM, Vector2(1, 1))
					};

					Vector3 verts[] = {
						Vector3(0.5, -0.5, -0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(-0.5, -0.5, -0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(-0.5, -0.5, 0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(0.5, -0.5, 0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale
					};

					for (int i = 0; i < 4; ++i) {
						add_normal(Vector3(0, -1, 0));

						if (use_lighting)
							add_color(light);

						add_uv(uvs[i]);
						add_vertex(verts[i]);
					}
				}

				//z + 1
				if (neighbours[4] == 0) {
					if (use_lighting) {
						light = Color(chunk->get_voxel(x, y, z + 1, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
								chunk->get_voxel(x, y, z + 1, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
								chunk->get_voxel(x, y, z + 1, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

						float ao = 0;

						if (use_ao)
							ao = chunk->get_voxel(x, y, z + 1, VoxelChunkDefault::DEFAULT_CHANNEL_AO) / 255.0;

						if (use_rao) {
							float rao = chunk->get_voxel(x, y, z + 1, VoxelChunkDefault::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;
							ao += rao;
						}

						light += base_light;

						if (ao > 0)
							light -= Color(ao, ao, ao) * _ao_strength;

						light.r = CLAMP(light.r, 0, 1.0);
						light.g = CLAMP(light.g, 0, 1.0);
						light.b = CLAMP(light.b, 0, 1.0);
					}

					int vc = get_vertex_count();
					add_indices(vc + 2);
					add_indices(vc + 1);
					add_indices(vc + 0);
					add_indices(vc + 3);
					add_indices(vc + 2);
					add_indices(vc + 0);

					Vector2 uvs[] = {
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(0, 1)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(0, 0)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(1, 0)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(1, 1))
					};

					Vector3 verts[] = {
						Vector3(0.5, -0.5, 0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(0.5, 0.5, 0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(-0.5, 0.5, 0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(-0.5, -0.5, 0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale
					};

					for (int i = 0; i < 4; ++i) {
						add_normal(Vector3(0, 0, 1));

						if (use_lighting)
							add_color(light);

						add_uv(uvs[i]);
						add_vertex(verts[i]);
					}
				}

				//z - 1
				if (neighbours[5] == 0) {
					if (use_lighting) {
						light = Color(chunk->get_voxel(x, y, z + 1, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
								chunk->get_voxel(x, y, z + 1, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
								chunk->get_voxel(x, y, z + 1, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

						float ao = 0;

						if (use_ao)
							ao = chunk->get_voxel(x, y, z + 1, VoxelChunkDefault::DEFAULT_CHANNEL_AO) / 255.0;

						if (use_rao) {
							float rao = chunk->get_voxel(x, y, z + 1, VoxelChunkDefault::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;
							ao += rao;
						}

						light += base_light;

						if (ao > 0)
							light -= Color(ao, ao, ao) * _ao_strength;

						light.r = CLAMP(light.r, 0, 1.0);
						light.g = CLAMP(light.g, 0, 1.0);
						light.b = CLAMP(light.b, 0, 1.0);
					}

					int vc = get_vertex_count();
					add_indices(vc + 0);
					add_indices(vc + 1);
					add_indices(vc + 2);

					add_indices(vc + 0);
					add_indices(vc + 2);
					add_indices(vc + 3);

					Vector2 uvs[] = {
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(0, 1)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(0, 0)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(1, 0)),
						surface->transform_uv(VoxelSurface::VOXEL_SIDE_SIDE, Vector2(1, 1))
					};

					Vector3 verts[] = {
						Vector3(0.5, -0.5, -0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(0.5, 0.5, -0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(-0.5, 0.5, -0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale,
						Vector3(-0.5, -0.5, -0.5) * voxel_scale + Vector3(x, y, z) * voxel_scale
					};

					for (int i = 0; i < 4; ++i) {
						add_normal(Vector3(0, 0, -1));

						if (use_lighting)
							add_color(light);

						add_uv(uvs[i]);
						add_vertex(verts[i]);
					}
				}
			}
		}
	}
}

VoxelMesherBlocky::VoxelMesherBlocky() {
}

VoxelMesherBlocky::~VoxelMesherBlocky() {
}

void VoxelMesherBlocky::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_add_chunk", "buffer"), &VoxelMesherBlocky::_add_chunk);
}
