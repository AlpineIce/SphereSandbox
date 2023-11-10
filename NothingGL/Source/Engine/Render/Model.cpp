#include "Model.h"
#include <iostream>

namespace Renderer
{


	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex)
		: vertices(vertices), indices(indices), materialIndex(materialIndex)
	{

		initMesh();
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::initMesh()
	{
		VA.createBuffer();
		VA.bind();
		VB.createBuffer(vertices);
		IB.createBuffer(indices);
		VA.setIndexBuffer(IB);
		VA.addBuffer(VB);
	}

	void Mesh::draw(const Camera& camera, glm::mat4& instMatrix, Material* material)
	{
		glm::mat4 modelMatrix = getModelMatrix();

		if (material)
		{
			material->useMaterial();
			glm::mat3 transposeMat = glm::inverseTranspose(modelMatrix * instMatrix);
			material->getShader()->setMat3("transposeMat", transposeMat);

			int projectionLoc = glGetUniformLocation(material->getShader()->getShaderID(), "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjection()));

			int viewLoc = glGetUniformLocation(material->getShader()->getShaderID(), "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getView()));

			int modelLoc = glGetUniformLocation(material->getShader()->getShaderID(), "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix * instMatrix));

			VA.bind();
			glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
			VA.unbind();

			//material->unbind();

		}
		else
		{
			std::cout << "Error: No material is assigned to mesh at location " << this << " please assign a material for proper rendering" << "\n";
		}
	}

	void Mesh::setTransformation(Transformation transform)
	{
		//quaternions SUCK
		glm::normalize(transform.rotation);
		if (!isnan(transform.rotation.x))
		{
			this->transformation.rotation = transform.rotation;
		}
		this->transformation.location = transform.location;
		this->transformation.scale = transform.scale;
	}

	void Mesh::setMaterialIndex(unsigned int materialIndex)
	{
		this->materialIndex = materialIndex;
	}

	glm::mat4 Mesh::getModelMatrix()
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), this->transformation.location);
		glm::mat4 rotate = glm::mat4_cast(this->transformation.rotation);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->transformation.scale);

		return translate * rotate * scale;
	}


	//----------MODEL DEFINITIONS----------//


	Model::Model(std::string path)
	{
		directory = "";
		name = "";
		loadModel(path);
#ifndef NDEBUG

		std::cout << "Created a model with " << meshes.size() << " meshes" << "\n";
#endif
	}

	Model::~Model()
	{
#ifndef NDEBUG
		std::cout << "destroyed a model with " << meshes.size() << " meshes" << "\n";
#endif
	}

	void Model::loadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "bruh yo model doesn't work or the path is wrong" << importer.GetErrorString() << std::endl;
			return;
		}
		name = path.substr(path.find_last_of('/') + 1, path.find_last_of(".") - (path.find_last_of('/') + 1));
		directory = path.substr(0, path.find_last_of('/') + 1);
		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			ReturnMesh meshData = processMesh(mesh, scene);

			meshes.push_back(std::make_shared<Mesh>(meshData.vertices, meshData.indices, meshData.materialIndex)); //TODO ASSIGN MATERIAL
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

		//assign the materials
		int a = 0;
	}

	Model::ReturnMesh Model::processMesh(aiMesh* mesh, const aiScene* scene) //btw 75% of this was copy pasted from opengl tutorial
	{
		ReturnMesh returnMesh;

		//vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			aiVector3D position = mesh->mVertices[i];
			aiVector3D normal = mesh->mNormals[i];
			aiVector3D texCoords = mesh->mTextureCoords[0][i];
			aiVector3D tangents = mesh->mBitangents[i];
			aiVector3D bitangents = mesh->mTangents[i];
			vertex.positions = { position.x, position.y, position.z };
			vertex.normals = { normal.x, normal.y, normal.z };
			

			//texture coordinates
			if (mesh->mTextureCoords[0])
			{
				vertex.texCoords = { texCoords.x, texCoords.y };
			}
			else
			{
				vertex.texCoords = { 0.0f, 0.0f };
			}

			//tangents
			if (mesh->mTextureCoords[0])
			{
				vertex.tangents = { tangents.x, tangents.y, tangents.z };
			}
			else
			{
				vertex.tangents = { 0.0f, 0.0f, 0.0f };
			}

			//bitangents
			if (mesh->mTextureCoords[0])
			{
				vertex.bitangents = { bitangents.x, bitangents.y, bitangents.z };
			}
			else
			{
				vertex.bitangents = { 0.0f, 0.0f, 0.0f };
			}


			returnMesh.vertices.push_back(vertex);
		}

		//indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				returnMesh.indices.push_back(face.mIndices[j]);
			}
		}
		
		//material slot
		returnMesh.materialIndex = mesh->mMaterialIndex;
		materialSlots[returnMesh.materialIndex] = nullptr;

		return returnMesh;
	}

	void Model::draw(const Camera& camera, glm::mat4& instMatrix)
	{
		unsigned int meshIndex = 0;
		for (std::shared_ptr<Mesh> mesh : meshes)
		{
			mesh->draw(camera, instMatrix, materialSlots.at(mesh->getMaterialIndex()));
			meshIndex++;
		}
	}

	void Model::setTransformation(Transformation transform)
	{
		for (auto mesh : meshes)
		{
			mesh->setTransformation(transform);
		}
	}

	//----------MODEL INSTANCE DEFINITIONS----------//

	ModelInstance::ModelInstance(Model* model)
		:parentModel(model)
	{
		modelMatrix = getMatrix();
	}

	ModelInstance::~ModelInstance()
	{
	}

	glm::mat4 ModelInstance::getMatrix()
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), this->transformation.location);
		glm::mat4 rotate = glm::mat4_cast(this->transformation.rotation);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->transformation.scale);

		return translate * rotate * scale;
	}

	void ModelInstance::setTransformation(Transformation transform)
	{
		this->transformation = transform;
		this->modelMatrix = getMatrix();
	}

	void ModelInstance::render(const Camera* camera)
	{
		if (parentModel != NULL)
		{
			parentModel->draw(*camera, modelMatrix);
		}
		else
		{
			std::cout << "Warning: model instance parent model isn't valid" << "\n";
		}
	}
}