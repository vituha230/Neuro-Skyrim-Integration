#pragma once

#include "DialogueProcessor.hpp"

namespace WalkerProcessor {

	void processor(float dtime);
	void lower_processor(float dtime);


	void notify_walker_to_stop_autolockpick_on_enemies();

	void stop_casting();

	bool have_doors_nearby(float range = 800.0f, bool ignore_raycast = false);
	RE::TESObjectREFR* get_nearest_door(bool ignore_raycast = false);

	bool is_casting_walker2(bool right);
	bool is_casting_walker3(bool right);

	bool detect_quest_target_changed_and_walk(bool manual_check = false);

	void install_hook();

	void reset_walker();
	//void reset_explore_mode();

	void refresh_reminder_start_pos();

	void test_new_very_close_quest();

	void reset_multiple_paths_quest();

	RE::TESObjectREFR* get_runaway_target();

	bool get_just_teleported();
	void clear_just_teleported();
	void set_just_teleported();

	void invalidate_path();

	bool no_charge(bool right);

	float get_spell_timeout(bool right);


	std::pair<bool, std::string> make_jumps();


	std::pair<bool, std::string> switch_vampirelord_mode_up();
	std::pair<bool, std::string> switch_vampirelord_mode_down();


	bool has_staff_equipped(bool right);

	bool is_door(RE::TESObjectREFR* refr);

	std::pair<bool, std::string> set_multiple_path_quest_choice(int id);


	std::pair<bool, std::string> walk_to_object_by_refr(RE::TESObjectREFR* target, int action, bool surrender_to_guards_mode = false);

	std::pair<bool, std::string> walk_to_object_by_index(int index, int interaction);
	std::pair<bool, std::string> walk_to_quest_by_index(int index, bool ignore_specified_target, bool skip_confirm = true, bool from_map = false);
	std::pair<bool, std::string> look_at_object_by_index(int index);

	std::pair<bool, std::string> explore_world(bool internal_call);


	bool interacting_with_workbench();




	bool close_enough();


	bool is_running_away();

	bool autoloader_door_evasion_active();


	std::pair<bool, std::string> set_closed_door_choice(int choice);

	std::pair<bool, std::string> set_stealing_choice(int id);

	std::pair<bool, std::string> walk_to_current_quest();


	std::pair<bool, std::string> set_attack_friend_choice(int id);

	std::pair<bool, std::string> run_away();
	std::pair<bool, std::string> exit_dungeon();


	bool lock_camera_onto_target(RE::TESObjectREFR* target, float dtime, float speed_koef = 1.0f, bool force_speed_correction = false, bool force_high_precision = false);

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

	void reset_attacking_inanimate_object_time();

	void clear_loop_evasion();

	bool pause_attacking(float dtime);
	void unpause_attacking();
	bool is_pickpocketing();

	bool interact_with_target(float dtime);
	bool walker_active();
	bool is_walking_important_path();
	bool target_is_too_high();

	bool has_bow_equipped(bool right);
	bool has_crossbow_equipped(bool right);
	bool has_staff_equipped(bool right);
	bool no_charge(bool right);

	std::pair<bool, std::string> look_at_object_by_refr(RE::TESObjectREFR* object, bool no_crouch = false, float look_speed_koef = 0.5f, bool dont_reset_threats = false);

	void look_up(float speed_koef = 0.5f);


	bool is_exploring();

	std::pair<bool, std::string> surrender_to_guards();
	bool is_surrendering();

	void get_into_carriage(RE::TESObjectREFR* seat);
	bool is_getting_into_carriage();


	std::pair<bool, std::string> escape_prison();

	void walk_whiterun_prison_grate();
	void walk_soltsheim_prison_grate();
	void walk_soltsheim_prison_grate2();

	bool is_concentration_spell(bool right);
	float get_spell_cost(bool right);
	std::string get_equipped_spell_name(bool right);

	RE::NiPoint3 get_estimate_aim_pos(RE::TESObjectREFR* target, bool sit_correction = true);

	float get_attack_time(bool right);
	

	int get_quest_id_by_refr(RE::TESQuest* quest, RE::BGSQuestObjective* specific_objective = nullptr, RE::TESQuestTarget* specific_target = nullptr);

	std::pair<bool, std::string> make_spins(int amount, int speed);

	bool ustengrev_puzzle_get_ready(int mode);
	bool processing_ustengrev();
	bool ustengrev_off_the_cliff();

	RE::TESObjectREFR* get_phantom_target(RE::TESQuest* quest, RE::BGSQuestObjective* specific_objective);

	std::pair<bool, std::string> set_ruin_pillar_choice(int id);
	std::pair<bool, std::string> set_change_quest_course_choice(int id);

	void reset_backup_pickup();

	float get_walker_inactive_time();
	void reset_inactive_timer();

	void walk_karthspire_plates();

	bool is_fire_and_forget_spell(bool right);
	//void set_last_teleport_timestamp(long long timestamp);

	std::pair<bool, std::string> shout_at_target(RE::TESObjectREFR* target, RE::TESShout* shout, bool is_gate_shout = false, RE::TESObjectREFR* refr_to_activate_when_done = nullptr);
	std::pair<bool, std::string> cast_spell_at_target(RE::TESObjectREFR* target, RE::SpellItem* spell, bool check_fight_condition = false);


	bool is_objective_phantom(RE::TESQuest* quest, RE::BGSQuestObjective* specific_objective);

	void put_follow_quest_on_cooldown();

	std::pair<bool, std::string> turn_sneak_on();
	std::pair<bool, std::string> turn_sneak_off();
	bool is_sneak_on();


	void start_emergency_swimup();
	void clear_emergency_swimup();

	void walk_again();
}