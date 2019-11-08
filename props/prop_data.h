#ifndef PROP_DATA_H
#define PROP_DATA_H

#include "core/reference.h"
#include "core/vector.h"

#include "prop_data_entry.h"
#include "prop_data_mesh.h"

#include "../../texture_packer/texture_packer.h"

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
