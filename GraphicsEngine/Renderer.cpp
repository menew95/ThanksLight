#include "pch.h"
#include "Renderer.h"
#include "GraphicsEngine.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "RenderQueue.h"
#include "EngineCamera.h"

#include "GraphicResourceManager.h"
#include "RenderTargetView.h"
#include "ConstBufferManager.h"

TLGraphicsEngine::Renderer* TLGraphicsEngine::Renderer::m_instance = nullptr;

TLGraphicsEngine::Renderer::Renderer()
: m_engineCamera(nullptr)
{

}

TLGraphicsEngine::Renderer::~Renderer()
{
	
}


TLGraphicsEngine::Renderer* TLGraphicsEngine::Renderer::Instance()
{
	if (m_instance == nullptr)
	{
		m_instance = new TLGraphicsEngine::Renderer;
	}

	return m_instance;
}

void TLGraphicsEngine::Renderer::Release()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

void TLGraphicsEngine::Renderer::Initialize(EngineCamera* camera, shared_ptr<Shader> staticVS, shared_ptr<Shader> skinVS, shared_ptr<Shader> quardVS, shared_ptr<Shader> texturePS, shared_ptr<Shader> gizmoVS, shared_ptr<Shader> gizmoPS)
{
	m_engineCamera = camera;
	m_staticVS = staticVS;
	m_skinVS = skinVS;
	m_quardVS = quardVS;
	m_texturePS = texturePS;
	m_gizmoVS = gizmoVS;
	m_gizmoPS = gizmoPS;

	m_outLineVS = GraphicResourceManager::Instance()->GetShaderRef("OutLineMesh_VS");
	m_outLinePS = GraphicResourceManager::Instance()->GetShaderRef("OutLineMesh_PS");
}

void TLGraphicsEngine::Renderer::RenderMesh(ID3D11DeviceContext* deviceContext, RenderObject* renderObject)
{
	if (renderObject == nullptr)
	{
		return;
	}

	m_staticVS->Bind(deviceContext);

	XMMATRIX world = XMLoadFloat4x4(&renderObject->m_World);
	XMMATRIX worldInv = XMMatrixTranspose(XMMatrixInverse(nullptr, world));
	XMMATRIX worldView = world * m_engineCamera->GetViewTM();
	XMMATRIX worldViewProj = worldView * m_engineCamera->GetProjTM();
	static const XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	PerObject_CB perObjectCB;
	perObjectCB.world = renderObject->m_World;
	XMStoreFloat4x4(&perObjectCB.worldInvTranspose, worldInv);
	XMStoreFloat4x4(&perObjectCB.worldView, worldView);
	XMStoreFloat4x4(&perObjectCB.worldViewProj, worldViewProj);
	XMStoreFloat4x4(&perObjectCB.texTransform, T);

	std::vector<CBBase*> cbs = {
		&perObjectCB
	};

	m_staticVS->Update(cbs.data(), nullptr);
	
	// 입력 배치 객체 셋팅
	deviceContext->IASetInputLayout(m_staticVS->GetInputLayout());
	// 렌더스테이트
	deviceContext->RSSetState((renderObject->m_Mesh->m_Renderstate.Get() != nullptr)
		? renderObject->m_Mesh->m_Renderstate.Get()
		: GraphicsEngine::Instance()->GetSolidRS());


	auto _indexBuffer = renderObject->m_Mesh->GetIndexBuffers();

	for (size_t i = 0; i < _indexBuffer.size(); i++)
	{
		if (_indexBuffer[i]->GetIndexCount() < 1)
		{
			continue;
		}

		Material* _material;

		if (i < renderObject->m_Mats.size())
		{
			_material = renderObject->m_Mats[i];
		}
		else if(renderObject->m_Mats.size() != 0)
		{
			_material = renderObject->m_Mats.back();
		}
		else
		{
			assert(false);
			return;
		}

		renderObject->m_Mesh->Bind(deviceContext, (int)i);

		auto ps = _material->GetShader();

		if (ps == nullptr)
		{
			// 쉐이더가 설정이 안된 이상한 머테리얼이 들어옴
			//assert(false);
			continue;
		}

		PerMaterial_CB perMaterialCB;
		perMaterialCB.material.diffuse = _material->m_MaterialCB.Diffuse;
		perMaterialCB.material.emissive = _material->m_MaterialCB.Emissive;
		perMaterialCB.material.metallic = _material->m_fMetallic;
		perMaterialCB.material.roughness = _material->m_fRoughness;
		perMaterialCB.material.m_tiling = _material->m_tiling;
		perMaterialCB.material.isUseAlbedo = _material->m_SetAlbedoMap;
		perMaterialCB.material.isUseBump = _material->m_SetBumpMap;

		std::vector<ID3D11ShaderResourceView**> srvs;

		if (_material->m_SetAlbedoMap)
		{
			srvs.push_back(_material->m_mainTex->GetSRVR());
		}
		else
		{
			srvs.push_back(nullptr);
		}
		if (_material->m_SetBumpMap)
		{
			srvs.push_back(_material->m_bumpTex->GetSRVR());
		}
		else
		{
			srvs.push_back(nullptr);
		}

		std::vector<CBBase*> cb2 = {
			&perMaterialCB
		};

		if (_material->m_setConstBuffer)
		{
			cb2.push_back(&_material->m_perCustumConstBuffer);
		}

		if (strcmp(ps->GetName(), "Deferred_PS_VantaBlack") == 0)
		{
			int a = 0;
		}

		ps->Update(cb2, srvs);

		ps->Bind(deviceContext);

		deviceContext->DrawIndexed(_indexBuffer[i]->GetIndexCount(), 0, 0);
	}
}

void TLGraphicsEngine::Renderer::RenderSkinnedMesh(ID3D11DeviceContext* deviceContext, RenderObject* renderObject)
{
	if (renderObject == nullptr)
	{
		return;
	}

	m_staticVS->Bind(deviceContext);

	XMMATRIX world = XMLoadFloat4x4(&renderObject->m_World);
	XMMATRIX worldInv = XMMatrixInverse(nullptr, world);
	XMMATRIX worldView = world * m_engineCamera->GetViewTM();
	XMMATRIX worldViewProj = worldView * m_engineCamera->GetProjTM();
	static const XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	PerObject_CB perObjectCB;
	perObjectCB.world = renderObject->m_World;
	XMStoreFloat4x4(&perObjectCB.worldInvTranspose, worldInv);
	XMStoreFloat4x4(&perObjectCB.worldView, worldView);
	XMStoreFloat4x4(&perObjectCB.worldViewProj, worldViewProj);
	XMStoreFloat4x4(&perObjectCB.texTransform, T);
	memcpy(perObjectCB.boneTransforms, renderObject->m_BoneTMList, sizeof(XMFLOAT4X4) * renderObject->m_Size);

	std::vector<CBBase*> cbs = {
		&perObjectCB
	};

	m_staticVS->Update(cbs.data(), nullptr);

	// 입력 배치 객체 셋팅
	deviceContext->IASetInputLayout(m_staticVS->GetInputLayout());
	// 렌더스테이트
	deviceContext->RSSetState((renderObject->m_Mesh->m_Renderstate.Get() != nullptr)
		? renderObject->m_Mesh->m_Renderstate.Get()
		: GraphicsEngine::Instance()->GetSolidRS());


	auto _indexBuffer = renderObject->m_Mesh->GetIndexBuffers();

	for (size_t i = 0; i < _indexBuffer.size(); i++)
	{
		if (_indexBuffer[i]->GetIndexCount() < 1)
		{
			continue;
		}
		Material* _material;
		if (i < renderObject->m_Mats.size())
		{
			_material = renderObject->m_Mats[i];
		}
		else
		{
			_material = renderObject->m_Mats.back();
		}

		renderObject->m_Mesh->Bind(deviceContext, (int)i);

		auto ps = _material->GetShader();

		if (ps == nullptr)
		{
			// 쉐이더가 설정이 안된 이상한 머테리얼이 들어옴
			assert(false);
			continue;
		}

		PerMaterial_CB perMaterialCB;
		perMaterialCB.material.diffuse = _material->m_color;
		perMaterialCB.material.emissive = _material->m_MaterialCB.Emissive;
		perMaterialCB.material.metallic = _material->m_fMetallic;
		perMaterialCB.material.roughness = _material->m_fRoughness;
		perMaterialCB.material.isUseAlbedo = _material->m_SetAlbedoMap;
		perMaterialCB.material.isUseBump = _material->m_SetBumpMap;

		std::vector<ID3D11ShaderResourceView**> srvs;

		if (_material->m_SetAlbedoMap)
		{
			srvs.push_back(_material->m_mainTex->GetSRVR());
		}
		else
		{
			srvs.push_back(nullptr);
		}
		if (_material->m_SetBumpMap)
		{
			srvs.push_back(_material->m_bumpTex->GetSRVR());
		}
		else
		{
			srvs.push_back(nullptr);
		}

		std::vector<CBBase*> cb2 = {
			&perMaterialCB
		};

		ps->Update(cb2.data(), srvs.data());

		ps->Bind(deviceContext);

		deviceContext->DrawIndexed(_indexBuffer[i]->GetIndexCount(), 0, 0);
	}
}

void TLGraphicsEngine::Renderer::Render2D(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* srv)
{
	// 렌더스테이트
	deviceContext->RSSetState(GraphicsEngine::Instance()->GetSolidRS());

	deviceContext->IASetInputLayout(m_quardVS->GetInputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(QuadVertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &RenderTargetView::m_VB, &stride, &offset);
	deviceContext->IASetIndexBuffer(RenderTargetView::m_IB, DXGI_FORMAT_R32_UINT, 0);

	ID3D11ShaderResourceView** srvs[1] = { &srv };

	m_texturePS->Update(nullptr, srvs);

	m_quardVS->Bind(deviceContext);
	m_texturePS->Bind(deviceContext);
	
	deviceContext->DrawIndexed(6, 0, 0);


	std::vector<ID3D11ShaderResourceView*> _nullSRV(1, nullptr);
	deviceContext->PSSetShaderResources(0, 1,_nullSRV.data());
}

void TLGraphicsEngine::Renderer::RenderGizmo(ID3D11DeviceContext* deviceContext, GizmoObject* gizmoObject)
{
	if (gizmoObject->m_Mesh == nullptr)
	{
		return;
	}

	auto indexBuffers = gizmoObject->m_Mesh->GetIndexBuffers();

	if (indexBuffers.size() < 1)
	{
		return;
	}

	// 렌더스테이트
	deviceContext->RSSetState(GraphicsEngine::Instance()->GetWireframeRS());

	// 입력 배치 객체 셋팅
	deviceContext->IASetInputLayout(m_gizmoVS->GetInputLayout());

	deviceContext->IASetPrimitiveTopology(gizmoObject->m_Mesh->m_Primitive);

	gizmoObject->m_Mesh->Bind(deviceContext, 0);
		
	XMMATRIX world = XMLoadFloat4x4(&gizmoObject->m_World);

	XMMATRIX worldViewProj = world * m_engineCamera->GetViewProjTM();

	{
		PerObject_CB cb;
		XMStoreFloat4x4(&cb.worldViewProj, worldViewProj);

		std::vector<CBBase*> cbs =
		{
			&cb
		};
		m_gizmoVS->Update(cbs.data(), nullptr);
	}

	/*{
		Gizmo gizmo;

		gizmo.color = color;

		std::vector<CBBase*> cbs =
		{
			&gizmo
		};

		m_gizmoPS->Update(cbs.data(), nullptr);
	}*/

	m_gizmoVS->Bind(deviceContext);
	m_gizmoPS->Bind(deviceContext);

	deviceContext->DrawIndexed(gizmoObject->m_Mesh->GetIndexBuffers()[0]->GetIndexCount(), 0, 0);
}

void TLGraphicsEngine::Renderer::RenderBoundingBox(ID3D11DeviceContext* deviceContext, IMesh* mesh, float WM[16])
{
	Mesh* _mesh = reinterpret_cast<Mesh*>(mesh);
	if (_mesh == nullptr)
	{
		return;
	}
	
	// 렌더스테이트
	deviceContext->RSSetState(GraphicsEngine::Instance()->GetWireframeRS());

	// 입력 배치 객체 셋팅
	deviceContext->IASetInputLayout(m_gizmoVS->GetInputLayout());

	deviceContext->IASetPrimitiveTopology(_mesh->m_BoundingBoxPrimitive);
	
	
	_mesh->BindBoundingBox(deviceContext);

	XMMATRIX worldViewProj = DirectX::XMMatrixSet(WM[0], WM[1], WM[2], WM[3], WM[4], WM[5], WM[6], WM[7]
		, WM[8], WM[9], WM[10], WM[11], WM[12], WM[13], WM[14], WM[15])
		* m_engineCamera->GetViewProjTM();


	PerObject_CB cb;
	XMStoreFloat4x4(&cb.worldViewProj, worldViewProj);

	std::vector<CBBase*> cbs =
	{
		&cb
	};

	m_gizmoVS->Update(cbs.data(), nullptr);
	m_gizmoVS->Bind(deviceContext);
	m_gizmoPS->Bind(deviceContext);
	
	deviceContext->DrawIndexed(static_cast<UINT>(_mesh->m_BoundingBoxTriangles.size()), 0, 0);
}

void TLGraphicsEngine::Renderer::RenderOutLine(ID3D11DeviceContext* deviceContext, RenderObject* renderObject)
{
	if (renderObject == nullptr)
	{
		return;
	}

	m_outLineVS->Bind(deviceContext);

	XMMATRIX world = XMLoadFloat4x4(&renderObject->m_World);
	XMMATRIX worldInv = XMMatrixTranspose(XMMatrixInverse(nullptr, world));
	XMMATRIX worldView = world * m_engineCamera->GetViewTM();
	XMMATRIX worldViewProj = worldView * m_engineCamera->GetProjTM();
	static const XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	PerObject_CB perObjectCB;
	perObjectCB.world = renderObject->m_World;
	XMStoreFloat4x4(&perObjectCB.worldInvTranspose, worldInv);
	XMStoreFloat4x4(&perObjectCB.worldView, worldView);
	XMStoreFloat4x4(&perObjectCB.worldViewProj, worldViewProj);
	XMStoreFloat4x4(&perObjectCB.texTransform, T);
	std::vector<CBBase*> cbs = {
		&perObjectCB
	};

	m_outLineVS->Update(cbs.data(), nullptr);

	// 입력 배치 객체 셋팅
	deviceContext->IASetInputLayout(m_staticVS->GetInputLayout());
	// 렌더스테이트
	deviceContext->RSSetState((renderObject->m_Mesh->m_Renderstate.Get() != nullptr)
		? renderObject->m_Mesh->m_Renderstate.Get()
		: GraphicsEngine::Instance()->GetSolidRS());


	auto _indexBuffer = renderObject->m_Mesh->GetIndexBuffers();

	for (size_t i = 0; i < _indexBuffer.size(); i++)
	{
		if (_indexBuffer[i]->GetIndexCount() < 1)
		{
			continue;
		}

		renderObject->m_Mesh->Bind(deviceContext, (int)i);

		/*ps->Update(cb2, srvs);

		ps->Bind(deviceContext);*/

		m_outLinePS->Bind(deviceContext);
		//deviceContext->PSSetShader(nullptr, nullptr, 0);
		deviceContext->DrawIndexed(_indexBuffer[i]->GetIndexCount(), 0, 0);
	}
}
