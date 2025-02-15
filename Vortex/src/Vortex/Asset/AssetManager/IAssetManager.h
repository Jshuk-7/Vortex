#pragma once

#include "Vortex/Core/Base.h"

#include "Vortex/Asset/Asset.h"
#include "Vortex/Asset/AssetTypes.h"

#include "Vortex/ReferenceCounting/SharedRef.h"

#include <unordered_map>
#include <unordered_set>

namespace Vortex {

	class VORTEX_API IAssetManager : public RefCounted
	{
	public:
		IAssetManager() = default;
		virtual ~IAssetManager() = default;

		virtual AssetType GetAssetType(AssetHandle handle) const = 0;
		virtual SharedReference<Asset> GetAsset(AssetHandle handle) = 0;
		virtual void AddMemoryOnlyAsset(SharedReference<Asset> asset) = 0;
		virtual bool ReloadData(AssetHandle handle) = 0;
		virtual bool IsHandleValid(AssetHandle handle) = 0;
		virtual bool IsMemoryOnlyAsset(AssetHandle handle) = 0;
		virtual bool IsAssetLoaded(AssetHandle handle) = 0;

		virtual std::unordered_set<AssetHandle> GetAllAssetsWithType(AssetType type) const = 0;
		virtual const std::unordered_map<AssetHandle, SharedReference<Asset>>& GetLoadedAssets() const = 0;
		virtual const std::unordered_map<AssetHandle, SharedReference<Asset>>& GetMemoryOnlyAssets() const = 0;
	};

}
