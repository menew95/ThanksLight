#pragma once

namespace TLGameEngine
{
enum class LayerType
{
	Default,
	UI,
};
class LayerMask
{
public:
	size_t GetValue() { return static_cast<size_t>(m_layer); }
	void SetValue(size_t layer) { m_layer = static_cast<LayerType>(layer); }
	void SetValue(LayerType layer) { m_layer = layer; }
private:
	LayerType m_layer = LayerType::Default;
};
}

