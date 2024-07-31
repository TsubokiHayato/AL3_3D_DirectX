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

	if (!ImGui::BeginMenuBar()) {
		ImGui::End(); // 追加: MenuBarが始まらなかった場合、ウィンドウも終了
		return;
	}

	for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end(); ++itGroup) {

		// group名を取得
		const std::string& groupName = itGroup->first;
		// グループの参照を取得
		Group& group = itGroup->second;

		if (ImGui::BeginMenu(groupName.c_str())) { // 変更: if文の中身を反転
			for (std::map<std::string, Item>::iterator itItem = group.item.begin(); itItem != group.item.end(); ++itItem) {

				const std::string& itemName = itItem->first;

				Item& item = itItem->second;
				if (std::holds_alternative<int32_t>(item.Value)) {

					int32_t* ptr = std::get_if<int32_t>(&item.Value);
					ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
				} else if (std::holds_alternative<float>(item.Value)) {

					float* ptr = std::get_if<float>(&item.Value);
					ImGui::SliderFloat(itemName.c_str(), ptr, 0.0f, 100.0f);

				} else if (std::holds_alternative<Vector3>(item.Value)) {

					Vector3* ptr = std::get_if<Vector3>(&item.Value);
					ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
				}
			}

			ImGui::EndMenu();
		}
	}

	ImGui::EndMenuBar();
	ImGui::End();
}