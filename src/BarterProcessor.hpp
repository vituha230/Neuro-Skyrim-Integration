#pragma once

#include "DialogueProcessor.hpp"

namespace BarterProcessor {

    enum barter_type
    {
        sell,
        buy
    };

    std::pair<bool, std::string> set_barter_type(int in_type);

    void processor(float dtime);

    //void get_category_list();
    //std::vector<MenuOption> get_barter_categories(barter_type type);
    std::pair<bool, std::string> set_category_choice(int id);
    std::pair<bool, std::string> set_item_choice(int id);
    std::pair<bool, std::string> set_slider_choice(int pos);
    std::pair<bool, std::string> set_vendor_not_enough_gold_choice(int choice);

    bool quit_menu();
    bool back_to_categories();
    bool back_to_items();
    bool back_to_barter_type_selection();
    
}


void debug_scan(float dtime);