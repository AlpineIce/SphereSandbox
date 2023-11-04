#pragma once

#include "Material.h"
#include "Vertexbuffer.h"
#include "Texture.h"
#include "Vertexbuffer.h"
#include "Indexbuffer.h"
#include "VertexArray.h"
#include "Camera.h"

#include <vector>
#include <memory>
#include <map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLM/gtc/matrix_inverse.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//	Mesh Mesh Mesh Mesh Mesh
//	  |	   |   |    |    |
//	  --------------------
//			   |
//			 Model
//			   |
//	 ---------------------
//	 |    |    |    |    |
//  Inst Inst Inst Inst Inst
//

namespace Renderer
{

	struct Transformation
	{
		glm::vec3 location = { 0.0f, 0.0f, 0.0f };
		glm::quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	};

	class Mesh
	{
	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		glm::mat4 modelMatrix;
		Transformation transformation;
		unsigned int materialIndex;

		VertexBuffer VB;
		IndexBuffer IB;
		VertexArray VA;

		void initMesh();

	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex);
		~Mesh();

		void draw(const Camera& camera, glm::mat4& instMatrix, Material* material);

		void setTransformation(Transformation transform);
		void setMaterialIndex(unsigned int materialIndex);

		glm::mat4 getModelMatrix();
		inline unsigned int getMaterialIndex() { return materialIndex; }
	};

	//wrapper for multiple Mesh classes (think of a model with multiple parts)
	class Model
	{
	private:
		std::vector<std::shared_ptr<Mesh>> meshes;
		Transformation transformation;
		std::string directory;
		std::string name;
		std::map<unsigned int, Material*> materialSlots;

		struct ReturnMesh {
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			unsigned int materialIndex = 0;
		};

		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		ReturnMesh processMesh(aiMesh* mesh, const aiScene* scene);

	public:
		Model(std::string path);
		~Model();

		void draw(const Camera& camera, glm::mat4& instMatrix);
		void setTransformation(Transformation transform);

		inline std::string getName() { return name; }
		inline std::string getDirectory() { return directory; }
		inline std::map<unsigned int, Material*> getMaterialSlots() { return materialSlots; }
		inline void setMaterialSlots(std::map<unsigned int, Material*> materialMap) { this->materialSlots = materialMap; }
	};

	//class to draw instanced models
	class ModelInstance
	{
	private:
		Model* parentModel;
		glm::mat4 modelMatrix;
		Transformation transformation;
		glm::mat4 getMatrix();

	public:
		ModelInstance(Model* model);
		~ModelInstance();

		void setTransformation(Transformation transform);
		void render(const Camera* camera);
	};

}