#include "prop_tool.h"

#include "../props/prop_data.h"
#include "../props/prop_data_prop.h"
#include "core/io/resource_saver.h"
#include "prop_tool_editor_plugin.h"

void PropTool::edit(const Ref<PropData> &prop) {
}

void PropTool::save() {
	if (!_target_prop.is_valid())
		return;

	print_verbose("save " + _target_prop->get_path());

	while (_target_prop->get_prop_count() > 0)
		_target_prop->remove_prop(0);

	for (int i = 0; i < get_child_count(); ++i) {
		save_node(get_child(i), get_transform());
	}

	_target_prop->set_snap_to_mesh(_snap_to_mesh);
	_target_prop->set_snap_axis(_snap_axis);

	ResourceSaver::save(_target_prop->get_path(), _target_prop);
}

void PropTool::save_node(Node *node, Transform parent_transform) {
	Spatial *snode = Object::cast_to<Spatial>(node);

	if (ObjectDB::instance_validate(snode) && snode->has_method("get_data")) {
		Ref<PropDataEntry> prop = node->call("get_data");

		if (prop.is_valid()) {
			prop->set_transform(parent_transform * snode->get_transform());

			_target_prop->add_prop(prop);
		}

		if (snode->has_method("evaluate_children") && !snode->call("evaluate_children"))
			return;

		for (int i = 0; i < get_child_count(); ++i) {
			save_node(get_child(i), parent_transform * snode->get_transform());
		}
	} else {
		if (node->has_method("set_target_prop") && node->has_method("get_target_prop")) {
			Ref<PropData> tprop = node->call("get_target_prop");

			if (tprop.is_valid()) {
				Ref<PropDataProp> prop;
				prop.instance();

				prop->set_prop(tprop);

				prop->set_transform(parent_transform * snode->get_transform());

				_target_prop->add_prop(prop);
			}
		} else {
			for (int i = 0; i < get_child_count(); ++i) {
				save_node(get_child(i), parent_transform * snode->get_transform());
			}
		}
	}
}

void PropTool::rebuild_hierarchy() {
	/*
	for ch in get_children():
		ch.queue_free()
	
	if target_prop == null:
		return
		
	snap_to_mesh = target_prop.snap_to_mesh
	snap_axis = target_prop.snap_axis
		
	for i in range(target_prop.get_prop_count()):
		print(i)
		var prop : PropDataEntry = target_prop.get_prop(i)
		
		if prop is PropDataLight:
			var l : PropToolLight = PropToolLight.new()
			
			add_child(l)
			l.owner = self
			l.transform = prop.transform
			
			l.set_data(prop as PropDataLight)
		elif prop is PropDataMesh:
			var m : PropToolMesh = PropToolMesh.new()
			
			add_child(m)
			m.owner = self
			m.transform = prop.transform
			
			m.set_data(prop as PropDataMesh)
		elif prop is PropDataScene:
			var s : PropToolScene = PropToolScene.new()
			
			add_child(s)
			s.owner = self
			s.transform = prop.transform
			
			s.set_data(prop as PropDataScene)
		elif prop is PropDataProp:
			var s : Node = plugin.create_or_get_scene(prop.prop)
			
			add_child(s)
			s.owner = self
			s.transform = prop.transform
#s.set_target_prop(prop.prop)
		elif prop is PropDataEntity:
			var s : PropToolEntity = PropToolEntity.new()
			
			add_child(s)
			s.owner = self
			s.transform = prop.transform
			
			s.set_data(prop as PropDataEntity)
*/
}

void PropTool::refresh_set(bool value) {
	if (value)
		rebuild_hierarchy();
}

void PropTool::set_target_prop(const Ref<PropData> &prop) {
	_target_prop = prop;

	rebuild_hierarchy();
}

void PropTool::get_plugin() {
	//return _plugin;
}

void PropTool::target_prop_set(const Ref<PropData> &prop) {
	_target_prop = prop;

	if (!prop.is_valid())
		return;

	PropTool *last_prop_tool = this;
	Node *root = this;

	while (root->get_parent() != NULL) {
		root = root->get_parent();

		if (root && root->has_method("target_prop_set"))
			last_prop_tool = Object::cast_to<PropTool>(root);
	}

	if (last_prop_tool == this)
		return;

	last_prop_tool->load_scene_for(this, prop);
}

void PropTool::load_scene_for(PropTool *t, const Ref<PropData> &prop) {
	if (_plugin == NULL)
		return;

	t->queue_delete();

	//Node *s = _plugin->create_or_get_scene(prop);

	//add_child(s);
	//s->set_owner(this);
}

PropTool::PropTool() {
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, -1, 0);
}
PropTool::PropTool(EditorNode *p_editor) {
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, -1, 0);
}
