#ifndef VOXELMAN_PROP_MESH_H
#define VOXELMAN_PROP_MESH_H

#include "voxelman_prop_entry.h"

#include "../../entity_spell_system/meshes/mesh_data_resource.h"

class VoxelmanPropMesh : public VoxelmanPropEntry {
	GDCLASS(VoxelmanPropMesh, VoxelmanPropEntry);
	
public:
	Ref<MeshDataResource> get_mesh() const;
	void set_mesh(const Ref<MeshDataResource> mesh);

	VoxelmanPropMesh();
	~VoxelmanPropMesh();

protected:
	static void _bind_methods();

private:
	Ref<MeshDataResource> _mesh;
};

#endif
