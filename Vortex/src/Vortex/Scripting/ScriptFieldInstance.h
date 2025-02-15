#pragma once

#include "Vortex/Core/Base.h"

#include "Vortex/Scripting/ScriptField.h"

namespace Vortex {

#define VX_SCRIPT_FIELD_MAX_BYTES 16

	struct VORTEX_API ScriptFieldInstance
	{
		ScriptField Field;

		ScriptFieldInstance()
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}
		~ScriptFieldInstance() = default;

		template <typename TFieldType>
		TFieldType GetValue() const
		{
			static_assert(sizeof(TFieldType) <= VX_SCRIPT_FIELD_MAX_BYTES, "Type too large!");
			return *(TFieldType*)m_Buffer;
		}

		template <typename TFieldType>
		void SetValue(TFieldType value)
		{
			static_assert(sizeof(TFieldType) <= VX_SCRIPT_FIELD_MAX_BYTES, "Type too large!");
			memcpy(m_Buffer, &value, sizeof(TFieldType));
		}

		const void* GetDataBuffer() const
		{
			return (const void*)m_Buffer;
		}

		constexpr size_t BufferSize() const { return VX_SCRIPT_FIELD_MAX_BYTES; }

	private:
		uint8_t m_Buffer[VX_SCRIPT_FIELD_MAX_BYTES];
	};

}
