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

#ifndef PROP_TOOL_ENTITY_H
#define PROP_TOOL_ENTITY_H

#include "scene/3d/spatial.h"

#include "../props/prop_data.h"
#include "../props/prop_data_entity.h"

class EntityData;
class Entity;

class PropToolEntity : public Spatial {
	GDCLASS(PropToolEntity, Spatial);

#ifdef TOOLS_ENABLED
public:
	Ref<PropDataEntity> get_data();
	void set_data(const Ref<PropDataEntity> &data);

	Ref<EntityData> get_entity_data();
	void set_entity_data(const Ref<EntityData> &data);

	int get_entity_data_id() const;
	void set_entity_data_id(const int value);

	int get_level() const;
	void set_level(const int value);

	bool evaluate_children() const;

	PropToolEntity();
	~PropToolEntity();

protected:
	static void _bind_methods();

private:
	Ref<EntityData> _entity_data;
	int _entity_data_id;
	int _level;

	Ref<PropDataEntity> _prop_entity;
	Entity *_entity;
#endif
};

#endif
