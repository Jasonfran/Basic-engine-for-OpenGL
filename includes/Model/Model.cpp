#include "Model.h"
#include "../Texture2D/Texture2D.h"
#include "../stb_image.h"

#include <iostream>
#include <memory>
GLint textureFromFile( const char* path, std::string directory );

void Model::draw(Shader &shader)
{
	for (Mesh &m : meshes) {
		m.getMaterial().setUniforms( shader );
		m.draw(shader);
	}
}

void Model::setMaterial( Material mat )
{
	for (Mesh &m : meshes)
	{
		m.setMaterial( mat );
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals );

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Error: Assimp: " << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
	//importer.FreeScene();
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back( this->processMesh( mesh, scene ) );
	}

	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode( node->mChildren[i], scene );
	}
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	std::vector<glm::vec3> colours(4); //0 = ambient, 1 = diffuse, 2 = specular, first element of 3 = specular exponent

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position = glm::vec3( mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = glm::vec3( mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z );
		if (mesh->mTextureCoords[0]) // can check if there are texture coordinatee
		{
			vertex.texCoords = glm::vec2( mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y );
		}
		else
		{
			vertex.texCoords = glm::vec2( 0.0f );
		}
		vertices.push_back( vertex );
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back( face.mIndices[j] );
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> ambientMaps = loadMaterialTextures( material, aiTextureType_AMBIENT, AMBIENT );
		if (ambientMaps.size() == 0)
		{
			aiColor3D ambient;
			if(material->Get( AI_MATKEY_COLOR_AMBIENT, ambient ) == aiReturn_SUCCESS)
				colours[0] = glm::vec3( ambient.r, ambient.g, ambient.b );
		}
		else
			textures.insert( textures.end(), ambientMaps.begin(), ambientMaps.end() );

		std::vector<Texture> diffuseMaps = loadMaterialTextures( material, aiTextureType_DIFFUSE, DIFFUSE );
		if (diffuseMaps.size() == 0)
		{
			aiColor3D diffuse;
			if(material->Get( AI_MATKEY_COLOR_DIFFUSE, diffuse ) == aiReturn_SUCCESS)
				colours[1] = glm::vec3( diffuse.r, diffuse.g, diffuse.b );
		}else
			textures.insert( textures.end(), diffuseMaps.begin(), diffuseMaps.end() );

		std::vector<Texture> specularMaps = loadMaterialTextures( material, aiTextureType_SPECULAR, SPECULAR );
		textures.insert( textures.end(), specularMaps.begin(), specularMaps.end() );
		std::vector<Texture> normalMaps = loadMaterialTextures( material, aiTextureType_NORMALS, NORMAL );
		textures.insert( textures.end(), normalMaps.begin(), normalMaps.end() );

		// use G and B to indicate whether there is or isn't a specular colour and shininess value. bad models sometimes miss them
		// This is lazy on my part and not clear without comments
		aiColor3D specularColour;
		if (material->Get( AI_MATKEY_COLOR_SPECULAR, specularColour ) == aiReturn_SUCCESS)
		{
			colours[2] = glm::vec3( specularColour.r, specularColour.g, specularColour.b );
			colours[3].g = 1.0f;
		}
		else
			colours[3].g = 0.0f;

		float shininess;
		if (material->Get( AI_MATKEY_SHININESS, shininess ) == aiReturn_SUCCESS)
		{
			colours[3] = glm::vec3( shininess, 0.0f, 0.0f );
			colours[3].b = 1.0f;
		}
		else
			colours[3].b = 0.0f;
	}
	return Mesh( vertices, indices, textures, colours );
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, MaterialType matType)
{
	std::vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount( type ); i++)
	{
		aiString str;
		mat->GetTexture( type, i, &str );
		Texture texture;
		texture.id = textureFromFile( str.C_Str(), this->directory );
		texture.type = matType;
		texture.path = str;
		textures.push_back( texture );
	}
	return textures;
}

Model::~Model()
{
	std::cout << "Model destroyed" << std::endl;
	for (Mesh &m : meshes)
	{
		std::cout << "Destroying meshes" << std::endl;
		m.deleteBuffers();
	}
}

GLint textureFromFile( const char* path, std::string directory )
{
	//Generate texture ID and load texture data 
	std::string filename = std::string( path );
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures( 1, &textureID );
	int width, height;
	unsigned char* image = stbi_load( filename.c_str(), &width, &height, 0, STBI_rgb );
	// Assign texture to ID
	glBindTexture( GL_TEXTURE_2D, textureID );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
	glGenerateMipmap( GL_TEXTURE_2D );

	// Parameters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glBindTexture( GL_TEXTURE_2D, 0 );
	stbi_image_free( image );
	return textureID;
}
