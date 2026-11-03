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
                struct FaceVertex
                {
                    int vIdx = -1;
                    int tIdx = -1;
                    int nIdx = -1;
                };

                std::vector<FaceVertex> faceVertices;
                std::string vertexData;

                while (ss >> vertexData)
                {
                    FaceVertex fv;

                    size_t firstSlash = vertexData.find('/');
                    if (firstSlash == std::string::npos)
                    {
                        // Format: "v"
                        fv.vIdx = std::stoi(vertexData);
                    }
                    else
                    {
                        // Format: "v/..."
                        fv.vIdx = std::stoi(vertexData.substr(0, firstSlash));
                        size_t secondSlash = vertexData.find('/', firstSlash + 1);

                        if (secondSlash == std::string::npos)
                        {
                            // Format: "v/t"
                            if (firstSlash + 1 < vertexData.size())
                            {
                                fv.tIdx = std::stoi(vertexData.substr(firstSlash + 1));
                            }
                        }
                        else
                        {
                            // Format: "v/t/n" or "v//n"
                            if (secondSlash > firstSlash + 1)
                            {
                                fv.tIdx = std::stoi(vertexData.substr(firstSlash + 1, secondSlash - firstSlash - 1));
                            }
                            if (secondSlash + 1 < vertexData.size())
                            {
                                fv.nIdx = std::stoi(vertexData.substr(secondSlash + 1));
                            }
                        }
                    }

                    fv.vIdx = ResolveObjIndex(fv.vIdx, temp_positions.size());
                    fv.tIdx = (fv.tIdx != -1) ? ResolveObjIndex(fv.tIdx, temp_uvs.size()) : -1;
                    fv.nIdx = (fv.nIdx != -1) ? ResolveObjIndex(fv.nIdx, temp_normals.size()) : -1;

                    faceVertices.push_back(fv);
                }

                // Triangulate polygon face using fan triangulation (0, i, i+1)
                for (size_t i = 1; i + 1 < faceVertices.size(); ++i)
                {
                    const FaceVertex tri[3] = { faceVertices[0], faceVertices[i], faceVertices[i + 1] };

                    bool validTriangle = true;
                    for (int k = 0; k < 3; ++k)
                    {
                        if (tri[k].vIdx < 0 || static_cast<size_t>(tri[k].vIdx) >= temp_positions.size())
                        {
                            validTriangle = false;
                            break;
                        }
                    }

                    if (!validTriangle) continue;

                    for (int k = 0; k < 3; ++k)
                    {
                        vertexIndices.push_back(tri[k].vIdx);
                        uvIndices.push_back(tri[k].tIdx);
                        normalIndices.push_back(tri[k].nIdx);
                    }
                }
            }
        }

        const size_t stride = includeNormals ? 8 : 5;
        std::vector<float> outData;
        outData.reserve(vertexIndices.size() * stride);

        for (size_t i = 0; i + 2 < vertexIndices.size(); i += 3)
        {
            bool validTri = true;
            for (size_t k = 0; k < 3; ++k)
            {
                int vIndex = vertexIndices[i + k];
                if (vIndex < 0 || static_cast<size_t>(vIndex) >= temp_positions.size())
                {
                    validTri = false;
                    break;
                }
            }
            if (!validTri) continue;

            for (size_t k = 0; k < 3; ++k)
            {
                size_t idx = i + k;
                Vec3 pos = temp_positions[vertexIndices[idx]];
                outData.push_back(pos.x);
                outData.push_back(pos.y);
                outData.push_back(pos.z);

                if (includeNormals)
                {
                    Vec3 norm = {0.0f, 1.0f, 0.0f}; // Default upward normal
                    int nIndex = normalIndices[idx];
                    if (nIndex >= 0 && static_cast<size_t>(nIndex) < temp_normals.size())
                    {
                        norm = temp_normals[nIndex];
                    }
                    outData.push_back(norm.x);
                    outData.push_back(norm.y);
                    outData.push_back(norm.z);
                }

                Vec2 uv = {0.0f, 0.0f};
                int tIndex = uvIndices[idx];
                if (tIndex >= 0 && static_cast<size_t>(tIndex) < temp_uvs.size())
                {
                    uv = temp_uvs[tIndex];
                }
                outData.push_back(uv.x);
                outData.push_back(uv.y);
            }
        }

        return outData;
    }
}
