#pragma once

#include "Vortex/Core/Base.h"

#include "Vortex/Math/Math.h"

#include "Vortex/Scene/Actor.h"
#include "Vortex/Scene/Components.h"

#include "Vortex/Physics/3D/PhysicsMaterial.h"

#include "Vortex/ReferenceCounting/SharedRef.h"

namespace physx {

	class PxMaterial;
	struct PxFilterData;
	class PxRigidActor;
	class PxShape;

}

namespace Vortex {

	class VORTEX_API ColliderShape : public RefCounted
	{
	protected:
		ColliderShape(ColliderType type, Actor actor, bool isShared = false);

	public:
		virtual ~ColliderShape();

		virtual void Release();

		inline virtual ColliderType GetType() const { return m_Type; }

		virtual const Math::vec3& GetOffset() const = 0;
		virtual void SetOffset(const Math::vec3& offset) = 0;

		virtual bool IsTrigger() const = 0;
		virtual void SetTrigger(bool isTrigger) = 0;

		virtual void SetFilterData(const physx::PxFilterData& filterData) = 0;

		virtual void DetachFromActor(physx::PxRigidActor* actor) = 0;

		virtual const char* GetShapeName() const = 0;

		inline virtual physx::PxMaterial* GetMaterial() const { return m_Material; }
		virtual void SetMaterial(SharedReference<PhysicsMaterial>& material);

		inline virtual bool IsShared() const { return m_IsShared; }
		inline virtual bool IsValid() const { return m_Material != nullptr; }

	protected:
		ColliderType m_Type;
		Actor m_Actor;
		physx::PxMaterial* m_Material = nullptr;
		bool m_IsShared = false;
	};

	class VORTEX_API BoxColliderShape : public ColliderShape
	{
	public:
		BoxColliderShape(BoxColliderComponent& component, physx::PxRigidActor& pxActor, Actor actor);
		~BoxColliderShape() override = default;

		const Math::vec3& GetHalfSize() const;
		void SetHalfSize(const Math::vec3& halfSize);

		const Math::vec3& GetOffset() const override;
		void SetOffset(const Math::vec3& offset) override;

		bool IsTrigger() const override;
		void SetTrigger(bool isTrigger) override;

		void SetFilterData(const physx::PxFilterData& filterData) override;

		void DetachFromActor(physx::PxRigidActor* actor) override;

		inline const char* GetShapeName() const override { return "BoxCollider"; }
		inline bool IsValid() const override { return ColliderShape::IsValid() && m_Shape != nullptr; }

		inline static ColliderType GetStaticType() { return ColliderType::Box; }

	private:
		physx::PxShape* m_Shape = nullptr;
	};

	class VORTEX_API SphereColliderShape : public ColliderShape
	{
	public:
		SphereColliderShape(SphereColliderComponent& component, physx::PxRigidActor& pxActor, Actor actor);
		~SphereColliderShape() override = default;

		float GetRadius() const;
		void SetRadius(float radius);

		const Math::vec3& GetOffset() const override;
		void SetOffset(const Math::vec3& offset) override;

		bool IsTrigger() const override;
		void SetTrigger(bool isTrigger) override;

		void SetFilterData(const physx::PxFilterData& filterData) override;

		void DetachFromActor(physx::PxRigidActor* actor) override;

		inline const char* GetShapeName() const override { return "SphereCollider"; }
		inline bool IsValid() const override { return ColliderShape::IsValid() && m_Shape != nullptr; }

		inline static ColliderType GetStaticType() { return ColliderType::Sphere; }

	private:
		physx::PxShape* m_Shape = nullptr;
	};

	class VORTEX_API CapsuleColliderShape : public ColliderShape
	{
	public:
		CapsuleColliderShape(CapsuleColliderComponent& component, physx::PxRigidActor& pxActor, Actor actor);
		~CapsuleColliderShape() override = default;

		float GetRadius() const;
		void SetRadius(float radius);

		float GetHeight() const;
		void SetHeight(float height);

		const Math::vec3& GetOffset() const override;
		void SetOffset(const Math::vec3& offset) override;

		bool IsTrigger() const override;
		void SetTrigger(bool isTrigger) override;

		void SetFilterData(const physx::PxFilterData& filterData) override;

		void DetachFromActor(physx::PxRigidActor* actor) override;

		inline const char* GetShapeName() const override { return "CapsuleCollider"; }
		inline bool IsValid() const override { return ColliderShape::IsValid() && m_Shape != nullptr; }

		inline static ColliderType GetStaticType() { return ColliderType::Capsule; }

	private:
		physx::PxShape* m_Shape = nullptr;
	};

	class VORTEX_API ConvexMeshShape : public ColliderShape
	{
	public:
		ConvexMeshShape(MeshColliderComponent& component, physx::PxRigidActor& pxActor, Actor actor);
		~ConvexMeshShape() override = default;

		virtual const Math::vec3& GetOffset() const override;
		void SetOffset(const Math::vec3& offset) override;

		bool IsTrigger() const override;
		void SetTrigger(bool isTrigger) override;

		void SetFilterData(const physx::PxFilterData& filterData) override;

		void DetachFromActor(physx::PxRigidActor* actor) override;

		inline const char* GetShapeName() const override { return "ConvexMeshCollider"; }
		inline bool IsValid() const override { return ColliderShape::IsValid() && !m_Shapes.empty(); }

		inline static ColliderType GetStaticType() { return ColliderType::ConvexMesh; }

	private:
		std::vector<physx::PxShape*> m_Shapes;
	};

	class VORTEX_API TriangleMeshShape : public ColliderShape
	{
	public:
		TriangleMeshShape(MeshColliderComponent& component, physx::PxRigidActor& pxActor, Actor actor);
		~TriangleMeshShape() override = default;

		const Math::vec3& GetOffset() const override;
		void SetOffset(const Math::vec3& offset) override;

		bool IsTrigger() const override;
		void SetTrigger(bool isTrigger) override;

		void SetFilterData(const physx::PxFilterData& filterData) override;

		void DetachFromActor(physx::PxRigidActor* actor) override;

		inline const char* GetShapeName() const override { return "TriangleMeshCollider"; }
		inline bool IsValid() const override { return ColliderShape::IsValid() && !m_Shapes.empty(); }

		inline static ColliderType GetStaticType() { return ColliderType::TriangleMesh; }

	private:
		std::vector<physx::PxShape*> m_Shapes;
	};

	class SharedShapeManager
	{
	public:
		struct SharedShapeData
		{
			ECollisionComplexity Usage = ECollisionComplexity::Default;
			std::unordered_map<uint32_t, physx::PxShape*> Shapes;
		};

		using SharedShapeMap = std::unordered_map<ColliderType, std::unordered_map<AssetHandle, std::vector<SharedShapeData>>>;

	public:
		static SharedShapeData* CreateSharedShapeData(ColliderType type, AssetHandle colliderHandle);
	};

}
