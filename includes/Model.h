#pragma once
#include "../includes/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../includes/SceneObject.h"

class Model : public SceneObject
{
public:
	Model() {}
	Model( GLchar* path )
	{
		loadModel( path );
	}
	void draw( Shader shader );
private:
	std::vector<Mesh> meshes;
	std::string directory;
	void loadModel( std::string path );
	void processNode( aiNode* node, const aiScene* scene );
	Mesh processMesh( aiMesh* mesh, const aiScene* scene );
	std::vector<Texture> loadMaterialTextures( aiMaterial* mat, aiTextureType type, MaterialType matType );
};