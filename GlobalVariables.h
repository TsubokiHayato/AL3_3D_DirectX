#pragma once

#include<map>
#include<string>
#include <variant>
#include"Vector3.h"


#include "External/nlohmann/json.hpp"

using Item=std::variant<int32_t, float, Vector3,bool>;
using Group=std::map<std::string, Item>;

extern std::map<std::string, Group> datas_;

class GlobalVariables {

public:

	static GlobalVariables* GetInstance();

	void CreateGroup(const std::string& groupName);

	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	void SetValue(const std::string& groupName, const std::string& key, float value);
	void SetValue(const std::string& groupName, const std::string& key, Vector3 value);
	void SetValue(const std::string& groupName, const std::string& key, bool value);


	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	void AddItem(const std::string& groupName, const std::string& key, float value);
	void AddItem(const std::string& groupName, const std::string& key, Vector3 value);
	void AddItem(const std::string& groupName, const std::string& key, bool value);


	void Update();

	void SaveFile(const std::string& groupName);

	void LoadFiles();
	void LoadFile(const std::string& groupName);


	int32_t GetIntValue(const std::string& groupName, const std::string& key) const;
	float GetFloatValue(const std::string& groupName, const std::string& key) const;
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key) const;
	bool GetBoolValue(const std::string& groupName, const std::string& key) const;

private:

	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	GlobalVariables& operator=(const GlobalVariables&) = delete;

	using json=nlohmann::json;

	const std::string kDirectionPath = "Resources/GlobalVariables/";

	
};
