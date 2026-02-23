
/* useful stuff


        M/Main.h has some game state info (active/paused/blabla)
        M/MenuControls.h has something interesting regarding Menus.
        T/TES.h is another important root file

        try M/Misc.cpp CreateMessage

        T/TesFaction.cpp misc info (crime gold etc)

        T/TutorialMenu.cpp - maybe here i can disable tutorial or it might help to click it


        //MENU FLAG - USES CURSOR. MAYBE REMOVE IT SO IT DOESNT INTERFERE
        //OR MAYBE HOOK PROCESS EVENTS AND FILTER BY SOURCE

        GFxValue.h interesting things


//menu process message hook example

    struct MainMenuProcessMessage {
        static RE::UI_MESSAGE_RESULTS thunk(RE::MainMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                Background::LoadLastSaveTexture();
            }
            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_MainMenu[0]).write_vfunc(0x4, thunk); }
    };







        //print console
        RE::ConsoleLog::GetSingleton()->Print(msg.c_str());

        //print log file
        REX::INFO("Hello World!");




        //INDICATES IF DIALOGUE WINDOW IS OPEN. WORKS.
        RE::UI* ui = RE::UI::GetSingleton();
        RE::ConsoleLog::GetSingleton()->Print(std::to_string(ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME)).c_str());



        //this shit shows journal!
        if (auto UIMessageQueue = RE::UIMessageQueue::GetSingleton())
        {
            auto data = UIMessageQueue->CreateUIMessageData("Show Journal");
            UIMessageQueue->AddMessage(RE::JournalMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, data);
        }


        //this shows cursor menu. maybe i can make click?
        RE::UIMessageQueue::GetSingleton()->AddMessage(RE::CursorMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);



        //actor values
        msg += std::to_string(a->GetActorValue(RE::ActorValue::kStamina)); //THIS WORKS!!!
        msg += "/";
        msg += std::to_string(a->GetBaseActorValue(RE::ActorValue::kStamina));



        //amount of player's dialogue options
        RE::ConsoleLog::GetSingleton()->Print(std::to_string(topic_manager->dialogueList->size()).c_str());

        //amount of NPC's responses to 1st player's dialogue option
        RE::ConsoleLog::GetSingleton()->Print(std::to_string(topic_manager->dialogueList->front()->responses.size()).c_str());

        //this prints response of NPC to the option
        RE::ConsoleLog::GetSingleton()->Print((topic_manager->dialogueList->front()->responses.front()->text).c_str());

        //THIS PRINTS PLAYER'S 1st OPTION!!!
        RE::ConsoleLog::GetSingleton()->Print((topic_manager->dialogueList->front()->topicText).c_str());


        //a->UpdateInDialogue(topic_manager->dialogueList->front()->responses.front(), false); //doesnt seem to do anything. maybe i use it wrong






        //RE::BGSDialogueBranch* dialogue_branch = a->GetExclusiveBranch();
        //RE::TESTopic* topic = dialogue_branch->startingTopic;
        //RE::BGSDialogueBranch* branch = topic->ownerBranch;


        //RE::DialogueItem dialogue_data = RE::DialogueItem::GetDialogueData(a->GetObjectReference());

        ///////////////////////////////////////////////////

        //a->StopCurrentDialogue(); //doesnt work

        //RUN BACK
        int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->back, RE::INPUT_DEVICES::kKeyboard);
        if (RE::BSInputEventQueue::GetSingleton()->buttonEventCount == 0)
            RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 1.0);


        //this shows empty dialogue menu with cursor.
        //RE::UIMessageQueue::GetSingleton()->AddMessage("Dialogue Menu", RE::UI_MESSAGE_TYPE::kShow, nullptr);
        //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::CursorMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);



        int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->click, RE::INPUT_DEVICES::kMouse);


        if (RE::BSInputEventQueue::GetSingleton()->buttonEventCount == 0)
        {

            //a->StopCurrentDialogue();

            //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();

            RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 1.0);
            RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
        }


        //RE::ConsoleLog::GetSingleton()->Print(std::to_string(timer_time).c_str()); //its seconds.




        //idk
        if (topic_manager->selectedResponseNode)
                if (topic_manager->selectedResponseNode->item)
                {
                    RE::ConsoleLog::GetSingleton()->Print("---");
                    RE::ConsoleLog::GetSingleton()->Print(topic_manager->selectedResponseNode->item->topicText.c_str()); //this starts printing our response after we chose it.
                }
                else
                    RE::ConsoleLog::GetSingleton()->Print("no item!!");
            else
            {
                RE::ConsoleLog::GetSingleton()->Print("no node!!");
                //topic_manager->selectedResponseNode //i dont like this idea. assigning node will probably break everything.
            }


          //hide activator label
          auto msg = queue->CreateUIMessageData(RE::InterfaceStrings::GetSingleton()->hudData);
          if (const auto data = static_cast<RE::HUDData*>(msg)) {
             data->text = "";
             data->type = RE::HUDData::Type::kActivateNoLabel;
             queue->AddMessage(RE::HUDMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kUpdate, data);
            }




            //it adds message on input... this prints amount of messages
            //msg = std::to_string(uiMessageQueue->messages.numEntries);
            //RE::ConsoleLog::GetSingleton()->Print(msg.c_str());

            //auto test = uiMessageQueue->CreateUIMessageData(RE::InterfaceStrings::GetSingleton()->bsUIScaleformData);

            if (auto UIMessageQueue = RE::UIMessageQueue::GetSingleton())
            {
                auto data = UIMessageQueue->CreateUIMessageData("UIMenuOK");


                //auto data = UIMessageQueue->CreateUIMessageData("UIMenuOK"); //this doesnt work but i should investigate more about createUIMessageData
                //UIMessageQueue->AddMessage(RE::DialogueMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kScaleformEvent, data);
            }

            //uiMessageQueue->AddMessage(RE::DialogueMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kScaleformEvent, data); //kHide


            if (RE::BSInputEventQueue::GetSingleton()->charEvents)
            {
                msg = "char event detected:";// +uiMessageQueue->messagePool[0].type;
                msg += std::to_string(RE::BSInputEventQueue::GetSingleton()->charEvents[0].keyCode);
                RE::ConsoleLog::GetSingleton()->Print(msg.c_str());
            }





            //ATTEMPTS TO PASS MESSAGES TO UIQUEUE

            if (false && in_dialogue && processing_dialogue && timer_time > 1.0f)
        {
            //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);

           // if (RE::BSInputEventQueue::GetSingleton()->buttonEventCount == 0)
           // {

               // RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
               // RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, 31, 1.0, 1.0);
               // RE::ConsoleLog::GetSingleton()->Print("ADDED INPUT: activate");
               // RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, 31, 0.0, 0.0);


                RE::UIMessageQueue* uiMessageQueue = RE::UIMessageQueue::GetSingleton();

                //auto test = uiMessageQueue->CreateUIMessageData(RE::BSUIScaleformData::CLASS_NAME);
                //uiMessageQueue->ProcessCommands();

                //auto test = uiMessageQueue->CreateUIMessageData(RE::InterfaceStrings::GetSingleton()->bsUIScaleformData);
                auto test = RE::UIMessageDataFactory::Create<RE::BSUIScaleformData>();



                RE::ConsoleLog::GetSingleton()->Print(RE::InterfaceStrings::GetSingleton()->bsUIScaleformData.c_str());

                //auto test = uiMessageQueue->CreateUIMessageData(RE::InterfaceStrings::GetSingleton()->hudData);
                if (test)
                    RE::ConsoleLog::GetSingleton()->Print("test ok!");
                else
                    RE::ConsoleLog::GetSingleton()->Print("test bad!");


                //RE::BSUIScaleformData my_data;


                if (const auto data = static_cast<RE::BSUIScaleformData*>(test))
                //if (data_to_copy.scaleformEvent->type)
                {
                    //auto data = &data_to_copy;

                    //auto data_new = static_cast<RE::GFxKeyEvent*>(data->scaleformEvent);
                    //GFxMouseEvent

                    RE::GFxMouseEvent data_new_obj;

                    RE::GFxMouseEvent* data_new = &data_new_obj;

                    if (data->scaleformEvent)
                    {
                        data_new = static_cast<RE::GFxMouseEvent*>(data->scaleformEvent);
                    }
                    else
                    {
                        //data->scaleformEvent = data_new;
                        RE::ConsoleLog::GetSingleton()->Print("data->scaleformEvent EMPTY");
                    }




                        //data_new = RE::GFxEvent::EventType::kKeyDown;
                        //data_new->keyCode = RE::GFxKey::kE;

                        if (mouse_action_type)
                        {
                            mouse_action_type = false;
                            data_new->type = RE::GFxEvent::EventType::kMouseUp;
                        }
                        else
                        {
                            mouse_action_type = false;
                            data_new->type = RE::GFxEvent::EventType::kMouseDown;
                        }

                        data_new->button = 0;
                        data_new->mouseIndex = 0;
                        data_new->y = 100.0;
                        data_new->x = 200.0;
                        data_new->scrollDelta = 0.0;



                        data->scaleformEvent = static_cast<RE::GFxEvent*>(data_new);


                        RE::ConsoleLog::GetSingleton()->Print("sending message");
                        uiMessageQueue->AddMessage("Top Menu", RE::UI_MESSAGE_TYPE::kScaleformEvent, data); //kHide
                        //uiMessageQueue->AddMessage(RE::DialogueMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kScaleformEvent, data); //kHide
                        RE::ConsoleLog::GetSingleton()->Print("message sent");

                        //uiMessageQueue->AddMessage(RE::DialogueMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kScaleformEvent, data); //kHide




                }
                else
                    RE::ConsoleLog::GetSingleton()->Print("FAILED TO CREATE DATA!");

                //processing_dialogue = false;
                timer_time = 0.0f;
           // }

        }

*/








////////////////////// OLD SHIT


    //old
    /*
    //////////////// DIALOGUE TEST ///////////////////////////////////
    RE::MenuTopicManager* topic_manager = RE::MenuTopicManager::GetSingleton();

    //topic_manager->
    //int cur_pos //= 0;
    int count_lines = 0;
    if (topic_manager)
        if (topic_manager->dialogueList)
            if (topic_manager->dialogueList->front())
                for (auto& dialogue : *topic_manager->dialogueList)
                    count_lines++;




    //TODO: proper choice mechanic. at least need to somehow check our current position, because it doesnt always start at 0 (for example when user changed it before we started to respond)
    if (count_lines > 0 && id < count_lines)
    {
        for (int i = 0; i < id; i++)
            dialogue_cursor_down();

        dialogue_say_confirm();

        return true;
    }

    return false;
    */




/*


        if (true)
        {
            //these are not all menus. there is at least "Top Menu" (maybe its some menu that always appears and is on top of all menus?)
            if (ui->IsMenuOpen("Top Menu")) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::BarterMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::BarterMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::BarterMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::BookMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::BookMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::Console::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::Console::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::ConsoleNativeUIMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::ConsoleNativeUIMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::ContainerMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::CraftingMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::CreationClubMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::CreationClubMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::CreditsMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::CreditsMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::CursorMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::CursorMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::DialogueMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::FaderMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::FaderMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::FavoritesMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::FavoritesMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::GiftMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::GiftMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::HUDMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::HUDMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::InventoryMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::InventoryMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::JournalMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::JournalMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::KinectMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::KinectMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::LevelUpMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::LevelUpMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::LoadingMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::LoadWaitSpinner::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::LoadWaitSpinner::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::LockpickingMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::LockpickingMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::MagicMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::MagicMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::MainMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::MainMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::MapMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::MapMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::MessageBoxMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::MessageBoxMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::MistMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::MistMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::ModManagerMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::ModManagerMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::RaceSexMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::RaceSexMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::SafeZoneMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::SafeZoneMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::SleepWaitMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::SleepWaitMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::TitleSequenceMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::TitleSequenceMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::TrainingMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::TrainingMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::TutorialMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::TutorialMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
            if (ui->IsMenuOpen(RE::TweenMenu::MENU_NAME)) {
                //RE::ConsoleLog::GetSingleton()->Print(RE::TweenMenu::MENU_NAME.data());
                dbg_breakpoint = false;
            }
        }



        //this doesnt work because tutorial pauses the game and update isnt called anymore. TODO: workaround
        if (ui->IsMenuOpen(RE::TutorialMenu::MENU_NAME))
        {

            //we have tutorial menu popup. send context and close in 2 seconds
            auto tutorial_menu = ui->GetMenu(RE::TutorialMenu::MENU_NAME);

            if (!first_turorial)
            {
                first_turorial = true;
                turorial_time = 0.0f;

            }
            else
            {
                if (turorial_time > 2.0f)
                {
                    turorial_time = 0.0f;
                    dialogue_say_confirm();
                    first_turorial = false;
                }
                else
                {
                    turorial_time += dtime;
                }
            }
        }
        else
        {
            first_turorial = false;
            turorial_time = 0.0f;
        }
        
        */



/*


///////////////////////////////////////////////////
        //debug. sniff ui messages
        if (is_in_dialogue())
        {

            if (RE::BSInputEventQueue::GetSingleton()->buttonEvents[0].Value() != 0)
                ;// RE::ConsoleLog::GetSingleton()->Print(std::to_string(RE::BSInputEventQueue::GetSingleton()->buttonEvents[0].idCode).c_str());


            RE::UIMessageQueue* uiMessageQueue = RE::UIMessageQueue::GetSingleton();


            if (uiMessageQueue->messagePool)
            {
                for (auto message : uiMessageQueue->messagePool)
                {
                    if (message.data)
                        if (message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent)
                        {

                            REX::EnumSet<RE::UI_MESSAGE_TYPE, std::uint32_t> this_type = message.type;

                            uint32_t this_type_int = 0;
                            std::memcpy(&this_type_int, &this_type, 4);

                            //OK, when i click it sends kScaleformEvent (type 6).
                            //must dig into that.

                            //" I think its ScaleformEvent then where UIMessageData is a ScaleformMessageType, with GfxButtonEvent as a data "
                            //uint32_t type = uiMessageQueue->messagePool[0].type.get();
                            //it gives Cursor Menu and Top Menu

                            msg = "NON-UPDATE EVENT DETECTED, TYPE= ";// +uiMessageQueue->messagePool[0].type;
                            msg += std::to_string(this_type_int);
                            msg += ", menu= ";
                            msg += message.menu;
                            msg += ", ";

                            if (message.menu == "Top Menu")
                                if (const auto data = static_cast<RE::BSUIScaleformData*>(message.data))
                                {
                                    //GFxMouseEvent
                                    if (data->scaleformEvent->type == RE::GFxEvent::EventType::kMouseDown || data->scaleformEvent->type == RE::GFxEvent::EventType::kMouseUp)
                                    {
                                        auto data_new = static_cast<RE::GFxMouseEvent*>(data->scaleformEvent);
                                        std::memcpy(&this_type_int, &data_new->type, 4);
                                        msg += std::to_string(this_type_int); //this gives 2 and 3 consequetive. some update-looking 6s too
                                        //2 and 3 are MOUSE DOWN AND MOUSE UP!
                                        msg += " ";
                                        msg += std::to_string(data_new->button); //this seem to work. 0 for left click, 1 for right click, 2 for middle click
                                        msg += " ";
                                        msg += std::to_string(data_new->mouseIndex); //this was 0 in both left and right clicks.

                                        //data_to_copy = data;

                                    }
                                    else
                                    {
                                        auto data_new = static_cast<RE::GFxMouseEvent*>(data->scaleformEvent);
                                        std::memcpy(&this_type_int, &data_new->type, 4);
                                        msg += std::to_string(this_type_int); //this gives 2 and 3 consequetive. some update-looking 6s too
                                    }

                                    //  PRINTS

                                    //RE::ConsoleLog::GetSingleton()->Print(msg.c_str());
                                    //REX::INFO(msg.c_str());
                                }

                        }
                }

            }

        }
        */


        //step1
        /*
        if (!action_queue.empty())
        {
            if (in_dialogue && !processing_cmd)
            {
                processing_cmd = true;

                int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->back, RE::INPUT_DEVICES::kKeyboard);

                std::string action_name = "";// action_queue[action_queue.size() - 1];

                if (action_name == Capabilities::DialogueDown::Name)
                {
                    my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->back, RE::INPUT_DEVICES::kKeyboard);
                    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 1.0);
                    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
                }

                if (action_name == Capabilities::DialogueUp::Name)
                {
                    my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
                    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 1.0);
                    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
                }

                if (action_name == Capabilities::DialogueSay::Name)
                {
                    my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
                    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
                    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
                }

                if (action_name == Capabilities::SelectChoiceOption::Name)
                {
                    my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
                    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
                    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
                }


                queue_pop();

                timer_time = 0.0f;
            }

            timer_time += dtime;

            if (processing_cmd)
            {
                if (timer_time > 0.5f)
                {
                    processing_cmd = false;
                }
            }


        }
        */


/* OLD SKILL TREE MOVER



void move_cursor_to_perk(int perk_id)
{
    //TODO: custom map for moving (i have no idea how to avoid it)
    RE::UI* ui = RE::UI::GetSingleton();
    auto menu = ui->GetMenu<RE::StatsMenu>();

    if (menu && menu->unk1C8)
    {
        int cur_id = menu->unk1C8->unk48;

        if (menu && perk_nodes_list_valid && perk_nodes_list.find(perk_id) != perk_nodes_list.end() && perk_nodes_list.find(cur_id) != perk_nodes_list.end())
        {
            auto target_perk = perk_nodes_list.find(perk_id);
            auto current_perk = perk_nodes_list.find(cur_id);

            if (current_perk->second->index != last_perk_index)
            {
                menu->horizontalVelocity = 0.0f;
                last_perk_index = current_perk->second->index;
                perk_cancel_inputs();

                set_universal_block(1.0f);
                return;
            }


            int target_gridX = target_perk->second->perkGridX;
            int target_gridY = target_perk->second->perkGridY;
            float target_posX = target_perk->second->horizontalPosition;// +target_gridX;
            float target_posY = target_perk->second->verticalPosition;// + target_gridY;

            int current_gridX = current_perk->second->perkGridX;
            int current_gridY = current_perk->second->perkGridY;
            float current_posX = current_perk->second->horizontalPosition;// +current_gridX;
            float current_posY = current_perk->second->verticalPosition;// +current_gridY;

            //int grid_difX = target_gridX - current_gridX;
            //int grid_difY = target_gridY - current_gridY;


            float pos_difX = target_posX - current_posX;
            float pos_difY = target_posY - current_posY;



            if (grid_difX != 0 || grid_difY != 0)
            {
                pos_difX *= grid_difX;
                pos_difY *= grid_difY;

            }


            if (abs(grid_difX) > abs(grid_difY) && abs(grid_difX == 1))
            {
                if (grid_difX > 0)
                    perk_left();
                else
                    perk_right();

                return;
            }

            if (abs(grid_difX) < abs(grid_difY) && abs(grid_difY == 1))
            {
                if (grid_difY > 0)
                    perk_up();
                else
                    perk_down();

                return;
            }

            //use normal coordinates, grid is not enough



            //perk_up();

            if (abs(pos_difX) > abs(pos_difY))
            {
                if (pos_difX > 0)
                    perk_right();
                else
                    perk_left();
            }

            if (abs(pos_difX) < abs(pos_difY))
            {
                if (pos_difY > 0)
                    perk_up();
                else
                    perk_down();
            }

        }
    }
    else
    {
        bool dbg_test = false;
    }
}



*/





//garbage from walker fix too thin navmesh 


                    /*
                    RE::NiTransform my_transform{};

                    player_ref->GetTransform(my_transform);

                    auto player_geom = player_3d->AsGeometry();

                    auto player_ni_geom = player_3d->AsNiGeometry();

                    player->AsReference()->Get3D()->worldBound.radius = 0.5;



                    if (player_geom)
                        player_geom->modelBound.radius = 0.1;

                    if (player_ni_geom)
                    {
                        player_ni_geom->worldBound.radius = 0.5;
                        player_ni_geom->UpdateWorldBound();
                        bool test_bool = false;
                    }

                    auto biped = player_3d_current->userData->GetCurrentBiped();
                    if (biped)
                        biped->root->worldBound.radius = 0.5;

                    //player_3d_current.as... - TRY ALL THESE
                    auto player_3d_current_niref = (RE::NiRefObject*)player_3d_current;

                    auto as1 = player_3d_current->AsBhkAttachmentCollisionObject();
                    auto as2 = player_3d_current->AsBhkBlendCollisionObject();
                    auto as3 = player_3d_current->AsBhkLimitedHingeConstraint();
                    auto as4 = player_3d_current->AsBhkNiCollisionObject();
                    auto as5 = player_3d_current->AsBhkRigidBody();
                    auto as6 = player_3d_current->AsDynamicTriShape();
                    auto as7 = player_3d_current->AsFadeNode();
                    auto as8 = player_3d_current->AsGeometry();
                    auto as9 = player_3d_current->AsLinesGeom();
                    auto as10 = player_3d_current->AsMultiBoundNode();
                    auto as11 = player_3d_current->AsNiControllerManager();
                    auto as12 = player_3d_current->AsNiGeometry();
                    auto as13 = player_3d_current->AsNiTriBasedGeom();
                    auto as14 = player_3d_current->AsNiTriShape();
                    auto as15 = player_3d_current->AsNode();
                    auto as16 = player_3d_current->AsParticlesGeom();
                    auto as17 = player_3d_current->AsSegmentedTriShape();
                    auto as18 = player_3d_current->AsSubIndexTriShape();
                    auto as19 = player_3d_current->AsSwitchNode();
                    auto as20 = player_3d_current->AsTriShape();
                    auto as21 = player_3d_current->AsTriStrips();


                    auto data_size = player_3d_current->GetExtraDataSize();

                    */
                    //originalGetBoundMin = REL::GetTrampoline().write_call<5>((uintptr_t)player_ref + 0x73, myGetBoundMin);
                    //originalGetBoundMax = REL::GetTrampoline().write_call<5>((uintptr_t)player_ref + 0x74, myGetBoundMax);
                    //VTABLE_TESObjectREFR
                    //VTABLE_Actor
                    //originalGetBoundMin = REL::Relocation<std::uintptr_t>(RE::VTABLE_TESObjectREFR[0]).write_vfunc(0x73, myGetBoundMin);
                    //originalGetBoundMax = REL::Relocation<std::uintptr_t>(RE::VTABLE_TESObjectREFR[0]).write_vfunc(0x74, myGetBoundMax);





/*



auto bound_max = player_ref->GetBoundMax();
                    auto bound_min = player_ref->GetBoundMin();

                    RE::ExtraDataList* extra_list_player = &player_ref->extraList;

                    auto test_bound = extra_list_player->GetByType(RE::ExtraDataType::kMultiBound);


                    for (int typeInt = (int)RE::ExtraDataType::kNone; typeInt <= (int)RE::ExtraDataType::kUnkBF; typeInt++)
                    {
                        RE::ExtraDataType type = static_cast<RE::ExtraDataType>(typeInt);
                        if (auto test_extra = extra_list_player->GetByType(type); test_extra)
                        {
                            bool got_extra = true;
                        }
                    }

                    RE::NiExtraData** extra_list_player_3d = player_3d_current->extra;

                    auto idk_data = player_3d_current->GetExtraData("BBX");

                    RE::BSBound* bounds_idk = (RE::BSBound*)idk_data;

                    if (bounds_idk)
                    {
                        bounds_idk->extents.x = 1.0;
                        bounds_idk->extents.y = 1.0;
                    }

                    /*
                    for (int typeInt = (int)RE::ExtraDataType::kNone; typeInt <= (int)RE::ExtraDataType::kUnkBF; typeInt++)
                    {
                        RE::ExtraDataType type = static_cast<RE::ExtraDataType>(typeInt);

                        auto data_name = (*extra_list_player_3d)->GetName();
                        RE::BSFaceGenAnimationData* bounds_idk2 = (RE::BSFaceGenAnimationData*)idk_data;
                        RE::BSFurnitureMarkerNode* bounds_idk3 = (RE::BSFurnitureMarkerNode*)idk_data;
                        RE::NiBooleanExtraData* bounds_idk4 = (RE::NiBooleanExtraData*)idk_data;
                        RE::NiFloatExtraData* bounds_idk5 = (RE::NiFloatExtraData*)idk_data;
                        RE::NiFloatsExtraData* bounds_idk6 = (RE::NiFloatsExtraData*)idk_data;
                        RE::NiIntegerExtraData* bounds_idk7 = (RE::NiIntegerExtraData*)idk_data;
                        RE::NiIntegersExtraData* bounds_idk8 = (RE::NiIntegersExtraData*)idk_data;
                        RE::NiStringExtraData* bounds_idk9 = (RE::NiStringExtraData*)idk_data;
                        RE::NiStringsExtraData* bounds_idk10 = (RE::NiStringsExtraData*)idk_data;


                        auto idk_2 = idk_data->GetGroup();




                        if (auto test_extra = extra_list_player_3d->GetByType(type); test_extra)
                        {
                            bool got_extra = true;
                        }
                        bool stop_here = false;

                    }


player->GetBoundMax();


bound_max = player_ref->GetBoundMax();
bound_min = player_ref->GetBoundMin(); 




                    RE::BSPathingActorAttributes* test = (RE::BSPathingActorAttributes*)(REL::Relocation<std::uintptr_t>(RE::VTABLE_BSPathingRequest[0]).address() + REL::Relocation<std::uintptr_t>(RE::VTABLE_BSPathingRequest[0]).offset() + 0xC0);

                    //auto player_ref = player->AsReference();

                    long long ref_address = 0;

                    RE::TESObjectREFR* player_ref = (RE::TESObjectREFR*)ref_address;

                    auto test_name = "NOT_YET";

                    if (player_ref)
                        test_name = player_ref->GetName();
                    else
                        player_ref = player->AsReference();

                    auto player_3d = player_ref->Get3D();
                    auto player_3d_current = player_ref->GetCurrent3D();
                    auto player_3d_1p = player_ref->Get3D1(true);
                    auto player_3d_3p = player_ref->Get3D1(false);


            static RE::NiPoint3* myGetBoundMin(RE::TESObjectREFR* actor)
            {
                bool test_bool1 = false;
                //RE::NiPoint3 fake_result = { 15.0, 15.0, 15.0 };

                RE::NiPoint3* result = originalGetBoundMin(actor);

                result->x = -10.0f;
                result->y = -10.0f;
                //RE::TESObjectREFR* test = REX::UNRESTRICTED_CAST<RE::TESObjectREFR*>(result);

                return result;
            }

            static RE::NiPoint3* myGetBoundMax(RE::TESObjectREFR* actor)
            {
                bool test_bool1 = false;
                //RE::NiPoint3 fake_result = { 15.0, 15.0, 15.0 };

                RE::NiPoint3* result = originalGetBoundMax(actor);

                result->x = 10.0f;
                result->y = 10.0f;
                //RE::TESObjectREFR* test = REX::UNRESTRICTED_CAST<RE::TESObjectREFR*>(result);

                return result;

            }



            static RE::NiPoint3* myGetBoundMin_HAZARD(RE::TESObjectREFR* actor)
            {
                bool test_bool1 = false;
                //RE::NiPoint3 fake_result = { 15.0, 15.0, 15.0 };

                RE::NiPoint3* result = originalGetBoundMin_HAZARD(actor);

                result->x = -10.0f;
                result->y = -10.0f;
                //RE::TESObjectREFR* test = REX::UNRESTRICTED_CAST<RE::TESObjectREFR*>(result);

                return result;
            }

            static RE::NiPoint3* myGetBoundMax_HAZARD(RE::TESObjectREFR* actor)
            {
                bool test_bool1 = false;
                //RE::NiPoint3 fake_result = { 15.0, 15.0, 15.0 };

                RE::NiPoint3* result = originalGetBoundMax_HAZARD(actor);

                result->x = 10.0f;
                result->y = 10.0f;
                //RE::TESObjectREFR* test = REX::UNRESTRICTED_CAST<RE::TESObjectREFR*>(result);

                return result;

            }

                        static inline REL::Relocation<decltype(myGetBoundMin)> originalGetBoundMin;
            static inline REL::Relocation<decltype(myGetBoundMax)> originalGetBoundMax;

            static inline REL::Relocation<decltype(myGetBoundMin_HAZARD)> originalGetBoundMin_HAZARD;
            static inline REL::Relocation<decltype(myGetBoundMax_HAZARD)> originalGetBoundMax_HAZARD;


                            //originalGetBoundMin = REL::Relocation<std::uintptr_t>(RE::VTABLE_Actor[0]).write_vfunc(0x73, myGetBoundMin);
                //originalGetBoundMax = REL::Relocation<std::uintptr_t>(RE::VTABLE_Actor[0]).write_vfunc(0x74, myGetBoundMax);

                originalGetBoundMin = REL::Relocation<std::uintptr_t>(RE::VTABLE_PlayerCharacter[0]).write_vfunc(0x73, myGetBoundMin);
                originalGetBoundMax = REL::Relocation<std::uintptr_t>(RE::VTABLE_PlayerCharacter[0]).write_vfunc(0x74, myGetBoundMax);

                originalGetBoundMin = REL::Relocation<std::uintptr_t>(RE::VTABLE_Character[0]).write_vfunc(0x73, myGetBoundMin);
                originalGetBoundMax = REL::Relocation<std::uintptr_t>(RE::VTABLE_Character[0]).write_vfunc(0x74, myGetBoundMax);

                //VTABLE_Hazard
                //VTABLE_Explosion
                //VTABLE_Projectile

                originalGetBoundMin = REL::Relocation<std::uintptr_t>(RE::VTABLE_Hazard[0]).write_vfunc(0x73, myGetBoundMin_HAZARD);
                originalGetBoundMax = REL::Relocation<std::uintptr_t>(RE::VTABLE_Hazard[0]).write_vfunc(0x74, myGetBoundMax_HAZARD);

                //auto address = REL::Relocation<std::uintptr_t>(RE::VTABLE_Actor[0]).address();
                //auto offset = REL::Relocation<std::uintptr_t>(RE::VTABLE_Actor[0]).offset();
                //auto address = REL::Relocation<std::uintptr_t>(RE::VTABLE_PlayerCharacter[0]).address();
                //auto offset = REL::Relocation<std::uintptr_t>(RE::VTABLE_PlayerCharacter[0]).offset();
                SKSE::AllocTrampoline(42);
                //originalGetBoundMin = REL::GetTrampoline().write_call<5>(address + offset + 0x73, myGetBoundMin);
                //originalGetBoundMax = REL::GetTrampoline().write_call<5>(address + offset + 0x74, myGetBoundMax);
                //originalGetBoundMin = REL::GetTrampoline().write_call<5>(address + 0x73, myGetBoundMin);
                //originalGetBoundMax = REL::GetTrampoline().write_call<5>(address + 0x74, myGetBoundMax);
                    //VTABLE_Actor
*/




/*


navmesh cutter shit 



//door_refr->GetObstacle();

            //extracolisiondata->layer (COL_LAYER)




            //auto my_data123 = (RE::ExtraCollisionData*)RE::BSExtraData::Create(0x18, REL::Relocation<std::uintptr_t>(RE::VTABLE_ExtraCollisionData[0]).address());
            //auto my_col_data = new RE::CollisionData{};

            //my_col_data->layer = RE::COL_LAYER::kAvoidBox;
            //my_col_data->layer = RE::COL_LAYER::kUnused3;
            //my_data123->collisionData = my_col_data;




                //->layer = RE::COL_LAYER::kAvoidBox;

            //target_refr->extraList.Add(my_data123);

            //my_data->collisionData->layer = RE::COL_LAYER::kAvoidBox;

           // target_refr->extraList.Add(my_data);







*/

//auto avoidbox_form = RE::TESObjectREFR::LookupByID(0x700285B);
                    //auto avoidbox_form2 = RE::TESObjectREFR::LookupByID(0x7002dbf);

                    //auto avoidbox_refr = avoidbox_form->AsReference();
                    //auto avoidbox_refr2 = avoidbox_form2->AsReference();

                    //auto door_form = RE::TESObjectREFR::LookupByID(0x9dc6e);
                    //auto door_refr = door_form->AsReference();


                    /*
                    if (avoidbox_refr2)
                    {
                        //RE::ExtraDataList* extra_list_player = &avoidbox_refr->extraList;


                        //data_to_copy = extra_list_player;

                        //avoidbox_refr2->MoveTo(door_refr);
                        //if (move_avoid_box)


                        //auto door_pos = door_refr->GetPosition();


                        //avoidbox_refr->SetPosition(door_pos);




                        if (data_to_copy && door_refr)
                        {
                            auto data0 = data_to_copy->GetByType(RE::ExtraDataType::kPrimitive);
                            auto data1 = data_to_copy->GetByType(RE::ExtraDataType::kUnkB4);
                            auto data2 = data_to_copy->GetByType(RE::ExtraDataType::kCollisionData);

                            {
                                //if (data1)
                                //    door_refr->extraList.Add(data1);

                                if (!door_refr->extraList.GetByType(RE::ExtraDataType::kPrimitive))
                                {
                                    if (data0)
                                        door_refr->extraList.Add(data0);

                                    player->extraList.Add(data0);


                                }
                                else
                                {
                                    avoidbox_refr->MoveTo(door_refr);

                                }




                            }

                        }




                        auto test_bound = extra_list_player->GetByType(RE::ExtraDataType::kCollisionData);

                        if (test_bound)
                        {
                            RE::ExtraCollisionData* collision_data = (RE::ExtraCollisionData*)test_bound;

                            if (collision_data->collisionData)
                                if (collision_data->collisionData->layer == RE::COL_LAYER::kAvoidBox)
                                    bool got_avoid_box = true;
                        }

                        for (int typeInt = (int)RE::ExtraDataType::kNone; typeInt <= (int)RE::ExtraDataType::kUnkBF; typeInt++)
                        {
                            RE::ExtraDataType type = static_cast<RE::ExtraDataType>(typeInt);
                            if (auto test_extra = extra_list_player->GetByType(type); test_extra)
                            {
                                if (type == RE::ExtraDataType::kCollisionData)
                                {
                                    bool got_one = false;

                                    RE::ExtraCollisionData* collision_data = (RE::ExtraCollisionData*)test_extra;

                                    if (collision_data->collisionData)
                                        if (collision_data->collisionData->layer == RE::COL_LAYER::kAvoidBox)
                                            bool got_avoid_box = true;

                                }

                                if (type == RE::ExtraDataType::kPrimitive)
                                {
                                    bool got_one = false;

                                    RE::ExtraPrimitive* collision_data = (RE::ExtraPrimitive*)test_extra;


                                }
                            }
                        }
                    }

                    */


/*

   bool FuncName(int params)
    {
        using func_t = decltype(&FuncName);
        static REL::Relocation<func_t> func{ RELOCATION_ID(0, 90396) };
        return func(params);
    }
    

    */



/*

if (init_delay)
        {
            if (!paused_before_interaction)
            {

                auto smelter = RE::TESObjectREFR::LookupByID(0x10e391);

                if (smelter)
                {
                    auto smelter_ref = smelter->AsReference();

                    if (smelter_ref)
                    {
                        auto player = RE::PlayerCharacter::GetSingleton();
                        auto player_ref = player->AsReference();

                        auto furniture_ref = (RE::TESObjectREFR*)(smelter_ref->data.objectReference);

                        if (furniture_ref && player_ref)
                        {
                            auto stop_here = RE::BSInputEventQueue::GetSingleton();


                            //confirm(); //clear_leftclick();
                            ;// furniture_ref->Activate(smelter_ref, player_ref, 0, nullptr, 1);
                            ;// workbench->Activate(target_ref, player_ref, 0, )

                            paused_before_interaction = true;
                            return;
                        }


                    }

                }


            }
        }



        auto saveloadgame = RE::BGSSaveLoadGame::GetSingleton();

        if (saveloadgame)
            if (saveloadgame->GetSaveGameLoading())
                bool works = true;

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();
        if (player_ref)
        {
            auto angle = player_ref->data.angle;
            bool stop_heredbg = false;
        }


        if (player)
        {
            auto parent_cell = player->GetParentCell();
            if (parent_cell)
            {
                parent_cell->ForEachReferenceInRange(player->GetPosition(), 30000000.0,
                    [&](RE::TESObjectREFR* a_ref) {


                        RE::ExtraDataList* extra_list_player = &a_ref->extraList;

                        auto test_bound = extra_list_player->GetByType(RE::ExtraDataType::kCollisionData);

                        if (test_bound)
                        {
                            RE::ExtraCollisionData* collision_data = (RE::ExtraCollisionData*)test_bound;

                            if (collision_data->collisionData)
                                if (collision_data->collisionData->layer == RE::COL_LAYER::kAvoidBox)
                                    bool got_avoid_box = true;
                        }

                        for (int typeInt = (int)RE::ExtraDataType::kNone; typeInt <= (int)RE::ExtraDataType::kUnkBF; typeInt++)
                        {
                            RE::ExtraDataType type = static_cast<RE::ExtraDataType>(typeInt);
                            if (auto test_extra = extra_list_player->GetByType(type); test_extra)
                            {
                                if (type == RE::ExtraDataType::kCollisionData)
                                {
                                    bool got_one = false;

                                    RE::ExtraCollisionData* collision_data = (RE::ExtraCollisionData*)test_extra;

                                    if (collision_data->collisionData)
                                        if (collision_data->collisionData->layer == RE::COL_LAYER::kAvoidBox)
                                            bool got_avoid_box = true;

                                }

                                if (type == RE::ExtraDataType::kPrimitive)
                                {
                                    bool got_one = false;

                                    RE::ExtraPrimitive* collision_data = (RE::ExtraPrimitive*)test_extra;


                                }
                            }
                        }

                        return RE::BSContainer::ForEachResult::kContinue;
                    });
            }
        }

        
        */




        //RE::BSAnimationGraphManager my_manager{};
/*
RE::BSAnimationGraphManagerPtr my_ptr;

bool result = target->GetAnimationGraphManager(my_ptr);


auto target_actor_3d = target_actor->Get3D();



auto angle = target->data.angle;  //target_actor->GetAngle();

RE::NiPoint3 uuh = { 0.0f, 1.0f, 0.0f };

uuh = MiscThings::rotate_vector_by_angles(uuh, angle);

auto target_state = target_actor->actorState2;


if (!target_state.talkingToPlayer)
target_center = camera_pos + uuh;

//target_actor

//if (target_actor->)
*/


/*
lookat_location.z = 0.0f;
player_temp_pos.z = 0.0f;

auto targets_target_handle = target_actor->currentProcess->target;

auto targets_target_ref_p = RE::TESObjectREFR::LookupByHandle(targets_target_handle);

if (targets_target_ref_p && targets_target_ref_p.get())
{
        auto targets_target_ref = targets_target_ref_p.get();
        auto targets_target_pos = targets_target_ref->GetPosition();
        targets_target_pos.z = 0.0f;

        if (targets_target_pos.GetDistance(player_temp_pos) > 40.0f)
        {
            height = 0.0f;
            target_center = target_actor->GetLookingAtLocation();
            specific_shift = { 0.0f, 0.0f, 0.0f };

        }
    }

    */




    /*
    * 
    * 
    * 
    * 
    * 
    * 	
						float dif_posX = marker_posX - cursor_posX;
						float dif_posY = marker_posY - cursor_posY;

						float mouse_moveX = dif_posX * K;
						float mouse_moveY = dif_posY * K;

						if (abs(mouse_moveX) < 10.0f)
							if (mouse_moveX < 0)
								mouse_moveX = -10.0f;
							else
								mouse_moveX = 10.0f;

						if (abs(mouse_moveY) < 7.0f)
							if (mouse_moveY < 0)
								mouse_moveY = -7.0f;
							else
								mouse_moveY = 5.0f;


						mouse_mouse_x_y(mouse_moveX, mouse_moveY);

						if (abs(dif_posX) < 0.006 && abs(dif_posY) < 0.006)


    * 
    * //map hell
    * 
    * 
            if (bool6)
            {
                //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::CursorMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
                //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MenuCursor::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);

                if (map_processor_timer > 3.0f)
                {
                    if (map_processor_timer < 5.0f)
                    {

                        mouse_mouse_x_y(100.0f, 100.0f);
                    }

                }

                if (map_processor_timer > 6.0f)
                {
                    if (!bool1)
                    {
                        bool1 = true;
                        RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
                        RE::UI* ui = RE::UI::GetSingleton();
                        auto menu = ui->GetMenu(RE::MapMenu::MENU_NAME);
                        //menu->UsesMovementToDirection()
                        //menu->RefreshPlatform();

                    }

                }

                if (map_processor_timer > 10.0f)
                {
                    if (!bool3)
                    {
                        //right();
                        //left();
                        bool3 = true;

                    }

                    if (map_processor_timer < 12.0f)
                    {
                        auto test_new_input3 = RE::ControlMap::GetSingleton();

                        for (int i = 0; i < 20; i++)
                        {
                            auto ii = static_cast<RE::UserEvents::INPUT_CONTEXT_ID>(i);

                            test_new_input3->PopInputContext(ii);
                        }
                        //test_new_input3->PushInputContext(RE::UserEvents::INPUT_CONTEXT_ID::kGameplay);
                        test_new_input3->PushInputContext(RE::UserEvents::INPUT_CONTEXT_ID::kMap);
                        mouse_mouse_x_y(-10.0f, -10.0f);
                    }
                    //else
                        //mouse_mouse_x_y(0.0f, 0.0f);
                }

                if (map_processor_timer > 17.0f)
                {
                    if (!bool2)
                    {
                        bool2 = true;

                        RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
                        auto test_new_input3 = RE::ControlMap::GetSingleton();

                        for (int i = 0; i < 20; i++)
                        {
                            auto ii = static_cast<RE::UserEvents::INPUT_CONTEXT_ID>(i);

                            test_new_input3->PopInputContext(ii);
                        }
                        test_new_input3->PushInputContext(RE::UserEvents::INPUT_CONTEXT_ID::kMap);
                    }
                }




                if (map_processor_timer > 20.0f)
                {
                    //if (map_processor_timer < 12.0f)
                    //{
                    //leftclick();
                    mouse_mouse_x_y(100.0f, 100.0f);
                    //}
                }

                //map_processor_timer += dtime;

                if (map_processor_timer > 25.0f)
                {
                    map_processor_timer = 0;
                    bool1 = false;
                    bool2 = false;
                    bool3 = false;
                }




                //worldSpace->worldMapData.cameraData.maxHeight = 1000000000.0f;
                //worldSpace->worldMapData.cameraData.minHeight = -1000000000.0f;


                RE::UI* ui = RE::UI::GetSingleton();

                auto menu1 = ui->IsMenuOpen(RE::MapMenu::MENU_NAME);
                auto menu2 = ui->IsMenuOpen(RE::CursorMenu::MENU_NAME);
                auto menu3 = ui->IsCursorHiddenWhenTopmost();
                auto menu4 = ui->IsUsingCustomRendering();

                auto menu5 = ui->GetMenu(RE::MapMenu::MENU_NAME);

                if (menu5)
                {
                    ;// menu5->inputContext = RE::UserEvents::INPUT_CONTEXT_IDS::kCursor;
                }

                //auto menu6 = menu5->inputContext

                bool debug = true;

            }

            return;
            */
