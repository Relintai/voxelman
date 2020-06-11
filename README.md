# Voxelman

A voxel engine module for godot, focusing more on editor integration, gameplay-related features, and extendability (even from gdscript), without sacrificing too much speed.

This is an engine module! Which means that you will need to compile it into Godot! [See the compiling section here.](#compiling)

## Optional Dependencies

`https://github.com/Relintai/texture_packer`: you get access to [VoxelmanLibraryMerger](#voxelmanlibrarymerger) \
`https://github.com/Relintai/mesh_data_resource`: you get access to a bunch of properties, and methods that can manipulate meshes.

Note: My props module also gets detected, but it won't do anything at the moment.

## Usage

First create a scene, and add a VoxelWorldBlocky node into it. Create a VoxelmanLibrary, and assign it to the Library property.
Also, add a VoxelSurface into your library.

(VoxelWorldBlocky is the only one that works properly for now, this will soon be fixed!)

Tick the editable property, deselect, then select the world again, and click the insert button at the top toolbar, or press B to insert a
voxel at the inspector's camera's location.

Select the add button, and now you can just add voxels with the mouse, by clicking on the newly added voxel.

## VoxelmanLibrary

This class stores the materials, and the VoxelSurfaces.

Note: If you want lods, assign equal (or more) materials than your maximum lod level. If you only want one material just assign it 
multiple times.

### VoxelmanLibrarySimple

The simplest library, just assign a material with a texture, and using the atlas_rows and atlas_culomns properties to tell the system
how the UVs should be divided.

### VoxelmanLibraryMerger

You will only have this if your godot also contains https://github.com/Relintai/texture_packer

You can assign any texture to your surfaces with this, and it will merge them together.

## Worlds

the 2 base classes:

VoxelWorld: Basic World, does not do anything until you implemnent the required virtual methods!\
VoxelWorldDefault: This adds threading, and LoD storage support to VoxelWorld. WIll not create meshes for you!

### VoxelWorldBlocky

The most basic world. It is the Minecraft-style world.

### VoxelWorldUVTransvoxel // VoxelVorldMarchingCubes

I just realized that I never got arount to adding this as a c++ class, will do very soon.
If you need it in the meantime it's here: https://github.com/Relintai/broken_seals/tree/master/game/voxelman/world
Actually except for the initializations and classes everything is c++ code.

It will be renamed VoxelVorldMarchingCubes, because I don't use the transvoxel algorithm's lod generation.

### VoxelWorldCubic

I never got around adding this as c++ classes aswell, will be done very soon.

This is my own meshing algorithm, it's basicly a Minecraft style mesher that can take isolevel into account.

### Level generation

Assign a VoxelManLevelGenerator to the Level Generator property.

You can write your own algorithm by implementing the ``` void _generate_chunk(chunk: VoxelChunk) virtual ``` method.

VoxelManLevelGeneratorFlat is also available, it will generate a floor for you, if you use it.

### Internal workings

#### VoxelWorld

Whenever you want to spawn a chunk your World will create it using the ``` VoxelChunk _create_chunk(x: int, y: int, z: int, chunk: VoxelChunk) virtual ``` method.

Since properly initializing a chunk usually takes quite a few steps that you probably don't want to repeat everywhere the `chunk`
parameter was added. This means you can just call the super `_create_chunk` methods, and you won't need to worry about your chunk
getting overridden. Like:

``` 
    func _create_chunk(x : int, y : int, z : int, chunk : VoxelChunk) -> VoxelChunk:
        if chunk == null:
	        chunk = MyChunk.new()

        #setup your chunk here

	    return ._create_chunk(x, y, z, chunk)
```

#### VoxelChunk

The most important method in VoxelChunk is the ``` void _create_meshers() virtual ```.

This is where you need to setup your meshers.

For example:

```
func _create_meshers():
	var mesher : MyMesher = MyMesher.new()
	add_mesher(mesher)
```

#### VoxelMesher

If you want to implement your own meshing algorithm you can do so by overriding ``` void _add_chunk(chunk: VoxelChunk) virtual ```.

VoxelMesher works similarly to SurfaceTool, so first you need to set colors, uvs, etc and then call add_vertex.
They won't get reset, so for exaple if you want all your vertices to have a certain color, you can get away with setting it only once.

## Compiling

First make sure that you can compile godot. See the official docs: https://docs.godotengine.org/en/3.2/development/compiling/index.html

1. Clone the engine if you haven't already:

```git clone https://github.com/godotengine/godot.git godot```

2. go into the modules folder inside the engine's directory"

```cd godot```
```cd modules```

3. clone this repository

```git clone https://github.com/Relintai/voxelman.git voxelman```

(the folder needs to be named voxelman!)

4. If you want the optional dependencies:

```git clone https://github.com/Relintai/texture_packer.git texture_packer```
```git clone https://github.com/Relintai/mesh_data_resource.git mesh_data_resource```

5. Go up one folder

```cd ..```

6. Compile godot.

For example:

```scons p=x11 t=release_debug tools=yes```
