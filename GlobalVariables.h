#pragma once

#include<map>
#include<string>
#include <variant>
#include"Vector3.h"


struct  Item {
	//項目の値
	std::variant<int32_t, float, Vector3> Value;
};

struct Group {
	std::map<std::string, Item> item;
};

extern std::map<std::string, Group> datas_;

class GlobalVariables {

public:

	static GlobalVariables* GetInstance();

	void CreateGroup(const std::string& groupName);

	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	void SetValue(const std::string& groupName, const std::string& key, float value);
	void SetValue(const std::string& groupName, const std::string& key, Vector3 value);


	void Update();

private:

	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	GlobalVariables& operator=(const GlobalVariables&) = delete;

};
