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

#include "prop_tool_editor_plugin.h"

#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"
#include "scene/resources/packed_scene.h"

#include "../props/prop_data.h"

#include "prop_tool.h"
#include "prop_tool_entity.h"
#include "prop_tool_light.h"
#include "prop_tool_mesh.h"
#include "prop_tool_scene.h"

#include "editor/plugins/spatial_editor_plugin.h"

void PropToolEditorPlugin::edit(Object *p_object) {
	Ref<PropData> pedited_prop(Object::cast_to<PropData>(p_object));

	String p = create_or_get_scene_path(pedited_prop);

	get_editor_interface()->open_scene_from_path(p);
}

bool PropToolEditorPlugin::handles(Object *p_object) const {

	bool handles = p_object->is_class("PropData");

	return handles;
}

void PropToolEditorPlugin::make_visible(bool p_visible) {
}

String PropToolEditorPlugin::create_or_get_scene_path(const Ref<PropData> &data) {
	ERR_FAIL_COND_V(!data.is_valid(), "");

	String temp_path = EditorSettings::get_singleton()->get("editors/prop_tool/temp_path");

	String path = temp_path + data->get_path().get_file().get_basename() + ".tscn";

	if (!FileAccess::exists(path))
		create_scene(data);

	return path;
}
PropTool *PropToolEditorPlugin::create_or_get_scene(const Ref<PropData> &data) {
	ERR_FAIL_COND_V(!data.is_valid(), NULL);

	String temp_path = EditorSettings::get_singleton()->get("editors/prop_tool/temp_path");

	String path = temp_path + data->get_path().get_file().get_basename() + ".tscn";

	Ref<PackedScene> ps;

	if (!FileAccess::exists(path))
		ps = create_scene(data);
	else
		ps = ResourceLoader::load(path, "PackedScene");

	if (!ps.is_valid())
		return NULL;

	PropTool *pt = Object::cast_to<PropTool>(ps->instance());
	pt->set_plugin(this);
	return pt;
}
Ref<PackedScene> PropToolEditorPlugin::create_scene(const Ref<PropData> &data) {
	PropTool *pt = memnew(PropTool);

	pt->set_plugin(this);
	pt->set_target_prop(data);

	Ref<PackedScene> ps;
	ps.instance();
	ps->pack(pt);

	String temp_path = EditorSettings::get_singleton()->get("editors/prop_tool/temp_path");

	Error err = ResourceSaver::save(temp_path + data->get_path().get_file().get_basename() + ".tscn", ps);

	if (err)
		print_error("PropTool: create_scene failed! Error_code:" + String::num(err));

	pt->queue_delete();
	return ps;
}

void PropToolEditorPlugin::on_scene_changed(Node *scene) {
	PropTool *pt = Object::cast_to<PropTool>(scene);

	if (ObjectDB::instance_validate(pt)) {
		pt->set_plugin(this);

		light_button->show();
		mesh_button->show();
		prop_button->show();
		scene_button->show();
		entity_button->show();
	} else {
		light_button->hide();
		mesh_button->hide();
		prop_button->hide();
		scene_button->hide();
		entity_button->hide();
	}
}

void PropToolEditorPlugin::apply_changes() {
	Node *scene = get_editor_interface()->get_edited_scene_root();
	PropTool *pt = Object::cast_to<PropTool>(scene);

	if (ObjectDB::instance_validate(pt)) {
		pt->save();
	}
}

void PropToolEditorPlugin::add_light() {
	Array selection = get_editor_interface()->get_selection()->get_selected_nodes();

	Node *selected;

	if (selection.size() != 1)
		selected = get_editor_interface()->get_edited_scene_root();
	else
		selected = selection[0];

	Node *s = selected;
	PropToolLight *n = memnew(PropToolLight);

	UndoRedo u = get_undo_redo();
	u.create_action("Add Light");
	u.add_do_method(s, "add_child", n);
	u.add_do_property(n, "owner", get_editor_interface()->get_edited_scene_root());
	u.add_undo_method(s, "remove_child", n);
	u.commit_action();

	get_editor_interface()->get_selection()->clear();
	get_editor_interface()->get_selection()->add_node(n);
}

void PropToolEditorPlugin::add_mesh() {
	Array selected = get_editor_interface()->get_selection()->get_selected_nodes();

	if (selected.size() != 1)
		return;

	Node *s = selected[0];
	PropToolMesh *n = memnew(PropToolMesh);

	UndoRedo u = get_undo_redo();
	u.create_action("Add Mesh");
	u.add_do_method(s, "add_child", n);
	u.add_do_property(n, "owner", get_editor_interface()->get_edited_scene_root());
	u.add_undo_method(s, "remove_child", n);
	u.commit_action();

	get_editor_interface()->get_selection()->clear();
	get_editor_interface()->get_selection()->add_node(n);
}

void PropToolEditorPlugin::add_prop() {

	Array selected = get_editor_interface()->get_selection()->get_selected_nodes();

	if (selected.size() != 1)
		return;

	Node *s = selected[0];
	PropTool *n = memnew(PropTool);

	UndoRedo u = get_undo_redo();
	u.create_action("Add Prop");
	u.add_do_method(s, "add_child", n);
	u.add_do_property(n, "owner", get_editor_interface()->get_edited_scene_root());
	u.add_undo_method(s, "remove_child", n);
	u.commit_action();

	get_editor_interface()->get_selection()->clear();
	get_editor_interface()->get_selection()->add_node(n);
}

void PropToolEditorPlugin::add_scene() {
	Array selected = get_editor_interface()->get_selection()->get_selected_nodes();

	if (selected.size() != 1)
		return;

	Node *s = selected[0];
	PropToolScene *n = memnew(PropToolScene);

	UndoRedo u = get_undo_redo();
	u.create_action("Add Scene");
	u.add_do_method(s, "add_child", n);
	u.add_do_property(n, "owner", get_editor_interface()->get_edited_scene_root());
	u.add_undo_method(s, "remove_child", n);
	u.commit_action();

	get_editor_interface()->get_selection()->clear();
	get_editor_interface()->get_selection()->add_node(n);
}

void PropToolEditorPlugin::add_entity() {
	Array selected = get_editor_interface()->get_selection()->get_selected_nodes();

	if (selected.size() != 1)
		return;

	Node *s = selected[0];
	PropToolEntity *n = memnew(PropToolEntity);

	UndoRedo u = get_undo_redo();
	u.create_action("Add Entity");
	u.add_do_method(s, "add_child", n);
	u.add_do_property(n, "owner", get_editor_interface()->get_edited_scene_root());
	u.add_undo_method(s, "remove_child", n);
	u.commit_action();

	get_editor_interface()->get_selection()->clear();
	get_editor_interface()->get_selection()->add_node(n);
}

PropToolEditorPlugin::PropToolEditorPlugin(EditorNode *p_node) {
	EDITOR_DEF("editors/prop_tool/temp_path", "res://.prop_tool_temp/");
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::STRING, "editors/prop_tool/temp_path"));

	String tp = EditorSettings::get_singleton()->get("editors/prop_tool/temp_path");

	if (!tp.ends_with("/")) {
		tp += "/";
		EditorSettings::get_singleton()->set("editors/prop_tool/temp_path", tp);
	}

	if (!DirAccess::exists(tp)) {
		DirAccess *d = DirAccess::create_for_path(tp);

		Error err = d->make_dir_recursive(tp);

		memdelete(d);

		if (err)
			print_error("PropTool: Temporary directory creation failed: error code: " + String::num(err));
	}

	editor = p_node;

	light_button = memnew(ToolButton);
	light_button->set_text("Light");
	light_button->connect("pressed", this, "add_light");

	mesh_button = memnew(ToolButton);
	mesh_button->set_text("Mesh");
	mesh_button->connect("pressed", this, "add_mesh");

	prop_button = memnew(ToolButton);
	prop_button->set_text("Prop");
	prop_button->connect("pressed", this, "add_prop");

	scene_button = memnew(ToolButton);
	scene_button->set_text("Scene");
	scene_button->connect("pressed", this, "add_scene");

	entity_button = memnew(ToolButton);
	entity_button->set_text("Entity");
	entity_button->connect("pressed", this, "add_entity");

	add_control_to_container(EditorPlugin::CONTAINER_SPATIAL_EDITOR_MENU, light_button);
	add_control_to_container(EditorPlugin::CONTAINER_SPATIAL_EDITOR_MENU, mesh_button);
	add_control_to_container(EditorPlugin::CONTAINER_SPATIAL_EDITOR_MENU, prop_button);
	add_control_to_container(EditorPlugin::CONTAINER_SPATIAL_EDITOR_MENU, scene_button);
	add_control_to_container(EditorPlugin::CONTAINER_SPATIAL_EDITOR_MENU, entity_button);

	light_button->hide();
	mesh_button->hide();
	prop_button->hide();
	scene_button->hide();
	entity_button->hide();

	call_deferred("connect", "scene_changed", this, "on_scene_changed");
}

PropToolEditorPlugin::~PropToolEditorPlugin() {
	_edited_prop.unref();
}

void PropToolEditorPlugin::_notification(int p_what) {
	if (p_what == EditorSettings::NOTIFICATION_EDITOR_SETTINGS_CHANGED) {

		String tp = EditorSettings::get_singleton()->get("editors/prop_tool/temp_path");

		if (!tp.ends_with("/")) {
			tp += "/";
			EditorSettings::get_singleton()->set("editors/prop_tool/temp_path", tp);
		}

		if (!DirAccess::exists(tp)) {
			DirAccess *d = DirAccess::create_for_path(tp);

			Error err = d->make_dir_recursive(tp);

			memdelete(d);

			ERR_FAIL_COND_MSG(err, "PropTool: Temporary directory creation failed: error code: " + String::num(err));
		}
	}
}

void PropToolEditorPlugin::_bind_methods() {
	ClassDB::bind_method(D_METHOD("on_scene_changed", "scene_root"), &PropToolEditorPlugin::on_scene_changed);
	ClassDB::bind_method(D_METHOD("apply_changes"), &PropToolEditorPlugin::apply_changes);
	ClassDB::bind_method(D_METHOD("add_light"), &PropToolEditorPlugin::add_light);
	ClassDB::bind_method(D_METHOD("add_mesh"), &PropToolEditorPlugin::add_mesh);
	ClassDB::bind_method(D_METHOD("add_prop"), &PropToolEditorPlugin::add_prop);
	ClassDB::bind_method(D_METHOD("add_scene"), &PropToolEditorPlugin::add_scene);
	ClassDB::bind_method(D_METHOD("add_entity"), &PropToolEditorPlugin::add_entity);
}
