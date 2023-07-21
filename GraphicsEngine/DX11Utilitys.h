#pragma once

struct ID3D11DeviceChild;

namespace TLGraphicsEngine
{
	void DX11SetObjectName(ID3D11DeviceChild* obj, const char* name);

	void DX11SetObjectNameSubscript(ID3D11DeviceChild* obj, const char* name, const char* subscript);

	void DX11SetObjectNameIndexed(ID3D11DeviceChild* obj, const char* name, unsigned int idx);
}