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

#if THREAD_POOL_PRESENT
#include "../../../thread_pool/thread_pool_job.h"
#else
#include "core/resource.h"
#endif

class VoxelChunkDefault;

#if THREAD_POOL_PRESENT
class VoxelJob : public ThreadPoolJob {
	GDCLASS(VoxelJob, ThreadPoolJob);
#else
class VoxelJob : public Resource {
	GDCLASS(VoxelJob, Resource);
#endif

public:
	enum {
		RESULT_TYPE_FLAG_MESH = 1 << 0,
		RESULT_TYPE_FLAG_COLLIDER = 1 << 1,
	};

public:
	void set_chunk(const Ref<VoxelChunkDefault> &chunk);
	void chunk_exit_tree();

	void finalize_build();
	void _execute();

	void generate_ao();
	void generate_random_ao(int seed, int octaves, int period, float persistence, float scale_factor);

	VoxelJob();
	~VoxelJob();

protected:
	static void _bind_methods();

private:
	bool _in_tree;

	Ref<VoxelChunkDefault> _chunk;

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

	float get_current_execution_time();

	bool should_do(const bool just_check = false);
	bool should_return();

	void execute();

private:
	bool _complete;
	bool _cancelled;

	float _max_allocated_time;
	uint64_t _start_time;

	int _current_run_stage;
	int _stage;
#endif
};

#endif
