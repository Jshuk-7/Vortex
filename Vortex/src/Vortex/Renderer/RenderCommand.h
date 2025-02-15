#pragma once

#include "Vortex/Core/Base.h"

#include "Vortex/Renderer/RendererAPI.h"

namespace Vortex {

	class VORTEX_API RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewport(const Viewport& viewport)
		{
			s_RendererAPI->SetViewport(viewport);
		}

		inline static void SetClearColor(const Math::vec3& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void SetWireframe(bool enabled)
		{
			s_RendererAPI->SetWireframeMode(enabled);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawTriangles(const SharedReference<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			s_RendererAPI->DrawTriangles(vertexArray, vertexCount);
		}

		inline static void DrawIndexed(const SharedReference<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void DrawLines(const SharedReference<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		}

		inline static void DrawTriangleStrip(const SharedReference<VertexArray>& vertexArray, uint32_t indexCount)
		{
			s_RendererAPI->DrawTriangleStrip(vertexArray, indexCount);
		}

		inline static void EnableDepthTest()
		{
			s_RendererAPI->EnableDepthTest();
		}

		inline static void DisableDepthTest()
		{
			s_RendererAPI->DisableDepthTest();
		}

		inline static void EnableStencilTest()
		{
			s_RendererAPI->EnableStencilTest();
		}

		inline static void DisableStencilTest()
		{
			s_RendererAPI->DisableStencilTest();
		}

		inline static void EnableDepthMask()
		{
			s_RendererAPI->EnableDepthMask();
		}

		inline static void DisableDepthMask()
		{
			s_RendererAPI->DisableDepthMask();
		}

		inline static void SetBlendMode(RendererAPI::BlendMode blendMode)
		{
			s_RendererAPI->SetBlendMode(blendMode);
		}

		inline static void SetLineSize(float thickness)
		{
			s_RendererAPI->SetLineWidth(thickness);
		}

		inline static void SetStencilOperation(RendererAPI::StencilOperation failOp, RendererAPI::StencilOperation zFailOp, RendererAPI::StencilOperation passOp)
		{
			s_RendererAPI->SetStencilOperation(failOp, zFailOp, passOp);
		}

		inline static void SetStencilFunc(RendererAPI::StencilOperation func, int ref, int mask)
		{
			s_RendererAPI->SetStencilFunc(func, ref, mask);
		}

		inline static void SetStencilMask(int mask)
		{
			s_RendererAPI->SetStencilMask(mask);
		}

		inline static void SetCullMode(RendererAPI::TriangleCullMode cullMode)
		{
			s_RendererAPI->SetCullMode(cullMode);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}
