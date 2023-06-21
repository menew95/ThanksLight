#pragma once

#include <map>

#include "Export/ResourceBase.h"

/// <summary>
/// LightManager
/// Light 가 현재 구성되는 오브젝트들에 영향을 끼치는지 계산
/// </summary>
namespace TLGraphicsEngine
{
	class Shader;
	class Light;
	struct RenderObject;
	struct Light_CB;
	struct PerFrame_CB;
	class DepthStencilView;

	class LightManager
	{
	public:
		LightManager();
		~LightManager();

	private:
		std::map<int, Light*> m_LightMap;

		std::queue<Light*> m_lightQueue;

		static int s_iIndex;

		shared_ptr<Shader> m_Shadow_Static_Mesh_VS;
		shared_ptr<Shader> m_Shadow_Skin_Mesh_VS;
		shared_ptr<Shader> m_Shadow_PS;

	private:
		void UpdateShadowMap(Light* light, std::vector<RenderObject>& renderObjects);
	
	public:
		void Initialize();
		
		void SetLightEnable(int id, bool enable);
		void UpdateLight(int id, int type, float range, float fallOff, float spotAngle, float fallOffAngle, float power, XMFLOAT3 pos, XMFLOAT3 dir, XMFLOAT3 color);
		void UpdateLightBuffer(Light_CB* lightCB, std::vector<ID3D11ShaderResourceView**>& srvs);

		int AddLight();
		int RegistDirectionalLight(XMFLOAT3 color, float intensity, XMFLOAT3 direction);
		int RegistPointLight(XMFLOAT3 color, float intensity, float range, XMFLOAT3 position);
		int RegistSpotLight(XMFLOAT3 color, float intensity, float range, float spotAngle, XMFLOAT3 position, XMFLOAT3 direction);

		void CalcLight(std::vector<RenderObject>& renderObjects);
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="id"></param>
		void DeleteLight(int id);

		void ClearLight();

		void BindShadowMap(std::vector<ID3D11ShaderResourceView**>& srvs);
		DepthStencilView* GetDepthBuffer();
	};
}