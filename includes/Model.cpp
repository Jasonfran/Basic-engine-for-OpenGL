#include "Model.h"

#include <iostream>

void Model::draw(Shader shader)
{
	for (Mesh m : meshes) {
		m.draw(shader);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Error: Assimp: " << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	return Mesh();
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, MaterialType matType)
{
	return std::vector<Texture>();
}
