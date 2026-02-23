#pragma once

#include "DialogueProcessor.hpp"


namespace AlchemyProcessor {



bool quit_menu();
void processor(float dtime);
std::pair<bool, std::string> choose_ingredients(std::vector<int> ingredients);
std::pair<bool, std::string> choose_craft_more(int amount_more);



}