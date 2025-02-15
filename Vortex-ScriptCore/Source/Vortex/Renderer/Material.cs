﻿namespace Vortex {

	public enum MaterialFlag
	{
		None = 0,
		NoDepthTest = 1 << 1,
	}

	public class Material
	{
		internal AssetHandle m_Handle;
		public AssetHandle Handle => m_Handle;

		internal Material() { m_Handle = AssetHandle.Invalid; }
		internal Material(AssetHandle assetHandle)
		{
			m_Handle = assetHandle;
		}

		public Vector3 Albedo
		{
			get
			{
				InternalCalls.Material_GetAlbedo(ref m_Handle, out Vector3 result);
				return result;
			}

			set => InternalCalls.Material_SetAlbedo(ref m_Handle, ref value);
		}

		public float Metallic
		{
			get => InternalCalls.Material_GetMetallic(ref m_Handle);
			set => InternalCalls.Material_SetMetallic(ref m_Handle, value);
		}

		public float Roughness
		{
			get => InternalCalls.Material_GetRoughness(ref m_Handle);
			set => InternalCalls.Material_SetRoughness(ref m_Handle, value);
		}

		public float Emission
		{
			get => InternalCalls.Material_GetEmission(ref m_Handle);
			set => InternalCalls.Material_SetEmission(ref m_Handle, value);
		}

		public Vector2 UV
		{
			get
			{
				InternalCalls.Material_GetUV(ref m_Handle, out Vector2 result);
				return result;
			}

			set => InternalCalls.Material_SetUV(ref m_Handle, ref value);
		}

		public float Opacity
		{
			get => InternalCalls.Material_GetOpacity(ref m_Handle);
			set => InternalCalls.Material_SetOpacity(ref m_Handle, value);
		}

		public bool HasTexture(string name) => InternalCalls.Material_HasTexture(ref m_Handle, name);

		public Texture2D GetTexture(string name)
		{
			return InternalCalls.Material_GetTexture(ref m_Handle, name, out AssetHandle textureHandle)
					? new Texture2D(textureHandle) : null;
		}

		public void SetTexture(string name, Texture2D texture)
		{
			AssetHandle textureHandle = texture.Handle;
			InternalCalls.Material_SetTexture(ref m_Handle, name, ref textureHandle);
		}

		public bool IsFlagSet(MaterialFlag flag) => InternalCalls.Material_IsFlagSet(ref m_Handle, flag);
		public void SetFlag(MaterialFlag flag, bool value) => InternalCalls.Material_SetFlag(ref m_Handle, flag, value);
	}

}
