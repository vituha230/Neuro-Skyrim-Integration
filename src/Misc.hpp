

#pragma once


namespace MiscThings {


    void notify_walker_to_stop_autolockpick_on_enemies();

    std::string replace_aliases(RE::TESQuest* quest, std::string displaytext, bool dont_remove = false);

    void set_shout_cooldown_broken(bool broken);
    void reset_shout_cooldown_monitor();

    bool quest_target_is_hidden(RE::TESQuest* quest, RE::BGSQuestObjective* objective, RE::TESQuestTarget* target);


    int safe_to_dodge_projectile();


    bool is_workbench(RE::TESObjectREFR* object);

    RE::ObjectRefHandle get_occupied_furniture_all_process(RE::TESObjectREFR* object);


    bool is_blocker_open(RE::TESObjectREFR* object);

    RE::TESObjectREFR* find_hermaeus_mora_face();

    bool in_madman_head();
    bool dont_use_head_node(RE::TESObjectREFR* target);


    std::vector<std::string> niav_recurse_names(RE::NiAVObject* object);
    std::vector<RE::NiPoint3> niav_recurse(RE::NiAVObject* object);

    bool is_seeker(RE::TESObjectREFR* target);

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
    bool is_actually_autoloader_door(RE::TESObjectREFR* object);

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

    RE::TESObjectREFR* look_at_something_after_quest_target_reached(RE::TESObjectREFR* target);

    bool is_wabbajack(bool right);

    RE::TESObjectREFR* GetRaycastRef(RE::NiPoint3 from, RE::NiPoint3 aimVector, float distance, RE::TESObjectREFR* target = nullptr, uint32_t filter = 0);

    void set_time_of_death(long long timestamp);
    long long get_time_of_death();

    bool is_in_third_person();


    std::pair<bool, std::string> activate_inventory_object_by_refr(RE::TESBoundObject* item);

    std::pair<bool, std::string> use_random_soulgem();

    void use_random_offensive_shout(RE::TESObjectREFR* target);

    void post_attack_advice();
    void walk_unstuck_advice();

    int get_city_sieged();
    

    bool item_is_inside_of_target_inventory(RE::TESObjectREFR* item, RE::TESObjectREFR* target);

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
        bool take_raw_description;
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


    bool inside_soulcairn_floating_tower(RE::TESObjectREFR* object = nullptr);
    bool inside_volkihar_balcony(RE::TESObjectREFR* object = nullptr);

    bool quest_leads_through_shit_volkihar_path(RE::TESQuestTarget* target, RE::TESQuest* quest);

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
    std::string insert_object_into_list_and_get_info(RE::TESObjectREFR* refr, bool no_chains = false, bool no_linked_chains = false, bool ignore_modelstate = false, bool ignore_disabled = false);
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
    bool is_object_in_the_list(int id); //variant to check if id is valid

    RE::TESObjectREFR* get_object_by_index(int id);

    std::string lowercase_string(std::string in);

    RE::TESObjectREFR* find_nearest_resurrectable_corpse();

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

    std::pair<bool, std::string> cast_spell_by_index(int id, bool fast = false, bool player_issued = false, int target_index = -1);
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

    std::string get_good_fasttravel_marker_for_quest_target(RE::TESQuestTarget* target, RE::TESQuest* quest, RE::TESObjectREFR* phantom_target = nullptr);
    std::string get_good_carriage_city_marker_for_quest_target(RE::TESQuestTarget* target, RE::TESQuest* quest, RE::TESObjectREFR* phantom_target = nullptr);


    bool dont_probe_navmesh();

    bool magnus_eye_attack_condition();
    bool object_inside_of_deadmens_dungeon_swinging_blade_area(RE::TESObjectREFR* object);

    int get_player_gold();

    bool inside_of_kilkreath_post_parkour(RE::TESObjectREFR* object);
    bool inside_of_hag_rock_pit(RE::TESObjectREFR* object);

    void disable_enable_special_navcuts(RE::TESObjectREFR* object, int activation);

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

    bool is_object_valid(RE::TESObjectREFR* a_ref, bool use_model_state = true, bool ignore_disabled = false);
    bool is_object_still_valid(RE::TESObjectREFR* test_object);
    bool is_new_object_valid(RE::TESObjectREFR* a_ref, bool ignore_modelstate = false, bool ignore_disabled = false);
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

    bool inside_meridia_flybox();

    bool is_enemy_to_actor(RE::TESObjectREFR* object, bool only_fighting = false, bool weapon_independent = false);
    bool recursive_quest_condition_check(RE::TESConditionItem* condition, RE::TESQuest* quest, RE::TESQuestTarget* target);

    bool have_any_quests();


    bool equip_ammo(bool crossbow);

    bool has_bound_weapon_equipped(bool right);

    bool can_fast_travel();


    bool projectile_flying_into_player_face();


    bool is_self_cast_spell(bool right);

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
    bool is_summon_spell(bool right);
    bool is_reanimate_spell(bool right);
    bool player_has_summon();

    float get_shout_cooldown();
    void shout_cooldown_fix(float dtime);


    bool is_digits(std::string& str, bool include_minus = false);

    void save_loader(float dtime);

    bool banned_because_vampirelord(RE::TESObjectREFR* target, int interaction);

    RE::NiPoint3 special_cave_entrance_redirect(RE::TESObjectREFR* object);

    std::string replace_aliases_all_quests(std::string displaytext);

    bool has_thrown_a_book();

    RE::TESObjectREFR* get_word_of_power(RE::TESObjectREFR* trigger_zone, bool strict = false);

    std::string fix_book_description(std::string description);

    std::string get_blocking_object_name(RE::TESObjectREFR* a_ref);
    

    std::string is_stealing(RE::TESObjectREFR* object);


    bool is_immortal(RE::Actor* actor);
    bool is_player_hidden();

    float get_quest_target_distance(RE::TESQuestTarget* target, RE::TESQuest* quest, RE::TESObjectREFR* start = nullptr, RE::TESObjectREFR* phantom_target = nullptr);

    std::string get_door_teleport(RE::TESObjectREFR* object);

    //std::string get_enchantment_info(RE::InventoryEntryData* item);
    std::string fix_enchantment_description(std::string description, RE::EnchantmentItem* spell);

    bool is_friend(RE::TESObjectREFR* object);

    bool player_has_key(RE::TESKey* key);
    bool is_door_locked(RE::TESObjectREFR* target_refr, bool ignore_player_has_key = false);

    bool is_werewolf();
    bool is_vampirelord();

    bool vampirelord_melee_mode();


    bool is_intro_quest_only();

    bool is_serving_jail();

    bool is_door_superlocked(RE::TESObjectREFR* target_refr);
    int get_picks_amount_int();


    bool is_bad_jailquest(RE::TESQuest* quest, RE::TESQuestTarget* target);

    bool player_brawling();

    bool is_inventory_object(RE::TESObjectREFR* target);

    RE::TESObjectREFR* redirect_quest_target(RE::TESQuest* quest, RE::TESObjectREFR* target, RE::TESQuestTarget* tes_quest_target);

    bool is_offensive_spell(bool right);
    bool is_offensive_spell(RE::SpellItem* spell);
    bool has_spell_equipped(bool right);


    void reveal_moths();

    void set_darkfall_bridge_after_reached();


    void reveal_chests();

    bool inside_volkihar_tower(RE::TESObjectREFR* object = nullptr);

    bool is_weapon_drawn();

    int get_object_by_refr(RE::TESObjectREFR* refr);

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


    uint64_t GetTickCount64();

    std::pair<bool, std::string> set_parthurnax_friendly_fire_choice(int id);
    void reset_parthurnax_friendly_fire();

    bool parthurnax_friendly_fire_check();


    bool make_double_confirm();
    bool get_double_confirm_choice();
    void reset_double_confirm();
    std::pair<bool, std::string> set_double_confirm_choice(int id);


    RE::NiPoint3 get_farthest_navmesh_node(RE::TESObjectREFR* object);
    

    bool same_worldspace(RE::TESObjectREFR* object1, RE::TESObjectREFR* object2);




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








//#include <REX/REX.h>
#include <shared_mutex>

namespace DebugAPI_IMPL {


    constexpr int CIRCLE_NUM_SEGMENTS = 32;

    constexpr float DRAW_LOC_MAX_DIF = 5.0f;

    constexpr float ROOT_TWO = 1.41421356f;// std::sqrt(2);//std::numbers::sqrt2_v<float>;


    inline std::uint8_t AlphaPct(const float a) // 0..100
    {
        if (a <= 0.0f) return 0;
        if (a >= 1.0f) return 100;
        return static_cast<std::uint8_t>(std::lround(a * 100.0f));
    }


    class DebugAPILine {
    public:
        DebugAPILine(RE::NiPoint3 from, RE::NiPoint3 to, const RE::NiColorA& color, float lineThickness,
            std::uint64_t destroyTickCount);

        RE::NiPoint3 From;
        RE::NiPoint3 To;
        RE::NiColorA Color;
        std::uint32_t fColor;
        std::uint8_t Alpha;
        float LineThickness;

        std::uint64_t DestroyTickCount;
    };

    class DebugAPI : public REX::Singleton<DebugAPI> {
    public:
        void Update();

        static RE::GPtr<RE::IMenu> GetHUD();

        void DrawLine2D(const RE::GPtr<RE::GFxMovieView>& movie, RE::NiPoint2 from, RE::NiPoint2 to, uint32_t color,
            float lineThickness, float alpha) const;
        void DrawLine2D(const RE::GPtr<RE::GFxMovieView>& movie, RE::NiPoint2 from, RE::NiPoint2 to,
            const RE::NiColorA& color,
            float lineThickness) const;
        void DrawLine3D(const RE::GPtr<RE::GFxMovieView>& movie, RE::NiPoint3 from, RE::NiPoint3 to, uint32_t color,
            float lineThickness, float alpha) const;
        void DrawLine3D(const RE::GPtr<RE::GFxMovieView>& movie, RE::NiPoint3 from, RE::NiPoint3 to,
            const RE::NiColorA& color,
            float lineThickness) const;
        static void ClearLines2D(const RE::GPtr<RE::GFxMovieView>& movie);

        void DrawLineForMS(const RE::NiPoint3& from, const RE::NiPoint3& to, int liftetimeMS = 10,
            const RE::NiColorA& color = { 1.0f, 0.0f, 0.0f, 1.0f }, float lineThickness = 1);
        void DrawSphere(RE::NiPoint3, float radius, int liftetimeMS = 10,
            const RE::NiColorA& color = { 1.0f, 0.0f, 0.0f, 1.0f }, float lineThickness = 1);
        void DrawCircle(RE::NiPoint3, float radius, RE::NiPoint3 eulerAngles, int liftetimeMS = 10,
            const RE::NiColorA& color = { 1.0f, 0.0f, 0.0f, 1.0f }, float lineThickness = 1);

        std::vector<DebugAPILine*> LinesToDraw;
        mutable std::shared_mutex mutex_;

        bool DEBUG_API_REGISTERED;


        static RE::NiPoint2 WorldToScreenLoc(const RE::GPtr<RE::GFxMovieView>& movie, RE::NiPoint3 worldLoc);

        void FastClampToScreen(RE::NiPoint2& point) const;

        bool IsOnScreen(RE::NiPoint2 from, RE::NiPoint2 to) const;
        bool IsOnScreen(RE::NiPoint2 point) const;

        void CacheMenuData();

        bool cachedMenuData;

        float ScreenResX;
        float ScreenResY;

    private:
        DebugAPILine* GetExistingLine(const RE::NiPoint3& from, const RE::NiPoint3& to, const RE::NiColorA& color,
            float lineThickness) const;
    };

    class DebugOverlayMenu : RE::IMenu {
    public:
        static constexpr auto MENU_PATH = "BetterThirdPersonSelection/overlay_menu";
        static constexpr auto MENU_NAME = "HUD Menu";

        DebugOverlayMenu();

        static void Register();

        static void Show();
        static void Hide();
        
        
        //static RE::stl::owner<IMenu*> Creator() { return new DebugOverlayMenu(); }
        
        //static RE::UI::Create_t Creator() { return []() -> RE::IMenu* { return new DebugOverlayMenu(); }; }

        static RE::IMenu* Creator() { return new DebugOverlayMenu(); }

        //static RE::UI::Create_t Creator() { return new DebugOverlayMenu(); }

        void AdvanceMovie(float a_interval, std::uint32_t a_currentTime) override;

    private:
        class Logger : public RE::GFxLog {
        public:
            void LogMessageVarg(LogMessageType, const char* a_fmt, const std::va_list a_argList) override {
                std::string fmt(a_fmt ? a_fmt : "");
                while (!fmt.empty() && fmt.back() == '\n') {
                    fmt.pop_back();
                }

                std::va_list args;
                va_copy(args, a_argList);
                std::vector<char> buf(static_cast<std::size_t>(std::vsnprintf(nullptr, 0, fmt.c_str(), a_argList) + 1));
                std::vsnprintf(buf.data(), buf.size(), fmt.c_str(), args);
                va_end(args);

                //logger::info("{}", buf.data());
            }
        };
    };

    namespace DrawDebug {
        namespace Colors {
            static constexpr auto RED = RE::NiColorA(1.0f, 0.0f, 0.0f, 1.0f);
            static constexpr auto GRN = RE::NiColorA(0.0f, 1.0f, 0.0f, 1.0f);
            static constexpr auto BLU = RE::NiColorA(0.0f, 0.0f, 1.0f, 1.0f);
        }

        template <int time = 100>
        void draw_line(const RE::NiPoint3& _from, const RE::NiPoint3& _to, const float size = 5.0f,
            const RE::NiColorA Color = Colors::RED) {
            DebugAPI::GetSingleton()->DrawLineForMS(_from, _to, time, Color, size);
        }


        template <RE::NiColorA Color = Colors::RED>
        void draw_sphere(const RE::NiPoint3& a_center, const float r = 5.0f, const float size = 5.0f,
            const int time = 3000) {
            DebugAPI::GetSingleton()->DrawSphere(a_center, r, time, Color, size);
        }

        
        //inline void DrawOBB(const DirectX::BoundingOrientedBox& obb) {
        //    DirectX::XMFLOAT3 c[8];
        //    obb.GetCorners(c);
        //
        //    auto P = [&](const int i) { return RE::NiPoint3{ c[i].x, c[i].y, c[i].z }; };
        //
        //    // bottom face
        //    draw_line(P(0), P(1));
        //    draw_line(P(1), P(2));
        //    draw_line(P(2), P(3));
        //    draw_line(P(3), P(0));

            // top face
        //    draw_line(P(4), P(5));
        //    draw_line(P(5), P(6));
        //    draw_line(P(6), P(7));
        //    draw_line(P(7), P(4));

            // vertical edges
        //    draw_line(P(0), P(4));
        //   draw_line(P(1), P(5));
        //   draw_line(P(2), P(6));
        //    draw_line(P(3), P(7));
        //}
       
    }

    inline RE::NiPoint3 NormalizeVector(RE::NiPoint3 p) {
        p.Unitize();
        return p;
    }

    inline RE::NiPoint3 RotateVector(const RE::NiQuaternion quatIn, const RE::NiPoint3 vecIn) {
        const float num = quatIn.x * 2.0f;
        const float num2 = quatIn.y * 2.0f;
        const float num3 = quatIn.z * 2.0f;
        const float num4 = quatIn.x * num;
        const float num5 = quatIn.y * num2;
        const float num6 = quatIn.z * num3;
        const float num7 = quatIn.x * num2;
        const float num8 = quatIn.x * num3;
        const float num9 = quatIn.y * num3;
        const float num10 = quatIn.w * num;
        const float num11 = quatIn.w * num2;
        const float num12 = quatIn.w * num3;
        RE::NiPoint3 result;
        result.x = (1.0f - (num5 + num6)) * vecIn.x + (num7 - num12) * vecIn.y + (num8 + num11) * vecIn.z;
        result.y = (num7 + num12) * vecIn.x + (1.0f - (num4 + num6)) * vecIn.y + (num9 - num10) * vecIn.z;
        result.z = (num8 - num11) * vecIn.x + (num9 + num10) * vecIn.y + (1.0f - (num4 + num5)) * vecIn.z;
        return result;
    }

    inline RE::NiPoint3 RotateVector(const RE::NiPoint3& eulerRad, const RE::NiPoint3& vecIn) {
        RE::NiMatrix3 m;
        m.SetEulerAnglesXYZ(eulerRad);
        return m * vecIn;
    }

    inline RE::NiPoint3 GetForwardVector(const RE::NiQuaternion quatIn) {
        return RotateVector(quatIn, RE::NiPoint3(0.0f, 1.0f, 0.0f));
    }

    inline RE::NiPoint3 GetForwardVector(const RE::NiPoint3 eulerIn) {
        const float pitch = eulerIn.x;
        const float yaw = eulerIn.z;

        return RE::NiPoint3(sin(yaw) * cos(pitch), cos(yaw) * cos(pitch), sin(pitch));
    }

    constexpr int FIND_COLLISION_MAX_RECURSION = 2;

    inline bool IsRoughlyEqual(const float first, const float second, const float maxDif) {
        return abs(first - second) <= maxDif;
    }

    inline RE::NiPoint3 GetCameraPos() {
        const auto playerCam = RE::PlayerCamera::GetSingleton();
        const auto pos = playerCam->pos;
        return RE::NiPoint3(pos.x, pos.y, pos.z);
    }

    inline RE::NiQuaternion GetCameraRot() {
        const auto playerCam = RE::PlayerCamera::GetSingleton();

        const auto cameraState = playerCam->currentState.get();
        if (!cameraState) return RE::NiQuaternion();

        RE::NiQuaternion niRotation;
        cameraState->GetRotation(niRotation);

        return niRotation;
    }

    inline bool IsPosBehindPlayerCamera(const RE::NiPoint3 pos) {
        const auto cameraPos = GetCameraPos();
        const auto cameraRot = GetCameraRot();

        const auto toTarget = NormalizeVector(pos - cameraPos);
        const auto cameraForward = NormalizeVector(GetForwardVector(cameraRot));

        const auto angleDif = abs((toTarget - cameraForward).Length());

        return angleDif > ROOT_TWO;
    }

    inline RE::NiPoint3 GetPointOnRotatedCircle(const RE::NiPoint3 origin, const float radius, const float i,
        const float maxI,
        const RE::NiPoint3 eulerAngles) {
        const float currAngle = i / maxI * RE::NI_TWO_PI;

        const RE::NiPoint3 targetPos(radius * cos(currAngle), radius * sin(currAngle), 0.0f);

        const auto targetPosRotated = RotateVector(eulerAngles, targetPos);

        return RE::NiPoint3(targetPosRotated.x + origin.x, targetPosRotated.y + origin.y,
            targetPosRotated.z + origin.z);
    }

    inline DebugAPILine::DebugAPILine(const RE::NiPoint3 from, const RE::NiPoint3 to, const RE::NiColorA& color,
        const float lineThickness,
        const std::uint64_t destroyTickCount) {
        From = from;
        To = to;
        Color = color;
        RE::NiColor a_rgb;
        a_rgb = color;
        fColor = a_rgb.ToInt();
        Alpha = AlphaPct(color.alpha);
        LineThickness = lineThickness;
        DestroyTickCount = destroyTickCount;
    }

    inline void DebugAPI::Update() {
        const auto hud = GetHUD();
        if (!hud || !hud->uiMovie) return;

        CacheMenuData();
        ClearLines2D(hud->uiMovie);

        std::unique_lock lock(mutex_);
        for (int i = 0; i < LinesToDraw.size(); i++) {
            const DebugAPILine* line = LinesToDraw[i];

            DrawLine3D(hud->uiMovie, line->From, line->To, line->fColor, line->LineThickness, line->Alpha);

            if (MiscThings::GetTickCount64() > line->DestroyTickCount) {
                LinesToDraw.erase(LinesToDraw.begin() + i);
                delete line;
                i--;
            }
        }
    }

    inline RE::GPtr<RE::IMenu> DebugAPI::GetHUD() {
        RE::GPtr<RE::IMenu> hud = RE::UI::GetSingleton()->GetMenu(DebugOverlayMenu::MENU_NAME);
        return hud;
    }

    inline void DebugAPI::DrawLine2D(const RE::GPtr<RE::GFxMovieView>& movie, RE::NiPoint2 from, RE::NiPoint2 to,
        const uint32_t color,
        const float lineThickness, const float alpha) const {
        if (!IsOnScreen(from, to)) return;

        FastClampToScreen(from);
        FastClampToScreen(to);

        const RE::GFxValue argsLineStyle[3]{ static_cast<double>(lineThickness), static_cast<double>(color),
                                            static_cast<double>(alpha) };
        movie->Invoke("lineStyle", nullptr, argsLineStyle, 3);

        const RE::GFxValue argsStartPos[2]{ from.x, from.y };
        movie->Invoke("moveTo", nullptr, argsStartPos, 2);

        const RE::GFxValue argsEndPos[2]{ to.x, to.y };
        movie->Invoke("lineTo", nullptr, argsEndPos, 2);

        movie->Invoke("endFill", nullptr, nullptr, 0);
    }

    inline void DebugAPI::DrawLine2D(const RE::GPtr<RE::GFxMovieView>& movie, const RE::NiPoint2 from,
        const RE::NiPoint2 to,
        const RE::NiColorA& color,
        const float lineThickness) const {
        RE::NiColor a_rgb;
        a_rgb = color;
        DrawLine2D(movie, from, to, a_rgb.ToInt(), lineThickness, AlphaPct(color.alpha));
    }

    inline void DebugAPI::DrawLine3D(const RE::GPtr<RE::GFxMovieView>& movie, const RE::NiPoint3 from,
        const RE::NiPoint3 to,
        const uint32_t color,
        const float lineThickness, const float alpha) const {
        if (IsPosBehindPlayerCamera(from) && IsPosBehindPlayerCamera(to)) return;

        const RE::NiPoint2 screenLocFrom = WorldToScreenLoc(movie, from);
        const RE::NiPoint2 screenLocTo = WorldToScreenLoc(movie, to);
        DrawLine2D(movie, screenLocFrom, screenLocTo, color, lineThickness, alpha);
    }

    inline void DebugAPI::DrawLine3D(const RE::GPtr<RE::GFxMovieView>& movie, const RE::NiPoint3 from,
        const RE::NiPoint3 to,
        const RE::NiColorA& color,
        const float lineThickness) const {
        RE::NiColor a_rgb;
        a_rgb = color;
        DrawLine3D(movie, from, to, a_rgb.ToInt(), lineThickness, AlphaPct(color.alpha));
    }

    inline void DebugAPI::ClearLines2D(const RE::GPtr<RE::GFxMovieView>& movie) {
        movie->Invoke("clear", nullptr, nullptr, 0);
    }

    inline void DebugAPI::DrawLineForMS(const RE::NiPoint3& from, const RE::NiPoint3& to, const int liftetimeMS,
        const RE::NiColorA& color,
        const float lineThickness) {
        if (DebugAPILine* oldLine = GetExistingLine(from, to, color, lineThickness)) {
            oldLine->From = from;
            oldLine->To = to;
            oldLine->DestroyTickCount = MiscThings::GetTickCount64() + liftetimeMS;
            oldLine->LineThickness = lineThickness;
            return;
        }

        const auto newLine = new DebugAPILine(from, to, color, lineThickness, MiscThings::GetTickCount64() + liftetimeMS);
        std::unique_lock lock(mutex_);
        LinesToDraw.push_back(newLine);
    }

    inline void DebugAPI::DrawSphere(const RE::NiPoint3 origin, const float radius, const int liftetimeMS,
        const RE::NiColorA& color,
        const float lineThickness) {
        DrawCircle(origin, radius, RE::NiPoint3(0.0f, 0.0f, 0.0f), liftetimeMS, color, lineThickness);
        DrawCircle(origin, radius, RE::NiPoint3(RE::NI_HALF_PI, 0.0f, 0.0f), liftetimeMS, color, lineThickness);
    }

    inline void DebugAPI::DrawCircle(const RE::NiPoint3 origin, const float radius, const RE::NiPoint3 eulerAngles,
        const int liftetimeMS,
        const RE::NiColorA& color, const float lineThickness) {
        RE::NiPoint3 lastEndPos =
            GetPointOnRotatedCircle(origin, radius, CIRCLE_NUM_SEGMENTS, static_cast<float>(CIRCLE_NUM_SEGMENTS - 1),
                eulerAngles);

        for (int i = 0; i <= CIRCLE_NUM_SEGMENTS; i++) {
            RE::NiPoint3 currEndPos =
                GetPointOnRotatedCircle(origin, radius, static_cast<float>(i),
                    static_cast<float>(CIRCLE_NUM_SEGMENTS - 1), eulerAngles);

            DrawLineForMS(lastEndPos, currEndPos, liftetimeMS, color, lineThickness);

            lastEndPos = currEndPos;
        }
    }

    inline DebugAPILine* DebugAPI::GetExistingLine(const RE::NiPoint3& from, const RE::NiPoint3& to,
        const RE::NiColorA& color,
        const float lineThickness) const {
        std::shared_lock lock(mutex_);
        for (int i = 0; i < LinesToDraw.size(); i++) {
            DebugAPILine* line = LinesToDraw[i];

            if (IsRoughlyEqual(from.x, line->From.x, DRAW_LOC_MAX_DIF) &&
                IsRoughlyEqual(from.y, line->From.y, DRAW_LOC_MAX_DIF) &&
                IsRoughlyEqual(from.z, line->From.z, DRAW_LOC_MAX_DIF) &&
                IsRoughlyEqual(to.x, line->To.x, DRAW_LOC_MAX_DIF) &&
                IsRoughlyEqual(to.y, line->To.y, DRAW_LOC_MAX_DIF) &&
                IsRoughlyEqual(to.z, line->To.z, DRAW_LOC_MAX_DIF) &&
                IsRoughlyEqual(lineThickness, line->LineThickness, DRAW_LOC_MAX_DIF) && color == line->Color) {
                return line;
            }
        }

        return nullptr;
    }

    constexpr float CLAMP_MAX_OVERSHOOT = 10000.0f;

    inline void DebugAPI::FastClampToScreen(RE::NiPoint2& point) const {
        if (point.x < 0.0) {
            const float overshootX = abs(point.x);
            if (overshootX > CLAMP_MAX_OVERSHOOT) point.x += overshootX - CLAMP_MAX_OVERSHOOT;
        }
        else if (point.x > ScreenResX) {
            const float overshootX = point.x - ScreenResX;
            if (overshootX > CLAMP_MAX_OVERSHOOT) point.x -= overshootX - CLAMP_MAX_OVERSHOOT;
        }

        if (point.y < 0.0) {
            const float overshootY = abs(point.y);
            if (overshootY > CLAMP_MAX_OVERSHOOT) point.y += overshootY - CLAMP_MAX_OVERSHOOT;
        }
        else if (point.y > ScreenResY) {
            const float overshootY = point.y - ScreenResY;
            if (overshootY > CLAMP_MAX_OVERSHOOT) point.y -= overshootY - CLAMP_MAX_OVERSHOOT;
        }
    }

    inline RE::NiPoint2
        DebugAPI::WorldToScreenLoc(const RE::GPtr<RE::GFxMovieView>& movie, const RE::NiPoint3 worldLoc) {
        static uintptr_t g_worldToCamMatrix = RELOCATION_ID(519579, 406126).address();
        static auto g_viewPort =
            reinterpret_cast<RE::NiRect<float>*>(RELOCATION_ID(519618, 406160).address());

        RE::NiPoint2 screenLocOut;

        float zVal;

        RE::NiCamera::WorldPtToScreenPt3(reinterpret_cast<float (*)[4]>(g_worldToCamMatrix), *g_viewPort, worldLoc,
            screenLocOut.x,
            screenLocOut.y, zVal, 1e-5f);
        const RE::GRectF rect = movie->GetVisibleFrameRect();

        screenLocOut.x = rect.left + (rect.right - rect.left) * screenLocOut.x;
        screenLocOut.y = 1.0f - screenLocOut.y;
        screenLocOut.y = rect.top + (rect.bottom - rect.top) * screenLocOut.y;

        return screenLocOut;
    }

    inline DebugOverlayMenu::DebugOverlayMenu() {
        const auto scaleformManager = RE::BSScaleformManager::GetSingleton();

        inputContext = Context::kNone;
        depthPriority = 127;

        menuFlags.set(RE::UI_MENU_FLAGS::kRequiresUpdate);
        menuFlags.set(RE::UI_MENU_FLAGS::kAllowSaving);
        menuFlags.set(RE::UI_MENU_FLAGS::kCustomRendering);

        scaleformManager->LoadMovieEx(this, MENU_PATH, [](RE::GFxMovieDef* a_def) -> void {
            a_def->SetState(RE::GFxState::StateType::kLog, RE::make_gptr<Logger>().get());
            });
    }

    inline void DebugOverlayMenu::Register() {
        if (const auto ui = RE::UI::GetSingleton()) {
            ui->Register(MENU_NAME, Creator);

            Show();
        }
    }

    inline void DebugOverlayMenu::Show() {
        if (const auto msgQ = RE::UIMessageQueue::GetSingleton()) {
            msgQ->AddMessage(MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
        }
    }

    inline void DebugOverlayMenu::Hide() {
        if (const auto msgQ = RE::UIMessageQueue::GetSingleton()) {
            msgQ->AddMessage(MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
        }
    }

    inline void DebugAPI::CacheMenuData() {
        if (cachedMenuData) return;

        const RE::GPtr<RE::IMenu> menu = RE::UI::GetSingleton()->GetMenu(DebugOverlayMenu::MENU_NAME);
        if (!menu || !menu->uiMovie) return;

        const RE::GRectF rect = menu->uiMovie->GetVisibleFrameRect();

        ScreenResX = abs(rect.left - rect.right);
        ScreenResY = abs(rect.top - rect.bottom);

        cachedMenuData = true;
    }

    inline bool DebugAPI::IsOnScreen(const RE::NiPoint2 from, const RE::NiPoint2 to) const {
        return IsOnScreen(from) || IsOnScreen(to);
    }

    inline bool DebugAPI::IsOnScreen(const RE::NiPoint2 point) const {
        return point.x <= ScreenResX && point.x >= 0.0 && point.y <= ScreenResY && point.y >= 0.0;
    }

    inline void DebugOverlayMenu::AdvanceMovie(const float a_interval, const std::uint32_t a_currentTime) {
        IMenu::AdvanceMovie(a_interval, a_currentTime);

        DebugAPI::GetSingleton()->Update();
    }
}








