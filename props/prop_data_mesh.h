#ifndef PROP_DATA_MESH_H
#define PROP_DATA_MESH_H

#include "prop_data_entry.h"
#include "core/math/vector3.h"

#include "scene/resources/texture.h"

#include "../../mesh_data_resource/mesh_data_resource.h"

class PropDataMesh : public PropDataEntry {
	GDCLASS(PropDataMesh, PropDataEntry);
	
public:
	Ref<MeshDataResource> get_mesh() const;
	void set_mesh(const Ref<MeshDataResource> mesh);

	Ref<Texture> get_texture() const;
	void set_texture(const Ref<Texture> texture);

	bool get_snap_to_mesh();
	void set_snap_to_mesh(bool value);

	Vector3 get_snap_axis();
	void set_snap_axis(Vector3 value);

	PropDataMesh();
	~PropDataMesh();

protected:
	static void _bind_methods();

private:
	bool _snap_to_mesh;
	Vector3 _snap_axis;
	Ref<MeshDataResource> _mesh;
	Ref<Texture> _texture;
};

#endif
