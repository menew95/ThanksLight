#pragma once


namespace TLGameEngine
{
class GameObject;

class Scene
{
public:
	Scene();
	virtual 
		~Scene();

	virtual void Load();
	virtual void Unload();

	void SetName(std::string name) { _name = name; }
	std::string GetName() const { return _name; }

	std::shared_ptr<GameObject> GetGameObject(std::string Uid);
	std::vector<std::shared_ptr<GameObject>>& GetGameObjects();
	std::string GetStrippedUID(std::string instanceUID);

	void AddGameObject(std::shared_ptr<GameObject> gameObject);
	void AddStrippedUID(std::string strippedUID, std::string instanceUID);
private:
	std::vector<std::shared_ptr<GameObject>> m_gameObjects;
	std::unordered_map<std::string, std::string> m_strippedUID_instanceUID;
	std::string _name;
};
}
