#pragma once

namespace Engine
{
	enum class ShaderDataType : uint8_t
	{
		None = 0,
		Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case Engine::ShaderDataType::Float:
			return sizeof(float);
		case Engine::ShaderDataType::Float2:
			return sizeof(float) * 2;
		case Engine::ShaderDataType::Float3:
			return sizeof(float) * 3;
		case Engine::ShaderDataType::Float4:
			return sizeof(float) * 4;
		case Engine::ShaderDataType::Mat3:
			return sizeof(float) * 3 * 3;
		case Engine::ShaderDataType::Mat4:
			return sizeof(float) * 4 * 4;
		case Engine::ShaderDataType::Int:
			return sizeof(int);
		case Engine::ShaderDataType::Int2:
			return sizeof(int) * 2;
		case Engine::ShaderDataType::Int3:
			return sizeof(int) * 3;
		case Engine::ShaderDataType::Int4:
			return sizeof(int) * 4;
		case Engine::ShaderDataType::Bool:
			return sizeof(bool);
		}
		EG_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, const std::string& name, bool normalize = false)
			:Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalize)
		{
		}
		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case Engine::ShaderDataType::Float:
				return 1;
			case Engine::ShaderDataType::Float2:
				return 2;
			case Engine::ShaderDataType::Float3:
				return 3;
			case Engine::ShaderDataType::Float4:
				return 4;
			case Engine::ShaderDataType::Mat3:
				return 3 * 3;
			case Engine::ShaderDataType::Mat4:
				return 4 * 4;
			case Engine::ShaderDataType::Int:
				return 1;
			case Engine::ShaderDataType::Int2:
				return 2;
			case Engine::ShaderDataType::Int3:
				return 3;
			case Engine::ShaderDataType::Int4:
				return 4;
			case Engine::ShaderDataType::Bool:
				return 1;
			}
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			:m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		BufferLayout() {}
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		uint32_t GetStride() const { return m_stride; }
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};
}
