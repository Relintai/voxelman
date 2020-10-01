

def can_build(env, platform):
    return True


def configure(env):
    pass


def get_doc_classes():
    return [
        "WorldArea",

        "VoxelLight",
        "VoxelmanLight",

        "VoxelmanLevelGenerator",
        "VoxelmanLevelGeneratorFlat",

        "VoxelSurfaceMerger",
        "VoxelSurfaceSimple",
        "VoxelSurface",
        "VoxelmanLibraryMerger",
        "VoxelmanLibrarySimple",
        "VoxelmanLibrary",

        "VoxelCubePoints",
        "VoxelMesherCubic",
        "VoxelMeshData",

        "MarchingCubesCellData",
        "VoxelMesherMarchingCubes",

        "VoxelMesher",

        "EnvironmentData",
        "VoxelChunk",
        "VoxelChunkDefault",
        "VoxelStructure",
        "BlockVoxelStructure",
        "VoxelWorld",

        "VoxelMesherBlocky",
        "VoxelWorldBlocky",
        "VoxelChunkBlocky",
        "VoxelMesherLiquidBlocky",

        "VoxelWorldMarchingCubes",
        "VoxelChunkMarchingCubes",

        "VoxelMesherCubic",
        "VoxelWorldCubic",
        "VoxelChunkCubic",

        "VoxelMesherDefault",
        "VoxelWorldDefault",

        "VoxelJob",
        "VoxelTerrarinJob",
        "VoxelLightJob",
    ]


def get_doc_path():
    return "doc_classes"
