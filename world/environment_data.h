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

#ifndef ENVIRONMENT_DATA_H
#define ENVIRONMENT_DATA_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/io/resource.h"
#include "core/math/color.h"
#else
#include "core/resource.h"
#include "core/color.h"
#endif

#include "../defines.h"

#include light_h

#include "scene/3d/world_environment.h"
#include "scene/main/node.h"

class EnvironmentData : public Resource {
	GDCLASS(EnvironmentData, Resource);

public:
	Ref<Environment> get_environment();
	void set_environment(const Ref<Environment> &value);

	Color get_color(const int index);
	void set_color(const int index, const Color &value);
	float get_energy(const int index);
	void set_energy(const int index, const float value);
	float get_indirect_energy(const int index);
	void set_indirect_energy(const int index, const float value);

	void setup(WorldEnvironment *world_environment, DirectionalLight *primary_light, DirectionalLight *secondary_light);
	void setup_bind(Node *world_environment, Node *primary_light, Node *secondary_light);

#if VERSION_MAJOR >= 4
	GDVIRTUAL3(_setup, WorldEnvironment*, DirectionalLight*, DirectionalLight*);
#endif

	EnvironmentData();
	~EnvironmentData();

public:
	enum {
		LIGHT_COUNT = 2,
	};

protected:
	static void _bind_methods();

private:
	Ref<Environment> _environment;

	Color _colors[LIGHT_COUNT];
	float _energies[LIGHT_COUNT];
	float _indirect_energies[LIGHT_COUNT];
};

#endif
