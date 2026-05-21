#include "model_importer.h"

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

        //cout << i << ") Node: " << node->mName.C_Str() << " -> Mesh: " << mesh->mName.C_Str() << endl;

        meshes.push_back(processMesh(mesh));
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