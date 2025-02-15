#pragma once

#include "Vortex/Asset/Asset.h"

#include "Vortex/Physics/3D/PhysicsTypes.h"

#include <string>

namespace Vortex {

	class VORTEX_API PhysicsMaterial : public Asset
	{
	public:
		std::string Name = "";
		float StaticFriction = 0.6f;
		float DynamicFriction = 0.6f;
		float Bounciness = 0.0f;

		CombineMode FrictionCombineMode = CombineMode::Average;
		CombineMode BouncinessCombineMode = CombineMode::Average;

	public:
		PhysicsMaterial() = default;
		PhysicsMaterial(const std::string& name, float staticFriction, float dynamicFriction, float bounciness);
		PhysicsMaterial(
			float staticFriction,
			float dynamicFriction,
			float bounciness,
			CombineMode frictionCombine = CombineMode::Average,
			CombineMode bouncinessCombine = CombineMode::Average
		);

		~PhysicsMaterial() override = default;

		ASSET_CLASS_TYPE(PhysicsMaterialAsset)
	};

}
