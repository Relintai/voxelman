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

#ifndef VOXEL_TERRARIN_JOB_H
#define VOXEL_TERRARIN_JOB_H

#include "voxel_job.h"

#include "../../defines.h"

#include pool_vector_h

include_pool_vector

		class VoxelMesher;

class VoxelTerrarinJob : public VoxelJob {
	GDCLASS(VoxelTerrarinJob, VoxelJob);

public:
	//Meshers
	Ref<VoxelMesher> get_mesher(const int index) const;
	void set_mesher(const int index, const Ref<VoxelMesher> &mesher);
	void remove_mesher(const int index);
	void add_mesher(const Ref<VoxelMesher> &mesher);
	int get_mesher_count() const;

	//Liquid Meshers
	Ref<VoxelMesher> get_liquid_mesher(const int index) const;
	void set_liquid_mesher(const int index, const Ref<VoxelMesher> &mesher);
	void remove_liquid_mesher(const int index);
	void add_liquid_mesher(const Ref<VoxelMesher> &mesher);
	int get_liquid_mesher_count() const;

	void phase_setup();
	void phase_terrarin_mesh_setup();
	void phase_collider();
	void phase_physics_proces();
	void phase_terrarin_mesh();
	void phase_finalize();
	void phase_physics_process();

	void _execute_phase();
	void _reset();
	void _physics_process(float delta);

	VoxelTerrarinJob();
	~VoxelTerrarinJob();

protected:
	static void _bind_methods();

	Vector<Ref<VoxelMesher>> _meshers;
	Vector<Ref<VoxelMesher>> _liquid_meshers;

	PoolVector<Vector3> temp_arr_collider;
	PoolVector<Vector3> temp_arr_collider_liquid;
	Array temp_mesh_arr;
};

#endif
