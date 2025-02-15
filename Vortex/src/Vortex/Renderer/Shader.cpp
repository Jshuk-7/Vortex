#include "vxpch.h"
#include "Shader.h"

#include "Vortex/Renderer/Renderer.h"
#include "Vortex/Platform/OpenGL/OpenGLShader.h"

/*
*** #ifdef VX_PLATFORM_WINDOWS
***		#include "Platform/Direct3D/Direct3DShader.h"
*** #endif // VX_PLATFORM_WINDOWS
*/

namespace Vortex {

	SharedReference<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetGraphicsAPI())
		{
			case RendererAPI::API::None:     VX_CORE_ASSERT(false, "Renderer API was set to RendererAPI::None!"); return nullptr;
			case RendererAPI::API::OpenGL:   return SharedReference<OpenGLShader>::Create(filepath);
#ifdef VX_PLATFORM_WINDOWS
			case RendererAPI::API::Direct3D: return nullptr;
#endif // VX_PLATFORM_WINDOWS
			case RendererAPI::API::Vulkan:   return nullptr;
		}

		VX_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	SharedReference<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc)
	{
		switch (Renderer::GetGraphicsAPI())
		{
			case RendererAPI::API::None:     VX_CORE_ASSERT(false, "Renderer API was set to RendererAPI::None!"); return nullptr;
			case RendererAPI::API::OpenGL:   return SharedReference<OpenGLShader>::Create(name, vertexSrc, fragmentSrc, geometrySrc);
#ifdef VX_PLATFORM_WINDOWS
			case RendererAPI::API::Direct3D: return nullptr;
#endif // VX_PLATFORM_WINDOWS
			case RendererAPI::API::Vulkan:   return nullptr;
		}

		VX_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	void ShaderLibrary::Add(const Key& name, const Value& shader)
	{
		VX_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_LibraryData[name] = shader;
	}

	void ShaderLibrary::Add(const Value& shader)
	{
		Add(shader->GetName(), shader);
	}

	uint8_t ShaderLibrary::Remove(const Key& name)
	{
		VX_CORE_ASSERT(Exists(name), "Unknown shader name!");

		if (!Exists(name))
		{
			return 1;
		}

		m_LibraryData.erase(name);
		return 0;
	}

	const ShaderLibrary::Value& ShaderLibrary::Get(const Key& identifier) const
	{
		return Get(identifier);
	}

	ShaderLibrary::Value& ShaderLibrary::Get(const Key& identifier)
	{
		VX_CORE_ASSERT(Exists(identifier), "Invalid identifier!");
		return m_LibraryData.find(identifier)->second;
	}

	SharedReference<Shader> ShaderLibrary::Load(const Key& filepath)
	{
		SharedReference<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	SharedReference<Shader> ShaderLibrary::Load(const std::string& name, const Key& filepath)
	{
		SharedReference<Shader> shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

}