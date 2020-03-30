/*
Copyright (c) 2020 Péter Magyar

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

#include "prop_tool_entity.h"

#ifdef TOOLS_ENABLED

#include "../../entity_spell_system/entities/data/entity_data.h"
#include "../../entity_spell_system/entities/entity.h"

Ref<PropDataEntity> PropToolEntity::get_data() {
	if (!is_visible() || !_entity_data.is_valid())
		return Ref<PropDataEntity>();

	if (!_prop_entity.is_valid())
		_prop_entity.instance();

	_prop_entity->set_entity_data_id(_entity_data_id);
	_prop_entity->set_level(_level);

	return _prop_entity;
}
void PropToolEntity::set_data(const Ref<PropDataEntity> &data) {
	_prop_entity = data;

	if (!_prop_entity.is_valid())
		return;

	_entity_data_id = _prop_entity->get_entity_data_id();
	_level = _prop_entity->get_level();

	/*
	TODO generic solution
	var dir = Directory.new()
	if dir.open("res://data/entities/") == OK:
		dir.list_dir_begin()
		var file_name = dir.get_next()
		
		while (file_name != ""):
			if not dir.current_is_dir():
				var ed : EntityData = ResourceLoader.load("res://data/entities/" + file_name, "EntityData")
				
				if ed != null and ed.id == entity_data_id:
					set_entity_data(ed)
					return
				
			file_name = dir.get_next()
			
		print("PropToolEntity: Entity not found!")
		*/
}

Ref<EntityData> PropToolEntity::get_entity_data() {
	return _entity_data;
}
void PropToolEntity::set_entity_data(const Ref<EntityData> &data) {
	_entity_data = data;

	if (!_entity_data.is_valid())
		return;

	_entity_data_id = _entity_data->get_id();

	if (_entity == NULL) {
		//TODO generic solutiuon

		//var scene : PackedScene = load("res://addons/prop_tool/player/PropToolDisplayPlayer.tscn")

		//_entity = scene.instance() as Entity

		//  add_child(_entity)
		//_entity.owner = owner

		//_entity.get_node(_entity.character_skeleton_path).owner = owner
		//_entity.get_node(_entity.character_skeleton_path).refresh_in_editor = true
		//_entity.get_character_skeleton().refresh_in_editor = true

		// _entity.sentity_data = entity_data

		//name = entity_data.text_name
	}
}

int PropToolEntity::get_entity_data_id() const {
	return _entity_data_id;
}
void PropToolEntity::set_entity_data_id(const int value) {
	_entity_data_id = value;
}

int PropToolEntity::get_level() const {
	return _level;
}
void PropToolEntity::set_level(const int value) {
	_level = value;
}

bool PropToolEntity::evaluate_children() const {
	return false;
}

PropToolEntity::PropToolEntity() {
	_entity_data_id = 0;
	_level = 1;

	_entity = NULL;
}
PropToolEntity::~PropToolEntity() {
	_entity_data.unref();
	_prop_entity.unref();
}

void PropToolEntity::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_data"), &PropToolEntity::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "value"), &PropToolEntity::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "PropDataProp"), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("get_entity_data_id"), &PropToolEntity::get_entity_data_id);
	ClassDB::bind_method(D_METHOD("set_entity_data_id", "value"), &PropToolEntity::set_entity_data_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "entity_data_id"), "set_entity_data_id", "get_entity_data_id");

	ClassDB::bind_method(D_METHOD("get_level"), &PropToolEntity::get_level);
	ClassDB::bind_method(D_METHOD("set_level", "value"), &PropToolEntity::set_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "level"), "set_level", "get_level");

	ClassDB::bind_method(D_METHOD("get_entity_data"), &PropToolEntity::get_entity_data);
	ClassDB::bind_method(D_METHOD("set_entity_data", "value"), &PropToolEntity::set_entity_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "entity_data", PROPERTY_HINT_RESOURCE_TYPE, "EntityData"), "set_entity_data", "get_entity_data");

	ClassDB::bind_method(D_METHOD("evaluate_children"), &PropToolEntity::evaluate_children);
}

#endif