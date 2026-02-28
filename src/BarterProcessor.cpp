

//TODO: quest items?
//TODO: print player and vendor's gold somewhere

#include "InputActions.hpp"
#include "BarterProcessor.hpp"
#include "main.hpp"

#include <chrono>


namespace BarterProcessor {

   

    bool vendor_not_enough_gold_request_sent = false;
    bool vendor_not_enough_gold_choice_valid = false;
    bool vendor_not_enough_gold_confirmed = false;
    bool vendor_not_enough_gold_confirming = false;
    int vendor_not_enough_gold_choice = 0;

    int slider_choice = 0;
    bool slider_choice_valid = false;
    bool slider_request_sent = false;
    bool slider_confirmed = false;
    bool slider_confirming = false;

    int category_choice = 0;
    bool category_choice_valid = false;
    bool categories_list_valid = false;
    bool barter_category_request_sent = false;
    bool filling_categories = false;
    bool fill_categories_one_direction_done = false;

    int item_choice = 0;
    bool item_choice_valid = false;
    bool items_list_valid = false;
    bool filling_items = false;
    bool fill_items_one_direction_done = false;
    bool barter_item_request_sent = false;
    bool item_confirmed = false;
    bool item_confirming = false;
    bool old_item_choice_text_valid = false;

    bool barter_type_defined = false;
    bool barter_type_request_sent = false;
    bool in_barter = false;
    bool up_done = false;
    bool down_done = false;
    bool block_set = false;
    bool missing_category_detected = false;
    bool missing_item_detected = false;
    int last_cursor_move = 0; //1 - up, 2 - down

    int last_scroll = -1;
    bool scroll_stuck = false;
    float scroll_stuck_time = 0.0f;

    struct item_data {
        std::string name;
        int price;
        std::string weight;
        std::string damage;
        std::string armor;

    };

    std::map<int, std::string> categories_list{};
    std::map<int, item_data> items_list{};

    double debug_time = 0.0;
    int scroll_dbg = 0;


    /*
    struct ItemOption
    {
        int id{};
        std::string text{}; //name
        int quantity{};
        int price{};
    };
    */


    barter_type type{};

    enum way_to_fill {
        down,
        up,
        downup,
        updown
    };

    way_to_fill way{};



    std::pair<bool, std::string> set_barter_type(int in_type)
    {
        std::pair<bool, std::string> result{};

        if (in_type == -1)
        {
            quit_menu();
            result.first = true;
            result.second = "[Barter stopped]";
            return result;
        }

        if (in_type == 0 || in_type == 1)
        {
            if (in_type == 0)
                type = barter_type::sell;
            else
                type = barter_type::buy;

            barter_type_defined = true;
            result.first = true;
            result.second = "[Processing...]";
        }
        else
        {
            result.first = false;
            result.second = "Invalid operation ID. ";
        }

        return result;
    }


    bool quit_menu()
    {
        RE::UIMessageQueue::GetSingleton()->AddMessage(RE::BarterMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
        return true;
    }


    /////////////////////////////////




    bool barter_reset_categories_selection()
    {
        last_cursor_move = 0;
        missing_category_detected = false;
        categories_list.clear();
        category_choice = 0;
        category_choice_valid = false;
        categories_list_valid = false;
        filling_categories = false;
        barter_category_request_sent = false;
        fill_categories_one_direction_done = false;
        up_done = false;
        down_done = false;
        last_scroll = -1;
        scroll_stuck = false;
        scroll_stuck_time = 0.0f;


        return true;
    }

    bool barter_reset_items_selection()
    {
        last_cursor_move = 0;
        missing_item_detected = false;
        items_list.clear();
        filling_items = false;
        item_choice = 0;
        item_choice_valid = false;
        items_list_valid = false;
        barter_item_request_sent = false;
        fill_items_one_direction_done = false;
        item_confirmed = false;
        item_confirming = false;
        up_done = false;
        down_done = false;
        old_item_choice_text_valid = false;

        slider_choice = 0;
        slider_request_sent = false;
        slider_choice_valid = false;
        slider_confirmed = false;
        slider_confirming = false;

        vendor_not_enough_gold_choice = 0;
        vendor_not_enough_gold_request_sent = false;
        vendor_not_enough_gold_choice_valid = false;
        vendor_not_enough_gold_confirmed = false;
        vendor_not_enough_gold_confirming = false;

        last_scroll = -1;
        scroll_stuck = false;
        scroll_stuck_time = 0.0f;

        return true;
    }


    bool barter_reset()
    {
        barter_reset_categories_selection();
        barter_reset_items_selection();

        barter_type_defined = false;
        barter_type_request_sent = false;
        up_done = false;
        down_done = false;
        in_barter = false;
        block_set = false;
        last_cursor_move = 0;
        missing_category_detected = false;

        slider_choice = 0;
        slider_choice_valid = false;
        slider_request_sent = false;

        return true;
    }




    std::pair<bool, std::string> set_category_choice(int id)
    {
        std::pair<bool, std::string> result{};


        if (id == -1)
        {
            quit_menu();
            result.first = true;
            result.second = "[Barter stopped]";
            return result;
        }

        if (id == -2)
        {
            back_to_barter_type_selection();
            result.first = true;
            result.second = "[Went back to barter type selection]";
            return result;
        }


        if (categories_list.find(id) == categories_list.end())
        {
            //barter_category_request_sent = false; //FAILED FORCED ACTIONS ARE REFORCED AUTOMATICALLY
            result.first = false;
            result.second = "Invalid category ID. ";
            return result;
        }

        if (category_choice_valid)
        {
            left(); //go back to reselect category
        }

        category_choice = id;
        category_choice_valid = true;

        result.first = true;
        result.second = "[Processing...]";
        return result;
    }

    int get_slider_min();
    int get_slider_max();


    std::pair<bool, std::string> set_slider_choice(int pos)
    {
        std::pair<bool, std::string> result{};


        if (pos == -1)
        {
            quit_menu();
            result.first = true;
            result.second = "[Barter stopped]";
            return result;
        }

        if (pos == -2)
        {
            back_to_barter_type_selection();
            result.first = true;
            result.second = "[Went back to barter type selection]";
            return result;
        }

        if (pos == -3)
        {
            back_to_categories();
            result.first = true;
            result.second = "[Went back to category selection]";
            return result;
        }

        if (pos == -4)
        {
            back_to_items();
            result.first = true;
            result.second = "[Went back to item selection]";
            return result;
        }

        if (pos < get_slider_min() || pos > get_slider_max() || !item_choice_valid || !category_choice_valid || !barter_type_defined)
        {
            //TODO: give int result, separate invalid id, category undefined and barter_type undefined.
            //barter_item_request_sent = false; //FAILED FORCED ACTIONS ARE REFORCED AUTOMATICALLY

            result.first = false;
            result.second = "Invalid slider position ID";
            return result;
        }

        slider_choice = pos;
        slider_choice_valid = true;

        result.first = true;
        result.second = "[Processing...]";
        return result;
    }

    std::pair<bool, std::string> set_vendor_not_enough_gold_choice(int choice)
    {
        std::pair<bool, std::string> result{};


        if (choice == -1)
        {
            quit_menu();
            result.first = true;
            result.second = "[Barter stopped]";
            return result;
        }

        if (choice == -2)
        {
            back_to_barter_type_selection();
            result.first = true;
            result.second = "[Went back to barter type selection]";
            return result;
        }

        if (choice == -3)
        {
            back_to_categories();
            result.first = true;
            result.second = "[Went back to category selection]";
            return result;
        }

        if (choice == -4)
        {
            back_to_items();
            result.first = true;
            result.second = "[Went back to item selection]";
            return result;
        }


        if (choice == 0 || choice == 1)
        {
            vendor_not_enough_gold_choice = choice;
            vendor_not_enough_gold_choice_valid = true;
            result.first = true;
            result.second = "[Processing...]";
        }
        else
        {
            result.first = false;
            result.second = "Invalid choice ID";
        }
        return result;
    }



    std::pair<bool, std::string> set_item_choice(int id)
    {
        std::pair<bool, std::string> result{};


        if (id == -1)
        {
            quit_menu();
            result.first = true;
            result.second = "[Barter stopped]";
            return result;
        }

        if (id == -2)
        {
            back_to_barter_type_selection();
            result.first = true;
            result.second = "[Went back to barter type selection]";
            return result;
        }

        if (id == -3)
        {
            back_to_categories();
            result.first = true;
            result.second = "[Went back to category selection]";
            return result;
        }



        if (items_list.find(id) == items_list.end() || !category_choice_valid || !barter_type_defined)
        {
            //TODO: give int result, separate invalid id, category undefined and barter_type undefined.
            //barter_item_request_sent = false; //FAILED FORCED ACTIONS ARE REFORCED AUTOMATICALLY
            result.first = false;
            result.second = "Invalid item ID";
            return result;
        }


        item_choice = id;
        item_choice_valid = true;
        result.first = true;
        result.second = "[Processing...]";
        return result;
    }


    std::vector<MenuOption> get_barter_categories()
    {
        std::vector<MenuOption> result{};
        for (std::pair<int, std::string> category : categories_list)
        {
            MenuOption option = { category.first, category.second };
            result.push_back(option);
        }

        result.push_back({ -1, "[QUIT BARTER]" });

        return result;
    }


    std::vector<MenuOption> get_barter_items()
    {
        std::vector<MenuOption> result{};
        for (std::pair<int, item_data> item : items_list)
        {
            std::string weight_text = "";
            if (item.second.weight != "")
                weight_text = "Weight: " + item.second.weight;
            std::string damage_text = "";
            if (item.second.damage != "")
                damage_text = ". Damage: " + item.second.damage;
            std::string armor_text = "";
            if (item.second.armor != "")
                armor_text = ". Armor: " + item.second.armor;


            std::string stats = "";

            stats = weight_text + damage_text + armor_text;
            if (stats != "")
                stats = " [" + stats + "]";

            MenuOption option = { item.first, item.second.name + stats + " - " + std::to_string(item.second.price) + " gold" };

            result.push_back(option);
        }

        result.push_back({ -1, "[QUIT BARTER]" });


        return result;
    }



    //im not sure if there is equivalent variable for items
    //this variable is a disaster. unreliable results. do not use
    /**/
    int DO_NOT_USE_get_selected_category()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.iSelectedCategory"))
                        //send_random_context(std::to_string(var1.GetNumber()).c_str());
                        return var1.GetNumber();

        return -1;
    }



    int get_item_selected_index()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.iSelectedIndex"))
                        return var1.GetNumber();
        return -1;
    }


    int get_item_scroll_position()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.iScrollPosition"))
                        //send_random_context(std::to_string(var1.GetNumber()).c_str());
                        return var1.GetNumber();
        return -1;
    }


    int get_item_max_scroll_position()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.iMaxScrollPosition"))
                        //send_random_context(std::to_string(var1.GetNumber()).c_str());
                        return var1.GetNumber();
        return -1;
    }



    int get_category_selected_index()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc.CategoriesListHolder.List_mc.iSelectedIndex"))//"_root.Menu_mc.InventoryLists_mc._CategoriesList.iSelectedIndex"))
                        //send_random_context(std::to_string(var1.GetNumber()).c_str());
                        return var1.GetNumber();

        return -1;
    }

    int get_category_scroll_position()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList.iScrollPosition"))
                        //send_random_context(std::to_string(var1.GetNumber()).c_str());
                        return var1.GetNumber();
        return -1;
    }



    int get_category_max_scroll_position()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList.iMaxScrollPosition"))
                        //send_random_context(std::to_string(var1.GetNumber()).c_str());
                        return var1.GetNumber();
        return -1;
    }






    void move_cursor_to_barter_category(int id)
    {
        int id_selected = get_category_selected_index();

        if (last_cursor_move != 1 && last_cursor_move != 2)
            bool test = true;


        if (id_selected > id)
        {
            if (last_cursor_move == 2)
                missing_category_detected = true;
            last_cursor_move = 1;
            cursor_up();
        }
        if (id_selected < id)
        {
            if (last_cursor_move == 1)
                missing_category_detected = true;
            last_cursor_move = 2;
            cursor_down();
        }
    }



    void move_cursor_to_barter_item(int id)
    {
        int id_selected = get_item_selected_index();

        if (id_selected > id)
        {
            if (last_cursor_move == 2)
                missing_item_detected = true;
            last_cursor_move = 1;
            cursor_up();
        }
        if (id_selected < id)
        {
            if (last_cursor_move == 1)
                missing_item_detected = true;
            last_cursor_move = 2;
            cursor_down();
        }
    }











    //FOR CATEGORIES:
    //npc starts at 0 category id (sell, 0 = all)
    //player starts at 11 category id (buy, 1 = all)



    void update_barter_categories(barter_type type)
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList")) //this gives only categories that appear on screen right now. if you scroll and some become invisible - this will not show them
                    {
                        std::string path_root = "_root.Menu_mc.InventoryLists_mc._CategoriesList.";
                        for (int i = 0; i < 30; i++)
                        {
                            std::string full_path = path_root + "Entry" + std::to_string(i);
                            if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))
                            {
                                if (menu->uiMovie->GetVariable(&var1, (full_path + ".itemIndex").c_str()))
                                {
                                    //got indexed item
                                    if (!var1.IsNull() && var1.IsNumber())
                                    {
                                        MenuOption result{};

                                        result.id = var1.GetNumber();
                                        std::string name = "";
                                        if (menu->uiMovie->GetVariable(&var1, (full_path + ".textField.text").c_str()))
                                            if (!var1.IsNull() && var1.IsString())
                                                name = var1.GetString();

                                        if (name != "")
                                            switch (type)
                                            {
                                            case barter_type::buy:
                                            {
                                                if (result.id > 0 && result.id < 11) //buy range
                                                {
                                                    result.text = name;
                                                    categories_list.insert({ result.id, result.text });
                                                }
                                                break;
                                            }

                                            case barter_type::sell:
                                            {
                                                if (result.id > 11 && result.id < 30) //sell range
                                                {
                                                    result.text = name;
                                                    categories_list.insert({ result.id, result.text });
                                                }
                                                break;
                                            }
                                            }
                                    }
                                }
                            }
                        }
                    }
    }


    bool update_barter_category_by_id(int id)
    {
        bool update_result = false;

        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList"))
                    {
                        std::string path_root = "_root.Menu_mc.InventoryLists_mc._CategoriesList.";
                        for (int i = 0; i < 30; i++)
                        {
                            std::string full_path = path_root + "Entry" + std::to_string(i);
                            if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))
                            {
                                if (menu->uiMovie->GetVariable(&var1, (full_path + ".itemIndex").c_str()))
                                {
                                    //got indexed item
                                    if (!var1.IsNull() && var1.IsNumber())
                                    {
                                        MenuOption result{};

                                        result.id = var1.GetNumber();
                                        std::string name = "";
                                        if (menu->uiMovie->GetVariable(&var1, (full_path + ".textField.text").c_str()))
                                            if (!var1.IsNull() && var1.IsString())
                                                name = var1.GetString();

                                        if (name != "")
                                        {
                                            result.text = name;
                                            if (result.id == id)
                                            {
                                                categories_list.insert_or_assign(result.id, result.text);
                                                update_result = true;

                                                /*
                                                auto item_pos = items_list.find(id);
                                                if (item_pos != items_list.end())
                                                {
                                                    items_list.erase(item_pos);
                                                    items_list.insert({ result.id, result.text });
                                                    update_result = true;
                                                }
                                                */
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }

        return update_result;
    }





    int get_price_selected_item()
    {

        //+		[1]	"_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.ItemValueText.text = 76"	std::string
        int result = -1;
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.ItemValueText.text"))
                        result = std::stoi(var1.GetString());

        return result;
    }



    bool update_barter_item_by_id(int id)
    {
        bool update_result = false;

        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList"))
                    {
                        std::string path_root = "_root.Menu_mc.InventoryLists_mc._ItemsList.";
                        for (int i = 0; i < 30; i++)
                        {
                            std::string full_path = path_root + "Entry" + std::to_string(i);
                            if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))
                            {
                                if (menu->uiMovie->GetVariable(&var1, (full_path + ".itemIndex").c_str()))
                                {
                                    //got indexed item
                                    if (!var1.IsNull() && var1.IsNumber())
                                    {
                                        MenuOption result{};

                                        result.id = var1.GetNumber();
                                        std::string name = "";
                                        if (menu->uiMovie->GetVariable(&var1, (full_path + ".textField.text").c_str()))
                                            if (!var1.IsNull() && var1.IsString())
                                                name = var1.GetString();

                                        if (name != "")
                                        {
                                            result.text = name;
                                            if (result.id == id)
                                            {
                                                item_data data{};

                                                std::string weight = "";
                                                std::string damage = "";
                                                std::string armor = "";

                                                RE::GFxValue subvar;

                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ItemWeightText.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                        weight = subvar.GetString();
                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.WeaponDamageValue.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                        damage = subvar.GetString();
                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ApparelArmorValue.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                        armor = subvar.GetString();



                                                data.name = name;
                                                data.price = get_price_selected_item();
                                                data.weight = weight;
                                                data.damage = damage;
                                                data.armor = armor;

                                                items_list.insert_or_assign(result.id, data);
                                                update_result = true;

                                                /*
                                                auto item_pos = items_list.find(id);
                                                if (item_pos != items_list.end())
                                                {
                                                    items_list.erase(item_pos);
                                                    items_list.insert({ result.id, result.text });
                                                    update_result = true;
                                                }
                                                */
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }

        return update_result;
    }


    //bool 
    //"_root.ShownMessageArray"
    /*  //top left message array
    +		[0]	"_root.HUDMovieBaseInstance.MessagesBlock.ShownMessageArray[0]"	std::string
    +		[1]	"_root.HUDMovieBaseInstance.HudElements[24].12.ShownMessageArray[0]"	std::string
    +		[2]	"_root.HUDMovieBaseInstance.MessagesInstance.ShownMessageArray[0]"	std::string
    */

    bool have_not_enough_gold_message()
    {
        bool result = false;

        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::HUDMenu>(); menu)
                if (menu->uiMovie)
                    //+		[11]	"_root.HUDMovieBaseInstance.MessagesInstance.ShownMessageArray[1].0.TextFieldClip.tf1.text = Quicksaving..."	std::string
                    if (menu->uiMovie->GetVariable(&var1, "_root.HUDMovieBaseInstance.MessagesInstance.ShownMessageArray"))
                    {
                        if (var1.IsArray())
                        {
                            for (int i = 0; i < var1.GetArraySize(); i++)
                            {
                                RE::GFxValue subvar;

                                if (var1.GetElement(i, &subvar))
                                {
                                    std::vector<std::string> names{};

                                    if (subvar.GetMember("TextFieldClip", &subvar))
                                        if (subvar.GetMember("tf1", &subvar))
                                            if (subvar.GetMember("text", &subvar))
                                            {
                                                //+			0x000002402afad020 "You don't have enough gold."	const char *
                                                if (subvar.IsString())
                                                {
                                                    std::string subvar_string = subvar.GetString();

                                                    std::string string_to_check = "You don't have enough gold.";
                                                    if (subvar_string == string_to_check)
                                                    {
                                                        result = true;
                                                        if (subvar_string != "")
                                                            MiscThings::update_old_topleft_nofification(subvar_string);
                                                    }

                                                }

                                            }
                                }

                            }

                        }
                    }

        return result;
    }


    //[1]	"_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.MessageText.text = Transaction value: 606 gold.\rVendor only has 179 gold."	std::string



    std::string vendor_not_enough_gold_message()
    {
        std::string result = "";

        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    //+		[11]	"_root.HUDMovieBaseInstance.MessagesInstance.ShownMessageArray[1].0.TextFieldClip.tf1.text = Quicksaving..."	std::string
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.MessageText.text"))
                    {
                        if (var1.IsString())
                        {
                            std::string var1_string = var1.GetString();
                            if (var1_string.find("Vendor only has"))
                            {
                                if (int pos = var1_string.find('\r'))
                                    result = var1_string.replace(pos, 1, 1, ' ');
                            }

                        }
                    }
        return result;
    }







    void update_barter_items()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList"))
                    {
                        std::string path_root = "_root.Menu_mc.InventoryLists_mc._ItemsList.";
                        for (int i = 0; i < 30; i++)
                        {
                            std::string full_path = path_root + "Entry" + std::to_string(i);
                            if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))
                            {
                                if (menu->uiMovie->GetVariable(&var1, (full_path + ".itemIndex").c_str()))
                                {
                                    //got indexed item
                                    if (!var1.IsNull() && var1.IsNumber())
                                    {
                                        MenuOption result{};

                                        result.id = var1.GetNumber();
                                        std::string name = "";
                                        if (menu->uiMovie->GetVariable(&var1, (full_path + ".textField.text").c_str()))
                                            if (!var1.IsNull() && var1.IsString())
                                                name = var1.GetString();

                                        if (name != "")
                                        {
                                            if (result.id == get_item_selected_index())
                                            {
                                                item_data data{};
                                                std::string weight = "";
                                                std::string damage = "";
                                                std::string armor = "";

                                                RE::GFxValue subvar;

                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ItemWeightText.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                        weight = subvar.GetString();
                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.WeaponDamageValue.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                        damage = subvar.GetString();
                                                if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ApparelArmorValue.text"))
                                                    if (!subvar.IsNull() && subvar.IsString())
                                                        armor = subvar.GetString();



                                                data.name = name;
                                                data.price = get_price_selected_item();
                                                data.weight = weight;
                                                data.damage = damage;
                                                data.armor = armor;
                                                items_list.insert({ result.id, data });
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }
    }



    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////// SLIDER
    /*

                "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._focused" - slider active
                _focused - slider active
                _minimum
                _maximum
                _value - current position
                */

    int get_slider_min()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._minimum"))
                        if (!var1.IsNull() && var1.IsNumber())
                            return var1.GetNumber();
        return -1;
    }

    int get_slider_max()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._maximum"))
                        if (!var1.IsNull() && var1.IsNumber())
                            return var1.GetNumber();
        return -1;
    }

    int get_slider_pos()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._value"))
                        if (!var1.IsNull() && var1.IsNumber())
                            return var1.GetNumber();
        return -1;
    }


    bool quantity_slider_active()
    {
        bool result = false;
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._focused"))
                        if (!var1.IsNull() && var1.IsNumber() && (int)var1.GetNumber() == 1)
                            result = true;

        return result;
    }


    void move_slider_to_pos(int desired_pos)
    {
        int slider_pos = get_slider_pos();

        if (slider_pos > desired_pos)
        {
            left();
        }
        if (slider_pos < desired_pos)
        {
            right();
        }
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    bool inside_of_category()
    {
        if (quantity_slider_active())
            return true;

        bool result = false;
        RE::UI* ui = RE::UI::GetSingleton();
        auto menu = ui->GetMenu<RE::BarterMenu>();
        RE::GFxValue var1;

        if (ui && menu && ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
            if (menu->uiMovie)
                if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.focused")) //TEST THIS (seems to work, didnt test deep)
                    if (!var1.IsNull() && var1.IsNumber())
                        result = var1.GetNumber();

        return result;

        /*
        bool inside_result = false;

        inside_result = (get_item_selected_index() > -1); //this works

        return inside_result;
        */
    }


    bool back_to_categories()
    {
        left();
        barter_reset_categories_selection();
        barter_reset_items_selection();

        return true;
    }


    bool back_to_items()
    {
        barter_reset_items_selection();

        if (!update_barter_category_by_id(category_choice))
        {
            back_to_categories(); //category died
        }

        return true;
    }


    bool back_to_slider()
    {
        vendor_not_enough_gold_confirming = false;
        vendor_not_enough_gold_confirmed = false;
        vendor_not_enough_gold_choice = false;
        vendor_not_enough_gold_choice_valid = false;
        vendor_not_enough_gold_request_sent = false;
        slider_confirming = false;
        slider_confirmed = false;
        slider_choice_valid = false;
        slider_choice = 0;
        slider_request_sent = false;

        return true;
    }

    bool back_to_barter_type_selection()
    {
        left();
        barter_reset();
        return true;
    }




    void fill_categories_list_one_direction(barter_type type, bool direction_down, float dtime)
    {
        int max_scroll = get_category_max_scroll_position();
        int scroll = get_category_scroll_position();

        if (scroll == last_scroll && !scroll_stuck)
            scroll_stuck_time += dtime;
        else
            scroll_stuck_time = 0.0f;

        if (scroll_stuck_time > 1.0f)
        {
            scroll_stuck = true;
            scroll_stuck_time = 0.0f;
            last_scroll = -1;
        }
        last_scroll = scroll;


        if (direction_down)
        {
            if (scroll < max_scroll)
            {
                update_barter_categories(type); //do this on every scroll
                if (scroll_stuck)
                    perk_down();
                else
                    cursor_down();
            }
            else
            {
                update_barter_categories(type); //do this on every scroll
                if (scroll_stuck)
                    filling_categories = false; //restart fill
                else
                    fill_categories_one_direction_done = true;
            }
        }
        else
            if ((type == barter_type::buy && scroll > 0) || (type == barter_type::sell && scroll > 11))
            {
                update_barter_categories(type); //do this on every scroll
                if (scroll_stuck)
                    perk_up();
                else
                    cursor_up();
            }
            else
            {
                update_barter_categories(type); //do this on every scroll
                if (scroll_stuck)
                    filling_categories = false; //restart fill
                else
                    fill_categories_one_direction_done = true;
            }
    }




    void fill_items_list_one_direction(barter_type type, bool direction_down, float dtime)
    {
        int max_scroll = get_item_max_scroll_position();
        int scroll = get_item_scroll_position();

        if (scroll == last_scroll && !scroll_stuck)
            scroll_stuck_time += dtime;
        else
            scroll_stuck_time = 0.0f;

        if (scroll_stuck_time > 1.0f)
        {
            scroll_stuck = true;
            scroll_stuck_time = 0.0f;
            last_scroll = -1;
        }
        last_scroll = scroll;


        if (direction_down)
        {
            if (scroll < max_scroll)
            {
                update_barter_items(); //do this on every scroll
                if (scroll_stuck)
                    perk_down();
                else
                    cursor_down();
            }
            else
            {
                update_barter_items(); //do this on every scroll
                if (scroll_stuck)
                    filling_items = false; //restart fill
                else
                    fill_items_one_direction_done = true;
            }
        }
        else
            if (scroll > 0)
            {
                update_barter_items(); //do this on every scroll
                if (scroll_stuck)
                    perk_up();
                else
                    cursor_up();
            }
            else
            {
                update_barter_items(); //do this on every scroll
                if (scroll_stuck)
                    filling_items = false; //restart fill
                else
                    fill_items_one_direction_done = true;
            }
    }





    void fill_categories_list(float dtime)
    {
        switch (way) {
        case (way_to_fill::down):
        {
            if (fill_categories_one_direction_done)
                categories_list_valid = true;
            else
                fill_categories_list_one_direction(type, true, dtime);

            break;
        }

        case (way_to_fill::up):
        {
            if (fill_categories_one_direction_done)
                categories_list_valid = true;
            else
                fill_categories_list_one_direction(type, false, dtime);

            break;
        }

        case (way_to_fill::downup):
        {
            if (down_done)
                if (fill_categories_one_direction_done)
                    categories_list_valid = true;
                else
                    fill_categories_list_one_direction(type, false, dtime);
            else
                if (fill_categories_one_direction_done)
                {
                    down_done = true;
                    fill_categories_one_direction_done = false;
                }
                else
                    fill_categories_list_one_direction(type, true, dtime);

            break;
        }

        case (way_to_fill::updown):
        {
            if (up_done)
                if (fill_categories_one_direction_done)
                    categories_list_valid = true;
                else
                    fill_categories_list_one_direction(type, true, dtime);
            else
                if (fill_categories_one_direction_done)
                {
                    up_done = true;
                    fill_categories_one_direction_done = false;
                }
                else
                    fill_categories_list_one_direction(type, false, dtime);

            break;
        }
        }
    }



    void fill_items_list(float dtime)
    {
        switch (way) {
        case (way_to_fill::down):
        {
            if (fill_items_one_direction_done)
                items_list_valid = true;
            else
                fill_items_list_one_direction(type, true, dtime);

            break;
        }

        case (way_to_fill::up):
        {
            if (fill_items_one_direction_done)
                items_list_valid = true;
            else
                fill_items_list_one_direction(type, false, dtime);

            break;
        }

        case (way_to_fill::downup):
        {
            if (down_done)
                if (fill_items_one_direction_done)
                    items_list_valid = true;
                else
                    fill_items_list_one_direction(type, false, dtime);
            else
                if (fill_items_one_direction_done)
                {
                    down_done = true;
                    fill_items_one_direction_done = false;
                }
                else
                    fill_items_list_one_direction(type, true, dtime);

            break;
        }

        case (way_to_fill::updown):
        {
            if (up_done)
                if (fill_items_one_direction_done)
                    items_list_valid = true;
                else
                    fill_items_list_one_direction(type, true, dtime);
            else
                if (fill_items_one_direction_done)
                {
                    up_done = true;
                    fill_items_one_direction_done = false;
                }
                else
                    fill_items_list_one_direction(type, false, dtime);

            break;
        }
        }
    }



    void setup_fill_category_list()
    {
        int min_scroll = 0;
        int max_scroll = get_category_max_scroll_position();
        int scroll = get_category_scroll_position();

        last_cursor_move = 0;
        missing_category_detected = false;

        up_done = false;
        down_done = false;
        fill_categories_one_direction_done = false;
        scroll_stuck = false;


        if (scroll <= min_scroll)
            way = way_to_fill::down;
        else
            if (scroll >= max_scroll)
                way = way_to_fill::up;
            else
                if (scroll <= (max_scroll - min_scroll) / 2)
                    way = way_to_fill::updown;
                else
                    way = way_to_fill::downup;
    }




    void setup_fill_item_list()
    {
        int min_scroll = 0;
        int max_scroll = get_item_max_scroll_position();
        int scroll = get_item_scroll_position();

        last_cursor_move = 0;
        missing_item_detected = false;

        up_done = false;
        down_done = false;
        fill_items_one_direction_done = false;
        scroll_stuck = false;

        if (scroll <= min_scroll)
            way = way_to_fill::down;
        else
            if (scroll >= max_scroll)
                way = way_to_fill::up;
            else
                if (scroll <= (max_scroll - min_scroll) / 2)
                    way = way_to_fill::updown;
                else
                    way = way_to_fill::downup;
    }




    std::string old_item_choice_text = "";


    std::string get_item_text_by_id(int id)
    {
        auto item_pos = items_list.find(id);

        if (item_pos == items_list.end())
            return "";
        else
            //THIS WORKED SUPRISINGLY WELL! maybe leave it like this? its not reliable though and we dont know the actual result (was it not enough gold or we bought the thing..)
            return item_pos->second.name;// +" - " + std::to_string(item_pos->second.price) + " gold";
    }

    bool detect_cancel_not_enough_gold_barter_result()
    {
        if (vendor_not_enough_gold_message() == "")
        {
            return true; //prompt disappeared
        }

        return false;
    }


    bool detect_item_barter_result(bool ignore_slider, bool ignore_vendor_not_enough_gold)
    {
        if (!update_barter_item_by_id(item_choice))
        {
            old_item_choice_text = "";
            return true; //item disappeared
        }

        if (have_not_enough_gold_message())
        {
            send_random_context("[Not enough gold!]");
            old_item_choice_text = "";
            return true; //not enough gold
        }

        if (!ignore_slider && quantity_slider_active())
        {
            return true; //processed later in algorithm
        }

        if (!ignore_vendor_not_enough_gold && vendor_not_enough_gold_message() != "")
        {
            return true; //processed later in algorithm
        }


        std::string item_choice_text = get_item_text_by_id(item_choice);

        if (item_choice_text == "")
        {
            old_item_choice_text = "";
            return true; //item disappeared
        }
        else
            if (item_choice_text != old_item_choice_text)
            {
                old_item_choice_text = "";
                return true; //text changed. probably quantity.
            }


        //TODO: popups


        return false; //nothing seem to be changed
    }




    void finish_transaction()
    {
        if (!block_set)
        {
            set_universal_block(0.5f);
            block_set = true;
        }
        else
        {
            item_confirmed = false;
            block_set = false;
            back_to_items();
        }
    }


    std::string get_barter_type_text()
    {
        std::string result = "";
        if (type == barter_type::sell)
            result = "sell";
        else
            result = "buy";

        return result;
    }

    float barter_processor_timer = 0;
    void processor(float dtime)
    {

        RE::UI* ui = RE::UI::GetSingleton();

        in_barter = ui->IsMenuOpen(RE::BarterMenu::MENU_NAME);


        if (barter_processor_timer > 0.02f)
        {
            barter_processor_timer = 0;

            if (in_barter)
            {
                WalkerProcessor::reset_walker();

                if (barter_type_defined)
                {
                    if (!categories_list_valid)
                        if (!filling_categories)
                        {
                            setup_fill_category_list();
                            filling_categories = true;
                        }
                        else
                            fill_categories_list(0.02f);
                    else
                    {
                        if (!barter_category_request_sent)
                        {

                            if (force_choice(get_barter_categories(), "You are bartering. Choose item category to " + get_barter_type_text(), force_type::barter_category))
                            {
                                missing_category_detected = false;
                                last_cursor_move = 0;
                                barter_category_request_sent = true;
                            }
                        }
                        else
                        {
                            if (category_choice_valid)
                            {
                                if (get_category_selected_index() != category_choice && !item_confirming && !item_confirmed)
                                {
                                    if (!missing_category_detected)
                                        move_cursor_to_barter_category(category_choice);
                                    else
                                        barter_reset_categories_selection();
                                }
                                else
                                    //if (get_selected_category() != category_choice && !item_confirming && !item_confirmed) 
                                    if (!inside_of_category() && !item_confirming && !item_confirmed)
                                        confirm();
                                    else
                                    {   //NOW ITEMS
                                        if (!items_list_valid)
                                            if (!filling_items)
                                            {
                                                setup_fill_item_list();
                                                filling_items = true;
                                            }
                                            else
                                                fill_items_list(0.02f);
                                        else
                                        {
                                            if (!barter_item_request_sent && !item_confirming && !item_confirmed)
                                            {

                                                if (force_choice(get_barter_items(), "You are bartering. Choose item to " + get_barter_type_text(), force_type::barter_item))
                                                {
                                                    missing_item_detected = false;
                                                    last_cursor_move = 0;
                                                    barter_item_request_sent = true;
                                                }
                                            }
                                            else
                                            {
                                                if (item_choice_valid)
                                                {
                                                    int selected_index = get_item_selected_index();
                                                    if (selected_index != item_choice && !item_confirmed && !item_confirming)
                                                    {
                                                        if (!missing_item_detected)
                                                            move_cursor_to_barter_item(item_choice);
                                                        else
                                                            back_to_items();
                                                    }
                                                    else
                                                    {
                                                        if (!old_item_choice_text_valid)
                                                        {
                                                            old_item_choice_text = get_item_text_by_id(item_choice);
                                                            old_item_choice_text_valid = true;
                                                        }

                                                        if (!item_confirmed) //TODO: replace with actual check
                                                        {
                                                            if (detect_item_barter_result(false, false))
                                                            {
                                                                item_confirming = false;
                                                                item_confirmed = true;
                                                                //RE::DebugMessageBox("SOLD");//this works but it gives message box with OK button. not what i need
                                                            }
                                                            else
                                                            {
                                                                //leftclick();
                                                                confirm();
                                                                item_confirming = true;
                                                            }

                                                        }
                                                        else
                                                        {
                                                            if (quantity_slider_active())
                                                            {
                                                                if (!slider_request_sent)
                                                                {
                                                                    
                                                                    if (force_choice({}, "You are bartering. Choose amount of " + get_item_text_by_id(item_choice) + " to " + get_barter_type_text() +
                                                                        ". Valid range: from " + std::to_string(get_slider_min()) + " to " + std::to_string(get_slider_max()), force_type::barter_quantity))
                                                                        slider_request_sent = true;
                                                                }
                                                                else
                                                                {
                                                                    if (slider_choice_valid)
                                                                    {
                                                                        if (get_slider_pos() != slider_choice)
                                                                            move_slider_to_pos(slider_choice);
                                                                        else
                                                                        {
                                                                            if (!slider_confirmed)
                                                                            {
                                                                                if (detect_item_barter_result(true, false))
                                                                                {
                                                                                    slider_confirming = false;
                                                                                    slider_confirmed = true;
                                                                                }
                                                                                else
                                                                                {
                                                                                    confirm();
                                                                                    slider_confirming = true;
                                                                                }
                                                                            }
                                                                            else
                                                                            {


                                                                                if (std::string test = vendor_not_enough_gold_message(); test != "")
                                                                                {
                                                                                    if (!vendor_not_enough_gold_request_sent)
                                                                                    {
                                                                                        
                                                                                        if (force_choice({ {0, "No"}, {1, "Yes"}, {-1, "[QUIT BARTER]"} }, test + " Confirm?", force_type::barter_vendor_not_enough_gold))
                                                                                            vendor_not_enough_gold_request_sent = true;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        if (vendor_not_enough_gold_choice_valid)
                                                                                        {
                                                                                            if (vendor_not_enough_gold_choice == 1)
                                                                                            {
                                                                                                if (!vendor_not_enough_gold_confirmed)
                                                                                                {
                                                                                                    if (detect_item_barter_result(false, true))
                                                                                                    {
                                                                                                        vendor_not_enough_gold_confirming = false;
                                                                                                        vendor_not_enough_gold_confirmed = true;
                                                                                                    }
                                                                                                    else
                                                                                                    {
                                                                                                        vendor_not_enough_gold_confirming = true;
                                                                                                        confirm();
                                                                                                    }
                                                                                                }
                                                                                                else
                                                                                                {
                                                                                                    finish_transaction();
                                                                                                }
                                                                                            }
                                                                                            else
                                                                                            {
                                                                                                if (!vendor_not_enough_gold_confirmed)
                                                                                                {
                                                                                                    if (detect_cancel_not_enough_gold_barter_result())
                                                                                                    {
                                                                                                        vendor_not_enough_gold_confirming = false;
                                                                                                        vendor_not_enough_gold_confirmed = true;
                                                                                                    }
                                                                                                    else
                                                                                                    {
                                                                                                        vendor_not_enough_gold_confirming = true;
                                                                                                        cancel(); //how to detect this?
                                                                                                    }
                                                                                                }
                                                                                                else
                                                                                                {
                                                                                                    //BACK TO SLIDER?
                                                                                                    //back_to_slider(); //this is actually useless, the game itself cancels the slider
                                                                                                    back_to_items();

                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                                else
                                                                                {
                                                                                    finish_transaction();
                                                                                }

                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                            else
                                                            {
                                                                if (std::string test = vendor_not_enough_gold_message(); test != "")
                                                                {
                                                                    if (!vendor_not_enough_gold_request_sent)
                                                                    {
                                                                        
                                                                        if (force_choice({ {0, "No"}, {1, "Yes"}, {-1, "[QUIT BARTER]"} }, test + " Confirm?", force_type::barter_vendor_not_enough_gold))
                                                                            vendor_not_enough_gold_request_sent = true;
                                                                    }
                                                                    else
                                                                    {
                                                                        if (vendor_not_enough_gold_choice_valid)
                                                                        {
                                                                            if (vendor_not_enough_gold_choice == 1)
                                                                            {
                                                                                if (!vendor_not_enough_gold_confirmed)
                                                                                {
                                                                                    if (detect_item_barter_result(false, true))
                                                                                    {
                                                                                        vendor_not_enough_gold_confirming = false;
                                                                                        vendor_not_enough_gold_confirmed = true;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        vendor_not_enough_gold_confirming = true;
                                                                                        confirm();
                                                                                    }
                                                                                }
                                                                                else
                                                                                {
                                                                                    finish_transaction();
                                                                                }
                                                                            }
                                                                            else
                                                                            {
                                                                                if (!vendor_not_enough_gold_confirmed)
                                                                                {
                                                                                    if (detect_item_barter_result(false, true))
                                                                                    {
                                                                                        vendor_not_enough_gold_confirming = false;
                                                                                        vendor_not_enough_gold_confirmed = true;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        vendor_not_enough_gold_confirming = true;
                                                                                        cancel();
                                                                                    }
                                                                                }
                                                                                else
                                                                                {
                                                                                    finish_transaction();
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    finish_transaction();
                                                                }

                                                            }
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    ;//todo: check for how long it remains invalid. resent request for item if invalid for too long
                                                }
                                            }
                                        }

                                    }

                            }
                            else
                            {
                                ;//todo: check for how long it remains invalid. resent request for category if invalid for too long
                            }
                        }
                    }

                }
                else
                {
                    if (!barter_type_request_sent)
                    {
                        
                        if (force_choice({ {0, "Sell"},{1, "Buy"}, {-1, "[QUIT BARTER]"} }, "You are bartering. Choose barter type", force_type::barter_type_force))
                            barter_type_request_sent = true;
                    }

                }

            }
            else
            {
                barter_reset();
            }
        }
        else
            barter_processor_timer += dtime;



    }
}







std::string scan_entries()
{
    std::string results = "\n";

    RE::GFxValue var1;
    RE::UI* ui = RE::UI::GetSingleton();
    if (ui)
        if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
            if (menu->uiMovie)
                if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList")) //this gives only categories that appear on screen right now. if you scroll and some become invisible - this will not show them
                {
                    std::string path_root = "_root.Menu_mc.InventoryLists_mc._CategoriesList.";

                    for (int i = 0; i < 30; i++)
                    {
                        std::string full_path = path_root + "Entry" + std::to_string(i);
                        if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))
                        {
                            if (menu->uiMovie->GetVariable(&var1, (full_path + ".itemIndex").c_str()))
                            {
                                //got indexed item
                                if (!var1.IsNull() && var1.IsNumber())
                                {
                                    int index = var1.GetNumber();
                                    std::string name = "";
                                    if (menu->uiMovie->GetVariable(&var1, (full_path + ".textField.text").c_str()))
                                        if (!var1.IsNull() && var1.IsString())
                                            name = var1.GetString();

                                    if (name != "")
                                        results += std::to_string(index) + ":" + name + "\n";
                                }


                            }
                        }
                    }
                }

    return results;
}



float debug_timer = 0.0f;
bool list_was_sent = false;
bool vars_were_dumped = false;


void debug_scan(float dtime)
{
    if (debug_timer > 1.0f)
    {
        debug_timer = 0.0f;
    

    RE::UI* ui = RE::UI::GetSingleton();
    RE::GFxValue gfx_selected_list_index;

    if (!list_was_sent)
    {
        if (auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
        {
            if (menu->itemList)
            {
                RE::BSTArray<RE::ItemList::Item*> items = menu->itemList->items;

                std::string result = "";

                for (RE::ItemList::Item* item : items)
                {
                    result += item->data.GetName();
                    result += " x";
                    result += std::to_string(item->data.GetCount());
                    result += "\n"; //TODO: replace with comma later

                    //item->data.GetFilterFlag
                    //item->data.objDesc->GetValue;
                }

                send_random_context(result);
                list_was_sent = true;

                ui = nullptr;

            }

        }
    }

    


    RE::GFxValue var1;

    
    /*
    auto now = std::chrono::steady_clock::now();

    if ((double)now.time_since_epoch().count() / 1000000.0 - debug_time > 1000.0f)
    {
        //send_random_context(std::to_string(get_current_category_index()).c_str()); 
        //send_random_context(scan_entries().c_str());

        std::string message = "";
        message += std::to_string(get_category_selected_index()) + " - selected category\n";
        message += std::to_string(get_category_scroll_position()) + "/" + std::to_string(get_category_max_scroll_position()) + " - category scroll\n";
        message += std::to_string(get_item_selected_index()) + " - selected item\n";
        message += std::to_string(get_item_scroll_position()) + "/" + std::to_string(get_item_max_scroll_position()) + " - item scroll\n";

        send_random_context(message.c_str());

        if (get_category_scroll_position() > -1)
        {
            //set_category_scroll_position(scroll_dbg);
            scroll_dbg++;
        }

        debug_time = (double)now.time_since_epoch().count() / 1000000.0f;
    }
        */

    


    
    /*
            //plan: scroll iScrollPosition until it reaches iMaxScrollPosition
            //while adding new items to the list of results
            

            iScrollPosition
            iMaxScrollPosition



            iMaxScrollPosition from category lists - gives max index available.
            clipIndex inside of Entry to get its index.
            iMaxScrollPosition and clipIndex together should give full info if we have everything availabie shown
            or maybe iScrollPosition.

            just scroll through entire list up and down (calculate whats closer)


    +		[9]	"_root.Menu_mc.InventoryLists_mc._ItemsList.iSelectedIndex = 3.000000"  //this is selected ITEM in entire list of items






    //category (armor/apparel/books..)
            "_root.Menu_mc.InventoryLists_mc.CategoriesListHolder.List_mc.iSelectedIndex = 2.000000"    //keyboard (unsure)
+			"_root.Menu_mc.InventoryLists_mc._CategoriesList.iSelectedIndex = 2.000000"                 //cursor (unsure)
+		
+		
        //these were -1. TODO: investigate
+		[0]	    "_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.CardList_mc.List_mc.iSelectedIndex"	
+		[1]	    "_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.ItemList.iSelectedIndex"	
+		[2]	    "_root.Menu_mc.InventoryLists_mc.ItemsListHolder.List_mc.iSelectedIndex"	
+		[5]	    "_root.Menu_mc.InventoryLists_mc._ItemsList.iSelectedIndex"
+		[10]    "_root.Menu_mc.ItemCard_mc.CardList_mc.List_mc.iSelectedIndex"	
+		[11]    "_root.Menu_mc.ItemCard_mc.ItemList.iSelectedIndex"	

    */

 
    //visit members (recursive)
    

    //player.advskill destruction 10000

    std::vector<std::string> results;

    if (!vars_were_dumped)
    {
        results.clear();




        if (const auto ui = RE::UI::GetSingleton(); ui) {
            //if (const auto menu = ui->GetMenu<RE::LevelUpMenu>(); menu) {
            if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu) {
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root"))
                    {
                        //std::string result;
                        if (true)
                        {
                            //int tabs1 = 0;
                            //std::ofstream fs;// ("dumpVarPaths2.txt");
                            //visit_all_members(false, fs, var1, &search_var, 0);
                            //fs.close();


                            //+		[0]	"_root.LevelUpMenu_mc.HealthButton"	std::string

                            bool search_success = false;
                            
                            std::string search_var = "_focused";

                            //const auto menu2 = ui->GetMenu<RE::LevelUpMenu>();

                            /*
                            search_success = visit_all_members2(results, var1, &search_var, 0, "_root");
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }
                            */

                            std::vector<std::string> skip_problematic = {
                                //"ItemInfo"
                            };

                            std::vector<std::string> skip_problematic_path = {
                                //"_root.Menu.ItemInfoHolder.ItemInfo"
                            };


                            search_var = "onPress";
                            //search_var = "Body";

                            //const auto menu2 = ui->GetMenu<RE::LevelUpMenu>();

                            //_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA[46].0.sliderMax
                           // search_success = visit_all_members2(results, var1, &search_var, 0, "_root", skip_problematic, skip_problematic_path, "", "");
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }
                            
                            search_var = "iScrollPosition";
                            //search_var = "Body";

                            //const auto menu2 = ui->GetMenu<RE::LevelUpMenu>();


                            //search_success = visit_all_members2(results, var1, &search_var, 0, "_root", skip_problematic, skip_problematic_path, "", "");
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }


                            search_var = "TextEntry";
                            //search_var = "Body";

                            //const auto menu2 = ui->GetMenu<RE::LevelUpMenu>();


                            search_success = visit_all_members2(results, var1, &search_var, 0, "_root", skip_problematic, skip_problematic_path, "", "");
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }


                            //SEARCH PART OF STRING
                            //bool search_success = visit_all_members3(results, var1, &search_var, 0, "_root", "How many");

                            
                            //button 4?
                            

                            //textColor = 16777215.000000

                            /*
                            search_success = visit_all_members3(results, var1, &search_var, 0, "_root", "Race", skip_problematic);
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }
                            */


                            search_var = "Enter the Keep";
                            search_success = visit_all_members3(results, var1, &search_var, 0, "_root", search_var, skip_problematic);
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }


                            //search_success = visit_all_members3(results, var1, &search_var, 0, "_root", "Take", skip_problematic);
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }

                            
                            /*
                            search_success = visit_all_members3(results, var1, &search_var, 0, "_root", "magic", skip_problematic);
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }
                            */

                            /*
                            //SEARCH EXACT NUMBER IN FORM OF STRING
                            search_success = visit_all_members4(results, var1, &search_var, 0, "_root", 76);
                            if (search_success)
                            {
                                auto test = var1.GetType();
                            }

                            */

                        }
                        auto test = var1.GetType();
                        //vars_were_dumped = true;
                    }
            }
        }
    }



    /*  //top left message array
+		[0]	"_root.HUDMovieBaseInstance.MessagesBlock.ShownMessageArray[0]"	std::string
+		[1]	"_root.HUDMovieBaseInstance.HudElements[24].12.ShownMessageArray[0]"	std::string
+		[2]	"_root.HUDMovieBaseInstance.MessagesInstance.ShownMessageArray[0]"	std::string


+		[3]	"_root.Menu_mc.ItemCard_mc.QuantitySlider_mc"	std::string
+ [17]	"_focused = 0.000000"	std::string

            "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._focused" - slider active
            _focused - slider active
            _minimum
            _maximum
            _value - current position



+		[1]	"_root.Menu_mc.ItemCardFadeHolder_mc.ItemCard_mc.MessageText.text = Transaction value: 606 gold.\rVendor only has 179 gold."	std::string





+		[0]	"StatsMenuBaseInstance{obj}"	std::string
+		[1]	"PickInstance{obj}"	std::string     //seems to be useless
+		[2]	"PerkInstance{obj}"	std::string     //seems to be useless
+		[3]	"PerksInstance{obj}"	std::string //looks like caption "Perks to increase: " (when we have available perk points)



_root.StatsMenuBaseInstance.CameraMovementInstance.CameraPositionAlpha - full camera contents, seems to be useless?

_root.StatsMenuBaseInstance.PerkName0 has bPlaying that is set to 1 when we are zoomed

+		[0]	"PerkNameClipInstance{obj}"	std::string


*/

    
    std::vector<std::string> paths_to_var_to_check = { 
        //"_root.Menu.ItemInfo",
        //"_root.Menu.ItemInfoHolder",
        //"_root.Menu.ItemInfo.EnchantingSlider_mc",
        //"_root.Menu.ItemInfoHolder.ItemInfo",
        //"_root.Bottom.LocalMapButton",
        //"_root.Bottom.LocalMapButton.border",
        //"_root.Bottom.LocalMapButton.constraints",
        //"_root.Bottom.LocalMapButton.constraints.elements.0",
        
        //"_root.Menu_mc.ItemCard_mc.ItemValueText"
        //"_root.HUDMovieBaseInstance.QuestUpdateBaseInstance.LevelUpTextInstance",
        //"_root.HUDMovieBaseInstance.HudElements.14.LevelUpTextInstance"
        //"_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA.0"
       // "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._TextEntryField",
       // "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._TextEntryField.TextField_txt",
       // "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._TextEntryField.TextInputInstance"
        //"_root.Bottom.LocalMapButton.constraints.elements.0.metrics",
        //"_root.Bottom.LocalMapButton.constraints.elements.0.clip",
        //"_root.Bottom.LocalMapButton.constraints.scope"

        "_root.SleepWaitMenu_mc",
        "_root.SleepWaitMenu_mc.HoursSlider"



        //"_root.Menu.InventoryLists._ItemsList.Entry0",
        //"_root.Menu.InventoryLists._ItemsList.Entry0.textField"
        //"_root.Menu.MenuNameHolder.MenuName",
       // "_root.Menu.MenuName",
//"_root.Menu.MenuDescription.text = Alchemy: Combine ingredients to make potions",
//"_root.Menu.BottomBarInfo.PlayerInfoCard_mc.SkillLevelLabel.text = Alchemy",

        //"_root.StatsMenuBaseInstance.AddPerkTextInstance.AddPerkTextField",
        //"_root.StatsMenuBaseInstance.AddPerkTextInstance"
        
        //"_root.StatsMenuBaseInstance.PerkName0.PerkNameClipInstance.NameText",
        //"_root.StatsMenuBaseInstance.PerkName0.PerkNameClipInstance.NameText",
        //"_root.StatsMenuBaseInstance.AddPerkTextInstance",
        //"_root.StatsMenuBaseInstance.AnimatingSkillTextInstance",
        //"_root.StatsMenuBaseInstance.AnimatingSkillTextInstance.SkillText0",
       // "_root.StatsMenuBaseInstance.AnimatingSkillTextInstance.SkillText0.LabelInstance",
        //"_root.StatsMenuBaseInstance.AnimatingSkillTextInstance.SkillText0.ShortBar"





        /*
        +		[0]	"LabelInstance{obj}"	std::string
+		[1]	"ShortBar{obj}"	std::string

*/
    };

   // std::string path_to_var_to_check2 = "_root.StatsMenuBaseInstance.PerkName0.PerkNameClipInstance.NameText";

    //visit members (not recursive)
    std::vector<std::vector<std::string>> names_all;

    names_all.clear();

    if (const auto ui = RE::UI::GetSingleton(); ui) 
        //if (const auto menu = ui->GetMenu<RE::HUDMenu>(); menu) 
        //if (const auto menu = ui->GetMenu<RE::LevelUpMenu>(); menu)
        if (const auto menu = ui->GetMenu<RE::TrainingMenu>(); menu)
        {

            if (menu->uiMovie)

                for (std::string path_to_var_to_check : paths_to_var_to_check)
                {
                    std::vector<std::string> names{};
                    if (menu->uiMovie->GetVariable(&var1, path_to_var_to_check.c_str()))
                    {
                        if (var1.IsObject())
                        {
                            var1.VisitMembers([&](const char* name, const RE::GFxValue& a_value)
                                {
                                    std::string name_str = name;
                                    std::string name_orig = name;

                                    if (!a_value.IsNull())
                                    {
                                        if (a_value.IsObject())
                                            name_str += "{obj}";
                                        else
                                        {
                                            if (a_value.IsArray())
                                                name_str += "[" + std::to_string(a_value.GetArraySize()) + "]";
                                            if (a_value.IsNumber())
                                                name_str += " = " + std::to_string(a_value.GetNumber());
                                            if (a_value.IsBool())
                                                name_str += " = " + std::to_string(a_value.GetBool());
                                            if (a_value.IsString())
                                            {
                                                std::string string_val = a_value.GetString();
                                                name_str += " = " + string_val;
                                            }
                                        }
                                    }
                                    else
                                        int isnullwtf = 1;

                                    names.push_back(name_str);

                                });
                        }
                        auto test = var1.GetType();
                    }

                    names_all.push_back(names);

                }
        }
            

    

    if (std::size(names_all) != 0)
    {
        bool have_anything = false;
        for (auto names : names_all)
            if (std::size(names) != 0)
                have_anything = true;


        if (have_anything)
            auto test = var1.GetType();
    }
    
                
    }
    else
        debug_timer += dtime;
}


/*


+		[0]	"_root.MessageMenu.Buttons.Button0.onPress"	std::string
+		[1]	"_root.MessageMenu.Buttons.Button1.onPress"	std::string
+		[2]	"_root.MessageMenu.ButtonContainer.Button0.onPress"	std::string
+		[3]	"_root.MessageMenu.ButtonContainer.Button1.onPress"	std::string
+		[4]	"_root.MessageMenu.MessageButtons[2].0.onPress"	std::string
+		[5]	"_root.MessageMenu.MessageButtons[2].1.onPress"	std::string
+		[6]	"_root.MessageMenu.MessageText.htmlText = Increase Health?"	std::string
+		[7]	"_root.MessageMenu.MessageText.text = Increase Health?"	std::string
+		[8]	"_root.MessageMenu.Message.htmlText = Increase Health?"	std::string
+		[9]	"_root.MessageMenu.Message.text = Increase Health?"	std::string



*/








//this is useless (it changes graphical part but doesnt actually change selection and messes everything up)
/*
void set_category_scroll_position(int i)
{
    RE::GFxValue var1;
    RE::UI* ui = RE::UI::GetSingleton();
    if (ui)
        if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
            if (menu->uiMovie)
                if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList.iScrollPosition"))
                {
                    //menu->uiMovie->SetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList.iScrollPosition")
                    //menu->uiMovie->SetVariable("_root.Menu_mc.InventoryLists_mc._CategoriesList.iScrollPosition", std::to_wstring(i).c_str());
                    menu->uiMovie->SetVariable("_root.Menu_mc.InventoryLists_mc._CategoriesList.iScrollPosition", (double)i);
                }
}
*/


/*
void confirmOLD()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
}
*/




//OLD VERSION
/*
std::vector<MenuOption> get_barter_categories(barter_type type) //OLD VERSION
{
    std::vector<MenuOption> results{};

    RE::GFxValue var1;
    RE::UI* ui = RE::UI::GetSingleton();
    if (ui)
        if (const auto menu = ui->GetMenu<RE::BarterMenu>(); menu)
            if (menu->uiMovie)
                if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._CategoriesList")) //this gives only categories that appear on screen right now. if you scroll and some become invisible - this will not show them
                {
                    std::string path_root = "_root.Menu_mc.InventoryLists_mc._CategoriesList.";

                    for (int i = 0; i < 30; i++)
                    {
                        std::string full_path = path_root + "Entry" + std::to_string(i);
                        if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))
                        {
                            if (menu->uiMovie->GetVariable(&var1, (full_path + ".itemIndex").c_str()))
                            {
                                //got indexed item
                                if (!var1.IsNull() && var1.IsNumber())
                                {
                                    MenuOption result{};

                                    result.id = var1.GetNumber();
                                    std::string name = "";
                                    if (menu->uiMovie->GetVariable(&var1, (full_path + ".textField.text").c_str()))
                                        if (!var1.IsNull() && var1.IsString())
                                            name = var1.GetString();

                                    if (name != "")
                                        switch (type)
                                        {
                                            case barter_type::buy:
                                            {
                                                if (result.id > 0 && result.id < 11) //buy range
                                                {
                                                    result.text = name;
                                                    results.push_back(result);
                                                }
                                                break;
                                            }

                                            case barter_type::sell:
                                            {
                                                if (result.id > 11 && result.id < 30) //sell range
                                                {
                                                    result.text = name;
                                                    results.push_back(result);
                                                }
                                                break;
                                            }

                                        }

                                }
                            }
                        }
                    }
                }

    return results;
}
*/


