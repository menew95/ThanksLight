#pragma once

#include <unordered_map>
#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <boost/mp11.hpp>

namespace TLGameEngine
{
class Component;
class GameObject;
class Texture;
class Sprite;
}

class Factory
{
public:
	static void Init();

	template<typename T>
	static std::shared_ptr<void> _CreateObjectFromJson(const boost::json::value& jv);
	template<typename T>
	static std::shared_ptr<T> CreateObjectFromJson(boost::json::value& jv);
	static std::shared_ptr<TLGameEngine::Component> CreateComponentFromJson(boost::json::value& jv);
	static std::unordered_map<boost::json::string, std::shared_ptr<void> (*)(const boost::json::value&)> __CreateObjectFunctions;
};

template<typename T>	
std::shared_ptr<void> Factory::_CreateObjectFromJson(const boost::json::value& jv)
{
	return std::make_shared<T>(boost::json::value_to<T>(jv));
}

template<typename T>
inline std::shared_ptr<T> Factory::CreateObjectFromJson(boost::json::value& jv)
{
	return std::make_shared<T>(boost::json::value_to<T>(jv));
}


#define RegistFactoryFromJson(name) \
Factory::__CreateObjectFunctions[#name] = &Factory::_CreateObjectFromJson<name>;

#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <boost/mp11.hpp>

template <typename T,
	typename Member = boost::describe::describe_members<T, boost::describe::mod_any_access>>
	void tag_invoke(boost::json::value_from_tag const&, boost::json::value& jv, T const& t)
{
	auto& obj = jv.emplace_object();

	boost::mp11::mp_for_each<Member>([&](auto d)
		{
			obj[d.name] = boost::json::value_from(t.*d.pointer);
		});
}


template<class T> void extract(boost::json::object const& obj, char const* name, T& value)
{
	value = boost::json::value_to<T>(obj.at(name));
}

template <typename T,
	typename Member = boost::describe::describe_members<T, boost::describe::mod_any_access>>
	T tag_invoke(boost::json::value_to_tag<T> const&, const boost::json::value& jv)
{
	auto const& obj = jv.as_object();

	T t{};

	boost::mp11::mp_for_each<Member>([&](auto d) {
		extract(obj, d.name, t.*d.pointer);
		});

	return t;
}

namespace TLGameEngine
{
template <typename T,
	typename Member = boost::describe::describe_members<T, boost::describe::mod_any_access>>
	void tag_invoke(boost::json::value_from_tag const&, boost::json::value& jv, T const& t)
{
	auto& obj = jv.emplace_object();

	boost::mp11::mp_for_each<Member>([&](auto d)
		{
			obj[d.name] = boost::json::value_from(t.*d.pointer);
		});
}

template <typename T,
	typename Member = boost::describe::describe_members<T, boost::describe::mod_any_access>>
	T tag_invoke(boost::json::value_to_tag<T> const&, const boost::json::value& jv)
{
	auto const& obj = jv.as_object();

	T t{};

	boost::mp11::mp_for_each<Member>([&](auto d) {
		extract(obj, d.name, t.*d.pointer);
		});

	return t;
}
}

namespace DirectX
{
namespace SimpleMath
{
template <typename T,
	typename Member = boost::describe::describe_members<T, boost::describe::mod_any_access>>
	void tag_invoke(boost::json::value_from_tag const&, boost::json::value& jv, T const& t)
{
	auto& obj = jv.emplace_object();

	boost::mp11::mp_for_each<Member>([&](auto d)
		{
			obj[d.name] = boost::json::value_from(t.*d.pointer);
		});
}

template <typename T,
	typename Member = boost::describe::describe_members<T, boost::describe::mod_any_access>>
	T tag_invoke(boost::json::value_to_tag<T> const&, const boost::json::value& jv)
{
	auto const& obj = jv.as_object();

	T t{};

	boost::mp11::mp_for_each<Member>([&](auto d) {
		extract(obj, d.name, t.*d.pointer);
		});

	return t;
}
}
}

