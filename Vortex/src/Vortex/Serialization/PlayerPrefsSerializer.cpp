#include "vxpch.h"
#include "PlayerPrefsSerializer.h"

#include "Vortex/Project/Project.h"

#include "Vortex/Utils/FileSystem.h"
#include "Vortex/Utils/YAML_SerializationUtils.h"

#include <fstream>

namespace Vortex {

	bool PlayerPrefsSerializer::HasKey(const std::string& key) const
	{
		if (m_ByteData.contains(key))
			return true;

		if (m_F32Data.contains(key))
			return true;
		if (m_F64Data.contains(key))
			return true;

		if (m_I8Data.contains(key))
			return true;
		if (m_I16Data.contains(key))
			return true;
		if (m_I32Data.contains(key))
			return true;
		if (m_I64Data.contains(key))
			return true;

		if (m_U8Data.contains(key))
			return true;
		if (m_U16Data.contains(key))
			return true;
		if (m_U32Data.contains(key))
			return true;
		if (m_U64Data.contains(key))
			return true;

		if (m_StringData.contains(key))
			return true;

		return false;
	}

	bool PlayerPrefsSerializer::Remove(const std::string& key)
	{
		if (!HasKey(key)) {
			return false;
		}

		RemoveKey(key);
		return true;
	}
	
	void PlayerPrefsSerializer::WriteInt(const std::string& key, int32_t value)
	{
		m_I32Data[key] = value;
	}

	bool PlayerPrefsSerializer::ReadInt(const std::string& key, int32_t* value)
	{
		if (!m_I32Data.contains(key)) {
			*value = 0;
			return false;
		}

		*value = m_I32Data[key];
		return true;
	}

	void PlayerPrefsSerializer::WriteString(const std::string& key, const std::string& value)
	{
		m_StringData[key] = value;
	}

	bool PlayerPrefsSerializer::ReadString(const std::string& key, std::string* value)
	{
		if (!m_StringData.contains(key)) {
			*value = std::string();
			return false;
		}

		*value = m_StringData[key];
		return true;
	}

	bool PlayerPrefsSerializer::Save(const std::string& filename) const
	{
		if (NoData()) { // we don't need to save
			return true;
		}

		YAML::Emitter out = YAML::Emitter();

		out << YAML::BeginMap; // Player Prefs
		out << YAML::Key << "PlayerPrefs";

		{
			out << YAML::BeginMap; // I32
			out << YAML::Key << "I32" << YAML::BeginSeq;
			for (const auto& [key, value] : m_I32Data)
			{
				out << YAML::BeginMap;

				VX_SERIALIZE_PROPERTY(Key, key, out);
				VX_SERIALIZE_PROPERTY(Value, value, out);

				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap; // I32
		}

		{
			out << YAML::BeginMap; // String
			out << YAML::Key << "String" << YAML::BeginSeq;
			for (const auto& [key, value] : m_StringData)
			{
				out << YAML::BeginMap;

				VX_SERIALIZE_PROPERTY(Key, key, out);
				VX_SERIALIZE_PROPERTY(Value, value, out);

				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap; // String
		}

		out << YAML::EndMap; // Player Prefs

		const Fs::Path path = Project::GetAssetDirectory();
		const Fs::Path playerPrefsPath = path / filename;
		
		std::ofstream fout(playerPrefsPath);
		if (!fout.is_open())
		{
			VX_CONSOLE_LOG_ERROR("[Script Registry] Failed to save Player Prefs!");
			return false;
		}

		fout << out.c_str();
		fout.close();

		return true;
	}

	bool PlayerPrefsSerializer::Load(const std::string& filename)
	{
		VX_CORE_ASSERT(false, "not implemented yet!");

		const Fs::Path assetDirectory = Project::GetAssetDirectory();
		const Fs::Path playerPrefsPath = assetDirectory / filename;

		if (!FileSystem::Exists(playerPrefsPath)) {
			return true; // can't load prefs if they weren't saved
		}
		
		const YAML::Node data = YAML::LoadFile(playerPrefsPath.string());

		const YAML::Node playerPrefsData = data["PlayerPrefs"];
		if (!playerPrefsData)
		{
			VX_CONSOLE_LOG_ERROR("[Script Registry] Player Prefs was corrupted!");
			return false;
		}

		const YAML::Node i32Data = playerPrefsData["I32"];
		for (auto entry : i32Data) {
			const std::string key = entry["Key"].as<std::string>();
			const int32_t value = entry["Value"].as<int32_t>();
			WriteInt(key, value);
		}

		const YAML::Node stringData = playerPrefsData["String"];
		for (auto entry : stringData) {
			const std::string key = entry["Key"].as<std::string>();
			const std::string value = entry["Value"].as<std::string>();
			WriteString(key, value);
		}

		return true;
	}

	void PlayerPrefsSerializer::RemoveKey(const std::string& key)
	{
		if (m_ByteData.contains(key)) {
			m_ByteData.erase(key);
			return;
		}

		if (m_F32Data.contains(key)) {
			m_F32Data.erase(key);
			return;
		}
		if (m_F64Data.contains(key)) {
			m_F64Data.erase(key);
			return;
		}

		if (m_I8Data.contains(key)) {
			m_I8Data.erase(key);
			return;
		}
		if (m_I16Data.contains(key)) {
			m_I16Data.erase(key);
			return;
		}
		if (m_I32Data.contains(key)) {
			m_I32Data.erase(key);
			return;
		}
		if (m_I64Data.contains(key)) {
			m_I64Data.erase(key);
			return;
		}

		if (m_U8Data.contains(key)) {
			m_U8Data.erase(key);
			return;
		}
		if (m_U16Data.contains(key)) {
			m_U16Data.erase(key);
			return;
		}
		if (m_U32Data.contains(key)) {
			m_U32Data.erase(key);
			return;
		}
		if (m_U64Data.contains(key)) {
			m_U64Data.erase(key);
			return;
		}

		if (m_StringData.contains(key)) {
			m_StringData.erase(key);
			return;
		}
	}

	bool PlayerPrefsSerializer::NoData() const
	{
		if (!m_ByteData.empty())
			return false;

		if (!m_F32Data.empty())
			return false;
		if (!m_F64Data.empty())
			return false;

		if (!m_I8Data.empty())
			return false;
		if (!m_I16Data.empty())
			return false;
		if (!m_I32Data.empty())
			return false;
		if (!m_I64Data.empty())
			return false;

		if (!m_U8Data.empty())
			return false;
		if (!m_U16Data.empty())
			return false;
		if (!m_U32Data.empty())
			return false;
		if (!m_U64Data.empty())
			return false;

		if (!m_StringData.empty())
			return false;

		return true;
	}

}
