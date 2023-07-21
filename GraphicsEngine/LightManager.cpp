#include "pch.h"
#include "LightManager.h"
#include "Light.h"
#include "GraphicsEngine.h"
#include "GraphicResourceManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "RenderQueue.h"
#include "ConstBufferDataInfo.h"
#include "ConstBufferManager.h"

int TLGraphicsEngine::LightManager::s_iIndex = 0;

TLGraphicsEngine::LightManager::LightManager()
{

}

TLGraphicsEngine::LightManager::~LightManager()
{
	for (auto& item : m_LightMap)
	{
		if (item.second != nullptr)
		{
			delete item.second;
			item.second = nullptr;
		}
	}
}

void TLGraphicsEngine::LightManager::Initialize()
{
	m_Shadow_Static_Mesh_VS = GraphicResourceManager::Instance()->GetShaderRef("Shadow_VS_Static_Mesh");
	m_Shadow_Skin_Mesh_VS = GraphicResourceManager::Instance()->GetShaderRef("Shadow_VS_Skin_Mesh");
	m_Shadow_PS = GraphicResourceManager::Instance()->GetShaderRef("Shadow_PS");
}

void TLGraphicsEngine::LightManager::UpdateShadowMap(Light* light, std::vector<RenderObject>& renderObjects)
{
	auto* deviceContext = GraphicsEngine::Instance()->GetDeviceContext();

	deviceContext->RSSetState(GraphicsEngine::Instance()->GetShadowMapRS());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
#ifdef _DEBUG
	deviceContext->PSSetShader(m_Shadow_PS->GetShader<ID3D11PixelShader>(), NULL, 0);
#else
	deviceContext->PSSetShader(NULL, NULL, 0);
#endif // _DEBUG

	light->BindShadowMap(deviceContext);

	Shader* vs = nullptr;

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	std::vector<CBBase*> cbs = {
	};
	auto lightViewProj = light->GetViewProjTM();

	for (size_t i = 0; i < renderObjects.size(); i++)
	{
		cbs.clear();
		XMMATRIX world = XMLoadFloat4x4(&renderObjects[i].m_World);

		PerObject_CB _perObjectCB;

		XMStoreFloat4x4(&_perObjectCB.worldViewProj, world * lightViewProj);

		switch (renderObjects[i].m_RenderType)
		{
			case RenderObject::RenderType::Mesh:
			{
				vs = m_Shadow_Static_Mesh_VS.get();

				break;
			}
			case RenderObject::RenderType::SkinnedMesh:
			{
				vs = m_Shadow_Skin_Mesh_VS.get();

				memcpy(_perObjectCB.boneTransforms, renderObjects[i].m_BoneTMList, sizeof(XMFLOAT4X4) * renderObjects[i].m_Size);

				break;
			}
			case RenderObject::RenderType::ProjectedMesh:
			case RenderObject::RenderType::OutLine:
			{
				continue;
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}

		cbs.push_back(&_perObjectCB);
		vs->Update(cbs.data(), nullptr);
		deviceContext->VSSetShader(vs->GetShader<ID3D11VertexShader>(), nullptr, 0);
		deviceContext->PSSetShader(nullptr, nullptr, 0);

		auto _indexBuffers = renderObjects[i].m_Mesh->GetIndexBuffers();

		for (size_t index = 0; index < _indexBuffers.size(); index++)
		{
			renderObjects[i].m_Mesh->Bind(deviceContext, (int)index);

			deviceContext->DrawIndexed(_indexBuffers[index]->GetIndexCount(), 0, 0);
		}
	}
}

void TLGraphicsEngine::LightManager::SetLightEnable(int id, bool enable)
{
	auto item = m_LightMap.find(id);

	if (item == m_LightMap.end())
	{
#ifdef _DEBUG
		assert(false);
#endif // _DEBUG
		return;
	}

	item->second->m_enable = enable;
}

void TLGraphicsEngine::LightManager::UpdateLight(int id, int type, float range, float fallOff, float spotAngle, float fallOffAngle, float power, XMFLOAT3 pos, XMFLOAT3 dir, XMFLOAT3 color)
{
	auto item = m_LightMap.find(id);

	if (item == m_LightMap.end())
	{
#ifdef _DEBUG
		assert(false);
#endif // _DEBUG
		return;
	}

	Light* light = item->second;

	if (type != (int)light->m_eLightType)
	{
		light->m_eLightType = (LightType)type;
		light->BuildShadowMap();
	}

	light->m_range = range;
	light->m_fallOff = fallOff;
	light->m_spotAngle = spotAngle;
	light->m_fallOffAngle = fallOffAngle;
	light->m_power = power;
	light->m_lightPosition = pos;
	light->m_lightDirection = dir;
	light->m_color = color;

	//if (light->m_eLightType == LightType::Directional)
	//{
	//	// 직사광일경우 그림자 맵의 TM이 이동해야 함
	//	light->m_Center = ????; 
	//}
}

void TLGraphicsEngine::LightManager::UpdateLightBuffer(Light_CB* lightCB, std::vector<ID3D11ShaderResourceView**>& srvs)
{
	lightCB->LightCount = static_cast<int>(m_lightQueue.size());
	
	int index = 0;

	while (!m_lightQueue.empty())
	{	
		Light* light = m_lightQueue.front();
		m_lightQueue.pop();

		lightCB->LightInfo[index].m_iType = static_cast<int>(light->m_eLightType);
		lightCB->LightInfo[index].m_fRange = light->m_range;
		lightCB->LightInfo[index].m_fFallOff = light->m_fallOff;
		lightCB->LightInfo[index].m_fSpotAngle = cos(light->m_spotAngle * 0.5f);
		lightCB->LightInfo[index].m_fFallOffAngle = cos(light->m_fallOffAngle * 0.5f);
		lightCB->LightInfo[index].m_fPower = light->m_power;
		lightCB->LightInfo[index].m_Position = light->m_lightPosition;
		lightCB->LightInfo[index].m_Direction = light->m_lightDirection;
		lightCB->LightInfo[index].m_Color = light->m_color;
		DirectX::XMStoreFloat4x4(&lightCB->LightInfo[index].m_ShadowTransform, light->m_shadowTM);

		srvs.push_back(light->GetShadowMapRef());
		
		index++;
	}
}

int TLGraphicsEngine::LightManager::AddLight()
{
	Light* newLight = new Light;
	
	m_LightMap.insert(std::make_pair(s_iIndex, newLight));

	return s_iIndex++;
}

int TLGraphicsEngine::LightManager::RegistDirectionalLight(XMFLOAT3 color, float intensity, XMFLOAT3 direction)
{
	Light* newLight = new Light;

	newLight->m_eLightType = LightType::Directional;
	newLight->m_color = color;
	newLight->m_power = intensity;
	newLight->m_lightDirection = direction;

	m_LightMap.insert(std::make_pair(s_iIndex, newLight));

	return s_iIndex++;
}

int TLGraphicsEngine::LightManager::RegistPointLight(XMFLOAT3 color, float intensity, float range, XMFLOAT3 position)
{
	Light* newLight = new Light;

	newLight->m_eLightType = LightType::Point;
	newLight->m_color = color;
	newLight->m_power = intensity;
	newLight->m_range = range;
	newLight->m_lightPosition = position;

	m_LightMap.insert(std::make_pair(s_iIndex, newLight));

	return s_iIndex++;
}

int TLGraphicsEngine::LightManager::RegistSpotLight(XMFLOAT3 color, float intensity, float range, float spotAngle, XMFLOAT3 position, XMFLOAT3 direction)
{
	Light* newLight = new Light;

	newLight->m_eLightType = LightType::Spot;
	newLight->m_color = color;
	newLight->m_power = intensity;
	newLight->m_range = range;
	newLight->m_spotAngle = spotAngle;
	newLight->m_lightPosition = position;
	newLight->m_lightDirection = direction;

	m_LightMap.insert(std::make_pair(s_iIndex, newLight));

	return s_iIndex++;
}

void TLGraphicsEngine::LightManager::CalcLight(std::vector<RenderObject>& renderObjects)
{
	for (auto& item : m_LightMap)
	{
		for (auto& obj : renderObjects)
		{
			if (item.second->CheckLight(&obj))
			{
				m_lightQueue.push(item.second);

				if (item.second->m_eLightType != LightType::Point &&
					item.second->m_eLightType != LightType::BlackSpotLight)
				{
					UpdateShadowMap(item.second, renderObjects);
				}
				break;
			}
		}
	}
}

void TLGraphicsEngine::LightManager::DeleteLight(int id)
{
	auto item = m_LightMap.find(id);

	if (item != m_LightMap.end())
	{
		delete item->second;
		m_LightMap.erase(id);
	}
}

void TLGraphicsEngine::LightManager::ClearLight()
{
	for (auto light : m_LightMap)
	{
		if (light.second != nullptr)
		{
			delete light.second;
		}
	}

	m_LightMap.clear();
	s_iIndex = 0;
}

void TLGraphicsEngine::LightManager::BindShadowMap(std::vector<ID3D11ShaderResourceView**>& srvs)
{

}

TLGraphicsEngine::DepthStencilView* TLGraphicsEngine::LightManager::GetDepthBuffer()
{
	return m_lightQueue.front()->GetDepthBuffer();
}
