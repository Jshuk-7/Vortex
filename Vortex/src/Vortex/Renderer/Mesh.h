#pragma once

#include "Vortex/Asset/Asset.h"

#include "Vortex/Math/Math.h"
#include "Vortex/Math/AABB.h"

#include "Vortex/Scene/Components.h"

#include "Vortex/Renderer/VertexArray.h"
#include "Vortex/Renderer/Material.h"
#include "Vortex/Renderer/Shader.h"
#include "Vortex/Renderer/Buffer.h"
#include "Vortex/Renderer/MeshImportOptions.h"
#include "Vortex/Renderer/VertexTypes.h"

#include "Vortex/ReferenceCounting/SharedRef.h"

#include <unordered_map>
#include <vector>
#include <string>

struct aiMesh;
struct aiNode;
struct aiScene;
struct aiMaterial;

namespace Vortex {

#define MAX_BONE_INFLUENCE 4

	struct VORTEX_API BoneInfo
	{
		uint32_t ID;
		Math::mat4 OffsetMatrix;
	};

	class VORTEX_API Submesh
	{
	public:
		Submesh() = default;
		Submesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, SharedReference<Material>& material);
		~Submesh() = default;

		VX_FORCE_INLINE const std::string& GetName() const { return m_MeshName; }

		void Render() const;
		void RenderToSkylightShadowMap() const;

		VX_FORCE_INLINE const SharedReference<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		VX_FORCE_INLINE const SharedReference<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
		VX_FORCE_INLINE const SharedReference<Material>& GetMaterial() const { return m_Material; }
		VX_FORCE_INLINE SharedReference<Material>& GetMaterial() { return m_Material; }
		void SetMaterial(SharedReference<Material>& material);
		
		const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
		std::vector<Vertex>& GetVertices() { return m_Vertices; }

		const std::vector<uint32_t> GetIndices() const { return m_Indices; }

		const Math::AABB& GetBoundingBox() const { return m_BoundingBox; }

	private:
		void CreateAndUploadMesh();
		void CreateBoundingBoxFromVertices();

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		SharedReference<Material> m_Material = nullptr;

		SharedReference<VertexArray> m_VertexArray = nullptr;
		SharedReference<VertexBuffer> m_VertexBuffer = nullptr;
		SharedReference<IndexBuffer> m_IndexBuffer = nullptr;

		Math::AABB m_BoundingBox;

		// Debug, Also for editor
		std::string m_MeshName;
	};

	class VORTEX_API Mesh : public Asset
	{
	public:
		Mesh() = default;
		Mesh(const std::string& filepath, const TransformComponent& transform, const MeshImportOptions& importOptions, int entityID);
		~Mesh() override = default;

		void OnUpdate(int entityID = -1);

		const Submesh& GetSubmesh() const { return m_Submesh; }
		Submesh& GetSubmesh() { return m_Submesh; }

		std::unordered_map<std::string, BoneInfo>& GetBoneInfoMap() { return m_BoneInfoMap; }
		uint32_t& GetBoneCount() { return m_BoneCounter; }

		const Math::AABB& GetBoundingBox() const { return m_BoundingBox; }
		inline const MeshImportOptions& GetImportOptions() const { return m_ImportOptions; }
		inline bool HasAnimations() const { return m_HasAnimations; }
		inline bool IsLoaded() const { return m_IsLoaded; }

		ASSET_CLASS_TYPE(MeshAsset)

		static SharedReference<Mesh> Create(const std::string& filepath, const TransformComponent& transform, const MeshImportOptions& importOptions = MeshImportOptions(), int entityID = -1);

	private:
		void ProcessNode(const std::string& filepath, aiNode* node, const aiScene* scene, const MeshImportOptions& importOptions, const int entityID);
		Submesh ProcessMesh(const std::string& filepath, aiMesh* mesh, const aiScene* scene, const MeshImportOptions& importOptions, const int entityID);

		void SetVertexBoneDataToDefault(Vertex& vertex) const;
		void SetVertexBoneData(Vertex& vertex, int boneID, float weight) const;
		bool ExtractBoneWeightsForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

		void CreateBoundingBoxFromSubmeshes();

	private:
		//Entity m_Entity;
		Submesh m_Submesh;
		MeshImportOptions m_ImportOptions;
		const aiScene* m_Scene;

		std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;
		uint32_t m_BoneCounter = 0;

		Math::AABB m_BoundingBox;

		bool m_HasAnimations = false;
		bool m_IsLoaded = false;
	};

}
