#pragma once
#include <unordered_map>
#include "Utilities.hpp"

template<typename Derived, typename T>
class ResourceManager {

private:
	std::unordered_map<std::string, std::pair<T*, unsigned int>> resources;
	std::unordered_map<std::string, std::string> paths;

	std::pair<T*, unsigned int>* find(const std::string& id) {
		auto itr = resources.find(id);
		return (itr != resources.end() ? &itr->second : nullptr);
	}

	bool unload(const std::string& id) {
		auto itr = resources.find(id);
		if (itr == resources.end())
			return false;
		delete itr->second.first;
		resources.erase(itr);
		return true;
	}

	void loadPaths(const std::string& pathFile) {
		std::ifstream pathf;
		pathf.open(Utils::GetWorkingDirectory() + pathFile);
		if (pathf.is_open()) {
			std::string line;
			while (std::getline(pathf, line)) {
				std::stringstream keystream(line);
				std::string pathName;
				std::string path;
				keystream >> pathName;
				keystream >> path;
				paths.emplace(pathName, path);
			}
			pathf.close();
			return;
		}
		std::cerr << "! Failed loading path file: " << pathFile << std::endl;
	}

public:
	ResourceManager(const std::string& pathsFile) {
		loadPaths(pathsFile);
	}

	T* getResource(const std::string& id) {
		auto res = find(id);
		return (res ? res->first : nullptr);
	}

	std::string getPath(const std::string& id) {
		auto path = paths.find(id);
		return (path != paths.end() ? path->second : "");
	}

	bool requireResource(const std::string& id) {
		auto res = find(id);
		if (res) {
			++res->second;
			return true;
		}
		auto path = paths.find(id);
		if (path == paths.end())
			return false;
		T* resource = load(path->second);
		if (!resource)
			return false;
		resources.emplace(id, std::make_pair(resource, 1));
		return true;
	}

	bool releaseResource(const std::string& id) {
		auto res = find(id);
		if (!res)
			return false;
		--res->second;
		if (!res->second)
			unload(id);
		return true;
	}

	void purgeResources() {
		while (resources.begin() != resources.end()) {
			delete resources.begin()->second.first;
			resources.erase(resources.begin());
		}
	}

	T* load(const std::string& path) {
		return static_cast<Derived*>(this)->load(path);
	}

	virtual ~ResourceManager() {
		purgeResources();
	}
};