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

#ifndef WORLD_AREA_H
#define WORLD_AREA_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/object/ref_counted.h"
#ifndef Reference
#define Reference RefCounted
#endif
#include "core/string/ustring.h"
#else
#include "core/reference.h"
#include "core/ustring.h"
#endif

#include "core/math/aabb.h"
#include "scene/resources/texture.h"

class WorldArea : public Reference {
	GDCLASS(WorldArea, Reference);

public:
	AABB get_aabb() const;
	void set_aabb(const AABB value);

	Ref<Texture> get_map_texture() const;
	void set_map_texture(const Ref<Texture> &value);

	Ref<Texture> get_fov_texture() const;
	void set_fov_texture(const Ref<Texture> &value);

	String get_name() const;
	void set_name(const String &value);

	int get_level() const;
	void set_level(const int value);

	WorldArea();
	~WorldArea();

private:
	static void _bind_methods();

private:
	AABB _aabb;
	Ref<Texture> _map_texture;
	Ref<Texture> _fov_texture;
	String _name;
	int _level;
};

#endif
