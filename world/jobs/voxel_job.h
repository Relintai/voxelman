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

#ifndef VOXEL_JOB_H
#define VOXEL_JOB_H

#include "scene/resources/texture.h"

#if THREAD_POOL_PRESENT
#include "../../../thread_pool/thread_pool_job.h"
#else

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/object/ref_counted.h"
#ifndef Reference
#define Reference RefCounted
#endif
#else
#include "core/reference.h"
#endif

#endif

#include "../../defines.h"

#if GODOT4
#define Texture Texture2D
#endif

class VoxelChunk;

#if THREAD_POOL_PRESENT
class VoxelJob : public ThreadPoolJob {
	GDCLASS(VoxelJob, ThreadPoolJob);
#else
class VoxelJob : public Reference {
	GDCLASS(VoxelJob, Reference);
#endif

public:
	static const String BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE;

	enum ActiveBuildPhaseType {
		BUILD_PHASE_TYPE_NORMAL = 0,
		BUILD_PHASE_TYPE_PROCESS,
		BUILD_PHASE_TYPE_PHYSICS_PROCESS,
	};

public:
	ActiveBuildPhaseType get_build_phase_type();
	void set_build_phase_type(VoxelJob::ActiveBuildPhaseType build_phase_type);

	void set_chunk(const Ref<VoxelChunk> &chunk);

	int get_phase();
	void set_phase(const int phase);
	void next_phase();

	bool get_build_done();
	void set_build_done(const bool val);

	void next_job();

	void reset();
	virtual void _reset();

	void _execute();

	void execute_phase();
	virtual void _execute_phase();

	void process(const float delta);
	void physics_process(const float delta);

	void generate_ao();
	void generate_random_ao(int seed, int octaves = 4, int period = 30, float persistence = 0.3, float scale_factor = 0.6);
	Array merge_mesh_array(Array arr) const;
	Array bake_mesh_array_uv(Array arr, Ref<Texture> tex, float mul_color = 0.7) const;

	void chunk_exit_tree();

#if VERSION_MAJOR >= 4
	GDVIRTUAL1(_process, float);
	GDVIRTUAL1(_physics_process, float);

	GDVIRTUAL0(_reset);
	GDVIRTUAL0(_execute_phase);
#endif

	VoxelJob();
	~VoxelJob();

protected:
	static void _bind_methods();

	ActiveBuildPhaseType _build_phase_type;
	bool _build_done;
	int _phase;
	bool _in_tree;
	Ref<VoxelChunk> _chunk;

public:
#if !THREAD_POOL_PRESENT
	bool get_complete() const;
	void set_complete(const bool value);

	bool get_cancelled() const;
	void set_cancelled(const bool value);

	float get_max_allocated_time() const;
	void set_max_allocated_time(const float value);

	int get_start_time() const;
	void set_start_time(const int value);

	int get_current_run_stage() const;
	void set_current_run_stage(const int value);

	int get_stage() const;
	void set_stage(const int value);

	void reset_stages();

	float get_current_execution_time();

	bool should_do(const bool just_check = false);
	bool should_return();

	void execute();

#if VERSION_MAJOR >= 4
	GDVIRTUAL0(_execute);
#endif

private:
	bool _complete;
	bool _cancelled;

	float _max_allocated_time;
	uint64_t _start_time;

	int _current_run_stage;
	int _stage;
#endif
};

VARIANT_ENUM_CAST(VoxelJob::ActiveBuildPhaseType);

#endif
