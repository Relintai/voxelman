#ifndef PROP_DATA_H
#define PROP_DATA_H

#include "core/math/rect2.h"
#include "core/math/transform.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/reference.h"
#include "core/vector.h"

#include "servers/physics_server.h"

#include "prop_data_entry.h"
#include "prop_data_mesh.h"

#include "../meshers/voxel_mesher.h"

#include "../../texture_packer/texture_packer.h"

class Spatial;
class VoxelChunk;

class PropData : public Resource {
	GDCLASS(PropData, Resource);

public:
	bool get_snap_to_mesh();
	void set_snap_to_mesh(bool value);

	Vector3 get_snap_axis();
	void set_snap_axis(Vector3 value);

	Ref<PropDataEntry> get_prop(const int index) const;
	void set_prop(const int index, const Ref<PropDataEntry> prop);
	void add_prop(const Ref<PropDataEntry> prop);
	void remove_prop(const int index);

	int get_prop_count() const;

	Vector<Variant> get_props();
	void set_props(const Vector<Variant> &props);

	void add_textures_into(Ref<TexturePacker> texture_packer);
	void add_prop_lights_into(VoxelChunk *chunk, Transform parent_transform, bool allow_snap);
	void add_prop_lights_into_bind(Node *chunk, Transform parent_transform, bool allow_snap);
	void add_meshes_into(Ref<VoxelMesher> mesher, Ref<TexturePacker> texture_packer, Transform parent_transform, Spatial *snap_spatial = NULL);
	void add_meshes_into_bind(Ref<VoxelMesher> mesher, Ref<TexturePacker> texture_packer, Transform parent_transform, Node *snap_spatial = NULL);

	Transform get_next_snapped_prop_transform(Spatial *s, Transform parent_transform, bool snap_to_mesh, Vector3 snap_axis);
	Transform get_next_snapped_prop_transform_bind(Node *spatial, Transform parent_transform, bool snap_to_mesh, Vector3 snap_axis);

	PropData();
	~PropData();

protected:
	static void _bind_methods();

private:
	bool _snap_to_mesh;
	Vector3 _snap_axis;

	Vector<Ref<PropDataEntry> > _props;
};

#endif
