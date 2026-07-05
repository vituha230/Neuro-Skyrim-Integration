#pragma once




namespace Apocrypha {







	struct apocrypha_result {
		int id;
		int action;
		RE::TESObjectREFR* target;
		bool dont_save_target;
		std::vector<RE::NiPoint3> custom_path;
		int interaction;
		bool dont_save_interaction;
		bool reset_curent_action;
		bool dont_save_id;
		bool dont_save_action;
		bool clear_path;
		bool correct_custom_path;
		bool append_to_normal_path;
	};


	apocrypha_result apocrypha_redirects(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id);
	void reset_apocrypha_redirects();


	bool in_apocrypha(RE::TESObjectREFR* object = nullptr);





}




