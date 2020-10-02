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

#include "voxel_prop_job.h"

#include "../../defines.h"

#include "../../library/voxel_surface.h"
#include "../../library/voxelman_library.h"

#include "../../meshers/voxel_mesher.h"
#include "../default/voxel_chunk_default.h"

#ifdef MESH_DATA_RESOURCE_PRESENT
#include "../../../mesh_data_resource/mesh_data_resource.h"
#endif

#include "../../world/default/voxel_world_default.h"

Ref<VoxelMesher> VoxelPropJob::get_prop_mesher() const {
	return _prop_mesher;
}
void VoxelPropJob::set_prop_mesher(const Ref<VoxelMesher> &mesher) {
	_prop_mesher = mesher;
}

void VoxelPropJob::phase_reset() {
	if (get_prop_mesher().is_valid()) {
		get_prop_mesher()->reset();
		get_prop_mesher()->set_library(_chunk->get_library());
	}
}

void VoxelPropJob::phase_prop() {
#ifdef MESH_DATA_RESOURCE_PRESENT
	Ref<VoxelChunkDefault> chunk = _chunk;

	if (!get_prop_mesher().is_valid()) {
		//next_phase();
		return;
	}

	if (should_do()) {
		if (chunk->get_mesh_data_resource_count() == 0) {
			//next_phase();
			return;
		}

		for (int i = 0; i < chunk->get_mesh_data_resource_count(); ++i) {
			if (chunk->get_mesh_data_resource_is_inside(i)) {
				get_prop_mesher()->add_mesh_data_resource_transform(chunk->get_mesh_data_resource(i), chunk->get_mesh_data_resource_transform(i), chunk->get_mesh_data_resource_uv_rect(i));
			}
		}

		if (get_prop_mesher()->get_vertex_count() == 0) {
			reset_stages();

			//next_phase();
			return;
		}

		if (should_return()) {
			return;
		}
	}

	if (should_do()) {
		if ((chunk->get_build_flags() & VoxelChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
			get_prop_mesher()->bake_colors(_chunk);
		}

		if (should_return()) {
			return;
		}
	}

	if (should_do()) {
		if ((chunk->get_build_flags() & VoxelChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
			VoxelWorldDefault *world = Object::cast_to<VoxelWorldDefault>(chunk->get_voxel_world());

			if (world) {
				for (int i = 0; i < chunk->get_mesh_data_resource_count(); ++i) {
					if (!chunk->get_mesh_data_resource_is_inside(i)) {
						Ref<MeshDataResource> mdr = chunk->get_mesh_data_resource(i);

						ERR_CONTINUE(!mdr.is_valid());

						Transform trf = chunk->get_mesh_data_resource_transform(i);

						Array arr = mdr->get_array();

						if (arr.size() <= Mesh::ARRAY_VERTEX) {
							continue;
						}

						PoolVector3Array varr = arr[Mesh::ARRAY_VERTEX];

						if (varr.size() == 0) {
							continue;
						}

						PoolColorArray carr = world->get_vertex_colors(trf, varr);

						get_prop_mesher()->add_mesh_data_resource_transform_colored(mdr, trf, carr, chunk->get_mesh_data_resource_uv_rect(i));
					}
				}
			}
		}

		if (should_return()) {
			return;
		}
	}

	if (get_prop_mesher()->get_vertex_count() != 0) {
		if (should_do()) {
			temp_mesh_arr = get_prop_mesher()->build_mesh();

			if (should_return()) {
				return;
			}
		}

		RID mesh_rid = chunk->get_mesh_rid_index(VoxelChunkDefault::MESH_INDEX_PROP, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 0);

		if (should_do()) {
			if (mesh_rid == RID()) {
				if ((chunk->get_build_flags() & VoxelChunkDefault::BUILD_FLAG_CREATE_LODS) != 0)
					chunk->create_meshes(VoxelChunkDefault::MESH_INDEX_PROP, chunk->get_lod_num() + 1);
				else
					chunk->create_meshes(VoxelChunkDefault::MESH_INDEX_PROP, 1);

				mesh_rid = chunk->get_mesh_rid_index(VoxelChunkDefault::MESH_INDEX_PROP, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 0);
			}

			if (VS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0)
#if !GODOT4
				VS::get_singleton()->mesh_remove_surface(mesh_rid, 0);
#else
				VS::get_singleton()->mesh_clear(mesh_rid);
#endif

			if (should_return()) {
				return;
			}
		}

		if (should_do()) {

			VS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

			if (chunk->get_library()->get_prop_material(0).is_valid())
				VS::get_singleton()->mesh_surface_set_material(mesh_rid, 0, chunk->get_library()->get_prop_material(0)->get_rid());

			if (should_return()) {
				return;
			}
		}

		if ((chunk->get_build_flags() & VoxelChunkDefault::BUILD_FLAG_CREATE_LODS) != 0) {
			if (should_do()) {

				if (chunk->get_lod_num() >= 1) {
					//for lod 1 just remove uv2
					temp_mesh_arr[VisualServer::ARRAY_TEX_UV2] = Variant();

					VisualServer::get_singleton()->mesh_add_surface_from_arrays(chunk->get_mesh_rid_index(VoxelChunkDefault::MESH_INDEX_PROP, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 1), VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

					if (chunk->get_library()->get_prop_material(1).is_valid())
						VisualServer::get_singleton()->mesh_surface_set_material(chunk->get_mesh_rid_index(VoxelChunkDefault::MESH_INDEX_PROP, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 1), 0, chunk->get_library()->get_prop_material(1)->get_rid());
				}

				if (should_return()) {
					return;
				}
			}

			if (should_do()) {
				if (chunk->get_lod_num() >= 2) {
					Array temp_mesh_arr2 = chunk->merge_mesh_array(temp_mesh_arr);
					temp_mesh_arr = temp_mesh_arr2;

					VisualServer::get_singleton()->mesh_add_surface_from_arrays(chunk->get_mesh_rid_index(VoxelChunkDefault::MESH_INDEX_PROP, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 2), VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr2);

					if (chunk->get_library()->get_prop_material(2).is_valid())
						VisualServer::get_singleton()->mesh_surface_set_material(chunk->get_mesh_rid_index(VoxelChunkDefault::MESH_INDEX_PROP, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 2), 0, chunk->get_library()->get_prop_material(2)->get_rid());
				}
				if (should_return()) {
					return;
				}
			}

			//	if (should_do()) {
			if (chunk->get_lod_num() >= 3) {
				Ref<ShaderMaterial> mat = chunk->get_library()->get_prop_material(0);
				Ref<SpatialMaterial> spmat = chunk->get_library()->get_prop_material(0);
				Ref<Texture> tex;

				if (mat.is_valid()) {
					tex = mat->get_shader_param("texture_albedo");
				} else if (spmat.is_valid()) {
					tex = spmat->get_texture(SpatialMaterial::TEXTURE_ALBEDO);
				}

				if (tex.is_valid()) {
					temp_mesh_arr = chunk->bake_mesh_array_uv(temp_mesh_arr, tex);
					temp_mesh_arr[VisualServer::ARRAY_TEX_UV] = Variant();

					VisualServer::get_singleton()->mesh_add_surface_from_arrays(chunk->get_mesh_rid_index(VoxelChunkDefault::MESH_INDEX_PROP, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 3), VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

					if (chunk->get_library()->get_prop_material(3).is_valid())
						VisualServer::get_singleton()->mesh_surface_set_material(chunk->get_mesh_rid_index(VoxelChunkDefault::MESH_INDEX_PROP, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 3), 0, chunk->get_library()->get_prop_material(3)->get_rid());
				}
			}

			/*
					if (should_do()) {
						if (chunk->get_lod_num() > 4) {
							Ref<FastQuadraticMeshSimplifier> fqms;
							fqms.instance();
							fqms->initialize(temp_mesh_arr);

							Array arr_merged_simplified;

							for (int i = 4; i < chunk->get_lod_num(); ++i) {
								fqms->simplify_mesh(arr_merged_simplified[0].size() * 0.8, 7);
								arr_merged_simplified = fqms->get_arrays();

								if (arr_merged_simplified[0].size() == 0)
									break;

								VisualServer::get_singleton()->mesh_add_surface_from_arrays(get_mesh_rid_index(MESH_INDEX_PROP, MESH_TYPE_INDEX_MESH, i), VisualServer::PRIMITIVE_TRIANGLES, arr_merged_simplified);

								if (get_library()->get_material(i).is_valid())
									VisualServer::get_singleton()->mesh_surface_set_material(get_mesh_rid_index(MESH_INDEX_PROP, MESH_TYPE_INDEX_MESH, i), 0, get_library()->get_material(i)->get_rid());
							}
						}
						if (should_return()) {
							return;
						}
					}
					*/
		}
	}

	//next_phase();

	return;
#endif
}

void VoxelPropJob::_execute() {
	ERR_FAIL_COND(!_chunk.is_valid());

	Ref<VoxelmanLibrary> library = _chunk->get_library();

	ERR_FAIL_COND(!library.is_valid());

	phase_prop();

	//finish
}

VoxelPropJob::VoxelPropJob() {
}

VoxelPropJob::~VoxelPropJob() {
}

void VoxelPropJob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_prop_mesher"), &VoxelPropJob::get_prop_mesher);
	ClassDB::bind_method(D_METHOD("set_prop_mesher", "mesher"), &VoxelPropJob::set_prop_mesher);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop_mesher", PROPERTY_HINT_RESOURCE_TYPE, "VoxelMesher", 0), "set_prop_mesher", "get_prop_mesher");
}
