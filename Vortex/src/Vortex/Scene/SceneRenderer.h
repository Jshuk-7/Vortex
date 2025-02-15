#pragma once

#include "Vortex/Core/Base.h"

#include "Vortex/Core/UUID.h"

#include "Vortex/Math/Math.h"

#include "Vortex/Renderer/Renderer.h"
#include "Vortex/Renderer/Renderer2D.h"

#include "Vortex/ReferenceCounting/SharedRef.h"

#include <map>

namespace Vortex {

	class Scene;
	class Actor;
	class Camera;
	class Skybox;
	class Material;
	class Framebuffer;
	struct TransformComponent;
	struct SkyboxComponent;

	struct VORTEX_API SceneRenderPacket
	{
		Camera* PrimaryCamera = nullptr;
		Math::mat4 PrimaryCameraViewMatrix = {};
		Math::mat4 PrimaryCameraProjectionMatrix = {};
		Math::vec3 PrimaryCameraWorldSpaceTranslation = {};
		SharedReference<Framebuffer> TargetFramebuffer = nullptr;
		Scene* Scene = nullptr;
		bool IsEditorScene = false;
	};

	class VORTEX_API SceneRenderer
	{
	public:
		SceneRenderer() = default;
		~SceneRenderer() = default;

		void RenderScene(const SceneRenderPacket& renderPacket);

	private:
		void OnRenderScene2D(const SceneRenderPacket& renderPacket);
		void OnRenderScene3D(const SceneRenderPacket& renderPacket);

		// 2D

		void BeginScene2D(const SceneRenderPacket& renderPacket);
		void EndScene2D();

		void LightPass2D(const SceneRenderPacket& renderPacket);
		void SpritePass2D(const SceneRenderPacket& renderPacket);
		void ParticlePass2D(const SceneRenderPacket& renderPacket);
		void TextPass2D(const SceneRenderPacket& renderPacket);
		void DebugRenderPass2D(const SceneRenderPacket& renderPacket);
		void SceneGizmosPass2D(const SceneRenderPacket& renderPacket);

		// 3D

		void BeginScene(const SceneRenderPacket& renderPacket);
		void EndScene();

		void PreparePostProcess(const SceneRenderPacket& renderPacket);

		void LightPass(const SceneRenderPacket& renderPacket);
		void EmissiveMeshPass(const SceneRenderPacket& renderPacket);
		void SortMeshGeometry(const SceneRenderPacket& renderPacket, std::map<float, Actor>& sortedGeometry);
		void SortActorByDistance(std::map<float, Actor>& sortedActors, float distance, Actor actor, uint32_t offset = 0);
		void GeometryPass(const SceneRenderPacket& renderPacket, const std::map<float, Actor>& sortedGeometry);
		void RenderMesh(Scene* scene, Actor actor, const SceneLightDescription& sceneLightDesc);
		void RenderStaticMesh(Scene* scene, Actor actor, const SceneLightDescription& sceneLightDesc);

		// Environment

		void FindCurrentEnvironment(const SceneRenderPacket& renderPacket, SkyboxComponent& skyboxComponent, SharedReference<Skybox>& environment);
		void SetEnvironment(AssetHandle environmentHandle, SkyboxComponent& skyboxComponent, SharedReference<Skybox>& environment);
		void ClearEnvironment();
		void RenderEnvironment(const Math::mat4& view, const Math::mat4& projection, SkyboxComponent* skyboxComponent, SharedReference<Skybox>& environment);

		// Helpers

		void SetMaterialFlags(const SharedReference<Material>& material);
		void ResetMaterialFlags();

	private:
		std::mutex m_GeometrySortMutex;
		RendererAPI::TriangleCullMode m_LastCullMode;

	private:
		friend class Scene;
	};

}
