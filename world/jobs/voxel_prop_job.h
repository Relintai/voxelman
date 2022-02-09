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

#ifndef VOXEL_PROP_JOB_H
#define VOXEL_PROP_JOB_H

#include "voxel_job.h"

#include "voxel_mesher_job_step.h"

class VoxelMesher;

class VoxelPropJob : public VoxelJob {
	GDCLASS(VoxelPropJob, VoxelJob);

public:
	Ref<VoxelMesher> get_prop_mesher() const;
	void set_prop_mesher(const Ref<VoxelMesher> &mesher);

	Ref<VoxelMesherJobStep> get_jobs_step(const int index) const;
	void set_jobs_step(const int index, const Ref<VoxelMesherJobStep> &step);
	void remove_jobs_step(const int index);
	void add_jobs_step(const Ref<VoxelMesherJobStep> &step);
	int get_jobs_step_count() const;

	void phase_physics_process();
	void phase_prop();

	void _physics_process(float delta);
	void _execute_phase();
	void _reset();

	void phase_setup();

	void phase_steps();

	void step_type_normal();
	void step_type_normal_lod();
	void step_type_drop_uv2();
	void step_type_merge_verts();
	void step_type_bake_texture();
	void step_type_simplify_mesh();

	VoxelPropJob();
	~VoxelPropJob();

protected:
	static void _bind_methods();

	Ref<VoxelMesher> _prop_mesher;

	Vector<Ref<VoxelMesherJobStep>> _job_steps;
	int _current_job_step;
	int _current_mesh;

	Array temp_mesh_arr;
};

#endif
