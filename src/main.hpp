
#pragma once


#include "Socket.hpp"
#include "DialogueProcessor.hpp"


//typedef struct sMenuOption MenuOption;

//bool SayChosenDialogue(int id);
//std::string GetInventory();
//std::string GetGold();

//void action_queue_add(neurosdk_action action);

bool is_universal_blocked();
void set_universal_block(float time);

void set_allowed_events(int amount);

void send_random_context(std::string context);

//debug scan
std::string visit_all_members(bool do_dump, std::ofstream& fs, const RE::GFxValue& in, std::string* search_var, int depth);
bool visit_all_members2(std::vector<std::string>& results, const RE::GFxValue& in, std::string* search_var, int depth, std::string current_path, std::vector<std::string>& skip_problematic, std::vector<std::string>& skip_problematic_path, std::string parent_name, std::string grand_parent_name);
bool visit_all_members3(std::vector<std::string>& results, const RE::GFxValue& in, std::string* search_var, int depth, std::string current_path, std::string val_to_search, std::vector<std::string>& skip_problematic);
bool visit_all_members4(std::vector<std::string>& results, const RE::GFxValue& in, std::string* search_var, int depth, std::string current_path, int val_to_search);



bool force_choice(std::vector<MenuOption> options, std::string message, int force_type);


/*
void force_dialogue(std::vector<MenuOption> dialogue_options);
void force_barter_operation();
void force_barter_category(std::vector<MenuOption> category_options, int type);
void force_barter_item(std::vector<MenuOption> item_options, int type);
void force_barter_slider(int min, int max, std::string item_name, int type);
void force_barter_vendor_not_enough_gold(std::string message);
void force_lockpick_angle(std::string lockpick_amount, std::string lock_level_text);
void force_levelup_attribute();
void force_perks_select_tree(std::vector<MenuOption> options, std::string available_perk_points_text);
void force_perks_select_perk(std::vector<MenuOption> options, std::string tree_name, std::string available_perk_points_text);
std::string get_all_perk_options_string(std::vector<MenuOption> options);
void force_perk_confirm(std::string perk_name, std::string perk_descrption);
void force_alchemy_ingredients(std::vector<MenuOption> category_options);
void force_alchemy_ask_for_more(int ask_for_more);
void force_enchant_operation();
void force_enchant_disenchant_item(std::vector<MenuOption> item_options);
void force_enchant_enchant_item(std::vector<MenuOption> item_options, int step);
void force_enchant_slider(int min, int max, std::string slider_text);
void force_smithing_category(std::vector<MenuOption> category_options, std::string message);
void force_smithing_item(std::vector<MenuOption> item_options, std::string message);
void force_map_location(std::vector<MenuOption> locations);
void force_container_item(std::vector<MenuOption> item_options, std::string message);
void force_gift_item(std::vector<MenuOption> item_options, std::string message);
void force_undiscovered_location(std::string text, std::vector<MenuOption> options);
void force_door_is_closed_choice(std::string message);
void force_message_box(std::string text, std::vector<MenuOption> options);
void force_threat_choice(std::vector<MenuOption> options, std::string message);
void force_character_creation(std::string text, std::vector<MenuOption> options);
void force_character_name(std::string text);
void force_sleepwait(std::string text, std::string range);
void force_book(std::string text, std::vector<MenuOption> options);

*/





