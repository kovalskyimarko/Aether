#pragma once

#include <string>
#include <vector>

namespace Aether
{
    class ObjLoader
    {
    public:
        // Reads an .obj file and returns interleaved vertex data:
        // If hasNormals is true (default): [pos.xyz, norm.xyz, uv.xy] (8 floats per vertex)
        // If hasNormals is false: [pos.xyz, uv.xy] (5 floats per vertex)
        static std::vector<float> Load(const std::string& filepath, bool includeNormals = true);
    };
}