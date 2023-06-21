#include "pch.h"
#include "Factory.h"
#include "ResourceManager.h"
#include "../ThanksLight/ClassResigter.h"

std::unordered_map<boost::json::string, std::shared_ptr<void> (*)(const boost::json::value&)> Factory::__CreateObjectFunctions;

using namespace TLGameEngine;


namespace DirectX
{
namespace SimpleMath
{
BOOST_DESCRIBE_STRUCT(DirectX::SimpleMath::Vector2, (), (x, y))
BOOST_DESCRIBE_STRUCT(DirectX::SimpleMath::Vector3, (), (x, y, z))
BOOST_DESCRIBE_STRUCT(DirectX::SimpleMath::Vector4, (), (x, y, z, w))
BOOST_DESCRIBE_STRUCT(DirectX::SimpleMath::Quaternion, (), (x, y, z, w))
}
}

void Factory::Init()
{
	boost::json::value jv{};
	auto a = boost::json::value_to_tag<typename std::remove_cv<DirectX::SimpleMath::Vector2>::type>();
	//boost::json::detail::value_to_impl(boost::json::value_to_tag<DirectX::SimpleMath::Test>(), jv);
	////auto b = boost::json::value_to_tag<TLGameEngine::Transform>();
	////boost::json::detail::value_to_impl(b, jv);
	////boost::json::value_to<TLGameEngine::Transform>(jv);
	ClassFactory::RegistClasses();
}

struct MonoBehaviour
{
	FileInfo m_Script;
};
BOOST_DESCRIBE_STRUCT(MonoBehaviour, (), (m_Script))

std::shared_ptr<TLGameEngine::Component> Factory::CreateComponentFromJson(boost::json::value& jv)
{
	boost::json::string className = jv.get_object()["className"].get_string();

	if (className == "MonoBehaviour")
	{
		std::shared_ptr<MonoBehaviour> gameObject = Factory::CreateObjectFromJson<MonoBehaviour>(jv);

		if (jv.get_object().contains("_className"))
		{
			className = jv.get_object()["_className"].get_string();
		}
		else
		{
			className = ResourceManager::Instance().GetScriptName(gameObject->m_Script.guid);
		}
	}

	std::string s = className.c_str();
	if (__CreateObjectFunctions.find(className) != __CreateObjectFunctions.end())
	{
		return std::reinterpret_pointer_cast<TLGameEngine::Component>(Factory::__CreateObjectFunctions[className](jv));
	}
	else
		return nullptr;
}
	