#include "pch.h"
#include "Light.h"
#include "RenderQueue.h"

#include "GraphicsEngine.h"

#include "DepthStencilView.h"
#include "RenderTargetView.h"

#include "EngineCamera.h"

#include "SimpleMath.h"
#include "SimpleMath.inl"

TLGraphicsEngine::Light::Light()
: m_lightPosition({ 0, 0, 0 })
, m_lightDirection({ 1, 0, 0 })
, m_color({ 0, 0, 0 })
, m_shadowMapDepthStencil(nullptr)
, m_shadowMapRenderTarget(nullptr)
{
	m_center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_radius = sqrtf(50.0f * 50.0f + 75.0f * 75.0f);
	//m_Radius = 2.0f;
	BuildShadowMap();
}

TLGraphicsEngine::Light::~Light()
{
	if(m_shadowMapDepthStencil != nullptr)
	{
		m_shadowMapDepthStencil->Release();
		delete m_shadowMapDepthStencil;
	}
	if (m_shadowMapRenderTarget != nullptr)
	{
		m_shadowMapRenderTarget->Release();
		delete m_shadowMapRenderTarget;
	}
}

void TLGraphicsEngine::Light::BuildShadowMap()
{
	if (!m_shadowMapDepthStencil)
	{
		m_shadowMapDepthStencil = new DepthStencilView;
	}

#if defined(_DEBUG)
	if (!m_shadowMapRenderTarget)
	{
		m_shadowMapRenderTarget = new RenderTargetView;
	}
#endif

	int width = GraphicsEngine::Instance()->GetClientWidth();

	switch (m_eLightType)
	{
		case LightType::Directional:
		{
			width *= 4;
			break;
		}
		case LightType::Spot:
		{
			width *= 2;
			break;
		}
		case LightType::Point:
		default:
		{
			break;
		}
	}
	m_shadowMapDepthStencil->OnResize(width, width, true);
	m_shadowMapRenderTarget->OnResize(width, width, DXGI_FORMAT_R8G8B8A8_UNORM);
}

bool TLGraphicsEngine::Light::CheckDirectionalLight(RenderObject* renderObject)
{
	bool ret = false;
	return true;
}

bool TLGraphicsEngine::Light::CheckPointLight(RenderObject* renderObject)
{
	DirectX::XMMATRIX _objectWorld = DirectX::XMLoadFloat4x4(&renderObject->m_World);

	DirectX::XMVECTOR _objectPosition, _objectScale, _objectRotate, _lightPos;
	
	DirectX::XMMatrixDecompose(&_objectScale, &_objectRotate, &_objectPosition, _objectWorld);
	_lightPos = DirectX::XMLoadFloat3(&m_lightPosition);

	float d = DirectX::XMVectorGetX(DirectX::XMVector3Length(_lightPos - _objectPosition));
	
	if (m_range < d)
	{
		return false;
	}

	return true;
}

bool TLGraphicsEngine::Light::CheckSpotLight(RenderObject* renderObject)
{
	DirectX::XMMATRIX _objectWorld = DirectX::XMLoadFloat4x4(&renderObject->m_World);

	DirectX::XMVECTOR _objectPosition, _objectScale, _objectRotate, _lightPos;

	DirectX::XMMatrixDecompose(&_objectScale, &_objectRotate, &_objectPosition, _objectWorld);
	_lightPos = DirectX::XMLoadFloat3(&m_lightPosition);

	float d = DirectX::XMVectorGetX(DirectX::XMVector3Length(_lightPos - _objectPosition));

	return true;
	if (m_range < d)
	{
		return false;
	}

	DirectX::XMVECTOR objDir = DirectX::XMVector3Normalize(_objectPosition - _lightPos);
	float dot = DirectX::XMVectorGetX(
		DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&m_lightDirection), objDir)
	);

	float angleRad = acosf(dot);

	if (angleRad > m_spotAngle)
	{
		return false;
	}

	return true;
}

void TLGraphicsEngine::Light::DirectionalLightViewProjTM()
{
	XMVECTOR _lightDir = XMLoadFloat3(&m_lightDirection);
	XMVECTOR m_LightPos = -2.0f * m_radius * _lightDir;
	//m_Center.x += 1;

	XMFLOAT3 tempCamPos = GraphicsEngine::Instance()->GetEngineCamera()->GetPosition();
	XMVECTOR camPos =	XMLoadFloat3(&tempCamPos);
	XMVECTOR _targetPos = XMLoadFloat3(&m_center);
	XMVECTOR _up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMFLOAT3 pos = GraphicsEngine::Instance()->GetEngineCamera()->GetPosition();
	//XMMATRIX camView = GraphicsEngine::Instance()->m_EngineCamera->GetViewTM();


	XMMATRIX camPosTM =	XMMatrixTranslation(pos.x, pos.y, pos.z);
	XMFLOAT3 lookAt = GraphicsEngine::Instance()->GetEngineCamera()->GetLookAt();
	XMVECTOR lv = XMLoadFloat3(&lookAt);
	XMVECTOR rv = DirectX::XMVector3Normalize(XMVector3Cross(_up, lv));
	DirectX::XMVECTOR uv = DirectX::XMVector3Cross(lv, rv);


	XMFLOAT3 l, u, r;
	XMStoreFloat3(&l, lv);
	XMStoreFloat3(&u, uv);
	XMStoreFloat3(&r, rv);
	XMMATRIX camRotTM = XMMATRIX(
		1, 0, 0, 0.0f,
		u.x, u.y, u.z, 0.0f,
		0, 0, 1, 0.0f,
		0, 0, 0, 1.0f);

	m_viewTM = XMMatrixLookAtLH(m_LightPos, _targetPos, _up);

	// 경계구를 광원 공간으로 변환한다
	XMFLOAT3 _sphereCenterLS;
	XMStoreFloat3(&_sphereCenterLS, XMVector3TransformCoord(_targetPos, m_viewTM));

	// 장면을 감싸는 광원 공간 직교 투영 상자
	float _viewLeft = _sphereCenterLS.x - m_radius;
	float _viewBottom = _sphereCenterLS.y - m_radius;
	float _viewNear = _sphereCenterLS.z - m_radius;
	float _viewRight = _sphereCenterLS.x + m_radius;
	float _viewTop = _sphereCenterLS.y + m_radius;
	float _viewFar = _sphereCenterLS.z + m_radius;

	m_projTM = XMMatrixOrthographicOffCenterLH(_viewLeft, _viewRight, _viewBottom, _viewTop, _viewNear, _viewFar);

	// NDC 공간 [-1, +1]^2을 텍스처 공간 [0,1]^2으로 변환한다

	XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	m_shadowTM = m_viewTM * m_projTM;// * T;

	m_viewProjTM = XMMatrixMultiply(m_viewTM, m_projTM);
}

void TLGraphicsEngine::Light::SpotLightViewProjTM()
{
	// 위쪽을 가리키는 벡터를 설정합니다.
	DirectX::SimpleMath::Vector3 _r;
	DirectX::SimpleMath::Vector3 _u = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);;
	DirectX::SimpleMath::Vector3 _l = XMLoadFloat3(&m_lightDirection);
	DirectX::SimpleMath::Vector3 _p = XMLoadFloat3(&m_lightPosition);

	_l = XMVector3Normalize(_l);
	_r = XMVector3Normalize(XMVector3Cross(_u, _l));
	_u = XMVector3Cross(_l, _r);

	float _x = -XMVectorGetX(XMVector3Dot(_p, _r));
	float _y = -XMVectorGetX(XMVector3Dot(_p, _u));
	float _z = -XMVectorGetX(XMVector3Dot(_p, _l));

	m_viewTM = DirectX::SimpleMath::Matrix(
		_r.x, _u.x, _l.x, 0.0f,
		_r.y, _u.y, _l.y, 0.0f,
		_r.z, _u.z, _l.z, 0.0f,
		_x, _y, _z, 1.0f);
	m_viewTM = DirectX::XMMatrixLookAtLH(_p, _p + _l, _u);

	// 정사각형 광원에 대한 시야 및 화면 비율을 설정합니다.
	//float fieldOfView = acos(m_spotAngle) * 2.0f;
	float screenAspect = 1.0f;
	
	// 빛의 투영 행렬을 만듭니다.
	m_projTM = XMMatrixPerspectiveFovLH(m_spotAngle, screenAspect, 0.1f, 1000.0f);

	m_shadowTM = m_viewTM * m_projTM;

	m_viewProjTM = XMMatrixMultiply(m_viewTM, m_projTM);
}

ID3D11ShaderResourceView** TLGraphicsEngine::Light::GetShadowMapRef()
{
	return m_shadowMapDepthStencil->GetSRVR();
}

bool TLGraphicsEngine::Light::CheckLight(RenderObject* renderObject)
{
	if (!m_enable)
	{
		return false;
	}

	switch (m_eLightType)
	{
		case LightType::Directional:
			return CheckDirectionalLight(renderObject);
		case LightType::Point:
			return CheckPointLight(renderObject);
		case LightType::Spot:
		case LightType::BlackSpotLight:
			return CheckSpotLight(renderObject);
		default:
#ifdef _DEBUG
			assert(false);
#endif // _DEBUG
			return false;
	}
}

void TLGraphicsEngine::Light::BindShadowMap(ID3D11DeviceContext* deviceContext)
{
	D3D11_VIEWPORT m_ShadowMapViewPort;
	m_ShadowMapViewPort.TopLeftX = 0;
	m_ShadowMapViewPort.TopLeftY = 0;
	m_ShadowMapViewPort.Width = static_cast<float>(m_shadowMapDepthStencil->GetWidth());
	m_ShadowMapViewPort.Height = static_cast<float>(m_shadowMapDepthStencil->GetHeight());
	m_ShadowMapViewPort.MinDepth = 0.f;
	m_ShadowMapViewPort.MaxDepth = 1.f;

	deviceContext->RSSetViewports(1, &m_ShadowMapViewPort);
	deviceContext->OMSetDepthStencilState(nullptr, 0);
	ID3D11RenderTargetView* rtv[1] = { nullptr };
	deviceContext->OMSetRenderTargets(1, rtv, m_shadowMapDepthStencil->GetDSV());

	deviceContext->ClearDepthStencilView(m_shadowMapDepthStencil->GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void TLGraphicsEngine::Light::BindShadowMapTexture(ID3D11DeviceContext* deviceContext)
{
	//ID3D11RenderTargetView* rtv[1] = { 0 };
	//deviceContext->OMSetRenderTargets(1, rtv, m_pShadowMapDepthStencil->GetDSV());

	//deviceContext->ClearDepthStencilView(m_pShadowMapDepthStencil->GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

DirectX::XMMATRIX TLGraphicsEngine::Light::GetViewProjTM()
{
	switch (m_eLightType)
	{
		case LightType::Directional:
		{
			DirectionalLightViewProjTM();
			break;
		}
		case LightType::Spot:
		{
			SpotLightViewProjTM();
			break;
		}
		case LightType::Point:
		default:
			assert(false);
			break;
	}

	return m_viewProjTM;
}
