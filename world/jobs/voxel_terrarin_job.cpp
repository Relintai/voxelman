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

#include "voxel_terrarin_job.h"

#include "../../defines.h"

#include "../../library/voxel_surface.h"
#include "../../library/voxelman_library.h"

#include "../../meshers/voxel_mesher.h"
#include "../default/voxel_chunk_default.h"

Ref<VoxelMesher> VoxelTerrarinJob::get_mesher(int index) const {
	ERR_FAIL_INDEX_V(index, _meshers.size(), Ref<VoxelMesher>());

	return _meshers.get(index);
}
void VoxelTerrarinJob::set_mesher(int index, const Ref<VoxelMesher> &mesher) {
	ERR_FAIL_INDEX(index, _meshers.size());

	_meshers.set(index, mesher);
}
void VoxelTerrarinJob::remove_mesher(const int index) {
	ERR_FAIL_INDEX(index, _meshers.size());

	_meshers.remove(index);
}
void VoxelTerrarinJob::add_mesher(const Ref<VoxelMesher> &mesher) {
	_meshers.push_back(mesher);
}
int VoxelTerrarinJob::get_mesher_count() const {
	return _meshers.size();
}

Ref<VoxelMesher> VoxelTerrarinJob::get_liquid_mesher(int index) const {
	ERR_FAIL_INDEX_V(index, _liquid_meshers.size(), Ref<VoxelMesher>());

	return _liquid_meshers.get(index);
}
void VoxelTerrarinJob::set_liquid_mesher(int index, const Ref<VoxelMesher> &mesher) {
	ERR_FAIL_INDEX(index, _liquid_meshers.size());

	_liquid_meshers.set(index, mesher);
}
void VoxelTerrarinJob::remove_liquid_mesher(const int index) {
	ERR_FAIL_INDEX(index, _liquid_meshers.size());

	_liquid_meshers.remove(index);
}
void VoxelTerrarinJob::add_liquid_mesher(const Ref<VoxelMesher> &mesher) {
	_liquid_meshers.push_back(mesher);
}
int VoxelTerrarinJob::get_liquid_mesher_count() const {
	return _liquid_meshers.size();
}

void VoxelTerrarinJob::_execute() {
	ERR_FAIL_COND(!_chunk.is_valid());

	Ref<VoxelmanLibrary> library = _chunk->get_library();

	ERR_FAIL_COND(!library.is_valid());
	/*
	int phase = 0;

	switch (phase) {
		case BUILD_PHASE_SETUP: {
			for (int i = 0; i < _meshers.size(); ++i) {
				Ref<VoxelMesher> mesher = _meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				mesher->set_library(_library);
				mesher->reset();
			}

			for (int i = 0; i < _liquid_meshers.size(); ++i) {
				Ref<VoxelMesher> mesher = _liquid_meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				mesher->set_library(_library);
				mesher->reset();
			}

			if (get_prop_mesher().is_valid()) {
				get_prop_mesher()->reset();
				get_prop_mesher()->set_library(_library);
			}

			next_phase();
			return;
		}
		case BUILD_PHASE_TERRARIN_MESH_SETUP: {
			int starti = 0;

			if (_job->has_meta("tms_m")) {
				starti = _job->get_meta("tms_m");
			}

			for (int i = starti; i < _meshers.size(); ++i) {
				if (_job->should_return()) {
					_job->set_meta("tms_m", i);
					return;
				}

				Ref<VoxelMesher> mesher = _meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				mesher->add_chunk(this);
			}

			starti = 0;

			if (_job->has_meta("tms_lm")) {
				starti = _job->get_meta("tms_lm");
			}

			for (int i = starti; i < _liquid_meshers.size(); ++i) {
				if (_job->should_return()) {
					_job->set_meta("tms_lm", i);
					return;
				}

				Ref<VoxelMesher> mesher = _liquid_meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				mesher->add_chunk(this);
			}

			next_phase();

			if (_job->has_meta("tms_m")) {
				_job->remove_meta("tms_m");
			}

			if (_job->has_meta("tms_lm")) {
				_job->remove_meta("tms_lm");
			}

			return;
		}
		case BUILD_PHASE_COLLIDER: {
			if ((_build_flags & BUILD_FLAG_CREATE_COLLIDER) == 0) {
				next_phase();
				return;
			}

			int starti = 0;

			if (_job->has_meta("bpc_aa")) {
				starti = _job->get_meta("bpc_aa");
			}

			for (int i = starti; i < _meshers.size(); ++i) {
				if (_job->should_return()) {
					_job->set_meta("bpc_aa", i);
					return;
				}

				Ref<VoxelMesher> mesher = _meshers.get(i);

				ERR_CONTINUE(!mesher.is_valid());

				temp_arr_collider.append_array(mesher->build_collider());
			}

			if (Engine::get_singleton()->is_editor_hint()) {

				starti = 0;

				if (_job->has_meta("bpc_laa")) {
					starti = _job->get_meta("bpc_laa");
				}

				for (int i = 0; i < _liquid_meshers.size(); ++i) {
					if (_job->should_return()) {
						_job->set_meta("bpc_laa", i);
						return;
					}

					Ref<VoxelMesher> mesher = _liquid_meshers.get(i);

					ERR_CONTINUE(!mesher.is_valid());

					temp_arr_collider_liquid.append_array(mesher->build_collider());
				}
			}

			if (_job->has_meta("bpc_aa")) {
				_job->remove_meta("bpc_aa");
			}

			if (_job->has_meta("bpc_laa")) {
				_job->remove_meta("bpc_laa");
			}

			if (temp_arr_collider.size() == 0 && temp_arr_collider_liquid.size() == 0
#ifdef MESH_DATA_RESOURCE_PRESENT
					&& get_mesh_data_resource_count() == 0
#endif
			) {
				next_phase();
				return;
			}

			set_active_build_phase_type(BUILD_PHASE_TYPE_PHYSICS_PROCESS);
			return;
		}
		case BUILD_PHASE_LIGHTS: {
			bool gr = (_build_flags & BUILD_FLAG_AUTO_GENERATE_RAO) != 0;

			if (!gr && (_build_flags & BUILD_FLAG_USE_LIGHTING) == 0) {
				next_phase();
				return;
			}

			bool bl = (_build_flags & BUILD_FLAG_BAKE_LIGHTS) != 0;

			if (bl && _job->should_do()) {
				clear_baked_lights();

				if (_job->should_return())
					return;
			}

			if (gr && _job->should_do()) {
				generate_random_ao(_voxel_world->get_current_seed());

				if (_job->should_return())
					return;
			}

			if (bl && _job->should_do()) {
				bake_lights();

				if (_job->should_return())
					return;
			}

			_job->reset_stages();

			next_phase();

			return;
		}
		case BUILD_PHASE_TERRARIN_MESH: {
			if (_job->should_do()) {
				for (int i = 0; i < _meshers.size(); ++i) {
					Ref<VoxelMesher> mesher = _meshers.get(i);

					ERR_CONTINUE(!mesher.is_valid());

					mesher->set_library(_library);
				}

				for (int i = 0; i < _liquid_meshers.size(); ++i) {
					Ref<VoxelMesher> mesher = _liquid_meshers.get(i);

					ERR_CONTINUE(!mesher.is_valid());

					mesher->set_library(_library);
				}

				if (_job->should_return())
					return;
			}

			if ((_build_flags & VoxelTerrarinJob::BUILD_FLAG_USE_LIGHTING) != 0) {
				int starti = 0;

				if (_job->has_meta("bptm_ulm")) {
					starti = _job->get_meta("bptm_ulm");
				}

				for (int i = starti; i < _meshers.size(); ++i) {
					if (_job->should_return()) {
						_job->set_meta("bptm_ulm", i);
					}

					Ref<VoxelMesher> mesher = _meshers.get(i);

					ERR_CONTINUE(!mesher.is_valid());

					mesher->bake_colors(this);
				}

				starti = 0;

				if (_job->has_meta("bptm_ullm")) {
					starti = _job->get_meta("bptm_ullm");
				}

				for (int i = starti; i < _liquid_meshers.size(); ++i) {
					if (_job->should_return()) {
						_job->set_meta("bptm_ullm", i);
					}

					Ref<VoxelMesher> mesher = _liquid_meshers.get(i);

					ERR_CONTINUE(!mesher.is_valid());

					mesher->bake_colors(this);
				}
			}

			int starti = 0;

			if (_job->has_meta("bptm_mm")) {
				starti = _job->get_meta("bptm_mm");
			}

			Ref<VoxelMesher> mesher;
			for (int i = starti; i < _meshers.size(); ++i) {
				if (_job->should_return()) {
					_job->set_meta("bptm_mm", i);
				}

				Ref<VoxelMesher> m = _meshers.get(i);

				ERR_CONTINUE(!m.is_valid());

				if (!mesher.is_valid()) {
					mesher = m;
					mesher->set_material(get_library()->get_material(0));
					continue;
				}

				mesher->set_material(get_library()->get_material(0));
				mesher->add_mesher(m);
			}

			ERR_FAIL_COND(!mesher.is_valid());

			starti = 0;

			if (_job->has_meta("bptm_lmm")) {
				starti = _job->get_meta("bptm_lmm");
			}

			Ref<VoxelMesher> liquid_mesher;
			for (int i = starti; i < _liquid_meshers.size(); ++i) {
				if (_job->should_return()) {
					_job->set_meta("bptm_lmm", i);
				}

				Ref<VoxelMesher> m = _liquid_meshers.get(i);

				ERR_CONTINUE(!m.is_valid());

				if (!liquid_mesher.is_valid()) {
					liquid_mesher = m;
					liquid_mesher->set_material(get_library()->get_material(0));
					continue;
				}

				liquid_mesher->set_material(get_library()->get_material(0));
				liquid_mesher->add_mesher(m);
			}

			if (mesher->get_vertex_count() == 0 && liquid_mesher.is_valid() && liquid_mesher->get_vertex_count() == 0) {

				if (_job->has_meta("bptm_ulm")) {
					_job->remove_meta("bptm_ulm");
				}

				if (_job->has_meta("bptm_ullm")) {
					_job->remove_meta("bptm_ullm");
				}

				if (_job->has_meta("bptm_mm")) {
					_job->remove_meta("bptm_mm");
				}

				if (_job->has_meta("bptm_lmm")) {
					_job->remove_meta("bptm_lmm");
				}

				_job->reset_stages();

				next_phase();
				return;
			}

			if (mesher->get_vertex_count() != 0) {

				if (_job->should_do()) {
					temp_mesh_arr = mesher->build_mesh();

					if (_job->should_return()) {
						return;
					}
				}

				RID mesh_rid = get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 0);

				if (_job->should_do()) {
					if (mesh_rid == RID()) {
						if ((_build_flags & BUILD_FLAG_CREATE_LODS) != 0)
							create_meshes(MESH_INDEX_TERRARIN, _lod_num + 1);
						else
							create_meshes(MESH_INDEX_TERRARIN, 1);

						mesh_rid = get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 0);
					}

					if (VS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0)
#if !GODOT4
						VS::get_singleton()->mesh_remove_surface(mesh_rid, 0);
#else
						VS::get_singleton()->mesh_clear(mesh_rid);
#endif

					if (_job->should_return()) {
						return;
					}
				}

				if (_job->should_do()) {
					VS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

					if (_library->get_material(0).is_valid())
						VS::get_singleton()->mesh_surface_set_material(mesh_rid, 0, _library->get_material(0)->get_rid());

					if (_job->should_return()) {
						return;
					}
				}

				if ((_build_flags & BUILD_FLAG_CREATE_LODS) != 0) {
					if (_job->should_do()) {
						if (_lod_num >= 1) {
							//for lod 1 just remove uv2
							temp_mesh_arr[VisualServer::ARRAY_TEX_UV2] = Variant();

							VisualServer::get_singleton()->mesh_add_surface_from_arrays(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 1), VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

							if (get_library()->get_material(1).is_valid())
								VisualServer::get_singleton()->mesh_surface_set_material(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 1), 0, get_library()->get_material(1)->get_rid());
						}
						if (_job->should_return()) {
							return;
						}
					}

					if (_job->should_do()) {
						if (_lod_num >= 2) {
							Array temp_mesh_arr2 = merge_mesh_array(temp_mesh_arr);
							temp_mesh_arr = temp_mesh_arr2;

							VisualServer::get_singleton()->mesh_add_surface_from_arrays(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 2), VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr2);

							if (get_library()->get_material(2).is_valid())
								VisualServer::get_singleton()->mesh_surface_set_material(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 2), 0, get_library()->get_material(2)->get_rid());
						}

						if (_job->should_return()) {
							return;
						}
					}

					if (_job->should_do()) {
						if (_lod_num >= 3) {
							Ref<ShaderMaterial> mat = get_library()->get_material(0);
							Ref<SpatialMaterial> spmat = get_library()->get_material(0);
							Ref<Texture> tex;

							if (mat.is_valid()) {
								tex = mat->get_shader_param("texture_albedo");
							} else if (spmat.is_valid()) {
								tex = spmat->get_texture(SpatialMaterial::TEXTURE_ALBEDO);
							}

							if (tex.is_valid()) {
								temp_mesh_arr = bake_mesh_array_uv(temp_mesh_arr, tex);
								temp_mesh_arr[VisualServer::ARRAY_TEX_UV] = Variant();

								VisualServer::get_singleton()->mesh_add_surface_from_arrays(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 3), VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

								if (get_library()->get_material(3).is_valid())
									VisualServer::get_singleton()->mesh_surface_set_material(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, 3), 0, get_library()->get_material(3)->get_rid());
							}
						}

						if (_job->should_return()) {
							return;
						}
					}

					//----
					if (_job->should_do()) {
						if (_lod_num > 4) {
							Ref<FastQuadraticMeshSimplifier> fqms;
							fqms.instance();
							fqms->initialize(temp_mesh_arr);

							Array arr_merged_simplified;

							for (int i = 4; i < _lod_num; ++i) {
								fqms->simplify_mesh(arr_merged_simplified[0].size() * 0.8, 7);
								arr_merged_simplified = fqms->get_arrays();

								if (arr_merged_simplified[0].size() == 0)
									break;

								VisualServer::get_singleton()->mesh_add_surface_from_arrays(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, i), VisualServer::PRIMITIVE_TRIANGLES, arr_merged_simplified);

								if (get_library()->get_material(i).is_valid())
									VisualServer::get_singleton()->mesh_surface_set_material(get_mesh_rid_index(MESH_INDEX_TERRARIN, MESH_TYPE_INDEX_MESH, i), 0, get_library()->get_material(i)->get_rid());
							}
						}

						if (_job->should_return()) {
							return;
						}
					}
					//----
}
}

if (liquid_mesher.is_valid() && liquid_mesher->get_vertex_count() != 0) {
	if (_job->should_do()) {
		temp_mesh_arr = liquid_mesher->build_mesh();

		if (_job->should_return()) {
			return;
		}
	}

	RID mesh_rid = get_mesh_rid_index(MESH_INDEX_LIQUID, MESH_TYPE_INDEX_MESH, 0);

	if (_job->should_do()) {
		if (mesh_rid == RID()) {
			create_meshes(MESH_INDEX_LIQUID, 1);

			mesh_rid = get_mesh_rid_index(MESH_INDEX_LIQUID, MESH_TYPE_INDEX_MESH, 0);
		}

		if (VS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0)
#if !GODOT4
			VS::get_singleton()->mesh_remove_surface(mesh_rid, 0);
#else
			VS::get_singleton()->mesh_clear(mesh_rid);
#endif

		if (_job->should_return()) {
			return;
		}
	}

	//	if (_job->should_do()) {
	VS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	if (_library->get_liquid_material(0).is_valid())
		VS::get_singleton()->mesh_surface_set_material(mesh_rid, 0, _library->get_liquid_material(0)->get_rid());

	//	if (_job->should_return()) {
	//		return;
	//	}
	//}
}

if (_job->has_meta("bptm_ulm")) {
	_job->remove_meta("bptm_ulm");
}

if (_job->has_meta("bptm_ullm")) {
	_job->remove_meta("bptm_ullm");
}

if (_job->has_meta("bptm_mm")) {
	_job->remove_meta("bptm_mm");
}

if (_job->has_meta("bptm_lmm")) {
	_job->remove_meta("bptm_lmm");
}

_job->reset_stages();

next_phase();

return;
}

case BUILD_PHASE_FINALIZE: {
#if TOOLS_ENABLED
	if (_debug_mesh_array.size() > 0) {
		debug_mesh_send();
	}
#endif

	set_current_lod_level(get_current_lod_level());

	call_deferred("update_transforms");

	next_phase();

	return;
}
	}

	next_phase();
	*/
}

VoxelTerrarinJob::VoxelTerrarinJob() {
}

VoxelTerrarinJob::~VoxelTerrarinJob() {
}

void VoxelTerrarinJob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesher", "index"), &VoxelTerrarinJob::get_mesher);
	ClassDB::bind_method(D_METHOD("set_mesher", "index", "mesher"), &VoxelTerrarinJob::set_mesher);
	ClassDB::bind_method(D_METHOD("remove_mesher", "index"), &VoxelTerrarinJob::remove_mesher);
	ClassDB::bind_method(D_METHOD("add_mesher", "mesher"), &VoxelTerrarinJob::add_mesher);
	ClassDB::bind_method(D_METHOD("get_mesher_count"), &VoxelTerrarinJob::get_mesher_count);

	ClassDB::bind_method(D_METHOD("get_liquid_mesher", "index"), &VoxelTerrarinJob::get_liquid_mesher);
	ClassDB::bind_method(D_METHOD("set_liquid_mesher", "index", "mesher"), &VoxelTerrarinJob::set_liquid_mesher);
	ClassDB::bind_method(D_METHOD("remove_liquid_mesher", "index"), &VoxelTerrarinJob::remove_liquid_mesher);
	ClassDB::bind_method(D_METHOD("add_liquid_mesher", "mesher"), &VoxelTerrarinJob::add_liquid_mesher);
	ClassDB::bind_method(D_METHOD("get_liquid_mesher_count"), &VoxelTerrarinJob::get_liquid_mesher_count);
}
