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

#include "../../library/voxel_surface.h"
#include "../../library/voxelman_library.h"

#include "../../meshers/default/voxel_mesher_default.h"
#include "../../meshers/voxel_mesher.h"

#include "../default/voxel_chunk_default.h"

#ifdef MESH_UTILS_PRESENT
#include "../../../mesh_utils/fast_quadratic_mesh_simplifier.h"
#endif

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

void VoxelTerrarinJob::phase_setup() {
	for (int i = 0; i < _meshers.size(); ++i) {
		Ref<VoxelMesher> mesher = _meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->set_library(_chunk->get_library());
		mesher->reset();
	}

	for (int i = 0; i < _liquid_meshers.size(); ++i) {
		Ref<VoxelMesher> mesher = _liquid_meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->set_library(_chunk->get_library());
		mesher->reset();
	}

	next_phase();
}

void VoxelTerrarinJob::phase_terrarin_mesh_setup() {
	int starti = 0;

	if (has_meta("tms_m")) {
		starti = get_meta("tms_m");
	}

	for (int i = starti; i < _meshers.size(); ++i) {
		if (should_return()) {
			set_meta("tms_m", i);
			return;
		}

		Ref<VoxelMesher> mesher = _meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->add_chunk(_chunk);
	}

	starti = 0;

	if (has_meta("tms_lm")) {
		starti = get_meta("tms_lm");
	}

	for (int i = starti; i < _liquid_meshers.size(); ++i) {
		if (should_return()) {
			set_meta("tms_lm", i);
			return;
		}

		Ref<VoxelMesher> mesher = _liquid_meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->add_chunk(_chunk);
	}

	if (has_meta("tms_m")) {
		remove_meta("tms_m");
	}

	if (has_meta("tms_lm")) {
		remove_meta("tms_lm");
	}

	next_phase();
}

void VoxelTerrarinJob::phase_collider() {
	Ref<VoxelChunkDefault> chunk = _chunk;

	if ((chunk->get_build_flags() & VoxelChunkDefault::BUILD_FLAG_CREATE_COLLIDER) == 0) {
		next_phase();
		return;
	}

	int starti = 0;

	if (has_meta("bpc_aa")) {
		starti = get_meta("bpc_aa");
	}

	for (int i = starti; i < _meshers.size(); ++i) {
		if (should_return()) {
			set_meta("bpc_aa", i);
			return;
		}

		Ref<VoxelMesher> mesher = _meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		temp_arr_collider.append_array(mesher->build_collider());
	}

	if (Engine::get_singleton()->is_editor_hint()) {
		starti = 0;

		if (has_meta("bpc_laa")) {
			starti = get_meta("bpc_laa");
		}

		for (int i = 0; i < _liquid_meshers.size(); ++i) {
			if (should_return()) {
				set_meta("bpc_laa", i);
				return;
			}

			Ref<VoxelMesher> mesher = _liquid_meshers.get(i);

			ERR_CONTINUE(!mesher.is_valid());

			temp_arr_collider_liquid.append_array(mesher->build_collider());
		}
	}

	if (has_meta("bpc_aa")) {
		remove_meta("bpc_aa");
	}

	if (has_meta("bpc_laa")) {
		remove_meta("bpc_laa");
	}

	if (temp_arr_collider.size() == 0 && temp_arr_collider_liquid.size() == 0) {
		next_phase();
		next_phase();
		return;
	}

	set_build_phase_type(BUILD_PHASE_TYPE_PHYSICS_PROCESS);
	next_phase();
}

void VoxelTerrarinJob::phase_physics_process() {
	Ref<VoxelChunkDefault> chunk = _chunk;

	if (temp_arr_collider.size() != 0) {
		if (!chunk->meshes_has(VoxelChunkDefault::MESH_INDEX_TERRARIN, VoxelChunkDefault::MESH_TYPE_INDEX_BODY)) {
			chunk->colliders_create(VoxelChunkDefault::MESH_INDEX_TERRARIN);
		}

		PhysicsServer::get_singleton()->shape_set_data(chunk->mesh_rid_get(VoxelChunkDefault::MESH_INDEX_TERRARIN, VoxelChunkDefault::MESH_TYPE_INDEX_SHAPE), temp_arr_collider);

		temp_arr_collider.resize(0);
	}

	if (temp_arr_collider_liquid.size() != 0) {
		if (Engine::get_singleton()->is_editor_hint()) {
			if (!chunk->meshes_has(VoxelChunkDefault::MESH_INDEX_LIQUID, VoxelChunkDefault::MESH_TYPE_INDEX_BODY)) {
				chunk->colliders_create(VoxelChunkDefault::MESH_INDEX_LIQUID);
			}
		}
		/*
			else {
				if (!has_meshes(MESH_INDEX_LIQUID, MESH_TYPE_INDEX_AREA)) {
					create_colliders_area(MESH_INDEX_LIQUID);
				}
			}*/

		PhysicsServer::get_singleton()->shape_set_data(chunk->mesh_rid_get(VoxelChunkDefault::MESH_INDEX_LIQUID, VoxelChunkDefault::MESH_TYPE_INDEX_SHAPE), temp_arr_collider_liquid);

		temp_arr_collider_liquid.resize(0);
	}

	set_build_phase_type(BUILD_PHASE_TYPE_NORMAL);
	next_phase();
}

void VoxelTerrarinJob::phase_terrarin_mesh() {
	Ref<VoxelChunkDefault> chunk = _chunk;

	if ((chunk->get_build_flags() & VoxelChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
		int starti = 0;

		if (has_meta("bptm_ulm")) {
			starti = get_meta("bptm_ulm");
		}

		for (int i = starti; i < _meshers.size(); ++i) {
			if (should_return()) {
				set_meta("bptm_ulm", i);
			}

			Ref<VoxelMesher> mesher = _meshers.get(i);

			ERR_CONTINUE(!mesher.is_valid());

			mesher->bake_colors(_chunk);
		}

		starti = 0;

		if (has_meta("bptm_ullm")) {
			starti = get_meta("bptm_ullm");
		}

		for (int i = starti; i < _liquid_meshers.size(); ++i) {
			if (should_return()) {
				set_meta("bptm_ullm", i);
			}

			Ref<VoxelMesher> mesher = _liquid_meshers.get(i);

			ERR_CONTINUE(!mesher.is_valid());

			mesher->bake_colors(_chunk);
		}
	}

	int starti = 0;

	if (has_meta("bptm_mm")) {
		starti = get_meta("bptm_mm");
	}

	Ref<VoxelMesher> mesher;
	for (int i = starti; i < _meshers.size(); ++i) {
		if (should_return()) {
			set_meta("bptm_mm", i);
		}

		Ref<VoxelMesher> m = _meshers.get(i);

		ERR_CONTINUE(!m.is_valid());

		if (!mesher.is_valid()) {
			mesher = m;
			mesher->set_material(_chunk->get_library()->material_get(0));
			continue;
		}

		mesher->set_material(_chunk->get_library()->material_get(0));
		mesher->add_mesher(m);
	}

	ERR_FAIL_COND(!mesher.is_valid());

	starti = 0;

	if (has_meta("bptm_lmm")) {
		starti = get_meta("bptm_lmm");
	}

	Ref<VoxelMesher> liquid_mesher;
	for (int i = starti; i < _liquid_meshers.size(); ++i) {
		if (should_return()) {
			set_meta("bptm_lmm", i);
		}

		Ref<VoxelMesher> m = _liquid_meshers.get(i);

		ERR_CONTINUE(!m.is_valid());

		if (!liquid_mesher.is_valid()) {
			liquid_mesher = m;
			liquid_mesher->set_material(_chunk->get_library()->material_get(0));
			continue;
		}

		liquid_mesher->set_material(_chunk->get_library()->material_get(0));
		liquid_mesher->add_mesher(m);
	}

	if (mesher->get_vertex_count() == 0 && liquid_mesher.is_valid() && liquid_mesher->get_vertex_count() == 0) {
		if (has_meta("bptm_ulm")) {
			remove_meta("bptm_ulm");
		}

		if (has_meta("bptm_ullm")) {
			remove_meta("bptm_ullm");
		}

		if (has_meta("bptm_mm")) {
			remove_meta("bptm_mm");
		}

		if (has_meta("bptm_lmm")) {
			remove_meta("bptm_lmm");
		}

		reset_stages();
		next_phase();

		return;
	}

	if (mesher->get_vertex_count() != 0) {
		if (should_do()) {
			temp_mesh_arr = mesher->build_mesh();

			if (should_return()) {
				return;
			}
		}

		RID mesh_rid = chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRARIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 0);

		if (should_do()) {
			if (mesh_rid == RID()) {
				if ((chunk->get_build_flags() & VoxelChunkDefault::BUILD_FLAG_CREATE_LODS) != 0)
					chunk->meshes_create(VoxelChunkDefault::MESH_INDEX_TERRARIN, chunk->get_lod_num() + 1);
				else
					chunk->meshes_create(VoxelChunkDefault::MESH_INDEX_TERRARIN, 1);

				mesh_rid = chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRARIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 0);
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

			if (chunk->get_library()->material_get(0).is_valid())
				VS::get_singleton()->mesh_surface_set_material(mesh_rid, 0, chunk->get_library()->material_get(0)->get_rid());

			if (should_return()) {
				return;
			}
		}

		if ((chunk->get_build_flags() & VoxelChunkDefault::BUILD_FLAG_CREATE_LODS) != 0) {
			if (should_do()) {
				if (chunk->get_lod_num() >= 1) {
					//for lod 1 just remove uv2
					temp_mesh_arr[VisualServer::ARRAY_TEX_UV2] = Variant();

					VisualServer::get_singleton()->mesh_add_surface_from_arrays(chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRARIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 1), VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

					if (chunk->get_library()->material_get(1).is_valid())
						VisualServer::get_singleton()->mesh_surface_set_material(chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRARIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 1), 0, chunk->get_library()->material_get(1)->get_rid());
				}
				if (should_return()) {
					return;
				}
			}

			if (should_do()) {
				if (chunk->get_lod_num() >= 2) {
					Array temp_mesh_arr2 = merge_mesh_array(temp_mesh_arr);
					temp_mesh_arr = temp_mesh_arr2;

					VisualServer::get_singleton()->mesh_add_surface_from_arrays(chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRARIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 2), VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr2);

					if (chunk->get_library()->material_get(2).is_valid())
						VisualServer::get_singleton()->mesh_surface_set_material(chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRARIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 2), 0, chunk->get_library()->material_get(2)->get_rid());
				}

				if (should_return()) {
					return;
				}
			}

			if (should_do()) {
				if (chunk->get_lod_num() >= 3) {
					Ref<ShaderMaterial> mat = chunk->get_library()->material_get(0);
					Ref<SpatialMaterial> spmat = chunk->get_library()->material_get(0);
					Ref<Texture> tex;

					if (mat.is_valid()) {
						tex = mat->get_shader_param("texture_albedo");
					} else if (spmat.is_valid()) {
						tex = spmat->get_texture(SpatialMaterial::TEXTURE_ALBEDO);
					}

					if (tex.is_valid()) {
						temp_mesh_arr = bake_mesh_array_uv(temp_mesh_arr, tex);
						temp_mesh_arr[VisualServer::ARRAY_TEX_UV] = Variant();

						VisualServer::get_singleton()->mesh_add_surface_from_arrays(
								chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRARIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 3),
								VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

						if (chunk->get_library()->material_get(3).is_valid())
							VisualServer::get_singleton()->mesh_surface_set_material(
									chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRARIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 3), 0,
									chunk->get_library()->material_get(3)->get_rid());
					}
				}

				if (should_return()) {
					return;
				}
			}

#ifdef MESH_UTILS_PRESENT
			if (should_do()) {
				if (chunk->get_lod_num() > 4) {
					Ref<FastQuadraticMeshSimplifier> fqms;
					fqms.instance();
					fqms->set_preserve_border_edges(true);
					fqms->initialize(temp_mesh_arr);

					for (int i = 4; i < chunk->get_lod_num(); ++i) {
						fqms->simplify_mesh(temp_mesh_arr.size() * 0.8, 7);
						temp_mesh_arr = fqms->get_arrays();

						VisualServer::get_singleton()->mesh_add_surface_from_arrays(
								chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRARIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, i),
								VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

						if (chunk->get_library()->material_get(i).is_valid())
							VisualServer::get_singleton()->mesh_surface_set_material(
									chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRARIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, i), 0,
									chunk->get_library()->material_get(i)->get_rid());
					}
				}

				if (should_return()) {
					return;
				}
			}
#endif
		}
	}

	if (liquid_mesher.is_valid() && liquid_mesher->get_vertex_count() != 0) {
		if (should_do()) {
			temp_mesh_arr = liquid_mesher->build_mesh();

			if (should_return()) {
				return;
			}
		}

		RID mesh_rid = chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_LIQUID, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 0);

		if (should_do()) {
			if (mesh_rid == RID()) {
				chunk->meshes_create(VoxelChunkDefault::MESH_INDEX_LIQUID, 1);

				mesh_rid = chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_LIQUID, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 0);
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

		//	if (should_do()) {
		VS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

		if (chunk->get_library()->liquid_material_get(0).is_valid())
			VS::get_singleton()->mesh_surface_set_material(mesh_rid, 0, chunk->get_library()->liquid_material_get(0)->get_rid());

		//	if (should_return()) {
		//		return;
		//	}
		//}
	}

	if (has_meta("bptm_ulm")) {
		remove_meta("bptm_ulm");
	}

	if (has_meta("bptm_ullm")) {
		remove_meta("bptm_ullm");
	}

	if (has_meta("bptm_mm")) {
		remove_meta("bptm_mm");
	}

	if (has_meta("bptm_lmm")) {
		remove_meta("bptm_lmm");
	}

	reset_stages();
	next_phase();
}

void VoxelTerrarinJob::phase_finalize() {
	set_complete(true); //So threadpool knows it's done

	next_job();
}

void VoxelTerrarinJob::_execute_phase() {
	ERR_FAIL_COND(!_chunk.is_valid());

	Ref<VoxelmanLibrary> library = _chunk->get_library();

	ERR_FAIL_COND(!library.is_valid());

	if (_phase == 0) {
		phase_setup();
	} else if (_phase == 1) {
		phase_terrarin_mesh_setup();
	} else if (_phase == 2) {
		phase_collider();
	} else if (_phase == 4) {
		phase_terrarin_mesh();
	} else if (_phase == 5) {
		phase_finalize();
	} else if (_phase > 5) {
		set_complete(true); //So threadpool knows it's done
		next_job();
		ERR_FAIL_MSG("VoxelTerrarinJob: _phase is too high!");
	}
}

void VoxelTerrarinJob::_reset() {
	VoxelJob::_reset();

	_build_done = false;
	_phase = 0;

	for (int i = 0; i < _meshers.size(); ++i) {
		Ref<VoxelMesher> mesher = _meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->set_voxel_scale(_chunk->get_voxel_scale());

		Ref<VoxelChunkDefault> chunk = _chunk;
		Ref<VoxelMesherDefault> md = mesher;

		if (chunk.is_valid() && md.is_valid()) {
			md->set_build_flags(chunk->get_build_flags());
		}
	}

	for (int i = 0; i < _liquid_meshers.size(); ++i) {
		Ref<VoxelMesher> mesher = _liquid_meshers.get(i);

		ERR_CONTINUE(!mesher.is_valid());

		mesher->set_voxel_scale(_chunk->get_voxel_scale());

		Ref<VoxelChunkDefault> chunk = _chunk;
		Ref<VoxelMesherDefault> md = mesher;

		if (chunk.is_valid() && md.is_valid()) {
			md->set_build_flags(chunk->get_build_flags());
		}
	}
}

void VoxelTerrarinJob::_physics_process(float delta) {
	if (_phase == 3)
		phase_physics_process();
}

VoxelTerrarinJob::VoxelTerrarinJob() {
}

VoxelTerrarinJob::~VoxelTerrarinJob() {
	_meshers.clear();
	_liquid_meshers.clear();
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

	ClassDB::bind_method(D_METHOD("_physics_process", "delta"), &VoxelTerrarinJob::_physics_process);
}
