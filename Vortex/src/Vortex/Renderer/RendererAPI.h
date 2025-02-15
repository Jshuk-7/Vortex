#pragma once

#include "Vortex/Core/Base.h"

#include "Vortex/Math/Math.h"

#include "Vortex/Renderer/VertexArray.h"

namespace Vortex {

#ifndef VX_DIST
	#define VX_ENABLE_RENDER_STATISTICS 1;
#else
	#define VX_ENABLE_RENDER_STATISTICS 0;
#endif // VX_DIST

	static constexpr uint32_t VERTICES_PER_QUAD = 4;
	static constexpr uint32_t INDICES_PER_QUAD = 6;

	struct VORTEX_API RenderStatistics
	{
		uint32_t DrawCalls;
		uint32_t QuadCount;
		uint32_t LineCount;

		uint32_t GetTriangleCount() const { return QuadCount * 2; }
		uint32_t GetVertexCount() const { return (QuadCount * VERTICES_PER_QUAD) + (LineCount * 2); }
		uint32_t GetIndexCount() const { return QuadCount * INDICES_PER_QUAD + (LineCount * 2); }
	};

	struct VORTEX_API ViewportBounds
	{
		Math::vec2 MinBound;
		Math::vec2 MaxBound;

		ViewportBounds(const Math::vec2& minBound, const Math::vec2& maxBound)
			: MinBound(minBound), MaxBound(maxBound) { }

		ViewportBounds()
			: MinBound(Math::vec2(0.0f)), MaxBound(Math::vec2(0.0f)) { }

		Math::vec2 CalculateViewportSize() const { return Math::vec2(MaxBound - MinBound); }
	};

	struct VORTEX_API Viewport
	{
		uint32_t TopLeftXPos = 0;
		uint32_t TopLeftYPos = 0;
		uint32_t Width = 0;
		uint32_t Height = 0;

		ViewportBounds Bounds;

		Math::vec2 GetSize() const { return Math::vec2((float)Width, (float)Height); }
	};

	class VORTEX_API RendererAPI
	{
	public:
		enum class VORTEX_API API
		{
			None = 0, OpenGL = 1, Direct3D = 2, Vulkan = 3
		};

		enum class VORTEX_API TriangleCullMode
		{
			None = 0, Front, Back, FrontAndBack
		};

		enum class VORTEX_API StencilOperation
		{
			None = 0, Keep, Always, NotEqual, Replace
		};

		enum class VORTEX_API BlendMode
		{
			None = 0, SrcAlphaOneMinusSrcAlpha, ZeroSrcColor
		};

		struct VORTEX_API Info
		{
			const char* Name;
			const unsigned char* GPU;
			const unsigned char* Vendor;
			const unsigned char* Version;
			const unsigned char* ShadingLanguageVersion;
		};

	public:
		virtual void Init() const = 0;

		virtual void SetViewport(const Viewport& viewport) const = 0;

		virtual void SetClearColor(const Math::vec3& color) const = 0;

		virtual void SetWireframeMode(bool enabled) const = 0;

		virtual void Clear() const = 0;

		virtual void DrawTriangles(const SharedReference<VertexArray>& vertexArray, uint32_t vertexCount) const = 0;
		virtual void DrawIndexed(const SharedReference<VertexArray>& vertexArray, uint32_t indexCount = 0) const = 0;
		virtual void DrawLines(const SharedReference<VertexArray>& vertexArray, uint32_t vertexCount) const = 0;
		virtual void DrawTriangleStrip(const SharedReference<VertexArray>& vertexArray, uint32_t indexCount) const = 0;

		virtual void EnableDepthTest() const = 0;
		virtual void DisableDepthTest() const = 0;

		virtual void EnableStencilTest() const = 0;
		virtual void DisableStencilTest() const = 0;

		virtual void EnableDepthMask() const = 0;
		virtual void DisableDepthMask() const = 0;

		virtual void SetBlendMode(BlendMode blendMode) const = 0;

		virtual void SetLineWidth(float thickness) const = 0;

		virtual void SetCullMode(TriangleCullMode cullMode) const = 0;

		virtual void SetStencilOperation(StencilOperation failOperation, StencilOperation zFailOperation, StencilOperation passOperation) const = 0;
		virtual void SetStencilFunc(StencilOperation func, int ref, int mask) const = 0;
		virtual void SetStencilMask(int mask) const = 0;

		inline static API GetAPI() { return s_API; }
		inline static void SetAPI(API api) { s_API = api; }

		inline static const Info& GetAPIInfo() { return s_RendererInfo; }
		inline static void SetAPIInfo(const Info& info) { s_RendererInfo = info; }

	private:
		static API s_API;
		static Info s_RendererInfo;
	};

	namespace Utils {

		static const char* TriangleCullModeToString(RendererAPI::TriangleCullMode cullMode)
		{
			switch (cullMode)
			{
				case Vortex::RendererAPI::TriangleCullMode::None:         return "None";
				case Vortex::RendererAPI::TriangleCullMode::Front:        return "Front";
				case Vortex::RendererAPI::TriangleCullMode::Back:         return "Back";
				case Vortex::RendererAPI::TriangleCullMode::FrontAndBack: return "Front And Back";
			}

			VX_CORE_ASSERT(false, "Unknown Triangle Cull Mode!");
			return "None";
		}

		static RendererAPI::TriangleCullMode TriangleCullModeFromString(const std::string& cullMode)
		{
			if (cullMode == "None") return RendererAPI::TriangleCullMode::None;
			if (cullMode == "Front") return RendererAPI::TriangleCullMode::Front;
			if (cullMode == "Back") return RendererAPI::TriangleCullMode::Back;
			if (cullMode == "FrontAndBack") return RendererAPI::TriangleCullMode::FrontAndBack;

			VX_CORE_ASSERT(false, "Unknown Triangle Cull Mode!");
			return RendererAPI::TriangleCullMode::None;
		}

	}

}
