#include "AssetImporter.h"
#include <fstream>
#include <stdio.h>
#include "Entity.h"
#include "Logic.h"
#include "MultiLighting.h"

namespace Meshes {

    // OBJ loader algorithms

    float Magnitude(const glm::vec3 in) {
        return (sqrtf(powf(in.x, 2) + powf(in.y, 2) + powf(in.z, 2)));
    }

    float Dot(const glm::vec3 a, const glm::vec3 b) {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    }

    glm::vec3 Vec3Multiply(const glm::vec3 vec, float value) {
        return glm::vec3(vec.x * value, vec.y * value, vec.z * value);
    }

    glm::vec3 Vec3Divide(const glm::vec3 vec, float value) {
        return glm::vec3(vec.x / value, vec.y / value, vec.z / value);
    }

    glm::vec3 Proj(const glm::vec3 a, const glm::vec3 b) {
        glm::vec3 bn = Vec3Divide(b, Magnitude(b));
        return Vec3Multiply(bn, Dot(a, bn));
    }

    glm::vec3 Cross(const glm::vec3 a, const glm::vec3 b) {
        return glm::vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    }

    glm::vec3 GenTriNormal(glm::vec3 t1, glm::vec3 t2, glm::vec3 t3) {
        glm::vec3 u = t2 - t1;
        glm::vec3 v = t3 - t1;

        glm::vec3 normal = Cross(u, v);

        return normal;
    }

    float AngleBetween(const glm::vec3 a, const glm::vec3 b) {
        float angle = Dot(a, b);
        angle /= (Magnitude(a) * Magnitude(b));
        return angle = acosf(angle);
    }

    bool SameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b) {
        glm::vec3 cp1 = Cross(b - a, p1 - a);
        glm::vec3 cp2 = Cross(b - a, p2 - a);

        if (Dot(cp1, cp2) >= 0) {
            return true;
        } else {
            return false;
        }
    }

    bool InTriangle(glm::vec3 point, glm::vec3 tri1, glm::vec3 tri2, glm::vec3 tri3) {
        bool withinTriPrism = SameSide(point, tri1, tri2, tri3) && SameSide(point, tri2, tri1, tri3) && SameSide(point, tri3, tri1, tri2);

        if (withinTriPrism)
            return false;

        glm::vec3 n = GenTriNormal(tri1, tri2, tri3);

        glm::vec3 proj = Proj(point, n);

        if (Magnitude(proj) == 0) {
            return true;
        } else {
            return false;
        }
    }

    inline void Split(const std::string& in, std::vector<std::string>& out, std::string token) {
        out.clear();

        std::string temp;

        for (int i = 0; i < int(in.size()); i++) {
            std::string test = in.substr(i, token.size());

            if (test == token) {
                if (!temp.empty()) {
                    out.push_back(temp);
                    temp.clear();
                    i += (int)token.size() - 1;
                } else {
                    out.push_back("");
                }
            } else if (i + token.size() >= in.size()) {
                temp += in.substr(i, token.size());
                out.push_back(temp);
                break;
            } else {
                temp += in[i];
            }
        }
    }

    inline std::string Tail(const std::string& in) {
        size_t tokenStart = in.find_first_not_of(" \t");
        size_t spaceStart = in.find_first_of(" \t", tokenStart);
        size_t tailStart = in.find_first_not_of(" \t", spaceStart);
        size_t tailEnd = in.find_last_not_of(" \t");

        if (tailStart != std::string::npos && tailEnd != std::string::npos) {
            return in.substr(tailStart, tailEnd - tailStart + 1);
        } else if (tailStart != std::string::npos) {
            return in.substr(tailStart);
        }

        return "";
    }

    inline std::string firstToken(const std::string& in) {
        if (!in.empty()) {
            size_t tokenStart = in.find_first_not_of(" \t");
            size_t tokenEnd = in.find_first_of(" \t", tokenStart);

            if (tokenStart != std::string::npos && tokenEnd != std::string::npos) {
                return in.substr(tokenStart, tokenEnd - tokenStart);
            } else if (tokenStart != std::string::npos) {
                return in.substr(tokenStart);
            }
        }

        return "";
    }

    template<class T>
    inline const T& getElement(const std::vector<T>& elements, std::string& index) {
        int idx = std::stoi(index);

        if (idx < 0) {
            idx = int(elements.size()) + idx;
        } else {
            idx--;
        }

        return elements[idx];
    }

    void VertexTriangulation(std::vector<unsigned int>& oIndices, const std::vector<Vertex>& iVerts) {
        // Check tri can be created
        if (iVerts.size() < 3) {
            return;
        }
        // Check if existing tri
        if (iVerts.size() == 3) {
            oIndices.push_back(0);
            oIndices.push_back(1);
            oIndices.push_back(2);
            return;
        }

        std::vector<Vertex> tVerts = iVerts;

        while (true) {
            for (int i = 0; i < int(tVerts.size()); i++) {
                Vertex pPrev;

                if (i == 0) {
                    pPrev = tVerts[tVerts.size() - 1];
                } else {
                    pPrev = tVerts[i - 1];
                }

                Vertex pCur = tVerts[i];

                Vertex pNext;

                if (i == tVerts.size() - 1) {
                    pNext = tVerts[0];
                } else {
                    pNext = tVerts[i + 1];
                }

                if (tVerts.size() == 3) {
                    for (int j = 0; j < int(tVerts.size()); j++) {
                        if (iVerts[j].position == pCur.position)
                            oIndices.push_back(j);
                        if (iVerts[j].position == pPrev.position)
                            oIndices.push_back(j);
                        if (iVerts[j].position == pNext.position)
                            oIndices.push_back(j);
                    }

                    tVerts.clear();
                    break;
                }

                if (tVerts.size() == 4) {
                    for (int j = 0; j < int(iVerts.size()); j++) {
                        if (iVerts[j].position == pCur.position)
                            oIndices.push_back(j);
                        if (iVerts[j].position == pPrev.position)
                            oIndices.push_back(j);
                        if (iVerts[j].position == pNext.position)
                            oIndices.push_back(j);
                    }

                    glm::vec3 tempVec;
                    for (int j = 0; j < int(tVerts.size()); j++) {
                        if (tVerts[j].position != pCur.position && tVerts[j].position != pPrev.position && tVerts[j].position != pNext.position) {
                            tempVec = tVerts[j].position;
                            break;
                        }
                    }

                    for (int j = 0; j < int(iVerts.size()); j++) {
                        if (iVerts[j].position == pPrev.position)
                            oIndices.push_back(j);
                        if (iVerts[j].position == pNext.position)
                            oIndices.push_back(j);
                        if (iVerts[j].position == tempVec)
                            oIndices.push_back(j);
                    }

                    tVerts.clear();
                    break;
                }

                float angle = AngleBetween(pPrev.position - pCur.position, pNext.position - pCur.position) * (180.0f / 3.13159265359f);
                if (angle <= 0 && angle >= 180)
                    continue;

                bool inTri = false;

                for (int j = 0; j < int(iVerts.size()); j++) {
                    if (InTriangle(iVerts[j].position, pPrev.position, pCur.position, pNext.position) && iVerts[j].position != pPrev.position && iVerts[j].position != pCur.position && iVerts[i].position != pNext.position) {
                        inTri = true;
                        break;
                    }
                }

                if (inTri)
                    continue;

                for (int j = 0; j < int(iVerts.size()); j++) {
                    if (iVerts[j].position == pCur.position)
                        oIndices.push_back(j);
                    if (iVerts[j].position == pPrev.position)
                        oIndices.push_back(j);
                    if (iVerts[j].position == pNext.position)
                        oIndices.push_back(j);
                }

                for (int j = 0; j < int(tVerts.size()); j++) {
                    if (tVerts[j].position == pCur.position) {
                        tVerts.erase(tVerts.begin() + j);
                        break;
                    }
                }

                i = -1;
            }

            if (oIndices.size() == 0)
                break;

            if (tVerts.size() == 0)
                break;
        }
    }

    void GenVerticesFromRawOBJ(std::vector<Vertex>& oVerts, const std::vector<glm::vec3>& iPos, const std::vector<glm::vec2>& iTCoords, const std::vector<glm::vec3>& iNor, std::string iCurrLine) {
        std::vector<std::string> sFace, sVert;
        Vertex vVert;
        Split(Tail(iCurrLine), sFace, " ");

        bool noNormal = false;

        for (int i = 0; i < int(sFace.size()); i++) {
            int vType = 0;

            Split(sFace[i], sVert, "/");

            // Check for just position
            if (sVert.size() == 1) {
                vType = 1;
            }

            // Check for pos and texture
            if (sVert.size() == 2) {
                vType = 2;
            }

            // Check for pos, tex, and normals
            if (sVert.size() == 3) {
                if (sVert[1] != "") {
                    // Pos, tex, and normal
                    vType = 4;
                } else {
                    // Pos, normal
                    vType = 3;
                }
            }

            switch (vType) {
            case 1:
                vVert.position = getElement(iPos, sVert[0]);
                vVert.texcoords = glm::vec2(0, 0);
                noNormal = true;
                oVerts.push_back(vVert);
                break;
            case 2:
                vVert.position = getElement(iPos, sVert[0]);
                vVert.texcoords = getElement(iTCoords, sVert[1]);
                noNormal = true;
                oVerts.push_back(vVert);
                break;
            case 3:
                vVert.position = getElement(iPos, sVert[0]);
                vVert.texcoords = glm::vec2(0, 0);
                vVert.normal = getElement(iNor, sVert[2]);
                oVerts.push_back(vVert);
                break;
            case 4:
                vVert.position = getElement(iPos, sVert[0]);
                vVert.texcoords = getElement(iTCoords, sVert[1]);
                vVert.normal = getElement(iNor, sVert[2]);
                oVerts.push_back(vVert);
                break;
            default:
                break;
            }
        }

        if (noNormal) {
            glm::vec3 a = oVerts[0].position - oVerts[1].position;
            glm::vec3 b = oVerts[2].position - oVerts[1].position;

            glm::vec3 normal = Cross(a, b);

            for (int i = 0; i < oVerts.size(); i++) {
                oVerts[i].normal = normal;
            }
        }
    }

    // Importer

    AssetImporter::AssetImporter() {

    }

    AssetImporter::~AssetImporter() {
        loadedMeshes.clear();
    }

    bool AssetImporter::ImportObj(std::string path) {
        if (path.substr(path.size() - 4, 4) != ".obj")
            return false;

        std::ifstream file(path);

        if (!file.is_open())
            return false;

        loadedMeshes.clear();
        loadedVertices.clear();
        loadedIndices.clear();

        std::vector<glm::vec3> positions, normals;
        std::vector<glm::vec2> texcoords;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        std::vector<std::string> materialNames;

        bool listening = false;
        std::string meshname, currLine;

        Mesh* tempMesh;

        while (std::getline(file, currLine)) {
            if (firstToken(currLine) == "o" || firstToken(currLine) == "g" || currLine[0] == 'g') {
                if (!listening) {
                    if (firstToken(currLine) == "o" || firstToken(currLine) == "g") {
                        meshname = Tail(currLine);
                    } else {
                        meshname = "unnamed";
                    }
                } else {
                    if (!indices.empty() && !vertices.empty()) {
                        tempMesh = new Mesh(vertices, indices);
                        tempMesh->meshName = meshname;

                        loadedMeshes.push_back(tempMesh);

                        vertices.clear();
                        indices.clear();
                        meshname.clear();

                        meshname = Tail(currLine);
                    } else {
                        if (firstToken(currLine) == "o" || firstToken(currLine) == "g") {
                            meshname = Tail(currLine);
                        } else {
                            meshname = "unnamed";
                        }
                    }
                }
            }

            if (firstToken(currLine) == "v") {
                std::vector<std::string> spos;
                glm::vec3 vpos;
                Split(Tail(currLine), spos, " ");

                vpos.x = std::stof(spos[0]);
                vpos.y = std::stof(spos[1]);
                vpos.z = std::stof(spos[2]);

                positions.push_back(vpos);
            }

            if (firstToken(currLine) == "vt") {
                std::vector<std::string> stex;
                glm::vec2 vtex;
                Split(Tail(currLine), stex, " ");

                vtex.x = std::stof(stex[0]);
                vtex.y = std::stof(stex[1]);

                texcoords.push_back(vtex);
            }

            if (firstToken(currLine) == "vn") {
                std::vector<std::string> snor;
                glm::vec3 vnor;
                Split(Tail(currLine), snor, " ");

                vnor.x = std::stof(snor[0]);
                vnor.y = std::stof(snor[1]);
                vnor.z = std::stof(snor[2]);

                normals.push_back(vnor);
            }

            if (firstToken(currLine) == "f") {
                std::vector<Vertex> vVert;
                GenVerticesFromRawOBJ(vVert, positions, texcoords, normals, currLine);

                for (int i = 0; i < vVert.size(); i++) {
                    vertices.push_back(vVert[i]);
                    loadedVertices.push_back(vVert[i]);
                }

                std::vector<unsigned int> iIndices;

                VertexTriangulation(iIndices, vVert);

                for (int i = 0; i < int(iIndices.size()); i++) {
                    unsigned int indum = (unsigned int)((vertices.size()) - vVert.size()) + iIndices[i];
                    indices.push_back(indum);

                    indum = (unsigned int)((loadedVertices.size()) - vVert.size()) + iIndices[i];
                    loadedIndices.push_back(indum);
                }
            }

            if (firstToken(currLine) == "usemtl") {
                materialNames.push_back(Tail(currLine));

                if (!indices.empty() && !vertices.empty()) {
                    tempMesh = new Mesh(vertices, indices);
                    tempMesh->meshName = meshname;

                    int i = 2;

                    while (1) {
                        tempMesh->meshName = meshname + "_" + std::to_string(i);

                        for (auto m : loadedMeshes) {
                            if (m->meshName == tempMesh->meshName)
                                continue;
                        }

                        break;
                    }

                    loadedMeshes.push_back(tempMesh);

                    vertices.clear();
                    indices.clear();
                }
            }

            if (firstToken(currLine) == "mtllib") {
                std::vector<std::string> temp;
                Split(path, temp, "/");

                std::string pathToMat = "";

                if (temp.size() != 1) {
                    for (int i = 0; i < int(temp.size()) - 1; i++) {
                        pathToMat += temp[i] + "/";
                    }
                }

                pathToMat += Tail(currLine);

                LoadMaterials(pathToMat);
            }
        }

        if (!indices.empty() && !vertices.empty()) {
            tempMesh = new Mesh(vertices, indices);
            tempMesh->meshName = meshname;

            loadedMeshes.push_back(tempMesh);
        }

        file.close();

        for (int i = 0; i < int(materialNames.size()); i++) {
            std::string matName = materialNames[i];

            for (int j = 0; j < int(loadedMaterials.size()); j++) {
                if (loadedMaterials[j].name == matName) {
                    loadedMeshes[j]->meshMaterial = loadedMaterials[j];
                    break;
                }
            }
        }

        if (loadedMeshes.empty() && loadedVertices.empty() && loadedIndices.empty()) {
            return false;
        } else {
            loadedMeshesCount++;
            return true;
        }
    }

    bool AssetImporter::LoadMaterials(std::string path) {
        if (path.substr(path.size() - 4, path.size()) != ".mtl")
            return false;

        std::ifstream file(path);

        if (!file.is_open())
            return false;

        Shaders::Material tempMaterial;

        bool listening = false;

        std::string currLine;

        while (std::getline(file, currLine)) {
            if (firstToken(currLine) == "newmtl") {
                if (!listening) {
                    listening = true;

                    if (currLine.size() > 7) {
                        tempMaterial.name = Tail(currLine);
                    } else {
                        tempMaterial.name = "none";
                    }
                } else {
                    loadedMaterials.push_back(tempMaterial);

                    tempMaterial = Shaders::Material();

                    if (currLine.size() > 7) {
                        tempMaterial.name = Tail(currLine);
                    } else {
                        tempMaterial.name = "none";
                    }
                }
            }
            // Ambient color
            if (firstToken(currLine) == "Ka") {
                std::vector<std::string> temp;
                Split(Tail(currLine), temp, " ");

                if (temp.size() != 3)
                    continue;

                tempMaterial.ambientColor.x = std::stof(temp[0]);
                tempMaterial.ambientColor.y = std::stof(temp[1]);
                tempMaterial.ambientColor.z = std::stof(temp[2]);
            }
            // Diffuse color
            if (firstToken(currLine) == "Kd") {
                std::vector<std::string> temp;
                Split(Tail(currLine), temp, " ");

                if (temp.size() != 3)
                    continue;

                tempMaterial.diffuseColor.x = std::stof(temp[0]);
                tempMaterial.diffuseColor.y = std::stof(temp[1]);
                tempMaterial.diffuseColor.z = std::stof(temp[2]);
            }
            // Specular color
            if (firstToken(currLine) == "Ks") {
                std::vector<std::string> temp;
                Split(Tail(currLine), temp, " ");

                if (temp.size() != 3)
                    continue;

                tempMaterial.specularColor.x = std::stof(temp[0]);
                tempMaterial.specularColor.y = std::stof(temp[1]);
                tempMaterial.specularColor.z = std::stof(temp[2]);
            }
            // Specular Exponent
            if (firstToken(currLine) == "Ns") {
                tempMaterial.specularExponent = std::stof(Tail(currLine));
            }
            // Optical Density
            if (firstToken(currLine) == "Ni") {
                tempMaterial.opticalDensity = std::stof(Tail(currLine));
            }
            // Dissolve
            if (firstToken(currLine) == "d") {
                tempMaterial.dissolve = std::stof(Tail(currLine));
            }
            // Illumination
            if (firstToken(currLine) == "illum") {
                tempMaterial.illumination = std::stof(Tail(currLine));
            }
            // Ambient Tex
            if (firstToken(currLine) == "map_Ka") {
                tempMaterial.ambientTexture = std::stof(Tail(currLine));
            }
            // Diffuse Tex
            if (firstToken(currLine) == "map_Kd") {
                tempMaterial.diffuseTexture = std::stof(Tail(currLine));
            }
            // Specular Tex
            if (firstToken(currLine) == "map_Ks") {
                tempMaterial.specularTexture = std::stof(Tail(currLine));
            }
            // Specular Highlight Tex
            if (firstToken(currLine) == "map_Ns") {
                tempMaterial.specularHighlight = std::stof(Tail(currLine));
            }
            // Alpha Tex
            if (firstToken(currLine) == "map_d") {
                tempMaterial.alphaMask = std::stof(Tail(currLine));
            }
            // Bump Map
            if (firstToken(currLine) == "map_Bump" || firstToken(currLine) == "map_bump" || firstToken(currLine) == "bump") {
                tempMaterial.bumpMap = std::stof(Tail(currLine));
            }
        }

        loadedMaterials.push_back(tempMaterial);

        if (loadedMaterials.empty()) {
            return false;
        } else {
            return true;
        }
    }

    std::shared_ptr<Shaders::MultiLighting> AssetImporter::LoadShader(std::string path) {
        std::unordered_map<std::string, std::shared_ptr<Shaders::MultiLighting>>::iterator a = loadedShaders.find(path);

        if (a == loadedShaders.end()) {
            a = loadedShaders.emplace(path, new Shaders::MultiLighting(path)).first;
        }

        return (*a).second;
    }

    // Initalisation list

    std::string GetTag(std::string value) {
        if (!value.empty()) {
            size_t tagStart = value.find_first_of("<");
            size_t tagEnd = value.find_first_of(" ", tagStart);

            if (tagStart != std::string::npos && tagEnd != std::string::npos) {
                return value.substr(tagStart, tagEnd);
            } else {
                return "";
            }
        } else {
            return "";
        }
    }

    std::string GetAttribute(std::string value, std::string attributeName, int stringLength) {
        if (!value.empty()) {
            size_t attribStart = value.find_first_of(attributeName) + stringLength + 2;
            size_t attribEnd = value.find_first_of("\" ", attribStart);

            if (attribStart != std::string::npos && attribEnd != std::string::npos) {
                return value.substr(attribStart, attribEnd - attribStart);
            } else {
                return "";
            }
        } else {
            return "";
        }
    }

    bool AssetImporter::LoadInitList(std::string path, EngineSystems::Logic& logic) {
        if (path.substr(path.size() - 4, 4) != ".xml")
            return false;

        std::ifstream file(path);

        if (!file.is_open())
            return false;

        std::vector<GameObjectNode*> gameObjectNodes;
        
        GameObjectNode* tempNode = nullptr;
        std::string currLine;
        bool isListening = false;

        while (std::getline(file, currLine)) {
            if (!isListening) {
                // Gets GameObject tag to fill a node
                if (currLine == "<GameObject>") {
                    tempNode = new GameObjectNode();
                    gameObjectNodes.push_back(tempNode);
                    isListening = true;
                }
            } else {
                // Fills node
                std::string tag = GetTag(currLine);

                if (currLine == "</GameObject>") {
                    // End of gameobject found
                    isListening = false;
                    continue;
                } else {
                    if (tag == "<Position ") {
                        std::string a = GetAttribute(currLine, "x", 1);
                        tempNode->transform.position.x = std::stof(GetAttribute(currLine, "x", 1));
                        tempNode->transform.position.y = std::stof(GetAttribute(currLine, "y", 1));
                        tempNode->transform.position.z = std::stof(GetAttribute(currLine, "z", 1));
                    } else if (tag == "<Rotation ") {
                        tempNode->transform.Rotate(
                            std::stof(GetAttribute(currLine, "x", 1)),
                            std::stof(GetAttribute(currLine, "y", 1)),
                            std::stof(GetAttribute(currLine, "z", 1))
                        );
                    } else if (tag == "<Scale ") {
                        tempNode->transform.scale.x = std::stof(GetAttribute(currLine, "x", 1));
                        tempNode->transform.scale.y = std::stof(GetAttribute(currLine, "y", 1));
                        tempNode->transform.scale.z = std::stof(GetAttribute(currLine, "z", 1));
                    } else if (tag == "<Mesh ") {
                        // String length needs to be 2 longer for whatever reason
                        tempNode->meshPath = GetAttribute(currLine, "path", 6);
                    } else if (tag == "<Shader ") {
                        // String length needs to be 4 longer for whatever reason
                        tempNode->shaderPath = GetAttribute(currLine, "shader", 10);
                    }
                }
            }
        }

        // Create Gameobjects and load meshes

        for (int i = 0; i < gameObjectNodes.size(); i++) {
            if (ImportObj(gameObjectNodes[i]->meshPath)) {
                std::shared_ptr<Shaders::MultiLighting> s = LoadShader(tempNode->shaderPath);
                
                logic.CreateGameEntity(loadedMeshes[loadedMeshesCount - 1], s, loadedMaterials[loadedMeshesCount - 1], gameObjectNodes[i]->transform);
                
                std::cerr << "Loaded Mesh" << std::endl;
            } else {
                std::cerr << "Failed to import: " + gameObjectNodes[i]->meshPath;
            }
        }

        return true;
    }
}