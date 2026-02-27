
#include "main.hpp"
#include "Misc.hpp"
#include "InputActions.hpp"
//TODO: at least put it in groups. maybe tell what can be done with each group



namespace MiscThings {




    long long time_of_death = 0;


    void set_time_of_death(long long timestamp)
    {
        time_of_death = timestamp;
    }

    long long get_time_of_death()
    {
        return time_of_death;
    }

    struct RayCastResult {
        float distance = -1.0f;
        RE::COL_LAYER layer = RE::COL_LAYER::kUnidentified;
        RE::hkVector4 normalOut = RE::hkVector4(0, 0, 0, 0);
        bool didHit = false;

        // Do a null check before using this
        RE::TESObjectREFR* hitObjectRef = nullptr;

        RE::FormType GetHitObjectFormType_Safe() const {
            if (!hitObjectRef) return RE::FormType::None;
            return hitObjectRef->GetObjectReference()->GetFormType();
        }

        RayCastResult() = default;

        RayCastResult(float d, RE::COL_LAYER l, const RE::hkVector4& n, bool h, RE::TESObjectREFR* r)
            : distance(d), layer(l), normalOut(n), didHit(h), hitObjectRef(r) {
        }
    };


    RayCastResult RayCast(RE::NiPoint3 rayStart, RE::NiPoint3 rayDir, float maxDist,
        RE::Actor* actor) {
        RayCastResult result{};
        result.distance = maxDist;

        if (!actor) {
            return result;
        }
        const auto& cell = actor->GetParentCell();
        if (!cell) {
            return result;
        }
        const auto& bhkWorld = cell->GetbhkWorld();
        if (!bhkWorld) {
            return result;
        }

        RE::bhkPickData pickData;
        const auto& havokWorldScale = RE::bhkWorld::GetWorldScale();

        // Set ray start and end points (scaled to Havok world)
        pickData.rayInput.from = rayStart * havokWorldScale;
        pickData.rayInput.to = (rayStart + rayDir * maxDist) * havokWorldScale;

        RE::CFilter cFilter_info{};

        actor->GetCollisionFilterInfo(cFilter_info);

        pickData.rayInput.filterInfo = cFilter_info;

        // static_cast<RE::CFilter>(cFilter.filter | static_cast<uint32_t>(layerMask));

        // Perform the raycast
        if (bhkWorld->PickObject(pickData) && pickData.rayOutput.HasHit()) {
            result.didHit = true;
            result.distance = maxDist * pickData.rayOutput.hitFraction;
            result.normalOut = pickData.rayOutput.normal;

            result.layer = pickData.rayOutput.rootCollidable->GetCollisionLayer();

            result.hitObjectRef = RE::TESHavokUtilities::FindCollidableRef(*pickData.rayOutput.rootCollidable);
        }

        return result;
    }


    //excludes player
    RE::TESObjectREFR* GetRaycastRef(RE::NiPoint3 from, RE::NiPoint3 aimVector, float distance)
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        auto raycast_result = RayCast(from, aimVector, distance, player_actor);

        return raycast_result.hitObjectRef;
    }



    /*
    RE::TESObjectREFR* GetRaycastRef(RE::NiPoint3 from, RE::NiPoint3 aimVector)
    {
        //REL::Relocation<RE::NiPointer<SceneGraph>*> Main_spWorldRoot{ REL::ID(569647) };

        auto Main_spWorldRoot = RE::Main::WorldRootNode();

        RE::NiPoint3 location;
        RE::NiPoint3 aimVector;

        if (player) {
            if (Main_spWorldRoot) {
                if (Main_spWorldRoot->camera.get())
                {
                    //ViewPointToRay(Main_spWorldRoot->get()->spCamera.get(), 0.0, 0.0, location, aimVector);
                    Main_spWorldRoot->camera.get()->WindowPointToRay(1, 1, location, aimVector, 2.0f, 2.0f);

                    auto raycast_result = RayCast(location, aimVector, 500.0f, (RE::Actor*)player->AsReference());

                    return raycast_result.hitObjectRef;

                }
            }
        }

        return nullptr;
    }
    */






    bool is_on_horse()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();
        //auto extralist = &player->extraList;
        //auto horse_data = extralist->GetByType(RE::ExtraDataType::kHorse);

        //if (horse_data)
        if (player_actor->IsOnMount())
            return true;

        return false;
    }


    std::string get_potential_blocking_object()
    {
        std::string result = "";

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();

        if (player_ref)
        {
            RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 300.0f,
                //player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
                [&](RE::TESObjectREFR* a_ref) {

                    std::string name = a_ref->GetName();
                    std::string player_name = RE::PlayerCharacter::GetSingleton()->GetName();

                    auto base_obj = a_ref->GetBaseObject();
                    RE::FormType base_type{};

                    if (base_obj)
                    {
                        base_type = base_obj->GetFormType();
                        bool debug_type = true;
                    }
                    else
                    {
                        bool no_base_object = true;
                    }

                    if (base_type == RE::FormType::Activator)
                    {
                        auto static_obj = (RE::TESObjectACTI*)base_obj;

                        std::string model = static_obj->GetModel();

                        if (model.find("FXspiderWebKitDoorSpecial") != std::string::npos)
                        {
                            std::string name = MiscThings::insert_into_list_custom_name("[Destructible] Cobweb", a_ref);

                            result = name;
                        }

                        if (model.find("PuzzleDoorKeyHole01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            std::string name = MiscThings::insert_into_list_custom_name("[Puzzle door] Ancient Nordic Door", a_ref);
                            result = name;
                        }

                    }

                    if (base_type == RE::FormType::Door)// && a_ref->GetDisplayFullName() == "")
                    {
                        auto door = (RE::TESObjectDOOR*)base_obj;
                        std::string model = door->GetModel();

                        if (model.find("CaveGSecretDoor") != std::string::npos)
                        {
                            std::string name = MiscThings::insert_into_list_custom_name("[Secret door] Stone wall door", a_ref);
                            result = name;
                        }
                    }




                    RE::ExtraDataList* extralist = &a_ref->extraList;
                    auto extra_anim = extralist->GetByType(RE::ExtraDataType::kAnimGraphManager);
                    if (extra_anim)
                    {
                        auto extra_anim_graph = (RE::ExtraAnimGraphManager*)extra_anim;
                        if (extra_anim_graph->animGraphMgr)
                        {
                            if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "NorRetractableBridge01")
                            {
                                std::string name = MiscThings::insert_into_list_custom_name("Large wooden bridge", a_ref);
                                result = name;
                            }

                            if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "ImpPortcullisSmall01")
                            {
                                std::string name = MiscThings::insert_into_list_custom_name("Heavy wooden gate", a_ref);
                                result = name;
                            }

                            if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "PortcullisLarge01")
                            {
                                std::string name = MiscThings::insert_into_list_custom_name("Metal gate", a_ref);
                                result = name;
                            }

                        }
                    }

                    return RE::BSContainer::ForEachResult::kContinue;
                });
        }



        return result;
    }

    int get_destructible_state(RE::TESObjectREFR* web)
    {
        int result = -1;

        auto base_obj = web->GetBaseObject();
        if (base_obj && (base_obj->formFlags & RE::TESForm::RecordFlags::kDestructible))
        {
            auto base_type = base_obj->GetFormType();

            if (base_type == RE::FormType::Activator)
            {
                auto static_obj = (RE::TESObjectACTI*)base_obj;

                std::string model = static_obj->GetModel();

                if (model.find("FXspiderWebKitDoorSpecial") != std::string::npos)
                {
                    auto extralist = &web->extraList;
                    auto extra_swap = extralist->GetByType(RE::ExtraDataType::kModelSwap);
                    if (extra_swap)
                        result = 1;
                    else
                        result = 0;
                }

            }
        }

        return result;
    }



    int two_state_activator_state(RE::TESObjectREFR* activator)
    {
        //default2StateActivator
        int result = -1;
        auto object_p = General::Script::GetObject(activator, "default2StateActivator");

        if (object_p)
        {

            RE::BSFixedString prop_name = "::isOpen_var";

            if (General::Script::GetVariable<bool>(object_p, prop_name))
                result = 0;
            else
                result = 1;

        }

        object_p = General::Script::GetObject(activator, "HallofStoriesKeyholeScript");
        if (object_p)
        {
            //RE::BSFixedString prop_name = "doorOpened";
            //if (General::Script::GetProperty<bool>(object_p, prop_name))
            //    result = 0;
            //else
            //    result = 1;




            //for some reason these variables dont work?

            if (object_p->currentState != "done" && object_p->currentState != "busy" && object_p->currentState != "mainState")
                bool wtf_is_this = true;


            if (object_p->currentState == "done")
            {
                //RE::BSFixedString prop_name = "::doorOpened_var";

                //if (General::Script::GetVariable<bool>(object_p, prop_name))
                //    result = 0;
                //else
                result = 1;
            }
            else
            {
                if (object_p->currentState == "busy")
                {
                    //RE::BSFixedString prop_name = "::puzzleSolved_var";

                    //if (General::Script::GetVariable<bool>(object_p, prop_name))
                    //    result = 2; //opening
                   // else
                    result = 3;
                }
                else
                {
                    if (object_p->currentState == "mainState")
                    {
                        result = 4; //didnt move...
                    }
                }
            }




        }



        if (result == -1)
        {
            auto base_obj = activator->GetBaseObject();
            if (base_obj)
            {
                auto base_type = base_obj->GetFormType();

                if (base_type == RE::FormType::Door)
                {
                    //might be cave stone wall door
                    auto door = (RE::TESObjectDOOR*)activator;

                    if (door->GetOpenState(activator) == RE::BGSOpenCloseForm::OPEN_STATE::kOpen)
                        result = 0;

                    if (door->GetOpenState(activator) == RE::BGSOpenCloseForm::OPEN_STATE::kClosed)
                        result = 1;

                    if (door->GetOpenState(activator) == RE::BGSOpenCloseForm::OPEN_STATE::kOpening)
                        result = 2;

                    if (door->GetOpenState(activator) == RE::BGSOpenCloseForm::OPEN_STATE::kClosing)
                        result = 3;
                }
            }
        }




        return result;
    }



    int trap_firing(RE::TESObjectREFR* trap)
    {
        int result = -1;
        auto object_p = General::Script::GetObject(trap, "DartTrap");
        if (object_p)
        {

            RE::BSFixedString prop_name = "::isFiring_var";
            if (General::Script::GetVariable<bool>(object_p, prop_name))
            {
                result = 1; //dart trap firing
            }

        }

        object_p = General::Script::GetObject(trap, "PressurePlate");
        if (object_p)
        {
            if (object_p->currentState != "inactive")
            {
                result = 2; //pressure plate triggered
            }
        }

        object_p = General::Script::GetObject(trap, "TrapSwingingWall");
        if (object_p)
        {
            if (object_p->currentState != "Idle")
            {
                result = 3; //swinging spiked shit triggered
            }
            else
                result = 4;
        }

        object_p = General::Script::GetObject(trap, "BladeTrap");
        if (object_p)
        {
            //finishedPlaying

            if (object_p->currentState == "Idle")
            {
                result = 5; //blade trap deactivated
            }
            else
            {
                if (object_p->currentState == "On")
                    result = 6; //blade trap activated
            }

        }

        object_p = General::Script::GetObject(trap, "TrapFallingOilLampColLayer");
        if (object_p)
        {
            if (object_p->currentState != "Waiting")
            {
                result = 7; //explosive lamp fell
            }
        }

        object_p = General::Script::GetObject(trap, "TrapOilPool");
        if (object_p)
        {
            if (object_p->currentState != "Waiting")
            {
                result = 8; //oil on the floor started burning
            }
        }



        return result;
    }



    std::string get_pillar_face_name(RE::TESObjectREFR* object, int code)
    {
        std::string result = "";

        int type = -1;

        auto extralist = &object->extraList;
        auto extra_anim = extralist->GetByType(RE::ExtraDataType::kAnimGraphManager);
        if (extra_anim)
        {
            auto extra_anim_graph_manager = (RE::ExtraAnimGraphManager*)extra_anim;
            if (extra_anim_graph_manager->animGraphMgr)
            {
                auto graph_ptr = extra_anim_graph_manager->animGraphMgr->variableCache.animationGraph;

                if (graph_ptr)
                {
                    std::string project_name = "";
                    project_name = graph_ptr->projectName;

                    if (project_name == "RuinsPuzzlePillar01")
                    {
                        if (code == 1)
                            result = "[front Hawk]";
                        if (code == 2)
                            result = "[front Snake]";
                        if (code == 3)
                            result = "[front Whale]";
                    }

                    if (project_name.find("PuzzleDoor") != std::string::npos && project_name.find("Wheel02") != std::string::npos)
                    {
                        if (code == 1)
                            result = "[front Owl]";
                        if (code == 2)
                            result = "[front Bear]";
                        if (code == 3)
                            result = "[front Moth]";
                    }
                }
            }
        }

        return result;
    }



    std::string get_pillar_solved_text(RE::TESObjectREFR* pillar)
    {
        std::string result = "[Doesn't seem to be correct...]";

        auto object_p = General::Script::GetObject(pillar, "defaultPuzzlePillarScript");
        bool solved = false;

        if (object_p)
        {
            RE::BSFixedString prop_name = "pillarSolved";

            solved = General::Script::GetVariable<bool>(object_p, prop_name);

            if (solved)
                result = "[Probably correct]";
        }

        object_p = General::Script::GetObject(pillar, "HallofStoriesDiskScript");

        if (object_p)
        {
            RE::BSFixedString prop_name = "ringSolved";

            solved = General::Script::GetVariable<bool>(object_p, prop_name);

            if (solved)
                result = "[Probably correct]";
        }

        return result;
    }

    int get_pillar_face_name(RE::TESObjectREFR* pillar)
    {
        int result = 0;
        if (pillar)
        {
            auto extralist = &pillar->extraList;
            auto extra_anim = extralist->GetByType(RE::ExtraDataType::kAnimGraphManager);
            if (extra_anim)
            {
                auto extra_anim_graph_manager = (RE::ExtraAnimGraphManager*)extra_anim;
                if (extra_anim_graph_manager->animGraphMgr)
                {
                    auto graph_ptr = extra_anim_graph_manager->animGraphMgr->variableCache.animationGraph;



                    if (graph_ptr)
                    {
                        std::string project_name = "";
                        project_name = graph_ptr->projectName;

                        if (project_name == "RuinsPuzzlePillar01")
                        {
                            if (graph_ptr->behaviorGraph)
                            {
                                auto active_nodes = graph_ptr->behaviorGraph->activeNodes;
                                if (active_nodes)
                                {
                                    auto node_info = active_nodes->data();
                                    if (node_info && node_info->nodeTemplate)
                                    {
                                        int node_id = node_info->nodeTemplate->id;
                                        result = node_id;
                                    }
                                }
                            }
                        }

                        if (project_name.find("PuzzleDoor") != std::string::npos && project_name.find("Wheel02") != std::string::npos)
                        {
                            if (graph_ptr->behaviorGraph)
                            {
                                auto active_nodes = graph_ptr->behaviorGraph->activeNodes;
                                if (active_nodes)
                                {
                                    auto node_info = active_nodes->data();
                                    if (node_info && node_info->nodeTemplate)
                                    {
                                        int node_id = node_info->nodeTemplate->id;
                                        result = node_id;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return result;
    }




    char asciitolower(char in) {
        if (in <= 'Z' && in >= 'A')
            return in - ('Z' - 'z');
        return in;
    }

    std::string lowercase_string(std::string in)
    {
        std::string result = in;
        std::transform(result.begin(), result.end(), result.begin(), asciitolower);
        return result;
    }




    std::string old_quest_notification = "";
    std::string old_subquest_notification = "";
    std::string old_subquest_notification1 = "";
    std::string old_topleft_notification = "";


    void update_old_topleft_nofification(std::string new_notif)
    {
        old_topleft_notification = new_notif;
    }

    void notifications()
    {
        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();
        if (ui)
            if (const auto menu = ui->GetMenu<RE::HUDMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.HUDMovieBaseInstance.QuestUpdateBaseInstance.AnimatedLetter_mc.QuestName"))
                        if (!var1.IsNull() && var1.IsString())
                        {
                            std::string var_string = var1.GetString();

                            if (old_quest_notification != var_string)
                            {
                                old_quest_notification = var_string;

                                if (var_string.find("STARTED: ") == 0)
                                    var_string = "Quest started: " + insert_quest_into_list_and_get_info(var_string.substr(9, var_string.length() - 9));
                                if (var_string.find("COMPLETED: ") == 0)
                                    var_string = "Quest completed: " + insert_quest_into_list_and_get_info(var_string.substr(9, var_string.length() - 9));

                                send_random_context("[" + var_string + "]");
                            }
                        }


        if (ui)
            if (const auto menu = ui->GetMenu<RE::HUDMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.HUDMovieBaseInstance.QuestUpdateBaseInstance.objective0.ObjectiveTextFieldInstance.TextFieldInstance.text"))
                    {
                        if (!var1.IsNull() && var1.IsString())
                        {
                            std::string var_string = var1.GetString();

                            if (old_subquest_notification != var_string)
                            {
                                old_subquest_notification = var_string;

                                if (var_string.find("Completed") == 0 || var_string.find("Failed") == 0)
                                    var_string = "Subquest " + var_string;
                                else
                                    var_string = "New subquest: " + insert_quest_into_list_and_get_info(var_string);

                                send_random_context("[" + var_string + "]");
                            }
                        }
                    }
                    else
                    {
                        if (menu->uiMovie->GetVariable(&var1, "_root.HUDMovieBaseInstance.QuestUpdateBaseInstance.objective1.ObjectiveTextFieldInstance.TextFieldInstance.text"))
                            if (!var1.IsNull() && var1.IsString())
                            {
                                std::string var_string = var1.GetString();

                                if (old_subquest_notification1 != var_string)
                                {
                                    old_subquest_notification1 = var_string;

                                    if (var_string.find("Completed") == 0 || var_string.find("Failed") == 0)
                                        var_string = "Subquest " + var_string;
                                    else
                                        var_string = "New subquest: " + insert_quest_into_list_and_get_info(var_string);

                                    send_random_context("[" + var_string + "]");
                                }
                            }
                    }



        if (ui)
            if (const auto menu = ui->GetMenu<RE::HUDMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.HUDMovieBaseInstance.MessagesInstance.ShownMessageArray"))
                        if (var1.IsArray())
                        {
                            int size = var1.GetArraySize();
                            if (size > 0)
                            {
                                RE::GFxValue subvar;
                                if (var1.GetElement(0, &subvar)) //size-1 or 0? idk the order..
                                    if (subvar.GetMember("TextFieldClip", &subvar))
                                        if (subvar.GetMember("tf1", &subvar))
                                            if (subvar.GetMember("text", &subvar))
                                                if (subvar.IsString())
                                                {
                                                    std::string result_string = subvar.GetString();
                                                    if (result_string != old_topleft_notification)
                                                    {
                                                        old_topleft_notification = result_string;
                                                        if (result_string != "Autosaving..." && result_string != "Quicksaving..." && result_string != "Quickloading...")
                                                        {
                                                            if (result_string.find("hands are bound") != std::string::npos)
                                                                if (MiscThings::is_intro())
                                                                    result_string += ". Wait for the game to progress. ";
                                                                else
                                                                    result_string += ". You probably need to follow some quest right now. ";

                                                            send_random_context("[" + result_string + "]");
                                                        }
                                                    }
                                                }
                            }
                            else
                                old_topleft_notification = "";
                        }
    }




    // = (Optional) Search the barrel for potions








    bool player_has_levelup()
    {

        bool result = false;
        auto player = RE::PlayerCharacter::GetSingleton();

        //RE::BSTEventSource<RE::LevelIncrease::Event>* GetEventSource();

        auto test = RE::LevelIncrease::GetEventSource();

        auto xp_needed = player->skills->data->levelThreshold;
        auto current_xp = player->skills->data->xp;

        if (player && !player->IsDead() && current_xp > 0 && xp_needed > 0)
            result = current_xp >= xp_needed;


        return result;
    }



    float get_player_mana()
    {
        float result = 0.0f;
        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
            result = player->GetActorValue(RE::ActorValue::kMagicka);

        return result;
    }

    float get_player_health()
    {
        float result = 0.0f;
        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
            result = player->GetActorValue(RE::ActorValue::kHealth);

        return result;
    }

    float get_player_stamina()
    {
        float result = 0.0f;
        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
            result = player->GetActorValue(RE::ActorValue::kStamina);

        return result;
    }


    float get_player_max_mana()
    {
        float result = 0.0f;
        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
            result = player->GetBaseActorValue(RE::ActorValue::kMagicka);

        return result;
    }

    float get_player_max_health()
    {
        float result = 0.0f;
        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
            result = player->GetBaseActorValue(RE::ActorValue::kHealth);

        return result;
    }

    float get_player_max_stamina()
    {
        float result = 0.0f;
        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
            result = player->GetBaseActorValue(RE::ActorValue::kStamina);

        return result;
    }






    void clean_controls_from_string(std::string* string)
    {
        std::vector<char> controls = { '\n', '\t', '\r', '"' };
        bool last_was_space = false;
        for (int i = 0; i < string->length(); i++)
        {
            bool remember_old_space = false;

            for (auto control : controls)
                if (string->at(i) == control)
                {
                    string->erase(i, 1);
                    if (!last_was_space)
                        string->insert(i, " ");
                    else
                    {
                        i--;
                        remember_old_space = true;
                    }



                    break;
                }

            if (!remember_old_space)
                if (string->at(i) == ' ')
                    last_was_space = true;
                else
                    last_was_space = false;
        }
    }




    bool is_game_paused()
    {
        return RE::UI::GetSingleton()->GameIsPaused();
    }

    void pause_game()
    {
        RE::UIMessageQueue::GetSingleton()->AddMessage(RE::TweenMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
    }

    void unpause_game()
    {
        RE::UIMessageQueue::GetSingleton()->AddMessage(RE::TweenMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);

        right_attack_cancel(); //alt tabs usually kill leftclick so it stays clicked. ruins walker's mouse actions 
        mouse_mouse_x_y(0, 0);
        clear_input_queue();
    }




    RE::NiPoint3 rotate_vector_by_angles(RE::NiPoint3 v, RE::NiPoint3 object_angles)
    {
        RE::NiPoint3 result = v;

        float angleX = object_angles.x;
        float angleY = object_angles.y;
        float angleZ = object_angles.z;

        RE::NiPoint3 axisX = { 1.0f, 0.0f, 0.0f };
        RE::NiPoint3 axisY = { 0.0f, 1.0f, 0.0f };
        RE::NiPoint3 axisZ = { 0.0f, 0.0f, 1.0f };


        result = result * cos(angleX) + result.Cross(axisX) * sin(angleX) + axisX * (axisX * result) * (1 - cos(angleX));
        result = result * cos(angleY) + result.Cross(axisY) * sin(angleY) + axisY * (axisY * result) * (1 - cos(angleY));
        result = result * cos(angleZ) + result.Cross(axisZ) * sin(angleZ) + axisZ * (axisZ * result) * (1 - cos(angleZ));


        return result;
    }



    RE::NiPoint3 get_looking_point_shift(RE::TESObjectREFR* object, bool pickpocket_mode)
    {
        RE::NiPoint3 result{};

        if (object)
        {
            if (pickpocket_mode)
            {
                if (object->IsHumanoid())
                {
                    auto actor_obj = (RE::Actor*)object;

                    RE::NiPoint3 base_shift_vector = { 0.0f, -100.0f, 0.0f };

                    if (actor_obj->IsWalking() || actor_obj->IsRunning())
                        base_shift_vector = { 0.0f, -50.0f, 0.0f };

                    RE::NiPoint3 object_angles = object->data.angle;
                    RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                    result = rotated_shift_vector;
                }
            }
            else
            {
                auto base_obj = object->GetBaseObject();
                if (base_obj)
                {
                    if (base_obj->GetFormType() == RE::FormType::Furniture)
                    {

                        auto furniture = (RE::TESFurniture*)base_obj;
                        RE::NiPoint3 object_angles = object->data.angle;

                        std::string model = furniture->GetModel();
                        if (model.find("SmelterMarker") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -120.0f, 90.0f, 70.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("BlacksmithForgeMarker") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 120.0f, 30.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("BlacksmithSharpeningWheel") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 100.0f, 30.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("TanningRackMarker") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 80.0f, 40.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("BlacksmithWorkbench") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 80.0f, 40.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("AlchemyWorkbench") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 60.0f, 40.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("EnchantingWorkbench") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 60.0f, 40.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }
                        //LeverAnimatingMetal
                        if (model.find("LeverAnimating") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 15.0f, 90.0f, 0.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("GenPullChainAnim01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 40.0f, 100.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }


                    }


                    if (base_obj->GetFormType() == RE::FormType::Activator)
                    {

                        auto activator = (RE::TESFurniture*)base_obj;
                        RE::NiPoint3 object_angles = object->data.angle;

                        std::string model = activator->GetModel();


                        if (model.find("NorPullChain01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 10.0f, -30.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        //PuzzleDoorSmallWheel02
                        //PuzzleDoorMediumWheel02
                        //PuzzleDoorLargeWheel02

                        if (model.find("PuzzleDoorSmallWheel02") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -10.0f, 0.0f, 107.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("PuzzleDoorMediumWheel02") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -10.0f, 0.0f, 127.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("PuzzleDoorLargeWheel02") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -10.0f, 0.0f, 153.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("PuzzleDoorKeyHole01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -10.0f, 0.0f, 80.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        //NorPullBar01


                        if (model.find("NorPullBar01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 7.0f, 0.0f, 10.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                    }

                    if (base_obj->GetFormType() == RE::FormType::Door)
                    {

                        auto door = (RE::TESObjectDOOR*)base_obj;
                        RE::NiPoint3 object_angles = object->data.angle;

                        std::string model = door->GetModel();

                        if (model.find("WRCastleDoor01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -30.0f, 0.0f, -30.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }
                        

                    }
                    //


                    /*
                    if (base_obj->GetFormType() == RE::FormType::Activator)
                    {
                        auto activator = (RE::TESFurniture*)base_obj;
                        RE::NiPoint3 object_angles = object->data.angle;
                        std::string model = activator->GetModel();

                        if (model.find("FXspiderWebKitDoorSpecial") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 0.1f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }
                    }
                    */
                }
            }
        
            std::string name = object->GetDisplayFullName();

            if (name.find("Shrine of ") != std::string::npos)
            {
                RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 60.0f };
                result = base_shift_vector;
            }

        }
        return result;
    }












    bool quest_list_valid = false;

    std::vector<quest> quests;



    bool is_quest_list_valid()
    {
        return quest_list_valid;
    }

    std::vector<quest>* get_p_quest_list()
    {
        return &quests;
    }



    std::string replace_aliases(RE::TESQuest* quest, std::string displaytext)
    {
    

        if (displaytext.find("<Alias=") != std::string::npos)
        {
            //<Alias=RiverwoodFriend>
            auto alias_start = displaytext.find("<Alias=");
            auto alias_end = displaytext.find(">");
            if (alias_start < alias_end)
            {
                std::string alias_name = displaytext.substr(alias_start + 7, alias_end - alias_start - 7);
                for (auto alias : quest->aliases)
                {
                    if (alias)
                    {
                        std::string alias_codename = "";
                        alias_codename = alias->aliasName;

                        if (alias_codename == alias_name)
                        {
                            auto ref_alias = (RE::BGSRefAlias*)alias;
                            auto the_ref = ref_alias->GetReference();
                            std::string ref_name = the_ref->GetDisplayFullName();
                            displaytext = displaytext.substr(0, alias_start) + displaytext.substr(alias_end + 1, displaytext.length() - alias_end);
                            displaytext.insert(alias_start, ref_name);

                            displaytext = replace_aliases(quest, displaytext); //if there are more
                            break;
                        }
                    }
                }
            }
        }
        return displaytext;
    }




    std::pair<bool, std::string> get_current_quests()
    {
        std::pair<bool, std::string> result{};

        if (is_quest_list_valid())
        {
            quest_list_valid = false;
            quests.clear();

        }
        std::string result_text = "";


        auto player = RE::PlayerCharacter::GetSingleton();
        auto quest_targets = player->questTargets;

        auto my_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("myscPath");

        int id = 0;

        for (auto quest_target : quest_targets)
        {
            if (quest_target.first != my_quest)
            {
            
                RE::TESQuest* the_quest = quest_target.first;



                for (auto objective : the_quest->objectives)
                {
                    if (objective->state.all(RE::QUEST_OBJECTIVE_STATE::kDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kCompletedDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kFailedDisplayed))
                    {
                        quest this_quest{};

                        this_quest.id = id;
                        this_quest.quest = the_quest;
                        this_quest.name = the_quest->GetFullName();
                    

                        std::string displaytext = "";
                        displaytext = objective->displayText;

                        this_quest.displaytext += replace_aliases(the_quest, displaytext);

                        this_quest.objective = objective;
                        this_quest.description = "";
                        this_quest.category = 0;

                        quests.push_back(this_quest);

                        result_text += "[id " + std::to_string(this_quest.id) + "] " + this_quest.name + ": " + this_quest.displaytext;
                        result_text += "\n";
                        id++;
                    }
                }
            }
        }

        quest_list_valid = true;
        result.first = true;
        result.second = result_text;

        return result;


    }




    std::string insert_quest_into_list_and_get_info(std::string quest_text)
    {
        std::string result = quest_text;

        quest_text = lowercase_string(quest_text);

        if (quest_text != "")
        {
            bool found = false;
            for (auto quest : quests)
            {
                auto displaytext_lower = lowercase_string(quest.displaytext);
                auto name_lower = lowercase_string(quest.name);

                if (name_lower == quest_text)
                {
                    result = "[id " + std::to_string(quest.id) + "] " + quest.name;
                    found = true;
                    break;
                }

                if (displaytext_lower == quest_text)
                {
                    result = "[id " + std::to_string(quest.id) + "] " + quest.name + ": " + quest.displaytext;
                    found = true;
                    break;
                }


            }

            if (!found)
            {
                auto player = RE::PlayerCharacter::GetSingleton();
                auto quest_targets = player->questTargets;

                auto my_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("myscPath");

                //int id = 0;

                for (auto quest_target : quest_targets)
                {
                    if (quest_target.first != my_quest)
                    {

                        RE::TESQuest* the_quest = quest_target.first;



                        for (auto objective : the_quest->objectives)
                        {
                            if (objective->state.all(RE::QUEST_OBJECTIVE_STATE::kDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kCompletedDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kFailedDisplayed))
                            {
                                quest this_quest{};

                                this_quest.id = std::size(quests);
                                this_quest.quest = the_quest;
                                this_quest.name = the_quest->GetFullName();

                                std::string displaytext = "";
                                displaytext = objective->displayText;
                                this_quest.displaytext += replace_aliases(the_quest, displaytext);

                                this_quest.objective = objective;
                                this_quest.description = "";
                                this_quest.category = 0;


                                auto displaytext_lower = lowercase_string(this_quest.displaytext);
                                auto name_lower = lowercase_string(this_quest.name);

                                if (name_lower == quest_text)
                                {
                                    result = "[id " + std::to_string(this_quest.id) + "] " + this_quest.name;
                                    quests.push_back(this_quest);
                                    found = true;

                                    if (!quest_list_valid)
                                        quest_list_valid = true;
                                    result = "[id " + std::to_string(this_quest.id) + "] " + this_quest.name;

                                    break;
                                }


                                if (displaytext_lower == quest_text)
                                {
                                    result = "[id " + std::to_string(this_quest.id) + "] " + this_quest.name + ": " + this_quest.displaytext;
                                    quests.push_back(this_quest);
                                    found = true;

                                    if (!quest_list_valid)
                                        quest_list_valid = true;
                                    result = "[id " + std::to_string(this_quest.id) + "] " + this_quest.name + ": " + this_quest.displaytext;

                                    break;
                                }


                            }
                        }
                    }
                }
            }
        }

        return result;
    }





    bool locations_around_valid = false;
    std::map<int, RE::TESObjectREFR*> locations_around{};


    bool is_locations_around_valid()
    {
        return locations_around_valid;
    }


    bool is_location_in_the_list(RE::TESObjectREFR* object)
    {
        bool result = false;

        if (locations_around_valid)
        {
            for (auto list_entry : locations_around)
            {
                if (list_entry.second == object)
                {
                    result = true;
                    break;
                }
            }
        }
        return result;
    }



    std::map<int, RE::TESObjectREFR*>* get_p_locations_around()
    {
        if (locations_around_valid)
        {
            return &locations_around;
        }

        return nullptr;
    }


    std::string get_location_full_info(RE::TESObjectREFR* refr)
    {
        std::string marker_category = "";

        auto data = (RE::ExtraMapMarker*)refr->extraList.GetByType(RE::ExtraDataType::kMapMarker);
        if (data && data->mapData)
        {
            std::string marker_name = data->mapData->locationName.GetFullName();
            auto type = data->mapData->type;
        

            //if (type == RE::MARKER_TYPE::kNone) marker_category = "Location";
            if (type == RE::MARKER_TYPE::kCity) marker_category = "City";
            if (type == RE::MARKER_TYPE::kTown) marker_category = "Town";
            if (type == RE::MARKER_TYPE::kSettlement) marker_category = "Settlement";
            if (type == RE::MARKER_TYPE::kCave) marker_category = "Cave";
            if (type == RE::MARKER_TYPE::kCamp) marker_category = "Camp";
            if (type == RE::MARKER_TYPE::kFort) marker_category = "Fort";
            if (type == RE::MARKER_TYPE::kNordicRuin) marker_category = "Nordic Ruin";
            if (type == RE::MARKER_TYPE::kDwemerRuin) marker_category = "Dwemer Ruin";
            if (type == RE::MARKER_TYPE::kShipwreck) marker_category = "Shipwreck";
            if (type == RE::MARKER_TYPE::kGrove) marker_category = "Grove";
            if (type == RE::MARKER_TYPE::kLandmark) marker_category = "Landmark";
            if (type == RE::MARKER_TYPE::kDragonLair) marker_category = "Dragon Lair";
            if (type == RE::MARKER_TYPE::kFarm) marker_category = "Farm";
            if (type == RE::MARKER_TYPE::kWoodMill) marker_category = "Wood Mill";
            if (type == RE::MARKER_TYPE::kMine) marker_category = "Mine";
            if (type == RE::MARKER_TYPE::kImperialCamp) marker_category = "Imperial Camp";
            if (type == RE::MARKER_TYPE::kStormcloakCamp) marker_category = "Stormcloak Camp";
            if (type == RE::MARKER_TYPE::kDoomstone) marker_category = "Doomstone";
            if (type == RE::MARKER_TYPE::kWheatMill) marker_category = "Wheat Mill";
            if (type == RE::MARKER_TYPE::kSmelter) marker_category = "Smelter";
            if (type == RE::MARKER_TYPE::kStable) marker_category = "Stable";
            if (type == RE::MARKER_TYPE::kImperialTower) marker_category = "Imperial Tower";
            if (type == RE::MARKER_TYPE::kClearing) marker_category = "Clearing"; //??? wtf is this
            if (type == RE::MARKER_TYPE::kPass) marker_category = "Pass";
            if (type == RE::MARKER_TYPE::kAltar) marker_category = "Altar";
            if (type == RE::MARKER_TYPE::kRock) marker_category = "Rock";
            if (type == RE::MARKER_TYPE::kLighthouse) marker_category = "Lighthouse";
            if (type == RE::MARKER_TYPE::kOrcStronghold) marker_category = "Orc Stronghold";
            if (type == RE::MARKER_TYPE::kGiantCamp) marker_category = "Giant Camp";
            if (type == RE::MARKER_TYPE::kShack) marker_category = "Shack";
            if (type == RE::MARKER_TYPE::kNordicTower) marker_category = "Nordic Tower";
            if (type == RE::MARKER_TYPE::kNordicDwelling) marker_category = "Nordic Dwelling";
            if (type == RE::MARKER_TYPE::kDocks) marker_category = "Docks";
            if (type == RE::MARKER_TYPE::kShrine) marker_category = "Shrine";
            if (type == RE::MARKER_TYPE::kRiftenCastle) marker_category = "Riften Castle";
            if (type == RE::MARKER_TYPE::kRiftenCapitol) marker_category = "Riften Capitol";
            if (type == RE::MARKER_TYPE::kWindhelmCastle) marker_category = "Windhelm Castle";
            if (type == RE::MARKER_TYPE::kWindhelmCapitol) marker_category = "Windhelm Capitol";
            if (type == RE::MARKER_TYPE::kWhiterunCastle) marker_category = "Whiterun Castle";
            if (type == RE::MARKER_TYPE::kWhiterunCapitol) marker_category = "Whiterun Capitol";
            if (type == RE::MARKER_TYPE::kSolitudeCastle) marker_category = "Solitude Castle";
            if (type == RE::MARKER_TYPE::kSolitudeCapitol) marker_category = "Solitude Capitol";
            if (type == RE::MARKER_TYPE::kMarkarthCastle) marker_category = "Markarth Castle";
            if (type == RE::MARKER_TYPE::kMarkarthCapitol) marker_category = "Markarth Capitol";
            if (type == RE::MARKER_TYPE::kWinterholdCastle) marker_category = "Winterhold Castle";
            if (type == RE::MARKER_TYPE::kWinterholdCapitol) marker_category = "Winterhold Capitol";
            if (type == RE::MARKER_TYPE::kMorthalCastle) marker_category = "Morthal Castle";
            if (type == RE::MARKER_TYPE::kMorthalCapitol) marker_category = "Morthal Capitol";
            if (type == RE::MARKER_TYPE::kFalkreathCastle) marker_category = "Falkreath Castle";
            if (type == RE::MARKER_TYPE::kFalkreathCapitol) marker_category = "Falkreath Capitol";
            if (type == RE::MARKER_TYPE::kDawnstarCastle) marker_category = "Dawnstar Castle";
            if (type == RE::MARKER_TYPE::kDawnstarCapitol) marker_category = "Dawnstar Capitol";
            if (type == RE::MARKER_TYPE::kDLC02MiraakTemple) marker_category = "Miraak Temple";
            if (type == RE::MARKER_TYPE::kDLC02RavenRock) marker_category = "Raven Rock";
            if (type == RE::MARKER_TYPE::kDLC02StandingStone) marker_category = "Standing Stone";
            if (type == RE::MARKER_TYPE::kDLC02TelvanniTower) marker_category = "Telvanni Tower";
            if (type == RE::MARKER_TYPE::kDLC02ToSkyrim) marker_category = "Skyrim";
            if (type == RE::MARKER_TYPE::kDLC02ToSolstheim) marker_category = "Solstheim";
            if (type == RE::MARKER_TYPE::kDLC02CastleKarstaag) marker_category = "Castle Karstaag";
            if (type == RE::MARKER_TYPE::kPlayerSet) 
                marker_category = "Your marker";
            else
            {
                if (!data->mapData->flags)
                    marker_category += " [Undiscovered]";
                else
                    if (data->mapData->flags & RE::MapMarkerData::Flag::kVisible)
                    {
                        std::string name = "";
                        name = data->mapData->locationName.fullName;
                        marker_category = name + " [Discovered]";
                    }

                    else
                        marker_category == ""; //some weird flags
            }
        


        }
        return marker_category;
    }



    std::string insert_location_into_list_and_get_info(RE::TESObjectREFR* refr)
    {
        std::string result = "";

        if (refr)
        {
            bool found = false;
            for (auto object_entry : locations_around)
            {
                if (object_entry.second == refr)
                {
                    result = "[id " + std::to_string(object_entry.first) + "]" + get_location_full_info(refr);
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                int new_id = std::size(locations_around);

                locations_around.insert({ new_id, refr });
                if (!locations_around_valid)
                    locations_around_valid = true;
                result = "[id " + std::to_string(new_id) + "] " + get_location_full_info(refr);
            }
        }

        return result;
    }


    std::pair<bool, std::string> get_locations_around()
    {
        std::pair<bool, std::string> result{};

        std::string result_text = "Locations around you:\n";


        auto player = RE::PlayerCharacter::GetSingleton();

        RE::BSTArray<RE::ObjectRefHandle> map_markers = player->currentMapMarkers;


        for (auto marker : map_markers)
        {
            if (marker.get())
            {
                auto real_marker = marker.get().get();

                auto test_name = real_marker->GetDisplayFullName();

            

                auto data = (RE::ExtraMapMarker*)real_marker->extraList.GetByType(RE::ExtraDataType::kMapMarker);
                if (data && data->mapData)// && !data->mapData->flags)
                {
                    std::string marker_name = data->mapData->locationName.GetFullName();

                    if (marker_name != "")
                    {
                        auto distance = real_marker->GetDistance(player);
                        if (distance < 20000.0f) //looks like devs put exactly this value for compass. at least it holds for some markers around helgen i tried.
                        {
                            if (!MiscThings::is_location_in_the_list(real_marker))
                            {
                                std::string location_info = MiscThings::insert_location_into_list_and_get_info(real_marker);
                            }
                        }
                    }
                }
            }
        }





        for (auto location : locations_around)
        {
            std::string location_info = get_location_full_info(location.second);
            if (location_info != "")
                result_text += "[id " + std::to_string(location.first) + "]" + location_info + "\n";
        }

        result.first = true;
        result.second = result_text;

        return result;
    }







    bool objects_around_valid = false;
    std::map<int, RE::TESObjectREFR*> objects_around{};


    bool is_objects_around_valid()
    {
        return objects_around_valid;
    }

    bool is_object_in_the_list(RE::TESObjectREFR* object)
    {
        bool result = false;

        if (objects_around_valid)
        {
            for (auto list_entry : objects_around)
            {
                if (list_entry.second == object)
                {
                    result = true;
                    break;
                }
            }
        }
        return result;
    }



    std::map<int, RE::TESObjectREFR*>* get_p_objects_around()
    {
        if (objects_around_valid)
        {
            return &objects_around;
        }

        return nullptr;
    }


    void clear_object_list()
    {
        objects_around_valid = false;
        objects_around.clear();

        locations_around_valid = false;
        locations_around.clear();

    }





    std::string get_object_category(RE::TESForm* base_obj)
    {
        std::string result = "";

        auto base_type = base_obj->GetFormType();

        if (base_obj->IsInventoryObject())
            result = "[Item]"; //will be specified below if necessary

        if (base_type == RE::FormType::Book)
            result = "[Book]";

        if (base_type == RE::FormType::Weapon)
            result = "[Weapon]";

        if (base_type == RE::FormType::Armor)
            result = "[Armor]";

        if (base_type == RE::FormType::Outfit)
            result = "[Outfit]";

        if (base_type == RE::FormType::Ingredient)
            result = "[Ingredient]";

        if (base_type == RE::FormType::AlchemyItem)
            result = "[Potion]";

        if (base_type == RE::FormType::SoulGem)
            result = "[Soulgem]";

        if (base_type == RE::FormType::Note)
            result = "[Note]";

        if (base_type == RE::FormType::Ammo)
            result = "[Ammo]";


        return result;
    }

    std::string is_stealing(RE::TESObjectREFR* object)
    {
        std::string result = "";
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        auto base_obj = object->GetBaseObject();
        if (base_obj)
        {
            auto base_type = base_obj->GetFormType();
            if (base_obj->IsInventoryObject() || base_type == RE::FormType::Tree || base_type == RE::FormType::Flora)
            {
                if (player_actor && player_actor->WouldBeStealing(object))
                    result = "[Is stealing]";
            }
        }

        return result;
    }


    std::string get_object_category(RE::TESObjectREFR* object)
    {
        if (!object->AsReference())
        {
            bool wtf_is_this = true;
            return "???";
        }
        

        std::string result = "";

        auto base_obj = object->GetBaseObject();
        RE::FormType base_type{};

        if (base_obj)
        {
            base_type = base_obj->GetFormType();

            if (base_type == RE::FormType::Tree || base_type == RE::FormType::Flora)
                result = "[Harvestable]";

            if (base_type == RE::FormType::Door)
            {
                auto door_object = (RE::TESObjectDOOR*)object;
            
                auto extra = object->extraList.GetByType(RE::ExtraDataType::kTeleport);

                std::string leads_to = "";

                if (extra)
                {
                    auto extra_teleport = (RE::ExtraTeleport*)extra;

                    if (auto teleport_target_data = extra_teleport->teleportData; teleport_target_data)
                        if (auto teleport_target_handle = teleport_target_data->linkedDoor; teleport_target_handle)
                            if (auto teleport_target_refr = teleport_target_handle.get(); teleport_target_refr)
                                if (auto target_cell = teleport_target_refr->GetParentCell(); target_cell)
                                {
                                    leads_to = target_cell->GetName();

                                    if (leads_to == "")
                                        leads_to = "outside";
                                }
                                else
                                    leads_to = "outside";
                }

                if (leads_to != "")
                    result = "[Door to " + leads_to + "]";
                else
                    result = "[Door]";
            }
            

            if (base_type == RE::FormType::Activator)
                result = "[Interactive object]";

            if (object->IsActor())
            {
                std::string race = "";
                std::string dead = "";
                std::string child = "";


                auto actor_object = (RE::Actor*)object;
                if (actor_object->race)
                {
                    race = actor_object->race->GetName();
                    if (race == "Old People Race")
                        race = "Old";

                    std::string temp_name = "";
                    temp_name = object->GetDisplayFullName();

                    if (race == "Rabbit" || race == temp_name)
                        race = "";
                    else
                        race = ", " + race;
                }


                if (actor_object->IsDead())
                    dead = ", dead";
                
                if (actor_object->IsChild())
                    child = ", Child";

                if (object->IsHumanoid())
                {
                    result = "[Person" + race + dead + child + "]";
                }
                else
                    result = "[Creature" + race + dead + "]";

            }
            

            if (base_obj->IsInventoryObject())
                result = "[Item]"; //will be specified below if necessary



            if (base_type == RE::FormType::Container)
            {
                result = "[Container]";
                /*
                auto container_obj = (RE::TESObjectCONT*)base_obj;
                auto flags = container_obj->data.flags.get();

                int flags_int = (int)flags;

                std::string name = object->GetDisplayFullName();

                if (name == "Chest" && (flags_int & 2))
                    result += "RESPAWN";
                    */
            }
            

            if (base_type == RE::FormType::Furniture)
            {
                auto furniture = (RE::TESFurniture*)base_obj;
                auto workbenchtype = furniture->workBenchData.benchType;
                if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kNone)
                {
                    if (furniture->HasKeywordString("ActivatorLever") || furniture->HasKeywordString("isPullChain"))
                        result = "[Interactive object]";
                    else
                        result = "[Furniture]";
                }
                
                if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kAlchemy || workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kAlchemyExperiment)
                    result = "[Alchemy Lab]";
                if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kCreateObject)
                    result = "[Workbench]";
                if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kEnchanting || workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kEnchantingExperiment)
                    result = "[Enchanting table]";
                if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kSmithingArmor)
                    result = "[Armor workbench]";
                if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kSmithingWeapon)
                    result = "[Weapon workbench]";
            }
            

            if (base_type == RE::FormType::Book)
                result = "[Book]";

            if (base_type == RE::FormType::Weapon)
                result = "[Weapon]";

            if (base_type == RE::FormType::Armor)
                result = "[Armor]";

            if (base_type == RE::FormType::Outfit)
                result = "[Outfit]";

            if (base_type == RE::FormType::Ingredient)
                result = "[Ingredient]";

            if (base_type == RE::FormType::AlchemyItem)
                result = "[Potion]";

            if (base_type == RE::FormType::SoulGem)
                result = "[Soulgem]";

            if (base_type == RE::FormType::Note)
                result = "[Note]";

            if (base_type == RE::FormType::Ammo)
                result = "[Ammo]";



        }

        if (result == "")
        {
            bool no_category = true;
        }

        return result;

    }






    //weight
    //damage
    //armor
    //

    std::string get_stats_info(RE::TESObjectREFR* refr)
    {
        std::string result = "";

        auto base_obj = refr->GetBaseObject();
        if (base_obj)
        {
            auto base_type = base_obj->GetFormType();


            if (base_type == RE::FormType::Weapon)
            {
                
                auto weapon = (RE::TESObjectWEAP*)base_obj;

                std::string twohanded = "";

                if (weapon->IsTwoHandedAxe() || weapon->IsTwoHandedSword() || weapon->IsBow() || weapon->IsCrossbow())
                    twohanded = "Two-handed";
                else
                    twohanded = "One-handed";


                auto damage = weapon->attackDamage;

                std::stringstream ss;
                ss << std::fixed << std::setprecision(1) << weapon->weight;
                std::string weight_text_number = ss.str();
                

                std::string damage_text = "Damage: " + std::to_string(damage);
                std::string weight_text = "Weight: " + weight_text_number;

                result = " [" + twohanded + ", " + damage_text + ", " + weight_text + "]";
            }


            if (base_type == RE::FormType::Armor)
            {

                auto armor = (RE::TESObjectARMO*)base_obj;

                std::string heavy = "";

                if (armor->IsHeavyArmor())
                    heavy = "Heavy, ";


                auto armor_val = armor->armorRating;

                std::stringstream ss;
                ss << std::fixed << std::setprecision(1) << armor->weight;
                std::string weight_text_number = ss.str();

                std::string armor_val_text = "Armor: " + std::to_string(armor_val);
                std::string weight_text = "Weight: " + weight_text_number;

                result = " [" + heavy + armor_val_text + ", " + weight_text + "]";
            }

        }

        return result;
    }



    std::string get_object_full_info(RE::TESObjectREFR* refr)
    {
        std::string result = "";

        if (refr)
        {
            std::string name = refr->GetDisplayFullName();
            if (refr->IsActor())
            {
                auto actor_refr = (RE::Actor*)refr;
                if (actor_refr->race->fullName == "Dragon Race" && !is_fighting_dragons_allowed())
                    name = "Dragon";
            }

            std::string category = get_object_category(refr);
            int pillar_face_code = get_pillar_face_name(refr);
            std::string pillar_face_name = get_pillar_face_name(refr, pillar_face_code);
            std::string stealing = is_stealing(refr);
            std::string stats = get_stats_info(refr);

            result = category + stealing + " " + name + stats + pillar_face_name;
        }

        return result;
    }


    std::string get_object_stateless_info(RE::TESObjectREFR* refr) //nice names
    {
        std::string result = "";

        if (refr)
        {
            std::string name = refr->GetDisplayFullName();
            if (refr->IsActor())
            {
                auto actor_refr = (RE::Actor*)refr;
                if (actor_refr->race->fullName == "Dragon Race" && !is_fighting_dragons_allowed())
                    name = "Dragon";
            }

            std::string category = get_object_category(refr);

            result = category + " " + name;
        }

        return result;
    }

    std::string get_stateless_info(RE::TESObjectREFR* refr) //nice names
    {
        std::string result = "";

        if (refr)
        {
            for (auto object_entry : objects_around)
            {
                if (object_entry.second == refr)
                {
                    result = "[id " + std::to_string(object_entry.first) + "]" + get_object_stateless_info(refr);
                    break;
                }
            }
        }

        return result;
    }


    std::string insert_into_list_custom_name(std::string name, RE::TESObjectREFR* refr)
    {
        std::string result = "";

        if (refr)
        {
            bool found = false;
            for (auto object_entry : objects_around)
            {
                if (object_entry.second == refr)
                {
                    result = "[id " + std::to_string(object_entry.first) + "] " + name;
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                int new_id = std::size(objects_around);

                objects_around.insert({ new_id, refr });
                if (!objects_around_valid)
                    objects_around_valid = true;
                result = "[id " + std::to_string(new_id) + "] " + name;
            }
        }

        return result;
    }



    std::string insert_into_list_and_get_info(RE::TESObjectREFR* refr)
    {
        std::string result = "";

        if (refr)
        {
            bool found = false;
            for (auto object_entry : objects_around)
            {
                if (object_entry.second == refr)
                {
                    result = "[id " + std::to_string(object_entry.first) + "]" + get_object_full_info(refr);
                    found = true;
                    break;
                }
            }
        
            if (!found)
            {
                int new_id = std::size(objects_around);

                objects_around.insert({ new_id, refr });
                if (!objects_around_valid)
                    objects_around_valid = true;
                result = "[id " + std::to_string(new_id) + "] " + get_object_full_info(refr);
            }
        }

        return result;
    }





    /*
    int insert_ref_into_obj_list(RE::TESObjectREFR* refr)
    {
        if (refr)
        {
            for (auto object_entry : objects_around)
            {
                if (object_entry.second == refr)
                    return object_entry.first;
            }
            //not found. add in the end.
            int new_id = std::size(objects_around);

            objects_around.insert({ new_id, refr });

            if (!objects_around_valid)
                objects_around_valid = true;

            return new_id;
        }

        return -1;
    }
    */

    bool inventory_valid = false;

    //RE::TESObjectREFR::InventoryItemMap inventory{};
    
    /*
            RE::TESObjectREFR::InventoryItemMap inventory = RE::PlayerCharacter::GetSingleton()->GetInventory([](RE::TESBoundObject& a_object)
            {
                return true;// a_object.IsObject();
            });
     */

    struct item_data {
        RE::TESBoundObject* object;
        int amount;
        //int price; //TODO: replace with something useful (maybe type of item)
    };

    std::map<int, item_data> items_list{};




    void reset_misc()
    {
    
        //quest_list_valid = false; //they usually stay the same after saveload
        //quests.clear();

        //inventory_valid = false;
        //inventory.clear();
        //items_list.clear();

        locations_around.clear();
        locations_around_valid = false;

        old_quest_notification = "";
        old_topleft_notification = "";
        old_subquest_notification = "";
        old_subquest_notification1 = "";
    }


    bool is_equippable(RE::TESBoundObject* object)
    {
        bool result = false;
        auto item_form = (RE::TESForm*)object;
        std::string actions = "";

        if (item_form)
            if (item_form->IsArmor() || item_form->IsWeapon() || item_form->IsAmmo())
                result = true;

        return result;
    }


    bool is_consumable(RE::TESBoundObject* object)
    {
        bool result = false;
        auto item_form = (RE::TESForm*)object;
        std::string actions = "";

        if (item_form)
            if (item_form->formType == RE::FormType::Ingredient || item_form->formType == RE::FormType::AlchemyItem)
                result = true;

        return result;

    
    }




    //TODO: both hands/ left hand / right hand
    //



    RE::BGSEquipSlot* get_free_slot()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto actor_equip = RE::ActorEquipManager::GetSingleton();


        if (player && actor_equip)
        {
            //weapons
            auto actor_process = player->currentProcess;
            auto equipped_list = actor_process->equippedObjects;

            if (!equipped_list[1])
                return (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F42); //right hand

            if (!equipped_list[0])
                return (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F43); //left hand


            return (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F42);
        }

        return nullptr;
    }


    RE::BGSEquipSlot* get_weapon_equipped_slot(RE::TESBoundObject* object)
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto actor_equip = RE::ActorEquipManager::GetSingleton();


        if (object && player && actor_equip)
        {
            //weapons
            auto actor_process = player->currentProcess;
            auto equipped_list = actor_process->equippedForms;

            for (auto equipped_item : equipped_list)
            {
                if (equipped_item.object == object)
                    return equipped_item.slot;
            }
        }

        return nullptr;
    }

    std::string get_equipped_hand_text(RE::TESBoundObject* object)
    {
        std::string result = "";
        auto player = RE::PlayerCharacter::GetSingleton();
        auto actor_equip = RE::ActorEquipManager::GetSingleton();


        if (object && player && actor_equip)
        {

            //weapons
            auto actor_process = player->currentProcess;
            auto equipped_list = actor_process->equippedObjects;
            bool right_hand = false;
            bool left_hand = false;

            if (equipped_list[0] == object)
                left_hand = true; //left hand

            if (equipped_list[1] == object)
                right_hand = true; //right hand

            auto weapon = (RE::TESObjectWEAP*)object;


            if (weapon->IsTwoHandedAxe() || weapon->IsTwoHandedSword() || weapon->IsBow() || (right_hand && left_hand))
                result = ", both hands";
            else
            {
                if (right_hand)
                    result = ", right hand";
                else
                    if (left_hand)
                        result = ", left hand";
            }

        }
        return result;
    }

    bool is_equipped(RE::TESBoundObject* object)
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto actor_equip = RE::ActorEquipManager::GetSingleton();

        auto form_type = object->GetFormType();

        if (form_type == RE::FormType::Ammo)
        {
            auto player_actor = (RE::Actor*)player->AsReference();

            auto worn_ammo = player_actor->GetCurrentAmmo();

            auto ammo_object = (RE::TESBoundObject*)worn_ammo;

            if (ammo_object == object)
            {
                return true;
            }
        }


        if (object && player && actor_equip)
        {

            //weapons
            auto actor_process = player->currentProcess;
            auto equipped_list = actor_process->equippedForms;

            for (auto equipped_item : equipped_list)
            {
                if (equipped_item.object == object)
                    return true;
            }


            //apparel
            for (unsigned int slotInt = (unsigned int)RE::BGSBipedObjectForm::BipedObjectSlot::kHead; slotInt > 0; slotInt <<= 1)
            {
                RE::BGSBipedObjectForm::BipedObjectSlot slot = static_cast<RE::BGSBipedObjectForm::BipedObjectSlot>(slotInt);

                auto worn_thing = player->GetWornArmor(slot);

                if (worn_thing == object)
                    return true;
            }
        }
        return false;
    }


    std::pair<bool, std::string> activate_inventory_object_by_index(int item_id, int action_id)
    {
        std::pair<bool, std::string> result{};


        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();


        if (items_list.find(item_id) != items_list.end())
        {
            auto object = items_list.find(item_id)->second.object;


            RE::TESObjectREFR::InventoryItemMap inventory = RE::PlayerCharacter::GetSingleton()->GetInventory([](RE::TESBoundObject& a_object)
                {
                    return true;// a_object.IsObject();
                });


            if (player_ref && inventory.find(object) != inventory.end())
            {
                auto item_form = (RE::TESForm*)object;

                switch (action_id)
                {
                case 1:
                {
                    if (is_equippable(object))
                    {
                        auto actor_equip = RE::ActorEquipManager::GetSingleton();
                        if (is_equipped(object))
                        {
                            result.first = actor_equip->UnequipObject((RE::Actor*)player_ref, object);
                            result.second = "[Unequipping...]";
                        }
                        else
                        {
                            if (object->IsWeapon())
                                actor_equip->EquipObject((RE::Actor*)player_ref, object, nullptr, 1, get_free_slot());
                            else
                                actor_equip->EquipObject((RE::Actor*)player_ref, object);

                            result.second = "[Equipping...]";
                        }

                        result.first = true;
                        return result;
                    }
                    else
                    {
                        if (is_consumable(object))
                        {

                            auto actor_equip = RE::ActorEquipManager::GetSingleton();
                            //result = object->Activate(player_ref, player_ref, 0, nullptr, 1);
                            actor_equip->EquipObject((RE::Actor*)player_ref, object);

                            result.first = true;
                            result.second = "[Consuming...]";
                            return result;
                        }
                        else
                        {
                            result.first = false;
                            result.second = "Cannot activate this item";
                            return result;
                        }
                    }

                    break;
                }

                case 2:
                {
                    auto player_actor = (RE::Actor*)player_ref;


                    bool cant_drop = false;

                    if (object->IsWeapon())
                    {
                        auto weapon = (RE::TESObjectWEAP*)object;

                        cant_drop = weapon->weaponData.flags.all(RE::TESObjectWEAP::Data::Flag::kCantDrop);

                    }

                    if (object->IsKey())
                        cant_drop = true;


                    if (cant_drop)
                    {
                        result.first = false;
                        result.second = "Cant drop this item";
                        return result;
                    }
                    else
                    {
                        player_actor->DropObject(object, nullptr, 1);
                        result.second = "[Dropping the item...]";
                        return result;
                    }

                    break;
                }

                default:
                {
                    result.first = false;
                    result.second = "Invalid action ID";
                    return result;
                }
                }
            }
            else
            {
                result.first = false;
                result.second = "[You dont have this item anymore]";
                return result;
            }
        }
        else
        {
            result.first = false;
            result.second = "Invalid item ID";
            return result;
        }


    
        //result.first = false;
        //result.second = "Invalid command";

        //return result;
    }


    std::pair<bool, std::string> GetInventory()
    {
        std::pair<bool, std::string> result{};

        inventory_valid = false;
        //inventory.clear();
        items_list.clear();

        std::string inventory_contents = "Your inventory contents:\n";

        RE::TESObjectREFR::InventoryItemMap inventory = RE::PlayerCharacter::GetSingleton()->GetInventory([](RE::TESBoundObject& a_object)
            {
                return true;// a_object.IsObject();
            });



        std::string gold_name = "Gold";


        int id = 0;
        for (auto& [item, data] : inventory)
        {
           // auto item_ref = item->AsReference();
            auto item_form = (RE::TESForm*)item;


            std::string actions = "";

            if (item_form)
            {
                if (item_form->IsArmor() || item_form->IsWeapon() || item_form->IsAmmo())
                {
                    std::string hand_text = "";
                    std::string twohanded = "";
                    if (item_form->IsWeapon())
                    {
                        hand_text = get_equipped_hand_text(item);

                        
                        auto weapon = (RE::TESObjectWEAP*)item_form;

                        if (weapon->IsTwoHandedAxe() || weapon->IsTwoHandedSword() || weapon->IsBow())
                            twohanded = "[Two-handed]";
                        else
                            twohanded = "[One-handed]";
                            
                    }
                    

                    std::string stats = "";

                    auto base_type = item_form->GetFormType();


                    if (base_type == RE::FormType::Weapon)
                    {

                        auto weapon = (RE::TESObjectWEAP*)item_form;

                        auto damage = weapon->attackDamage;

                        std::stringstream ss;
                        ss << std::fixed << std::setprecision(1) << weapon->weight;
                        std::string weight_text_number = ss.str();


                        std::string damage_text = "Damage: " + std::to_string(damage);
                        std::string weight_text = "Weight: " + weight_text_number;

                        stats = " [" + damage_text + ", " + weight_text + "]";
                    }


                    if (base_type == RE::FormType::Armor)
                    {

                        auto armor = (RE::TESObjectARMO*)item_form;

                        std::string heavy = "";

                        if (armor->IsHeavyArmor())
                            heavy = "Heavy, ";


                        auto armor_val = armor->armorRating;

                        std::stringstream ss;
                        ss << std::fixed << std::setprecision(1) << armor->weight;
                        std::string weight_text_number = ss.str();

                        std::string armor_val_text = "Armor: " + std::to_string(armor_val);
                        std::string weight_text = "Weight: " + weight_text_number;

                        stats = " [" + heavy + armor_val_text + ", " + weight_text + "]";
                    }



                    if (is_equipped(item))
                        actions += "[Equipped" + hand_text + "][Can unequip]";
                    else
                        actions += "[Can equip]" + twohanded;

                    actions += stats;
                }
                
                if (item_form->formType == RE::FormType::Ingredient || item_form->formType == RE::FormType::AlchemyItem)
                    actions += "[Can consume]";

                if (data.first > 0 && item->GetName() != gold_name)
                {
                    inventory_contents += "[id " + std::to_string(id) + "] ";
                    inventory_contents += item->GetName();
                    inventory_contents += " x";
                    inventory_contents += std::to_string(data.first) + " ";
                    inventory_contents += get_object_category(item_form);
                    inventory_contents += actions;
                    inventory_contents += "\n"; //TODO: replace with comma later

                    item_data database_data{};
                    database_data.amount = data.first;
                    database_data.object = data.second->GetObject();

                    items_list.insert({ id , database_data });
                    id++;

                }
            }
        }

        auto player = RE::PlayerCharacter::GetSingleton();

        std::string cur_weight = std::to_string((int)player->GetActorValue(RE::ActorValue::kInventoryWeight));
        std::string max_weight = std::to_string((int)player->GetActorValue(RE::ActorValue::kCarryWeight));

        inventory_contents += "Total weight: " + cur_weight + "/" + max_weight;

        inventory_valid = true;

        result.first = true;
        result.second = inventory_contents;


        return result;
    }





    bool has_digits(std::string& str)
    {
        for (char ch : str) {
            int v = ch; // ASCII Val converted
            if (ch >= 48 && ch <= 57) {
                return true;
            }
        }

        return false;
    }



    bool is_digits(std::string& str)
    {
        for (char ch : str) {
            int v = ch; // ASCII Val converted
            if (!(ch >= 48 && ch <= 57)) {
                return false;
            }
        }

        return true;
    }


    std::string fix_spell_description(std::string description, RE::SpellItem* spell)
    {
        bool something_found = false;

        if (auto pos = description.find("<"); pos != std::string::npos)
        {
            auto pos2 = description.find(">");
            if ((pos < (pos2 - 1)) && pos2 < description.length())
            {
                std::string keyword = description.substr(pos + 1, pos2 - pos - 1);


                std::string replacement = "???";

                if (keyword == "mag" || keyword == "MAG")
                    if (spell->effects.front())
                    {
                        something_found = true;
                        replacement = std::to_string((int)spell->effects.front()->GetMagnitude());
                    }

                if (keyword == "dur")
                    if (spell->effects.front())
                    {
                        something_found = true;
                        replacement = std::to_string((int)spell->effects.front()->GetDuration());
                    }


                if (is_digits(keyword))
                {
                    something_found = true;
                    replacement = keyword;
                }



                if (replacement != "???")
                {
                    description.erase(pos, pos2 - pos + 1);
                    description.insert(pos, replacement);
                }
            }

        }

        if (something_found)
            return fix_spell_description(description, spell);
        else
            return description;
    }






    struct spell {
        int id;
        RE::SpellItem* spell;
        RE::TESShout* shout;
    };

    std::vector<spell> spells{};



    bool does_spell_use_hand(RE::SpellItem* spell)
    {
        bool result = false;

        auto equip_manager = RE::ActorEquipManager::GetSingleton();

        if (equip_manager)
        {
            auto equip_slot = spell->GetEquipSlot();

            auto slot_id = equip_slot->GetFormID();

            if (slot_id == 0x00013F44) //either hand
            {
                result = true;
            }
        }

        return result;
    }


    std::pair<bool, std::string> get_available_spells()
    {
        std::pair<bool, std::string> result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        std::string result_text = "List of spells available to use: \n";


        player->GetCurrent3D();

        class MyVisitor : public RE::Actor::ForEachSpellVisitor
        {
        public:
            std::string* active_spells;
            std::string* passive_effects;
            //std::string* shouts; //this doesnt work for shouts
            std::string* ults;
            int* max_id;

            int i = 0;
            RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();



            RE::BSContainer::ForEachResult Visit(RE::SpellItem* a_spell) override {
                //if (active_spells && passive_effects && shouts && player)// && player->HasSpell(a_spell))
                if (active_spells && passive_effects && player)
                {
                    std::string name = a_spell->GetFullName();
                    std::string description = "";



                    auto effect = a_spell->GetAVEffect();

                    std::string descr = "";

                    if (effect)
                    {

                        descr = effect->magicItemDescription;
                        descr = fix_spell_description(descr, a_spell);


                    }

                    if (descr == "")
                    {
                        RE::BSString temp_string;

                        a_spell->GetDescription(temp_string, a_spell, 'MANC');

                        descr = temp_string;
                        descr = fix_spell_description(descr, a_spell);

                    }



                    description = " " + descr;

                    if (a_spell->menuDispObject)
                    {
                        if (a_spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell)
                        {
                            *active_spells += "[id " + std::to_string(i) + "] " + name + " - " + description + "[Takes hand slot]\n";
                            spells.push_back({ i, a_spell, nullptr });
                            i++;
                            *max_id = i;
                        }
                        else
                            //if (a_spell->GetSpellType() == RE::MagicSystem::SpellType::kVoicePower)
                            //{
                            //    *shouts += "[id " + std::to_string(i) + "] " + name + " - " + description + "\n";
                            //    spells.push_back({ i, a_spell });
                            //     i++;
                            //}
                            //else
                            {
                                if (a_spell->GetCastingType() == RE::MagicSystem::CastingType::kConstantEffect)
                                    *passive_effects += name + " - " + description + "\n";
                                else
                                {
                                    std::string ult_type = "";
                                    if (does_spell_use_hand(a_spell))
                                        ult_type = "[Takes hand slot]";
                                    else
                                        ult_type = "[Instant use]";

                                    *ults += "[id " + std::to_string(i) + "] " + name + " - " + description + " " + ult_type + "\n";
                                    spells.push_back({ i, a_spell, nullptr });
                                    i++;
                                    *max_id = i;
                                }

                            }
                    }
                    else
                    {
                        if (a_spell->GetSpellType() == RE::MagicSystem::SpellType::kDisease)
                        {
                            *passive_effects += "[DESEASE] " + name + " - " + description + "\n";
                        }

                    }
                }
                return RE::BSContainer::ForEachResult::kContinue;
            }
        };

        MyVisitor visitor{};

        RE::Actor::ForEachSpellVisitor* test_visitor = &visitor;


        std::string active_spells = "";
        std::string passive_effects = "";
        std::string shouts = "";
        std::string ults = "";
        int max_id = 0;

        visitor.active_spells = &active_spells;
        visitor.passive_effects = &passive_effects;
        //visitor.shouts = &shouts;
        visitor.ults = &ults;
        visitor.max_id = &max_id;

        player->VisitSpells(visitor);
        auto player_actor = (RE::Actor*)player->AsReference();

        RE::TESNPC* player_npc = (RE::TESNPC*)RE::TESForm::LookupByID(0x7); //left hand
            //RE::TESNPC::LookupByID<RE::TESNPC>(RE::TESForm::GetFormID("000007", "Skyrim.esm")->formID);

        //auto player_ref = player->AsReference();

        //RE::TESNPC* player_npc = (RE::TESNPC*)player_ref;

        auto spellList = player_npc->GetSpellList();

        auto shouts_pp = spellList->shouts;


        int dragon_souls = player->GetActorValue(RE::ActorValue::kDragonSouls);

        for (auto* shout_p : std::span(shouts_pp, spellList->numShouts))
        {
            if (shout_p)
            {
                int known_words = 0;
                int unlocked_words = 0;
                int max_words = 0;
                //bool set_known_one = false;

                for (auto variation : shout_p->variations)
                {
                    if ((variation.word->formFlags & 65600) == 65600)
                        unlocked_words++;

                    if (variation.word->GetKnown())
                        known_words++;
                    //else
                    //{
                        //if (!set_known_one)
                        //{
                         //   variation.word->SetPlayerKnows(true); //this works. but how to get if its learned? (if dragonsoul injected)
                            //player_actor->UnlockWord(variation.word);
                     //       known_words++;
                     //       set_known_one = true;
                     //   }

                    //}


                    max_words++;
                }



                std::string name = shout_p->GetFullName();

                name += "[Words (unlocked/known/total): " + std::to_string(unlocked_words) + "/" + std::to_string(known_words) + "/" + std::to_string(max_words) + "]";

                std::string description = "";

                std::string descr = "";
            
                if (descr == "")
                {
                    RE::BSString temp_string;
                    shout_p->GetDescription(temp_string, shout_p, 'CSED');
                    descr = temp_string;
                    //descr = fix_spell_description(descr, shout_p); //TODO
                }

                description = descr;

                shouts += "[id " + std::to_string(max_id) + "] " + name + " - " + description + "\n";
                spells.push_back({ max_id, nullptr, shout_p });
                max_id++;
            }
        }

        /*
        auto addedSpells = &player->addedSpells;

        for (auto addedSpell : *addedSpells)
        {
            std::string name_addedspell = "";
            name_addedspell = addedSpell->GetFullName();
            bool stop_here1233123 = false;
        }
        //auto test = RE::TESForm::GetAllForms();
    
        RE::BGSListForm::ForEachForm(
            [&](RE::TESForm* a_form) {
                return RE::BSContainer::ForEachResult::kContinue;
            });
    */

        if (active_spells != "")
            result_text += "Active spells:\n" + active_spells + "\n";

        if (shouts != "")
            result_text += "Shouts:\n" + shouts + "\n";

        if (ults != "")
            result_text += "Special (ults, may be limited to 1 use per day):\n" + ults + "\n";

        if (passive_effects != "")
            result_text += "Passive effects:\n" + passive_effects + "\n";

        result.first = true;
        result.second = result_text;

        return result;
    }



    std::pair<bool, std::string> unlock_shout_level(int shout_id)
    {
        
        std::pair<bool, std::string> result{};

        if (shout_id >= 0 && shout_id < std::size(spells))
        {
            auto shout_p = spells.at(shout_id).shout;
            if (shout_p)
            {
                for (auto variation : shout_p->variations)
                {
                    if (variation.word->GetKnown()) //known
                    {
                        if (!(variation.word->formFlags & (65536))) //not unlocked     //bit16 - unlocked, bit6 - known
                        {
                            auto player = RE::PlayerCharacter::GetSingleton();
                            auto player_actor = (RE::Actor*)player->AsReference();
                            int dragon_souls = player->GetActorValue(RE::ActorValue::kDragonSouls);
                            if (dragon_souls > 0)
                            {
                                player_actor->UnlockWord(variation.word);
                                player->SetActorValue(RE::ActorValue::kDragonSouls, -1);

                                result.first = true;
                                result.second = "[Unlocked shout level]";

                                break;
                            }
                            else
                            {
                                result.first = false;
                                result.second = "Not enough dragon souls. ";
                                break;
                            }
                        }
                        else
                        {
                            result.first = false;
                            result.second = "Already unlocked all levels of this shout. ";
                        }
                    }
                    else
                    {
                        result.first = false;
                        result.second = "You dont know new levels of this shout yet. ";
                    }
                }
            }
            else
            {
                result.first = false;
                result.second = "This spell ID is not a shout. ";
            }
        }
        else
        {
            result.first = false;
            result.second = "Invalid shout ID. Use get_available_spells to get valid IDs.  ";
        }

        return result;
    }





    std::pair<bool, std::string> use_spell_by_index(int id)
    {

        //TODO: somehow get proper result of this action
        //TODO: some abilities are one time per day use but must be equipped. need different mechanic 



        std::pair<bool, std::string> result{};

        if (MiscThings::is_intro() || MiscThings::is_intro2())
        {
            result.first = false;
            result.second = "Cannot use spells right now. ";
            return result;
        }

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)(player->AsReference());


        if (player_actor && id >= 0 && id < std::size(spells))
        {
            if (spells.at(id).spell)
            {
                RE::SpellItem* spell = spells.at(id).spell;

                auto equip_slot = spell->GetEquipSlot();

                auto slot_id = equip_slot->GetFormID();

                auto equip_manager = RE::ActorEquipManager::GetSingleton();

                if (equip_manager)
                {
                    if (slot_id == 0x00025BEE) //voice
                    {
                        equip_manager->EquipSpell(player_actor, spell, equip_slot);
                        use_ult();
                        result.first = true;
                        result.second = "[Casting the spell]";
                    }
                    else
                    {
                        if (slot_id == 0x00013F44) //either hand
                        {
                            equip_manager->EquipSpell(player_actor, spell, get_free_slot());
                            result.first = true;
                            result.second = "[Casting the spell]";
                        }
                        else
                        {
                            result.first = false;
                            result.second = "Cannot use this spell. ";
                        }
                    }
                }
            }
            else
            {
                if (spells.at(id).shout)
                {
                    RE::TESShout* shout = spells.at(id).shout;

                    auto equip_slot = shout->GetEquipSlot();

                    auto slot_id = equip_slot->GetFormID();

                    auto equip_manager = RE::ActorEquipManager::GetSingleton();

                    if (equip_manager)
                    {
                        if (slot_id == 0x00025BEE) //voice
                        {
                            if (player_actor->GetVoiceRecoveryTime() < 0.01)
                            {
                                int unlocked_words = 0;

                                for (auto variation : shout->variations)
                                {
                                    if ((variation.word->formFlags & 65600) == 65600)
                                        unlocked_words++;
                                }
                                if (unlocked_words > 0)
                                {
                                    equip_manager->EquipShout(player_actor, shout);
                                    //use_ult();
                                    make_long_ult_cast();
                                    result.first = true;
                                    result.second = "[Using the shout]";
                                }
                                else
                                {
                                    result.first = false;
                                    result.second = "You have no unlocked words of this shout. ";
                                }
                            }
                            else
                            {
                                result.first = false;
                                result.second = "Cannot shout, the voice has not recovered yet. ";
                            }
                        }
                        else
                        {
                            result.first = false;
                            result.second = "Cannot use this shout. ";
                        }
                    }
                    else
                    {
                        result.first = false;
                        result.second = "You cannot use it now. ";
                    }
                }
                else
                {
                    result.first = false;
                    result.second = "Cannot use this spell. ";
                }
            }

        }
        else
        {
            result.first = false;
            result.second = "Invalid spell ID. Use get_available_spells to get valid IDs. ";
        }


        return result;
    }










    std::pair<bool, std::string> GetGold()
    {
        std::pair<bool, std::string> result{};

        std::string inventory_contents = "You have ";
        const auto inv = RE::PlayerCharacter::GetSingleton()->GetInventory([](RE::TESBoundObject& a_object)
            {
                return a_object.IsGold();// idk why but this doesnt work
            });

        bool have_gold = false;
        std::string gold_name = "Gold";

        for (auto& [item, data] : inv)
        {
            if (data.first > 0 && item->GetName() == gold_name)
            {
                inventory_contents += std::to_string(data.first);
                have_gold = true;
            }
        }

        if (!have_gold)
            inventory_contents += " 0 ";

        inventory_contents += " gold";

        result.first = true;
        result.second = inventory_contents;

        return result;
    }

    bool is_intro()
    {
        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking);
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        bool result = false;
        auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
        if (threshold_quest)
            if (threshold_quest->GetCurrentStageID() < 160)
                result = true;


        result |= !can_walk || !can_look;

        return result;
    }

    bool is_intro2()
    {
        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking);
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);


        bool result = false;
        auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
        if (threshold_quest)
            if (threshold_quest->GetCurrentStageID() < 200)
                result = true;

        result |= !can_interact;

        return result;
    }

    bool is_fighting_dragons_allowed()
    {
        bool result = false;
        auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ102");
        if (threshold_quest)
            if (threshold_quest->GetCurrentStageID() > 30)
                result = true;

        return result;
    }

    std::vector<RE::Actor*> get_player_attackers()
    {
        std::vector<RE::Actor*> result{};


        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();
        auto player_actor = (RE::Actor*)player_ref;

        float scan_distance = 3000.0f;
        auto player_cell = player->GetParentCell();
        if (player_cell && player_cell->IsInteriorCell())
            scan_distance = 1000.0f;

        RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 9000.0,
            //player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
            [&](RE::TESObjectREFR* a_ref) {

                if (a_ref->IsActor())
                {
                    auto actor_refr = (RE::Actor*)a_ref;

                    auto controller = actor_refr->combatController;

                    if (controller)
                    {
                        auto target_handle = controller->targetHandle;
                        auto target_ref = RE::TESObjectREFR::LookupByHandle(target_handle.native_handle());




                        if (actor_refr->race->fullName != "Dragon Race" || is_fighting_dragons_allowed())
                        {
                            if (target_ref && target_ref.get() == player_ref)
                                result.push_back(actor_refr);
                            else
                            {
                                auto is_enemy = actor_refr->GetFactionReaction(player_actor);

                                if (is_enemy == RE::FIGHT_REACTION::kEnemy)
                                {
                                    if (controller->startedCombat)
                                        result.push_back(actor_refr);
                                }
                            }
                        }

                    
                    }
                }


                return RE::BSContainer::ForEachResult::kContinue;
            });

        auto player_pos = player_ref->GetPosition();


        std::sort(result.begin(), result.end(), [&](RE::Actor* left, RE::Actor* right) {
            //return left->GetDistance(player) > right->GetDistance(player); //switch > to < for inversed order. this is last->closest
            RE::NiPoint3 pos_left = left->GetPosition();
            RE::NiPoint3 pos_right = right->GetPosition();

            return pos_left.GetDistance(player_pos) < pos_right.GetDistance(player_pos); //alphabetical order. top = A

            });



        return result;
    }



    std::pair<bool, std::string> GetObjectsAround(int type)
    {
        std::pair<bool, std::string> result{};


        auto player = RE::PlayerCharacter::GetSingleton();

        auto player_ref = player->AsReference();

        std::vector<RE::TESObjectREFR*> objects{};

        //objects_around.clear();


        int i = 0;


        /*
        RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 4000.0,
            //player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
            [&](RE::TESObjectREFR* a_ref) {
                std::string name = a_ref->GetName();
                std::string player_name = RE::PlayerCharacter::GetSingleton()->GetName();

                //objects.push_back(a_ref); //UNLIMITED
                //return RE::BSContainer::ForEachResult::kContinue;

                if (!a_ref)
                    return RE::BSContainer::ForEachResult::kContinue;

                if (name[0] != '\0' && std::size(name) > 1 && name != player_name)
                {

                    auto base_obj = a_ref->GetBaseObject();
                    RE::FormType base_type{};

                    if (base_obj)
                    {
                        base_type = base_obj->GetFormType();
                        bool debug_type = true;


                    }
                    else
                    {
                        bool no_base_object = true;
                    }

                    std::string form_editor_name = a_ref->GetFormEditorID();

                    if (form_editor_name.find("Merchant") != std::string::npos && form_editor_name.find("Chest") != std::string::npos)
                        return RE::BSContainer::ForEachResult::kContinue; //attempt to skip merchant secret chests


                    if (a_ref->AsReference()->modelState == 0)
                        return RE::BSContainer::ForEachResult::kContinue; //skip objects without world model


                    if (has_digits(name))
                        return RE::BSContainer::ForEachResult::kContinue;



                    if (base_type == RE::FormType::Activator)
                    {
                        auto test = (RE::TESObjectACTI*)base_obj;
                        std::string model = test->GetModel();
                        if (model.find("Marker_LinkMarker") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                            return RE::BSContainer::ForEachResult::kContinue;
                    }


                    if (base_type == RE::FormType::Furniture)
                    {
                        //-		benchType	kAlchemy	REX::EnumSet<enum RE::TESFurniture::WorkBenchData::BenchType,unsigned char>

                        auto furniture = (RE::TESFurniture*)base_obj;

                        if (furniture->workBenchData.benchType != RE::TESFurniture::WorkBenchData::BenchType::kNone)
                        {
                            //this is a functional workbench.
                            ;
                        }
                        else
                        {
                            auto test = (RE::TESFurniture*)base_obj;
                            std::string model = test->GetModel();
                            if (model.find("Markers//") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                                return RE::BSContainer::ForEachResult::kContinue;
                        }
                    }

                    if (name.find("not be visible") != std::string::npos) //"This should not be visible [Furniture]"
                        return RE::BSContainer::ForEachResult::kContinue;

                    if (name.find("Do Not Delete") != std::string::npos)
                        return RE::BSContainer::ForEachResult::kContinue;

                    if (name.find("Invisible") != std::string::npos && name.find("Marker") != std::string::npos)
                        return RE::BSContainer::ForEachResult::kContinue;




                    switch (type)
                    {
                    case (1):
                    {
                        if (a_ref->AsReference()->IsActor())
                        {
                            objects.push_back(a_ref);
                            //objects_around.insert({ i, a_ref });
                            i++;
                        }
                        break;
                    }

                    case (2):
                    {
                        if (a_ref->AsReference()->IsInventoryObject())
                        {
                            objects.push_back(a_ref);
                            //objects_around.insert({ i, a_ref });
                            i++;
                        }
                        break;
                    }

                    case (3):
                    {
                        //if (a_ref->AsReference()->HasContainer() && !a_ref->AsReference()->IsActor() && a_ref->AsReference()->modelState != 0)
                        if (base_type == RE::FormType::Container)
                        {
                            //TODO: filter merchant chests
                            
                            //const std::string_view a_editorID = "MerchantSolitudeBlacksmith";
                            //std::string form_editor_name = a_ref->GetFormEditorID();
                            //std::string form_editor_name2 = base_obj->GetFormEditorID();
                            //auto test_form = RE::TESObjectREFR::LookupByID(0xa6c07);
                            //auto test_form2 = RE::TESObjectREFR::LookupByID(0xa6c0b);
                            //std::string form_editor_name3 = test_form->GetFormEditorID();
                            //std::string form_editor_name4 = test_form2->GetFormEditorID();
                            //auto test123 = base_obj->GetFormID();
                            //auto test = RE::TESForm::LookupByEditorID(a_editorID);
                            

                            objects.push_back(a_ref);
                            //objects_around.insert({ i, a_ref });
                            i++;
                        }
                        break;
                    }

                    case (4):
                    {
                        //fill with somethins useful
                        break;
                    }

                    case (5):
                    {

                        if (base_type == RE::FormType::Door)
                        {
                            objects.push_back(a_ref);
                            //objects_around.insert({ i, a_ref });
                            i++;
                        }

                        break;
                    }

                    case (6):
                    {
                        if (base_type == RE::FormType::Activator)
                        {
                            objects.push_back(a_ref);
                            i++;
                        }

                        if (base_type == RE::FormType::Furniture)
                        {
                            auto furniture = (RE::TESFurniture*)base_obj;
                            auto workbenchtype = furniture->workBenchData.benchType;
                            if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kNone)
                            {
                                if (furniture->HasKeywordString("ActivatorLever") || furniture->HasKeywordString("isPullChain"))
                                {
                                    objects.push_back(a_ref);
                                    i++;
                                }
                            }
                        }



                        break;
                    }


                    case (7):
                    {
                        if (base_obj)
                        {
                            bool is_harvestable = false;

                            if (base_type == RE::FormType::Tree)
                            {
                                auto tree_form = (RE::TESObjectTREE*)base_obj;

                                auto test_flags = a_ref->AsReference()->GetFormFlags();

                                bool already_harvested = false;

                                if (test_flags & RE::TESObjectREFR::RecordFlags::kHarvested) //THIS FLAG IS POTENTIALLY INCORRECT.
                                    already_harvested = true;

                                if (test_flags & 2048) //this is potentially only one we need here
                                    already_harvested = true;




                                if (tree_form->produceItem && !already_harvested)
                                    is_harvestable = true;
                            }

                            if (base_type == RE::FormType::Flora)
                            {
                                auto tree_form = (RE::TESFlora*)base_obj;

                                if (tree_form->produceItem)
                                    is_harvestable = true;
                            }


                            if (is_harvestable)
                            {
                                objects.push_back(a_ref);
                                //objects_around.insert({ i, a_ref });
                                i++;
                            }
                        }

                        break;
                    }


                    case (8):
                    {
                        if (base_type == RE::FormType::Furniture)
                        {
                            bool this_isnt_a_furniture = false;
                            auto furniture = (RE::TESFurniture*)base_obj;
                            auto workbenchtype = furniture->workBenchData.benchType;
                            if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kNone)
                            {
                                if (furniture->HasKeywordString("ActivatorLever") || furniture->HasKeywordString("isPullChain"))
                                    this_isnt_a_furniture = true;
                            }

                            if (!this_isnt_a_furniture)
                            {
                                objects.push_back(a_ref);
                                i++;
                            }

                        }

                        break;
                    }




                    default:
                    {
                        objects.push_back(a_ref);
                        //objects_around.insert({ i, a_ref });
                        i++;

                        break;
                    }
                    }

                    //FormType::Door
                    //a_ref->extraList.

                    
                    //if (a_ref->AsReference()->IsInventoryObject() || a_ref->AsReference()->IsActor() || a_ref->AsReference()->HasContainer() || a_ref->AsReference()) //TODO: this probably doesnt include levers and other stuff like that. need to deal with this
                    //{
                    //    objects.push_back(a_ref);
                    //    objects_around.insert({ i, a_ref });
                    //    i++;
                    //}
                    
                }
                return RE::BSContainer::ForEachResult::kContinue;
            });

        std::sort(objects.begin(), objects.end(), [&](RE::TESObjectREFR* left, RE::TESObjectREFR* right) {
            //return left->GetDistance(player) > right->GetDistance(player); //switch > to < for inversed order. this is last->closest
            std::string name_left = left->GetDisplayFullName();
            std::string name_right = right->GetDisplayFullName();
            return name_left < name_right; //alphabetical order. top = A

            });

        


        //i = 0;
        for (auto object : objects)
        {
            if (!is_object_in_the_list(object))
                objects_around.insert({ std::size(objects_around), object });
            //i++;
        }

        */

        objects_around_valid = true;

        for (auto object : objects_around)
        {
            if (player_ref->GetDistance(object.second) < 10000.0f)
            {
                //std::string category = get_object_category(object.second);
                result.second += insert_into_list_and_get_info(object.second); //they are all in the list but whatever. just to get the name
                result.second += +"\n";
            }
        }

        result.first = true;

        return result;
    }

}
