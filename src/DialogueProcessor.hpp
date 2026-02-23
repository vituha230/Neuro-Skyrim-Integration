#pragma once

//#include "main.hpp"
#include <vector>


    struct MenuOption
    {
        int id{};
        std::string text{};
    };


    enum force_type
    {
        dialogue_line,
        barter_type_force,
        barter_category,
        barter_item,
        barter_quantity,
        barter_vendor_not_enough_gold,
        lockpick_angle,
        levelup_attribute,
        perk_tree,
        perk_perk,
        perk_confirm,
        map_undiscovered,
        alchemy_amount_more,
        enchant_type,
        enchant_item,
        enchant_strength,
        smithing_category,
        smithing_item,
        map_location,
        container_item,
        gift_item,
        messagebox_option,
        threat_response,
        closed_door_choice,
        race,
        sleepwait_time,
        book,
        alchemy_ingredients,
        character_name
    };

    int get_active_force();
    void set_active_force(int id);




    namespace DialogueProcessor {


        std::pair<bool, std::string> say_chosen_line(int id);

        std::vector<MenuOption> get_dialogue_options();
        bool is_selected_line_correct();
        void dialogue_choice_reset();

        void processor(float dtime);
        bool is_in_dialogue(RE::TESObjectREFR* speaker);
        void clean_old_dialogue();

    }
