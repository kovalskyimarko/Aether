#include "Aether/objLoader.hpp"
#include "Aether/math.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace Aether
{
    // Resolves 1-based positive indices and negative relative indices into 0-based absolute indices
    static inline int ResolveObjIndex(int index, size_t count)
    {
        if (index > 0)
        {
            return index - 1;
        }
        if (index < 0)
        {
            return static_cast<int>(count) + index;
        }
        return -1;
    }

    std::vector<float> ObjLoader::Load(const std::string& filepath, bool includeNormals)
    {
        std::vector<Vec3> temp_positions;
        std::vector<Vec3> temp_normals;
        std::vector<Vec2> temp_uvs;

        std::vector<int> vertexIndices;
        std::vector<int> uvIndices;
        std::vector<int> normalIndices;

        std::ifstream file(filepath);
        if (!file.is_open())
        {
            std::cerr << "ERROR::OBJLOADER: Could not open file " << filepath << std::endl;
            return {};
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#') continue;

            std::stringstream ss(line);
            std::string prefix;
            ss >> prefix;

            if (prefix == "v")
            {
                Vec3 pos;
                ss >> pos.x >> pos.y >> pos.z;
                temp_positions.push_back(pos);
            }
            else if (prefix == "vt")
            {
                Vec2 uv;
                ss >> uv.x >> uv.y;
                temp_uvs.push_back(uv);
            }
            else if (prefix == "vn")
            {
                Vec3 norm;
                ss >> norm.x >> norm.y >> norm.z;
                temp_normals.push_back(norm);
            }
            else if (prefix == "f")
            {
                std::string vertexData;
                for (int i = 0; i < 3; ++i)
                {
                    if (!(ss >> vertexData)) break;

                    int vIdx = -1;
                    int tIdx = -1;
                    int nIdx = -1;

                    size_t firstSlash = vertexData.find('/');
                    if (firstSlash == std::string::npos)
                    {
                        // Format: "v"
                        vIdx = std::stoi(vertexData);
                    }
                    else
                    {
                        // Format: "v/..."
                        vIdx = std::stoi(vertexData.substr(0, firstSlash));
                        size_t secondSlash = vertexData.find('/', firstSlash + 1);

                        if (secondSlash == std::string::npos)
                        {
                            // Format: "v/t"
                            tIdx = std::stoi(vertexData.substr(firstSlash + 1));
                        }
                        else
                        {
                            // Format: "v/t/n" or "v//n"
                            if (secondSlash > firstSlash + 1)
                            {
                                tIdx = std::stoi(vertexData.substr(firstSlash + 1, secondSlash - firstSlash - 1));
                            }
                            if (secondSlash + 1 < vertexData.size())
                            {
                                nIdx = std::stoi(vertexData.substr(secondSlash + 1));
                            }
                        }
                    }

                    vertexIndices.push_back(ResolveObjIndex(vIdx, temp_positions.size()));
                    uvIndices.push_back(tIdx != -1 ? ResolveObjIndex(tIdx, temp_uvs.size()) : -1);
                    normalIndices.push_back(nIdx != -1 ? ResolveObjIndex(nIdx, temp_normals.size()) : -1);
                }
            }
        }

        const size_t stride = includeNormals ? 8 : 5;
        std::vector<float> outData;
        outData.reserve(vertexIndices.size() * stride);

        for (size_t i = 0; i < vertexIndices.size(); ++i)
        {
            int vIndex = vertexIndices[i];
            if (vIndex < 0 || static_cast<size_t>(vIndex) >= temp_positions.size())
            {
                continue;
            }

            Vec3 pos = temp_positions[vIndex];
            outData.push_back(pos.x);
            outData.push_back(pos.y);
            outData.push_back(pos.z);

            if (includeNormals)
            {
                Vec3 norm = {0.0f, 1.0f, 0.0f}; // Default upward normal
                int nIndex = normalIndices[i];
                if (nIndex >= 0 && static_cast<size_t>(nIndex) < temp_normals.size())
                {
                    norm = temp_normals[nIndex];
                }
                outData.push_back(norm.x);
                outData.push_back(norm.y);
                outData.push_back(norm.z);
            }

            Vec2 uv = {0.0f, 0.0f};
            int tIndex = uvIndices[i];
            if (tIndex >= 0 && static_cast<size_t>(tIndex) < temp_uvs.size())
            {
                uv = temp_uvs[tIndex];
            }
            outData.push_back(uv.x);
            outData.push_back(uv.y);
        }

        return outData;
    }
}
