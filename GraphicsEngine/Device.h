#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;

#include "ComPtr.h"

namespace TLGraphicsEngine
{
	/// <summary>
	/// Device & DeviceContext Class
	/// </summary>
	class Device
	{
	public:
		Device();
		~Device();

	protected:
		ComPtr<ID3D11Device> m_D3dDevice = nullptr;
		ComPtr<ID3D11DeviceContext> m_D3dImmediateContext = nullptr;

	public:
		ID3D11Device* GetDevice() { return m_D3dDevice.Get(); }
		ID3D11Device** GetDeviceRef() { return m_D3dDevice.GetAddressOf(); }

		ID3D11DeviceContext* GetDeviceContext() { return m_D3dImmediateContext.Get(); }
		ID3D11DeviceContext** GetDeviceContextRef() { return m_D3dImmediateContext.GetAddressOf(); }


	};
}