#include "GlobalVariables.h"
#include "ImGuiManager.h"

#include <fstream>
#include <filesystem>
#include <string>

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
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, Vector3 value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
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
			for (std::map<std::string, Item>::iterator itItem = group.items.begin(); itItem != group.items.end(); ++itItem) {

				const std::string& itemName = itItem->first;

				Item& item = itItem->second;
				if (std::holds_alternative<int32_t>(item.value)) {

					int32_t* ptr = std::get_if<int32_t>(&item.value);
					ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
				} else if (std::holds_alternative<float>(item.value)) {

					float* ptr = std::get_if<float>(&item.value);
					ImGui::SliderFloat(itemName.c_str(), ptr, 0.0f, 100.0f);

				} else if (std::holds_alternative<Vector3>(item.value)) {

					Vector3* ptr = std::get_if<Vector3>(&item.value);
					ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
				}
			}
			ImGui::Text("\n");
			if (ImGui::Button("Save")) {
				SaveFile(groupName);
				std::string message = std::format("{}.json saved.", groupName);
				MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
			}
		
			ImGui::EndMenu();
		}
	}

	ImGui::EndMenuBar();
	ImGui::End();
}

void GlobalVariables::SaveFile(const std::string& groupName) {

	// グループ検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	// 未登録チェック
	assert(itGroup != datas_.end());

	json root;

	root = json::object();

	root[groupName] = json::object();

	for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin(); itItem != itGroup->second.items.end(); ++itItem) {

		// 項目数を取得
		const std::string& itemName = itItem->first;
		// 項目の参照を取得
		Item& item = itItem->second;

		// int32_t型の値を保持していれば
		if (std::holds_alternative<int32_t>(item.value)) {
			// int2_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item.value);


			// float型の値を保持していれば
		} else if (std::holds_alternative<float>(item.value)) {
			// float型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);


			// Vector3型の値を保持していれば
		} else if (std::holds_alternative<Vector3>(item.value)) {

			// Vector3型のjson配列登録
			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = json::array({value.x, value.y, value.z});
		}



		
	}

	 // ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectionPath);
	if (!std::filesystem::exists(dir)) {
		std::filesystem::create_directory(dir);
	}

	//書き込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectionPath + groupName + ".json";
	//書き込む用ファイルストリーム
	std::ofstream ofs;
	ofs.open(filePath);

	//ファイルオープン失敗?
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}
	//ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	//ファイルを閉じる
	ofs.close();



}
