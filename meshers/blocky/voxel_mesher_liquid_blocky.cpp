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

#include "voxel_mesher_liquid_blocky.h"

#include "../../world/default/voxel_chunk_default.h"

void VoxelMesherLiquidBlocky::_add_chunk(Ref<VoxelChunk> p_chunk) {
	Ref<VoxelChunkDefault> chunk = p_chunk;

	ERR_FAIL_COND(!chunk.is_valid());

	//if ((get_build_flags() & VoxelChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
	//	chunk->generate_ao();

	int x_size = chunk->get_size_x();
	int y_size = chunk->get_size_y();
	int z_size = chunk->get_size_z();

	float voxel_scale = get_voxel_scale();

	uint8_t *channel_type = chunk->channel_get(VoxelChunkDefault::DEFAULT_CHANNEL_TYPE);

	if (!channel_type)
		return;

	uint8_t *channel_color_r = NULL;
	uint8_t *channel_color_g = NULL;
	uint8_t *channel_color_b = NULL;
	uint8_t *channel_ao = NULL;
	uint8_t *channel_rao = NULL;

	Color base_light(_base_light_value, _base_light_value, _base_light_value);
	Color light;
	bool use_lighting = (get_build_flags() & VoxelChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0;
	bool use_ao = (get_build_flags() & VoxelChunkDefault::BUILD_FLAG_USE_AO) != 0;
	bool use_rao = (get_build_flags() & VoxelChunkDefault::BUILD_FLAG_USE_RAO) != 0;

	if (use_lighting) {
		channel_color_r = chunk->channel_get(VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
		channel_color_g = chunk->channel_get(VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
		channel_color_b = chunk->channel_get(VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);

		if (use_ao)
			channel_ao = chunk->channel_get(VoxelChunkDefault::DEFAULT_CHANNEL_AO);

		if (use_rao)
			channel_rao = chunk->channel_get(VoxelChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);
	}

	Vector<uint8_t> liquids;
	for (int i = 0; i < _library->voxel_surface_get_num(); ++i) {
		Ref<VoxelSurface> surface = _library->voxel_surface_get(i);

		if (!surface.is_valid())
			continue;

		if (surface->get_liquid())
			liquids.push_back(static_cast<uint8_t>(i + 1));
	}

	for (int y = chunk->get_margin_start(); y < y_size + chunk->get_margin_start(); ++y) {
		for (int z = chunk->get_margin_start(); z < z_size + chunk->get_margin_start(); ++z) {
			for (int x = chunk->get_margin_start(); x < x_size + chunk->get_margin_start(); ++x) {

				int index = chunk->get_data_index(x, y, z);
				int indexxp = chunk->get_data_index(x + 1, y, z);
				int indexxn = chunk->get_data_index(x - 1, y, z);
				int indexyp = chunk->get_data_index(x, y + 1, z);
				int indexyn = chunk->get_data_index(x, y - 1, z);
				int indexzp = chunk->get_data_index(x, y, z + 1);
				int indexzn = chunk->get_data_index(x, y, z - 1);

				uint8_t type = channel_type[index];

				if (type == 0)
					continue;

				if (liquids.find(type) == -1)
					continue;

				Ref<VoxelSurface> surface = _library->voxel_surface_get(type - 1);

				if (!surface.is_valid())
					continue;

				uint8_t neighbours[] = {
					channel_type[indexxp],
					channel_type[indexxn],
					channel_type[indexyp],
					channel_type[indexyn],
					channel_type[indexzp],
					channel_type[indexzn],
				};

				//x + 1
				if (neighbours[0] == 0) {
					if (use_lighting) {
						light = Color(channel_color_r[indexxp] / 255.0,
								channel_color_g[indexxp] / 255.0,
								channel_color_b[indexxp] / 255.0);

						float ao = 0;

						if (use_ao)
							ao = channel_ao[indexxp] / 255.0;

						if (use_rao) {
							float rao = channel_rao[indexxp] / 255.0;
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
						Vector3(1, 0, 0) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(1, 1, 0) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(1, 1, 1) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(1, 0, 1) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale
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
						light = Color(channel_color_r[indexxn] / 255.0,
								channel_color_g[indexxn] / 255.0,
								channel_color_b[indexxn] / 255.0);

						float ao = 0;

						if (use_ao)
							ao = channel_ao[indexxn] / 255.0;

						if (use_rao) {
							float rao = channel_rao[indexxn] / 255.0;
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
						Vector3(0, 0, 0) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(0, 1, 0) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(0, 1, 1) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(0, 0, 1) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale
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
						light = Color(channel_color_r[indexyp] / 255.0,
								channel_color_g[indexyp] / 255.0,
								channel_color_b[indexyp] / 255.0);

						float ao = 0;

						if (use_ao)
							ao = channel_ao[indexyp] / 255.0;

						if (use_rao) {
							float rao = channel_rao[indexyp] / 255.0;
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
						Vector3(1, 1, 0) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(0, 1, 0) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(0, 1, 1) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(1, 1, 1) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale
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
						light = Color(channel_color_r[indexyn] / 255.0,
								channel_color_g[indexyn] / 255.0,
								channel_color_b[indexyn] / 255.0);

						float ao = 0;

						if (use_ao)
							ao = channel_ao[indexyn] / 255.0;

						if (use_rao) {
							float rao = channel_rao[indexyn] / 255.0;
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
						Vector3(1, 0, 0) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(0, 0, 0) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(0, 0, 1) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(1, 0, 1) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale
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
						light = Color(channel_color_r[indexzp] / 255.0,
								channel_color_g[indexzp] / 255.0,
								channel_color_b[indexzp] / 255.0);

						float ao = 0;

						if (use_ao)
							ao = channel_ao[indexzp] / 255.0;

						if (use_rao) {
							float rao = channel_rao[indexzp] / 255.0;
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
						Vector3(1, 0, 1) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(1, 1, 1) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(0, 1, 1) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(0, 0, 1) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale
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
						light = Color(channel_color_r[indexzn] / 255.0,
								channel_color_g[indexzn] / 255.0,
								channel_color_b[indexzn] / 255.0);

						float ao = 0;

						if (use_ao)
							ao = channel_ao[indexzn] / 255.0;

						if (use_rao) {
							float rao = channel_rao[indexzn] / 255.0;
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
						Vector3(1, 0, 0) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(1, 1, 0) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(0, 1, 0) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale,
						Vector3(0, 0, 0) * voxel_scale + Vector3(x - 1, y - 1, z - 1) * voxel_scale
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

VoxelMesherLiquidBlocky::VoxelMesherLiquidBlocky() {
}

VoxelMesherLiquidBlocky::~VoxelMesherLiquidBlocky() {
}

void VoxelMesherLiquidBlocky::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_add_chunk", "buffer"), &VoxelMesherLiquidBlocky::_add_chunk);
}
