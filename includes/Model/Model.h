#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public SceneObject
{
public:
	Model( GLchar* path )
	{
		loadModel( path );
	}
	void draw( Shader &shader );
	void setMaterial( Material mat );
	~Model();
private:
	std::vector<Mesh> meshes;
	std::string directory;
	void loadModel( std::string path );
	void processNode( aiNode* node, const aiScene* scene );
	Mesh processMesh( aiMesh* mesh, const aiScene* scene );
	std::vector<Texture> loadMaterialTextures( aiMaterial* mat, aiTextureType type, MaterialType matType );
};