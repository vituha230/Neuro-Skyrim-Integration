#pragma once

#include "DialogueProcessor.hpp"

namespace WalkerProcessor {

	void processor(float dtime);

	void install_hook();

	void reset_walker();

	void walk_to_object_by_refr(RE::TESObjectREFR* target, int action);

	std::pair<bool, std::string> walk_to_object_by_index(int index, int interaction);
	std::pair<bool, std::string> walk_to_quest_by_index(int index);


	std::pair<bool, std::string> set_closed_door_choice(int choice);


	std::pair<bool, std::string> run_away();


	bool lock_camera_onto_target(RE::TESObjectREFR* target, float dtime);

	std::pair<bool, std::string> walk_to_location_by_index(int location);
	bool walk_to_location_by_refr(RE::TESObjectREFR* refr);


	bool walk_to_player_marker();

	bool has_ranged_weapon_equipped(bool right);

	bool is_fighting();

	bool no_ammo();


	float get_weapon_range(bool right);
	float get_weapon_projectile_speed(bool right);
	bool get_current_active_hand();
	bool is_casting_something(bool right);
	void stop_casting_hand(bool right);


	bool pause_attacking(float dtime);
	void unpause_attacking();
	bool is_pickpocketing();


}