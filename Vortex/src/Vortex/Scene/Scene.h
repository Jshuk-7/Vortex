#pragma once

#include "Vortex/Asset/Asset.h"

#include "Vortex/Core/UUID.h"
#include "Vortex/Core/Timer.h"
#include "Vortex/Core/TimeStep.h"

#include "Vortex/Project/ProjectType.h"

#include "Vortex/Scene/Components.h"

#include "Vortex/Renderer/Framebuffer.h"

#include "Vortex/stl/function_queue.h"

#include <unordered_map>
#include <vector>

#include <entt/entt.hpp>

namespace Vortex {

	class Actor;
	class Prefab;
	class Mesh;
	class StaticMesh;
	class EditorCamera;

	struct VORTEX_API SceneGeometry : public RefCounted
	{
		std::vector<SharedReference<Mesh>> Meshes;
		std::vector<Math::mat4> WorldSpaceMeshTransforms;
		std::vector<Actor> MeshEntities;

		std::vector<SharedReference<StaticMesh>> StaticMeshes;
		std::vector<Math::mat4> WorldSpaceStaticMeshTransforms;
	};

	class VORTEX_API Scene : public Asset
	{
	public:
		Scene() = default;
		Scene(SharedReference<Framebuffer>& targetFramebuffer);
		~Scene() override;

		Actor CreateActor(const std::string& name = std::string(), const std::string& marker = std::string());
		Actor CreateChildActor(Actor parent, const std::string& name = std::string(), const std::string& marker = std::string());
		Actor CreateActorWithUUID(UUID uuid, const std::string& name = std::string(), const std::string& marker = std::string());

		Actor DuplicateActor(Actor actor);
		Actor CreatePrefabActor(Actor prefabActor, Actor parent, const Math::vec3* translation = nullptr, const Math::vec3* eulerRotation = nullptr, const Math::vec3* scale = nullptr);

		Actor Instantiate(SharedReference<Prefab> prefab, const Math::vec3* translation = nullptr, const Math::vec3* eulerRotation = nullptr, const Math::vec3* scale = nullptr);
		Actor InstantiateChild(SharedReference<Prefab> prefab, Actor parent, const Math::vec3* translation = nullptr, const Math::vec3* eulerRotation = nullptr, const Math::vec3* scale = nullptr);

		void SubmitToDestroyActor(Actor actor, bool excludeChildren = false);

		void ClearActors();

		void OnRuntimeStart(bool muteAudio = false);
		void OnRuntimeStop();

		void OnPhysicsSimulationStart();
		void OnPhysicsSimulationUpdate(TimeStep delta);
		void OnPhysicsSimulationStop();

		void OnUpdateRuntime(TimeStep delta);
		void OnUpdateSimulation(TimeStep delta, EditorCamera* camera);
		void OnUpdateEditor(TimeStep delta, EditorCamera* camera);

		VX_FORCE_INLINE vxstl::function_queue<void>& GetPreUpdateFunctionQueue() const { return m_PreUpdateFunctionQueue; }
		VX_FORCE_INLINE vxstl::function_queue<void>& GetPostUpdateFunctionQueue() const { return m_PostUpdateFunctionQueue; }

		VX_FORCE_INLINE const std::string& GetName() const { return m_Name; }
		VX_FORCE_INLINE void SetName(const std::string& name) { m_Name = name; }

		void InvokeActorOnGuiRender();

		VX_FORCE_INLINE SharedReference<Framebuffer> GetTargetFramebuffer() const { return m_TargetFramebuffer; }
		VX_FORCE_INLINE void SetTargetFramebuffer(SharedReference<Framebuffer> target) { m_TargetFramebuffer = target; }

		VX_FORCE_INLINE bool IsRunning() const { return m_IsRunning; }
		VX_FORCE_INLINE bool IsSimulating() const { return m_IsSimulating; }
		VX_FORCE_INLINE bool IsPaused() const { return m_IsPaused; }
		void SetPaused(bool paused);

		VX_FORCE_INLINE Math::uvec2 GetViewportSize() const { return Math::uvec2(m_ViewportWidth, m_ViewportHeight); }
		VX_FORCE_INLINE const ViewportBounds& GetViewportBounds() const { return m_ViewportBounds; }
		VX_FORCE_INLINE void SetViewportBounds(const ViewportBounds& viewportBounds) { m_ViewportBounds = viewportBounds; }

		VX_FORCE_INLINE size_t GetActorCount() const { return m_Registry.alive(); }
		size_t GetScriptActorCount();

		void Step(uint32_t frames = 1) { m_StepFrames = frames; }

		void OnViewportResize(uint32_t width, uint32_t height);

		const Timer& TryGetTimerByName(Actor actor, const std::string& name);
		Timer& TryGetMutableTimerByName(Actor actor, const std::string& name);
		void EmplaceOrReplaceTimer(Actor actor, Timer&& timer);

		void ParentActor(Actor actor, Actor parent);
		void UnparentActor(Actor actor, bool convertToWorldSpace = true);

		void ActiveateChildren(Actor actor);
		void DeactiveateChildren(Actor actor);

		Actor GetRootActorInHierarchy(Actor child) const;

		Actor GetPrimaryCameraActor();
		Actor GetEnvironmentActor();
		Actor GetSkyLightActor();

		Actor TryGetActorWithUUID(UUID uuid) const;
		Actor FindActorByName(std::string_view name);
		Actor FindActorByID(entt::entity actorID);

		void SortActors();

		void ConvertToLocalSpace(Actor actor);
		void ConvertToWorldSpace(Actor actor);
		Math::mat4 GetWorldSpaceTransformMatrix(Actor actor);
		TransformComponent GetWorldSpaceTransform(Actor actor);

		SharedReference<SceneGeometry>& GetSceneMeshes();

		template <typename TComponent>
		VX_FORCE_INLINE void CopyComponentIfExists(entt::entity dst, entt::registry& dstRegistry, entt::entity src) const
		{
			if (m_Registry.all_of<TComponent>(src))
			{
				auto& srcComponent = m_Registry.get<TComponent>(src);
				dstRegistry.emplace_or_replace<TComponent>(dst, srcComponent);
			}
		}

		template <typename... TComponent>
		VX_FORCE_INLINE auto GetAllActorsWith()
		{
			return m_Registry.view<TComponent...>();
		}

		static SharedReference<Scene> Copy(SharedReference<Scene>& source);
		static void CreateSampleScene(ProjectType type, SharedReference<Scene>& context);

		ASSET_CLASS_TYPE(SceneAsset)

		static SharedReference<Scene> Create(SharedReference<Framebuffer> targetFramebuffer);
		static SharedReference<Scene> Create();

	private:
		void FlushPreUpdateQueue();
		void FlushPostUpdateQueue();

		void OnComponentUpdate(TimeStep delta);
		void OnSystemUpdate(TimeStep delta);

		void DestroyActorInternal(Actor actor, bool excludeChildren = false);

		void OnUpdateActorTimers(TimeStep delta);

		void OnCameraConstruct(entt::registry& registry, entt::entity e);
		void OnStaticMeshConstruct(entt::registry& registry, entt::entity e);
		void OnParticleEmitterConstruct(entt::registry& registry, entt::entity e);
		void OnParticleEmitterDestruct(entt::registry& registry, entt::entity e);
		void OnTextMeshConstruct(entt::registry& registry, entt::entity e);
		void OnAnimatorConstruct(entt::registry& registry, entt::entity e);
		void OnAnimationConstruct(entt::registry& registry, entt::entity e);
		void OnAudioSourceConstruct(entt::registry& registry, entt::entity e);
		void OnAudioSourceDestruct(entt::registry& registry, entt::entity e);
		void OnAudioListenerConstruct(entt::registry& registry, entt::entity e);
		void OnAudioListenerDestruct(entt::registry& registry, entt::entity e);

		void ResizePrimaryCamera();

		void StopAnimatorsRuntime();

		void OnMeshUpdateRuntime();
		void OnAnimatorUpdateRuntime(TimeStep delta);

		void ClearSceneMeshes();

	private:
		SharedReference<Framebuffer> m_TargetFramebuffer = nullptr;
		mutable SharedReference<SceneGeometry> m_SceneMeshes = nullptr;
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;
		ViewportBounds m_ViewportBounds;
		uint32_t m_StepFrames = 0;

		using ActorMap = std::unordered_map<UUID, Actor>;
		ActorMap m_ActorMap;

		std::unordered_map<UUID, std::vector<Timer>> m_Timers;
		std::vector<Timer> m_FinishedTimers;

		mutable vxstl::function_queue<void> m_PreUpdateFunctionQueue;
		mutable vxstl::function_queue<void> m_PostUpdateFunctionQueue;

		std::string m_Name;

		bool m_IsRunning = false;
		bool m_IsSimulating = false;
		bool m_IsPaused = false;

	private:
		friend class Actor;
		friend class Prefab;
		friend class PrefabAssetSerializer;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
		friend class ECSDebugPanel;
		friend class EditorLayer;
	};

}
