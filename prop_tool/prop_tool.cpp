#include "prop_tool.h"

#ifdef TOOLS_ENABLED

#include "../props/prop_data.h"
#include "../props/prop_data_entity.h"
#include "../props/prop_data_light.h"
#include "../props/prop_data_mesh.h"
#include "../props/prop_data_prop.h"
#include "../props/prop_data_scene.h"

#include "prop_tool_entity.h"
#include "prop_tool_light.h"
#include "prop_tool_mesh.h"
#include "prop_tool_prop.h"
#include "prop_tool_scene.h"

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
	for (int i = 0; i < get_child_count(); ++i)
		get_child(i)->queue_delete();

	if (!_target_prop.is_valid())
		return;

	_snap_to_mesh = _target_prop->get_snap_to_mesh();
	_snap_axis = _target_prop->get_snap_axis();

	for (int i = 0; i < _target_prop->get_prop_count(); ++i) {
		Ref<PropDataEntry> prop = _target_prop->get_prop(i);

		Ref<PropDataLight> prop_light = prop;
		Ref<PropDataMesh> prop_mesh = prop;
		Ref<PropDataScene> prop_scene = prop;
		Ref<PropDataProp> prop_prop = prop;
		Ref<PropDataEntity> prop_entity = prop;

		if (prop_light.is_valid()) {
			PropToolLight *l = memnew(PropToolLight);

			add_child(l);
			l->set_owner(this);
			l->set_transform(prop->get_transform());

			l->set_data(prop_light);
		} else if (prop_mesh.is_valid()) {
			PropToolMesh *m = memnew(PropToolMesh);

			add_child(m);
			m->set_owner(this);
			m->set_transform(prop->get_transform());

			m->set_data(prop_mesh);
		} else if (prop_scene.is_valid()) {
			PropToolScene *s = memnew(PropToolScene);

			add_child(s);
			s->set_owner(this);
			s->set_transform(prop->get_transform());

			s->set_data(prop_scene);
		} else if (prop_prop.is_valid()) {
			PropTool *s = _plugin->create_or_get_scene(prop_prop->get_prop());

			add_child(s);
			s->set_owner(this);
			s->set_transform(prop->get_transform());
			s->set_target_prop(prop_prop->get_prop());
		} else if (prop_entity.is_valid()) {
			PropToolEntity *s = memnew(PropToolEntity);

			add_child(s);
			s->set_owner(this);
			s->set_transform(prop->get_transform());

			s->set_data(prop_entity);
		}
	}
}

void PropTool::refresh_set(bool value) {
	if (value)
		rebuild_hierarchy();
}

void PropTool::set_target_prop(const Ref<PropData> &prop) {
	_target_prop = prop;

	rebuild_hierarchy();
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

	Node *s = _plugin->create_or_get_scene(prop);

	add_child(s);
	//s->set_owner(this);
}

PropToolEditorPlugin *PropTool::get_plugin() {
	return _plugin;
}

void PropTool::set_plugin(PropToolEditorPlugin *plugin) {
	_plugin = plugin;
}

PropTool::PropTool() {
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, -1, 0);
}
PropTool::PropTool(EditorNode *p_editor) {
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, -1, 0);
}
PropTool::~PropTool() {
	_target_prop.unref();
}

void PropTool::_bind_methods() {
}

#endif