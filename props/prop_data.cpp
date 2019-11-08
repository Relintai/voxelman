#include "prop_data.h"

#include "prop_data_prop.h"

bool PropData::get_snap_to_mesh() {
	return _snap_to_mesh;
}
void PropData::set_snap_to_mesh(bool value) {
	_snap_to_mesh = value;
}

Vector3 PropData::get_snap_axis() {
	return _snap_axis;
}
void PropData::set_snap_axis(Vector3 value) {
	_snap_axis = value;
}

Ref<PropDataEntry> PropData::get_prop(const int index) const {
	ERR_FAIL_INDEX_V(index, _props.size(), Ref<PropDataEntry>());

	return _props.get(index);
}
void PropData::set_prop(const int index, const Ref<PropDataEntry> prop) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.set(index, prop);
}
void PropData::add_prop(const Ref<PropDataEntry> prop) {
	_props.push_back(prop);
}
void PropData::remove_prop(const int index) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.remove(index);
}

int PropData::get_prop_count() const {
	return _props.size();
}

Vector<Variant> PropData::get_props() {
	Vector<Variant> r;
	for (int i = 0; i < _props.size(); i++) {
		r.push_back(_props[i].get_ref_ptr());
	}
	return r;
}
void PropData::set_props(const Vector<Variant> &props) {
	_props.clear();
	for (int i = 0; i < props.size(); i++) {
		Ref<PropDataEntry> prop = Ref<PropDataEntry>(props[i]);

		_props.push_back(prop);
	}
}

void PropData::add_textures_into(Ref<TexturePacker> texture_packer) {
    ERR_FAIL_COND(!texture_packer.is_valid());
    
    for (int i = 0; i < _props.size(); ++i) {
        Ref<PropDataEntry> entry = _props.get(i);
        
        Ref<PropDataMesh> pmesh = entry;
        
        if (pmesh.is_valid() && pmesh->get_texture().is_valid()) {
            texture_packer->add_texture(pmesh->get_texture());
        }
        
        Ref<PropDataProp> pdataprop = entry;
        
        if (pdataprop.is_valid() && pdataprop->get_prop().is_valid()) {
            pdataprop->get_prop()->add_textures_into(texture_packer);
        }
    }
}

PropData::PropData() {
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, -1, 0);
}
PropData::~PropData() {
	_props.clear();
}

void PropData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &PropData::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &PropData::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &PropData::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &PropData::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");

	ClassDB::bind_method(D_METHOD("get_prop", "index"), &PropData::get_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "index", "spell"), &PropData::set_prop);
	ClassDB::bind_method(D_METHOD("add_prop", "prop"), &PropData::add_prop);
	ClassDB::bind_method(D_METHOD("remove_prop", "index"), &PropData::remove_prop);

	ClassDB::bind_method(D_METHOD("get_prop_count"), &PropData::get_prop_count);

	ClassDB::bind_method(D_METHOD("get_props"), &PropData::get_props);
	ClassDB::bind_method(D_METHOD("set_props", "props"), &PropData::set_props);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "props", PROPERTY_HINT_NONE, "17/17:PropDataEntry", PROPERTY_USAGE_DEFAULT, "PropDataEntry"), "set_props", "get_props");
    
    ClassDB::bind_method(D_METHOD("add_textures_into", "texture_packer"), &PropData::add_textures_into);
}
