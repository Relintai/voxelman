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

#include "voxel_world_marching_cubes.h"

#include "voxel_chunk_marching_cubes.h"

#include "../../meshers/marching_cubes/voxel_mesher_marching_cubes.h"
#include "../jobs/voxel_light_job.h"
#include "../jobs/voxel_prop_job.h"
#include "../jobs/voxel_terrain_job.h"

#ifdef MESH_UTILS_PRESENT
#include "../../../mesh_utils/fast_quadratic_mesh_simplifier.h"
#endif

Ref<VoxelChunk> VoxelWorldMarchingCubes::_create_chunk(int x, int y, int z, Ref<VoxelChunk> chunk) {
	if (!chunk.is_valid()) {
		chunk = Ref<VoxelChunk>(memnew(VoxelChunkMarchingCubes));
	}

	if (chunk->job_get_count() == 0) {
		Ref<VoxelLightJob> lj;
		lj.INSTANCE();

		Ref<VoxelTerrainJob> tj;
		tj.INSTANCE();

		Ref<VoxelMesherJobStep> s;
		s.instance();
		s->set_job_type(VoxelMesherJobStep::TYPE_NORMAL);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(VoxelMesherJobStep::TYPE_DROP_UV2);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(VoxelMesherJobStep::TYPE_MERGE_VERTS);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(VoxelMesherJobStep::TYPE_BAKE_TEXTURE);
		tj->add_jobs_step(s);

#ifdef MESH_UTILS_PRESENT
		s.instance();
		Ref<FastQuadraticMeshSimplifier> fqms;
		fqms.instance();
		s->set_fqms(fqms);
		s->set_job_type(VoxelMesherJobStep::TYPE_SIMPLIFY_MESH);
		s->set_simplification_step_ratio(0.8);
		s->set_simplification_agressiveness(7);
		s->set_simplification_steps(1);
		tj->add_jobs_step(s);
#endif

		Ref<VoxelMesher> m = Ref<VoxelMesher>(memnew(VoxelMesherMarchingCubes()));
		m->set_channel_index_type(VoxelChunkDefault::DEFAULT_CHANNEL_TYPE);
		m->set_channel_index_isolevel(VoxelChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);

		tj->add_mesher(m);
		//add_liquid_mesher(Ref<VoxelMesher>(memnew(VoxelMesherLiquidMarchingCubes())));

		Ref<VoxelPropJob> pj;
		pj.INSTANCE();
		pj->set_prop_mesher(Ref<VoxelMesher>(memnew(VoxelMesherMarchingCubes)));

		s.instance();
		s->set_job_type(VoxelMesherJobStep::TYPE_NORMAL);
		pj->add_jobs_step(s);

		s.instance();
		s->set_job_type(VoxelMesherJobStep::TYPE_MERGE_VERTS);
		pj->add_jobs_step(s);

		s.instance();
		s->set_job_type(VoxelMesherJobStep::TYPE_BAKE_TEXTURE);
		pj->add_jobs_step(s);

		s.instance();
		s->set_job_type(VoxelMesherJobStep::TYPE_SIMPLIFY_MESH);
#ifdef MESH_UTILS_PRESENT
		fqms.instance();
		s->set_fqms(fqms);
		s->set_simplification_steps(2);
#endif
		pj->add_jobs_step(s);

		chunk->job_add(lj);
		chunk->job_add(tj);
		chunk->job_add(pj);

		// TODO this should be removed
		set_num_lods(5);
	}

	return VoxelWorld::_create_chunk(x, y, z, chunk);
}

void VoxelWorldMarchingCubes::_set_voxel_with_tool(const bool mode_add, const Vector3 hit_position, const Vector3 hit_normal, const int selected_voxel, const int isolevel) {
	Vector3 pos;

	Vector3 hp = hit_position;
	if (hp.x >= 0)
		hp.x = (hp.x) - 0.5 * get_voxel_scale();
	//else
	//	hp.x = (hp.x) + 0.5 * get_voxel_scale();

	if (hp.x >= 0)
		hp.y = (hp.y) - 0.5 * get_voxel_scale();
	//else
	//	hp.y = (hp.y) + 0.5 * get_voxel_scale();

	if (hp.x >= 0)
		hp.z = (hp.z) - 0.5 * get_voxel_scale();
	//else
	//	hp.z = (hp.z) + 0.5 * get_voxel_scale();

	hp.x = static_cast<int>(hp.x);
	hp.y = static_cast<int>(hp.y);
	hp.z = static_cast<int>(hp.z);

	if (mode_add) {
		pos = (hit_position + (Vector3(0.8, 0.8, 0.8) * hit_normal * get_voxel_scale()));
	} else {
		pos = (hit_position + (Vector3(0.8, 0.8, 0.8) * -hit_normal * get_voxel_scale()));
	}

	int channel_type = get_channel_index_info(VoxelWorld::CHANNEL_TYPE_INFO_TYPE);
	int channel_isolevel = get_channel_index_info(VoxelWorld::CHANNEL_TYPE_INFO_ISOLEVEL);

	if (channel_isolevel == -1) {
		set_voxel_at_world_position(pos, selected_voxel, channel_type);
	} else {
		set_voxel_at_world_position(pos, selected_voxel, channel_type, false);
		set_voxel_at_world_position(pos, isolevel, channel_isolevel);
	}
}

VoxelWorldMarchingCubes::VoxelWorldMarchingCubes() {
	set_data_margin_start(1);
	set_data_margin_end(2);
}

VoxelWorldMarchingCubes ::~VoxelWorldMarchingCubes() {
}

void VoxelWorldMarchingCubes::_bind_methods() {
}