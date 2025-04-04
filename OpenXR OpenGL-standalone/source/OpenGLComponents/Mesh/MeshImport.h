#pragma once
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

bool LoadMesh(std::string path, Mesh& mesh);
bool ProcessNode(aiNode* node, const aiScene* scene, Mesh& mesh);
Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);