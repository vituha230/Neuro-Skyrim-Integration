

#include "InputActions.hpp"
#include "DialogueProcessor.hpp"
#include "main.hpp"
#include "Socket.hpp"
#include "WalkerProcessor.hpp"





//namespace force {
    int active_force = -1;

    int get_active_force()
    {
        return active_force;
    }

    void set_active_force(int id)
    {
        active_force = id;
    }
//}



namespace DialogueProcessor {


    bool dialogue_choice_made = false;
    int dialogue_choice = 0;
    float dialogue_proc_time = 0.0f;
    bool in_dialogue = false;
    float pause_time = 0.0f;



    RE::MenuTopicManager::Dialogue old_dialogue;

    void clean_old_dialogue()
    {
        old_dialogue.topicText = "";
    }

    bool is_in_dialogue(RE::TESObjectREFR* speaker)
    {
        bool result = false;

        if (!speaker)
        {
            RE::UI* ui = RE::UI::GetSingleton();
            return ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME);
        }
        else
        {
            if (in_dialogue)
            {
                RE::MenuTopicManager* topic_manager = RE::MenuTopicManager::GetSingleton();
                if (topic_manager)
                    if (topic_manager->speaker.get())
                    {
                        auto speaker_ref = topic_manager->speaker.get().get();
                        if (speaker_ref == speaker)
                            result = true;
                    }
            }

            
        }

        return result;
    }



    void dialogue_choice_reset()
    {
        dialogue_choice = 0;
    }


    std::string get_chosen_line_text(int chosen_id)
    {
        std::string result = "";

        RE::MenuTopicManager* topic_manager = RE::MenuTopicManager::GetSingleton();

        if (topic_manager)
            if (topic_manager->dialogueList)
                if (topic_manager->dialogueList->front())
                {
                    int id = 0;
                    for (auto& dialogue : *topic_manager->dialogueList)
                    {
                        MenuOption option;
                        option.id = id;
                        option.text = dialogue->topicText.c_str();


                        if (id == chosen_id)
                        {
                            result = dialogue->topicText;
                            return result;
                        }

                        id++;
                    }
                }

        return result;
    }



    std::vector<MenuOption> get_dialogue_options()
    {
        std::vector<MenuOption> dialogue_options;

        RE::MenuTopicManager* topic_manager = RE::MenuTopicManager::GetSingleton();

        if (topic_manager)
            if (topic_manager->dialogueList)
                if (topic_manager->dialogueList->front())
                {
                    int id = 0;
                    for (auto& dialogue : *topic_manager->dialogueList)
                    {
                        MenuOption option;
                        option.id = id;
                        option.text = dialogue->topicText.c_str();

                        dialogue_options.push_back(option);

                        id++;
                        //RE::ConsoleLog::GetSingleton()->Print(dialogue->topicText.c_str());
                    }

                    dialogue_options.push_back({ -1, "[STOP DIALOGUE]" });
                }

        return dialogue_options;
    }



    int count_dialogue_options()
    {
        RE::MenuTopicManager* topic_manager = RE::MenuTopicManager::GetSingleton();
        int count_lines = 0;
        if (topic_manager)
            if (topic_manager->dialogueList)
                if (!topic_manager->dialogueList->empty() && topic_manager->dialogueList->front())
                    for (auto& dialogue : *topic_manager->dialogueList)
                        count_lines++;

        return count_lines;
    }


    bool quit_menu()
    {
        RE::UIMessageQueue::GetSingleton()->AddMessage(RE::DialogueMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
        return true;
    }

    std::pair<bool, std::string> say_chosen_line(int id)
    {
        std::pair<bool, std::string> result{};

        if (id == -1)
        {
            //RE::MenuTopicManager* topic_manager = RE::MenuTopicManager::GetSingleton();
            //if (topic_manager)
            //{
            //    bool stop_here = false;
            //}
                //if (topic_manager->dialogueList)
                 //   if (!topic_manager->dialogueList->empty() && topic_manager->dialogueList->front())


            quit_menu();
            result.first = true;
            result.second = "[Stopped the dialogue]";
            return result;
        }

        int amount_lines = count_dialogue_options();

        if (dialogue_choice > amount_lines - 1 || dialogue_choice < 0)
        {
            result.first = false;
            result.second = "Invalid choice ID";
            return result;
        }

        RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
        dialogue_choice = id;
        dialogue_choice_made = true;
        dialogue_proc_time = 100.0f;
        //dialogue_processor_skip_all = true;

        result.first = true;
        result.second = "[Dialogue] [You said: \""+ get_chosen_line_text(id) + "\"]";

        return result;

    }





    //this gives current selected index of dialogue options.
    int get_selected_dialogue_line()
    {
        RE::UI* ui = RE::UI::GetSingleton();
        RE::GFxValue gfx_selected_list_index;

        if (const auto var1 = ui->GetMenu(RE::DialogueMenu::MENU_NAME))
            if (const auto var2 = var1->uiMovie)
                if (const auto var3 = var2->GetVariable(&gfx_selected_list_index, "_root.DialogueMenu_mc.TopicListHolder.List_mc.iSelectedIndex"))
                {
                    return gfx_selected_list_index.GetNumber();
                }
        return -1;
    }

    bool is_selected_line_correct()
    {
        if (in_dialogue)
            return get_selected_dialogue_line() == dialogue_choice;


        return true;
    }


    void processor(float dtime)
    {

        RE::UI* ui = RE::UI::GetSingleton();

        in_dialogue = ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME);

        //////////////// DIALOGUE TEST ///////////////////////////////////
        RE::MenuTopicManager* topic_manager = RE::MenuTopicManager::GetSingleton();

        if (in_dialogue)
            WalkerProcessor::reset_walker();

        if (in_dialogue && topic_manager && !is_universal_blocked())
        {
            if (topic_manager)
                if (topic_manager->speaker.get())
                {
                    auto speaker_ref = topic_manager->speaker.get().get();
                    if (speaker_ref)
                    {
                        auto player = RE::PlayerCharacter::GetSingleton();
                        auto player_ref = player->AsReference();
                        if (speaker_ref != player_ref)
                            WalkerProcessor::lock_camera_onto_target(speaker_ref, dtime);
                        else
                            bool test_this = false;
                    }

                }

            auto speaker_ref = topic_manager->speaker.get().get();
            auto speaker_actor = (RE::Actor*)speaker_ref;

            //auto test = speaker_actor->boolBits;


            if (topic_manager->dialogueList)
                if (!topic_manager->dialogueList->empty() && topic_manager->dialogueList->front())
                {
                    if (old_dialogue.topicText != topic_manager->dialogueList->front()->topicText)
                    {
                        bool finished_speaking = speaker_actor->boolBits.any(RE::Actor::BOOL_BITS::kVoiceFileDone);
                        //if (!finished_speaking)
                        //    pause_time = 0.0f;

                        if (pause_time > 1.5f && finished_speaking)
                        {



                            /*
                            std::vector<MenuOption> dialogue_options = get_dialogue_options();
                            int id = 0;
                            for (auto& dialogue : *topic_manager->dialogueList)
                            {
                                MenuOption option;
                                option.id = id;
                                option.text = dialogue->topicText.c_str();
                                dialogue_options.push_back(option);
                                id++;
                                //RE::ConsoleLog::GetSingleton()->Print(dialogue->topicText.c_str());
                            }
                            //TODO: add timer or check if npc finished talking. maybe autoskp for already said options
                            force_dialogue(dialogue_options);
                            */
                            auto options = get_dialogue_options();
                            if (std::size(options) == 0 || (std::size(options) == 1 && options[0].text == "..."))
                                return; //just wait for it to disappear its fake

                            if (force_choice(options, "You are in dialogue. Choose a line to say", force_type::dialogue_line))
                            {
                                old_dialogue = *topic_manager->dialogueList->front();
                                pause_time = 0.0f;
                            }

                        }
                        else
                            pause_time += dtime;

                    }
                }
        }



        if (!in_dialogue)
        {
            old_dialogue.topicText = "";
            dialogue_choice_made = false;
            dialogue_choice = 0;
            pause_time = 0.0f;
        }



        if (in_dialogue && dialogue_choice_made)
        {
            if (dialogue_proc_time > 0.3f)
            {
                int cursor_pos = get_selected_dialogue_line();

                if (cursor_pos > -1)
                {
                    std::string line = std::to_string(dialogue_choice) + "/" + std::to_string(cursor_pos);
                    RE::ConsoleLog::GetSingleton()->Print(line.c_str());

                    if (dialogue_choice > cursor_pos)
                    {
                        //RE::ConsoleLog::GetSingleton()->Print("moving cursor down...");
                        dialogue_cursor_down();
                    }


                    if (dialogue_choice < cursor_pos)
                    {
                        //RE::ConsoleLog::GetSingleton()->Print("moving cursor up...");
                        dialogue_cursor_up();
                    }

                    if (dialogue_choice == cursor_pos)
                    {
                        //RE::ConsoleLog::GetSingleton()->Print("confirming choice...");

                        dialogue_choice_made = false;
                        //dialogue_choice = 0; //confirmation in dialogue process message hook
                        set_universal_block(1.0f);
                        //old_dialogue.topicText = "";
                        //leftclick();


                        //confirm();

                        leftclick();

                        /*
                        RE::GFxKeyEvent* my_event = new RE::GFxKeyEvent; //im not sure if its destroyed after use but i dont really care since this is supposed to be unique event in playthrough

                        my_event->type = RE::GFxKeyEvent::EventType::kKeyDown;
                        my_event->keyCode = (RE::GFxKey::Code)RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);//(RE::GFxKey::Code)in_char;//RE::GFxKey::Code::kKP_Multiply;
                        //my_event->wCharCode = 13;

                        const auto uiMessageQueue = RE::UIMessageQueue::GetSingleton();
                        if (uiMessageQueue) {
                            const auto msgData = RE::UIMessageDataFactory::Create<RE::BSUIScaleformData>();
                            msgData->scaleformEvent = my_event;
                            uiMessageQueue->AddMessage(RE::DialogueMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kScaleformEvent, msgData);
                        }
                        */

                    }

                    dialogue_proc_time = 0.0f;
                }

            }
            else
                dialogue_proc_time += dtime;
        }
        else
            dialogue_choice_made = false;
    }




}



