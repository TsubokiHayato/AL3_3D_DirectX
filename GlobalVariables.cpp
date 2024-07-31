#include "GlobalVariables.h"
#include "ImGuiManager.h"
std::map<std::string, Group> datas_;

GlobalVariables* GlobalVariables::GetInstance() {
	static GlobalVariables instance; // C++11 guarantees this is thread-safe
	return &instance;
}
void GlobalVariables::CreateGroup(const std::string& groupName) {
	//
	datas_[groupName];
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value) {

	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.Value = value;
	// 設定した項目をstd::mapに追加
	group.item[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.Value = value;
	// 設定した項目をstd::mapに追加
	group.item[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, Vector3 value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.Value = value;
	// 設定した項目をstd::mapに追加
	group.item[key] = newItem;
}

void GlobalVariables::Update() {

	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}

	if (!ImGui::BeginMenuBar())
		return;

	for (std::map<std::string, Group>::iterator itGroup = datas_.begin();, itGroup != datas_.end(); ++itGroup) {

		// group名を取得
		const std::string& groupName = itGroup->first;
		// グループの参照を取得
		Group& group = itGroup->second;


		if (!ImGui::BeginMenu(groupName.c_str())) {
			continue;

			if (std::holds_alternative<int32_t>(item.value)) {

				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::SliderInt(itemname.c_str(), ptr, 0, 100);

			}


			//04_07 33slide kara

			ImGui::EndMenu();
		}
	}

	ImGui::EndMenuBar();
	ImGui::End();
}
