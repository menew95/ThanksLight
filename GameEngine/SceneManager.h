#pragma once

#include "Singleton.h"

namespace TLGameEngine 
{
class Scene;
class GameObject;

class SceneManager : public Singleton<SceneManager>
{
private:
	friend Singleton;
	SceneManager() = default;
	~SceneManager() = default;

public:
	void LoadScene(std::string sceneName);
	void LoadScene(int sceneNum);
	void ChangeScene(std::string sceneName);
	void ChangeScene(int sceneNum);
	void UnloadScene(std::shared_ptr<Scene> scene);
	void UnloadScene(std::string name);
	void UnloadScene(int sceneNum);
	const std::vector<std::shared_ptr<Scene>>& GetLoadedScenes() { return m_loadedScene; }
	const int GetLoadedSceneCount() { return static_cast<int>(m_scenesName.size()); }
	void Clear();

	int GetCurrentSceneIndex() { return m_currentSceneIndex; }

	std::shared_ptr<TLGameEngine::GameObject> FindObject(std::string Uid);
	std::shared_ptr<TLGameEngine::GameObject> FindScene(std::string Uid);
	std::shared_ptr<TLGameEngine::GameObject> FindObject(FileID fileId);

	// Todo : 현재 씬 이름 정보를 디버그로 띄우기 위해 이름을 알고 싶다아아
	std::shared_ptr<Scene> GetScene();

	void LoadDontDestroyScene();
	std::shared_ptr<Scene> m_currScene;
private:
	void LoadScenesName(std::string filePath);
	bool LoadSceneFromUnityScene(std::string fileName);

private:
	std::vector<std::shared_ptr<Scene>> m_loadedScene;
	std::vector<std::string> m_scenesName;
	int m_currentSceneIndex;

	std::shared_ptr<Scene> m_unloadScene;
};
}

