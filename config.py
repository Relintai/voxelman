

def can_build(env, platform):
  return True


def configure(env):
	pass


def get_doc_classes():
    return [
        "WorldArea",

        "GroundClutterFoliage",
        "GroundClutter",

        "VoxelLight",

        "VoxelmanLevelGenerator",

        "VoxelSurfaceMerger",
        "VoxelSurfaceSimple",
        "VoxelSurface",
        "VoxelmanLibraryMerger",
        "VoxelmanLibrarySimple",
        "VoxelmanLibrary",

        "VoxelCubePoints",
        "VoxelMesherCubic",
        "TransvoxelCellData",
        "VoxelMeshData",
        "VoxelMesherTransvoxel",
        "VoxelMesher",

        "PropDataEntity",
        "PropDataEntry",
        "PropDataLight",
        "PropDataMesh",
        "PropDataProp",
        "PropDataScene",
        "PropData",

        "EnvironmentData",
        "VoxelChunkPropData",
        "VoxelChunk",
        "VoxelChunkDefault",
        "VoxelStructure",
        "BlockVoxelStructure",
        "VoxelWorld",

        "PropTool",
        "PropToolEntity",
        "PropToolLight",
        "PropToolMesh",
        "PropToolProp",
        "PropToolScene",
    ]

def get_doc_path():
    return "doc_classes"

