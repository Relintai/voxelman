#ifndef VOXELMAN_PROP_MESH_H
#define VOXELMAN_PROP_MESH_H

#include "voxelman_prop_entry.h"
#include "core/math/vector3.h"

#include "../../entity_spell_system/meshes/mesh_data_resource.h"

class VoxelmanPropMesh : public VoxelmanPropEntry {
	GDCLASS(VoxelmanPropMesh, VoxelmanPropEntry);
	
public:
	Ref<MeshDataResource> get_mesh() const;
	void set_mesh(const Ref<MeshDataResource> mesh);

	bool get_snap_to_mesh();
	void set_snap_to_mesh(bool value);

	Vector3 get_snap_axis();
	void set_snap_axis(Vector3 value);

	VoxelmanPropMesh();
	~VoxelmanPropMesh();

protected:
	static void _bind_methods();

private:
	bool _snap_to_mesh;
	Vector3 _snap_axis;
	Ref<MeshDataResource> _mesh;
};

#endif
