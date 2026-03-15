#pragma once

#include "DialogueProcessor.hpp"

namespace WalkerProcessor {

	void processor(float dtime);

	void stop_casting();


	void install_hook();

	void reset_walker();
	//void reset_explore_mode();


	std::pair<bool, std::string> walk_to_object_by_refr(RE::TESObjectREFR* target, int action, bool surrender_to_guards_mode = false);

	std::pair<bool, std::string> walk_to_object_by_index(int index, int interaction);
	std::pair<bool, std::string> walk_to_quest_by_index(int index, bool ignore_specified_target);
	std::pair<bool, std::string> look_at_object_by_index(int index);

	std::pair<bool, std::string> explore_world(bool internal_call);


	std::pair<bool, std::string> set_closed_door_choice(int choice);


	std::pair<bool, std::string> run_away();
	std::pair<bool, std::string> exit_dungeon();


	bool lock_camera_onto_target(RE::TESObjectREFR* target, float dtime);

	std::pair<bool, std::string> walk_to_location_by_index(int location);
	bool walk_to_location_by_refr(RE::TESObjectREFR* refr);


	bool walk_to_player_marker(RE::TESObjectREFR* target_location);

	bool has_ranged_weapon_equipped(bool right);

	bool is_fighting();

	bool no_ammo();

	bool was_casting_clairvoyance();

	float get_weapon_range(bool right);
	float get_weapon_projectile_speed(bool right);
	bool get_current_active_hand();
	bool is_casting_walker(bool right);
	void stop_casting_hand(bool right);


	bool pause_attacking(float dtime);
	void unpause_attacking();
	bool is_pickpocketing();

	bool interact_with_target(float dtime);
	bool walker_active();
	bool is_walking_important_path();
	bool target_is_too_high();



	bool is_exploring();

	std::pair<bool, std::string> surrender_to_guards();
	bool is_surrendering();

	void get_into_carriage(RE::TESObjectREFR* seat);
	bool is_getting_into_carriage();


	std::pair<bool, std::string> escape_prison();

	void walk_whiterun_prison_grate();
	bool is_concentration_spell(bool right);
	bool has_spell_equipped(bool right);
	float get_spell_cost(bool right);
	std::string get_equipped_spell_name(bool right);

	RE::NiPoint3 get_estimate_aim_pos(RE::TESObjectREFR* target);

	float get_attack_time(bool right);

	std::pair<bool, std::string> make_spins(int amount, int speed);



	void reset_backup_pickup();

	float get_walker_inactive_time();
	void reset_inactive_timer();

}