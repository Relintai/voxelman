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
#include "prop_tool.h"
#include "scene/resources/packed_scene.h"

void PropToolEditorPlugin::edit(Object *p_object) {
	Ref<PropData> pedited_prop(p_object);

	String p = create_or_get_scene_path(pedited_prop);

	get_editor_interface()->open_scene_from_path(p);
}

bool PropToolEditorPlugin::handles(Object *p_object) const {

	bool handles = p_object->is_class("PropData");

	if (handles) {
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

	return handles;
}

void PropToolEditorPlugin::make_visible(bool p_visible) {
}

String PropToolEditorPlugin::create_or_get_scene_path(const Ref<PropData> &data) {

	String path = temp_path + data->get_path().get_file().get_basename() + ".tscn";

	if (!FileAccess::exists(path))
		create_scene(data);

	return path;
}
PropTool *PropToolEditorPlugin::create_or_get_scene(const Ref<PropData> &data) {
	ERR_FAIL_COND_V(!data.is_valid(), NULL);

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

	Error err = ResourceSaver::save(temp_path + data->get_path().get_file().get_basename() + ".tscn", ps);

	pt->queue_delete();
	return ps;
}

PropToolEditorPlugin::PropToolEditorPlugin(EditorNode *p_node) {
	temp_path = "res://addons/prop_tool/scenes/temp/";

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

	//connect("scene_changed", this, "scene_changed");
}

PropToolEditorPlugin::~PropToolEditorPlugin() {
	_edited_prop.unref();
}

/*

func scene_changed(scene):
	if scene.has_method("set_target_prop"):
		scene.plugin = self
		
		if not buttons_added:
			light_button.show()
			mesh_button.show()
			prop_button.show()
			scene_button.show()
			entity_button.show()
#			add_control_to_container(EditorPlugin.CONTAINER_SPATIAL_EDITOR_MENU, light_button)
#			add_control_to_container(EditorPlugin.CONTAINER_SPATIAL_EDITOR_MENU, mesh_button)
#			add_control_to_container(EditorPlugin.CONTAINER_SPATIAL_EDITOR_MENU, prop_button)
#			add_control_to_container(EditorPlugin.CONTAINER_SPATIAL_EDITOR_MENU, scene_button)
#			add_control_to_container(EditorPlugin.CONTAINER_SPATIAL_EDITOR_MENU, entity_button)
			
			buttons_added = true
	else:
		if buttons_added:
			light_button.hide()
			mesh_button.hide()
			prop_button.hide()
			scene_button.hide()
			entity_button.hide()
#
#			remove_control_from_container(EditorPlugin.CONTAINER_SPATIAL_EDITOR_MENU, light_button)
#			remove_control_from_container(EditorPlugin.CONTAINER_SPATIAL_EDITOR_MENU, mesh_button)
#			remove_control_from_container(EditorPlugin.CONTAINER_SPATIAL_EDITOR_MENU, prop_button)
#			remove_control_from_container(EditorPlugin.CONTAINER_SPATIAL_EDITOR_MENU, scene_button)
#			remove_control_from_container(EditorPlugin.CONTAINER_SPATIAL_EDITOR_MENU, entity_button)
			
			buttons_added = false

func apply_changes() -> void:
	var scene : Node = get_editor_interface().get_edited_scene_root()
	
	if scene is PropTool:
#	if scene.has_method("set_target_prop") and scene.has_method("save"):
		scene.save()



func add_light():
	var selection : Array = get_editor_interface().get_selection().get_selected_nodes()
	var selected : Node
	
	if selection.size() != 1:
		selected = get_editor_interface().get_edited_scene_root()
	else:
		selected = selection[0]
	
	var s : Node = selected
	var n = PropToolLight.new()
	
	var u : UndoRedo = get_undo_redo()
	u.create_action("Add Light")
	u.add_do_method(s, "add_child", n)
	u.add_do_property(n, "owner", get_editor_interface().get_edited_scene_root())
	u.add_undo_method(s, "remove_child", n)
	u.commit_action()
	
	get_editor_interface().get_selection().clear()
	get_editor_interface().get_selection().add_node(n)
	
	
func add_mesh():
	var selected : Array = get_editor_interface().get_selection().get_selected_nodes()
	
	if selected.size() != 1:
		return

	var s : Node = selected[0]
	var n = PropToolMesh.new()
	
	var u : UndoRedo = get_undo_redo()
	u.create_action("Add Mesh")
	u.add_do_method(s, "add_child", n)
	u.add_do_property(n, "owner", get_editor_interface().get_edited_scene_root())
	u.add_undo_method(s, "remove_child", n)
	u.commit_action()
	
	get_editor_interface().get_selection().clear()
	get_editor_interface().get_selection().add_node(n)

	
func add_prop():
	var selected : Array = get_editor_interface().get_selection().get_selected_nodes()
	
	if selected.size() != 1:
		return
	
	var s : Node = selected[0]
	var n = PropTool.new()
	
	var u : UndoRedo = get_undo_redo()
	u.create_action("Add Prop")
	u.add_do_method(s, "add_child", n)
	u.add_do_property(n, "owner", get_editor_interface().get_edited_scene_root())
	u.add_undo_method(s, "remove_child", n)
	u.commit_action()
	
	get_editor_interface().get_selection().clear()
	get_editor_interface().get_selection().add_node(n)
	
func add_scene():
	var selected : Array = get_editor_interface().get_selection().get_selected_nodes()
	
	if selected.size() != 1:
		return
	
	var s : Node = selected[0]
	var n = PropToolScene.new()
	
	var u : UndoRedo = get_undo_redo()
	u.create_action("Add Scene")
	u.add_do_method(s, "add_child", n)
	u.add_do_property(n, "owner", get_editor_interface().get_edited_scene_root())
	u.add_undo_method(s, "remove_child", n)
	u.commit_action()
	
	get_editor_interface().get_selection().clear()
	get_editor_interface().get_selection().add_node(n)
	
func add_entity():
	var selected : Array = get_editor_interface().get_selection().get_selected_nodes()
	
	if selected.size() != 1:
		return
	
	var s : Node = selected[0]
	var n = PropToolEntity.new()
	
	var u : UndoRedo = get_undo_redo()
	u.create_action("Add Entity")
	u.add_do_method(s, "add_child", n)
	u.add_do_property(n, "owner", get_editor_interface().get_edited_scene_root())
	u.add_undo_method(s, "remove_child", n)
	u.commit_action()
	
	get_editor_interface().get_selection().clear()
	get_editor_interface().get_selection().add_node(n)





*/