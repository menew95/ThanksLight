#pragma once
namespace TLGraphicsEngine
{
	class RenderTargetView;
	class DepthStencilView;
	struct RenderObject;

	class Deferred_Pass
	{
	public:
		Deferred_Pass();
		~Deferred_Pass();

	private:
		std::vector<shared_ptr<RenderTargetView>> m_defferedRTV;

		std::shared_ptr<DepthStencilView> m_depthStencilView;

	public:
		void OnResize(int width, int height);

		void Render(ID3D11DeviceContext* deviceContext, std::queue<RenderObject>& renderQueue);

		void RenderLight(ID3D11DeviceContext* deviceContext);

		void RenderToMain(std::shared_ptr<RenderTargetView> mainRTV);
	};
}