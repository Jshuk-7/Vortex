#include "vxpch.h"
#include "AssetImporter.h"

#include "Vortex/Project/Project.h"

#include "Vortex/Serialization/SceneSerializer.h"

namespace Vortex {

	void AssetImporter::Init()
	{
		s_Serializers[AssetType::MeshAsset] = CreateUnique<MeshSerializer>();
		s_Serializers[AssetType::FontAsset] = CreateUnique<FontSerializer>();
		s_Serializers[AssetType::AudioAsset] = CreateUnique<AudioSerializer>();
		s_Serializers[AssetType::SceneAsset] = CreateUnique<SceneAssetSerializer>();
		s_Serializers[AssetType::PrefabAsset] = CreateUnique<PrefabAssetSerializer>();
		s_Serializers[AssetType::ScriptAsset] = CreateUnique<ScriptSerializer>();
		s_Serializers[AssetType::TextureAsset] = CreateUnique<TextureSerializer>();
		s_Serializers[AssetType::ParticleAsset] = CreateUnique<ParticleEmitterSerializer>();
		s_Serializers[AssetType::MaterialAsset] = CreateUnique<MaterialSerializer>();
		s_Serializers[AssetType::AnimatorAsset] = CreateUnique<AnimatorSerializer>();
		s_Serializers[AssetType::AnimationAsset] = CreateUnique<AnimationSerializer>();
		s_Serializers[AssetType::StaticMeshAsset] = CreateUnique<StaticMeshSerializer>();
		s_Serializers[AssetType::EnvironmentAsset] = CreateUnique<EnvironmentSerializer>();
		s_Serializers[AssetType::AudioListenerAsset] = CreateUnique<AudioListenerSerializer>();
		s_Serializers[AssetType::PhysicsMaterialAsset] = CreateUnique<PhysicsMaterialSerializer>();
	}

	void AssetImporter::Serialize(const AssetMetadata& metadata, const SharedReference<Asset>& asset)
	{
		if (!s_Serializers.contains(metadata.Type))
		{
			VX_CONSOLE_LOG_WARN("There are currently no asset serializers for assets of type {}", Utils::StringFromAssetType(metadata.Type));
			return;
		}

		s_Serializers[metadata.Type]->Serialize(metadata, asset);
	}

	void AssetImporter::Serialize(const SharedReference<Asset>& asset)
	{
		const AssetMetadata& metadata = Project::GetEditorAssetManager()->GetMetadata(asset->Handle);
		Serialize(metadata, asset);
	}

	bool AssetImporter::TryLoadData(const AssetMetadata& metadata, SharedReference<Asset>& asset)
	{
		if (!s_Serializers.contains(metadata.Type))
		{
			VX_CONSOLE_LOG_WARN("There are currently no importers for assets of type {}", Utils::StringFromAssetType(metadata.Type));
			return false;
		}

		return s_Serializers[metadata.Type]->TryLoadData(metadata, asset);
	}

}
