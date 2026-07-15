#include "model_importer.h"
#include "loader.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace gllib;
using namespace std;

Mesh ModelImporter::processMesh(aiMesh* mesh) {
    vector<Vertex> vertices;
    vector<int> indices;

    for (int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        vertex.color = Color({1,1,1,1});

        if (mesh->mTextureCoords[0]) {
            vertex.uv.x = mesh->mTextureCoords[0][i].x;
            vertex.uv.y = mesh->mTextureCoords[0][i].y;
        }
        else {
            vertex.uv = Vector2(0,0);
        }

        vertices.push_back(vertex);
    }

    for (int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return Mesh(vertices, indices);
}

Material ModelImporter::processMaterial(aiMaterial* material, const string& directory) {
    Material mat;

    aiColor3D color(0.f, 0.f, 0.f);
    float shininess = 0.0f;
    float opacity = 1.0f;

    if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) {
        mat.ambient = Vector3(color.r, color.g, color.b);
    }
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
        mat.diffuse = Vector3(color.r, color.g, color.b);
    }
    if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
        mat.specular = Vector3(color.r, color.g, color.b);
    }
    if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
        mat.shininess = shininess;
    }
    if (material->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS) {
        mat.alpha = opacity;
    }

    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString str;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &str);

        string texturePath = string(str.C_Str());

#ifndef _WIN32
        replace(texturePath.begin(), texturePath.end(), '\\', '/');
#endif

        if (!texturePath.empty() && (texturePath[0] == '/' || texturePath[0] == '\\')) {
            texturePath = texturePath.substr(1);
        }

        string fullPath = "";
        if (!directory.empty()) {
            fullPath = directory + "/" + texturePath;
        } else {
            fullPath = texturePath;
        }

        bool isTransparent = (opacity < 1.0f);
        mat.texture = Loader::loadTextureAdvanced(fullPath, GL_REPEAT, GL_LINEAR, isTransparent);
    }

    return mat;
}

void ModelImporter::processNode(aiNode* node, const aiScene* scene, vector<Mesh>& meshes) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        cout << i << ") Node: " << node->mName.C_Str() << " -> Mesh: " << mesh->mName.C_Str() << " | Mat Index: " << mesh->mMaterialIndex << endl;

        Mesh processedMesh = processMesh(mesh);
        Mesh finalMesh(processedMesh.getVertices(), processedMesh.getIndices(), mesh->mMaterialIndex);

        meshes.push_back(finalMesh);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, meshes);
    }
}

ModelNode* ModelImporter::processNodeSynchronized(aiNode* node, const aiScene* scene, vector<Mesh>& outMeshes, int& meshCounter, int& nodeCounter) {
    aiVector3D aiScale;
    aiQuaternion aiRotation;
    aiVector3D aiTranslation;

    node->mTransformation.Decompose(aiScale, aiRotation, aiTranslation);

    glm::quat glmQuat(aiRotation.w, aiRotation.x, aiRotation.y, aiRotation.z);
    glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(glmQuat));

    int currentNodeId = nodeCounter++;
    ModelNode* mNode = new ModelNode(currentNodeId);

    mNode->setLocalPosition({aiTranslation.x, aiTranslation.y, aiTranslation.z});
    mNode->setLocalRotation({eulerRotation.x, eulerRotation.y, eulerRotation.z});
    mNode->setLocalScale({aiScale.x, aiScale.y, aiScale.z});

    cout << "[" << currentNodeId << "] Node: " << node->mName.C_Str();

    if (node->mNumMeshes == 1) {
        aiMesh* aiMeshRef = scene->mMeshes[node->mMeshes[0]];
        Mesh processedMesh = processMesh(aiMeshRef);
        Mesh finalMesh(processedMesh.getVertices(), processedMesh.getIndices(), aiMeshRef->mMaterialIndex);

        int currentMeshIndex = meshCounter++;
        outMeshes.push_back(finalMesh);

        BoundingBox localAABB = calculateLocalBounds(finalMesh);

        int childNodeId = nodeCounter++;
        ModelNode* meshChild = new ModelNode(childNodeId, currentMeshIndex, localAABB);
        mNode->addChild(meshChild);

        cout << " -> Mesh Name: " << aiMeshRef->mName.C_Str() << " (Geom Node ID: " << childNodeId << ")";
    }
    else if (node->mNumMeshes > 1) {
        cout << " -> Meshes: [";
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* aiMeshRef = scene->mMeshes[node->mMeshes[i]];
            Mesh processedMesh = processMesh(aiMeshRef);
            Mesh finalMesh(processedMesh.getVertices(), processedMesh.getIndices(), aiMeshRef->mMaterialIndex);

            int currentMeshIndex = meshCounter++;
            outMeshes.push_back(finalMesh);

            BoundingBox localAABB = calculateLocalBounds(finalMesh);

            int childNodeId = nodeCounter++;
            ModelNode* meshChild = new ModelNode(childNodeId, currentMeshIndex, localAABB);
            mNode->addChild(meshChild);

            cout << aiMeshRef->mName.C_Str() << " (ID: " << childNodeId << ")" << (i < node->mNumMeshes - 1 ? ", " : "");
        }
        cout << "]";
    }
    cout << endl;

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ModelNode* childNode = processNodeSynchronized(node->mChildren[i], scene, outMeshes, meshCounter, nodeCounter);
        mNode->addChild(childNode);
    }

    return mNode;
}

glm::mat4 ModelImporter::aiMatrixToGlm(const aiMatrix4x4& from) {
    glm::mat4 to;
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    return to;
}

BoundingBox ModelImporter::calculateLocalBounds(Mesh& mesh) {
    BoundingBox bounds;
    auto& vertices = mesh.getVertices();
    if (vertices.empty()) {
        return { {0,0,0}, {0,0,0} };
    }
    bounds.min = vertices[0].position;
    bounds.max = vertices[0].position;
    for (const auto& vertex : vertices) {
        if (vertex.position.x < bounds.min.x) bounds.min.x = vertex.position.x;
        if (vertex.position.y < bounds.min.y) bounds.min.y = vertex.position.y;
        if (vertex.position.z < bounds.min.z) bounds.min.z = vertex.position.z;

        if (vertex.position.x > bounds.max.x) bounds.max.x = vertex.position.x;
        if (vertex.position.y > bounds.max.y) bounds.max.y = vertex.position.y;
        if (vertex.position.z > bounds.max.z) bounds.max.z = vertex.position.z;
    }
    return bounds;
}

ImportedModelParts ModelImporter::loadModelParts(const string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        throw runtime_error(importer.GetErrorString());
    }

    ImportedModelParts parts;
    int meshCounter = 0;
    int nodeCounter = 0;

    parts.rootNode = processNodeSynchronized(scene->mRootNode, scene, parts.meshes, meshCounter, nodeCounter);

    return parts;
}

MeshGroup ModelImporter::loadMeshGroup(const string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);
    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        throw runtime_error(importer.GetErrorString());
    }

    vector<Mesh> meshes;

    processNode(scene->mRootNode, scene, meshes);

    return MeshGroup(meshes);
}

MaterialGroup ModelImporter::loadMaterialGroup(const string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);
    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        throw runtime_error(importer.GetErrorString());
    }

    string directory = "";
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != string::npos) {
        directory = path.substr(0, lastSlash);
    }
    vector<Material> materials;

    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial* material = scene->mMaterials[i];
        materials.push_back(processMaterial(material, directory));
    }

    return MaterialGroup(materials);
}