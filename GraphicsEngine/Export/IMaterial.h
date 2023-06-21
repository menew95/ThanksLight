#pragma once

#include "Export/ResourceBase.h"
#include "../../DXTK/Inc/SimpleMath.h"

namespace TLGraphicsEngine
{
	class Shader;
	class ITexture;
	class IMaterial : 
		public ResourceBase
	{
	public:
		IMaterial() {}
		IMaterial(const char* name) : ResourceBase(eResourceType::Material, name) {}

		virtual ~IMaterial() {}

	public:
		virtual bool SetShader(std::shared_ptr<Shader> pixelShader) abstract;
		virtual void SetColor(const char* name, DirectX::SimpleMath::Color value) abstract;
		virtual void SetFloat(const char* name, float value) abstract;
		virtual void SetInt(const char* name, int value) abstract;
		virtual void SetTexture(const char* name, std::shared_ptr<ITexture> texture) abstract;
		virtual void SetTexture(const char* name, UID texture) abstract;
		virtual void SetVector(const char* name, DirectX::SimpleMath::Vector4 value) abstract;
		virtual void SetAlbedoColor(DirectX::SimpleMath::Color value) abstract;
		virtual void SetEmissiveColor(DirectX::SimpleMath::Color value) abstract;


		// ÁË¾ÇÀ» Áû¾îÁø ¹öÆÛ
		virtual void SetConstBuffer(void* constBuffer, int size) abstract;
		virtual void RemoveConstBuffer() abstract;
	};
}