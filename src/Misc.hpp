

#pragma once


namespace MiscThings {


    bool is_blocker_open(RE::TESObjectREFR* object);

    bool check_autolook_ban();

    std::pair<bool, std::string> get_ingame_time();

    bool object_inside_of_trevas_watch(RE::TESObjectREFR* object);


    bool inside_solstheim_superwaterfall();


    int get_ingame_hour();

    bool is_inside_of_riften_watchtower(RE::TESObjectREFR* object);
    bool is_inside_of_riften_watchtower_top(RE::TESObjectREFR* object);
    bool is_inside_of_riften_watchtower_bottom(RE::TESObjectREFR* object);




    float get_hud_stealthmeter_value(bool stealth_probe_done = false);
    float get_detection_level_value();
    bool safe_to_stealthwalk(bool stealth_probe_done = false);


    bool dont_autointerract_check(bool quest_mode, RE::TESQuest* quest = nullptr, RE::TESObjectREFR* target = nullptr);

    std::string remove_aliases(std::string displaytext);

    bool is_drowning();

    RE::TESObjectREFR* get_generic_full_redirect(RE::TESObjectREFR* target);


    int mage_quest_armillary_state();

    RE::TESObjectREFR* get_alias_ref_by_name(RE::TESQuest* quest, std::string name);

    bool is_door_closed(RE::TESObjectREFR* door);

    RE::NiPoint3 get_nearest_navmesh_node(RE::TESObjectREFR* object);
    RE::NiPoint3 EXP_get_nearest_navmesh_node(RE::TESObjectREFR* object, RE::NiPoint3 loop1, RE::NiPoint3 loop2, bool inverse_orth_loop_evasion);

    bool is_cave_autoloader_door(RE::TESObjectREFR* object);

    RE::TESObjectREFR* get_nearest_mapmarker_to_object(RE::TESObjectREFR* target);


    std::string get_timestamp_string();

    void initiate_prelast_save_load();
    void reset_prelast_save_load();
    bool is_prelast_saveloading();

    bool is_flora(RE::TESObjectREFR* object);
    bool is_tree(RE::TESObjectREFR* object);
    bool is_ore(RE::TESObjectREFR* object);

    RE::TESObjectREFR* get_generic_redirect(RE::TESObjectREFR* target, bool quest_mode, bool runaway_mode, bool already_redirecting);

    RE::TESObjectREFR* get_nearest_door_to_object(RE::TESObjectREFR* object, float range = 5000.0f);

    bool object_inside_of_windhelm_redirect_box(RE::TESObjectREFR* object);

    struct interesting_place {
        RE::TESObjectREFR* npc;
        int type;
        std::string name;
        int id;
    };

    struct interesting_places {
        std::string settlement_name;
        std::vector<interesting_place> places;
    };

    /*
    struct interesting_places {
        std::string settlement_name;
        std::vector<interesting_place> places;
        RE::TESObjectREFR* trader;
        RE::TESObjectREFR* tavern;
        RE::TESObjectREFR* alchemist;
        RE::TESObjectREFR* blacksmith;
        RE::TESObjectREFR* jarl;
        RE::TESObjectREFR* court_wizard;
        RE::TESObjectREFR* church;
    };
    */

    bool weird_close_enough_checks(RE::TESObjectREFR* target);

    bool is_aurora_borealis_in_the_sky();


    std::string get_shout_known_words_text(RE::TESShout* shout);

    bool object_inside_of_helgen_keep_cage(RE::TESObjectREFR* object);

    bool is_carriage_driver(RE::TESObjectREFR* object);

    std::string get_best_items_list();

    bool is_in_settlement();

    std::pair<bool, std::string> check_interesting_places();

    void settlement_places_processor(float dtime);
    void reset_settlement();
    std::pair<bool, std::string> visit_interesting_place_by_index(int id);


    RE::TESObjectREFR* GetRaycastRef(RE::NiPoint3 from, RE::NiPoint3 aimVector, float distance, RE::TESObjectREFR* target = nullptr);

    void set_time_of_death(long long timestamp);
    long long get_time_of_death();

    bool is_in_third_person();


    std::pair<bool, std::string> activate_inventory_object_by_refr(RE::TESBoundObject* item);

    std::pair<bool, std::string> use_random_soulgem();

    void use_random_offensive_shout(RE::TESObjectREFR* target);

    void post_attack_advice();
    void walk_unstuck_advice();

    int get_city_sieged();
    

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
        float estimate_distance;
        bool phantom_objective;
        RE::TESObjectREFR* phantom_target;
    };


    struct item_data {
        RE::TESBoundObject* object;
       // RE::InventoryEntryData* entry;
        int amount;
        //int price; //TODO: replace with something useful (maybe type of item)
    };



    struct object_data {
        RE::TESObjectREFR* object;
        std::string custom_name = "";
    };

    float is_inside_of_rectangle(RE::NiPoint2 p, RE::NiPoint2 a, RE::NiPoint2 b, RE::NiPoint2 c, RE::NiPoint2 d);
    bool player_inside_of_alftand_goodbox();

    void check_unseen_levers_if_no_levers(float range = 1400.0f, RE::TESObjectREFR* ignore_ref = nullptr);



    bool player_inside_of_ustengrev_gate_puzzle();
    bool player_inside_of_karthspire_plate_puzzle();

    RE::TESObjectREFR* get_linked_ref_children(RE::TESObjectREFR* object, int index = 0);
    RE::TESObjectREFR* get_linked_ref(RE::TESObjectREFR* object, int index = 0);

    bool is_barricade(RE::TESObjectREFR* barricade);
    void destroy_barricade(RE::TESObjectREFR* barricade);

    bool is_on_horse();

    bool is_running_allowed_in_current_cell();

    std::pair<bool, std::string> GetInventory();
    std::pair<bool, std::string> GetGold();
    std::pair<bool, std::string> GetObjectsAround(int type);
    bool is_objects_around_valid();
    std::map<int, object_data>* get_p_objects_around();
    void clear_object_list();
    RE::NiPoint3 get_looking_point_shift(RE::TESObjectREFR* object, bool pickpocket_mode);
    RE::NiPoint3 rotate_vector_by_angles(RE::NiPoint3 v, RE::NiPoint3 object_angles);
    RE::NiPoint3 rotate_vector_by_angles2(RE::NiPoint3 v, RE::NiPoint3 object_angles);

    RE::NiPoint3 get_walking_point_shift(RE::TESObjectREFR* object);

    void reset_misc();
    std::pair<bool, std::string> activate_inventory_object_by_index(int item_id, int action_id);
    std::pair<bool, std::string> drop_array_of_inventory_objects(std::vector<int> ids);


    std::pair<bool, std::string> get_current_quests();

    bool is_quest_list_valid();

    std::vector<quest>* get_p_quest_list();

    bool sees_player(RE::TESObjectREFR* actor_ref, bool stealth_probe_done = false);

    bool is_equipped(RE::TESBoundObject* object);

    bool is_known_shit_door(RE::TESObjectREFR* door);

    std::vector<RE::Actor*> get_player_attackers(bool raycastable_only = false, RE::TESObjectREFR* exclude_ref = nullptr, bool only_fighting = false, float range = 9000.0f);

    bool is_pillar_solved(RE::TESObjectREFR* pillar);

    std::pair<bool, std::string> cast_spell_by_refr(RE::SpellItem* spell, bool fast = false);
    bool player_has_spell(RE::SpellItem* spell);

    //int insert_ref_into_obj_list(RE::TESObjectREFR* refr);
    std::string insert_object_into_list_and_get_info(RE::TESObjectREFR* refr, bool no_chains = false, bool no_linked_chains = false, bool ignore_modelstate = false);
    std::string insert_quest_into_list_and_get_info(std::string quest_text);
    std::string insert_object_into_list_custom_name(std::string name, RE::TESObjectREFR* refr);

    std::string check_very_interesting_objects();

    std::string get_object_full_info(RE::TESObjectREFR* refr, bool no_linked_chains = false);

    bool raycastable(RE::TESObjectREFR* object, float range, bool only_forward = true);

    std::string get_blocking_object_name2(RE::TESObjectREFR* a_ref);

    bool quest_is_hidden(RE::TESQuest* quest, RE::BGSQuestObjective* objective = nullptr);

    bool object_inside_solitude_escape_bridge(RE::TESObjectREFR* object);
    bool object_inside_katariah_balcony(RE::TESObjectREFR* object);

    bool kataria_exists();
    bool is_object_inside_of_kataria(RE::TESObjectREFR* object);


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

    bool is_insect(RE::TESObjectREFR* object);
    bool player_escaping_jail();

    void notifications();
    void update_old_topleft_nofification(std::string new_notif);

    bool is_object_in_the_list(RE::TESObjectREFR* object);

    std::string lowercase_string(std::string in);


    bool is_fighting_dragons_allowed();
    bool is_intro();
    bool is_intro2();
    bool helgen_bride_destroyed();
    bool escaped_helgen();
    bool is_interior_cell();
    bool player_has_shouts_to_unlock();

    bool is_in_main_menu();
        
    std::pair<bool, std::string> get_locations_around();
    bool is_location_in_the_list(RE::TESObjectREFR* object);
    std::string insert_location_into_list_and_get_info(RE::TESObjectREFR* refr);

    bool is_locations_around_valid();
    std::map<int, RE::TESObjectREFR*>* get_p_locations_around();

    void nullify_object_by_id(int id);

    std::pair<bool, std::string> get_available_spells();


    bool has_digits(std::string& str);

    std::pair<bool, std::string> cast_spell_by_index(int id, bool fast = false, bool player_issued = false);
    std::pair<bool, std::string> equip_spell_by_index(int id, bool fast = false);

    int player_overencumbered_by();
    bool puzzle_door_open(RE::TESObjectREFR* a_ref);

    int get_pillar_face_name(RE::TESObjectREFR* pillar);
    std::string get_stateless_info(RE::TESObjectREFR* refr);
    std::string get_pillar_solved_text(RE::TESObjectREFR* pillar);
    //std::string get_puzzle_ring_solved_text(RE::TESObjectREFR* ring);


    std::string get_pillar_face_name(RE::TESObjectREFR* object, int code);

    int get_very_close_quest();

    bool is_dragon(RE::TESObjectREFR* refr);
    bool is_flying(RE::TESObjectREFR* refr);

    bool is_player_swimming();

    bool sneak_is_allowed();
    bool have_any_enemies_nearby(float range);

    std::string get_good_fasttravel_marker_for_quest_target(RE::TESQuestTarget* target, RE::TESQuest* quest);
    std::string get_good_carriage_city_marker_for_quest_target(RE::TESQuestTarget* target, RE::TESQuest* quest);


    bool magnus_eye_attack_condition();

    int get_player_gold();

    int trap_firing(RE::TESObjectREFR* trap);
    int two_state_activator_state(RE::TESObjectREFR* activator);
    int get_destructible_state(RE::TESObjectREFR* web);
    std::string get_potential_blocking_object(float range = 1400.0f, RE::TESObjectREFR* ignore_ref = nullptr);

    float get_weird_threshold(float original_threshold, RE::TESObjectREFR* target);

    bool object_is_interactive(RE::TESObjectREFR* object);

    bool ignore_faraway_interior(RE::TESObjectREFR* object);

    std::string get_special_text(RE::TESObjectREFR* object);


    std::string lever_interaction_advice(RE::TESObjectREFR* lever);

    std::pair<bool, std::string> unlock_shout_level(int shout_id);

    bool player_hp_more_than(float val_percent);
    bool player_hp_less_than(float val_percent);

    bool is_object_valid(RE::TESObjectREFR* a_ref, bool use_model_state = true);
    bool is_object_still_valid(RE::TESObjectREFR* test_object);
    bool is_new_object_valid(RE::TESObjectREFR* a_ref, bool ignore_modelstate = false);
    bool in_soltsheim(RE::TESObjectREFR* object = nullptr);
    bool in_skuldafn();
    bool player_inside_of_soltsteim_midgates();

    void reveal_tricky_activators_linked_to_object(RE::TESObjectREFR* object);

    RE::BGSQuestObjective* get_quest_objective_by_index(RE::TESQuest* quest, int index);

    bool player_has_item(RE::TESBoundObject* item);

    bool have_force_only_menu_open();

    bool eat_corpse_check(RE::TESObjectREFR* target);


    void prepare_for_unseen_scan(float min_dist, float max_dist);
    RE::TESObjectREFR* find_distant_unseen_reference(float min_dist, float max_dist);

    int get_nettlebane_hand_for_target(RE::TESObjectREFR* target);
    void nettlebane_advice_check(RE::TESObjectREFR* target);



    bool is_enemy_to_actor(RE::TESObjectREFR* object, bool only_fighting = false, bool weapon_independent = false);
    bool recursive_quest_condition_check(RE::TESConditionItem* condition, RE::TESQuest* quest, RE::TESQuestTarget* target);

    bool have_any_quests();

    bool can_fast_travel();

    std::string get_quest_type_text(RE::TESQuest* quest);

    bool is_quest_active(RE::TESQuest* quest);

    bool target_is_behind_labyrinthian_gate(RE::TESObjectREFR* target);

    bool has_something_equipped(bool right);

    std::string get_object_category(RE::TESForm* base_obj, RE::TESBoundObject* object = nullptr);

    void SetPosition_moveto(RE::TESObjectREFR* a_target, RE::NiPoint3 new_pos);

    RE::TESObjectREFR* get_dragon_for_dragonrend();

    bool is_inventory_item_in_the_list(RE::TESBoundObject* item);
    std::string insert_item_into_inventory_list_and_get_info(RE::TESBoundObject* item, bool compact = false);
    std::map<int, item_data>* get_p_inventory_items_list();
    int remove_item_from_inventory_list(RE::TESBoundObject* item);

    float armor_damage_difference(RE::TESBoundObject* item);

    bool player_has_fishing_rod();
    std::pair<bool, std::string> equip_spell_by_refr(RE::SpellItem* spell);
    int random_int_from_range(int min, int max);

    RE::TESForm* get_hand_contents(bool right);
    bool is_self_healing_spell(bool right);

    void book_reader(float dtime);

    void save_loader(float dtime);


    std::string replace_aliases_all_quests(std::string displaytext);

    bool has_thrown_a_book();

    RE::TESObjectREFR* get_word_of_power(RE::TESObjectREFR* trigger_zone, bool strict = false);

    std::string fix_book_description(std::string description);

    std::string get_blocking_object_name(RE::TESObjectREFR* a_ref);
    

    std::string is_stealing(RE::TESObjectREFR* object);


    bool is_immortal(RE::Actor* actor);
    bool is_player_hidden();

    float get_quest_target_distance(RE::TESQuestTarget* target, RE::TESQuest* quest, RE::TESObjectREFR* start = nullptr);

    std::string get_door_teleport(RE::TESObjectREFR* object);

    //std::string get_enchantment_info(RE::InventoryEntryData* item);
    std::string fix_enchantment_description(std::string description, RE::EnchantmentItem* spell);

    bool is_friend(RE::TESObjectREFR* object);

    bool player_has_key(RE::TESKey* key);
    bool is_door_locked(RE::TESObjectREFR* target_refr, bool ignore_player_has_key = false);

    bool is_werewolf();


    bool is_intro_quest_only();

    bool is_serving_jail();

    bool is_door_superlocked(RE::TESObjectREFR* target_refr);
    int get_picks_amount_int();


    bool is_bad_jailquest(RE::TESQuest* quest, RE::TESQuestTarget* target);

    bool player_brawling();

    bool is_inventory_object(RE::TESObjectREFR* target);

    RE::TESObjectREFR* redirect_quest_target(RE::TESQuest* quest, RE::TESObjectREFR* target);

    bool is_offensive_spell(bool right);
    bool is_offensive_spell(RE::SpellItem* spell);
    bool has_spell_equipped(bool right);


    void reveal_chests();


    bool is_loading();
    bool dont_interact_with(RE::TESObjectREFR* target);

    bool is_container_empty(RE::TESObjectREFR* object);

    bool get_peace_quest_state();

    bool is_consumable(RE::TESBoundObject* object);


    int get_restore_value(RE::TESBoundObject* object, RE::ActorValue av);


    void close_all_closable_menus();

    RE::TESObjectREFR::InventoryItemMap get_filtered_inventory();

    bool is_sleeping();

    bool is_inside_of_thief_guild_exit(RE::TESObjectREFR* object);


    bool is_settlement_advice_on_cooldown();
    void set_settlement_advice_timestamp();

    std::pair<bool, std::string> activate_array_of_inventory_objects(std::vector<int> ids);

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


        //DOESNT WORK
        inline void ForEachScript(RE::TESForm* a_form, RE::BSScript::IForEachScriptObjectFunctor* a_functor)
        {
            auto vm = InternalVM::GetSingleton();
            auto handle = GetHandle(a_form);

            vm->ForEachBoundObject(handle, a_functor);

            return;
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

        inline RE::BSScript::Variable* GetVariable2(ObjectPtr a_obj, RE::BSFixedString a_prop)
        {
            auto var = a_obj->GetVariable(a_prop);
            return var;
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





    class EventSink :
        public REX::TSingleton<EventSink>,
        public RE::BSTEventSink<RE::TESEquipEvent>
    {

    public:
        void Init()
        {
            RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESEquipEvent>(this);
        }

        virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>*) override
        {
            if (a_event)
            {
                auto player = RE::PlayerCharacter::GetSingleton();
            }
            return RE::BSEventNotifyControl::kContinue;

        }
    };


}
