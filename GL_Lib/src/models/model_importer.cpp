#include "model_importer.h"
#include "loader.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

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
        mat.texture = Loader::loadTexture(fullPath, isTransparent);
    }

    return mat;
}

Mesh ModelImporter::loadMesh(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw runtime_error(importer.GetErrorString());
    }
    if (scene->mNumMeshes == 0) {
        throw runtime_error("Model contains no meshes.");
    }
    aiMesh* mesh = scene->mMeshes[0];

    return processMesh(mesh);
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