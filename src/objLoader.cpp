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
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.size() < 2 || line[0] == '#') continue;

            const char* p = line.c_str();
            while (*p == ' ' || *p == '\t') ++p;

            if (p[0] == 'v' && p[1] == ' ')
            {
                p += 2;
                char* end = nullptr;
                float x = std::strtof(p, &end);
                float y = std::strtof(end, &end);
                float z = std::strtof(end, nullptr);
                temp_positions.push_back({x, y, z});
            }
            else if (p[0] == 'v' && p[1] == 't' && (p[2] == ' ' || p[2] == '\t'))
            {
                p += 3;
                char* end = nullptr;
                float u = std::strtof(p, &end);
                float v = std::strtof(end, nullptr);
                temp_uvs.push_back({u, v});
            }
            else if (p[0] == 'v' && p[1] == 'n' && (p[2] == ' ' || p[2] == '\t'))
            {
                p += 3;
                char* end = nullptr;
                float nx = std::strtof(p, &end);
                float ny = std::strtof(end, &end);
                float nz = std::strtof(end, nullptr);
                temp_normals.push_back({nx, ny, nz});
            }
            else if (p[0] == 'f' && (p[1] == ' ' || p[1] == '\t'))
            {
                p += 2;
                struct FaceVertex
                {
                    int vIdx = -1;
                    int tIdx = -1;
                    int nIdx = -1;
                };

                static thread_local std::vector<FaceVertex> faceVertices;
                faceVertices.clear();

                while (*p)
                {
                    while (*p == ' ' || *p == '\t') ++p;
                    if (!*p) break;

                    char* end = nullptr;
                    long v = std::strtol(p, &end, 10);
                    if (end == p) break;
                    p = end;

                    long t = -1, n = -1;
                    if (*p == '/')
                    {
                        ++p;
                        if (*p != '/')
                        {
                            t = std::strtol(p, &end, 10);
                            p = end;
                        }
                        if (*p == '/')
                        {
                            ++p;
                            n = std::strtol(p, &end, 10);
                            p = end;
                        }
                    }

                    FaceVertex fv;
                    fv.vIdx = ResolveObjIndex(static_cast<int>(v), temp_positions.size());
                    fv.tIdx = (t != -1) ? ResolveObjIndex(static_cast<int>(t), temp_uvs.size()) : -1;
                    fv.nIdx = (n != -1) ? ResolveObjIndex(static_cast<int>(n), temp_normals.size()) : -1;
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
