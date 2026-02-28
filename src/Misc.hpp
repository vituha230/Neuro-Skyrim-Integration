

#pragma once


namespace MiscThings {


    RE::TESObjectREFR* GetRaycastRef(RE::NiPoint3 from, RE::NiPoint3 aimVector, float distance);

    void set_time_of_death(long long timestamp);
    long long get_time_of_death();



    struct quest {
        int id;
        RE::TESQuest* quest;
        RE::BGSQuestObjective* objective;
        RE::TESQuestTarget* target;
        int category; //0 - main, 1 - major sidequest, 2 - monor sidequest
        std::string name;
        std::string displaytext;
        std::string description;
        std::string target_name;
    };


    bool is_on_horse();


    std::pair<bool, std::string> GetInventory();
    std::pair<bool, std::string> GetGold();
    std::pair<bool, std::string> GetObjectsAround(int type);
    bool is_objects_around_valid();
    std::map<int, RE::TESObjectREFR*>* get_p_objects_around();
    void clear_object_list();
    RE::NiPoint3 get_looking_point_shift(RE::TESObjectREFR* object, bool pickpocket_mode);
    RE::NiPoint3 rotate_vector_by_angles(RE::NiPoint3 v, RE::NiPoint3 object_angles);

    void reset_misc();
    std::pair<bool, std::string> activate_inventory_object_by_index(int item_id, int action_id);

    std::pair<bool, std::string> get_current_quests();

    bool is_quest_list_valid();

    std::vector<quest>* get_p_quest_list();

    std::vector<RE::Actor*> get_player_attackers();

    //int insert_ref_into_obj_list(RE::TESObjectREFR* refr);
    std::string insert_into_list_and_get_info(RE::TESObjectREFR* refr);
    std::string insert_quest_into_list_and_get_info(std::string quest_text);
    std::string insert_into_list_custom_name(std::string name, RE::TESObjectREFR* refr);


    bool is_game_paused();

    void pause_game();
    void unpause_game();

    void clean_controls_from_string(std::string* string);


    float get_player_mana();
    float get_player_health();
    float get_player_stamina();


    float get_player_max_mana();
    float get_player_max_health();
    float get_player_max_stamina();

    bool player_has_levelup();



    void notifications();
    void update_old_topleft_nofification(std::string new_notif);

    bool is_object_in_the_list(RE::TESObjectREFR* object);

    std::string lowercase_string(std::string in);


    bool is_fighting_dragons_allowed();
    bool is_intro();
    bool is_intro2();


    std::pair<bool, std::string> get_locations_around();
    bool is_location_in_the_list(RE::TESObjectREFR* object);
    std::string insert_location_into_list_and_get_info(RE::TESObjectREFR* refr);

    bool is_locations_around_valid();
    std::map<int, RE::TESObjectREFR*>* get_p_locations_around();


    std::pair<bool, std::string> get_available_spells();


    bool has_digits(std::string& str);

    std::pair<bool, std::string> use_spell_by_index(int id);

    int get_pillar_face_name(RE::TESObjectREFR* pillar);
    std::string get_stateless_info(RE::TESObjectREFR* refr);
    std::string get_pillar_solved_text(RE::TESObjectREFR* pillar);
    //std::string get_puzzle_ring_solved_text(RE::TESObjectREFR* ring);

    std::string get_pillar_face_name(RE::TESObjectREFR* object, int code);


    int trap_firing(RE::TESObjectREFR* trap);
    int two_state_activator_state(RE::TESObjectREFR* activator);
    int get_destructible_state(RE::TESObjectREFR* web);
    std::string get_potential_blocking_object();


    std::pair<bool, std::string> unlock_shout_level(int shout_id);



    bool is_enemy_to_actor(RE::TESObjectREFR* object);
    bool recursive_quest_condition_check(RE::TESConditionItem* condition, RE::TESQuest* quest);



    namespace General::Script
    {
        using InternalVM = RE::BSScript::Internal::VirtualMachine;
        using ObjectPtr = RE::BSTSmartPointer<RE::BSScript::Object>;
        using ArrayPtr = RE::BSTSmartPointer<RE::BSScript::Array>;
        using CallbackPtr = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>;
        using Args = RE::BSScript::IFunctionArguments;

        inline RE::VMHandle GetHandle(RE::TESForm* a_form)
        {
            auto vm = InternalVM::GetSingleton();
            auto policy = vm->GetObjectHandlePolicy();
            return policy->GetHandleForObject(a_form->GetFormType(), a_form);
        }

        inline ObjectPtr GetObject(RE::TESForm* a_form, const char* a_class, bool a_create = false)
        {
            auto vm = InternalVM::GetSingleton();
            auto handle = GetHandle(a_form);

            ObjectPtr object = nullptr;
            bool found = vm->FindBoundObject(handle, a_class, object);
            if (!found && a_create) {
                vm->CreateObject2(a_class, object);
                vm->BindObject(object, handle, false);
            }

            return object;
        }

        template <class T>
        inline T GetVariable(ObjectPtr a_obj, RE::BSFixedString a_prop)
        {
            auto var = a_obj->GetVariable(a_prop);
            return RE::BSScript::UnpackValue<T>(var);
        }

        template <class T>
        inline T GetProperty(ObjectPtr a_obj, RE::BSFixedString a_prop)
        {
            auto var = a_obj->GetProperty(a_prop);
            return RE::BSScript::UnpackValue<T>(var);
        }

        template <class T>
        inline void SetProperty(ObjectPtr a_obj, RE::BSFixedString a_prop, T a_val)
        {
            auto var = a_obj->GetProperty(a_prop);
            if (!var)
                return;

            RE::BSScript::PackValue(var, a_val);
        }

        template <class... Args>
        inline bool DispatchMethodCall(ObjectPtr a_obj, RE::BSFixedString a_fnName, CallbackPtr a_callback, Args&&... a_args)
        {
            auto vm = InternalVM::GetSingleton();
            auto args = RE::MakeFunctionArguments(std::forward<Args>(a_args)...);
            return vm->DispatchMethodCall(a_obj, a_fnName, args, a_callback);
        }

        template <class... Args>
        inline bool DispatchStaticCall(RE::BSFixedString a_class, RE::BSFixedString a_fnName, CallbackPtr a_callback, Args&&... a_args)
        {
            auto vm = InternalVM::GetSingleton();
            auto args = RE::MakeFunctionArguments(std::forward<Args>(a_args)...);
            return vm->DispatchStaticCall(a_class, a_fnName, args, a_callback);
        }
    }



}
