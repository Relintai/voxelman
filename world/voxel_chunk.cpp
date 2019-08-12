#include "voxel_chunk.h"

#include "voxel_world.h"

int VoxelChunk::get_chunk_position_x() {
	return _chunk_position.x;
}
void VoxelChunk::set_chunk_position_x(int value) {
	_chunk_position.x = value;
}

int VoxelChunk::get_chunk_position_y() {
	return _chunk_position.y;
}
void VoxelChunk::set_chunk_position_y(int value) {
	_chunk_position.y = value;
}

int VoxelChunk::get_chunk_position_z() {
	return _chunk_position.z;
}
void VoxelChunk::set_chunk_position_z(int value) {
	_chunk_position.z = value;
}

Vector3i VoxelChunk::get_chunk_position() const {
	return _chunk_position;
}
void VoxelChunk::set_chunk_position(int x, int y, int z) {
	_chunk_position.x = x;
	_chunk_position.y = y;
	_chunk_position.z = z;
}

int VoxelChunk::get_chunk_size_x() {
	return _chunk_size.x;
}
void VoxelChunk::set_chunk_size_x(int value) {
	_chunk_size.x = value;
}

int VoxelChunk::get_chunk_size_y() {
	return _chunk_size.y;
}
void VoxelChunk::set_chunk_size_y(int value) {
	_chunk_size.y = value;
}

int VoxelChunk::get_chunk_size_z() {
	return _chunk_size.z;
}
void VoxelChunk::set_chunk_size_z(int value) {
	_chunk_size.z = value;
}

Vector3i VoxelChunk::get_chunk_size() const {
	return _chunk_size;
}
void VoxelChunk::set_chunk_size(int x, int y, int z) {
	_chunk_size.x = x;
	_chunk_size.y = y;
	_chunk_size.z = z;
}

Ref<VoxelmanLibrary> VoxelChunk::get_library() {
	return _library;
}
void VoxelChunk::set_library(Ref<VoxelmanLibrary> value) {
	_library = value;
}

int VoxelChunk::get_lod_size() const {
	return _lod_size;
}
void VoxelChunk::set_lod_size(const int lod_size) {
	_lod_size = lod_size;

	if (_mesher.is_valid()) {
		_mesher->set_lod_size(_lod_size);
	}
}

float VoxelChunk::get_voxel_scale() const {
	return _voxel_scale;
}
void VoxelChunk::set_voxel_scale(float value) {
	_voxel_scale = value;

	if (_mesher.is_valid()) {
		_mesher->set_voxel_scale(_voxel_scale);
	}
}

Ref<VoxelMesher> VoxelChunk::get_mesher() const {
	return _mesher;
}
void VoxelChunk::set_mesher(Ref<VoxelMesher> mesher) {
	_mesher = mesher;
}

VoxelWorld *VoxelChunk::get_voxel_world() const {
	return _voxel_world;
}
void VoxelChunk::set_voxel_world(VoxelWorld *world) {
	_voxel_world = world;
}
void VoxelChunk::set_voxel_world_bind(Node *world) {
	if (world == NULL) {
		_voxel_world = NULL;
		return;
	}

	_voxel_world = Object::cast_to<VoxelWorld>(world);
}

bool VoxelChunk::get_create_collider() const {
	return _create_collider;
}
void VoxelChunk::set_create_collider(bool value) {
	_create_collider = value;
}

bool VoxelChunk::get_bake_lights() const {
	return _bake_lights;
}
void VoxelChunk::set_bake_lights(bool value) {
	_bake_lights = value;
}

Ref<VoxelBuffer> VoxelChunk::get_buffer() const {
	return _buffer;
}

void VoxelChunk::create_mesher() {
	call("_create_mesher");

	ERR_FAIL_COND(!_mesher.is_valid());

	_mesher->set_lod_size(get_lod_size());
	_mesher->set_voxel_scale(get_voxel_scale());
}

void VoxelChunk::_create_mesher() {
	_mesher = Ref<VoxelMesher>(memnew(VoxelMesherCubic()));
}

void VoxelChunk::finalize_mesh() {
	_mesher->set_library(_library);

	if (_mesh_rid == RID()) {
		allocate_main_mesh();
	}

	get_mesher()->build_mesh(_mesh_rid);
}

void VoxelChunk::build() {
	ERR_FAIL_COND(!_library.is_valid());

	if (!_mesher.is_valid()) {
		create_mesher();
	}

	_mesher->set_library(_library);

	_mesher->reset();

	if (has_method("_create_mesh")) {
		call("_create_mesh");
	} else {
		_mesher->add_buffer(_buffer);
	}

	finalize_mesh();

	if (get_create_collider()) {
		build_collider();
	}

	_mesher->reset();

	if (_props.size() > 0) {
		process_props();
		build_prop_meshes();

		if (get_create_collider()) {
			build_prop_collider();
		}
	}

	_mesher->reset();
}

void VoxelChunk::clear() {
	_voxel_lights.clear();
}

void VoxelChunk::create_colliders() {
	ERR_FAIL_COND(_voxel_world == NULL);

	_shape_rid = PhysicsServer::get_singleton()->shape_create(PhysicsServer::SHAPE_CONCAVE_POLYGON);
	_body_rid = PhysicsServer::get_singleton()->body_create(PhysicsServer::BODY_MODE_STATIC);

	PhysicsServer::get_singleton()->body_set_collision_layer(_body_rid, 1);
	PhysicsServer::get_singleton()->body_set_collision_mask(_body_rid, 1);

	PhysicsServer::get_singleton()->body_add_shape(_body_rid, _shape_rid);

	PhysicsServer::get_singleton()->body_set_state(_body_rid, PhysicsServer::BODY_STATE_TRANSFORM, Transform(Basis(), Vector3(_chunk_position.x * _chunk_size.x * _voxel_scale, _chunk_position.y * _chunk_size.y * _voxel_scale, _chunk_position.z * _chunk_size.z * _voxel_scale)));
	PhysicsServer::get_singleton()->body_set_space(_body_rid, get_voxel_world()->get_world()->get_space());
}

void VoxelChunk::build_collider() {
	if (_body_rid == RID()) {
		create_colliders();
	}

	_mesher->build_collider(_shape_rid);
}

void VoxelChunk::remove_colliders() {
	if (_body_rid != RID()) {
		PhysicsServer::get_singleton()->free(_body_rid);
		PhysicsServer::get_singleton()->free(_shape_rid);

		_body_rid = RID();
		_shape_rid = RID();
	}
}

void VoxelChunk::add_lights(Array lights) {
	for (int i = 0; i < lights.size(); ++i) {
		Ref<VoxelLight> light = Ref<VoxelLight>(lights.get(i));

		if (light.is_valid()) {
			add_voxel_light(light);
		}
	}
}
void VoxelChunk::add_voxel_light(Ref<VoxelLight> light) {
	_voxel_lights.push_back(light);
}

void VoxelChunk::create_voxel_light(const Color color, const int size, const int x, const int y, const int z) {
	Ref<VoxelLight> light;
	light.instance();

	Vector3i pos = get_chunk_position();
	Vector3i csize = get_chunk_size();

	light->set_world_position(pos.x * csize.x + x, pos.y * csize.y + y, pos.z * csize.z + z);
	light->set_color(color);
	light->set_size(size);

	add_voxel_light(light);
}

void VoxelChunk::remove_voxel_light(Ref<VoxelLight> light) {
	for (int i = 0; i < _voxel_lights.size(); ++i) {
		if (_voxel_lights[i] == light) {
			_voxel_lights.remove(i);
			return;
		}
	}
}
void VoxelChunk::clear_voxel_lights() {
	_voxel_lights.clear();
}

void VoxelChunk::add_lights_into(Array target) {
	for (int i = 0; i < _voxel_lights.size(); ++i) {
		target.append(_voxel_lights[i]);
	}
}

void VoxelChunk::add_unique_lights_into(Array target) {
	for (int i = 0; i < _voxel_lights.size(); ++i) {
        Ref<VoxelLight> l = _voxel_lights.get(i);
        
        bool append = true;
        for (int j = 0; j < target.size(); ++j) {
            Ref<VoxelLight> l2 = target.get(j);
            
            if (!l2.is_valid())
                continue;
            
            if (l2->get_world_position() == l->get_world_position() && l2->get_size() == l->get_size()) {
                append = false;
                break;
            }
        }
        
        if (append)
            target.append(l);
	}
}

Array VoxelChunk::get_lights() {
	Array target;

	for (int i = 0; i < _voxel_lights.size(); ++i) {
		target.append(_voxel_lights[i]);
	}

	return target;
}

void VoxelChunk::bake_lights() {
	clear_baked_lights();

	for (int i = 0; i < _voxel_lights.size(); ++i) {
		bake_light(_voxel_lights[i]);
	}
}
void VoxelChunk::bake_light(Ref<VoxelLight> light) {
	ERR_FAIL_COND(!light.is_valid());

	Vector3i wp = light->get_world_position();

	int wpx = wp.x - (_chunk_position.x * _chunk_size.x);
	int wpy = wp.y - (_chunk_position.y * _chunk_size.y);
	int wpz = wp.z - (_chunk_position.z * _chunk_size.z);

	//int wpx = (int)(wp.x / _chunk_size.x) - _chunk_position.x;
	//int wpy = (int)(wp.y / _chunk_size.y) - _chunk_position.y;
	//int wpz = (int)(wp.z / _chunk_size.z) - _chunk_position.z;

	_buffer->add_light(wpx, wpy, wpz, light->get_size(), light->get_color());
}

void VoxelChunk::clear_baked_lights() {
	_buffer->clear_lights();
}

void VoxelChunk::add_prop_mesh(const Ref<MeshDataResource> mesh, const Vector3 position, const Vector3 rotation, const Vector3 scale) {
	VCPropData data;

	data.position = position;
	data.rotation = rotation;
	data.scale = scale;

	data.mesh = mesh;

	_props.push_back(data);
}

void VoxelChunk::add_prop_spawned(const Ref<PackedScene> scene, const Vector3 position, const Vector3 rotation, const Vector3 scale) {
	VCPropData data;

	data.position = position;
	data.rotation = rotation;
	data.scale = scale;

	data.scene = scene;

	_props.push_back(data);
}

void VoxelChunk::add_prop(const Ref<VoxelmanProp> prop, const Vector3 position, const Vector3 rotation, const Vector3 scale) {
	VCPropData data;

	data.position = position;
	data.rotation = rotation;
	data.scale = scale;

	data.mesh = prop;

	_props.push_back(data);
}

void VoxelChunk::clear_props() {
	_props.clear();
}

void VoxelChunk::allocate_prop_mesh() {
	ERR_FAIL_COND(_voxel_world == NULL);
	ERR_FAIL_COND(!get_library().is_valid());
	ERR_FAIL_COND(!get_library()->get_prop_material().is_valid());

	_prop_mesh_instance_rid = VS::get_singleton()->instance_create();

	if (get_library()->get_prop_material().is_valid()) {
		VS::get_singleton()->instance_geometry_set_material_override(_prop_mesh_instance_rid, get_library()->get_prop_material()->get_rid());
	}

	if (get_voxel_world()->get_world().is_valid())
		VS::get_singleton()->instance_set_scenario(_prop_mesh_instance_rid, get_voxel_world()->get_world()->get_scenario());

	_prop_mesh_rid = VS::get_singleton()->mesh_create();

	VS::get_singleton()->instance_set_base(_prop_mesh_instance_rid, _prop_mesh_rid);

	VS::get_singleton()->instance_set_transform(_prop_mesh_instance_rid, Transform(Basis(), Vector3(_chunk_position.x * _chunk_size.x * _voxel_scale, _chunk_position.y * _chunk_size.y * _voxel_scale, _chunk_position.z * _chunk_size.z * _voxel_scale)));
}

void VoxelChunk::process_prop_lights() {
	Transform transform(Basis().scaled(Vector3(_voxel_scale, _voxel_scale, _voxel_scale)), Vector3(_chunk_position.x * _chunk_size.x, _chunk_position.y * _chunk_size.y, _chunk_position.z * _chunk_size.z));

	for (int i = 0; i < _props.size(); ++i) {
		VCPropData prop = _props[i];

		if (prop.prop.is_valid()) {
			process_prop_light(prop.prop, transform);
		}
	}
}

void VoxelChunk::process_props() {
	if (_prop_mesh_rid == RID()) {
		allocate_prop_mesh();
	}

	for (int i = 0; i < _props.size(); ++i) {
		VCPropData prop = _props[i];

		Transform transform(Basis(prop.rotation).scaled(prop.scale), prop.position);

		if (prop.mesh.is_valid()) {
			_mesher->add_mesh_data_resource_transform(prop.mesh, transform);
		}

		if (prop.prop.is_valid()) {
			process_prop(prop.prop, transform);
		}

		if (prop.scene.is_valid()) {
			spawn_prop(prop.scene, transform);
		}
	}

	_mesher->bake_colors(_buffer);

	_mesher->build_mesh(_prop_mesh_rid);
}

void VoxelChunk::process_prop_light(Ref<VoxelmanProp> prop, const Transform transform) {
	ERR_FAIL_COND(!prop.is_valid());

	for (int i = 0; i < prop->get_prop_count(); ++i) {
		Ref<VoxelmanPropData> data = prop->get_prop(i);

		if (!data.is_valid())
			continue;

		Transform pt(Basis(data->get_rotation()).scaled(data->get_scale()), data->get_position());

		pt = transform * pt;

		if (data->get_has_light()) {
			Vector3 lp = pt.xform_inv(Vector3());

			create_voxel_light(data->get_light_color(), data->get_light_size(), (int)lp.x, (int)lp.y, (int)lp.z);
		}

		if (data->get_prop().is_valid()) {
			process_prop_light(data->get_prop(), pt);
		}
	}
}

void VoxelChunk::process_prop(Ref<VoxelmanProp> prop, const Transform transform) {
	ERR_FAIL_COND(!prop.is_valid());

	for (int i = 0; i < prop->get_prop_count(); ++i) {
		Ref<VoxelmanPropData> data = prop->get_prop(i);

		if (!data.is_valid())
			continue;

		Transform pt(Basis(data->get_rotation()).scaled(data->get_scale()), data->get_position());

		if (data->get_mesh().is_valid()) {
			_mesher->add_mesh_data_resource_transform(data->get_mesh(), pt);
		}

		if (data->get_scene().is_valid()) {
			spawn_prop(data->get_scene(), pt);
		}

		if (data->get_prop().is_valid()) {
			process_prop(data->get_prop(), pt);
		}
	}
}

void VoxelChunk::spawn_prop(const Ref<PackedScene> scene, const Transform transform) {
	ERR_FAIL_COND(!scene.is_valid());
	ERR_FAIL_COND(get_voxel_world() == NULL);

	Node *n = scene->instance();

	ERR_FAIL_COND(n == NULL);

	get_voxel_world()->add_child(n);
	n->set_owner(get_voxel_world());

	_spawned_props.push_back(n);

	Spatial *spatial = Object::cast_to<Spatial>(n);

	ERR_FAIL_COND(spatial == NULL);

	spatial->set_transform(transform);
}

void VoxelChunk::build_prop_meshes() {
	if (_prop_mesh_rid == RID()) {
		allocate_prop_mesh();
	}

	_mesher->bake_colors(_buffer);

	_mesher->build_mesh(_prop_mesh_rid);
}

void VoxelChunk::free_prop_mesh() {
	if (_prop_mesh_instance_rid != RID()) {
		VS::get_singleton()->free(_prop_mesh_instance_rid);
		VS::get_singleton()->free(_prop_mesh_rid);

		_prop_mesh_instance_rid = RID();
		_prop_mesh_rid = RID();
	}
}

void VoxelChunk::allocate_prop_colliders() {
	ERR_FAIL_COND(_voxel_world == NULL);

	_prop_shape_rid = PhysicsServer::get_singleton()->shape_create(PhysicsServer::SHAPE_CONCAVE_POLYGON);
	_prop_body_rid = PhysicsServer::get_singleton()->body_create(PhysicsServer::BODY_MODE_STATIC);

	PhysicsServer::get_singleton()->body_set_collision_layer(_prop_body_rid, 1);
	PhysicsServer::get_singleton()->body_set_collision_mask(_prop_body_rid, 1);

	PhysicsServer::get_singleton()->body_add_shape(_prop_body_rid, _prop_shape_rid);

	PhysicsServer::get_singleton()->body_set_state(_prop_body_rid, PhysicsServer::BODY_STATE_TRANSFORM, Transform(Basis(), Vector3(_chunk_position.x * _chunk_size.x * _voxel_scale, _chunk_position.y * _chunk_size.y * _voxel_scale, _chunk_position.z * _chunk_size.z * _voxel_scale)));
	PhysicsServer::get_singleton()->body_set_space(_prop_body_rid, get_voxel_world()->get_world()->get_space());
}
void VoxelChunk::build_prop_collider() {
	if (_prop_shape_rid == RID()) {
		allocate_prop_colliders();
	}

	_mesher->build_collider(_prop_shape_rid);
}
void VoxelChunk::free_prop_colliders() {
	if (_prop_body_rid != RID()) {
		PhysicsServer::get_singleton()->free(_prop_body_rid);
		PhysicsServer::get_singleton()->free(_prop_shape_rid);

		_prop_body_rid = RID();
		_prop_shape_rid = RID();
	}
}

void VoxelChunk::free_spawn_props() {
	for (int i = 0; i < _spawned_props.size(); ++i) {
		_spawned_props[i]->queue_delete();
	}

	_spawned_props.clear();
}

void VoxelChunk::allocate_main_mesh() {
	ERR_FAIL_COND(_voxel_world == NULL);

	ERR_FAIL_COND(!get_library().is_valid());
	ERR_FAIL_COND(!get_library()->get_material().is_valid());

	_mesh_instance_rid = VS::get_singleton()->instance_create();

	if (get_library()->get_material().is_valid()) {
		VS::get_singleton()->instance_geometry_set_material_override(_mesh_instance_rid, get_library()->get_material()->get_rid());
	}

	if (get_voxel_world()->get_world().is_valid())
		VS::get_singleton()->instance_set_scenario(_mesh_instance_rid, get_voxel_world()->get_world()->get_scenario());

	_mesh_rid = VS::get_singleton()->mesh_create();

	VS::get_singleton()->instance_set_base(_mesh_instance_rid, _mesh_rid);

	VS::get_singleton()->instance_set_transform(_mesh_instance_rid, Transform(Basis(), Vector3(_chunk_position.x * _chunk_size.x * _voxel_scale, _chunk_position.y * _chunk_size.y * _voxel_scale, _chunk_position.z * _chunk_size.z * _voxel_scale)));
}

void VoxelChunk::free_main_mesh() {
	if (_mesh_instance_rid != RID()) {
		VS::get_singleton()->free(_mesh_instance_rid);
		VS::get_singleton()->free(_mesh_rid);

		_mesh_instance_rid = RID();
		_mesh_rid = RID();
	}
}

void VoxelChunk::create_debug_immediate_geometry() {
	ERR_FAIL_COND(_voxel_world == NULL);
	ERR_FAIL_COND(_debug_drawer != NULL);

	_debug_drawer = memnew(ImmediateGeometry());

	get_voxel_world()->add_child(_debug_drawer);
	_debug_drawer->set_owner(get_voxel_world());

	_debug_drawer->set_transform(Transform(Basis(), Vector3(_chunk_position.x * _chunk_size.x * _voxel_scale, _chunk_position.y * _chunk_size.y * _voxel_scale, _chunk_position.z * _chunk_size.z * _voxel_scale)));
}

void VoxelChunk::free_debug_immediate_geometry() {
	if (_debug_drawer != NULL) {
		_debug_drawer->queue_delete();

		_debug_drawer = NULL;
	}
}

void VoxelChunk::free_chunk() {
	free_debug_immediate_geometry();
	free_main_mesh();
	remove_colliders();
	free_prop_mesh();
	free_prop_colliders();
	free_spawn_props();
}

void VoxelChunk::draw_cross_voxels(Vector3 pos) {
	pos *= _voxel_scale;

	_debug_drawer->add_vertex(pos + Vector3(0, 0, -0.2));
	_debug_drawer->add_vertex(pos + Vector3(0, 0, 0.2));

	_debug_drawer->add_vertex(pos + Vector3(0, -0.2, 0));
	_debug_drawer->add_vertex(pos + Vector3(0, 0.2, 0));

	_debug_drawer->add_vertex(pos + Vector3(-0.2, 0, 0));
	_debug_drawer->add_vertex(pos + Vector3(0.2, 0, 0));
}

void VoxelChunk::draw_cross_voxels_fill(Vector3 pos, float fill) {
	pos *= _voxel_scale;

	_debug_drawer->add_vertex(pos + Vector3(0, 0, -0.5 * fill));
	_debug_drawer->add_vertex(pos + Vector3(0, 0, 0.5 * fill));

	_debug_drawer->add_vertex(pos + Vector3(0, -0.5 * fill, 0));
	_debug_drawer->add_vertex(pos + Vector3(0, 0.5 * fill, 0));

	_debug_drawer->add_vertex(pos + Vector3(-0.5 * fill, 0, 0));
	_debug_drawer->add_vertex(pos + Vector3(0.5 * fill, 0, 0));
}

void VoxelChunk::draw_debug_voxels(int max, Color color) {
	if (_debug_drawer == NULL) {
		create_debug_immediate_geometry();
	}

	ERR_FAIL_COND(_debug_drawer == NULL);

	_debug_drawer->clear();
	_debug_drawer->begin(Mesh::PRIMITIVE_LINES);
	_debug_drawer->set_color(color);

	int a = 0;
	Vector3i size = _buffer->get_size();

	for (int y = 0; y < size.y; ++y) {
		for (int z = 0; z < size.z; ++z) {
			for (int x = 0; x < size.x; ++x) {

				int type = _buffer->get_voxel(x, y, z, VoxelBuffer::CHANNEL_TYPE);

				if (type == 0) {
					continue;
				}

				draw_cross_voxels_fill(Vector3(x, y, z), _buffer->get_voxel(x, y, z, VoxelBuffer::CHANNEL_ISOLEVEL) / 255.0);

				++a;

				if (a > max) {
					break;
				}
			}
		}
	}

	_debug_drawer->end();
}

void VoxelChunk::draw_debug_voxel_lights() {
	if (_debug_drawer == NULL) {
		create_debug_immediate_geometry();
	}

	ERR_FAIL_COND(_debug_drawer == NULL);

	_debug_drawer->clear();
	_debug_drawer->begin(Mesh::PrimitiveType::PRIMITIVE_LINES);
	_debug_drawer->set_color(Color(1, 1, 1));

	for (int i = 0; i < _voxel_lights.size(); ++i) {
		Ref<VoxelLight> v = _voxel_lights[i];

		Vector3i pos = v->get_world_position();

		int pos_x = pos.x - (_chunk_size.x * _chunk_position.x);
		int pos_y = pos.y - (_chunk_size.y * _chunk_position.y);
		int pos_z = pos.z - (_chunk_size.z * _chunk_position.z);

		draw_cross_voxels_fill(Vector3(pos_x, pos_y, pos_z), 1.0);
	}

	_debug_drawer->end();
}

VoxelChunk::VoxelChunk() {
	_build_mesh = true;
	_create_collider = true;
	_bake_lights = true;

	_voxel_scale = 1;
	_lod_size = 1;

	_buffer.instance();

	_debug_drawer = NULL;
	_voxel_world = NULL;
}

VoxelChunk::~VoxelChunk() {
	free_main_mesh();
	remove_colliders();
	free_prop_mesh();
	free_prop_colliders();
	//do not call free here, the app will crash on exit, if you try to free nodes too.

	_voxel_lights.clear();

	if (_mesher.is_valid()) {
		_mesher.unref();
	}

	_buffer.unref();

	if (_library.is_valid()) {
		_library.unref();
	}
}

void VoxelChunk::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_create_mesh"));
	BIND_VMETHOD(MethodInfo("_create_mesher"));

	ClassDB::bind_method(D_METHOD("_create_mesher"), &VoxelChunk::_create_mesher);

	ClassDB::bind_method(D_METHOD("get_chunk_position_x"), &VoxelChunk::get_chunk_position_x);
	ClassDB::bind_method(D_METHOD("set_chunk_position_x", "value"), &VoxelChunk::set_chunk_position_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_position_x"), "set_chunk_position_x", "get_chunk_position_x");

	ClassDB::bind_method(D_METHOD("get_chunk_position_y"), &VoxelChunk::get_chunk_position_y);
	ClassDB::bind_method(D_METHOD("set_chunk_position_y", "value"), &VoxelChunk::set_chunk_position_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_position_y"), "set_chunk_position_y", "get_chunk_position_y");

	ClassDB::bind_method(D_METHOD("get_chunk_position_z"), &VoxelChunk::get_chunk_position_z);
	ClassDB::bind_method(D_METHOD("set_chunk_position_z", "value"), &VoxelChunk::set_chunk_position_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_position_z"), "set_chunk_position_x", "get_chunk_position_z");

	ClassDB::bind_method(D_METHOD("set_chunk_position", "x", "y", "z"), &VoxelChunk::set_chunk_position);

	ClassDB::bind_method(D_METHOD("get_chunk_size_x"), &VoxelChunk::get_chunk_size_x);
	ClassDB::bind_method(D_METHOD("set_chunk_size_x", "value"), &VoxelChunk::set_chunk_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_x"), "set_chunk_size_x", "get_chunk_size_x");

	ClassDB::bind_method(D_METHOD("get_chunk_size_y"), &VoxelChunk::get_chunk_size_y);
	ClassDB::bind_method(D_METHOD("set_chunk_size_y", "value"), &VoxelChunk::set_chunk_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_y"), "set_chunk_size_y", "get_chunk_size_y");

	ClassDB::bind_method(D_METHOD("get_chunk_size_z"), &VoxelChunk::get_chunk_size_z);
	ClassDB::bind_method(D_METHOD("set_chunk_size_z", "value"), &VoxelChunk::set_chunk_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_z"), "set_chunk_size_x", "get_chunk_size_z");

	ClassDB::bind_method(D_METHOD("set_chunk_size", "x", "y", "z"), &VoxelChunk::set_chunk_size);

	ClassDB::bind_method(D_METHOD("get_library"), &VoxelChunk::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "value"), &VoxelChunk::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, "VoxelmanLibrary"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_lod_size"), &VoxelChunk::get_lod_size);
	ClassDB::bind_method(D_METHOD("set_lod_size", "value"), &VoxelChunk::set_lod_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_size"), "set_lod_size", "get_lod_size");

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &VoxelChunk::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &VoxelChunk::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("get_buffer"), &VoxelChunk::get_buffer);

	ADD_GROUP("Meshing", "meshing");

	ClassDB::bind_method(D_METHOD("meshing_get_create_collider"), &VoxelChunk::get_create_collider);
	ClassDB::bind_method(D_METHOD("meshing_set_create_collider", "value"), &VoxelChunk::set_create_collider);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "meshing_create_collider"), "meshing_set_create_collider", "meshing_get_create_collider");

	ClassDB::bind_method(D_METHOD("meshing_get_bake_lights"), &VoxelChunk::get_bake_lights);
	ClassDB::bind_method(D_METHOD("meshing_set_bake_lights", "value"), &VoxelChunk::set_bake_lights);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "meshing_bake_lights"), "meshing_set_bake_lights", "meshing_get_bake_lights");

	ADD_GROUP("Settings", "setting");

	ClassDB::bind_method(D_METHOD("get_mesher"), &VoxelChunk::get_mesher);
	ClassDB::bind_method(D_METHOD("set_mesher", "Mesher"), &VoxelChunk::set_mesher);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesher", PROPERTY_HINT_RESOURCE_TYPE, "VoxelMesher"), "set_mesher", "get_mesher");

	ClassDB::bind_method(D_METHOD("get_voxel_world"), &VoxelChunk::get_voxel_world);
	ClassDB::bind_method(D_METHOD("set_voxel_world", "world"), &VoxelChunk::set_voxel_world_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "voxel_world", PROPERTY_HINT_RESOURCE_TYPE, "VoxelWorld"), "set_voxel_world", "get_voxel_world");

	ClassDB::bind_method(D_METHOD("finalize_mesh"), &VoxelChunk::finalize_mesh);

	ClassDB::bind_method(D_METHOD("build"), &VoxelChunk::build);
	ClassDB::bind_method(D_METHOD("clear"), &VoxelChunk::clear);

	ClassDB::bind_method(D_METHOD("create_colliders"), &VoxelChunk::create_colliders);
	ClassDB::bind_method(D_METHOD("build_collider"), &VoxelChunk::build_collider);
	ClassDB::bind_method(D_METHOD("remove_colliders"), &VoxelChunk::remove_colliders);

	ClassDB::bind_method(D_METHOD("add_lights", "lights"), &VoxelChunk::add_lights);
	ClassDB::bind_method(D_METHOD("add_voxel_light", "light"), &VoxelChunk::add_voxel_light);
	ClassDB::bind_method(D_METHOD("create_voxel_light", "color", "size", "x", "y", "z"), &VoxelChunk::create_voxel_light);

	ClassDB::bind_method(D_METHOD("remove_voxel_light", "light"), &VoxelChunk::remove_voxel_light);
	ClassDB::bind_method(D_METHOD("clear_voxel_lights"), &VoxelChunk::clear_voxel_lights);
	ClassDB::bind_method(D_METHOD("add_lights_into", "lights"), &VoxelChunk::add_lights_into);
	ClassDB::bind_method(D_METHOD("add_unique_lights_into", "lights"), &VoxelChunk::add_unique_lights_into);
	ClassDB::bind_method(D_METHOD("get_lights"), &VoxelChunk::get_lights);

	ClassDB::bind_method(D_METHOD("bake_lights"), &VoxelChunk::bake_lights);
	ClassDB::bind_method(D_METHOD("bake_light", "light"), &VoxelChunk::bake_light);
	ClassDB::bind_method(D_METHOD("clear_baked_lights"), &VoxelChunk::clear_baked_lights);

	ClassDB::bind_method(D_METHOD("add_prop_mesh", "mesh", "position", "rotation", "scale"), &VoxelChunk::add_prop_mesh, DEFVAL(Vector3()), DEFVAL(Vector3()), DEFVAL(Vector3(1.0, 1.0, 1.0)));
	ClassDB::bind_method(D_METHOD("add_prop_spawned", "scene", "position", "rotation", "scale"), &VoxelChunk::add_prop_spawned, DEFVAL(Vector3()), DEFVAL(Vector3()), DEFVAL(Vector3(1.0, 1.0, 1.0)));
	ClassDB::bind_method(D_METHOD("add_prop", "prop", "position", "rotation", "scale"), &VoxelChunk::add_prop, DEFVAL(Vector3()), DEFVAL(Vector3()), DEFVAL(Vector3(1.0, 1.0, 1.0)));
	ClassDB::bind_method(D_METHOD("clear_props"), &VoxelChunk::clear_props);

	ClassDB::bind_method(D_METHOD("process_prop_lights"), &VoxelChunk::process_prop_lights);
	ClassDB::bind_method(D_METHOD("process_props"), &VoxelChunk::process_props);
	ClassDB::bind_method(D_METHOD("process_prop", "prop", "transform"), &VoxelChunk::process_prop, DEFVAL(Transform()));
	ClassDB::bind_method(D_METHOD("process_prop_light", "prop", "transform"), &VoxelChunk::process_prop_light, DEFVAL(Transform()));
	ClassDB::bind_method(D_METHOD("spawn_prop", "transform"), &VoxelChunk::spawn_prop, DEFVAL(Transform()));

	ClassDB::bind_method(D_METHOD("build_prop_meshes"), &VoxelChunk::build_prop_meshes);
	ClassDB::bind_method(D_METHOD("build_prop_collider"), &VoxelChunk::build_prop_collider);
	ClassDB::bind_method(D_METHOD("free_spawn_props"), &VoxelChunk::free_spawn_props);

	ClassDB::bind_method(D_METHOD("allocate_main_mesh"), &VoxelChunk::allocate_main_mesh);
	ClassDB::bind_method(D_METHOD("free_main_mesh"), &VoxelChunk::free_main_mesh);

	ClassDB::bind_method(D_METHOD("allocate_prop_mesh"), &VoxelChunk::allocate_prop_mesh);
	ClassDB::bind_method(D_METHOD("free_prop_mesh"), &VoxelChunk::free_prop_mesh);

	ClassDB::bind_method(D_METHOD("allocate_prop_colliders"), &VoxelChunk::allocate_prop_colliders);
	ClassDB::bind_method(D_METHOD("free_prop_colliders"), &VoxelChunk::free_prop_colliders);

	ClassDB::bind_method(D_METHOD("create_mesher"), &VoxelChunk::create_mesher);

	ClassDB::bind_method(D_METHOD("create_debug_immediate_geometry"), &VoxelChunk::create_debug_immediate_geometry);
	ClassDB::bind_method(D_METHOD("free_debug_immediate_geometry"), &VoxelChunk::free_debug_immediate_geometry);

	ClassDB::bind_method(D_METHOD("free_chunk"), &VoxelChunk::free_chunk);

	ClassDB::bind_method(D_METHOD("draw_cross_voxels", "pos"), &VoxelChunk::draw_cross_voxels);
	ClassDB::bind_method(D_METHOD("draw_cross_voxels_fill", "pos", "fill"), &VoxelChunk::draw_cross_voxels_fill);
	ClassDB::bind_method(D_METHOD("draw_debug_voxels", "pos", "color"), &VoxelChunk::draw_debug_voxels, DEFVAL(Color(1, 1, 1)));

	ClassDB::bind_method(D_METHOD("draw_debug_voxel_lights"), &VoxelChunk::draw_debug_voxel_lights);
}

