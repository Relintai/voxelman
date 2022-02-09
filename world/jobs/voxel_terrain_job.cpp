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

#include "voxel_terrain_job.h"

#include "../../library/voxel_library.h"
#include "../../library/voxel_surface.h"

#include "../../meshers/default/voxel_mesher_default.h"
#include "../../meshers/voxel_mesher.h"

#include "../../library/voxel_material_cache.h"
#include "../default/voxel_chunk_default.h"

#ifdef MESH_UTILS_PRESENT
#include "../../../mesh_utils/fast_quadratic_mesh_simplifier.h"
#endif

Ref<VoxelMesher> VoxelTerrainJob::get_mesher(int index) const {
	ERR_FAIL_INDEX_V(index, _meshers.size(), Ref<VoxelMesher>());

	return _meshers.get(index);
}
void VoxelTerrainJob::set_mesher(int index, const Ref<VoxelMesher> &mesher) {
	ERR_FAIL_INDEX(index, _meshers.size());

	_meshers.set(index, mesher);
}
void VoxelTerrainJob::remove_mesher(const int index) {
	ERR_FAIL_INDEX(index, _meshers.size());

	_meshers.VREMOVE(index);
}
void VoxelTerrainJob::add_mesher(const Ref<VoxelMesher> &mesher) {
	_meshers.push_back(mesher);
}
int VoxelTerrainJob::get_mesher_count() const {
	return _meshers.size();
}

Ref<VoxelMesher> VoxelTerrainJob::get_liquid_mesher(int index) const {
	ERR_FAIL_INDEX_V(index, _liquid_meshers.size(), Ref<VoxelMesher>());

	return _liquid_meshers.get(index);
}
void VoxelTerrainJob::set_liquid_mesher(int index, const Ref<VoxelMesher> &mesher) {
	ERR_FAIL_INDEX(index, _liquid_meshers.size());

	_liquid_meshers.set(index, mesher);
}
void VoxelTerrainJob::remove_liquid_mesher(const int index) {
	ERR_FAIL_INDEX(index, _liquid_meshers.size());

	_liquid_meshers.VREMOVE(index);
}
void VoxelTerrainJob::add_liquid_mesher(const Ref<VoxelMesher> &mesher) {
	_liquid_meshers.push_back(mesher);
}
int VoxelTerrainJob::get_liquid_mesher_count() const {
	return _liquid_meshers.size();
}

Ref<VoxelMesherJobStep> VoxelTerrainJob::get_jobs_step(int index) const {
	ERR_FAIL_INDEX_V(index, _job_steps.size(), Ref<VoxelMesherJobStep>());

	return _job_steps.get(index);
}
void VoxelTerrainJob::set_jobs_step(int index, const Ref<VoxelMesherJobStep> &step) {
	ERR_FAIL_INDEX(index, _job_steps.size());

	_job_steps.set(index, step);
}
void VoxelTerrainJob::remove_jobs_step(const int index) {
	ERR_FAIL_INDEX(index, _job_steps.size());

	_job_steps.remove(index);
}
void VoxelTerrainJob::add_jobs_step(const Ref<VoxelMesherJobStep> &step) {
	_job_steps.push_back(step);
}
int VoxelTerrainJob::get_jobs_step_count() const {
	return _job_steps.size();
}

void VoxelTerrainJob::phase_setup() {
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

void VoxelTerrainJob::phase_library_setup() {
	if (should_return()) {
		return;
	}

	Ref<VoxelLibrary> lib = _chunk->get_library();

	if (!lib.is_valid()) {
		next_phase();
		return;
	}

	if (lib->supports_caching()) {
		if (!_chunk->material_cache_key_has()) {
			lib->material_cache_get_key(_chunk);
		} else {
			Ref<VoxelMaterialCache> cache = lib->material_cache_get(_chunk->material_cache_key_get());

			if (!cache.is_valid()) {
				next_phase();
				return;
			}

			//Note: without threadpool and threading none of this can happen, as cache will get initialized the first time a thread requests it!
			while (!cache->get_initialized()) {
				//Means it's currently merging the atlases on a different thread.
				//Let's just wait
				OS::get_singleton()->delay_usec(100);
			}
		}
	}

	next_phase();

	if (should_return()) {
		return;
	}
}

void VoxelTerrainJob::phase_terrain_mesh_setup() {
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

void VoxelTerrainJob::phase_collider() {
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
		reset_stages();
		next_phase();
		next_phase();
		return;
	}

	set_build_phase_type(BUILD_PHASE_TYPE_PHYSICS_PROCESS);
	reset_stages();
	next_phase();
}

void VoxelTerrainJob::phase_physics_process() {
	Ref<VoxelChunkDefault> chunk = _chunk;

	if (temp_arr_collider.size() != 0) {
		if (!chunk->meshes_has(VoxelChunkDefault::MESH_INDEX_TERRAIN, VoxelChunkDefault::MESH_TYPE_INDEX_BODY)) {
			chunk->colliders_create(VoxelChunkDefault::MESH_INDEX_TERRAIN);
		}

		PhysicsServer::get_singleton()->shape_set_data(chunk->mesh_rid_get(VoxelChunkDefault::MESH_INDEX_TERRAIN, VoxelChunkDefault::MESH_TYPE_INDEX_SHAPE), temp_arr_collider);

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
	reset_stages();
	next_phase();
}

void VoxelTerrainJob::phase_terrain_mesh() {
	Ref<VoxelChunkDefault> chunk = _chunk;

	ERR_FAIL_COND(_meshers.size() == 0);

	if (should_return()) {
		return;
	}

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

	if (mesher->get_vertex_count() == 0 && (!liquid_mesher.is_valid() || liquid_mesher->get_vertex_count() == 0)) {
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

	//set up the meshes
	if (should_do()) {
		RID mesh_rid = chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRAIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, 0);

		if (mesh_rid == RID()) {
			//need to allocate the meshes

			//first count how many we need
			int count = 0;
			for (int i = 0; i < _job_steps.size(); ++i) {
				Ref<VoxelMesherJobStep> step = _job_steps[i];

				ERR_FAIL_COND(!step.is_valid());

				switch (step->get_job_type()) {
					case VoxelMesherJobStep::TYPE_NORMAL:
						++count;
						break;
					case VoxelMesherJobStep::TYPE_NORMAL_LOD:
						++count;
						break;
					case VoxelMesherJobStep::TYPE_DROP_UV2:
						++count;
						break;
					case VoxelMesherJobStep::TYPE_MERGE_VERTS:
						++count;
						break;
					case VoxelMesherJobStep::TYPE_BAKE_TEXTURE:
						++count;
						break;
					case VoxelMesherJobStep::TYPE_SIMPLIFY_MESH:
#ifdef MESH_UTILS_PRESENT
						count += step->get_simplification_steps();
#endif
						break;
					default:
						break;
				}
			}

			//allocate
			if (count > 0)
				chunk->meshes_create(VoxelChunkDefault::MESH_INDEX_TERRAIN, count);

		} else {
			//we have the meshes, just clear
			int count = chunk->mesh_rid_get_count(VoxelChunkDefault::MESH_INDEX_TERRAIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH);

			for (int i = 0; i < count; ++i) {
				mesh_rid = chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRAIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, i);

				if (VS::get_singleton()->mesh_get_surface_count(mesh_rid) > 0)
#if !GODOT4
					VS::get_singleton()->mesh_remove_surface(mesh_rid, 0);
#else
					VS::get_singleton()->mesh_clear(mesh_rid);
#endif
			}
		}
	}

	for (; _current_job_step < _job_steps.size();) {
		Ref<VoxelMesherJobStep> step = _job_steps[_current_job_step];

		ERR_FAIL_COND(!step.is_valid());

		switch (step->get_job_type()) {
			case VoxelMesherJobStep::TYPE_NORMAL:
				step_type_normal();
				break;
			case VoxelMesherJobStep::TYPE_NORMAL_LOD:
				step_type_normal_lod();
				break;
			case VoxelMesherJobStep::TYPE_DROP_UV2:
				step_type_drop_uv2();
				break;
			case VoxelMesherJobStep::TYPE_MERGE_VERTS:
				step_type_merge_verts();
				break;
			case VoxelMesherJobStep::TYPE_BAKE_TEXTURE:
				step_type_bake_texture();
				break;
			case VoxelMesherJobStep::TYPE_SIMPLIFY_MESH:
				step_type_simplify_mesh();
				break;
			case VoxelMesherJobStep::TYPE_OTHER:
				//do nothing
				break;
		}

		++_current_job_step;

		if (should_return()) {
			return;
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

		if (chunk->get_library()->liquid_material_lod_get(0).is_valid())
			VS::get_singleton()->mesh_surface_set_material(mesh_rid, 0, chunk->get_library()->liquid_material_lod_get(0)->get_rid());

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

void VoxelTerrainJob::phase_finalize() {
	set_complete(true); //So threadpool knows it's done

	next_job();
}

void VoxelTerrainJob::_execute_phase() {
	ERR_FAIL_COND(!_chunk.is_valid());

	Ref<VoxelLibrary> library = _chunk->get_library();

	ERR_FAIL_COND(!library.is_valid());

	if (_phase == 0) {
		phase_setup();
	} else if (_phase == 1) {
		phase_library_setup();
	} else if (_phase == 2) {
		phase_terrain_mesh_setup();
	} else if (_phase == 3) {
		phase_collider();
	} else if (_phase == 5) {
		phase_terrain_mesh();
	} else if (_phase == 6) {
		phase_finalize();
	} else if (_phase > 6) {
		set_complete(true); //So threadpool knows it's done
		next_job();
		ERR_FAIL_MSG("VoxelTerrainJob: _phase is too high!");
	}
}

void VoxelTerrainJob::_reset() {
	VoxelJob::_reset();

	_build_done = false;
	_phase = 0;

	_current_job_step = 0;
	_current_mesh = 0;

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

void VoxelTerrainJob::_physics_process(float delta) {
	if (_phase == 4)
		phase_physics_process();
}

void VoxelTerrainJob::step_type_normal() {
	Ref<VoxelChunkDefault> chunk = _chunk;

	Ref<VoxelMesher> mesher;
	for (int i = 0; i < _meshers.size(); ++i) {
		mesher = _meshers.get(i);

		if (mesher.is_valid()) {
			break;
		}
	}

	//TODO make this automatic in build_mesh
	if ((chunk->get_build_flags() & VoxelChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
		mesher->bake_colors(_chunk);
	}

	temp_mesh_arr = mesher->build_mesh();

	RID mesh_rid = chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRAIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

	VS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	Ref<Material> lmat;

	if (chunk->material_cache_key_has()) {
		lmat = chunk->get_library()->material_cache_get(_chunk->material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->material_lod_get(_current_mesh);
	}

	if (lmat.is_valid()) {
		VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
	}

	++_current_mesh;
}

void VoxelTerrainJob::step_type_normal_lod() {
	Ref<VoxelMesherJobStep> step = _job_steps[_current_job_step];

	ERR_FAIL_COND(!step.is_valid());

	Ref<VoxelChunkDefault> chunk = _chunk;

	Ref<VoxelMesher> mesher;
	for (int i = 0; i < _meshers.size(); ++i) {
		mesher = _meshers.get(i);

		if (mesher.is_valid()) {
			break;
		}
	}

	mesher->set_lod_index(step->get_lod_index());
	mesher->reset();
	mesher->add_chunk(_chunk);

	//TODO make this automatic in build_mesh
	if ((chunk->get_build_flags() & VoxelChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
		mesher->bake_colors(_chunk);
	}

	temp_mesh_arr = mesher->build_mesh();

	RID mesh_rid = chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRAIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

	VS::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	Ref<Material> lmat;

	if (chunk->material_cache_key_has()) {
		lmat = chunk->get_library()->material_cache_get(_chunk->material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->material_lod_get(_current_mesh);
	}

	if (lmat.is_valid()) {
		VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
	}

	++_current_mesh;
}

void VoxelTerrainJob::step_type_drop_uv2() {
	Ref<VoxelChunkDefault> chunk = _chunk;

	RID mesh_rid = chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRAIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

	temp_mesh_arr[VisualServer::ARRAY_TEX_UV2] = Variant();

	VisualServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	Ref<Material> lmat;

	if (chunk->material_cache_key_has()) {
		lmat = chunk->get_library()->material_cache_get(_chunk->material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->material_lod_get(_current_mesh);
	}

	if (lmat.is_valid()) {
		VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
	}

	++_current_mesh;
}

void VoxelTerrainJob::step_type_merge_verts() {
	Array temp_mesh_arr2 = merge_mesh_array(temp_mesh_arr);
	temp_mesh_arr = temp_mesh_arr2;

	Ref<VoxelChunkDefault> chunk = _chunk;
	RID mesh_rid = chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRAIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

	VisualServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

	Ref<Material> lmat;

	if (chunk->material_cache_key_has()) {
		lmat = chunk->get_library()->material_cache_get(_chunk->material_cache_key_get())->material_lod_get(_current_mesh);
	} else {
		lmat = chunk->get_library()->material_lod_get(_current_mesh);
	}

	if (lmat.is_valid()) {
		VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
	}

	++_current_mesh;
}

void VoxelTerrainJob::step_type_bake_texture() {
	Ref<VoxelChunkDefault> chunk = _chunk;

	Ref<ShaderMaterial> mat = chunk->get_library()->material_lod_get(0);
	Ref<SpatialMaterial> spmat = chunk->get_library()->material_lod_get(0);
	Ref<Texture> tex;

	if (mat.is_valid()) {
		tex = mat->get_shader_param("texture_albedo");
	} else if (spmat.is_valid()) {
		tex = spmat->get_texture(SpatialMaterial::TEXTURE_ALBEDO);
	}

	if (tex.is_valid()) {
		temp_mesh_arr = bake_mesh_array_uv(temp_mesh_arr, tex);
		temp_mesh_arr[VisualServer::ARRAY_TEX_UV] = Variant();

		RID mesh_rid = chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRAIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

		VisualServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

		Ref<Material> lmat;

		if (chunk->material_cache_key_has()) {
			lmat = chunk->get_library()->material_cache_get(_chunk->material_cache_key_get())->material_lod_get(_current_mesh);
		} else {
			lmat = chunk->get_library()->material_lod_get(_current_mesh);
		}

		if (lmat.is_valid()) {
			VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
		}
	}

	++_current_mesh;
}

void VoxelTerrainJob::step_type_simplify_mesh() {
#ifdef MESH_UTILS_PRESENT

	Ref<VoxelChunkDefault> chunk = _chunk;
	Ref<VoxelMesherJobStep> step = _job_steps[_current_job_step];
	ERR_FAIL_COND(!step.is_valid());
	Ref<FastQuadraticMeshSimplifier> fqms = step->get_fqms();
	ERR_FAIL_COND(!fqms.is_valid());

	fqms->initialize(temp_mesh_arr);

	for (int i = 0; i < step->get_simplification_steps(); ++i) {
		fqms->simplify_mesh(temp_mesh_arr.size() * step->get_simplification_step_ratio(), step->get_simplification_agressiveness());
		temp_mesh_arr = fqms->get_arrays();

		RID mesh_rid = chunk->mesh_rid_get_index(VoxelChunkDefault::MESH_INDEX_TERRAIN, VoxelChunkDefault::MESH_TYPE_INDEX_MESH, _current_mesh);

		VisualServer::get_singleton()->mesh_add_surface_from_arrays(mesh_rid, VisualServer::PRIMITIVE_TRIANGLES, temp_mesh_arr);

		Ref<Material> lmat;

		if (chunk->material_cache_key_has()) {
			lmat = chunk->get_library()->material_cache_get(_chunk->material_cache_key_get())->material_lod_get(_current_mesh);
		} else {
			lmat = chunk->get_library()->material_lod_get(_current_mesh);
		}

		if (lmat.is_valid()) {
			VisualServer::get_singleton()->mesh_surface_set_material(mesh_rid, 0, lmat->get_rid());
		}

		++_current_mesh;
	}
#endif
}

VoxelTerrainJob::VoxelTerrainJob() {
	_current_job_step = 0;
	_current_mesh = 0;
}

VoxelTerrainJob::~VoxelTerrainJob() {
	_meshers.clear();
	_liquid_meshers.clear();
}

void VoxelTerrainJob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesher", "index"), &VoxelTerrainJob::get_mesher);
	ClassDB::bind_method(D_METHOD("set_mesher", "index", "mesher"), &VoxelTerrainJob::set_mesher);
	ClassDB::bind_method(D_METHOD("remove_mesher", "index"), &VoxelTerrainJob::remove_mesher);
	ClassDB::bind_method(D_METHOD("add_mesher", "mesher"), &VoxelTerrainJob::add_mesher);
	ClassDB::bind_method(D_METHOD("get_mesher_count"), &VoxelTerrainJob::get_mesher_count);

	ClassDB::bind_method(D_METHOD("get_liquid_mesher", "index"), &VoxelTerrainJob::get_liquid_mesher);
	ClassDB::bind_method(D_METHOD("set_liquid_mesher", "index", "mesher"), &VoxelTerrainJob::set_liquid_mesher);
	ClassDB::bind_method(D_METHOD("remove_liquid_mesher", "index"), &VoxelTerrainJob::remove_liquid_mesher);
	ClassDB::bind_method(D_METHOD("add_liquid_mesher", "mesher"), &VoxelTerrainJob::add_liquid_mesher);
	ClassDB::bind_method(D_METHOD("get_liquid_mesher_count"), &VoxelTerrainJob::get_liquid_mesher_count);

	ClassDB::bind_method(D_METHOD("get_jobs_step", "index"), &VoxelTerrainJob::get_jobs_step);
	ClassDB::bind_method(D_METHOD("set_jobs_step", "index", "mesher"), &VoxelTerrainJob::set_jobs_step);
	ClassDB::bind_method(D_METHOD("remove_jobs_step", "index"), &VoxelTerrainJob::remove_jobs_step);
	ClassDB::bind_method(D_METHOD("add_jobs_step", "mesher"), &VoxelTerrainJob::add_jobs_step);
	ClassDB::bind_method(D_METHOD("get_jobs_step_count"), &VoxelTerrainJob::get_jobs_step_count);

	ClassDB::bind_method(D_METHOD("_physics_process", "delta"), &VoxelTerrainJob::_physics_process);
}
