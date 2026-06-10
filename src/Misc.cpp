
#include "main.hpp"
#include "Misc.hpp"
#include "InputActions.hpp"

#include <ctime>

//TODO: at least put it in groups. maybe tell what can be done with each group



namespace MiscThings {


    bool threw_a_book_out_to_read = false;

    long long gave_interesting_notification_timestamp = 0;
    long long settlement_advice_timestamp = 0;



    bool CanFastTravel(RE::Actor* a_actor, bool a_arg2)
    {
        // SkyrimSE 1.6.318.0: 0x6C5560
        using func_t = decltype(&CanFastTravel);
        static REL::Relocation<func_t> func{ REL::ID(40444) };
        return func(a_actor, a_arg2);
    }


    bool can_fast_travel()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        if (player)
            return CanFastTravel(player, true); //true means silent (no topleft message)

        return false;
    }


    bool player_has_item(RE::TESBoundObject* item)
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        if (item && item->IsBoundObject())
        {
            return player->GetItemCount(item) > 0;
        }

        return false;
    }



    bool kataria_exists()
    {
        //kataria sail
        return RE::TESObjectREFR::LookupByID(0x97ed5);
    }


    bool is_object_inside_of_kataria(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto object_worldspace = object->GetWorldspace();
            if (object_worldspace && object_worldspace->GetFormID() == 0x3c)
            {
                auto object_pos = object->GetPosition();

                if (object_pos.GetDistance({ -48751.1250, 106907.523, -13562.1621 }) < 3000.0f && object_pos.z > -14000.0f)
                    return true;
            }
        }
        return false;
    }



    bool object_inside_katariah_balcony(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto object_worldspace = object->GetWorldspace();
            auto object_pos = object->GetPosition();

            if (object_worldspace && object_worldspace->GetFormID() == 0x3c) //solitude
            {
                if (object_pos.GetDistance({ -47691.2500, 108715.141, -13583.8281 }) < 300.0f)
                {
                    return true;
                }

            }
        }

        return false;

    }



    bool object_inside_solitude_escape_bridge(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto object_worldspace = object->GetWorldspace();
            auto object_pos = object->GetPosition();

            if (object_worldspace && object_worldspace->GetFormID() == 0x37edf) //solitude
            {
                if (object_pos.GetDistance({ -60283.3633, 105597.867, -7073.46729 }) < 1800.0f)
                {
                    if (abs(-7073.46729 - object_pos.z) < 500.0f)
                        return true;
                }
                    
            }
        }

        return false;
    }


    bool magnus_eye_attack_condition()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto parent_cell = player->GetParentCell();

        if (parent_cell && parent_cell->GetFormID() == 0x1380e)
        {
            auto mage_final_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MG08");
            if (mage_final_quest)
            {
                auto stage = mage_final_quest->GetCurrentStageID();

                if (stage == 30)
                {
                    bool staff_of_magnus_in_left = false;
                    auto magnus_eye = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x25224);
                    auto redirect_force_field2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x702c923);

                    auto left_weapon = MiscThings::get_hand_contents(false);
                    if (left_weapon && left_weapon->GetFormID() == 0x35369)
                        return true; //we are in hall with eye of magnus, current quest is to defeat ancano, we have magnus staff equipped. call this function to redirect to attacking the eye when needed
                }
            }
        }

        return false;
    }


    bool is_known_shit_door(RE::TESObjectREFR* door)
    {
        if (door)
        {
            
            auto bad_ratway_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9fb92);

            if (door == bad_ratway_door)
                return true;

            auto bad_mage_quest_book_dungeon_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x73e1a);

            if (door == bad_mage_quest_book_dungeon_door)
                return true;

            auto dawnstar_sanct_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x919cf);

            if (door == dawnstar_sanct_door)
                return true;
        }

        return false;
    }



    bool sees_player(RE::TESObjectREFR* actor_ref)
    {
        if (actor_ref && actor_ref->IsActor())
        {
            auto player = RE::PlayerCharacter::GetSingleton();
            auto player_actor = (RE::Actor*)player->AsReference();

            auto actor = (RE::Actor*)actor_ref;

            auto detection1 = actor->RequestDetectionLevel(player_actor, RE::DETECTION_PRIORITY::kCritical);
            auto detection2 = actor->RequestDetectionLevel(player_actor, RE::DETECTION_PRIORITY::kHigh);
            auto detection3 = actor->RequestDetectionLevel(player_actor, RE::DETECTION_PRIORITY::kLow);
            auto detection4 = actor->RequestDetectionLevel(player_actor, RE::DETECTION_PRIORITY::kNone);
            auto detection5 = actor->RequestDetectionLevel(player_actor, RE::DETECTION_PRIORITY::kNormal);
            auto detection6 = actor->RequestDetectionLevel(player_actor, RE::DETECTION_PRIORITY::kVeryLow);
            

            return detection5 > 0;
        }

        return false;
    }



    void chain_insert_twin_object(RE::TESObjectREFR* object)
    {
        auto tg08_lever1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xfdcda);
        auto tg08_lever2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xfdcd7);

        if (object == tg08_lever1)
        {
            auto temp_result = insert_object_into_list_and_get_info(tg08_lever2, true);
        }
  
        if (object == tg08_lever2)
        {
            auto temp_result = insert_object_into_list_and_get_info(tg08_lever1, true);
        }

    }




    float get_weird_threshold(float original_threshold, RE::TESObjectREFR* target)
    {
        float result = 0.0f;

        auto meadery_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3cf65);

        if (target == meadery_door)
            return 300.0f;


        auto nightingale_sanctum_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x47cd7);

        if (target == nightingale_sanctum_marker)
            return 50.0f;


        auto nocturnal = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a2eb);

        if (target == nocturnal)
            return 200.0f;

        auto nocturnal_circle1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a3b7);
        auto nocturnal_circle2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a3b9);
        auto nocturnal_circle3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a3ba);

        if (target == nocturnal_circle1 || target == nocturnal_circle2 || target == nocturnal_circle3)
        {
            return 40.0f;
        }

        RE::TESObjectREFR* college_ward_spell_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xf9363);

        if (target == college_ward_spell_marker)
        {
            return 40.0f;
        }

        auto saartal_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1b24d);

        if (target == saartal_door)
        {
            return 300.0f;
        }

        auto saartal_orb = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x51aec);

        if (target == saartal_orb)
        {
            return 300.0f;
        }

        auto magnus_eye = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x25224);

        if (target == magnus_eye)
        {
            return 300.0f;
        }

        auto augur = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x108d7c);

        if (target == augur)
        {
            return 300.0f;
        }

        auto mzulft_crystal_pickup_fake = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7029a55);

        if (target == mzulft_crystal_pickup_fake)
        {
            return 180.0f;
        }

        /*
        RE::TESObjectREFR* mage_force_field_2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x10d99d);

        if (target == mage_force_field_2) //it should never reach it, its redirected to a marker in 1st frame. this is just to not let it count as arrived on first frame
        {
            return 180.0f;
        }
        */

        return 0.0f;
    }


    bool is_cave_autoloader_door(RE::TESObjectREFR* object)
    {

        if (object)
        {
            auto base_obj = object->GetBaseObject();

            if (base_obj)
            {
                auto base_type = base_obj->GetFormType();

                if (base_type == RE::FormType::Door)
                {
                    auto door = (RE::TESObjectDOOR*)base_obj;

                    std::string model = door->GetModel();

                    if (model == "AutoLoadMarker01.nif")
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }



    std::vector<RE::NiPoint3> test_navmesh_path(RE::TESObjectREFR* source, RE::TESObjectREFR* target)
    {
        std::vector<RE::NiPoint3> result{};

        if (source && target)
        {
            ;//for later
        }

        return result;
    }


    RE::NiPoint3 get_nearest_navmesh_node_in_cell(RE::TESObjectREFR* object, RE::TESObjectCELL* cell)
    {
        RE::NiPoint3 result = RE::NiPoint3::Zero();

        if (object)
        {
            auto object_pos = object->GetPosition();
        }

        return result;
    }


    RE::NiPoint3 get_nearest_navmesh_node_in_cell(RE::NiPoint3 object_pos, RE::TESObjectCELL* cell)
    {
        RE::NiPoint3 result = RE::NiPoint3::Zero();

        if (object_pos != RE::NiPoint3::Zero())
        {
            auto pos = object_pos;

            if (cell)
            {
                //must check adjacent cells if in overworld

                auto navmeshes_array = cell->navMeshes;

                if (navmeshes_array)
                {
                    float min_distance = FLT_MAX;

                    RE::BSTArray<RE::BSTSmartPointer<RE::NavMesh>>* navmeshes_bs_array = &navmeshes_array->navMeshes;

                    for (auto& navmesh : *navmeshes_bs_array)
                    {
                        if (navmesh)
                        {
                            auto vertices = navmesh->vertices;


                            auto triangles = navmesh->triangles;

                            for (RE::BSNavmeshTriangle triangle : triangles)
                            {
                                auto vertex_id0 = triangle.vertices[0];
                                auto vertex_id1 = triangle.vertices[1];
                                auto vertex_id2 = triangle.vertices[2];

                                auto size_vertices = std::size(vertices);

                                if (vertex_id0 < size_vertices && vertex_id1 < size_vertices && vertex_id2 < size_vertices)
                                {
                                    RE::BSNavmeshVertex vertex0 = vertices[vertex_id0];
                                    RE::BSNavmeshVertex vertex1 = vertices[vertex_id1];
                                    RE::BSNavmeshVertex vertex2 = vertices[vertex_id2];

                                    auto v0 = vertex0.location;
                                    auto v1 = vertex1.location;
                                    auto v2 = vertex2.location;

                                    RE::NiPoint3 centre = { (v0.x + v1.x + v2.x) / 3.0f, (v0.y + v1.y + v2.y) / 3.0f, (v0.z + v1.z + v2.z) / 3.0f };

                                    auto pos_dif = centre - pos;

                                    pos_dif.z = pos_dif.z * 1.5f; //too low/high are low priority

                                    float distance = pos_dif.Length();

                                    if (distance < min_distance)
                                    {
                                        min_distance = distance;
                                        result = centre;
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


    RE::NiPoint3 get_nearest_navmesh_node(RE::TESObjectREFR* object)
    {
        RE::NiPoint3 result = RE::NiPoint3::Zero();

        if (object)
        {
            auto parent_cell = object->GetParentCell();

            if (parent_cell)
            {
                float min_distance = FLT_MAX;

                bool interiorCell = parent_cell->IsInteriorCell();

                if (interiorCell)
                {
                    return get_nearest_navmesh_node_in_cell(object, parent_cell);
                }
                else 
                {
                    auto gridCells = RE::TES::GetSingleton()->gridCells;

                    if (gridCells)
                    {
                        //check all cells within 1 cell radius
                        auto parent_cell_coords_raw = parent_cell->GetCoordinates();

                        RE::NiPoint2 parent_cell_coords = { parent_cell_coords_raw->worldX, parent_cell_coords_raw->worldY };

                        for (int x = 0; x < gridCells->length; x++)
                            for (int y = 0; y < gridCells->length; y++)
                            {
                                auto adjacent_cell = gridCells->GetCell(x, y);

                                if (adjacent_cell && adjacent_cell->IsAttached())
                                {
                                    auto adjacent_cell_coords_raw = adjacent_cell->GetCoordinates();

                                    RE::NiPoint2 adjacent_cell_coords = { adjacent_cell_coords_raw->worldX, adjacent_cell_coords_raw->worldY };

                                    auto cell_distance = adjacent_cell_coords - parent_cell_coords;

                                    if (cell_distance.Length() < 4100.0f)
                                    {
                                        auto nearest_in_cell = get_nearest_navmesh_node_in_cell(object, adjacent_cell);

                                        if (nearest_in_cell != RE::NiPoint3::Zero())
                                        {
                                            auto distance = nearest_in_cell.GetDistance(object->GetPosition());

                                            if (distance < min_distance)
                                            {
                                                min_distance = distance;
                                                result = nearest_in_cell;
                                            }
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


    RE::NiPoint3 EXP_get_nearest_navmesh_node(RE::TESObjectREFR* object, RE::NiPoint3 loop1, RE::NiPoint3 loop2, bool inverse_orth_loop_evasion)
    {
        RE::NiPoint3 result = RE::NiPoint3::Zero();

        auto player = RE::PlayerCharacter::GetSingleton();

        //vector from loop2 to loop1 is evasion vector

        RE::NiPoint3 evasion_direction = { loop1.x - loop2.x, loop1.y - loop2.y, 0.0f };


        if (player)
        {
            auto parent_cell = player->GetParentCell();

            if (parent_cell)
            {
                float min_distance = FLT_MAX;

                bool interiorCell = parent_cell->IsInteriorCell();

                if (interiorCell)
                {
                    return RE::NiPoint3::Zero(); //this function must not work in interior cells
                        //get_nearest_navmesh_node_in_cell(object, parent_cell);
                }
                else
                {
                    auto gridCells = RE::TES::GetSingleton()->gridCells;

                    if (gridCells)
                    {
                        //check all cells within 1 cell radius
                        auto parent_cell_coords_raw = parent_cell->GetCoordinates();

                        RE::NiPoint2 parent_cell_coords = { parent_cell_coords_raw->worldX, parent_cell_coords_raw->worldY };

                        for (int x = 0; x < gridCells->length; x++)
                            for (int y = 0; y < gridCells->length; y++)
                            {
                                auto adjacent_cell = gridCells->GetCell(x, y);

                                if (adjacent_cell && adjacent_cell->IsAttached())
                                {
                                    auto adjacent_cell_coords_raw = adjacent_cell->GetCoordinates();

                                    RE::NiPoint2 adjacent_cell_coords = { adjacent_cell_coords_raw->worldX, adjacent_cell_coords_raw->worldY };

                                    auto cell_distance = adjacent_cell_coords - parent_cell_coords;

                                    //if (cell_distance.Length() < 4100.0f)
                                    {

                                        evasion_direction.Unitize();

                                        evasion_direction = evasion_direction * 30000.0f;

                                        

                                        auto delta1 = object->GetPosition() - player->GetPosition();

                                        RE::NiPoint3 orth1 = { -1.0f * delta1.y, delta1.x, 0.0f };
                                        RE::NiPoint3 orth2 = {delta1.y, -1.0f * delta1.x, 0.0f };

                                        orth1.Unitize();
                                        orth2.Unitize();

                                        auto player_noZ_position = player->GetPosition();
                                        player_noZ_position.z = 0.0f;

                                        auto object_noZ_position = object->GetPosition();
                                        object_noZ_position.z = 0.0f;


                                        auto test1 = player_noZ_position + evasion_direction;
                                        auto test2 = object_noZ_position + orth1;
                                        auto test3 = object_noZ_position + orth2;

                                        RE::NiPoint3 result_orth;

                                        if (test2.GetDistance(test1) < test3.GetDistance(test1))
                                        {
                                            if (inverse_orth_loop_evasion)
                                                result_orth = orth1;
                                            else
                                                result_orth = orth2;
                                        }
                                        else
                                        {
                                            if (inverse_orth_loop_evasion)
                                                result_orth = orth2;
                                            else
                                                result_orth = orth1;
                                        }

                                        evasion_direction = evasion_direction + result_orth * 20000.0f;



                                        RE::NiPoint3 evasion_far_point = player->GetPosition() + evasion_direction;

                                        auto nearest_in_cell = get_nearest_navmesh_node_in_cell(evasion_far_point, adjacent_cell);

                                        if (nearest_in_cell != RE::NiPoint3::Zero())
                                        {
                                            auto distance = nearest_in_cell.GetDistance(evasion_far_point);

                                            if (distance < min_distance)
                                            {
                                                min_distance = distance;
                                                result = nearest_in_cell;
                                            }
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


    bool in_skuldafn()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto skuldafn_worldspace = RE::TESForm::LookupByID(0x278dd);
        auto player_worldspace = player->GetWorldspace();

        if (player_worldspace == skuldafn_worldspace && skuldafn_worldspace)
            return true;

        if (!player_worldspace)
        {
            //might be interior cell in skuldafn, check locs
            auto loc = player->GetCurrentLocation();

            RE::BGSLocation* skuldafn_top = (RE::BGSLocation*)RE::TESForm::LookupByID(0x000192AB);

            if (loc)
            {
                if (loc == skuldafn_top)
                {
                    return true;
                }

                unsigned int safeguard = 0;

                auto* parent = loc->parentLoc;

                while (parent && safeguard < 100)
                {
                    if (parent == skuldafn_worldspace)
                    {
                        return true;
                    }
                    parent = parent->parentLoc;

                    safeguard++;
                }
            }

        }

        return false;
    }

    bool in_soltsheim() 
    {

        auto player = RE::PlayerCharacter::GetSingleton();

        if (player)
        {
            auto loc = player->GetCurrentLocation();

            RE::BGSLocation* soltsheim_top = (RE::BGSLocation*)RE::TESForm::LookupByID(0x04016E2A);

            if (loc)
            {
                if (loc == soltsheim_top)
                {
                    return true;
                }

                unsigned int safeguard = 0;

                auto* parent = loc->parentLoc;

                while (parent && safeguard < 100)
                {
                    if (parent == soltsheim_top)
                    {
                        return true;
                    }
                    parent = parent->parentLoc;

                    safeguard++;
                }

                if (safeguard == 100)
                {
                    //gamble checking if its 0x04

                    auto parent_formid = parent->formID;

                    return (parent_formid & 0xFF000000) == 0x04000000;
                }
            }
        }
        

        
        return false;
    }



    //OBJECT MUST BE VALID. WE CANT CHECK IT HERE BECAUSE ITS DESIGNED FOR VERY DISTANT LOCATION MARKERS THAT HAVE MODEL STATE = 0 ON CALL
    //use with caution
    RE::TESObjectREFR* get_nearest_door_to_object(RE::TESObjectREFR* object, float range)
    {
        RE::TESObjectREFR* result = nullptr;

        if (object && MiscThings::is_object_valid(object, false))
        {
            float min_distance = FLT_MAX;

            RE::TES::GetSingleton()->ForEachReferenceInRange(object, range,
                [&](RE::TESObjectREFR* a_ref) {

                    if (a_ref)
                    {
                        if (WalkerProcessor::is_door(a_ref))
                        {
                            float distance = a_ref->GetDistance(object);

                            if (distance < min_distance)
                            {
                                min_distance = distance;
                                result = a_ref;
                            }
                        }
                    }

                    return RE::BSContainer::ForEachResult::kContinue;
                });
        }
        

        return result;
    }




    std::string get_timestamp_string()
    {
        std::time_t now = std::time(nullptr);
        // ctime returns a pointer to a static string: Www Mmm dd hh:mm:ss yyyy\n
        char* dt = std::ctime(&now);

        std::string result = dt;

        if (result.length() > 0)
            result = result.substr(0, result.length() - 1); //remove endln

        return result;
    }



    bool player_has_fishing_rod()
    {
        auto inventory = get_filtered_inventory();

        std::vector<RE::TESForm*> fishing_poles{};

        fishing_poles.push_back(RE::TESObjectWEAP::LookupByID(0x05000850));
        fishing_poles.push_back(RE::TESObjectWEAP::LookupByID(0x0500084d));
        fishing_poles.push_back(RE::TESObjectWEAP::LookupByID(0x0500084e));
        fishing_poles.push_back(RE::TESObjectWEAP::LookupByID(0x0500084f));


        for (auto& [item, data] : inventory)
        {
            if (item)
            {
                auto form = item->GetFormID();
                for (auto pole : fishing_poles)
                    if (pole && form == pole->GetFormID())
                        return true;

            }

        }

        return false;
    }












    //form is settlement/cell. for worlds check if player is inside, for cells check if player is within some range
    std::map<RE::TESForm*, interesting_places> settlements{};

    
    //RE::TESObjectREFR* trader;
    //RE::TESObjectREFR* tavern;
    //RE::TESObjectREFR* alchemist;
    //RE::TESObjectREFR* blacksmith;
    //RE::TESObjectREFR* jarl;
    //RE::TESObjectREFR* court_wizard;
    //RE::TESObjectREFR* church;



    void fill_settlements()
    {
        settlements =
        {
            //winterhold
        {
            RE::TESForm::LookupByID(0x8ea2),
            {
                "Winterhold",
                (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1c18c), //trader
                (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1c18d), //tavern //FIX THIS
                nullptr, //alchemist
                nullptr, //blacksmith
                (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1c1b5), //jarl
                nullptr, //wizard
                nullptr //church
            }
        },

            //windhelm
            {
                RE::TESForm::LookupByID(0x1691d),
                {
                    "Windhelm",
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1b11c), //trader
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1b118), //tavern
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1b115), //alchemist
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1b135), //blacksmith
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1b131), //jarl
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1b132), //wizard
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x16962), //church
                }
            },

            //riften
    {
        RE::TESForm::LookupByID(0x16bb4),
        {
            "Riften",
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4e0b3), //trader
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x19dc8), //tavern
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x19dfc), //alchemist
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x19df1), //blacksmith
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x19dea), //jarl
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x19df0), //wizard
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4227b), //church
        }
    },

            //riverwood
            {
                RE::TESForm::LookupByID(0x9731),
                {
                    "Riverwood",
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x13487), //trader
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x13486), //tavern
                    nullptr, //alchemist
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x13482), //blacksmith
                    nullptr, //jarl
                    nullptr, //wizard
                    nullptr, //church
                }
            },

            //falkreath
            {
                RE::TESForm::LookupByID(0x9c80),
                {
                    "Falkreath",
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1981e), //trader
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3a198), //tavern
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3a19b), //alchemist
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3a19f), //blacksmith
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x19822), //jarl
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1b132), //wizard
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x17766), //church
                }
            },

            //markarth
            {
                RE::TESForm::LookupByID(0x16d71),
                {
                    "Markarth",
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x198ea), //trader
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x198ec), //tavern
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x198f7), //alchemist
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x19906), //blacksmith
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x19903), //jarl
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x19908), //wizard
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x16e3e), //church
                }
            },

            //solitude
            {
                RE::TESForm::LookupByID(0x37edf),
                {
                    "Solitude",
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x198c9), //trader
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x198a0), //tavern
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x198d2), //alchemist
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x198d8), //blacksmith
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x198c1), //jarl
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x198c5), //wizard
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x37f3e), //church
                }
            },

            //morthal
            {
                RE::TESForm::LookupByID(0x939d),
                {
                    "Morthal",
                    nullptr, //trader
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1aa62), //tavern
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1aa61), //alchemist
                    nullptr, //blacksmith
                    (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1aa66), //jarl
                    nullptr, //wizard
                    nullptr, //church
                }
            },

            //dawnstar
                    {
                        RE::TESForm::LookupByID(0x8eb5),
                        {
                            "Dawnstar",
                            nullptr, //trader
                            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a6c8), //tavern
                            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a6b5), //alchemist
                            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a6be), //blacksmith
                            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a6c0), //jarl
                            nullptr, //wizard
                            nullptr, //church
                        }
                    },


            //whiterun
    {
        RE::TESForm::LookupByID(0x1a26f),
        {
            "Whiterun",
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a672), //trader
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a66e), //tavern
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a66d), //alchemist
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a67c), //blacksmith
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a677), //jarl
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a67e), //wizard
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a700), //church
        }
    },

        };
    }





    bool is_aurora_borealis_in_the_sky()
    {
        auto sky = RE::Sky::GetSingleton();
        auto player = RE::PlayerCharacter::GetSingleton();

        if (sky && player)
        {
            auto hour = sky->currentGameHour;

            auto weather = sky->currentWeather;

            if (weather)
            {
                auto player_worldspace = player->GetWorldspace();
                auto tamriel_worldspace = RE::TESForm::LookupByID(0x3c);

                if (!MiscThings::is_interior_cell() && player_worldspace == tamriel_worldspace)
                {
                    std::string aurora_model = "";
                    aurora_model = weather->aurora.model;
                    if (aurora_model != "" && (hour < 3 || hour > 19.6))
                    {
                        return true;
                    }
                }

            }
        }

        return false;
    }


    RE::TESForm* get_closest_settlement()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_worldspace = player->GetWorldspace();
        auto player_cell = player->GetParentCell();
        auto tamriel_worldspace = RE::TESForm::LookupByID(0x3c);

        
        //if (std::size(settlements) < 2)

        fill_settlements();

        if (tamriel_worldspace && player_worldspace == tamriel_worldspace)
        {
            //check cells

            float min_dist = FLT_MAX;
            RE::TESForm* best_key = nullptr;

            for (auto settlement : settlements)
            {
                if (settlement.first)
                {
                    auto key_type = settlement.first->GetFormType();
                    auto key = (RE::TESObjectCELL*)settlement.first;
                    if (key_type == RE::FormType::Cell)
                    {
                        if (player_cell)
                        {
                            auto distX = abs(player_cell->GetCoordinates()->worldX - key->GetCoordinates()->worldX);
                            auto distY = abs(player_cell->GetCoordinates()->worldY - key->GetCoordinates()->worldY);
                            auto dist = distX + distY;

                            if (dist < min_dist)
                            {
                                min_dist = dist;
                                best_key = key;
                            }
                        }
                    }
                }
                
            }

            if (min_dist < 9000)
            {
                return best_key;
            }
        }
        else
        {
            auto settlement_worldspace = settlements.find(player_worldspace);

            if (settlement_worldspace != settlements.end())
                return settlement_worldspace->first;
        }

        return nullptr;
    }


    bool is_in_settlement()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_worldspace = player->GetWorldspace();
        auto player_cell = player->GetParentCell();
        auto tamriel_worldspace = RE::TESForm::LookupByID(0x3c);


        //if (std::size(settlements) < 2)

        fill_settlements();

        if (tamriel_worldspace && player_worldspace == tamriel_worldspace)
        {
            //check cells

            float min_dist = FLT_MAX;
            RE::TESForm* best_key = nullptr;

            for (auto settlement : settlements)
            {
                if (settlement.first)
                {
                    auto key_type = settlement.first->GetFormType();
                    auto key = (RE::TESObjectCELL*)settlement.first;
                    if (key_type == RE::FormType::Cell)
                    {
                        if (player_cell)
                        {
                            auto distX = abs(player_cell->GetCoordinates()->worldX - key->GetCoordinates()->worldX);
                            auto distY = abs(player_cell->GetCoordinates()->worldY - key->GetCoordinates()->worldY);
                            auto dist = distX + distY;

                            if (dist < min_dist)
                            {
                                min_dist = dist;
                                best_key = key;
                            }
                        }
                    }
                }

            }

            if (min_dist < 7000)
            {
                return true;
            }
        }
        else
        {
            auto settlement_worldspace = settlements.find(player_worldspace);

            if (settlement_worldspace != settlements.end() && settlement_worldspace->first)
                return true;

        }


        return false;
    }


    bool not_in_settlement_anymore()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_worldspace = player->GetWorldspace();
        auto player_cell = player->GetParentCell();
        auto tamriel_worldspace = RE::TESForm::LookupByID(0x3c);


        //if (std::size(settlements) < 2)

        fill_settlements();

        if (tamriel_worldspace && player_worldspace == tamriel_worldspace)
        {
            //check cells

            float min_dist = FLT_MAX;
            RE::TESForm* best_key = nullptr;

            for (auto settlement : settlements)
            {
                if (settlement.first)
                {
                    auto key_type = settlement.first->GetFormType();
                    auto key = (RE::TESObjectCELL*)settlement.first;
                    if (key_type == RE::FormType::Cell)
                    {
                        if (player_cell)
                        {
                            auto distX = abs(player_cell->GetCoordinates()->worldX - key->GetCoordinates()->worldX);
                            auto distY = abs(player_cell->GetCoordinates()->worldY - key->GetCoordinates()->worldY);
                            auto dist = distX + distY;

                            if (dist < min_dist)
                            {
                                min_dist = dist;
                                best_key = key;
                            }
                        }
                    }
                }

            }

            if (min_dist < 10000)
            {
                return false;
            }
        }
        else
        {
            auto settlement_worldspace = settlements.find(player_worldspace);

            if (settlement_worldspace != settlements.end() && settlement_worldspace->first)
                return false;

        }


        return true;
    }







    std::string get_settlement_name()
    {
        auto key = get_closest_settlement();

        if (key)
        {
            auto settlement = settlements.find(key);

            if (settlement != settlements.end() && settlement->first)
            {
                return settlement->second.settlement_name;
            }
        }

        return "";
    }


    bool is_settlement_place_valid(RE::TESObjectREFR* object, int mode)
    {
        if (object)
        {
            //if (is_object_valid(object))
            {
                if (object->IsActor())
                {
                    bool is_dead = object->IsDead();

                    bool not_trading = false;

                    if (mode == 1 || mode == 3 || mode == 4)
                    {
                        auto actor = (RE::Actor*)object;

                        not_trading = !actor->CanOfferServices();
                        bool stop_here = false;
                    }

                    return !not_trading && !is_dead;
                }
                else
                    return true;
            }
        }

        return false;
    }


    std::vector<MenuOption> get_interesting_places_options()
    {
        std::vector<MenuOption> options{};

        auto key = get_closest_settlement();

        if (key)
        {
            auto settlement = settlements.find(key);

            if (settlement != settlements.end())
            {
                if (is_settlement_place_valid(settlement->second.trader, 1)) 
                    options.push_back({ 1, "Trader" });

                if (is_settlement_place_valid(settlement->second.tavern, 2))
                    options.push_back({ 2, "Tavern" });

                if (is_settlement_place_valid(settlement->second.alchemist, 3))
                    options.push_back({ 3, "Alchemist" });

                if (is_settlement_place_valid(settlement->second.blacksmith, 4))
                    options.push_back({ 4, "Blacksmith" });

                if (is_settlement_place_valid(settlement->second.jarl, 5))
                    options.push_back({ 5, "Jarl" });

                if (is_settlement_place_valid(settlement->second.court_wizard, 6))
                    options.push_back({ 6, "Court Wizard" });

                if (is_settlement_place_valid(settlement->second.church, 7))
                    options.push_back({ 7, "Church" });

                if (std::size(options) > 0)
                    options.push_back({ -1, "[CANCEL]" });

            }
        }

        return options;
    }


    



//RE::TESObjectREFR* trader;
//RE::TESObjectREFR* tavern;
//RE::TESObjectREFR* alchemist;
//RE::TESObjectREFR* blacksmith;
//RE::TESObjectREFR* jarl;
//RE::TESObjectREFR* court_wizard;
//RE::TESObjectREFR* church;



    bool send_settlement_force = false;
    bool settlement_request_sent = false;




    std::pair<bool, std::string> check_interesting_places()
    {
        //supposed to be called near/inside of towns/cities to get a list of traders and other places to visit

        std::pair<bool, std::string> result{};

        auto temp_result = get_interesting_places_options();

        if (std::size(temp_result) < 1)
        {
            result.first = false;
            result.second = "Cannot find any available interesting places to visit";
            return result;
        }

        //and now it should give some kind of force with options from updater

        send_settlement_force = true;

        result.first = true;
        result.second = "Processing...";

        return result;
    }


    float settlement_places_processor_timer = 0.0f;

    
    void reset_settlement()
    {
        settlement_places_processor_timer = 0.0f;
        send_settlement_force = false;
        settlement_request_sent = false;
    }

    std::pair<bool, std::string> visit_interesting_place_by_index(int id)
    {
        std::pair<bool, std::string> result{};

        auto key = get_closest_settlement();

        if (key)
        {
            auto settlement = settlements.find(key);

            if (settlement != settlements.end())
            {
                switch (id)
                {
                case 1:
                    if (is_settlement_place_valid(settlement->second.trader, 0))
                    {
                        WalkerProcessor::walk_to_object_by_refr(settlement->second.trader, 1);
                        result.first = true;
                        result.second = "Started walking to trader...";
                        register_allowed_actions();
                        return result;
                    }

                    break;

                case 2:
                    if (is_settlement_place_valid(settlement->second.tavern, 0))
                    {
                        WalkerProcessor::walk_to_object_by_refr(settlement->second.tavern, 1);
                        result.first = true;
                        result.second = "Started walking to tavern...";
                        register_allowed_actions();
                        return result;
                    }
                    break;

                case 3:
                    if (is_settlement_place_valid(settlement->second.alchemist, 0))
                    {
                        WalkerProcessor::walk_to_object_by_refr(settlement->second.alchemist, 1);
                        result.first = true;
                        result.second = "Started walking to alchemist...";
                        register_allowed_actions();
                        return result;
                    }
                    break;

                case 4:
                    if (is_settlement_place_valid(settlement->second.blacksmith, 0))
                    {
                        WalkerProcessor::walk_to_object_by_refr(settlement->second.blacksmith, 1);
                        result.first = true;
                        result.second = "Started walking to blacksmith...";
                        register_allowed_actions();
                        return result;
                    }
                    break;

                case 5:
                    if (is_settlement_place_valid(settlement->second.jarl, 0))
                    {
                        WalkerProcessor::walk_to_object_by_refr(settlement->second.jarl, 1);
                        result.first = true;
                        result.second = "Started walking to jarl...";
                        register_allowed_actions();
                        return result;
                    }
                    break;

                case 6:
                    if (is_settlement_place_valid(settlement->second.court_wizard, 0))
                    {
                        WalkerProcessor::walk_to_object_by_refr(settlement->second.court_wizard, 1);
                        result.first = true;
                        result.second = "Started walking to court wizard...";
                        register_allowed_actions();
                        return result;
                    }
                    break;

                case 7:
                    if (is_settlement_place_valid(settlement->second.church, 0))
                    {
                        WalkerProcessor::walk_to_object_by_refr(settlement->second.church, 1);
                        result.first = true;
                        result.second = "Started walking to church...";
                        register_allowed_actions();
                        return result;
                    }
                    break;

                case -1:
                {
                    reset_settlement();
                    result.first = true;
                    result.second = "Cancelled";
                    register_allowed_actions();
                    return result;
                }
                    break;

                default:
                    result.first = false;
                    result.second = "Invalid choice ID";
                    return result;
                }
            }
            else
            {
                result.first = true;
                result.second = "This settlement is unavailable. Cancelling";
                register_allowed_actions();
                reset_settlement();
                return result;
            }
        }
        else
        {
            result.first = true;
            result.second = "You are not near settlement anymore. Cancelling";
            register_allowed_actions();
            reset_settlement();
            return result;
        }


        result.first = false;
        result.second = "This place is unavailable right now. Choose something else";
        register_allowed_actions();
        return result;
    }



    void settlement_places_processor(float dtime)
    {
        if (settlement_places_processor_timer > 0.5f)
        {

            
            if (is_in_settlement())
            {
                if (!get_visit_places_action_status() && get_active_force() == -1 && !is_intro() && !is_intro2() && is_something_registered())
                {
                    std::string advice = "[You are in a settlement, you can use check_interesting_places action to visit trader, alchemist, or other useful NPC if you want]";
                    register_visit_interesting();
                    
                    auto now = std::chrono::steady_clock::now().time_since_epoch().count();
                    float delta_settlement = (double)(now - settlement_advice_timestamp) / 1000000000.0;

                    if (delta_settlement > 600.0f)
                    {
                        settlement_advice_timestamp = now;
                        send_random_context(advice, false);
                    }

                }
            }
            else
            {
                if (get_visit_places_action_status() && get_active_force() == -1 && not_in_settlement_anymore())
                    unregister_visit_interesting();
            }




            if (!WalkerProcessor::is_fighting())
            {
                if (send_settlement_force)
                {
                    if (!settlement_request_sent)
                    {
                        auto temp_result = get_interesting_places_options();

                        if (std::size(temp_result) < 1)
                        {
                            reset_settlement();
                            return;
                        }

                        std::string force_text = "You are in " + get_settlement_name() + ". Choose a place to visit";

                        unregister_all_actions();

                        if (force_choice(temp_result, force_text, force_type::visit_places))
                        {
                            reset_settlement();
                        }
                    }
                }
            }
            else
                reset_settlement();
        }
        else
            settlement_places_processor_timer += dtime;
    }








    bool is_loading()
    {
        auto ui = RE::UI::GetSingleton();
        if (ui && ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME))
            return true;

        return false;
    }


    bool is_inventory_object(RE::TESObjectREFR* target)
    {
        if (target)
        {
            auto base_obj = target->GetBaseObject();
            if (base_obj && base_obj->IsInventoryObject())
                return true;
        }


        return false;
    }




    
    bool is_in_third_person()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            auto current = player->GetCurrent3D();
            auto person3 = player->Get3D1(false);

            if (current == person3)
                return true;
        }

        return false;
    }



    bool dont_interact_with(RE::TESObjectREFR* target)
    {
        auto greybeard_1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x886b4);
        auto greybeard_2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x886b5);
        auto greybeard_3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x886b6);

        auto greybeard_quest_1 = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ105");

        if (greybeard_quest_1)
        {
            auto greybeard_quest_1_stage = greybeard_quest_1->GetCurrentStageID();

            if (greybeard_quest_1_stage < 300)
            {
                if (target == greybeard_1 || target == greybeard_2 || target == greybeard_3)
                    return true;
            }
        }
        

        
        return false;
    }





    bool is_friend(RE::TESObjectREFR* object)
    {
        if (is_enemy_to_actor(object))
            return false;


        if (object && object->IsActor())
        {
            auto actor_refr = (RE::Actor*)object;

            auto player = RE::PlayerCharacter::GetSingleton();
            auto player_ref = player->AsReference();
            auto player_actor = (RE::Actor*)player_ref;


            auto reaction = actor_refr->GetFactionReaction(player_actor);

            bool is_hostile_to_player = actor_refr->IsHostileToActor(player_actor);

            if (!is_hostile_to_player && (reaction != RE::FIGHT_REACTION::kEnemy))
            {
                return true;
            }

        }

        return false;
    }


    bool player_has_key(RE::TESKey* key)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();

        if (player)
        {
            auto inventory = player->GetInventory();

            auto key_bound_object = (RE::TESBoundObject*)key;

            if (inventory.find(key_bound_object) != inventory.end())
                result = true;
        }


        return result;
    }



    bool is_bad_jailquest(RE::TESQuest* quest, RE::TESQuestTarget* target)
    {
        bool result = false;

        
        auto jail_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("JailQuest");

        if (quest && target && jail_quest && quest == jail_quest)
        {
            RE::ObjectRefHandle quest_ref_handle{};

            target->GetTargetRef(quest_ref_handle, false, quest); //no tracked - try actual target

            if (quest_ref_handle)
                if (quest_ref_handle.get())
                {
                    auto quests_target_ref = quest_ref_handle.get().get();

                    if (quests_target_ref)
                    {
                        auto base_obj = quests_target_ref->GetBaseObject();

                        if (base_obj && base_obj->GetFormType() == RE::FormType::Container)
                        {
                            //now check if its empty

                            RE::ExtraDataList* extralist = &quests_target_ref->extraList;

                            if (extralist)
                            {
                                RE::ExtraContainerChanges* extra_changes = (RE::ExtraContainerChanges*)extralist->GetByType(RE::ExtraDataType::kContainerChanges);

                                if (extra_changes)
                                {
                                    if (extra_changes->changes && extra_changes->changes->entryList)
                                    {
                                        if (extra_changes->changes->entryList->size() <= 0)
                                            return true; //its an empty jail container
                                    }
                                }
                            }
                            



                            
                        }
                    }
                }
        }
        
        return result;
    }



    int get_picks_amount_int()
    {
        int result = 0;

        const auto inv = RE::PlayerCharacter::GetSingleton()->GetInventory([](RE::TESBoundObject& a_object)
            {
                return a_object.IsLockpick() || a_object.GetFormID() == 0x3A070;;// a_object.IsObject();
            });


        std::string lockpick_name = "Lockpick";

        for (auto& [item, data] : inv)
        {
            if (data.first > 0 && item->GetName() != lockpick_name)
            {
                result = 999; //i think there are only normal lockpicks and that one infinite lockpick.. so if its not "Lockpick" then its infinite
                break;
            }
            else
                result = data.first;
        }

        return result;
    }




    bool is_door_superlocked(RE::TESObjectREFR* target_refr)
    {
        bool result = false;

        if (target_refr)
        {
            auto base_obj = target_refr->GetBaseObject();

            if (base_obj->GetFormType() == RE::FormType::Door)
            {
                auto door_refr = (RE::TESObjectDOOR*)target_refr;


                if (auto extra = target_refr->extraList.GetByType(RE::ExtraDataType::kLock); extra)
                {
                    auto extra_lock = (RE::ExtraLock*)extra;

                    if (auto extra_lock_data = extra_lock->lock; extra_lock_data)
                        if (extra_lock_data->IsLocked())
                        {
                            bool requires_key = extra_lock_data->GetLockLevel(target_refr) == RE::LOCK_LEVEL::kRequiresKey;
                            if (requires_key)
                            {
                                if (auto key = extra_lock_data->key; key)
                                {
                                    if (!MiscThings::player_has_key(key))
                                        result = true; //it requires key but player doesnt have it
                                }
                                else
                                    result = true; //key doesnt exist
                            }
                        }
                            



                }

                if (target_refr->IsLocked())
                {
                    auto lock = target_refr->GetLock();

                    if (lock)
                    {

                        bool requires_key = lock->GetLockLevel(target_refr) == RE::LOCK_LEVEL::kRequiresKey;
                        if (requires_key)
                        {
                            if (auto key = lock->key; key)
                            {
                                if (!MiscThings::player_has_key(key))
                                    result = true; //it requires key but player doesnt have it
                            }
                            else
                                result = true; //key doesnt exist
                        }
                    }
                }
            }
        }

        return result;
    }


    bool is_door_locked(RE::TESObjectREFR* target_refr)
    {
        bool result = false;

        if (target_refr)
        {
            //auto base_obj = target_refr->GetBaseObject();

            //if (base_obj->GetFormType() == RE::FormType::Door || base_obj->GetFormType() == RE::FormType::Container)
            {
                //auto door_refr = (RE::TESObjectDOOR*)target_refr;


                if (auto extra = target_refr->extraList.GetByType(RE::ExtraDataType::kLock); extra)
                {
                    auto extra_lock = (RE::ExtraLock*)extra;

                    if (auto extra_lock_data = extra_lock->lock; extra_lock_data)
                        if (extra_lock_data->IsLocked())
                            if (auto key = extra_lock_data->key; key)
                            {
                                if (!MiscThings::player_has_key(key))
                                    result = true; //locked, no key. TODO: may be inaccessible at all. we wont be able to lockpick it
                            }
                            else
                                result = true; //locked but no key



                }

                if (target_refr->IsLocked())
                {
                    auto lock = target_refr->GetLock();

                    if (lock)
                    {
                        auto key = lock->key;
                        if (!MiscThings::player_has_key(key))
                            result = true; //locked, no key. TODO: may be inaccessible at all. we wont be able to lockpick it

                    }
                    else
                        result = true; //locked but no lock.. ok..


                    //if (target_refr->GetLockLevel() != RE::LOCK_LEVEL::kRequiresKey)
                    //    return true;
                }
            }
        }
        
        return result;
    }



    bool is_player_swimming()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        if (player)
        {
            return player->actorState1.swimming;
        }

        return false;
    }



    bool is_dragon(RE::TESObjectREFR* refr)
    {
        if (refr)
        {
            if (refr->IsActor())
            {
                auto actor_refr = (RE::Actor*)refr;
                if (actor_refr->race->fullName == "Dragon Race")
                    return true;
            }
        }


        return false;
    }




    bool is_sitting(RE::TESObjectREFR* refr)
    {
        if (refr)
        {
            if (refr->IsActor())
            {
                auto actor_refr = (RE::Actor*)refr;

                auto sit_state = actor_refr->actorState1.sitSleepState;

                return sit_state == RE::SIT_SLEEP_STATE::kIsSitting;
            }
        }

        return false;

    }


    bool is_flying(RE::TESObjectREFR* refr)
    {
        if (is_sitting(refr))
            return false;

        if (refr)
        {
            if (refr->IsActor())
            {
                auto actor_refr = (RE::Actor*)refr;

                auto fly_state = actor_refr->actorState1.flyState;

                return fly_state != RE::FLY_STATE::kNone && fly_state != RE::FLY_STATE::kTakeOff;

                //return fly_state == RE::FLY_STATE::kCruising || fly_state == RE::FLY_STATE::kHovering;
            }
        }
        
        return false;
        
    }







    int get_player_gold()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        if (player)
            return player->GetGoldAmount();
        else
            return 0;
    }


    int get_very_close_quest()
    {
        auto temp_result = MiscThings::get_current_quests();
        //if (!MiscThings::is_quest_list_valid())
            

        if (!MiscThings::is_quest_list_valid())
            return -1;


        auto quest_list = MiscThings::get_p_quest_list();

        float min_distance = FLT_MAX;
        
        int best_id = -1;

        for (auto quest_entry : *quest_list)
        {
            float corrected_distance = quest_entry.estimate_distance;
            if (corrected_distance == 0.0f)
                corrected_distance = 9999999.0f;

            if (corrected_distance < min_distance)
            {
                best_id = quest_entry.id;
                min_distance = quest_entry.estimate_distance;
            }
        }

        if (min_distance < 2000.0f)
        {
            return best_id;
        }

        return -1;

    }



    float get_quest_target_distance(RE::TESQuestTarget* target, RE::TESQuest* quest, RE::TESObjectREFR* start)
    {
        float result = 0.0f;

        if (target)
        {
            auto player = RE::PlayerCharacter::GetSingleton();

            if (player)
            {

                if (!start)
                    start = player->AsReference();
            

                auto player_worldspace = start->GetWorldspace();

                //if any teleports are in the same worldspace where player is, start from that teleport


                auto player_pos = start->GetPosition();

                RE::NiPoint3 last_teleport_pos_end = RE::NiPoint3::Zero();

                RE::ObjectRefHandle quest_ref_handle{};
                target->GetTargetRef(quest_ref_handle, false, quest);
                //target->GetTrackingRef(quest_ref_handle, quest);


                RE::TESObjectREFR* quest_target_ref = nullptr;

                if (quest_ref_handle)
                    if (quest_ref_handle.get())
                        quest_target_ref = quest_ref_handle.get().get();

                RE::TESWorldSpace* quest_target_ref_worldspace = nullptr;

                if (quest_target_ref)
                    quest_target_ref_worldspace = quest_target_ref->GetWorldspace();

                bool shortcut_used = false;


                bool same_worldspace = player_worldspace == quest_target_ref_worldspace;

                if (!same_worldspace || player_worldspace == nullptr)
                {
                    for (auto teleport : target->teleportPath.teleportRefs)
                    {
                        if (teleport.ref)
                        {
                            auto teleport_pos = teleport.ref->GetPosition(); //from where it teleports

                            auto teleport_worldspace = teleport.ref->GetWorldspace();

                            if (teleport_worldspace == player_worldspace && !shortcut_used)
                            {
                                shortcut_used = true;
                                result = 0.0f;
                                result += player_pos.GetDistance(teleport_pos);
                                last_teleport_pos_end = teleport.teleportLocation;
                            }
                            else
                            {
                                if (last_teleport_pos_end == RE::NiPoint3::Zero())
                                {
                                    result += player_pos.GetDistance(teleport_pos);
                                }
                                else
                                    result += teleport_pos.GetDistance(last_teleport_pos_end);

                                last_teleport_pos_end = teleport.teleportLocation; //where it teleports
                            }

                        }

                    }

                    if (quest_target_ref)
                    {
                        auto quest_target_ref_pos = quest_target_ref->GetPosition();
                        if (last_teleport_pos_end != RE::NiPoint3::Zero())
                            result += quest_target_ref_pos.GetDistance(last_teleport_pos_end);
                        else
                            result += quest_target_ref_pos.GetDistance(player_pos);
                    }
                }
                else
                {
                    //if same worldspace - we can take direct distance
                    if (quest_target_ref)
                    {
                        auto quest_target_ref_pos = quest_target_ref->GetPosition();
                        result = quest_target_ref_pos.GetDistance(player_pos);
                    }
                }


                if (result == 0.0f)
                {
                    RE::ObjectRefHandle tracking_ref_handle{};

                    target->GetTrackingRef(tracking_ref_handle, quest);

                    RE::TESObjectREFR* tracking_ref = nullptr;

                    if (tracking_ref_handle)
                        if (tracking_ref_handle.get())
                            tracking_ref = tracking_ref_handle.get().get();

                    if (tracking_ref)
                    {
                        auto tracking_ref_pos = tracking_ref->GetPosition();

                        result += tracking_ref_pos.GetDistance(player_pos);
                    }
                }


            }
        }

        return result;
    }







    std::string get_good_carriage_city_marker_for_quest_target(RE::TESQuestTarget* target, RE::TESQuest* quest)
    {

        auto player = RE::PlayerCharacter::GetSingleton();
        RE::BSTArray<RE::ObjectRefHandle> map_markers_raw = player->currentMapMarkers;


        auto blackreach_worldspace = RE::TESForm::LookupByID(0x1ee62);
        auto player_worldspace = player->GetWorldspace();


        if (is_interior_cell() || !MapProcessor::map_is_allowed())
            return "";

        float min_distance = FLT_MAX;

        std::string sublocation_name = "";


        std::vector<RE::TESObjectREFR*> city_markers =
        {
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x177B0),
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x177EF),
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1773A),
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x17760),
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x38436),
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1C390),
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1C38A),
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x162CE),
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4D0F4)
        };

        for (auto marker : city_markers)
        {
            if (marker)
            {
                auto real_marker = marker;
                auto data = (RE::ExtraMapMarker*)real_marker->extraList.GetByType(RE::ExtraDataType::kMapMarker);
                if (real_marker && !real_marker->IsDisabled() && data && data->mapData && data->mapData->flags)
                {
                    if (real_marker == (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x162CE) && !data->mapData->flags.any(RE::MapMarkerData::Flag::kCanTravelTo))
                    {
                        return ""; //we cant even travel to whiterun its useless
                    }

                    std::string marker_name = data->mapData->locationName.GetFullName();
                    if (marker_name != "" && marker_name.find("Military Camp") == std::string::npos)
                    {

                        RE::ObjectRefHandle quest_ref_handle{};
                        target->GetTargetRef(quest_ref_handle, false, quest);

                        RE::TESObjectREFR* quest_target_ref = nullptr;

                        if (quest_ref_handle)
                            if (quest_ref_handle.get())
                                quest_target_ref = quest_ref_handle.get().get();


                        //distance from target to location
                        auto distance = get_quest_target_distance(target, quest, real_marker);

                        //auto distance = real_marker->GetDistance(target, true, true);

                        //distance from player to location
                        auto player_distance = player->GetDistance(real_marker, true, true);


                        if (player_distance > 80000.0f && distance < min_distance)
                        {
                            min_distance = distance;
                            sublocation_name = marker_name;
                        }
                    }
                }
            }
        }

        if (min_distance <= 80000.0f && sublocation_name != "")
        {
            return sublocation_name;
        }

        return "";

    }







    std::string get_good_fasttravel_marker_for_quest_target(RE::TESQuestTarget* target, RE::TESQuest* quest)
    {

        auto player = RE::PlayerCharacter::GetSingleton();
        RE::BSTArray<RE::ObjectRefHandle> map_markers = player->currentMapMarkers;


        auto blackreach_worldspace = RE::TESForm::LookupByID(0x1ee62);
        auto player_worldspace = player->GetWorldspace();


        if (is_interior_cell() || !MapProcessor::map_is_allowed())
            return "";

        float min_distance = FLT_MAX;

        std::string sublocation_name = "";

        for (auto marker : map_markers)
        {
            if (marker.get())
            {
                auto real_marker = marker.get().get();
                auto data = (RE::ExtraMapMarker*)real_marker->extraList.GetByType(RE::ExtraDataType::kMapMarker);
                if (real_marker && !real_marker->IsDisabled() && data && data->mapData && data->mapData->flags)
                {
                    if (data->mapData->flags.any(RE::MapMarkerData::Flag::kCanTravelTo))
                    {

                        std::string marker_name = data->mapData->locationName.GetFullName();
                        if (marker_name != "" && marker_name.find("Military Camp") == std::string::npos)
                        {

                            RE::ObjectRefHandle quest_ref_handle{};
                            target->GetTargetRef(quest_ref_handle, false, quest);

                            RE::TESObjectREFR* quest_target_ref = nullptr;

                            if (quest_ref_handle)
                                if (quest_ref_handle.get())
                                    quest_target_ref = quest_ref_handle.get().get();


                            auto distance = get_quest_target_distance(target, quest, real_marker);

                            //auto distance = real_marker->GetDistance(target, true, true);
                            
                            auto player_distance = player->GetDistance(real_marker, true, true);


                            if (player_distance > 25000.0f && distance < min_distance)
                            {
                                min_distance = distance;
                                sublocation_name = marker_name;
                            }
                        }
                    }
                }
            }
        }

        if (min_distance <= 25000.0f && sublocation_name != "")
        {
            return sublocation_name;
        }

        return "";

    }




    /*
    float get_quest_target_distance(RE::TESObjectREFR* target)
    {
        if (target && target->data.objectReference)
        {
            auto player = RE::PlayerCharacter::GetSingleton();
            auto player_pos = player->GetPosition();
            auto player_cell = player->GetParentCell();
            auto target_cell = target->GetParentCell();


            if (player_cell == target_cell)
                return player->GetDistance(target, true);
            else
            {
                if (target_cell)
                {
                    //target is inside
                    auto target_cell_position = target_cell;

                    bool stop_here = false;
                }
            }

        }



        return 9999999.0f;

    }
    */


    bool is_running_allowed_in_current_cell()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        if (player)
        {
            auto player_cell = player->GetParentCell();

            auto irkngthand_2_cell = RE::TESForm::LookupByID(0x7614C);

            if (player_cell == irkngthand_2_cell)
                return true;
        }

        return false;
    }


    bool target_is_behind_labyrinthian_gate(RE::TESObjectREFR* target)
    {
        if (target)
        {
            auto runaway_target = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x7003887); //runaway marker default skyrim

            if (target == runaway_target)
                return true;

            auto player = RE::PlayerCharacter::GetSingleton();
            auto player_cell = player->GetParentCell();

            if (player_cell && player_cell->GetFormID() == 0x83559)
            {
                auto target_pos = target->GetPosition();

                return target_pos.x >= -5276.0f;
            }
        }

        return false;
    }

    RE::TESObjectREFR* get_generic_redirect(RE::TESObjectREFR* target, bool quest_mode, bool runaway_mode, bool already_redirecting)
    {
        auto player = RE::PlayerCharacter::GetSingleton();


        if (player && target)
        {
            auto player_worldspace = player->GetWorldspace();
            auto tamriel_worldspace = RE::TESForm::LookupByID(0x3c);
            auto player_cell = player->GetParentCell();
            auto bleak_falls_cell = RE::TESForm::LookupByID(0x371de);
            auto player_pos = player->GetPosition();
            auto target_pos = target->GetPosition();
            auto player_ref = player->AsReference();

            if (!quest_mode)
            {
                //non-quest only

                if (player_cell == bleak_falls_cell)
                {
                    if (player_pos.z < -1118.0 && target_pos.z < -1118.0)
                    {
                        if (player_pos.y > -2875.0 && target_pos.y <= -2875.0)
                        {
                            //redirect to egg sac
                            auto spider_eggs = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3E63E);

                            if (spider_eggs)
                            {
                                return spider_eggs;
                            }
                        }
                    }
                }
            }


            //including quests
            if (player_worldspace && player_worldspace == tamriel_worldspace)
            {
                auto redirect_water = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70152d2);
                auto redirect_dock = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70152d1);

                if (player_pos.z < -14000.0f && (target_pos.z > -14000.0f || target == redirect_water) && object_inside_of_windhelm_redirect_box(player_ref) && object_inside_of_windhelm_redirect_box(target))
                {
                    if (redirect_water)
                    {
                        auto redirect_water_distance = redirect_water->GetDistance(player);

                        if (redirect_water_distance < 150.0f)
                        {
                            return redirect_dock; //so it doesnt try to swim through boat
                        }
                        else
                            return redirect_water;
                    }
                }
            }


            if (player_cell && player_cell->GetFormID() == 0x83559)
            {
                //labyrinthian 1

                auto redirect_marker1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7029a5a);
                auto redirect_marker2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7029a59);

                auto labyrinth_lever1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9187f);
                auto labyrinth_lever2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9187e);


                if (redirect_marker1 && redirect_marker2 && labyrinth_lever1 && labyrinth_lever2)
                {
                    //we need to walk to marker before gate, pull lever, walk to another marker
                    if (target == redirect_marker2 || target == redirect_marker1 || ((target == labyrinth_lever1 || target == labyrinth_lever2)))// && already_redirecting))
                        return target; //continue redirections
                    else
                    {
                        if (target_is_behind_labyrinthian_gate(target) && !target_is_behind_labyrinthian_gate(player))
                        {
                            return redirect_marker1; //initialize redirection
                        }

                        if ((!target_is_behind_labyrinthian_gate(target) || runaway_mode) && target_is_behind_labyrinthian_gate(player))
                        {
                            return redirect_marker2; //initialize redirection
                        }
                    }
                }
            }




        }

        return nullptr;
    }


    //this fully redirects to new object without any further conditions. good for aiming at some giant shit like mage forcefields using redirect xmarkers

    RE::TESObjectREFR* get_generic_full_redirect(RE::TESObjectREFR* target)
    {

        auto player = RE::PlayerCharacter::GetSingleton();

        if (player)
        {
            RE::TESObjectREFR* mage_force_field_1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xf0ea9);
            RE::TESObjectREFR* redirect_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7029a57);

            if (target == mage_force_field_1)
            {
                if (redirect_marker)
                    return redirect_marker;
            }



            RE::TESObjectREFR* mage_force_field_2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x10d99d);
            RE::TESObjectREFR* redirect_marker2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x702c923);

            if (target == mage_force_field_2)
            {
                if (redirect_marker2)
                    return redirect_marker2;
            }


            auto shoot_point_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7038459);
            //auto top_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x27eed);
            auto ramp_mechanism = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4ec0b);
            //auto shoot_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xc04b9);

            if (target == ramp_mechanism && ramp_mechanism)
            {
                if (player->GetDistance(shoot_point_marker) >= 300.0f)
                    return shoot_point_marker;
            }
        }

        



        return nullptr;
    }



    bool is_door_closed(RE::TESObjectREFR* door)
    {
        bool result = false;

        if (door)
        {
            auto base_obj = door->GetBaseObject();

            if (base_obj)
            {
                auto base_type = base_obj->GetFormType();

                if (base_type == RE::FormType::Door)
                {
                    auto door_refr = (RE::TESObjectDOOR*)door;

                    if (door_refr->GetOpenState(door) == RE::BGSOpenCloseForm::OPEN_STATE::kClosed)
                        result = true;
                }
            }

            
        }

        return result;

    }


    bool is_inside_of_thief_guild_exit(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto object_pos = object->GetPosition();
            auto object_worldspace = object->GetWorldspace();
            auto riften_worldspace = RE::TESForm::LookupByID(0x16bb4);

            if (object_worldspace && object_worldspace == riften_worldspace)
            {
                if (object_pos.z < 11030.0f)
                {
                    RE::NiPoint2 a = { 177006.469, -97190.8281 }; //-5733.7241
                    RE::NiPoint2 b = { 177006.469, -97959.4844 }; //-5633.2554
                    RE::NiPoint2 c = { 176680.766, -97959.4844 }; //-5640.6494
                    RE::NiPoint2 d = { 176680.766, -97190.8281 }; //-5724.9077

                    RE::NiPoint2 p = { object_pos.x, object_pos.y };
                    if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
                        return true;
                }
            }
        }

        return false;
    }



    RE::TESObjectREFR* redirect_quest_target(RE::TESQuest* quest, RE::TESObjectREFR* target)
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto parent_cell = player->GetParentCell();
        auto player_worldspace = player->GetWorldspace();

        auto tamriel_worldspace = RE::TESForm::LookupByID(0x3c);

        auto katariah_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DB11");

        if (quest == katariah_quest && katariah_quest)
        {
            RE::TESObjectREFR* anchor = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9939b);
            RE::TESObjectREFR* door1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x985bf);
            RE::TESObjectREFR* door2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x985b9);
            RE::TESObjectREFR* redirect_anchor = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70326b6);
            RE::TESObjectREFR* captain = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xc603c);
            RE::TESObjectREFR* emperor = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4fa39);

            auto stage = katariah_quest->GetCurrentStageID();

            //either board ship or kill the emperor
            if (stage == 15 || stage == 20)
            {
                if (target && player_worldspace == tamriel_worldspace && tamriel_worldspace && (target == redirect_anchor || target == anchor || target == door1 || target == door2 || target == captain || target == emperor))
                {
                    if (redirect_anchor)
                        return redirect_anchor;
                }
            }
            
            
            /*
            
            {
                if (target == redirect_anchor || (player_worldspace == tamriel_worldspace && tamriel_worldspace && player_pos.z < -14000.0f && player->GetDistance(anchor) < 4000.0f)) //we are in the water near ship and objective is on the ship
                {
                    if (redirect_anchor)
                        return redirect_anchor;
                }
            }
            */


            //inside of the ship
            if (parent_cell && parent_cell->GetFormID() == 0xbbb2f)
            {
                //redirect to captain if doors are locked
                RE::TESObjectREFR* door1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xbbbae);
                RE::TESObjectREFR* door2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xbbbac);

                if (door1 && door2 && (MiscThings::is_door_locked(door1) || MiscThings::is_door_locked(door2)))
                {
                    //RE::TESObjectREFR* captain = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xbbe7d); //this is actually captains chair i missclicked
                    RE::TESObjectREFR* captain = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xc603c);
                    if (captain)
                        return captain;
                }

            }
        }


        //auto dawnstar_kill_cicero_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DB07");

        /*
        if (quest == dawnstar_kill_cicero_quest && dawnstar_kill_cicero_quest)
        {
            if (parent_cell && parent_cell->GetFormID() == 0x193ee)
            {
                auto stage = dawnstar_kill_cicero_quest->GetCurrentStageID();

                RE::TESObjectREFR* blocking_gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9430e);
                RE::TESObjectREFR* redirect_chain = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x94300);
                if (stage == 40 && blocking_gate && MiscThings::two_state_activator_state(blocking_gate) == 1)
                {
                    if (redirect_chain)
                    {
                        return redirect_chain;
                    }
                }
                else
                {
                    if (target == redirect_chain)
                        WalkerProcessor::reset_walker();
                }

                RE::TESObjectREFR* blocking_gate2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xe8a7c);
                RE::TESObjectREFR* redirect_chain2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xe8a87);
                if (stage == 40 && blocking_gate2 && MiscThings::two_state_activator_state(blocking_gate2) == 1)
                {
                    
                    if (redirect_chain2)
                    {
                        return redirect_chain2;
                    }
                }
                else
                {
                    if (target == redirect_chain2)
                        WalkerProcessor::reset_walker();
                }
            }
        }
        */

        RE::TESObjectREFR* mage_force_field_2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x10d99d);

        auto mage_final_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MG08");

        if (quest == mage_final_quest && mage_final_quest)
        {
            auto stage = mage_final_quest->GetCurrentStageID();

            RE::TESObjectREFR* mage_force_field_2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x10d99d);
            RE::TESObjectREFR* original_target = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x177F9);
            RE::TESObjectREFR* redirect_force_field2_mage = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x702c923);

            if (stage == 10 && (target == original_target || target == redirect_force_field2_mage || target == mage_force_field_2) && mage_force_field_2)
            {
                return redirect_force_field2_mage;
            }
        }

        auto mage_labyrinthian_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MG07");

        if (quest == mage_labyrinthian_quest && mage_labyrinthian_quest)
        {
            if (player_worldspace && player_worldspace->GetFormID() == 0x21edb)
            {
                auto stage = mage_labyrinthian_quest->GetCurrentStageID();

                if (stage >= 50 && stage < 80)
                {
                    auto gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x86d9e);
                    auto redirect_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x2289f);
                    //(RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x86d8b); //REPLACE THIS

                    return redirect_door;

                    //
                    //if (MiscThings::two_state_activator_state(gate) == 1)
                    //    return redirect_door;
                    //else
                    //{
                    //
                    //}
                }
                else
                {
                    auto dragonpriest = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x86CF4);

                    if (target == dragonpriest)
                    {
                        auto mage1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xf4927);
                        auto mage2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xf496f);

                        if (mage1 && mage1->IsActor() && !mage1->IsDead())
                            return mage1;

                        if (mage2 && mage2->IsActor() && !mage2->IsDead())
                            return mage2;
                    }
                }
            }
            

        }

        auto mage_quest_mzulft = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MG06");

        if (quest == mage_quest_mzulft)
        {
            auto stage = mage_quest_mzulft->GetCurrentStageID();

            if (stage == 30)
            {
                if (parent_cell && parent_cell->GetFormID() == 0x1523f)
                {
                    auto redirect_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1ed31);
                    auto dead_mage = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1ed2f);

                    if (redirect_door && is_door_locked(redirect_door))
                    {
                        return dead_mage;
                    }

                    //it stops being locked when we obtain the key 

                    if (redirect_door && is_door_closed(redirect_door))
                    {
                        return redirect_door;
                    }
                }


                if (parent_cell && parent_cell->GetFormID() == 0x54b6f)
                {
                    auto redirect_door2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x57b65);
                    auto key_chest = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xb0b56);

                    if (redirect_door2 && is_door_locked(redirect_door2))
                    {
                        return key_chest;
                    }

                    if (redirect_door2 && is_door_closed(redirect_door2))
                    {
                        return redirect_door2;
                    }


                    //if door is locked - redirect to door
                    auto redirect_door3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7d419);

                    if (redirect_door3 && is_door_locked(redirect_door3))
                        return redirect_door3;

                    auto focusing_crystal_ref = get_alias_ref_by_name(mage_quest_mzulft, "MG06Crystal");
                    
                    if (focusing_crystal_ref)
                    {
                        auto focusing_crystal = focusing_crystal_ref->GetBaseObject();

                        if (focusing_crystal && !MiscThings::player_has_item(focusing_crystal))
                        {
                            //redirect to npc
                            auto paratus = get_alias_ref_by_name(mage_quest_mzulft, "MG06ParatusAlias");
                            if (paratus)
                                return paratus;

                        }
                    }

                }

            }

            if (stage >= 30 && stage < 60)
            {

                if (mage_quest_armillary_state() != 2)
                {
                    auto mzulft_crystal_pickup = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x102ed3);

                    if (target == mzulft_crystal_pickup)
                    {
                        auto mzulft_crystal_pickup_fake = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7029a55);

                        if (mzulft_crystal_pickup_fake)
                            return mzulft_crystal_pickup_fake;
                    }
                }
                else
                {
                    if (parent_cell && parent_cell->GetFormID() == 0x54b6f)
                    {
                        auto dome1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x94abc);
                        auto dome2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x94abb);
                        auto dome3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x94aba);

                        bool dome1_solved = MiscThings::is_pillar_solved(dome1);
                        bool dome2_solved = MiscThings::is_pillar_solved(dome2);
                        bool dome3_solved = MiscThings::is_pillar_solved(dome3);

                        auto button3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9bc59);
                        auto button2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9bc58);
                        auto button1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9bc57);

                        if (!dome1_solved && button1)
                            return button1;

                        if (!dome2_solved && button2)
                            return button2;

                        if (!dome3_solved && button3)
                            return button3;
                            
                    }
                }

            }

            if (stage == 60)
            {
                if (parent_cell && parent_cell->GetFormID() == 0x54b6f)
                {
                    auto redirect_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x55ef0);

                    if (redirect_door)
                        return redirect_door;
                }

                if (parent_cell && (parent_cell->GetFormID() == 0xb9da || parent_cell->GetFormID() == 0xb9fb))
                {
                    auto redirect_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7029a56);
                    if (redirect_marker)
                        return redirect_marker;
                }

            }

        }


        auto mage_quest_4 = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MG04");

        if (quest == mage_quest_4)
        {
            auto stage = mage_quest_4->GetCurrentStageID();

            if (stage == 45)
            {
                auto augur = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x108d7c);

                auto redirect_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7284d);

                if (augur && target == augur && redirect_door && is_door_locked(redirect_door))
                {
                    return redirect_door;
                }

            }
        }



        auto thief_guild_hatch = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x2d2dd);

        if (thief_guild_hatch && target == thief_guild_hatch && player_pos.z > 11030.0f)
        {
            //check if mausoleum is open, if not - redirect to button
            auto mausoleum_blocker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xc730a);

            if (mausoleum_blocker && MiscThings::two_state_activator_state(mausoleum_blocker) == 1)
            {
                auto redirect_button = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xc730a);

                if (redirect_button)
                    return redirect_button;
            }
        }


        if (is_inside_of_thief_guild_exit(player) && !is_inside_of_thief_guild_exit(target))
        {
            auto mausoleum_blocker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xc730a);

            if (mausoleum_blocker && MiscThings::two_state_activator_state(mausoleum_blocker) == 1)
            {
                auto redirect_chain = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xc7312);

                if (redirect_chain)
                    return redirect_chain;
            }
        }



        auto saartal_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MG02");

        if (quest == saartal_quest)
        {
            auto stage = saartal_quest->GetCurrentStageID();

            if (stage == 60)
            {
                auto saartall_cell1 = RE::TESForm::LookupByID(0x15220);

                auto redirect_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1b24d);

                if (redirect_door && parent_cell == saartall_cell1)
                {
                    return redirect_door;
                }

                auto saartall_cell2 = RE::TESForm::LookupByID(0x1aec3);

                if (saartall_cell2 && parent_cell == saartall_cell2)
                {
                    auto redirect_thing1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x72469);
                    auto redirect_thing2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x724fd);

                    auto test_door1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x72434);
                    auto test_door2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x725b5);

                    if (test_door1 && MiscThings::two_state_activator_state(test_door1) == 1 && redirect_thing1)
                    {
                        return redirect_thing1;
                    }
                    else
                    {
                        if (test_door2 && MiscThings::two_state_activator_state(test_door2) == 1 && redirect_thing2)
                        {
                            return redirect_thing2;
                        }
                    }
                }

            }
        }

        auto mercer_house_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("TG07");

        if (quest == mercer_house_quest)
        {
            auto stage = mercer_house_quest->GetCurrentStageID();

            if (stage == 40)
            {
                //plan is this: if target is shoot marker (from optional subquest to shoot the mechanism) or the door itself, redirect to "stand point" marker from which player is prompted to shoot

                auto shoot_point_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7038459);
                auto top_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x27eed);
                auto ramp_mechanism = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4ec0b);
                auto shoot_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xc04b9);

                
                auto vald = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x72b09);
                auto vald_bottom_gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x98e71);
                auto vald_top_gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9e578);
                auto vald_side_gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9e57a);

                if (vald && vald->IsActor() && !vald->IsDead() && vald_side_gate && vald_bottom_gate && vald_top_gate && (MiscThings::is_door_locked(vald_bottom_gate) && MiscThings::is_door_locked(vald_top_gate) && MiscThings::is_door_locked(vald_side_gate)))
                    return vald_bottom_gate; //hope he actually always aggros when we get there and opens that gate

                if (shoot_point_marker && top_door && ramp_mechanism && shoot_marker && MiscThings::two_state_activator_state(ramp_mechanism) != 0 && (target == top_door || target == shoot_marker || target == shoot_point_marker))
                    return shoot_point_marker;
                else
                    if (top_door && target == top_door && MiscThings::is_door_locked(top_door) && MiscThings::get_picks_amount_int() <= 0)
                    {
                        
                        if (vald)
                            return vald; //grab the key if door is locked and we have no lockpicks
                    }
            }
        }


        auto twilight_sanct_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("TG09");

        if (quest == twilight_sanct_quest)
        {
            auto stage = twilight_sanct_quest->GetCurrentStageID();

            if (stage == 30)
            {
                //actual target is fake - meant to be 100% redirected. original quest has no target here

                auto redirect_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7020df8);

                if (redirect_marker)
                    return redirect_marker;
            }
        }


        auto thief_guild_mansion = (RE::TESQuest*)RE::TESForm::LookupByEditorID("TG02");

        if (quest == thief_guild_mansion)
        {
            auto stage = thief_guild_mansion->GetCurrentStageID();

            if (stage <= 40)
            {
                auto bad_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xa38c6);
                auto redirect_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xa01ba);

                if (bad_door && redirect_door && target == bad_door)
                    return redirect_door;


                auto backdoor = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xbccb4);
                auto frontdoor = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1a712);

                if (backdoor && frontdoor && target == backdoor && MiscThings::get_picks_amount_int() <= 0) //no lockpicks but quest still redirects to backdoor - redirect to front door
                {
                    return frontdoor;
                }

            }
        }

        /* //this is useless - both doors are inaccessible. must break that bridge thingy
        auto thief_guild_mansion_mercer = (RE::TESQuest*)RE::TESForm::LookupByEditorID("TG07");

        if (quest == thief_guild_mansion_mercer)
        {
            auto stage = thief_guild_mansion_mercer->GetCurrentStageID();

            if (stage <= 40)
            {
                auto bad_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x27eed);
                auto redirect_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x27eec);

                if (bad_door && redirect_door && target == bad_door)
                    return redirect_door;
            }
        }
        */

        auto diplomatic_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ201");

        if (quest == diplomatic_quest)
        {
            auto the_cook = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x353be);
            auto redirect_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xd0f06);

            if (redirect_door && the_cook && target == the_cook)
                return redirect_door;

        }

        auto walk_to_riverwood_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ102");

        if (quest == walk_to_riverwood_quest)
        {
            auto riverwood_stage = walk_to_riverwood_quest->GetCurrentStageID();

            if (riverwood_stage == 17)
            {
                auto player = RE::PlayerCharacter::GetSingleton();
                auto riverwood_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x162a4);

                if (player && riverwood_marker)
                {
                    float distance = player->GetDistance(riverwood_marker);
                    if (distance > 17870.0f)
                    {
                        auto redirect_flower = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x700efdd);

                        if (redirect_flower)
                            return redirect_flower;
                    }
                }
            }
        }



        auto riften_bad_door1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x53057); //house door
        auto riften_bad_door2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x42286); //south big gate
        auto riften_good_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x42284); //north big gate

        if (riften_good_door)
        {
            if (riften_bad_door1)
            {
                if (target == riften_bad_door1)
                {
                    auto base_obj = riften_bad_door1->GetBaseObject();
                    if (base_obj && base_obj->GetFormType() == RE::FormType::Door)
                    {
                        if (riften_bad_door1->IsLocked())
                        {
                            return riften_good_door;
                        }
                    }
                }
            }

            if (riften_bad_door2)
            {
                if (target == riften_bad_door2)
                {
                    auto base_obj = riften_bad_door2->GetBaseObject();
                    if (base_obj && base_obj->GetFormType() == RE::FormType::Door)
                    {
                        if (riften_bad_door2->IsLocked())
                        {
                            return riften_good_door;
                        }
                    }
                }
            }
        }


        //cc530 madesi door
        //76f14 madesi safe

        auto madesi_ring_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("TG00");

        if (quest == madesi_ring_quest)
        {
            auto madesi_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xcc530);
            auto madesi_safe = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x76f14);

            if (madesi_door && madesi_safe)
            {
                if (target == madesi_safe)
                {
                    auto base_obj = madesi_door->GetBaseObject();

                    if (base_obj && base_obj->GetFormType() == RE::FormType::Door)
                    {
                        auto base_door = (RE::TESObjectDOOR*)base_obj;
                        if (base_door->GetOpenState(madesi_door) == RE::BGSOpenCloseForm::OPEN_STATE::kClosed)
                            return madesi_door;
                    }
                }
            }
        }



        auto golden_claw_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MS13");

        if (quest == golden_claw_quest)
        {
            auto claw_stage = golden_claw_quest->GetCurrentStageID();

            if (claw_stage == 12) //arvel is target, pathfinding will be fucked if cobweb isnt destroyed
            {
                auto cobweb = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xAA83F);

                if (cobweb && (MiscThings::get_destructible_state(cobweb) == 0 || MiscThings::get_destructible_state(cobweb) == -1)) //it exists and it isnt destroyed
                {
                    auto spider_eggs = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3E63E);

                    return spider_eggs;
                }
            }
        }


        auto parthurnax_walk_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ204");
        
        if (quest == parthurnax_walk_quest)
        {
            auto stage = parthurnax_walk_quest->GetCurrentStageID();
            auto parthurnax_target2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3c57d);

            if (stage < 95 && target == parthurnax_target2)
            {
                //parthurnax is disabled in the air - redirect to something
                auto wall = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9413e);

                return wall;
            }
        }


        

        bool check_elder_scroll_zone_dwemer_mechanism = false;

        auto elder_scroll_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ205");
        auto elder_lexicon_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DA04");

        

        bool player_in_tower_of_mzark = player && player->GetParentCell() == RE::TESForm::LookupByID(0x2d4e3);

        if (elder_scroll_quest && (quest == elder_scroll_quest))
        {
            auto elder_scroll_quest_stage = elder_scroll_quest->GetCurrentStageID();
            auto faralda = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1c1a5);
            auto gates = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x37a7d);
            auto urag = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1c1b4);
            auto door_to_urag = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x177f9);
            auto book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x32787);
            auto book2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x32786); //wrong book

            if (elder_scroll_quest_stage >= 50 && (target == urag || target == door_to_urag))
            {

                auto septimus_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DA04");
                if (septimus_quest)
                {
                    auto septimus_stage = septimus_quest->GetCurrentStageID();

                    if (septimus_stage == 5)
                        return urag;
                }


                bool gates_are_locked = false;

                if (gates && gates->IsLocked())
                    return faralda;


                if (book && !book->IsDisabled())
                    return book;
            }

            if (elder_scroll_quest_stage == 80 && player_in_tower_of_mzark)
            {
                check_elder_scroll_zone_dwemer_mechanism = true;
            }
        }


        if (elder_lexicon_quest && (quest == elder_lexicon_quest) && player_in_tower_of_mzark)
        {
            auto elder_lexicon_quest_stage = elder_lexicon_quest->GetCurrentStageID();
            if (elder_lexicon_quest_stage == 20 || elder_lexicon_quest_stage == 30)
                check_elder_scroll_zone_dwemer_mechanism = true;

        }



        if (check_elder_scroll_zone_dwemer_mechanism)
        {
            auto elder_scroll = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x2d4f1);
            auto elder_scroll_pickup = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x88268);
            auto elder_scroll_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xd3149);
            auto empty_stand = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1ba5f);
            auto full_stand = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x88269);

            auto button0 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1ba5b);
            auto button1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1ba5d);
            auto button2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1ba5c);
            auto button3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1ba5e);

            auto object_p_0 = MiscThings::General::Script::GetObject(button0, "DA04ButtonScript");
            std::string state_0 = object_p_0->currentState.c_str();
            bool button_0_opened = state_0 == "opened" || state_0 == "busy";

            auto object_p_1 = MiscThings::General::Script::GetObject(button1, "DA04ButtonScript");
            std::string state_1 = object_p_1->currentState.c_str();
            bool button_1_opened = state_1 == "opened" || state_1 == "busy";

            auto object_p_2 = MiscThings::General::Script::GetObject(button2, "DA04ButtonScript");
            std::string state_2 = object_p_2->currentState.c_str();
            bool button_2_opened = state_2 == "opened" || state_2 == "busy";

            auto object_p_3 = MiscThings::General::Script::GetObject(button3, "DA04ButtonScript");
            std::string state_3 = object_p_3->currentState.c_str();
            bool button_3_opened = state_3 == "opened" || state_3 == "busy";

            auto full_cube_item = RE::TESForm::LookupByID(0x3a3dd);
            auto elder_scroll_item = RE::TESForm::LookupByID(0x2d513);



            bool player_has_full_cube = false;
            bool player_has_elder_scroll = false;

            if (player)
            {
                player_has_full_cube = player->GetItemCount((RE::TESBoundObject*)full_cube_item) > 0;
                player_has_elder_scroll = player->GetItemCount((RE::TESBoundObject*)elder_scroll_item) > 0;
            }

            if (!player_has_full_cube || !player_has_elder_scroll)
            {
                bool prefer_button_0 = false;

                auto armillary = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xa0332);
                if (armillary)
                {
                    int armillary_pos = MiscThings::two_state_activator_state(armillary);

                    if (armillary_pos > 5)
                        prefer_button_0 = true;
                }

                if (!player_has_full_cube && !full_stand->IsDisabled())
                    return full_stand;

                if (!elder_scroll_pickup->IsDisabled())
                    return elder_scroll_pickup;


                if (button_3_opened)
                    return button3;

                if (button_2_opened)
                    return button2;

                if (button_1_opened)
                {
                    if (prefer_button_0)
                        return button0;
                    else
                        return button1;
                }
                    

                return empty_stand;

            }
        }



        auto blackreach_tower_mzark_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xef7cd);
        auto blackreach_tower_mzark_elevator = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x2872a);

        if (target == blackreach_tower_mzark_door)
            target = blackreach_tower_mzark_elevator;




        RE::TESObjectREFR* skuldafn_inner_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xa73f7);
        RE::TESObjectREFR* skuldafn_web = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xab25b);

        if (target == skuldafn_inner_door && skuldafn_inner_door && skuldafn_web)
        {
            if (MiscThings::get_destructible_state(skuldafn_web) == 0 || MiscThings::get_destructible_state(skuldafn_web) == -1) //not destroyed
            {
                return skuldafn_web;
            }
        }



        auto skuldafn_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ303");

        if (skuldafn_quest)
        {
            auto stage = skuldafn_quest->GetCurrentStageID();
        }


        auto sovngarde_portal = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xe2d48);
        auto redirect_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x700bf58);

        if (target == sovngarde_portal)
            return redirect_marker;


        //if door open - no redirect
        // if gate2 closed - redirect to barrel
        // if gate1 closed - redirect to torch (only if hadvar exists
        // if door closed 

        //ralof route:
        //if door closed - redirect to barrel

        //hadvar route:
        //if gate1 closed - redirect to torch
        //if door closed - redirect to barrel

        //


        auto helgen_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");

        if (player && quest == helgen_quest && helgen_quest)
        {
            auto helgen_escape_target = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x2636a);
            
            if (target == helgen_escape_target)
            {

                auto stage = helgen_quest->GetCurrentStageID();

                auto helgen_hadvar = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x2bfa2);
                auto helgen_ralof = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x2bf9e);

                bool ralof_mode = helgen_ralof && MiscThings::is_object_valid(helgen_ralof);


                if (stage == 250)
                {
                    if (ralof_mode)
                        return helgen_ralof;
                    else
                        return helgen_hadvar;
                }

                auto helgen_keyed_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9b0a5);
                auto helgen_redirect_barrel = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xb7e61);

                if (helgen_keyed_door)
                {
                    if (ralof_mode)
                    {
                        //ralof route
                        if (is_door_locked(helgen_keyed_door))
                        {
                            if (helgen_redirect_barrel)
                                return helgen_redirect_barrel;
                        }
                    }
                    else
                    {
                        //hadvar route
                        auto helgen_gate1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x90a05);
                        auto helgen_gate2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xb35af);
                        auto helgen_redirect_torch = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xb6bca);

                        if (two_state_activator_state(helgen_gate1) == 1)
                        {
                            if (helgen_redirect_torch)
                                return helgen_redirect_torch;
                        }
                        else
                        {
                            if (two_state_activator_state(helgen_gate2) == 1)
                            {
                                if (helgen_keyed_door)
                                    return helgen_keyed_door;
                            }
                            else
                            {
                                if (is_door_locked(helgen_keyed_door))
                                {
                                    if (helgen_redirect_barrel)
                                        return helgen_redirect_barrel;
                                }
                            }
                        }
                    }
                }
            }
        }


        auto bleak_falls_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x38dfb);

        if (target == bleak_falls_door)
        {
            auto bleak_falls_gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x2ad21);
            if (bleak_falls_gate && two_state_activator_state(bleak_falls_gate) == 1)
            {
                if (player->GetDistance(bleak_falls_gate) < 700.0f)
                {
                    //gate closed, we are close to gate, target is temple door. it will point us into the wall, redirect to ore vein behind the gate
                    auto redirect_candle = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x395f3);
                    if (redirect_candle)
                        return redirect_candle;
                }
            }
        }


        auto elder_tree_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("T03");
        auto the_tree = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x79af9);


        if (quest == elder_tree_quest)
        {
            auto stage = elder_tree_quest->GetCurrentStageID();

            auto root1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x5b611);
            auto root2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7873e);
            auto root3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x15cb2);
            auto root4 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x41511);

            auto root5 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x15cb7); //this is redirect-only root


            if (root1 && root2 && root3 && root4 && stage == 40)
            {
                if (MiscThings::two_state_activator_state(root1) == 1)
                    return root2;

                if (MiscThings::two_state_activator_state(root2) == 1)
                    return root3;

                if (MiscThings::two_state_activator_state(root3) == 1)
                    return root4;

                if (MiscThings::two_state_activator_state(root4) == 1)
                    return root5;

            }
        }



        //replaced with generic completely
        /*
        auto get_to_soltstheim_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DLC2MQ01");

        if (quest == get_to_soltstheim_quest)
        {
            auto stage = get_to_soltstheim_quest->GetCurrentStageID();

            auto soltsheim_boat_driver = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x040182AF);

            if (stage == 10 && target == soltsheim_boat_driver)
            {
                auto player_worldspace = player->GetWorldspace();
                auto tamriel_worldspace = RE::TESForm::LookupByID(0x3c);
                auto player_pos = player->GetPosition();

                if (player_worldspace && player_worldspace == tamriel_worldspace)
                {
                    if (player_pos.z < -14000.0f)
                    {
                        auto redirect_water = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70152d2);
                        auto redirect_dock = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70152d1);

                        if (redirect_water)
                        {
                            auto redirect_water_distance = redirect_water->GetDistance(player);

                            if (redirect_water_distance < 150.0f)
                            {
                                return redirect_dock; //so it doesnt try to swim through boat
                            }
                            else
                                return redirect_water;
                        }
                    }
                }
            }
        }
        */

        return target;
    }





    bool quest_is_hidden(RE::TESQuest* quest, RE::BGSQuestObjective* objective)
    {
        auto barrow_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ103");
        auto golden_claw_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MS13");

        if (golden_claw_quest)
        {
            auto claw_stage = golden_claw_quest->GetCurrentStageID();

            if (claw_stage == 35 && quest == barrow_quest)
            {
                return true;
            }
        }



        auto diplomatic_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ201");

        if (diplomatic_quest)
        {
            auto diplomatic_stage = diplomatic_quest->GetCurrentStageID();

            if (diplomatic_stage >= 100 && diplomatic_stage < 230 && quest != diplomatic_quest)
            {
                return true;
            }
        }



        auto find_esbern_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ202");
        auto madesi_ring_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("TG00");

        if (find_esbern_quest && madesi_ring_quest)
        {
            auto stage1 = find_esbern_quest->GetCurrentStageID();
            auto stage2 = madesi_ring_quest->GetCurrentStageID();


            if (stage1 < 20 && stage2 >= 10 && stage2 < 35 && quest == find_esbern_quest)
            {
                return true;
            }
        }



        

        auto capture_dragon_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ301");
        auto peace_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ302");


        if (capture_dragon_quest && peace_quest)
        {
            auto capture_dragon_quest_stage = capture_dragon_quest->GetCurrentStageID();
            auto peace_quest_stage = peace_quest->GetCurrentStageID();

            bool peace_quest_present = (peace_quest->data.flags.all(RE::QuestFlag::kDisplayedInHUD) || peace_quest->data.flags.all(RE::QuestFlag::kEnabled)) && !peace_quest->data.flags.all(RE::QuestFlag::kCompleted);
            
            if (peace_quest_present && quest == capture_dragon_quest)
            {
                return true; //hide dragon quest until we have the peace
            }


            auto player = RE::PlayerCharacter::GetSingleton();

            if (player)
            {
                auto player_cell = player->GetParentCell();
                auto hrothgar_interior_cell = RE::TESForm::LookupByID(0x87764);

                if (player_cell == hrothgar_interior_cell)
                {
                    if (peace_quest_stage >= 40 && peace_quest_stage < 320)
                    {
                        if (quest != peace_quest && quest != capture_dragon_quest)
                        {
                            return true;
                        }
                    }
                }

            }

        }


        auto skuldafn_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ303");

        if (skuldafn_quest)
        {
            auto stage = skuldafn_quest->GetCurrentStageID();

            if (stage >= 100 && stage < 300)
            {
                if (quest != skuldafn_quest)
                    return true;
            }
        }

        auto sovngarde_quest1 = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ304");
        auto sovngarde_quest2 = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ305");
        auto sovngarde_quest3 = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ306");

        if (quest == sovngarde_quest3)
            return false;

        if (sovngarde_quest1)
        {
            auto stage = sovngarde_quest1->GetCurrentStageID();

            if (stage > 5 && stage < 200)
            {
                if (quest != sovngarde_quest1)
                    return true;
            }
        }


        if (sovngarde_quest2)
        {
            auto stage = sovngarde_quest2->GetCurrentStageID();

            if (stage > 0 && stage < 300)
            {
                if (quest != sovngarde_quest2)
                    return true;
            }
        }

        /*
        if (sovngarde_quest3)
        {
            auto stage = sovngarde_quest3->GetCurrentStageID();

            if (stage > 0 && stage < 60)
            {
                if (quest != sovngarde_quest3)
                    return true;
            }
        }
        */


        auto drag_orc_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DB08");

        if (quest == drag_orc_quest && objective && objective->index == 15)
            return true; //dragging corpses to hidden places is wip


        return false;
    }



    bool get_peace_quest_state()
    {
        auto peace_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ302");
        auto player = RE::PlayerCharacter::GetSingleton();

        if (peace_quest && player)
        {
            auto stage = peace_quest->GetCurrentStageID();
            auto player_cell = player->GetParentCell();
            auto hrothgar_interior_cell = RE::TESForm::LookupByID(0x87764);
            auto player_sit_state = player->GetSitSleepState();


            if (player_cell == hrothgar_interior_cell && hrothgar_interior_cell && stage == 55 && player_sit_state == RE::SIT_SLEEP_STATE::kIsSitting)
            {
                return true;
            }
        }

        return false;
    }




    int get_restore_value(RE::TESBoundObject* object,  RE::ActorValue av)
    {
        if (object)
        {
            if (object->GetFormType() == RE::FormType::AlchemyItem)
            {
                auto alch_item = (RE::AlchemyItem*)object;

                if (!alch_item->IsPoison())
                {
                    auto magic_item = object->As<RE::MagicItem>();

                    if (magic_item)
                    {
                        for (auto effect : magic_item->effects)
                        {
                            if (effect->baseEffect->data.primaryAV == av)
                            {
                                return effect->GetMagnitude();
                            }
                        }
                    }
                }
            }
        }
        return 0;
    }



    int mage_quest_armillary_state()
    {
        auto armillary = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x94abd);

        if (armillary)
        {
            auto object_p = General::Script::GetObject(armillary, "MG06ArmillaryScript");

            if (object_p)
            {
                //RE::BSFixedString prop_name = "::ReadyForSpells_var";
                //bool some_shit = General::Script::GetVariable<int>(object_p, prop_name);


                std::string state = "";
                state = object_p->currentState;

                //4 is correct. frost gives +1, fire gives -1

                if (state == "position01" || state == "position02" || state == "position03")
                    return 0;//needs cold

                if (state == "position05" || state == "Position06")
                    return 1;//needs hot

                if (state == "Position04")
                    return 2; //correct temperature

            }
        }

        return -1;
    }



    std::string lever_interaction_advice(RE::TESObjectREFR* lever)
    {
        if (!lever)
            return "";

        auto object_p = General::Script::GetObject(lever, "defaultPillarPuzzleLever");

        if (!object_p)
        {
            object_p = General::Script::GetObject(lever, "dunSkuldafnPuzzleControlSCRIPT");
            if (!object_p)
            {
                object_p = General::Script::GetObject(lever, "defaultPillarPuzzleLeverNoFurn");
                if (!object_p)
                    return "";
                else
                {
                    goto normal_pillar;
                }
            }
            else
            {
                RE::BSFixedString prop_name = "puzzleSolved";
                bool puzzle_solved = General::Script::GetProperty<bool>(object_p, prop_name);

                RE::BSFixedString prop_name2 = "altSolution";
                bool alt_solution = General::Script::GetProperty<bool>(object_p, prop_name2);

                RE::BSFixedString prop_name3 = "numPillarsSolved";
                int pillars_solved = General::Script::GetProperty<int>(object_p, prop_name3);

                RE::BSFixedString prop_name4 = "doorOpened";
                bool door_opened = General::Script::GetProperty<bool>(object_p, prop_name4);


                std::string advice = "Check surroundings to see if there is something linked to the lever that needs to be fixed";

                std::string pillar_check = check_very_interesting_objects();
                if (pillar_check.find("Pillar"))
                    advice = "There are puzzle pillars nearby, interact with them until they are all in correct positions so lever opens the path";


                if (pillars_solved < 2 || !(alt_solution || puzzle_solved))
                    return " Something is wrong. The lever did not do anything..." + advice;
                else
                {
                    if (pillars_solved == 2)
                    {
                        if (alt_solution)
                        {
                            if (!door_opened)
                                return " Something is wrong. The lever opened a gate, but the passage behind that gate is blocked... you need to open another gate" + advice;
                            else
                                return " The lever closed some gate...";
                        }
                        else
                            if (puzzle_solved)
                                if (!door_opened)
                                    return " Looks like the lever opened the correct gate... you can probably continue with your quest now";
                                else
                                    return " The lever closed some gate...";
                            else
                                return " Something is wrong. The lever did not do anything..." + advice;
                    }
                    else
                        return " Something is wrong. The lever did not do anything..." + advice;
                }
            }
        }
        else
        {
        normal_pillar:

            RE::BSFixedString prop_name = "numPillarsSolved";
            int pillars_solved = General::Script::GetProperty<int>(object_p, prop_name);

            RE::BSFixedString prop_name2 = "pillarCount";
            int pillars_need = General::Script::GetProperty<int>(object_p, prop_name2);

            std::string advice = "Check surroundings to see if there is something linked to the lever that needs to be fixed";

            std::string pillar_check = check_very_interesting_objects();
            if (pillar_check.find("Pillar"))
                advice = "There are puzzle pillars nearby, interact with them until they are all in correct positions so lever opens the path";


            if (pillars_solved < pillars_need)
                return " Something is wrong. The lever activated a trap instead of opening the door. " + advice;
            else
                return " Looks like the lever opened something...";
        }
    }




    std::string check_very_interesting_objects()
    {
        auto now = std::chrono::steady_clock::now().time_since_epoch().count();
        float delta_interesting = (double)(now - gave_interesting_notification_timestamp) / 1000000000.0;

        if (delta_interesting > 120.0f)
        {
            
            auto objects_around = GetObjectsAround(1);

            std::vector<std::string> very_interesting_objects{};

            if (objects_around.second.find("Pillar") != std::string::npos)
                very_interesting_objects.push_back("Puzzle pillar");

            if (objects_around.second.find("Lever") != std::string::npos)
                very_interesting_objects.push_back("Lever");

            if (objects_around.second.find("Chain") != std::string::npos)
                very_interesting_objects.push_back("Chain");

            if (objects_around.second.find("Ancient Nordic Door") != std::string::npos)
                very_interesting_objects.push_back("Ancient Nordic Door");


            if (std::size(very_interesting_objects) > 0)
            {
                std::string message = "";
                bool first = true;

                for (auto interesting : very_interesting_objects)
                {
                    if (first)
                        message += interesting;
                    else
                        message += ", " + interesting;

                    first = false;
                }

                gave_interesting_notification_timestamp = now;

                return "[You look around and see interesting objects: " + message + ". Maybe it's worth investigating it?";
            }
        }
        

        return "";
    }




    int player_overencumbered_by()
    {
        auto player_actor = (RE::Actor*)(RE::PlayerCharacter::GetSingleton()->AsReference());

        if (player_actor && player_actor->IsOverEncumbered())
        {
            int cur_weight = (int)player_actor->GetActorValue(RE::ActorValue::kInventoryWeight);
            int max_weight = (int)player_actor->GetActorValue(RE::ActorValue::kCarryWeight);

            return cur_weight - max_weight;
        }

        return 0;
    }



    void myMoveTo_Impl(RE::TESObjectREFR* object, const RE::ObjectRefHandle& a_targetHandle, RE::TESObjectCELL* a_targetCell, RE::TESWorldSpace* a_selfWorldSpace, const RE::NiPoint3& a_position, const RE::NiPoint3& a_rotation)
    {
        if (!object)
            return;

        using func_t = decltype(&myMoveTo_Impl);
        static REL::Relocation<func_t> func{ RELOCATION_ID(56227, 56626) };
        return func(object, a_targetHandle, a_targetCell, a_selfWorldSpace, a_position, a_rotation);
    }

    void SetPosition_moveto(RE::TESObjectREFR* a_target, RE::NiPoint3 new_pos)
    {
        if (!a_target)
            return;

        //assert(a_target);

        auto handle = a_target->GetHandle();

        myMoveTo_Impl(a_target, handle, a_target->GetParentCell(), a_target->GetWorldspace(), new_pos, a_target->data.angle);
    }







    bool raycastable(RE::TESObjectREFR* object, float range, bool only_forward)
    {
        if (!object)
            return false;

        auto camera = RE::PlayerCamera::GetSingleton();

        auto camera_pos = camera->pos;


        auto camera_dir = camera->cameraRoot.get()->world.rotate;
        auto camera_lookat = camera_dir.GetVectorY();

        auto aim_pos = WalkerProcessor::get_estimate_aim_pos(object, false);

        auto delta_pos = aim_pos - camera_pos;

        auto delta_pos_noZ = delta_pos;
        auto camera_lookat_noZ = camera_lookat;
        delta_pos_noZ.z = 0.0f;
        camera_lookat_noZ.z = 0.0f;
        delta_pos_noZ.Unitize();
        camera_lookat_noZ.Unitize();

        if (only_forward)
        {
            float mulY = camera_lookat_noZ * delta_pos_noZ;
            if (mulY < -0.1)
                return false;
        }

        auto raycast_ref = MiscThings::GetRaycastRef(camera_pos, delta_pos, range, object);

        auto player_ref = RE::PlayerCharacter::GetSingleton()->AsReference();

        if (!only_forward && raycast_ref == player_ref)
        {

            raycast_ref = MiscThings::GetRaycastRef(camera_pos - camera_lookat*15.0f, delta_pos, range, object);
        }
            

        return raycast_ref == object;
    }









    RE::TESObjectREFR* get_word_of_power(RE::TESObjectREFR* trigger_zone_ref, bool strict)
    {
        if (!trigger_zone_ref)
            return nullptr;


        auto object_p = General::Script::GetObject(trigger_zone_ref, "WordWallTriggerScript");
        //auto object_p = General::Script::GetObject(trigger_zone_ref, "WordWallTrigger02Script");
        
        if (!object_p)
            return nullptr;
        else
        {

            RE::BSFixedString prop_name = "::wordLearned_var";

            bool word_learned = General::Script::GetVariable<bool>(object_p, prop_name);

            if (!word_learned)
            {
                RE::BSFixedString prop_name2 = "lookTarget";
                auto lookat_marker1 = General::Script::GetProperty<RE::TESObjectREFR*>(object_p, prop_name2);

                if (!strict && (lookat_marker1 == nullptr))
                {
                    RE::BSFixedString prop_name3 = "lookTargetWord02";
                    lookat_marker1 = General::Script::GetProperty<RE::TESObjectREFR*>(object_p, prop_name3);
                }


                return lookat_marker1;
            }
            

            return (RE::TESObjectREFR*)(-1); //means its actually a wall but its inactiva
        }

        
    }








    bool player_escaping_jail()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        return player->playerFlags.escaping;
    }


    bool is_insect(RE::TESObjectREFR* object)
    {
        if (!object)
            return false;

        auto base_obj = object->GetBaseObject();
        auto base_type = base_obj->GetFormType();

        if (base_type == RE::FormType::Activator)
        {
            auto activator = (RE::TESObjectACTI*)base_obj;

            std::string model = activator->GetModel();

            if (model.find("Critters") != std::string::npos)
            {

                auto path_extra = object->extraList.GetByType(RE::ExtraDataType::kRefrPath);

                if (path_extra)
                    return true;
            }
        }

        return false;
    }

    bool is_ore(RE::TESObjectREFR* object)
    {
        if (!object)
            return false;

        auto base_obj = object->GetBaseObject();
        auto base_type = base_obj->GetFormType();

        if (base_type == RE::FormType::Activator)
        {
            auto activator = (RE::TESObjectACTI*)base_obj;

            std::string model = activator->GetModel();

            if (model.find("MineOre") != std::string::npos)
            {
                return true;
            }
        }

        return false;
    }


    bool is_flora(RE::TESObjectREFR* object)
    {
        if (!object)
            return false;

        auto base_obj = object->GetBaseObject();
        auto base_type = base_obj->GetFormType();

        if (base_type == RE::FormType::Flora)
        {
            return true;
        }

        return false;
    }

    bool is_tree(RE::TESObjectREFR* object)
    {
        if (!object)
            return false;

        auto base_obj = object->GetBaseObject();
        auto base_type = base_obj->GetFormType();

        if (base_type == RE::FormType::Tree)
        {
            return true;
        }

        return false;
    }




    bool player_hp_more_than(float val_percent)
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        int cur_health = player->GetActorValue(RE::ActorValue::kHealth);
        int max_health = player->GetActorValueMax(RE::ActorValue::kHealth);

        return cur_health >= max_health*val_percent/100.0f;

    }


    bool player_hp_less_than(float val_percent)
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        int cur_health = player->GetActorValue(RE::ActorValue::kHealth);
        int max_health = player->GetActorValueMax(RE::ActorValue::kHealth);

        return cur_health < max_health * val_percent / 100.0f;

    }




    long long time_of_death = 0;


    void set_time_of_death(long long timestamp)
    {
        time_of_death = timestamp;
    }

    long long get_time_of_death()
    {
        return time_of_death;
    }


    bool is_in_main_menu()
    {
        RE::UI* ui = RE::UI::GetSingleton();
        return ui->IsMenuOpen(RE::MainMenu::MENU_NAME);
    }


    float triangle_area(RE::NiPoint2 a, RE::NiPoint2 b, RE::NiPoint2 c)
    {
        return 0.5f * abs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    }

    float is_inside_of_rectangle(RE::NiPoint2 p, RE::NiPoint2 a, RE::NiPoint2 b, RE::NiPoint2 c, RE::NiPoint2 d)
    {
        float rectangle_area = triangle_area(a, b, c) + triangle_area(a, d, c);

        float point_area = triangle_area(p, a, b) + triangle_area(p, b, c) + triangle_area(p, c, d) + triangle_area(p, d, a);

        return rectangle_area >= point_area*0.99f;
    }




    RE::TESObjectREFR* get_linked_ref_children(RE::TESObjectREFR* object, int index)
    {
        if (object)
        {
            auto extra = object->extraList.GetByType(RE::ExtraDataType::kLinkedRefChildren);

            if (extra)
            {
                auto extra_linked = (RE::ExtraLinkedRefChildren*)extra;

                int id = 0;
                for (auto linked_ref : extra_linked->linkedChildren)
                {
                    if (id == index && linked_ref.refr && linked_ref.refr.get() && linked_ref.refr.get().get())
                    {
                        return linked_ref.refr.get().get();
                    }
                    id++;
                }
            }
        }

        return nullptr;
    }


    bool object_inside_of_windhelm_redirect_box(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto player_pos = object->GetPosition();

            auto player_worldspace = object->GetWorldspace();

            auto tamriel_worldspace = RE::TESForm::LookupByID(0x3c);

            if (player_worldspace && player_worldspace == tamriel_worldspace)
            {
                RE::NiPoint2 a = { 139748.0156, 31356.5215 }; //-5733.7241
                RE::NiPoint2 b = { 135511.3906, 35718.7227 }; //-5633.2554
                RE::NiPoint2 c = { 139265.0781, 39366.6172 }; //-5640.6494
                RE::NiPoint2 d = { 143502.7969, 35001.8555 }; //-5724.9077

                RE::NiPoint2 p = { player_pos.x, player_pos.y };
                if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
                    return true;
            }
            
        }


        return false;
    }



    bool player_inside_of_alftand_goodbox()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            auto player_pos = player->GetPosition();

            RE::NiPoint2 a = { 77113.9219, 78285.6953 }; //-5733.7241
            RE::NiPoint2 b = { 78506.6953, 76860.0625 }; //-5633.2554
            RE::NiPoint2 c = { 78288.2188, 76661.2422 }; //-5640.6494
            RE::NiPoint2 d = { 76897.3906, 78095.0078 }; //-5724.9077

            float z = -5780.0f;

            if (abs(player_pos.z - z) < 150.0f)
            {
                RE::NiPoint2 p = { player_pos.x, player_pos.y };
                if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
                    return true;
            }
        }


        return false;
    }

    bool player_inside_of_soltsteim_midgates()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            auto player_pos = player->GetPosition();

            auto player_cell = player->GetParentCell();

            if (player_cell && player_cell->GetFormID() == 0x04019AD8)
            {
                RE::NiPoint2 a = { -136.172562,  1702.44800 }; //242
                RE::NiPoint2 b = { -136.172562,  2364.24829 }; //242
                RE::NiPoint2 c = { 1637.52136, 2364.24829 }; //242
                RE::NiPoint2 d = { 1637.52136, 1702.44800 }; //242

                float z = 242.0f;

                if (abs(player_pos.z - z) < 150.0f)
                {
                    RE::NiPoint2 p = { player_pos.x, player_pos.y };
                    if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
                        return true;
                }
            }

        }

        return false;
    }



    bool player_inside_of_ustengrev_gate_puzzle()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto player_parent_cell = player->GetParentCell();
        RE::TESObjectREFR* done_trigger = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x52477);
        if (player_parent_cell && player_parent_cell->formID == 0x152aa && !done_trigger->IsDisabled())
        {
            RE::NiPoint2 a = { -2108.78174, -743.853455 };//, -1103.65869
            RE::NiPoint2 b = { -604.535400, -212.883499 };// , -1095.44202
            RE::NiPoint2 c = { -303.614716, -1026.13635 };// , -1113.10095
            RE::NiPoint2 d = { -1777.06445, -1566.35852 };// , -1104.09680

            float z = -1100.0f;

            if (abs(player_pos.z - z) < 150.0f)
            {
                RE::NiPoint2 p = { player_pos.x, player_pos.y };
                if (is_inside_of_rectangle(p, a, b, c, d))
                {
                    //player inside of ustengrev gate puzzle zone
                    return true;
                }
            }
        }

        return false;
    }


    bool player_inside_of_karthspire_plate_puzzle()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto player_worldspace = player->GetWorldspace();
        RE::TESObjectREFR* second_bridge = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4e862);
        if (player_worldspace && player_worldspace->formID == 218777 && MiscThings::two_state_activator_state(second_bridge) == 0)
        {
            RE::NiPoint2 a = { 132945.125, -4114.51172 };//, -1103.65869
            RE::NiPoint2 b = { 131872.359, -4103.85986 };// , -1095.44202
            RE::NiPoint2 c = { 131746.062, -2716.95947 };// , -1113.10095
            RE::NiPoint2 d = { 132914.531, -2947.98657 };// , -1104.09680

            float z = 2560.0f;

            if (abs(player_pos.z - z) < 150.0f)
            {
                RE::NiPoint2 p = { player_pos.x, player_pos.y };
                if (is_inside_of_rectangle(p, a, b, c, d))
                {
                    //player inside
                    return true;
                }
            }
        }

        return false;
    }


    bool object_inside_of_helgen_keep_cage(RE::TESObjectREFR* object)
    {
        if (object && is_object_valid(object))
        {
            auto object_pos = object->GetPosition();

            auto object_parent_cell = object->GetParentCell();
            if (object_parent_cell && object_parent_cell->formID == 0x152aa)
            {
                RE::NiPoint2 a = { -1268.38196, 1375.06421 };//, -1103.65869
                RE::NiPoint2 b = { -1417.26965, 1372.94409 };// , -1095.44202
                RE::NiPoint2 c = { -1416.88232, 1548.62952 };// , -1113.10095
                RE::NiPoint2 d = { -1269.51685, 1548.62476 };// , -1104.09680

                float z = -750.0f;

                if (abs(object_pos.z - z) < 150.0f)
                {
                    RE::NiPoint2 p = { object_pos.x, object_pos.y };
                    if (is_inside_of_rectangle(p, a, b, c, d))
                    {
                        //object inside
                        return true;
                    }
                }
            }
        }
        

        return false;
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


    //from discord
    //16-31  15      14          13   8-12   7    0-6
    //group  linked  dontcollide idk  part#  idk  layer
    //Things in the same group generally don't collide with each other unless bit 15 is set, in which case it uses the part# to determine whether to collide. That's used for ragdolls.
    //The layer is in the first 7 bits and that's why filterInfo & 0x7f is the layer.
    //    One of the bhkCollisionFilter funcs takes in 2 filter infos and returns whether they should collide or not if you want to see exactly how the two infos are compared

    //                                      |15     |7     |0
    uint32_t my_filter4 = 0b00000101011001110000000000011110; //whiterun guard
    uint32_t my_filter0 = 0b00000101011010101000000000011110; //bjorlams filter (has bit 15 set)
    uint32_t my_filter1 = 0b00000000000010010000000000011110; //player filter
    uint32_t my_filter2 = 0b00000000000010010000000000001110; //filter without invisible zones, that doesnt work on bjorlam
    uint32_t my_filter3 = 0b00011000101111000000000000011110; //alduin filter (maybe all dragons)

    RayCastResult RayCast(RE::NiPoint3 rayStart, RE::NiPoint3 rayDir, float maxDist,
        RE::Actor* actor, RE::TESObjectREFR* target) {
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
        rayDir.Unitize();
        pickData.rayInput.to = (rayStart + rayDir * maxDist) * havokWorldScale;



        bool linked = false;

        if (target && target->IsActor())
        {
            auto target_actor = (RE::Actor*)target;
            RE::CFilter cFilter_info{};
            target_actor->GetCollisionFilterInfo(cFilter_info);

            if (cFilter_info.filter & (1 << 15))
                linked = true;
        }



        if (linked)
            pickData.rayInput.filterInfo = (RE::CFilter)my_filter1; //idk why but for sitting actors filter2 doesnt work
        else
            pickData.rayInput.filterInfo = (RE::CFilter)my_filter2;

        // static_cast<RE::CFilter>(cFilter.filter | static_cast<uint32_t>(layerMask));

        // Perform the raycast
        if (bhkWorld->PickObject(pickData) && pickData.rayOutput.HasHit()) {
            result.didHit = true;
            result.didHit = true;
            result.distance = maxDist * pickData.rayOutput.hitFraction;
            result.normalOut = pickData.rayOutput.normal;

            result.layer = pickData.rayOutput.rootCollidable->GetCollisionLayer();

            result.hitObjectRef = RE::TESHavokUtilities::FindCollidableRef(*pickData.rayOutput.rootCollidable);
        }

        return result;
    }


    //excludes player
    RE::TESObjectREFR* GetRaycastRef(RE::NiPoint3 from, RE::NiPoint3 aimVector, float distance, RE::TESObjectREFR* target)
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        auto raycast_result = RayCast(from, aimVector, distance, player_actor, target);

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
        if (player)
        {
            auto player_actor = (RE::Actor*)player->AsReference();
            //auto extralist = &player->extraList;
            //auto horse_data = extralist->GetByType(RE::ExtraDataType::kHorse);

            //if (horse_data)
            if (player_actor->IsOnMount())
                return true;
        }


        return false;
    }

    std::string get_blocking_object_name2(RE::TESObjectREFR* a_ref)
    {
        if (!a_ref)
            return "";

       // return "";

        std::string result = "";

        result = get_blocking_object_name(a_ref);

        if (result == "")
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

            if (base_type == RE::FormType::Activator)
            {
                auto static_obj = (RE::TESObjectACTI*)base_obj;

                std::string model = static_obj->GetModel();

                if (model.find("PuzzleDoorBase01") != std::string::npos)
                {
                    result = "[Puzzle door] Ancient Nordic Door";
                }

                if (model.find("Wheel02") != std::string::npos)
                {
                    result = "[Puzzle door] Ancient Nordic Door";
                }
            }
        }

        return result;
    }



    bool object_is_interactive(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto base_obj = object->GetBaseObject();

            auto base_type = base_obj->GetFormType();

            if (object->modelState != 0 && base_obj && base_type != RE::FormType::Static)
            {
                auto player = RE::PlayerCharacter::GetSingleton();
                auto player_ref = player->AsReference();
                RE::BSString result_string = "";

                RE::TESNPC* player_npc = (RE::TESNPC*)RE::TESForm::LookupByID(0x7); //left hand
                player_npc->GetActivateText(object, result_string);

                //base_obj->GetActivateText(player_ref, result_string);

                if (result_string != "" && result_string != "Search\n")
                    return true;
            }
        }

        return false;
    }




    std::string get_blocking_object_name(RE::TESObjectREFR* a_ref)
    {
        if (!a_ref)
            return "";

       // return "";
        std::string result = "";

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
                std::string name = MiscThings::insert_object_into_list_custom_name("[Destructible] Cobweb", a_ref);

                result = name;
            }

            if (model.find("StockadeBarricade") != std::string::npos)
            {
                std::string name = MiscThings::insert_object_into_list_custom_name("[Destructible] Barricade", a_ref);

                result = name;
            }

            if (model.find("RTMercerRamp01") != std::string::npos)
            {
                std::string name = MiscThings::insert_object_into_list_custom_name("[Destructible (with Shout or Ranged Weapon)] Ramp Mechanism", a_ref);

                result = name;
            }
            

            if (model.find("RTMausoleumDoor01") != std::string::npos)
            {
                std::string name = MiscThings::insert_object_into_list_custom_name("[Secret door] Secret Sliding Altar Door", a_ref);

                result = name;
            }
            


            if (model.find("PuzzleDoorKeyHole") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
            {
                std::string name = MiscThings::insert_object_into_list_custom_name("[Puzzle door] Ancient Nordic Door", a_ref);
                result = name;
            }

            if (model.find("WRTempleTree02Root") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
            {
                std::string name = MiscThings::insert_object_into_list_custom_name("Elder Tree Roots", a_ref);
                result = name;
            }
        }

        if (base_type == RE::FormType::Door)// && a_ref->GetDisplayFullName() == "")
        {
            auto door = (RE::TESObjectDOOR*)base_obj;
            std::string model = door->GetModel();

            if (model.find("CaveGSecretDoor") != std::string::npos)
            {
                std::string name = MiscThings::insert_object_into_list_custom_name("[Secret door] Stone wall door", a_ref);
                result = name;
            }

            if (model.find("DwePtnDoor01") != std::string::npos)
            {
                if (!MiscThings::object_is_interactive(a_ref)) //these gates can be interactive and can be not interactive, using same model. interactive - are not blocking in style of this function
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("Dwemer metal gate", a_ref);
                    result = name;
                }

            }
        }

            auto fire_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xf4986);

            if (a_ref == fire_door && fire_door)
            {
                std::string name = MiscThings::insert_object_into_list_custom_name("Magical Fire Door", a_ref);
                result = name;
            }

            auto ice_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xf4973);

            if (a_ref == ice_door && ice_door)
            {
                std::string name = MiscThings::insert_object_into_list_custom_name("Magical Ice Door", a_ref);
                result = name;
            }



        RE::ExtraDataList* extralist = &a_ref->extraList;
        auto extra_anim = extralist->GetByType(RE::ExtraDataType::kAnimGraphManager);
        if (extra_anim)
        {
            auto extra_anim_graph = (RE::ExtraAnimGraphManager*)extra_anim;
            if (extra_anim_graph->animGraphMgr)
            {

                if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "PortImpGate01")
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("Pole Gate", a_ref);
                    result = name;
                }


                if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "NorRetractableBridge01")
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("Large wooden bridge", a_ref);
                    result = name;
                }

                if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "ImpPortcullisSmall01")
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("Heavy wooden gate", a_ref);
                    result = name;
                }

                if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "PortcullisLarge01")
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("Metal gate", a_ref);
                    result = name;
                }

                if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "NorPortcullisGate01")
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("Small metal gate", a_ref);
                    result = name;
                }


                if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "NorSecRmSmDoorSm01")
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("Secret cave wall door", a_ref);
                    result = name;
                }
                //NorSecRmSmDoorSm01

                if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "PortGatePole06")
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("Dwemer metal pole gate", a_ref);
                    result = name;
                }

                if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "PortGatePole01")
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("Nordic metal pole gate", a_ref);
                    result = name;
                }

                if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "PortGatePoleDwemer01")
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("Wide dwemer metal pole gate", a_ref);
                    result = name;
                }

            }
        }

        return result;
    }



    std::string get_potential_blocking_object(float range, RE::TESObjectREFR* ignore_ref)
    {
        std::string result = "";

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();

        if (player_ref)
        {
            RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, range,
                //player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
                [&](RE::TESObjectREFR* a_ref) {

                    if (a_ref == ignore_ref)
                        return RE::BSContainer::ForEachResult::kContinue;

                    if (!MiscThings::is_object_valid(a_ref))
                        return RE::BSContainer::ForEachResult::kContinue;

                    if (result != "")
                        return RE::BSContainer::ForEachResult::kStop;

                    result = get_blocking_object_name(a_ref);

                    if (result != "")
                        return RE::BSContainer::ForEachResult::kStop; //this stop isnt stopping for some reason

                    return RE::BSContainer::ForEachResult::kContinue;
                });
        }



        return result;
    }


    void check_unseen_levers_if_no_levers(float range, RE::TESObjectREFR* ignore_ref)
    {
        //intended to be called after we have detected blocking object, in case there are no activators currently in the list
        //maybe its behind the corner or something


        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();


        //first - check if we have activators in the list

        if (MiscThings::is_objects_around_valid())
        {
            auto p_objects_around = MiscThings::get_p_objects_around();

            if (p_objects_around)
            {
                for (auto object_around : *p_objects_around)
                {
                    if (is_object_valid(object_around.second.object))
                    {
                        auto base_obj = object_around.second.object->GetBaseObject();
                        if (base_obj)
                        {
                            auto base_type = base_obj->GetFormType();

                            if (base_type == RE::FormType::Furniture) //pullchains/levers
                            {
                                auto furniture = (RE::TESFurniture*)base_obj;
                                auto workbenchtype = furniture->workBenchData.benchType;
                                if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kNone)
                                {
                                    if (furniture->HasKeywordString("ActivatorLever") || furniture->HasKeywordString("isPullChain"))
                                    {
                                        return; //we have some lever or chain. not interesting anymore
                                    }
                                }
                            }

                            if (base_type == RE::FormType::Activator) //pullchains/levers
                            {
                                auto activator = (RE::TESObjectACTI*)base_obj;

                                std::string model = activator->GetModel();

                                if (model.find("Lever") != std::string::npos || model.find("Chain") != std::string::npos)
                                    return; //we have some lever or chain. not interesting anymore
                            }

                        }
                    }
                    
                }
            }
        }

        //if we got here - no levers/pullchains

        //now find the blocking object

        RE::TESObjectREFR* blocking_object = nullptr;

        if (player_ref)
        {
            RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, range,
                //player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
                [&](RE::TESObjectREFR* a_ref) {

                    if (a_ref == ignore_ref)
                        return RE::BSContainer::ForEachResult::kContinue;

                    if (!MiscThings::is_object_valid(a_ref))
                        return RE::BSContainer::ForEachResult::kContinue;


                    std::string blocking_name = get_blocking_object_name(a_ref);

                    if (blocking_name != "")
                    {
                        blocking_object = a_ref; //blocker found
                        return RE::BSContainer::ForEachResult::kStop;
                    }
                        

                    return RE::BSContainer::ForEachResult::kContinue;
                });
        }


        
        if (blocking_object)
        {
            std::vector<RE::TESObjectREFR*> levers{};

            RE::TES::GetSingleton()->ForEachReferenceInRange(blocking_object, 3000.0f,
                //player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
                [&](RE::TESObjectREFR* a_ref) {

                    if (a_ref == ignore_ref)
                        return RE::BSContainer::ForEachResult::kContinue;

                    if (!MiscThings::is_object_valid(a_ref))
                        return RE::BSContainer::ForEachResult::kContinue;


                    auto base_obj = a_ref->GetBaseObject();
                    if (base_obj)
                    {
                        auto base_type = base_obj->GetFormType();

                        if (base_type == RE::FormType::Furniture) //pullchains/levers
                        {
                            auto furniture = (RE::TESFurniture*)base_obj;
                            auto workbenchtype = furniture->workBenchData.benchType;

                            bool found = false;

                            if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kNone)
                            {
                                if (furniture->HasKeywordString("ActivatorLever") || furniture->HasKeywordString("isPullChain"))
                                {
                                    //already checked for being valid above
                                    levers.push_back(a_ref);
                                    found = true;
                                }
                            }
                            
                            /*
                            if (!found)
                            {
                                std::string model = furniture->GetModel();

                                if (model.find("Lever") != std::string::npos || model.find("Chain") != std::string::npos)
                                    levers.push_back(a_ref);
                            }
                            */

                        }


                        if (base_type == RE::FormType::Activator) //pullchains/levers
                        {
                            auto activator = (RE::TESObjectACTI*)base_obj;

                            std::string model = activator->GetModel();

                            if (model.find("Lever") != std::string::npos || model.find("Chain") != std::string::npos)
                                levers.push_back(a_ref);
                        }
                    }
                    return RE::BSContainer::ForEachResult::kContinue;
                });


            if (std::size(levers) > 0)
            {
                //something is found... sort by distance from blocking object. if more than 1 - get 2nd (closest one is probably backside lever)

                std::sort(levers.begin(), levers.end(), [&](RE::TESObjectREFR* left, RE::TESObjectREFR* right) {

                    if (!left || !right)
                        return false;

                    if (left->data.objectReference && right->data.objectReference && left->formID && right->formID)
                        return left->GetDistance(blocking_object) < right->GetDistance(blocking_object); //switch > to < for inversed order. this is last->closest
                    else
                        return false;
                    });


                std::vector<RE::TESObjectREFR*> best_candidates{};

                if (std::size(levers) > 1)
                {
                    auto closest_candidate = levers.at(0);
                    float closest_distance = closest_candidate->GetDistance(blocking_object);

                    if (closest_distance > 700.0f)
                        best_candidates.push_back(closest_candidate); //add even closest if he is far enough
                
                    for (auto i = 1; i < std::size(levers); i++)
                    {
                        best_candidates.push_back(levers.at(i));
                    }
                }
                else
                {
                    auto closest_candidate = levers.at(0);
                    float closest_distance = closest_candidate->GetDistance(blocking_object);

                    best_candidates.push_back(closest_candidate);
                }

                
                    
                std::string info = "";
                for (auto best_candidate : best_candidates)
                {
                    info += MiscThings::insert_object_into_list_and_get_info(best_candidate) + "; ";
                }

                if (info != "")
                {
                    send_random_context("You see: " + info);
                }
            }
        }
    }




    int get_destructible_state(RE::TESObjectREFR* web)
    {
        int result = 0;

        if (!web)
            return 0;

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
                        result = -1;
                }

                if (model.find("StockadeBarricade") != std::string::npos)
                {
                    auto extralist = &web->extraList;
                    auto extra_swap = extralist->GetByType(RE::ExtraDataType::kModelSwap);
                    if (extra_swap)
                        result = 2;
                    else
                        result = -1;
                }

                if (model.find("TG01DwemerUrn") != std::string::npos)
                {
                    auto extralist = &web->extraList;
                    auto extra_swap = extralist->GetByType(RE::ExtraDataType::kModelSwap);
                    if (extra_swap)
                        result = 3;
                    else
                        result = -1;
                }

                if (model.find("BeeHive01.nif") != std::string::npos)
                {
                    auto extralist = &web->extraList;
                    auto extra_swap = extralist->GetByType(RE::ExtraDataType::kObjectHealth);
                    if (extra_swap)
                    {
                        auto extra_health = (RE::ExtraObjectHealth*)extra_swap;

                        if (extra_health->health <= 60.0f)
                            result = 4;
                    }
                    else
                        result = -1;
                }

            }
        }

        return result;
    }



    int two_state_activator_state(RE::TESObjectREFR* activator)
    {
        //default2StateActivator
        int result = -1;

        if (!activator)
            return -1;

        auto object_p = General::Script::GetObject(activator, "default2StateActivator");
        //dunCGdefault2StateActivator
        if (!object_p)
            object_p = General::Script::GetObject(activator, "dunCGdefault2StateActivator");

        if (object_p)
        {

            RE::BSFixedString prop_name = "::isOpen_var";

            if (General::Script::GetVariable<bool>(object_p, prop_name))
                result = 0;
            else
                result = 1;

        }

        

        object_p = General::Script::GetObject(activator, "dunLabyrinthianElementalDoor");

        if (object_p)
        {
            std::string state = "";
            state = object_p->currentState;

            if (state == "inactive")
                return 0;
            else
                return 1;
        }



        object_p = General::Script::GetObject(activator, "dunUstenPuzGateSCRIPT");
        
            if (object_p)
            {
                RE::BSFixedString prop_name = "::isOpen_var";

                if (General::Script::GetVariable<bool>(object_p, prop_name))
                    result = 0;
                else
                    result = 1;

            }
        
            object_p = General::Script::GetObject(activator, "NorPortcullisSCRIPT");

            if (object_p)
            {
                //RE::BSFixedString prop_name = "isClosing";

                //bool is_closing = General::Script::GetProperty<bool>(object_p, prop_name);

                RE::BSFixedString prop_name = "isOpen";

                if (General::Script::GetVariable<bool>(object_p, prop_name))
                    result = 0;
                else
                    result = 1;

            }


            object_p = General::Script::GetObject(activator, "RiftenCasketDoorScript");

            if (object_p)
            {
                //RE::BSFixedString prop_name = "isClosing";

                //bool is_closing = General::Script::GetProperty<bool>(object_p, prop_name);

                RE::BSFixedString prop_name = "::isOpen_var";

                if (General::Script::GetVariable<bool>(object_p, prop_name))
                    result = 0;
                else
                    result = 1;

            }

            //


        object_p = General::Script::GetObject(activator, "DA04HubScript");
        if (object_p)
        {
            if (object_p->currentState == "opened")
                return 20; //done

            if (object_p->currentState == "busy")
                return 10; //"started moving" or some shit

            RE::BSFixedString prop_name = "currentPos";
            int current_pos = General::Script::GetVariable<int>(object_p, prop_name);
            result = current_pos;
        }


        object_p = General::Script::GetObject(activator, "MG06Dome01Script");
        if (object_p)
        {

            std::string state = "";
            state = object_p->currentState;

            if (state.length() > 0)
            {
                auto num_string_p = state.back();

                if (state.find("osition") != std::string::npos)
                {
                    if (num_string_p != std::string::npos)
                    {
                        char last_char = num_string_p;

                        int num_int = (int)last_char - '0';

                        return num_int;
                    }
                }
                else
                {
                    return 0;
                }

            }
        }




        object_p = General::Script::GetObject(activator, "DA04ArmillaryScript");
        if (object_p)
        {
            if (object_p->currentState == "busy")
                return 10;

            RE::BSFixedString prop_name = "::currentPos_var";
            int current_pos = General::Script::GetVariable<int>(object_p, prop_name);
            result = current_pos;
        }

        object_p = General::Script::GetObject(activator, "DA04ButtonScript");
        if (object_p)
        {
            if (object_p->currentState == "opened")
                return 0; 

            if (object_p->currentState == "Closed")
                return 1;

            if (object_p->currentState == "busy")
                return -1;
        }

        object_p = General::Script::GetObject(activator, "tg07RampScript");
        if (object_p)
        {
            if (object_p->currentState == "Done")
                return 0;

            if (object_p->currentState == "Waiting")
                return 1;

            if (object_p->currentState == "busy")
                return -1;
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


            if (object_p->currentState == "done" || object_p->currentState == "Done")
            {
                //RE::BSFixedString prop_name = "::doorOpened_var";

                //if (General::Script::GetVariable<bool>(object_p, prop_name))
                //    result = 0;
                //else
                result = 1;
            }
            else
            {
                if (object_p->currentState == "busy" || object_p->currentState == "Busy")
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



        /*
        auto test = RE::TESForm::GetAllForms();

        auto test2 = test.first;

        for (auto form : *test2)
        {
            object_p = General::Script::GetObject(form.second, "MG08EyeScript");

            if (object_p)
            {
                bool stop_here = false;
            }
        }
        */



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


                if (base_type == RE::FormType::Activator)
                {
                    auto acti = (RE::TESObjectACTI*)base_obj;
                    std::string model = acti->GetModel();

                    if (model.find("SldJailWallCollapse01") != std::string::npos)
                    {
                        auto extra_action = activator->extraList.GetByType(RE::ExtraDataType::kAction);

                        if (extra_action)
                            result = 1;

                    }

                    if (model.find("WRTempleTree02Root") != std::string::npos)
                    {

                        auto extra = activator->extraList.GetByType(RE::ExtraDataType::kStartingPosition);

                        if (extra)
                            result = 0;
                        else
                            result = 1;

                        /*
                            auto extra_linked = (RE::ExtraLinkedRef*)extra;

                            for (auto linked_ref : extra_linked->linkedRefs)
                            {
                                if (linked_ref.refr)
                                {
                                    auto linked_ref_ref = linked_ref.refr;

                                    if (linked_ref_ref->IsDisabled()) //collision box disabled/enabled
                                        
                                    else
                                        result = 1;
                                }
                            }
                        }
                        */
                    }

                }
            }
        }

        return result;
    }


    std::string var_name123 = "enable";


    int trap_firing(RE::TESObjectREFR* trap)
    {
        int result = -1;

        if (!trap)
            return -1;

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


        object_p = General::Script::GetObject(trap, "PhysicsTrap");
        if (object_p)
        {
            if (object_p->currentState == "Done")
            {
                result = 9; //falling rocks
            }
        }

        object_p = General::Script::GetObject(trap, "Tripwire");
        if (object_p)
        {
            if (object_p->currentState == "DoNothing")
            {
                result = 10; //tripwire
            }
        }

        object_p = General::Script::GetObject(trap, "TrapBear");
        if (object_p)
        {
            if (object_p->currentState == "Closed")
            {
                result = 11; //beartrap rearmed
            }

            if (object_p->currentState == "Open")
            {
                result = 12; //beartrap triggered
            }

        }

        object_p = General::Script::GetObject(trap, "MaceTrap");
        if (object_p)
        {
            if (object_p->currentState == "doOnce")
            {
                result = 13; //mammoth skull
            }
        }



        object_p = General::Script::GetObject(trap, "TrapTriggerHinge");
        if (object_p)
        {
            if (object_p->currentState == "triggered")
            {
                result = 14; //hinge triggered
            }

            if (object_p->currentState == "disarmed")
            {
                result = 15; //hinge disarmed
            }

        }

        object_p = General::Script::GetObject(trap, "BatteringTrap");
        if (object_p)
        {
            if (object_p->currentState == "doOnce")
            {
                result = 16; //battering ram log
            }
        }



        auto saartal_fx = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x106bba);

        if (trap == saartal_fx)
        {

            auto saartal_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MG02");


            if (saartal_quest)
            {
                auto stage = saartal_quest->GetCurrentStageID();

                if (stage == 40)
                {
                    auto amulet = (RE::TESBoundObject*)RE::TESForm::LookupByID(0x233d0);

                    if (amulet)
                    {
                        if (is_equipped(amulet))
                        {
                            return 17;
                        }
                    }
                }
            }

            return 18;

            /*
            RE::BSAnimationGraphManagerPtr p_manager = nullptr;

            bool test = trap->GetAnimationGraphManager(p_manager);

            bool result = false;

            


            bool test2 = trap->GetGraphVariableBool(var_name123, result);

            if (test && p_manager)
            {
                if (p_manager->activeGraph < std::size(p_manager->graphs))
                {
                    auto& graph = p_manager->graphs[p_manager->activeGraph];

                    bool stop_here = false;
                }

                bool stop_here = false;
            }

            bool stop_here = false;

            if (saartal_quest)
            {
                object_p = General::Script::GetObject(saartal_quest, "MG02QuestScript");
                if (object_p)
                {
                    RE::BSFixedString prop_name = "::WallTrigger_var";

                    int test1 = General::Script::GetVariable<int>(object_p, prop_name);

                    prop_name = "::WallTrigger_var";


                    if (false)
                    {
                        result = 1; //dart trap firing
                    }

                    bool stop_here = false;
                }
            }
            */
        }



        return result;
    }


    bool is_pillar_solved(RE::TESObjectREFR* pillar)
    {
        if (!pillar)
            return false;

        bool result = false;


        auto object_p = General::Script::GetObject(pillar, "defaultPuzzlePillarScript");
        bool solved = false;

        if (object_p)
        {
            RE::BSFixedString prop_name = "pillarSolved";

            solved = General::Script::GetVariable<bool>(object_p, prop_name);

            if (solved)
                return true;
        }

        


        object_p = General::Script::GetObject(pillar, "dunSkuldafnPuzzlePillar01SCRIPT");

        if (object_p)
        {
            RE::BSFixedString prop_name = "pillarSolved";

            solved = General::Script::GetVariable<bool>(object_p, prop_name);

            if (solved)
                return true;
        }


        object_p = General::Script::GetObject(pillar, "defaultPuzzlePillarNoFurn");

        if (object_p)
        {
            RE::BSFixedString prop_name = "pillarSolved";

            solved = General::Script::GetVariable<bool>(object_p, prop_name);

            if (solved)
                return true;
        }


        object_p = General::Script::GetObject(pillar, "MG06Dome01Script");

        if (object_p)
        {
            RE::BSFixedString prop_name = "::DomeNumber_var";

            int dome_number = General::Script::GetVariable<int>(object_p, prop_name);

            std::string state = "";

            state = object_p->currentState;

            if (dome_number == 1)
                return state == "Position06";

            if (dome_number == 2)
                return state == "position07";

            if (dome_number == 3)
                return state == "position02";

        }


        object_p = General::Script::GetObject(pillar, "HallofStoriesDiskScript");

        if (object_p)
        {
            RE::BSFixedString prop_name = "ringSolved";

            solved = General::Script::GetVariable<bool>(object_p, prop_name);

            if (solved)
                return true;
        }


        object_p = General::Script::GetObject(pillar, "dunKarthspirePillarPuzzle");

        if (object_p)
        {
            if (get_pillar_face_name(pillar) == 2)
                return true;
        }


        return result;

    }


    std::string get_special_text(RE::TESObjectREFR* object)
    {
        std::string result = "";

        bool solved = false;

        if (!object)
            return "";


        auto object_p = General::Script::GetObject(object, "TG08LeverPuzzle");

        if (object_p)
        {
            std::string prop_name = "::puzzleSolved_var";

            solved = General::Script::GetVariable<bool>(object_p, prop_name);

            if (!solved)
            {
                //{name="::leverActivated_var" type={_rawType=kBool } }

                bool activated = false;

                std::string current_state = "";

                current_state = object_p->currentState;

                if (current_state == "pushedposition")
                    activated = true;

                result = "[Timed lever]";

                if (activated)
                    result += "[Is pulled. Quickly pull another timed lever!]";
                else
                    result += "[Not pulled]";
            }
            else
            {
                result = "[Solved]";
            }


        }


        auto saartal_pole_gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xde086);

        if (object == saartal_pole_gate && saartal_pole_gate)
        {
            result = "[Linked to pole gate, blocking the path]";
        }

        return result;
    }


    bool ignore_faraway_interior(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto base_object = object->GetBaseObject();

            auto base_type = base_object->GetFormType();

            if (base_type == RE::FormType::Activator)
            {
                auto object_p = General::Script::GetObject(object, "TG08LeverPuzzle");

                if (object_p)
                    return true;

            }
        }

        return false;
    }




    std::string get_pillar_solved_text(RE::TESObjectREFR* pillar)
    {
        std::string result = "";

        if (!pillar)
            return "";


        auto object_p = General::Script::GetObject(pillar, "defaultPuzzlePillarScript");
        auto object_p2 = General::Script::GetObject(pillar, "defaultPuzzlePillarPullBarNoFurn");
        auto object_p_skuldafn = General::Script::GetObject(pillar, "dunSkuldafnPuzzlePillar01SCRIPT");
        auto object_p_skuldafn2 = General::Script::GetObject(pillar, "dunSkuldafnPuzzlePillarTwoStage");//
        auto object_p_skuldafn3 = General::Script::GetObject(pillar, "defaultPuzzlePillarNoFurn");//
        //defaultPuzzlePillarNoFurn
        bool solved = false;

        if (object_p || object_p2 || object_p_skuldafn || object_p_skuldafn2 || object_p_skuldafn3)
        {
            RE::BSFixedString prop_name = "pillarSolved";

            if (object_p)
                solved = General::Script::GetVariable<bool>(object_p, prop_name);
            else
                if (object_p2)
                {
                    solved = General::Script::GetVariable<bool>(object_p2, prop_name);
                }
                else
                    if (object_p_skuldafn)
                    {
                        auto special_pillar = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xab223);
                        if (pillar == special_pillar)
                            solved = get_pillar_face_name(pillar) == 2;
                        else
                            solved = General::Script::GetVariable<bool>(object_p_skuldafn, prop_name);
                    }       
                    else
                        if (object_p_skuldafn2)
                        {
                            auto special_pillar = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xab223);
                            if (pillar == special_pillar)
                            {
                                using ObjectPtr = RE::BSTSmartPointer<RE::BSScript::Object>;

                                auto lever = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xab22b);
                            
                                if (lever)
                                {
                                    solved = get_pillar_face_name(pillar) == 2;
                                }

                            }
                            else
                                solved = General::Script::GetVariable<bool>(object_p_skuldafn2, prop_name);
                        }
                        else
                            if (object_p_skuldafn3)
                            {
                                solved = General::Script::GetVariable<bool>(object_p_skuldafn3, prop_name);
                            }


            if (solved)
                result = "[Is correct position]";
            else
                result = "[Is wrong position]";
        }

        object_p = General::Script::GetObject(pillar, "HallofStoriesDiskScript");

        if (object_p)
        {
            RE::BSFixedString prop_name = "ringSolved";

            solved = General::Script::GetVariable<bool>(object_p, prop_name);

            if (solved)
                result = "[Is correct position]";
            else
                result = "[Is wrong position]";
                
        }



        object_p = General::Script::GetObject(pillar, "MG06Dome01Script");

        if (object_p)
        {
            bool solved_dome = MiscThings::is_pillar_solved(pillar);

            if (solved_dome)
                result = "[Is correct position]";
            else
                result = "[Is wrong position]";
        }


        object_p = General::Script::GetObject(pillar, "dunKarthspirePillarPuzzle");

        if (object_p)
        {
            if (get_pillar_face_name(pillar) == 2)
                result = "[Is correct position]";
            else
                result = "[Is wrong position]";
        }


        


        return result;
    }



    std::string get_pillar_face_name(RE::TESObjectREFR* object, int code)
    {
        std::string result = "";

        if (!object)
            return "";


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
                            result = " [position Hawk]";
                        if (code == 2)
                            result = " [position Snake]";
                        if (code == 3)
                            result = " [position Whale]";
                    }

                    if (project_name == "SkyHavenPuzzlePillar01")
                    {
                        if (code == 1)
                            result = " [position King]";
                        if (code == 2)
                            result = " [position Dragonborn]";
                        if (code == 3)
                            result = " [position Warrior]";
                    }

                    if (project_name.find("PuzzleDoor") != std::string::npos && project_name.find("Wheel01") != std::string::npos)
                    {
                        if (code == 1)
                            result = " [position Hawk]";
                        if (code == 2)
                            result = " [position Whale]";
                        if (code == 3)
                            result = " [position Snake]";
                    }

                    if (project_name.find("PuzzleDoor") != std::string::npos && project_name.find("Wheel02") != std::string::npos)
                    {
                        if (code == 1)
                            result = " [position Owl]";
                        if (code == 2)
                            result = " [position Bear]";
                        if (code == 3)
                            result = " [position Moth]";
                    }

                    if (project_name.find("PuzzleDoor") != std::string::npos && project_name.find("Wheel03") != std::string::npos)
                    {
                        if (code == 1)
                            result = " [position Dragon]";
                        if (code == 2)
                            result = " [position Fox]";
                        if (code == 3)
                            result = " [position Moth]";
                    }

                    if (project_name.find("PuzzleDoor") != std::string::npos && project_name.find("Wheel04") != std::string::npos)
                    {
                        if (code == 1)
                            result = " [position Dragon]";
                        if (code == 2)
                            result = " [position Wolf]";
                        if (code == 3)
                            result = " [position Hawk]";
                    }

                    if (result != "")
                        result += " " + get_pillar_solved_text(object);
                }
            }
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

                        if (project_name == "SkyHavenPuzzlePillar01")
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


                        if (project_name.find("PuzzleDoor") != std::string::npos && project_name.find("Wheel") != std::string::npos)
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


                        /*
                        if (project_name.find("PuzzleDoor") != std::string::npos && project_name.find("Wheel03") != std::string::npos)
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
                        */
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
    //std::string old_subquest_notification = "";
    //std::string old_subquest_notification1 = "";
    std::vector<std::string> old_subquest_notification_vector{};
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
                                {
                                    if (var_string.find("SOVNGARDE") != std::string::npos)
                                        WalkerProcessor::look_up(0.25f);


                                    var_string = "Quest started: " + insert_quest_into_list_and_get_info(var_string.substr(9, var_string.length() - 9));
                                    WalkerProcessor::test_new_very_close_quest();


                                }
                                    
                                if (var_string.find("COMPLETED: ") == 0)
                                {
                                    if (var_string.find("DRAGONSLAYER") != std::string::npos)
                                        WalkerProcessor::look_up(0.25f);

                                    var_string = "Quest completed: " + insert_quest_into_list_and_get_info(var_string.substr(9, var_string.length() - 9));
                                    WalkerProcessor::test_new_very_close_quest();
                                }
                                    

                                if (var_string.find("DRAGON SOUL ABSORBED") != std::string::npos || var_string.find("WORD OF POWER LEARNED") != std::string::npos)
                                {
                                    if (player_has_shouts_to_unlock())
                                        register_unlock_shout_action();
                                }

                                send_random_context("[" + var_string + "]", false);
                            }
                        }


        if (ui)
            if (const auto menu = ui->GetMenu<RE::HUDMenu>(); menu)
                if (menu->uiMovie)
                {
                    for (int i = 0; i < 100; i++)
                    {
                        std::string num = std::to_string(i);

                        std::string path = "_root.HUDMovieBaseInstance.QuestUpdateBaseInstance.objective" + num + ".ObjectiveTextFieldInstance.TextFieldInstance.text";
                        if (menu->uiMovie->GetVariable(&var1, path.c_str()))
                        {
                            if (!var1.IsNull() && var1.IsString())
                            {
                                std::string var_string = var1.GetString();

                                bool was_before = false;
                                bool send_it = false;

                                for (auto old_subquest_notification : old_subquest_notification_vector)
                                {
                                    if (old_subquest_notification == var_string)
                                    {
                                        was_before = true;
                                        break;
                                    }
                                }

                                if (!was_before)
                                {
                                    old_subquest_notification_vector.push_back(var_string);
                                    send_it = true;
                                }


                                if (send_it)
                                {
                                    if (var_string.find("Completed") == 0 || var_string.find("Failed") == 0)
                                    {
                                        var_string = "Subquest " + var_string;
                                        WalkerProcessor::test_new_very_close_quest();

                                        if (var_string == "Subquest Completed: Kill Cicero")
                                        {
                                            RE::TESObjectREFR* blocker_1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x702f7ed);

                                            if (blocker_1)
                                            {
                                                auto old_pos = blocker_1->GetPosition();
                                                old_pos.z += 1000.0f;
                                                MiscThings::SetPosition_moveto(blocker_1, old_pos);
                                            }

                                            auto dawnstar_cutter = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x702f7ec);
                                            if (dawnstar_cutter)
                                            {
                                                MiscThings::SetPosition_moveto(dawnstar_cutter, { 1913.8124, 4435.2495, 7040.7354 }); //+1000 z coordinate so it doesnt block
                                            }
                                                
                                        }
                                    }  
                                    else
                                    {
                                        var_string = "New subquest: " + insert_quest_into_list_and_get_info(var_string);
                                        WalkerProcessor::test_new_very_close_quest();
                                    }
                                        

                                    send_random_context("[" + var_string + "]", false);
                                }


                            }
                        }

                      //_root.HUDMovieBaseInstance.QuestUpdateBaseInstance.objective2.ObjectiveTextFieldInstance.TextFieldInstance.text = Enter the Keep with Hadvar or Ralof
                    }
                    
                    /*
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
                    */
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
                                RE::GFxValue subvar2;
                                if (var1.GetElement(0, &subvar)) //size-1 or 0? idk the order..
                                    if (subvar.GetMember("TextFieldClip", &subvar))
                                        if (subvar.GetMember("tf1", &subvar))
                                            if (subvar.GetMember("text", &subvar2))
                                                if (subvar2.IsString())
                                                {
                                                    std::string result_string = subvar2.GetString();

                                                    if (result_string.find("No direct path seen") != std::string::npos)
                                                    {
                                                        subvar2.SetString("");
                                                        subvar.SetMember("text", subvar2);
                                                        return;
                                                    }

                                                    if (result_string != old_topleft_notification)
                                                    {
                                                        old_topleft_notification = result_string;
                                                        if (result_string != "Autosaving..." && result_string != "Quicksaving..." && result_string != "Quickloading..." && result_string.find("is too powerful") == std::string::npos)
                                                        {

                                                            if (result_string.find("You must raise the bar to open this door.") != std::string::npos)
                                                            {
                                                                WalkerProcessor::reset_walker();
                                                                Observer::detect_interesting_objects(0.016, true); //these bars are hard to raycast for some reason so just scan around no restrictions
                                                            }

                                                            if (result_string.find("This door is barred from the other side.") != std::string::npos)
                                                            {
                                                                WalkerProcessor::reset_walker();
                                                            }


                                                            if (result_string.find("item has insufficient charge") != std::string::npos)
                                                            {
                                                                auto temp_tesult = MiscThings::use_random_soulgem();

                                                                if (temp_tesult.first)
                                                                    send_random_context(temp_tesult.second);

                                                                return;
                                                            }
                                                                //result_string = "Weapon's enchantment charge depleted. Use soulgems to charge it";

                                                            if (result_string.find("has already caught you") != std::string::npos)
                                                                WalkerProcessor::reset_walker();

                                                            if (WalkerProcessor::is_pickpocketing() && result_string == "This person is busy.")
                                                            {
                                                                WalkerProcessor::reset_walker();

                                                                result_string += " Pickpocketing cancelled";
                                                            }


                                                            if (result_string.find("hands are bound") != std::string::npos)
                                                                if (MiscThings::is_intro())
                                                                    result_string += ". Wait for the game to progress. ";
                                                                else
                                                                {
                                                                    std::string bonus = ". You probably need to follow some quest right now. ";
                                                                    auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
                                                                    if (threshold_quest)
                                                                        if (threshold_quest->GetCurrentStageID() > 200)
                                                                            bonus = ". You need someone to help you untie your hands. Try walking to someone. ";

                                                                    result_string += bonus;
                                                                }
                                                                   
                                                            bool silent = false;

                                                            if (result_string.find("Critical Strike on") != std::string::npos)
                                                                silent = true;


                                                            if (result_string.find("You need the Diamond Claw to activate the keyhole") != std::string::npos)
                                                            {
                                                                result_string += " You see something shiny in the pockets of that draugr neaby...";
                                                            }


                                                            send_random_context("[" + result_string + "]", silent);
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


    RE::TESObjectREFR::InventoryItemMap get_filtered_inventory()
    {
        RE::TESObjectREFR::InventoryItemMap inventory = RE::PlayerCharacter::GetSingleton()->GetInventory([](RE::TESBoundObject& a_object)
            {
                if (a_object.IsKey())
                    return false;


                return true;// a_object.IsObject();
            });

        return inventory;
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
        //mouse_mouse_x_y(0, 0);
        mouse_look(0, 0);

        //clear_input_queue();
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


        //result = result * cos(angleX) + result.Cross(axisX) * sin(angleX) + axisX * (axisX * result) * (1 - cos(angleX));
        //result = result * cos(angleY) + result.Cross(axisY) * sin(angleY) + axisY * (axisY * result) * (1 - cos(angleY));
        //result = result * cos(angleZ) + result.Cross(axisZ) * sin(angleZ) + axisZ * (axisZ * result) * (1 - cos(angleZ));

        //has to be like this
        result = result * cos(angleZ) + result.Cross(axisZ) * sin(angleZ) + axisZ * (axisZ * result) * (1 - cos(angleZ));
        result = result * cos(angleY) + result.Cross(axisY) * sin(angleY) + axisY * (axisY * result) * (1 - cos(angleY));
        result = result * cos(angleX) + result.Cross(axisX) * sin(angleX) + axisX * (axisX * result) * (1 - cos(angleX));

        return result;
    }


    RE::NiPoint3 rotate_vector_by_angles2(RE::NiPoint3 v, RE::NiPoint3 object_angles)
    {
        RE::NiPoint3 result = v;

        float angleX = object_angles.x;
        float angleY = object_angles.y;
        float angleZ = object_angles.z;

        RE::NiPoint3 axisX = { 1.0f, 0.0f, 0.0f };
        RE::NiPoint3 axisY = { 0.0f, 1.0f, 0.0f };
        RE::NiPoint3 axisZ = { 0.0f, 0.0f, 1.0f };

        result = result * cos(angleZ) + result.Cross(axisZ) * sin(angleZ) + axisZ * (axisZ * result) * (1 - cos(angleZ));
        result = result * cos(angleY) + result.Cross(axisY) * sin(angleY) + axisY * (axisY * result) * (1 - cos(angleY));
        result = result * cos(angleX) + result.Cross(axisX) * sin(angleX) + axisX * (axisX * result) * (1 - cos(angleX));
        
        


        return result;
    }



    //pathfinding goes crazy
    RE::NiPoint3 get_walking_point_shift(RE::TESObjectREFR* object)
    {
        RE::NiPoint3 result{};

        if (object)
        {
            auto base_obj = object->GetBaseObject();
            if (base_obj)
            {
                if (base_obj->GetFormType() == RE::FormType::Door)
                {

                    auto furniture = (RE::TESObjectDOOR*)base_obj;
                    RE::NiPoint3 object_angles = object->data.angle;

                    std::string model = furniture->GetModel();
                    if (model.find("DweFacadeLiftLeverLoad") != std::string::npos)
                    {
                        RE::NiPoint3 base_shift_vector = { 0.0f, -600.0f, 60.0f };
                        RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                        result = rotated_shift_vector;
                    }
                }

                if (base_obj->GetFormType() == RE::FormType::Activator)
                {

                    auto activator = (RE::TESObjectACTI*)base_obj;

                    std::string model = activator->GetModel();
                    auto object_angles = object->data.angle;

                    if (model.find("WRTempleTree02Root") != std::string::npos)
                    {
                        RE::NiPoint3 base_shift_vector = { -330.0f, 0.0f, 180.0f };
                        RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                        result = rotated_shift_vector;
                    }
                }
            }
        }

        return result;
    }


    bool weird_close_enough_checks(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto base_obj = object->GetBaseObject();

            auto player = RE::PlayerCharacter::GetSingleton();
            
            if (player)
            {
                auto player_ref = player->AsReference();

                if (player_ref)
                {
                    auto player_pos = player_ref->GetPosition();

                    if (base_obj->GetFormType() == RE::FormType::Activator)
                    {

                        auto static_obj = (RE::TESObjectACTI*)base_obj;

                        std::string model = static_obj->GetModel();

                        if (model.find("WRTempleTree02Root") != std::string::npos)
                        {
                            auto compare_pos = MiscThings::get_looking_point_shift(object, false) + object->GetPosition();


                            auto distance = compare_pos.GetDistance(player_pos);

                            if (distance < 400.0f)
                            {
                                return true;
                            }
                        }
                    }

                    if (is_cave_autoloader_door(object))
                    {
                        //auto pos = object->GetPosition();
                        auto nearest_navmesh_pos = MiscThings::get_nearest_navmesh_node(object);

                        auto distance = player_pos.GetDistance(nearest_navmesh_pos);

                        if (distance < 300.0f)
                            return true;
                    }

                    auto ramp_mechanism = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4ec0b);

                    if (ramp_mechanism && object == ramp_mechanism)
                    {
                        auto shoot_point_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x7038459);

                        if (player->GetDistance(shoot_point_marker) < 300.0f)
                            return true;
                    }
                }
            }
        }

        return false;
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

                    RE::TESObjectREFR* madesi = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x19ddc);
                    auto madesi_ring_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("TG00");

                    if (madesi_ring_quest && madesi && object == madesi)
                    {
                        auto stage2 = madesi_ring_quest->GetCurrentStageID();
                        if (stage2 >= 10 && stage2 < 35)
                            if (actor_obj->GetSitSleepState() == RE::SIT_SLEEP_STATE::kIsSitting)
                            {
                                base_shift_vector = { 70.0f, -100.0f, 0.0f };
                            }
                    }



                    RE::NiPoint3 object_angles = object->data.angle;
                    RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                    result = rotated_shift_vector;
                }
            }
            else
            {
                /* //changed approach
                auto word_of_power = MiscThings::get_word_of_power(object);
                if (word_of_power && word_of_power != (RE::TESObjectREFR*)(-1))
                {
                    auto wall_pos = object->GetPosition();
                    auto word_of_power_pos = word_of_power->GetPosition();

                    RE::NiPoint3 base_shift_vector = word_of_power_pos - wall_pos;
                    result = base_shift_vector;
                    return result;

                }
                */


                auto saartal_fx = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x106bba);

                if (saartal_fx && object == saartal_fx)
                {
                    RE::NiPoint3 object_angles = object->data.angle;
                    RE::NiPoint3 base_shift_vector = { 100.0f, 0.0f, 40.0f };
                    RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                    result = rotated_shift_vector;
                    return result;
                }


                auto odawing_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x46efb);

                if (odawing_marker && object == odawing_marker)
                {
                    auto base_obj = object->GetBaseObject();
                    if (base_obj && base_obj->GetFormType() == RE::FormType::Static)
                    {
                        auto marker_static = (RE::TESObjectSTAT*)base_obj;
                        RE::NiPoint3 object_angles = object->data.angle;
                        RE::NiPoint3 base_shift_vector = { 0.0f, -500.0f, 100.0f };
                        RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                        result = rotated_shift_vector;
                        return result;
                    }

                }


                if (object->IsActor() && is_dragon(object) && !is_enemy_to_actor(object))
                {
                    auto target_actor = (RE::Actor*)object;
                    if (target_actor->currentProcess)
                        if (target_actor->currentProcess->middleHigh)
                            if (target_actor->currentProcess->middleHigh->headNode)
                            {
                                auto target_pos = object->GetPosition();
                                auto head_pos = target_actor->currentProcess->middleHigh->headNode->world.translate;
                                auto lookat_location = head_pos;
                                return lookat_location - target_pos;
                            }
                }




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
                            RE::NiPoint3 base_shift_vector = { 0.0f, 40.0f, 55.0f };
                            //RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 1.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("AlchemyWorkstation") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 10.0f, 60.0f, 75.0f };
                            //RE::NiPoint3 base_shift_vector = { 10.0f, 55.0f, 75.0f };
                            //RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 1.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("EnchantingWorkStation") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 40.0f, 57.5f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }



                        if (model.find("EnchantingWorkbench") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 40.0f, 55.0f };
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

                        if (model.find("CartFurnPassanger01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 50.0f, 90.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("CookingPot") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 50.0f, 45.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                    }


                    if (base_obj->GetFormType() == RE::FormType::Activator)
                    {

                        auto activator = (RE::TESFurniture*)base_obj;
                        RE::NiPoint3 object_angles = object->data.angle;

                        std::string model = activator->GetModel();

                        if (model.find("DoorDeadBolt01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 55.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("MeaderyBrewer01Act") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 300.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("NorSecRmSmDoorSm02") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -100.0f, 0.0f, 140.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("RTMercerRamp01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            //need to replace coordinates completely its just too far away
                            auto actual_position_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xc04b9);
                            if (actual_position_marker)
                            {
                                auto actual_position = actual_position_marker->GetPosition();
                                auto shit_position = object->GetPosition();
                                result = actual_position - shit_position;

                            }
                        }

                        if (model.find("NorPullChain01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 10.0f, -10.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("GenPullChain01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            //RE::NiPoint3 base_shift_vector = { 0.0f, 40.0f, 100.0f };
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, -5.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        //PuzzleDoorSmallWheel02
                        //PuzzleDoorMediumWheel02
                        //PuzzleDoorLargeWheel02

                        if (model.find("PuzzleDoorSmallWheel") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -10.0f, 0.0f, 107.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("PuzzleDoorMediumWheel") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -10.0f, 0.0f, 127.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("PuzzleDoorLargeWheel") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -10.0f, 0.0f, 153.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("PuzzleDoorKeyHole") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -10.0f, 0.0f, 80.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("PuzzleDoorKeyHoleIvory") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -10.0f, 0.0f, 80.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }


                        //NorPullBar01


                        if (model.find("NorPullBar01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::TESObjectREFR* weird_handle = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xe730f);

                            if (object == weird_handle)
                            {
                                RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 1.0f };
                                RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                                result = rotated_shift_vector;
                            }
                            else
                            {
                                RE::NiPoint3 base_shift_vector = { 7.0f, 0.0f, 10.0f };
                                RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                                result = rotated_shift_vector;
                            }


                        }

                        if (model.find("NorLever01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 1.0f, 1.0f, 1.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("DweLever01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 1.0f, 1.0f, 1.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("DweLever02") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 15.0f, 7.5f, 90.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }


                        if (model.find("WindCallerTomb01") != std::string::npos || model.find("WindcallerHornPlacement") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 120.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }


                        if (model.find("PortcullisLarge01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -200.0f, 0.0f, 120.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("FXspiderWebKitDoorSpecial") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 140.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }


                        if (model.find("NorSecRmSmDoorSm01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { -100.0f, 0.0f, 120.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }


                        if (model.find("DweLexiconStand") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 70.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }


                        if (model.find("DweButton01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 70.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("DweAstrolabeHub01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 550.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }


                        if (model.find("DweObservatoryDome") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 550.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }


                        if (model.find("WRTempleTree02Root") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { -330.0f, 0.0f, 180.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }


                    }

                    if (base_obj->GetFormType() == RE::FormType::Door)
                    {

                        auto door = (RE::TESObjectDOOR*)base_obj;
                        RE::NiPoint3 object_angles = object->data.angle;

                        std::string model = door->GetModel();


                        if (model.find("SLGDoor01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, -100.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }


                        if (model.find("ShipAnchorChain01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, -500.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }


                        if (model.find("FarmhouseAnimDoor02") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -50.0f, 0.0f, 100.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("WRPrisonCellFloorGrate01Door") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 1.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("WRCastleDoor01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { -30.0f, 0.0f, -30.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("ImpJailDoor01") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                        {
                            RE::NiPoint3 base_shift_vector = { 95.0f, 0.0f, 110.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("TrapHingeTrigger01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 34.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("MetalCageDoor01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { -12.0f, 0.0f, 0.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("ImpWoodDoorSingle01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 55.0f, 0.0f, 100.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }


                        if (model.find("ImpWoodDoorDoubleLoad01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 250.0f, 100.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("MetalCageLongGate01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { -20.0f, 100.0f, 70.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }


                        if (model.find("RTMainGate01") != std::string::npos || model.find("RiftenDoor01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 100.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        
                        if (model.find("RuinsMediumDoorLoad01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { -150.0f, 0.0f, 100.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("Ruins_LargeDoor01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 180.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("WRDoorMainGate01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 20.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("WRShackDoor01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 1.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("ShipTrapdoor01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 1.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("LoadMarker") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 1.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("WinterholdLDoor01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 120.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        //if (model.find("DwemerLargeDoorLoad01") != std::string::npos)
                        if (model.find("DwemerLargeDoor") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 120.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        //if (model.find("DwemerSmallDoorLoad01") != std::string::npos)
                        if (model.find("DwemerSmallDoor") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 120.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        //if (model.find("DweFacadeLiftLeverLoadUp01") != std::string::npos)
                        if (model.find("DweFacadeLiftLeverLoad") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 1.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("SOVDoorShort") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, -200.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("SOVDoorTall") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, -200.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("MarketStallDoor") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, -45.0f, 20.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("SGateDoor") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 60.0f, -70.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("RRJailGrate01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 160.0f, 6.0f }; //thats a very weird shift
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("RiftenRWHallSewerHole") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 1.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model.find("WinterholdTrapdoor01") != std::string::npos)
                        {
                            RE::NiPoint3 base_shift_vector = { 0.0f, 0.0f, 1.0f };
                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                        }

                        if (model == "AutoLoadMarker01.nif")
                        {
                            auto pos = object->GetPosition();
                            auto nearest_navmesh_pos = MiscThings::get_nearest_navmesh_node(object);

                            if (nearest_navmesh_pos != RE::NiPoint3::Zero())
                                result = nearest_navmesh_pos - pos;
                        }





                        if (result == RE::NiPoint3::Zero())
                        {




                            auto player = RE::PlayerCharacter::GetSingleton();

                            auto player_pos = player->GetPosition();
                            auto object_pos = object->GetPosition();

                            auto pos_dif = player_pos - object_pos;

                            
                            RE::NiPoint3 dbg_point = { 1.0f, 1.0f, 1.0f };

                            if (abs(pos_dif.z) < 40.0f)
                                dbg_point.z = 100.0f;

                            return dbg_point;

                            /*
                            auto bounds_max = object->GetBoundMax(); //looks like this is better than height
                            auto bounds_min = object->GetBoundMin();

                            RE::NiPoint3 bound_dif = bounds_max - bounds_min;

                            auto center_shift = bound_dif / 2.0f;

                            if (bound_dif.z > 130.0f)
                                bound_dif.z = 130.0f;

                            RE::NiPoint3 base_shift_vector = center_shift;

                            RE::NiPoint3 object_angles = object->data.angle;//{ 0.0f, 0.0f, 0.0f };

                            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
                            result = rotated_shift_vector;
                            */
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



        /* //this doesnt work and ruins many things instead.
        if (result == RE::NiPoint3::Zero())
        {
            auto bounds_max = object->GetBoundMax(); //looks like this is better than height
            auto bounds_min = object->GetBoundMin();

            RE::NiPoint3 bound_dif = bounds_max - bounds_min;

            auto center_shift = bound_dif / 2.0f;

            RE::NiPoint3 base_shift_vector = center_shift;

            RE::NiPoint3 object_angles = object->data.angle;//{ 0.0f, 0.0f, 0.0f };

            RE::NiPoint3 rotated_shift_vector = rotate_vector_by_angles(base_shift_vector, object_angles);
            result = rotated_shift_vector;

        }
        */


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


    RE::TESObjectREFR* get_alias_ref_by_name(RE::TESQuest* quest, std::string name)
    {

        if (quest && quest->GetFormType() == RE::FormType::Quest)
        {
            for (auto alias : quest->aliases)
            {
                if (alias)
                {
                    std::string this_name = "";
                    this_name = alias->aliasName;

                    if (this_name == name)
                    {
                        auto ref_handle = quest->GetAliasedRef(alias->aliasID);

                        if (ref_handle && ref_handle.get() && ref_handle.get().get())
                            return ref_handle.get().get();
                    }
                }
            }
        }

        return nullptr;
    }


    std::string get_alias_name_by_id(RE::TESQuest* quest, int alias_id)
    {
        for (auto alias : quest->aliases)
        {
            if (alias && alias->aliasID == alias_id)
            {
                std::string result = "";
                result = alias->aliasName;
            }
        }

        return "";
    }



    std::string remove_aliases(std::string displaytext)
    {
        if (displaytext.find("<Alias=") != std::string::npos)
        {
            //<Alias=RiverwoodFriend>
            auto alias_start = displaytext.find("<Alias=");
            auto alias_end = displaytext.find(">");
            if (alias_start < alias_end)
            {
                displaytext = displaytext.substr(0, alias_start) + displaytext.substr(alias_end + 1, displaytext.length() - alias_end);
                displaytext = remove_aliases(displaytext); //if there are more
            }
        }
        return displaytext;
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
                            std::string type_name = alias->GetTypeString().c_str();

                            if (type_name == "Loc")
                            {

                                if (alias->flags.any(RE::BGSBaseAlias::FLAGS::kStoreName))
                                {
                                    //???

                                    //auto object_p = General::Script::GetObject(quest, "TrapTriggerHinge");

                                    //RE::BSScript::IForEachScriptObjectFunctor* my_functor = &my_functor_idk;


                                    //General::Script::ForEachScript(quest, 
                                     //   [&]();
                                }

                                auto loc_alias = (RE::BGSLocAlias*)alias;
                                RE::TESObjectREFR* the_ref = nullptr;
                                const auto     owner = loc_alias->owningQuest;
                                if (owner) {
                                    auto       handle = owner->GetAliasedRef(alias->aliasID);
                                    const auto refPtr = handle.get();
                                    the_ref = refPtr.get();
                                }

                                if (the_ref)
                                {
                                    std::string ref_name = the_ref->GetDisplayFullName();
                                    displaytext = displaytext.substr(0, alias_start) + displaytext.substr(alias_end + 1, displaytext.length() - alias_end);
                                    displaytext.insert(alias_start, ref_name);

                                    displaytext = replace_aliases(quest, displaytext); //if there are more
                                }
                                else
                                {
                                    //just erase it idk what to do with it
                                    displaytext = displaytext.substr(0, alias_start) + displaytext.substr(alias_end + 1, displaytext.length() - alias_end);
                                    displaytext = replace_aliases(quest, displaytext); //if there are more
                                }
                            }

                            if (type_name == "Ref")
                            {
                                auto ref_alias = (RE::BGSRefAlias*)alias;
                                auto the_ref = ref_alias->GetReference();
                                if (the_ref)
                                {
                                    std::string ref_name = the_ref->GetDisplayFullName();
                                    displaytext = displaytext.substr(0, alias_start) + displaytext.substr(alias_end + 1, displaytext.length() - alias_end);
                                    displaytext.insert(alias_start, ref_name);

                                    displaytext = replace_aliases(quest, displaytext); //if there are more
                                }
                                else
                                {
                                    displaytext = displaytext.substr(0, alias_start) + displaytext.substr(alias_end + 1, displaytext.length() - alias_end);
                                    displaytext = replace_aliases(quest, displaytext); //if there are more
                                }
                            }

                            break;
                        }
                    }
                }
            }
        }

        if (displaytext.find("<Alias=") != std::string::npos)
        {
            displaytext = remove_aliases(displaytext); //clear strays if they werent replaced
        }


        return displaytext;
    }




    std::string replace_aliases_all_quests(std::string displaytext)
    {
        if (displaytext.find("<Alias=") != std::string::npos)
        {
            auto player = RE::PlayerCharacter::GetSingleton();
            auto quest_targets = player->questTargets;

            for (auto quest_target : quest_targets)
                displaytext = replace_aliases(quest_target.first, displaytext); //if there are more
        }

        return displaytext;
    }


    bool recursive_quest_condition_check(RE::TESConditionItem* condition, RE::TESQuest* quest, RE::TESQuestTarget* target)
    {
        if (condition)
        {
            auto params = RE::ConditionCheckParams::ConditionCheckParams(nullptr, nullptr);

            auto player_ref = RE::PlayerCharacter::GetSingleton()->AsReference();



            int run_on_who_flag = condition->data.object.underlying();

            if (run_on_who_flag == (int)RE::CONDITIONITEMOBJECT::kSelf)
            {
                RE::ObjectRefHandle quest_ref_handle{};
                target->GetTargetRef(quest_ref_handle, false, quest); //no tracked - try actual target

                if (quest_ref_handle && quest_ref_handle.get() && quest_ref_handle.get().get())
                params.targetRef = quest_ref_handle.get().get();
            }

            if (run_on_who_flag == (int)RE::CONDITIONITEMOBJECT::kRef)
            {
                if (condition->data.runOnRef && condition->data.runOnRef.get() && condition->data.runOnRef.get().get())
                    params.targetRef == condition->data.runOnRef.get().get();
            }

            if (run_on_who_flag == (int)RE::CONDITIONITEMOBJECT::kQuestAlias)
            {
                int alias_id = condition->data.dataID;

                if (quest->refAliasMap.find(alias_id) != quest->refAliasMap.end())
                {
                    auto alias_handle = quest->refAliasMap.find(alias_id)->second;

                    if (alias_handle && alias_handle.get() && alias_handle.get().get())
                    params.targetRef = alias_handle.get().get();
                } 
            }

            //params.targetRef = player_ref;

            params.quest = quest;

            params.actionRef = params.targetRef;

            if (condition->IsTrue(params))
            {//this one is true
                if (!condition->data.flags.isOR && condition->next)
                    return recursive_quest_condition_check(condition->next, quest, target); //if its not OR and have next, check next.
                else
                    return true; //its OR, or there is no next
            }
            else
            {//this one is false
                if (condition->data.flags.isOR && condition->next) //if its OR and we have next, check next.
                    return recursive_quest_condition_check(condition->next, quest, target);
                else
                    return false; //its false, there its not OR with next even if next exists
            }

        }
        else
            return true; //no condition
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
        auto player_quest_targets = player->questTargets;

        auto my_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("myscPath");

        int id = 1;
        bool got_any_quests = false;
        
        std::vector<quest> sortable_quests{};


        //player->objectives

        auto player_objectives = player->objectives;


        std::map<RE::TESQuest*, int> quests_and_objectives{};

        //for (auto player_objective : player_objectives)
        for (auto quest_target : player_quest_targets)
        {

            RE::TESQuest* the_quest = quest_target.first;


            bool found_objective = false;

            for (auto objective : the_quest->objectives)
            {
                if ((the_quest->data.flags.all(RE::QuestFlag::kDisplayedInHUD) || the_quest->data.flags.all(RE::QuestFlag::kEnabled)) && !the_quest->data.flags.all(RE::QuestFlag::kCompleted))
                {
                    if (the_quest != my_quest && !MiscThings::quest_is_hidden(the_quest, objective))
                    {

                        if (objective->state.all(RE::QUEST_OBJECTIVE_STATE::kDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kCompletedDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kFailedDisplayed))
                        //if (objective->state.all(RE::QUEST_OBJECTIVE_STATE::kDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kFailedDisplayed))
                        {

                            if (objective->numTargets != 0)
                            {
                                auto quest_targets = objective->targets;

                                for (auto* target : std::span(quest_targets, objective->numTargets)) {
                                    if (target)
                                    {
                                        if (the_quest)
                                        {
                                            bool conditions_met = false;
                                            //this needs to be figured out
                                            try {
                                                if (target->conditions.head)
                                                {
                                                    auto the_condition = target->conditions.head;

                                                    conditions_met = recursive_quest_condition_check(the_condition, the_quest, target);

                                                    bool test_stop = false;

                                                }
                                                else
                                                    conditions_met = true;

                                            }
                                            catch (...) {
                                                conditions_met = true;//met ?
                                            }

                                            if (conditions_met)
                                            {
                                                quest this_quest{};


                                                if (is_bad_jailquest(the_quest, target))
                                                    continue;

                                                this_quest.id = id;
                                                this_quest.quest = the_quest;
                                                this_quest.name = the_quest->GetFullName();
                                                this_quest.target = target;

                                                std::string displaytext = "";
                                                displaytext = objective->displayText;

                                                std::string target_name = "";
                                                target_name = get_alias_name_by_id(the_quest, target->alias);

                                                this_quest.displaytext += replace_aliases(the_quest, displaytext);


                                                this_quest.displaytext = MiscThings::fix_book_description(this_quest.displaytext);


                                                if (this_quest.name == "Unbound" && target->alias == 124)
                                                    target_name = "Go with Hadvar (Imperials)";

                                                if (this_quest.name == "Unbound" && target->alias == 125)
                                                    target_name = "Go with Ralof (Stormcloaks)";

                                                this_quest.target_name = target_name;

                                                this_quest.objective = objective;
                                                this_quest.description = "";
                                                this_quest.category = 0;


                                                this_quest.estimate_distance = get_quest_target_distance(target, this_quest.quest);

                                                
                                                //if (std::size(target->teleportPath.teleportRefs) > 0)
                                                //{
                                                //    //in different location
                                                //    auto last_teleport_ref = target->teleportPath.teleportRefs.back().ref;
                                                //    this_quest.estimate_distance = player->GetDistance(last_teleport_ref, true, true);
                                                //}
                                                //else
                                                //{
                                                //    this_quest.estimate_distance = 0.0f; //this means we are in the same cell
                                                //}
                                                

                                                sortable_quests.push_back(this_quest);

                                                id++;
                                                got_any_quests = true;


                                                if (quests_and_objectives.find(the_quest) == quests_and_objectives.end())
                                                {
                                                    quests_and_objectives.insert({ the_quest, 0 });
                                                }

                                                found_objective = true;
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                //objective is displayed but has no targets. potentially quest without a target. still add in the list

                                quest this_quest{};

                                this_quest.id = id;
                                this_quest.quest = the_quest;
                                this_quest.name = the_quest->GetFullName();
                                this_quest.target = nullptr;

                                std::string displaytext = "";
                                displaytext = objective->displayText;

                                std::string target_name = "";

                                this_quest.displaytext += replace_aliases(the_quest, displaytext);

                                this_quest.target_name = target_name;

                                this_quest.objective = objective;
                                this_quest.description = "";
                                this_quest.category = 0;

                                this_quest.estimate_distance = 0.0f;

                                sortable_quests.push_back(this_quest);

                                id++;
                                got_any_quests = true;

                                found_objective = true;

                                if (quests_and_objectives.find(the_quest) == quests_and_objectives.end())
                                {
                                    quests_and_objectives.insert({ the_quest, 0 });
                                }
                            }
                        }
                    }
                }
            }

            //all objectives are checked

            if (the_quest && !found_objective)
            {
                if ((the_quest->data.flags.all(RE::QuestFlag::kDisplayedInHUD) || the_quest->data.flags.all(RE::QuestFlag::kEnabled)) && !the_quest->data.flags.all(RE::QuestFlag::kCompleted))
                {
                    //displayed but no objective. still add into list, maybe it just needs to wait or do something else
                    if (the_quest != my_quest && !MiscThings::quest_is_hidden(the_quest))
                    {
                        quest this_quest{};

                        this_quest.id = id;
                        this_quest.quest = the_quest;
                        this_quest.name = the_quest->GetFullName();
                        this_quest.target = nullptr;

                        std::string displaytext = "";
                        std::string target_name = "";

                        this_quest.displaytext += replace_aliases(the_quest, displaytext);

                        this_quest.target_name = target_name;

                        this_quest.objective = nullptr;
                        this_quest.description = "";
                        this_quest.category = 0;

                        this_quest.estimate_distance = 0.0f;

                        sortable_quests.push_back(this_quest);

                        id++;
                        got_any_quests = true;


                        if (quests_and_objectives.find(the_quest) == quests_and_objectives.end())
                        {
                            quests_and_objectives.insert({ the_quest, 0 });
                        }
                    }
                }
            }

        }
        


       
        //for (auto quest_target : player_quest_targets)
        for (auto player_objective : player_objectives)
        {
            auto objective = player_objective.Objective;

            if (objective)
            {

                bool found_objective = false;

                //for (auto objective : the_quest->objectives)


                RE::TESQuest* the_quest = objective->ownerQuest;

                if (quests_and_objectives.find(the_quest) == quests_and_objectives.end())
                {

                    if ((the_quest->data.flags.all(RE::QuestFlag::kDisplayedInHUD) || the_quest->data.flags.all(RE::QuestFlag::kEnabled)) && !the_quest->data.flags.all(RE::QuestFlag::kCompleted))
                    {
                        if (the_quest != my_quest && !MiscThings::quest_is_hidden(the_quest, objective))
                        {

                            if (objective->state.all(RE::QUEST_OBJECTIVE_STATE::kDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kCompletedDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kFailedDisplayed))
                            {

                                if (objective->numTargets != 0)
                                {
                                    auto quest_targets = objective->targets;

                                    for (auto* target : std::span(quest_targets, objective->numTargets)) {
                                        if (target)
                                        {
                                            if (the_quest)
                                            {
                                                bool conditions_met = false;
                                                //this needs to be figured out
                                                try {
                                                    if (target->conditions.head)
                                                    {
                                                        auto the_condition = target->conditions.head;

                                                        conditions_met = recursive_quest_condition_check(the_condition, the_quest, target);

                                                        bool test_stop = false;

                                                    }
                                                    else
                                                        conditions_met = true;

                                                }
                                                catch (...) {
                                                    conditions_met = true;//met ?
                                                }

                                                if (conditions_met)
                                                {
                                                    quest this_quest{};


                                                    if (is_bad_jailquest(the_quest, target))
                                                        continue;

                                                    this_quest.id = id;
                                                    this_quest.quest = the_quest;
                                                    this_quest.name = the_quest->GetFullName();
                                                    this_quest.target = target;

                                                    std::string displaytext = "";
                                                    displaytext = objective->displayText;

                                                    std::string target_name = "";
                                                    target_name = get_alias_name_by_id(the_quest, target->alias);

                                                    this_quest.displaytext += replace_aliases(the_quest, displaytext);

                                                    if (this_quest.name == "Unbound" && target->alias == 124)
                                                        target_name = "Go with Hadvar (Imperials)";

                                                    if (this_quest.name == "Unbound" && target->alias == 125)
                                                        target_name = "Go with Ralof (Stormcloaks)";

                                                    this_quest.target_name = target_name;

                                                    this_quest.objective = objective;
                                                    this_quest.description = "";
                                                    this_quest.category = 0;


                                                    this_quest.estimate_distance = get_quest_target_distance(target, this_quest.quest);

                                                    
                                                    //if (std::size(target->teleportPath.teleportRefs) > 0)
                                                    //{
                                                    //    //in different location
                                                    //    auto last_teleport_ref = target->teleportPath.teleportRefs.back().ref;
                                                    //    this_quest.estimate_distance = player->GetDistance(last_teleport_ref, true, true);
                                                    //}
                                                    //else
                                                    //{
                                                    //    this_quest.estimate_distance = 0.0f; //this means we are in the same cell
                                                    //}
                                                    

                                                    if (quests_and_objectives.find(the_quest) != quests_and_objectives.end())
                                                    {
                                                        quests_and_objectives.find(the_quest)->second++;
                                                    }



                                                    sortable_quests.push_back(this_quest);

                                                    id++;
                                                    got_any_quests = true;

                                                    found_objective = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    //objective is displayed but has no targets. potentially quest without a target. still add in the list

                                    quest this_quest{};

                                    this_quest.id = id;
                                    this_quest.quest = the_quest;
                                    this_quest.name = the_quest->GetFullName();
                                    this_quest.target = nullptr;

                                    std::string displaytext = "";
                                    displaytext = objective->displayText;

                                    std::string target_name = "";

                                    this_quest.displaytext += replace_aliases(the_quest, displaytext);

                                    this_quest.target_name = target_name;

                                    this_quest.objective = objective;
                                    this_quest.description = "";
                                    this_quest.category = 0;

                                    this_quest.estimate_distance = 0.0f;

                                    sortable_quests.push_back(this_quest);

                                    id++;
                                    got_any_quests = true;

                                    found_objective = true;
                                }
                            }
                        }
                    }
                }
            }
        }
           


        //test quests with no objectives
        /*
        for (auto quest_and_objective : quests_and_objectives)
        {
            auto the_quest = quest_and_objective.first;

            if (the_quest && quest_and_objective.second <= 0)
            {
                if ((the_quest->data.flags.all(RE::QuestFlag::kDisplayedInHUD) || the_quest->data.flags.all(RE::QuestFlag::kEnabled)) && !the_quest->data.flags.all(RE::QuestFlag::kCompleted))
                {
                    //displayed but no objective. still add into list, maybe it just needs to wait or do something else

                    quest this_quest{};

                    this_quest.id = id;
                    this_quest.quest = the_quest;
                    this_quest.name = the_quest->GetFullName();
                    this_quest.target = nullptr;

                    std::string displaytext = "";
                    std::string target_name = "";

                    this_quest.displaytext += replace_aliases(the_quest, displaytext);

                    this_quest.target_name = target_name;

                    this_quest.objective = nullptr;
                    this_quest.description = "";
                    this_quest.category = 0;

                    this_quest.estimate_distance = 0.0f;

                    sortable_quests.push_back(this_quest);

                    id++;
                    got_any_quests = true;
                }
            }
        }
        */







        //sort

        std::sort(sortable_quests.begin(), sortable_quests.end(), [&](quest left, quest right) {
            //return left->GetDistance(player) > right->GetDistance(player); //switch > to < for inversed order. this is last->closest
            char priority_left = left.quest->data.priority;
            char priority_right = right.quest->data.priority;

            if (priority_left == priority_right)
            {
                if (left.quest && right.quest)
                {
                    if (left.quest->formID == right.quest->formID)
                    {
                        if (left.target && right.target)
                        {
                            if (left.target->alias == right.target->alias)
                            {
                                if (left.objective && right.objective)
                                {
                                    std::string left_text = "";
                                    left_text = left.objective->displayText;

                                    std::string right_text = "";
                                    right_text = right.objective->displayText;

                                    return left_text < right_text;
                                }
                            }
                            else
                                return left.target->alias < right.target->alias;
                        }
                    }
                    else
                        return left.quest->formID < right.quest->formID;
                }
            }
                
            return priority_left > priority_right;
            });

        //update ids
        for (int i = 1; i <= std::size(sortable_quests); i++)
        {
            sortable_quests.at(i-1).id = i;


            quests.push_back(sortable_quests.at(i - 1));

            auto target_name = sortable_quests.at(i - 1).target_name;

            if (target_name != "")
                target_name = " (" + target_name + ")";


            float distance = MiscThings::get_quest_target_distance(sortable_quests.at(i - 1).target, sortable_quests.at(i - 1).quest);

            
            std::string big_distance = "";//
            
            if (distance > 0.0f)
                big_distance = ". Distance to target : " + std::to_string((int)distance / 100) + " m. ";



            result_text += "[id " + std::to_string(sortable_quests.at(i - 1).id) + "]" + get_quest_type_text(sortable_quests.at(i - 1).quest) + " " + sortable_quests.at(i - 1).name + ": " + sortable_quests.at(i - 1).displaytext + target_name + big_distance;
            result_text += "\n";

        }






        if (!got_any_quests)
        {
            result.first = false;
            result.second = "No active quests found. You can use map to travel to some location (even if it is not discovered yet, you can still travel there by foot), or go to some location nearby. ";
        }
        else
        {
            quest_list_valid = true;
            result.first = true;
            result.second = result_text;
        }

        return result;
    }


    bool have_any_quests()
    {
        auto test_result = get_current_quests();

        return test_result.first;
    }




    std::string get_quest_type_text(RE::TESQuest* quest)
    {
        if (quest)
        {
            if (quest->data.questType == RE::QUEST_DATA::Type::kMainQuest)
                return "[Main Quest]";

            if (quest->data.questType == RE::QUEST_DATA::Type::kSideQuest || quest->data.questType == RE::QUEST_DATA::Type::kMiscellaneous)
                return "[Side Quest]";

            /*
            if (quest->data.questType == RE::QUEST_DATA::Type::kThievesGuild)
                return "[Thieves Guild] ";

            if (quest->data.questType == RE::QUEST_DATA::Type::kDarkBrotherhood)
                return "[Dark Brotherhood] ";


           
            if (quest->data.questType == RE::QUEST_DATA::Type::kMagesGuild)
                return "[Mage College] ";

            if (quest->data.questType == RE::QUEST_DATA::Type::kCivilWar)
                return "[Civil War] ";

            if (quest->data.questType == RE::QUEST_DATA::Type::kCompanionsQuest)
                return "[Companions] ";

            if (quest->data.questType == RE::QUEST_DATA::Type::kDLC02_Dragonborn)
                return "[Dragonborn] ";
            */
        }

        return "";
    }


    bool is_quest_active(RE::TESQuest* quest)
    {
        auto temp_result = get_current_quests();

        if (is_quest_list_valid())
        {
            auto p_quests = get_p_quest_list();

            if (p_quests)
            {
                for (auto quest_entry : *p_quests)
                {
                    if (quest_entry.quest == quest)
                        return true;
                }
            }
        }

        return false;
    }


    std::string insert_quest_into_list_and_get_info(std::string quest_text)
    {
        std::string result = "";// quest_text;

        std::string original_name = quest_text;

        quest_text = lowercase_string(quest_text);

        if (quest_text != "")
        {
            //this is questionable
            bool found = false;
            for (auto quest : quests)
            {
                auto displaytext_lower = lowercase_string(quest.displaytext);
                auto name_lower = lowercase_string(quest.name);

                std::string target_name_local = quest.target_name;
                if (target_name_local != "")
                    target_name_local = " (" + target_name_local + ")";

                if (name_lower == quest_text)
                {
                    result = "[id " + std::to_string(quest.id) + "]" + get_quest_type_text(quest.quest) + " " + quest.name + ": " + quest.displaytext + quest.target_name;
                    found = true;
                    //break;
                }

                if (displaytext_lower == quest_text)
                {
                    result = "[id " + std::to_string(quest.id) + "]" + get_quest_type_text(quest.quest) + " " + quest.name + ": " + quest.displaytext + quest.target_name;
                    found = true;
                    //break;
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

                                if (objective->numTargets != 0)
                                {
                                    auto quest_targets = objective->targets;

                                    for (auto* target : std::span(quest_targets, objective->numTargets)) {
                                        if (target)
                                        {
                                            if (the_quest)
                                            {
                                                bool conditions_met = false;
                                                //this needs to be figured out
                                                try {
                                                    if (target->conditions.head)
                                                    {
                                                        auto the_condition = target->conditions.head;

                                                        conditions_met = recursive_quest_condition_check(the_condition, the_quest, target);

                                                        bool test_stop = false;

                                                    }
                                                    else
                                                        conditions_met = true;

                                                }
                                                catch (...) {
                                                    conditions_met = true;//met ?
                                                }

                                                if (conditions_met)
                                                {
                                                    
                                                    quest this_quest{};

                                                    this_quest.id = std::size(quests) + 1;
                                                    this_quest.quest = the_quest;
                                                    this_quest.name = the_quest->GetFullName();
                                                    this_quest.target = target;

                                                    std::string displaytext = "";
                                                    displaytext = objective->displayText;
                                                    this_quest.displaytext = replace_aliases(the_quest, displaytext);

                                                    std::string target_name = "";
                                                    target_name = get_alias_name_by_id(the_quest, target->alias);

                                                    //this_quest.displaytext = replace_aliases(the_quest, displaytext);

                                                    if (this_quest.name == "Unbound" && target->alias == 124)
                                                        target_name = "Go with Hadvar (Imperials)";

                                                    if (this_quest.name == "Unbound" && target->alias == 125)
                                                        target_name = "Go with Ralof (Stormcloaks)";

                                                    this_quest.objective = objective;
                                                    this_quest.description = "";
                                                    this_quest.category = 0;


                                                    auto displaytext_lower = lowercase_string(this_quest.displaytext);
                                                    auto name_lower = lowercase_string(this_quest.name);

                                                    if (target_name != "")
                                                        target_name = " (" + target_name + ")";

                                                    this_quest.target_name = target_name;

                                                    if (name_lower == quest_text)
                                                    {
                                                        if (result != "")
                                                            result += "; ";

                                                        result += "[id " + std::to_string(this_quest.id) + "]" + get_quest_type_text(this_quest.quest) + " " + this_quest.name;
                                                        quests.push_back(this_quest);
                                                        found = true;

                                                        if (!quest_list_valid)
                                                            quest_list_valid = true;
                                                        //result += "[id " + std::to_string(this_quest.id) + "] " + this_quest.name;

                                                        //break;
                                                    }

                                                    //this one is raw
                                                    std::string objective_displaytext = "";
                                                    objective_displaytext = objective->displayText;
                                                    objective_displaytext = lowercase_string(objective_displaytext);

                                                    if (objective_displaytext == quest_text)
                                                    {
                                                        if (result != "")
                                                            result += "; ";


                                                        result += "[id " + std::to_string(this_quest.id) + "]" + get_quest_type_text(this_quest.quest) + " " + this_quest.name + ": " + this_quest.displaytext + target_name;
                                                        quests.push_back(this_quest);
                                                        found = true;

                                                        if (!quest_list_valid)
                                                        {
                                                            quest_list_valid = true;
                                                            register_quest_actions();
                                                        }
                                                            
                                                        //result = "[id " + std::to_string(this_quest.id) + "] " + this_quest.name + ": " + this_quest.displaytext + target_name;


                                                        //break;
                                                    }

                                                    /*
                                                    if (displaytext_lower == quest_text)
                                                    {
                                                        result = "[id " + std::to_string(this_quest.id) + "] " + this_quest.name + ": " + this_quest.displaytext + target_name;
                                                        quests.push_back(this_quest);
                                                        found = true;

                                                        if (!quest_list_valid)
                                                            quest_list_valid = true;
                                                        result = "[id " + std::to_string(this_quest.id) + "] " + this_quest.name + ": " + this_quest.displaytext + target_name;

                                                        //break;
                                                    }
                                                    */
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (result == "")
            result = original_name;


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

        if (!refr)
            return "";

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




        bool found_any_locations = false;

        for (auto location : locations_around)
        {
            found_any_locations = true;
            std::string location_info = get_location_full_info(location.second);
            if (location_info != "")
                result_text += "[id " + std::to_string(location.first) + "]" + location_info + "\n";
        }


        if (!found_any_locations)
        {
            result.first = true;
            result.second = "No locations around you. ";
        }
        else
        {
            result.first = true;
            result.second = result_text;
        }


        return result;
    }






    bool objects_around_valid = false;
    std::map<int, object_data> objects_around{};


    RE::TESObjectREFR* get_dragon_for_dragonrend()
    {
        RE::TESObjectREFR* result = nullptr;

        RE::TESObjectREFR* parthurnax = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3c57d);

        auto player = RE::PlayerCharacter::GetSingleton();

        if (objects_around_valid && player)
        {

            float min_distance = FLT_MAX;

            for (auto object_around : objects_around)
            {
                auto object = object_around.second.object;

                if (object && object->data.objectReference && object->formID)
                {
                    if (MiscThings::is_dragon(object) && !object->IsDead())
                    {
                        float distance = player->GetDistance(object);

                        if ((distance < min_distance || result == parthurnax) && (!result || object != parthurnax))
                        {
                            min_distance = distance;
                            result = object;
                        }
                    }
                }
            }
        }

        return result;
    }






    void nullify_object_by_id(int id)
    {
        auto object_p = objects_around.find(id);

        if (object_p != objects_around.end())
        {
            object_p->second = { nullptr, "" };
        }
    }



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
                if (list_entry.second.object == object)
                {
                    result = true;
                    break;
                }
            }
        }
        return result;
    }


    std::map<int, object_data>* get_p_objects_around()
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


    std::string get_soul_charge_text(REX::TEnumSet<RE::SOUL_LEVEL> soul)
    {
        int soul_int = (int)soul.underlying();

        if (soul_int == (int)RE::SOUL_LEVEL::kPetty)
            return "Petty";
        if (soul_int == (int)RE::SOUL_LEVEL::kLesser)
            return "Lesser";
        if (soul_int == (int)RE::SOUL_LEVEL::kCommon)
            return "Common";
        if (soul_int == (int)RE::SOUL_LEVEL::kGreater)
            return "Greater";
        if (soul_int == (int)RE::SOUL_LEVEL::kGrand)
            return "Grand";

        return "";
    }


    std::string get_object_category(RE::TESForm* base_obj, RE::TESBoundObject* object)
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
            result = "[Consumable]";

        if (base_type == RE::FormType::SoulGem)
        {
            result = "[Soulgem";

            if (object)
            {
                auto soulgem = (RE::TESSoulGem*)object;
                auto soul = soulgem->currentSoul;

                std::string charge_text = get_soul_charge_text(soul);

                if (charge_text != "")
                {
                    result += ", " + charge_text + " soul";
                }
                else
                {
                    result += ", empty";
                }
            }

            result += "]";

        }
            

        if (base_type == RE::FormType::Note)
            result = "[Note]";

        if (base_type == RE::FormType::Ammo)
            result = "[Ammo]";


        return result;
    }


    bool is_player_hidden()
    {
        if (auto ProcessLists = RE::ProcessLists::GetSingleton())
        {
            std::uint32_t LOSCount{ 1 };
            return !(ProcessLists->RequestHighestDetectionLevelAgainstActor(RE::PlayerCharacter::GetSingleton(), LOSCount) > 0);
        }
        return false;

    }


    bool is_serving_jail()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        if (player)
        {
            auto escaping_jail = player->playerFlags.escaping;
            bool serving_jail = (bool)player->currentPrisonFaction && !escaping_jail && !MiscThings::is_intro() && !MiscThings::is_intro2() && MiscThings::escaped_helgen();

            return serving_jail;
        }
       
        return false;
    }



    bool player_brawling()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        if (player && player->currentProcess)
        {
            auto left_hand = player->currentProcess->equippedObjects[0];
            auto right_hand = player->currentProcess->equippedObjects[1];

            if (left_hand && right_hand)
            {
                if (left_hand->GetFormID() == 0x1f4 && right_hand->GetFormID() == 0x1f4)
                {
                    //both hands are fists
                    return true;
                }
            }
        }

        return false;
    }


    bool is_container_empty(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto base_obj = object->GetBaseObject();

            if (base_obj->GetFormType() == RE::FormType::Container)
            {
                auto container = (RE::TESObjectCONT*)base_obj;

                auto extalist = &object->extraList;

                auto extra_changes = (RE::ExtraContainerChanges*)extalist->GetByType(RE::ExtraDataType::kContainerChanges);

                bool no_changes = true;

                if (extra_changes)
                {
                    if (extra_changes->changes)
                    {
                        //if (extra_changes->changes->changed)
                        {
                            if (extra_changes->changes->entryList)
                            {
                                for (auto entry : *extra_changes->changes->entryList)
                                {
                                    if (entry && entry->countDelta > 0)
                                        return false;
                                }

                                return true;
                            }
                            else
                                return true;
                        }       
                    }
                }

                //if we are here - no changes. check base info

                return container->numContainerObjects == 0;
            }
        }

        return true;
    }


    std::string is_stealing(RE::TESObjectREFR* object)
    {
        std::string result = "";

        if (!object)
            return "";

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        auto base_obj = object->GetBaseObject();
        if (base_obj)
        {
            auto base_type = base_obj->GetFormType();


            if (base_type == RE::FormType::Furniture)
            {
                return "";
            }


            if (base_obj->IsInventoryObject() || base_type == RE::FormType::Tree || base_type == RE::FormType::Flora)
            {
                if (player_actor && player_actor->WouldBeStealing(object) && !object->IsActor())
                    result = "[Is stealing]";
            }
            if (result == "")
            {
                if (base_type == RE::FormType::Activator)
                {
                    auto activator = (RE::TESObjectACTI*)base_obj;
                    std::string model = activator->GetModel();
                    if (model.find("BarenziahsJewelryBox") != std::string::npos)
                    {
                        result = "[Is stealing]";
                    }
                }
                else
                {
                    if (object->IsCrimeToActivate() || player_actor->WouldBeStealing(object))
                    {
                        if (base_type == RE::FormType::Door)
                        {
                            if (is_door_locked(object))
                                result = "[Is crime to break in]";
                        }
                        else
                            if (!object->IsActor() && base_type != RE::FormType::Static)
                                result = "[Is stealing]";

                    }
                }
            }
        }




        return result;
    }




    bool is_carriage_driver(RE::TESObjectREFR* object)
    {
        if (object && object->IsActor())
        {
            auto actor_object = (RE::Actor*)object;

            auto extra = actor_object->extraList.GetByType(RE::ExtraDataType::kAliasInstanceArray);
            if (extra)
            {
                auto extra_alias_array = (RE::ExtraAliasInstanceArray*)extra;

                for (auto extra_alias : extra_alias_array->aliases)
                {
                    if (extra_alias->alias)
                    {
                        std::string alias_name = extra_alias->alias->aliasName.c_str();

                        if (alias_name.find("Driver") != std::string::npos)
                            return true;
                    }
                }
            }
        }

        return false;
    }




    std::string get_door_teleport(RE::TESObjectREFR* object)
    {

        std::string leads_to = "";

        if (object)
        {
            auto extra = object->extraList.GetByType(RE::ExtraDataType::kTeleport);

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
                                {
                                    //worldspace name
                                    auto target_worldspace = teleport_target_refr->GetWorldspace();

                                    if (target_worldspace)
                                    {
                                        std::string worldspace_name = target_worldspace->GetName();

                                        if (worldspace_name == "" || worldspace_name == "Skyrim")
                                            leads_to = "outside";
                                        else
                                            leads_to = worldspace_name;
                                    }
                                    else
                                        leads_to = "outside";

                                }

                            }
                            else
                            {
                                //worldspace name
                                auto target_worldspace = teleport_target_refr->GetWorldspace();

                                if (target_worldspace)
                                {
                                    std::string worldspace_name = target_worldspace->GetName();

                                    if (worldspace_name == "" || worldspace_name == "Skyrim")
                                        leads_to = "outside";
                                    else
                                        leads_to = worldspace_name;
                                }
                                else
                                    leads_to = "outside";
                            }
            }
        }
        


        return leads_to;
    }


    std::string get_object_category(RE::TESObjectREFR* object, bool no_linked_chains)
    {
        if (!object)
            return "";

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
                auto door_object = (RE::TESObjectDOOR*)base_obj;
            

                std::string model = door_object->GetModel();

                if (model.find("TrapHingeTrigger01") != std::string::npos)
                {
                    result = "[Trap hinge";

                    auto extra = object->extraList.GetByType(RE::ExtraDataType::kActivateRef);
                    std::string attatched_to = "";

                    if (extra)
                    {
                        auto extra_activate = (RE::ExtraActivateRef*)extra;
                        
                        for (auto parent : extra_activate->parents)
                        {
                            if (parent && parent->activateRef && parent->activateRef.get() && parent->activateRef.get().get())
                            {
                                result += ", attatched to " + insert_object_into_list_and_get_info(parent->activateRef.get().get());
                            }
                        }
                    }

                    result += "]";

                    /*
                    auto object_p = General::Script::GetObject(object, "TrapTriggerHinge");
                    if (object_p)
                    {
                        RE::BSFixedString prop_name = "trapLinkedRef";
                        RE::TESObjectREFR* linked = General::Script::GetVariable<RE::TESObjectREFR*>(object_p, prop_name);
                        if (linked) //THIS IS WHAT IS GOING TO BE TRIGGERED BY IT. the one who activates this, is in ActivateRef extradata of object
                        {
                            result += ", attatched to " + insert_object_into_list_and_get_info(linked);
                        }

                        if (object_p->currentState == "Triggered")
                        {
                            ;// result = 14; //hinge triggered
                        }

                        if (object_p->currentState == "Disarmed")
                        {
                            ;// result = 15; //hinge disarmed
                        }
                    }
                    */
                    
                }
                else
                {
                    auto extra = object->extraList.GetByType(RE::ExtraDataType::kTeleport);

                    std::string leads_to = get_door_teleport(object);

                    if (leads_to != "")
                        result = "[Door to " + leads_to + "]";
                    else
                        result = "[Door]";
                }

            }
            

            if (base_type == RE::FormType::Activator)
            {
                result = "[Interactive]";

                

                std::vector<std::string> linked_to{};
                std::string linked_to_text = "";

                auto extra = object->extraList.GetByType(RE::ExtraDataType::kLinkedRef);

                if (!no_linked_chains)
                {
                    if (extra)
                    {
                        auto extra_linked = (RE::ExtraLinkedRef*)extra;

                        for (auto linked_ref : extra_linked->linkedRefs)
                        {
                            if (linked_ref.refr)
                            {
                                std::string name_linked = insert_object_into_list_and_get_info(linked_ref.refr, false, true);
                                if (name_linked != "")
                                {
                                    linked_to.push_back(name_linked);
                                }
                            }
                        }
                    }

                    extra = object->extraList.GetByType(RE::ExtraDataType::kActivateRefChildren);

                    if (extra)
                    {
                        auto extra_linked = (RE::ExtraActivateRefChildren*)extra;

                        for (auto linked_ref : extra_linked->children)
                        {
                            if (linked_ref->activateRef && linked_ref->activateRef.get() && linked_ref->activateRef.get().get())
                            {
                                if (std::size(linked_to) == 0)
                                {
                                    std::string name_linked = insert_object_into_list_and_get_info(linked_ref->activateRef.get().get(), false, true);
                                    if (name_linked != "")
                                    {
                                        linked_to.push_back(name_linked);
                                    }
                                }

                            }
                        }
                    }


                    //this gave me link to shit lever and didnt give link to actual lever
                   /*
                   extra = object->extraList.GetByType(RE::ExtraDataType::kActivateRef);
                   if (extra)
                   {
                       auto extra_linked = (RE::ExtraActivateRef*)extra;

                       for (auto linked_ref : extra_linked->parents)
                       {
                           if (linked_ref->activateRef && linked_ref->activateRef.get() && linked_ref->activateRef.get().get())
                           {
                               RE::TESObjectREFR* activate_ref = linked_ref->activateRef.get().get();

                               std::string temp_name = activate_ref->GetDisplayFullName();

                               if (temp_name != "" || is_object_in_the_list(activate_ref))
                               {
                                   std::string name_linked = insert_object_into_list_and_get_info(activate_ref);
                                   if (name_linked != "")
                                   {
                                       linked_to.push_back(name_linked);
                                   }
                               }
                               else
                               {
                                   //maybe its some script marker that has some lever attatched even higher
                                   auto extra2 = activate_ref->extraList.GetByType(RE::ExtraDataType::kActivateRefChildren);

                                   if (extra2)
                                   {
                                       auto extra_linked2 = (RE::ExtraActivateRefChildren*)extra2;

                                       for (auto linked_ref2 : extra_linked2->children)
                                       {
                                           if (linked_ref2->activateRef && linked_ref2->activateRef.get() && linked_ref2->activateRef.get().get())
                                           {
                                               RE::TESObjectREFR* activate_ref2 = linked_ref2->activateRef.get().get();

                                               std::string temp_name2 = activate_ref2->GetDisplayFullName();

                                               if (temp_name2 != "" || is_object_in_the_list(activate_ref2))
                                               {
                                                   std::string name_linked2 = insert_object_into_list_and_get_info(activate_ref2);
                                                   if (name_linked2 != "")
                                                   {
                                                       linked_to.push_back(name_linked2);
                                                   }
                                               }
                                           }
                                       }
                                   }
                               }
                           }
                       }
                   }
                   */


                    if (std::size(linked_to) != 0)
                    {
                        linked_to_text = "[Linked to ";
                        bool first_linked = true;
                        for (auto linked_to_one : linked_to)
                        {
                            if (first_linked)
                                linked_to_text += linked_to_one;
                            else
                                linked_to_text += ", " + linked_to_one;

                            first_linked = false;
                        }
                        linked_to_text += "]";

                        result += linked_to_text;

                    }
                }
                


                std::string generic_solved_text = MiscThings::get_special_text(object);

                if (generic_solved_text != "")
                    result += " " + generic_solved_text;



            }
                

            if (object->IsActor())
            {
                std::string race = "";
                std::string dead = "";
                std::string child = "";
                std::string mount_text = "";
                std::string ghost = "";

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

                if (actor_object->IsGhost())
                {
                    //TODO probably better to filter actual ghosts here
                   // auto ancano = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x1e79d);
                   // ghost = "[Ghost]";
                }
                    


                if (actor_object->IsAMount() && !actor_object->IsDead())
                {
                    RE::ActorPtr mounter = nullptr;
                    bool is_mounted = actor_object->GetMountedBy(mounter);

                    auto player = RE::PlayerCharacter::GetSingleton();
                    RE::BSString result_string = "";

                    RE::TESNPC* player_npc = RE::TESForm::LookupByID(0x7)->As<RE::TESNPC>();
                    player_npc->GetActivateText(actor_object, result_string);


                    bool is_blocked = actor_object->IsActivationBlocked();

                    bool is_crime_to_mount = actor_object->IsCrimeToActivate();


                    if (!is_mounted)
                    {
                        if (!is_blocked)
                        {
                            mount_text = "[Can mount]";
                            if (is_crime_to_mount)
                                mount_text += "[Is crime]";
                        }
                        else
                        {
                            mount_text = "[Cannot mount]";
                        }
                    }
                    else
                    {
                        std::string mounter_name = "";

                        if (mounter && mounter.get())
                            mounter_name = insert_object_into_list_and_get_info(mounter.get());

                        if (mounter_name != "")
                            mounter_name = " by " + mounter_name;
                        else
                            mounter_name = " by someone";

                        mount_text = "[Mounted" + mounter_name + "]";

                    }

                }

                if (actor_object->IsDead())
                    dead = "[DEAD]";
                
                if (actor_object->IsChild())
                    child = ", Child";

                std::string enemy_text = "";

                if (is_enemy_to_actor(actor_object))
                    enemy_text = "[ENEMY]";

                if (object->IsHumanoid())
                {
                    result = dead + "[Person" + race + child + "]";
                }
                else
                    result = dead + "[Creature" + race + "]";



                std::string sitsleep = "";

                if (actor_object->actorState1.sitSleepState == RE::SIT_SLEEP_STATE::kIsSleeping)
                {
                    if (object->IsHumanoid())
                        sitsleep = "[Sleeping]";
                    else
                        sitsleep = "[Dormant]";
                }

                std::string driver_text = "";

                auto extra = actor_object->extraList.GetByType(RE::ExtraDataType::kAliasInstanceArray);
                if (extra)
                {
                    auto extra_alias_array = (RE::ExtraAliasInstanceArray*)extra;

                    for (auto extra_alias : extra_alias_array->aliases)
                    {
                        if (extra_alias->alias)
                        {
                            std::string alias_name = extra_alias->alias->aliasName.c_str();

                            if (alias_name.find("Driver") != std::string::npos)
                                driver_text = "[Carriage driver]";
                        }
                    }
                }

                //if (actor_object->actorState1.sitSleepState == RE::SIT_SLEEP_STATE::kIsSitting)
                //    sitsleep = "[Sitting]";



                result += mount_text;
                result += sitsleep;
                result += enemy_text;
                result += ghost;
                result += driver_text;
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
                        result = "[Interactive]";
                    else
                    {
                        std::string name_furniture = object->GetDisplayFullName();
                        if (name_furniture != "Bed")
                            result = "[Furniture]";
                        else
                        {
                            if (furniture->furnFlags.any(RE::TESFurniture::ActiveMarker::kCanSleep))
                            {
                                std::string owner_text = "";

                                auto owner_extra = (RE::ExtraOwnership*)object->extraList.GetByType(RE::ExtraDataType::kOwnership);

                                if (owner_extra)
                                {
                                    if (owner_extra->owner)
                                    {
                                        if (owner_extra->owner->GetFormID() == 0x7)
                                            owner_text = "[Yours]";
                                        else
                                        {
                                            if (owner_extra->owner->GetFormID() != 0x0)
                                                owner_text = "[Not yours]";
                                        }
                                    }
                                }

                                result = "[Furniture, Bed]" + owner_text;
                            }
                            else
                                result = "[Furniture]";
                        }
                    }
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
                result = "[Consumable]";

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

        if (!refr)
            return "";

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


                auto armor_val = armor->armorRating / 100;

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



    std::string get_object_full_info(RE::TESObjectREFR* refr, bool no_linked_chains)
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


            if (name == "Broken Shackle") //broken riften jail escape
                return "";

            if (name == "")
                return "";


            auto base_obj = refr->GetBaseObject();
            auto base_type = base_obj->GetFormType();

            if (base_type == RE::FormType::Container)
            {
                auto door_obj = (RE::TESObjectCONT*)base_obj;
                std::string model = door_obj->GetModel();

                if (model.find("Ruins_LargeChest") != std::string::npos)
                {
                    name = "Large Treasure Chest";
                }
            }

            if (base_type == RE::FormType::Container && name == "Draugr")
            {
                name = "Mummified draugr";
            }

            std::string category = get_object_category(refr, no_linked_chains);
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
                if (object_entry.second.object == refr)
                {
                    result = "[id " + std::to_string(object_entry.first) + "]" + get_object_stateless_info(refr);
                    break;
                }
            }
        }

        return result;
    }


    std::string insert_object_into_list_custom_name(std::string name, RE::TESObjectREFR* refr)
    {
        std::string result = "";

        //name = get_object_category(refr) + " " + name;


        if (refr)
        {
            bool found = false;
            for (auto object_entry : objects_around)
            {
                if (object_entry.second.object == refr)
                {
                    result = "[id " + std::to_string(object_entry.first) + "] " + name;
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                int new_id = std::size(objects_around);

                objects_around.insert({ new_id, {refr, name} });
                if (!objects_around_valid)
                    objects_around_valid = true;
                result = "[id " + std::to_string(new_id) + "] " + name;
            }
        }

        return result;
    }



    std::string insert_object_into_list_and_get_info(RE::TESObjectREFR* refr, bool no_chains, bool no_linked_chains)
    {
        std::string result = "";

        if (refr)
        {
            if (!no_chains)
                chain_insert_twin_object(refr);


            auto base_obj = refr->GetBaseObject();
            auto base_type = base_obj->GetFormType();

            if (base_type == RE::FormType::Activator)
            {
                auto static_obj = (RE::TESObjectACTI*)base_obj;

                std::string model = static_obj->GetModel();

                if (model.find("FXspiderWebKitDoorSpecial") != std::string::npos)
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("[Destructible] Cobweb", refr);

                    result = name;
                }

                if (model.find("StockadeBarricade") != std::string::npos)
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("[Destructible] Barricade", refr);

                    result = name;
                }

                if (model.find("TG01DwemerUrn") != std::string::npos)
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("[Destructible] Dwemer Urn", refr);

                    result = name;
                }



                if (model.find("PuzzleDoorKeyHole") != std::string::npos && model.find("PuzzleDoorKeyHoleIvory") == std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("[Puzzle door] Ancient Nordic Door", refr);
                    result = name;
                }

            }

            if (base_type == RE::FormType::Door)// && a_ref->GetDisplayFullName() == "")
            {
                auto door = (RE::TESObjectDOOR*)base_obj;
                std::string model = door->GetModel();

                if (model.find("CaveGSecretDoor") != std::string::npos)
                {
                    std::string name = MiscThings::insert_object_into_list_custom_name("[Secret door] Stone wall door", refr);
                    result = name;
                }
            }


            RE::ExtraDataList* extralist = &refr->extraList;
            auto extra_anim = extralist->GetByType(RE::ExtraDataType::kAnimGraphManager);
            if (extra_anim)
            {
                auto extra_anim_graph = (RE::ExtraAnimGraphManager*)extra_anim;
                if (extra_anim_graph->animGraphMgr)
                {
                    if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "NorRetractableBridge01")
                    {
                        std::string name = MiscThings::insert_object_into_list_custom_name("Large wooden bridge", refr);
                        result = name;
                    }

                    if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "ImpPortcullisSmall01")
                    {
                        std::string name = MiscThings::insert_object_into_list_custom_name("Heavy wooden gate", refr);
                        result = name;
                    }

                    if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "PortcullisLarge01")
                    {
                        std::string name = MiscThings::insert_object_into_list_custom_name("Metal gate", refr);
                        result = name;
                    }

                    if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "NorPortcullisGate01")
                    {
                        std::string name = MiscThings::insert_object_into_list_custom_name("Small metal gate", refr);
                        result = name;
                    }
                }
            }












            bool found = false;
            for (auto object_entry : objects_around)
            {
                if (object_entry.second.object == refr)
                {
                    if (object_entry.second.custom_name != "")
                    {
                        result = "[id " + std::to_string(object_entry.first) + "]" + object_entry.second.custom_name;
                    }
                    else
                    {
                        std::string info = get_object_full_info(refr, no_linked_chains);
                        if (info != "")
                            result = "[id " + std::to_string(object_entry.first) + "]" + get_object_full_info(refr, no_linked_chains);

                    }

                    found = true;
                    break;
                }
            }
        
            if (!found && is_new_object_valid(refr))
            {
                int new_id = std::size(objects_around);

                objects_around.insert({ new_id, {refr, ""} });
                if (!objects_around_valid)
                    objects_around_valid = true;

                std::string info = get_object_full_info(refr, no_linked_chains);
                if (info != "")
                    result = "[id " + std::to_string(new_id) + "]" + get_object_full_info(refr, no_linked_chains);

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


    std::map<int, item_data> inventory_items_list{};


    std::map<int, item_data>* get_p_inventory_items_list()
    {
        return &inventory_items_list;
    }




    RE::BGSEquipSlot* get_weapon_slot(RE::TESBoundObject* item)
    {
        if (item->IsArmor())
        {
            auto armor = (RE::TESObjectARMO*)item;

            return armor->GetEquipSlot();
        }
        else
        {
            if (item->IsWeapon())
            {
                auto weapon = (RE::TESObjectWEAP*)item;

                return weapon->GetEquipSlot();
            }
        }

        return nullptr;
    }


    RE::BGSBipedObjectForm::BipedObjectSlot get_armor_slot(RE::TESBoundObject* item)
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto actor_equip = RE::ActorEquipManager::GetSingleton();


        if (item && player && actor_equip)
        {
            //apparel
            if (item->IsArmor())
            {
                auto armor = (RE::TESObjectARMO*)item;

                auto slot = armor->GetSlotMask();

                return slot;
            }
        }
        return (RE::BGSBipedObjectForm::BipedObjectSlot)0;
    }



    RE::TESForm* get_weapon_slot_contents(RE::BGSEquipSlot* slot)
    {

        if (slot)
        {
            auto player = RE::PlayerCharacter::GetSingleton();

            if (player)
                return player->GetEquippedObjectInSlot(slot);
        }

        return nullptr;
    }


    RE::TESForm* get_armor_slot_contents(RE::BGSBipedObjectForm::BipedObjectSlot slot)
    {

        if ((int)slot)
        {
            auto player = RE::PlayerCharacter::GetSingleton();

            if (player)
                return player->GetWornArmor(slot);
        }

        return nullptr;
    }




    float armor_damage_difference(RE::TESBoundObject* item)
    {
        if (item)
        {
            if (item->IsArmor())
            {
                auto slot = get_armor_slot(item);

                if ((int)slot != -1)
                {
                    auto new_armor = (RE::TESObjectARMO*)item;

                    auto new_armor_val = new_armor->GetArmorRating();

                    float current_armor_val = 0.0f;

                    auto current_armor = (RE::TESObjectARMO*)get_armor_slot_contents(slot);

                    if (current_armor)
                    {
                        if (current_armor->IsArmor())
                        {
                            current_armor_val = current_armor->GetArmorRating();
                        }
                    }

                    if (slot == RE::BGSBipedObjectForm::BipedObjectSlot::kShield && (!current_armor || has_spell_equipped(false)))
                    {
                        return 0; //dont advice shield if we dont have shield in left or if we have spell there
                    }

                    return new_armor_val - current_armor_val;
                }
            }
            else
            {
                if (item->IsWeapon())
                {
                    auto slot = get_weapon_slot(item);

                    if (slot)
                    {
                        auto new_weapon = (RE::TESObjectWEAP*)item;

                        auto new_weapon_damage = new_weapon->GetAttackDamage();

                        if (new_weapon->IsTwoHandedAxe() || new_weapon->IsTwoHandedSword() || new_weapon->IsBow() || new_weapon->IsCrossbow())
                            new_weapon_damage /= 2.0f;


                        float current_weapon_damage = 0.0f;

                        auto current_weapon = (RE::TESObjectWEAP*)get_weapon_slot_contents(slot);

                        if (current_weapon)
                        {
                            auto right_slot = (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F42);
                            auto left_slot = (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F43);


                            if (current_weapon->IsStaff())
                            {
                                bool is_right_slot = slot == right_slot;

                                if (!WalkerProcessor::no_charge(is_right_slot))
                                    return 0; //dont advise to replace charged staves (might be a bad idea to do this...)

                            }

                            if (current_weapon->IsWeapon())
                            {
                                current_weapon_damage = current_weapon->GetAttackDamage();

                                if (current_weapon->IsTwoHandedAxe() || current_weapon->IsTwoHandedSword() || current_weapon->IsBow() || current_weapon->IsCrossbow())
                                    current_weapon_damage /= 2.0f;
                            }
                            else
                            {


                                if (slot == right_slot && has_spell_equipped(true))
                                    return 0;

                                if (slot == left_slot && has_spell_equipped(false))
                                    return 0;
                            }
                        }

                        return new_weapon_damage - current_weapon_damage;
                    }
                }
            }
        }

        return 0;
    }




    std::string get_best_weapon_for_slot(RE::BGSEquipSlot* slot)
    {
        if (slot)
        {
            if (!inventory_valid)
                auto temp = GetInventory();

            if (!inventory_valid)
                return "";

            auto p_inventory = get_p_inventory_items_list();

            std::map<RE::BGSEquipSlot*, std::pair<std::string, float>> best_weapon{};

            for (auto inventory_entry : *p_inventory)
            {
                if (inventory_entry.second.amount > 0 && inventory_entry.second.object)
                {
                    if (inventory_entry.second.object->IsWeapon())
                    {
                        float benefit = armor_damage_difference(inventory_entry.second.object);

                        if (benefit > 2.0f)
                        {
                            auto this_slot = get_weapon_slot(inventory_entry.second.object);

                            if (this_slot)
                            {
                                auto current_best = best_weapon.find(this_slot);

                                std::string item_info = insert_item_into_inventory_list_and_get_info(inventory_entry.second.object, true);

                                if (current_best == best_weapon.end())
                                    best_weapon.insert({ this_slot, {item_info, benefit} });
                                else
                                {
                                    float current_benefit = current_best->second.second;

                                    if (benefit > current_benefit)
                                    {
                                        current_best->second.first = item_info;
                                        current_best->second.second = benefit;
                                    }
                                }
                            }
                        }
                    }
                    
                }
            }


            auto best_item = best_weapon.find(slot);

            if (best_item != best_weapon.end())
                return best_item->second.first;
        }
        return "";
    }


    std::string get_best_armor_for_slot(RE::BGSBipedObjectForm::BipedObjectSlot slot)
    {
        if ((int)slot)
        {
            if (!inventory_valid)
                auto temp = GetInventory();

            if (!inventory_valid)
                return "";

            auto p_inventory = get_p_inventory_items_list();

            std::map<RE::BGSBipedObjectForm::BipedObjectSlot, std::pair<std::string, float>> best_armor{};

            for (auto inventory_entry : *p_inventory)
            {
                if (inventory_entry.second.amount > 0 && inventory_entry.second.object)
                {

                    float benefit = armor_damage_difference(inventory_entry.second.object);

                    if (benefit > 2.0f)
                    {
                        auto this_slot = get_armor_slot(inventory_entry.second.object);

                        if ((int)this_slot)
                        {
                            auto current_best = best_armor.find(this_slot);

                            std::string item_info = insert_item_into_inventory_list_and_get_info(inventory_entry.second.object, true);

                            if (current_best == best_armor.end())
                                best_armor.insert({ this_slot, {item_info, benefit} });
                            else
                            {
                                float current_benefit = current_best->second.second;

                                if (benefit > current_benefit)
                                {
                                    current_best->second.first = item_info;
                                    current_best->second.second = benefit;
                                }
                            }

                        }
                    }
                }
            }


            auto best_item = best_armor.find(slot);

            if (best_item != best_armor.end())
                return best_item->second.first;
        }
        return "";
    }






    std::string get_best_items_list()
    {

        std::string result = "";

        auto player = RE::PlayerCharacter::GetSingleton();
        auto actor_equip = RE::ActorEquipManager::GetSingleton();


        std::vector<std::string> best_items{};

        if (player && actor_equip && player->currentProcess)
        {

            //weapons
            auto actor_process = player->currentProcess;
            auto equipped_list = actor_process->equippedForms;

            std::string weapon_slot_last_info = "";
            for (auto equipped_item : equipped_list)
            {
                std::string best_weapon_for_slot = get_best_weapon_for_slot(equipped_item.slot);

                if (best_weapon_for_slot != weapon_slot_last_info)
                {
                    best_items.push_back(best_weapon_for_slot);
                }
                weapon_slot_last_info = best_weapon_for_slot;
            }


            //apparel

            if (!inventory_valid)
                auto temp = GetInventory();

            if (!inventory_valid)
                return "";

            auto p_inventory = get_p_inventory_items_list();



            std::map<RE::BGSBipedObjectForm::BipedObjectSlot, std::pair<RE::TESBoundObject*, float>> best_armor{};


            for (auto inventory_entry : *p_inventory)
            {
                if (inventory_entry.second.amount > 0 && inventory_entry.second.object)
                {

                    float benefit = armor_damage_difference(inventory_entry.second.object);

                    if (benefit > 2.0f)
                    {
                        auto this_slot = get_armor_slot(inventory_entry.second.object);

                        if ((int)this_slot)
                        {
                            auto current_best = best_armor.find(this_slot);

                            if (current_best == best_armor.end())
                                best_armor.insert({ this_slot, {inventory_entry.second.object, benefit} });
                            else
                            {
                                float current_benefit = current_best->second.second;

                                if (benefit > current_benefit)
                                {
                                    current_best->second.first = inventory_entry.second.object;
                                    current_best->second.second = benefit;
                                }
                            }
                        }
                    }
                }
            }

            for (auto best_gear_piece : best_armor)
            {
                std::string item_info = insert_item_into_inventory_list_and_get_info(best_gear_piece.second.first, true);

                if (item_info != "")
                    best_items.push_back(item_info);
            }

            bool first = true;
            for (auto best_item : best_items)
            {
                if (best_item != "")
                {
                    if (!first)
                        result += "; " + best_item;
                    else
                        result += best_item;

                    first = false;
                }
            }

        }
        return result;
    }











    void reset_misc()
    {
    
        //quest_list_valid = false; //they usually stay the same after saveload
        //quests.clear();

        //inventory_valid = false;
        //inventory.clear();
        //items_list.clear();

        threw_a_book_out_to_read = false;

        gave_interesting_notification_timestamp = 0;

        locations_around.clear();
        locations_around_valid = false;

        old_quest_notification = "";
        old_topleft_notification = "";
        //old_subquest_notification = "";
        //old_subquest_notification1 = "";
        old_subquest_notification_vector.clear();
    }


    bool is_equippable(RE::TESBoundObject* object)
    {
        bool result = false;
        auto item_form = (RE::TESForm*)object;
        std::string actions = "";

        if (item_form)
            if (item_form->IsArmor() || item_form->IsWeapon() || item_form->IsAmmo() || item_form->GetFormType() == RE::FormType::Scroll)
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


    //std::string get_hands_equipped_text()
    //{
    //
    //}



    bool has_something_equipped(bool right)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player && player->currentProcess)
        {
            auto left_hand = player->currentProcess->equippedObjects[0];
            auto right_hand = player->currentProcess->equippedObjects[1];

            if (right)
            {
                if (right_hand)
                    result = true;
            }
            else
            {
                if (left_hand)
                    result = true;
            }

        }
        return result;
    }



    bool has_spell_equipped(bool right)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            RE::MagicItem* spell = (RE::MagicItem*)MiscThings::get_hand_contents(right);

            if (spell && (spell->GetFormType() == RE::FormType::Spell || spell->GetFormType() == RE::FormType::Scroll))
                if (spell->GetSpellType() != RE::MagicSystem::SpellType::kEnchantment)
                    result = true;

        }
        return result;
    }



    bool last_free_was_left = false;

    RE::BGSEquipSlot* get_free_slot(bool offensive = true)
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto actor_equip = RE::ActorEquipManager::GetSingleton();


        auto right_slot = (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F42);
        auto left_slot = (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F43);

        if (player && actor_equip && player->currentProcess)
        {
            //weapons
            auto actor_process = player->currentProcess;
            auto equipped_list = actor_process->equippedObjects;

            if (!equipped_list[1])
                return right_slot; //right hand

            if (!equipped_list[0])
                return left_slot; //left hand

            //both are busy

            last_free_was_left = !last_free_was_left;

            //it cant be used like this - then if non offensive spell is in left hand - cant equip anything else in that slot
            //if (offensive && has_spell_equipped(false) && !is_offensive_spell(false)) //if this one is offensive and we have non offensive spell in left hand - 
            //    return (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F42);


            if (last_free_was_left)
                return right_slot;
            else
                return left_slot;
                

            
        }

        return nullptr;
    }






    RE::BGSEquipSlot* get_weapon_equipped_slot(RE::TESBoundObject* object)
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto actor_equip = RE::ActorEquipManager::GetSingleton();


        if (object && player && actor_equip && player->currentProcess)
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


        if (object && player && actor_equip && player->currentProcess)
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


        if (object && player && actor_equip && player->currentProcess)
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

    bool is_spell_equipped(bool right, RE::MagicItem* test_spell)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player && player->currentProcess)
        {
            RE::MagicItem* spell;

            auto actor_process = player->currentProcess;
            auto equipped_list = actor_process->equippedObjects;

            if (right)
                spell = (RE::MagicItem*)equipped_list[1];
            else
                spell = (RE::MagicItem*)equipped_list[0];

            if (spell)
                result = spell == test_spell;
        }

        return result;
    }

    RE::TESObjectBOOK* book_to_activate = nullptr;
    float book_activate_time = 0.0f;


    /*
    void delayed_activate_book(RE::TESObjectBOOK* book)
    {
        book_to_activate = book;
        book_activate_time = 0.0f;
    }
    */
    


    bool has_thrown_a_book()
    {
        return threw_a_book_out_to_read;
    }

    //replaced with proper reading
    void book_reader(float dtime)
    {
        if (threw_a_book_out_to_read)
        {
            if (book_activate_time > 2.0f)
            {
                threw_a_book_out_to_read = false;
                book_activate_time = 0.0f;
            }
            else
                book_activate_time += dtime;
        }
        else
            book_activate_time = 0.0f;

        /*
        if (book_to_activate)
        {
            if (book_activate_time > 2.0f)
            {
                auto player_ref = RE::PlayerCharacter::GetSingleton()->AsReference();
                //book_to_activate->Activate(player_ref, player_ref, 0, book_to_activate, 1);
                auto book_ref = (RE::TESObjectREFR*)book_to_activate;
                auto player = RE::PlayerCharacter::GetSingleton();
                //player->Activate()
                player->Activate(book_ref, book_ref, 0, book_to_activate, 1);
                RE::TESNPC* player_npc = (RE::TESNPC*)RE::TESForm::LookupByID(0x7); //left hand

                player_npc->Activate(book_ref, book_ref, 0, book_to_activate, 1);
                player_npc->Activate(player_ref, book_ref, 0, book_to_activate, 1);
                player_npc->Activate(book_ref, player_ref, 0, book_to_activate, 1);

                player_ref->Activate(book_ref, book_ref, 0, book_to_activate, 1);
                player_ref->Activate(player_ref, book_ref, 0, book_to_activate, 1);
                player_ref->Activate(book_ref, player_ref, 0, book_to_activate, 1);

                player->Activate(book_ref, book_ref, 0, book_to_activate, 1);
                player->Activate(player_ref, book_ref, 0, book_to_activate, 1);
                player->Activate(book_ref, player_ref, 0, book_to_activate, 1);

                book_to_activate = nullptr;
                book_activate_time = 0.0f;
            }
            else
                book_activate_time += dtime;
        }
        else
            book_activate_time = 0.0f;

            */

    }


    bool save_loader_settings_done = false;
    bool auto_load = false;
    bool save_loader_done = false;
    std::string character_name = "";
    bool main_menu_was_opened = false;
    bool selected_character = false;
    bool confirming_character = false;
    bool force_new_game = false;
    bool cancel_character_name = false;


    int get_main_menu_selected_index()
    {
        int result = -1;

        auto ui = RE::UI::GetSingleton();
        if (ui->IsMenuOpen(RE::MainMenu::MENU_NAME))
        {
            RE::GFxValue var1;
            //_root.MenuHolder.Menu_mc.MainList.EntriesA[6].0.text = $CONTINUE
            const auto menu_view = ui->GetMovieView("Main Menu").get();
            if (menu_view)
            {
                if (menu_view->GetVariable(&var1, "_root.MenuHolder.Menu_mc.MainListHolder.List_mc.iSelectedIndex"))
                {
                    if (!var1.IsNull() && var1.IsNumber())
                        result = var1.GetNumber();
                }
            }
        }
        return result;
    }


    bool is_in_character_slide()
    {
        auto ui = RE::UI::GetSingleton();
        if (ui->IsMenuOpen(RE::MainMenu::MENU_NAME))
        {
            RE::GFxValue var1;
            //_root.MenuHolder.Menu_mc.MainList.EntriesA[6].0.text = $CONTINUE
            const auto menu_view = ui->GetMovieView("Main Menu").get();
            if (menu_view)
            {
                menu_view->GetVariable(&var1, "_root.MenuHolder.Menu_mc.SaveLoadPanel_mc.showingCharacterList");
                if (!var1.IsNull() && var1.IsBool())
                    return var1.GetBool();

                if (!var1.IsNull() && var1.IsNumber())
                    return var1.GetNumber();

            }
        }
        return false;
    }


    int get_main_menu_selected_save()
    {
        int result = -1;

        auto ui = RE::UI::GetSingleton();
        if (ui->IsMenuOpen(RE::MainMenu::MENU_NAME))
        {
            RE::GFxValue var1;
            //_root.MenuHolder.Menu_mc.MainList.EntriesA[6].0.text = $CONTINUE
            const auto menu_view = ui->GetMovieView("Main Menu").get();
            if (menu_view)
            {
                if (menu_view->GetVariable(&var1, "_root.MenuHolder.Menu_mc.SaveLoadPanel_mc.List_mc.iSelectedIndex"))
                {
                    if (!var1.IsNull() && var1.IsNumber())
                        result = var1.GetNumber();
                }
            }
        }
        return result;
    }


    bool is_in_save_list()
    {
        auto ui = RE::UI::GetSingleton();
        if (ui->IsMenuOpen(RE::MainMenu::MENU_NAME))
        {
            RE::GFxValue var1;
            //_root.MenuHolder.Menu_mc.MainList.EntriesA[6].0.text = $CONTINUE
            const auto menu_view = ui->GetMovieView("Main Menu").get();
            if (menu_view)
            {
                menu_view->GetVariable(&var1, "_root.MenuHolder.Menu_mc.SaveLoadPanel_mc.List_mc.focused");
                if (!var1.IsNull() && var1.IsNumber())
                    return var1.GetNumber();
            }
        }
        return false;
    }

    int find_character_save_index()
    {
        int result = -1;

        std::vector<std::string> character_names{};

        auto ui = RE::UI::GetSingleton();
        if (ui->IsMenuOpen(RE::MainMenu::MENU_NAME))
        {
            RE::GFxValue var1;
            //_root.MenuHolder.Menu_mc.MainList.EntriesA[6].0.text = $CONTINUE
            const auto menu_view = ui->GetMovieView("Main Menu").get();
            if (menu_view)
            {
                RE::GFxValue save_list;
                int save_list_size = 0;
                menu_view->GetVariable(&save_list, "_root.MenuHolder.Menu_mc.SaveLoadListHolder.List_mc.EntriesA");
                if (!save_list.IsNull() && save_list.IsArray())
                    save_list_size = save_list.GetArraySize();

                std::string base_path = "_root.MenuHolder.Menu_mc.SaveLoadListHolder.List_mc.EntriesA.";
                for (int i = 0; i < save_list_size; i++)
                {
                    std::string var_path = base_path + std::to_string(i) + ".text";
                    RE::GFxValue var_save_file_name;
                    menu_view->GetVariable(&var_save_file_name, var_path.c_str());
                    if (!var_save_file_name.IsNull() && var_save_file_name.IsString())
                    {
                        std::string save_file_name = var_save_file_name.GetString();
                        character_names.push_back(save_file_name);

                    }
                }
            }
        }

        int i = 0; //try full overlap
        for (auto a_name : character_names)
        {
            if (a_name == character_name)
                return i;
            i++;
        }

        i = 0; //try full overlap with modded
        for (auto a_name : character_names)
        {
            if (a_name == character_name + " [Modded]")
                return i;
            i++;
        }

        i = 0; //try partial
        for (auto a_name : character_names)
        {
            if (a_name.find(character_name) != std::string::npos)
                return i;
            i++;
        }


        return -1;
    }




    void move_to_item(int index)
    {
        int selected_item = get_main_menu_selected_index();

        if (selected_item > index)
            cursor_up();

        if (selected_item < index)
            cursor_down();
    }


    void move_to_save(int index)
    {
        int selected_item = get_main_menu_selected_save();

        if (selected_item > index)
            cursor_up();

        if (selected_item < index)
            cursor_down();
    }



    bool prelast_save_load = false;
    bool prelast_save_load_system_menu_called = false;
    bool prelast_save_load_system_page_open = false;

    bool is_prelast_saveloading()
    {
        return prelast_save_load;
    }

    bool is_inside_of_system_tab()
    {
        bool result = false;

        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();

        if (ui)
            if (const auto menu = ui->GetMenu<RE::JournalMenu>(); menu)
                if (menu->uiMovie)
                    //if (menu->uiMovie->GetVariable(&var1, "_root.QuestJournalFader.Menu_mc.SystemTab._group.scope.TopmostPage.Page_mc.CategoryList.Entry7.textField.text"))
                    if (menu->uiMovie->GetVariable(&var1, "_root.QuestJournalFader.Menu_mc.SystemTab._group.scope.TopmostPage.Page_mc.CategoryList.EntriesA.0.text"))
                        if (!var1.IsNull() && var1.IsString())
                        {
                            std::string temp_result = var1.GetString();

                            if (temp_result == "$QUICKSAVE")
                                result = true;
                        }

        return result;

    }


    int get_system_menu_category()
    {
        int result = -1;

        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();

        if (ui)
            if (const auto menu = ui->GetMenu<RE::JournalMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.QuestJournalFader.Menu_mc.SystemTab._group.scope.TopmostPage.Page_mc.CategoryList_mc.List_mc.iSelectedIndex"))
                        if (!var1.IsNull() && var1.IsNumber())
                        {
                            result = var1.GetNumber();
                        }

        return result;
    }


    int get_selected_load()
    {
        int result = -1;

        RE::GFxValue var1;
        RE::UI* ui = RE::UI::GetSingleton();

        if (ui)
            if (const auto menu = ui->GetMenu<RE::JournalMenu>(); menu)
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.QuestJournalFader.Menu_mc.SystemTab._group.scope.TopmostPage.Page_mc.SaveLoadPanel.List_mc.iSelectedIndex"))
                        if (!var1.IsNull() && var1.IsNumber())
                        {
                            result = var1.GetNumber();
                        }
        return result;
    }

    bool is_inside_of_category()
    {
        bool result = false;
        RE::UI* ui = RE::UI::GetSingleton();
        auto menu = ui->GetMenu<RE::JournalMenu>();
        RE::GFxValue var1;

        if (ui && menu && ui->IsMenuOpen(RE::JournalMenu::MENU_NAME))
            if (menu->uiMovie)
                if (menu->uiMovie->GetVariable(&var1, "_root.QuestJournalFader.Menu_mc.SystemTab._group.scope.SystemFader.Page_mc.SaveLoadPanel.SaveLoadList_mc.focused"))
                    if (!var1.IsNull() && var1.IsNumber())
                        result = var1.GetNumber();

        if (!result)
        {
            if (ui && menu && ui->IsMenuOpen(RE::JournalMenu::MENU_NAME))
                if (menu->uiMovie)
                    if (menu->uiMovie->GetVariable(&var1, "_root.QuestJournalFader.Menu_mc.SystemTab._group.scope.SystemFader.Page_mc.ConfirmPanel.focused"))
                        if (!var1.IsNull() && var1.IsNumber())
                            result = var1.GetNumber();
        }

        return result;
    }


    void move_system_cursor_to_category(int category)
    {
        int selected_category = get_system_menu_category();

        if (selected_category > category)
            cursor_up();

        if (selected_category < category)
            cursor_down();
    }


    void move_system_cursor_to_load(int category)
    {
        int selected_category = get_selected_load();

        if (selected_category > category)
            cursor_up();

        if (selected_category < category)
            cursor_down();
    }



    void initiate_prelast_save_load()
    {
        prelast_save_load = true;
    }

    void reset_prelast_save_load()
    {
        prelast_save_load = false;
        prelast_save_load_system_menu_called = false;
        prelast_save_load_system_page_open = false;
    }

    void save_loader(float dtime)
    {
        if (!save_loader_done)
        {
            if (!save_loader_settings_done)
            {
                std::fstream fs("_neuroSkyrim.ini");

                if (!fs)
                {
                    //file doesnt exist
                    std::ofstream o_fs("_neuroSkyrim.ini");
                    o_fs << "[Settings]\n";
                    o_fs << "autoLoadLastSave = 0\n";
                    o_fs << "forceNewGame = 0\n";
                    o_fs << "character = Gary\n";
                    o_fs.close();
                    save_loader_settings_done = true;
                    save_loader_done = true;
                }
                else
                {
                    //file exists
                    std::string line = "";


                    while (std::getline(fs, line)) {

                        auto endline_pos = fs.tellg();
                        auto startline_pos = endline_pos - static_cast<std::streampos>(line.length());

                        //autoload check
                        auto autoload = line.find("autoLoadLastSave");
                        if (autoload != std::string::npos)
                        {
                            auto val = line.find_first_of("01");
                            if (val != std::string::npos)
                            {
                                auto autoload_setting_char = line.at(val);

                                if (autoload_setting_char == '1')
                                {
                                    auto_load = true;
                                }
                            }
                        }

                        auto save_name = line.find("character");
                        if (save_name != std::string::npos)
                        {
                            auto delimeter_pos = line.find("=");
                            if (delimeter_pos != std::string::npos)
                            {
                                auto character_name_pos_start = line.find_first_not_of("= ", delimeter_pos);

                                if (character_name_pos_start != std::string::npos)
                                {
                                    auto character_name_pos_end = line.find_first_of("\n=", character_name_pos_start);

                                    if (character_name_pos_end == std::string::npos)
                                    {
                                        character_name_pos_end = line.length();
                                    }

                                    if (character_name_pos_end >= character_name_pos_start)
                                    {
                                        character_name = line.substr(character_name_pos_start, character_name_pos_end - character_name_pos_start);
                                    }
                                }
                            }
                        }


                        auto force_new_game_line = line.find("forceNewGame");
                        if (force_new_game_line != std::string::npos)
                        {
                            auto val = line.find_first_of("01");
                            if (val != std::string::npos)
                            {
                                auto autoload_setting_char = line.at(val);

                                if (autoload_setting_char == '1')
                                {
                                    force_new_game = true;
                                    fs.seekp(startline_pos + val - 2);
                                    fs.put('0');
                                    fs.seekp(endline_pos);
                                }
                            }
                        }


                    }
                }

                fs.close();

                save_loader_settings_done = true;
            }
            else
            {
                if (auto_load)
                {

                    auto inputManager = RE::BSInputDeviceManager::GetSingleton();
                    if (inputManager) 
                    {
                        auto keyboard = inputManager->GetKeyboard();

                            bool pressed = (keyboard->curState[1] & 0x80) != 0; //escape

                            if (pressed)
                            {
                                auto_load = false;
                                save_loader_done = true;
                                return;
                            }
                        

                    }

                    auto ui = RE::UI::GetSingleton();
                    if (ui->IsMenuOpen(RE::MainMenu::MENU_NAME))
                    {
                        RE::GFxValue var1;
                        const auto menu_view = ui->GetMovieView("Main Menu").get();
                        if (menu_view)
                        {
                            //_root.MenuHolder.Menu_mc.MainList.Entry8.textField.text = CONTINUE        //0
                            //_root.MenuHolder.Menu_mc.MainList.Entry9.textField.text = NEW             //1
                            //_root.MenuHolder.Menu_mc.MainList.Entry10.textField.text = LOAD           //2

                            if (force_new_game)
                            {
                                if (get_main_menu_selected_index() != 1)
                                    move_to_item(1);
                                else
                                    confirm();
                            }
                            else
                            {
                                bool can_continue = false;

                                if (menu_view->GetVariable(&var1, "_root.MenuHolder.Menu_mc.MainList.Entry8.textField.textColor"))
                                {
                                    int text_color = 0;


                                    if (!var1.IsNull() && var1.IsNumber())
                                        text_color = var1.GetNumber();

                                    can_continue = text_color > 16750000;

                                }

                                set_universal_block(0.5f);

                                if (cancel_character_name && !is_in_save_list())
                                {
                                    character_name = "";
                                }


                                if (character_name != "")
                                {
                                    if (get_main_menu_selected_index() != 2)
                                        move_to_item(2);
                                    else
                                    {
                                        if (!is_in_save_list())
                                            confirm();
                                        else
                                        {
                                            if (!selected_character)
                                            {
                                                if (!confirming_character)
                                                {
                                                    int save_index = find_character_save_index();

                                                    if (save_index == -1)
                                                    {
                                                        //cant find this name
                                                        if (is_in_save_list())
                                                        {
                                                            cancel_character_name = true;
                                                            cancel();
                                                        }
                                                    }
                                                    else
                                                    {
                                                        if (get_main_menu_selected_save() != save_index)
                                                            move_to_save(save_index);
                                                        else
                                                        {
                                                            confirming_character = true;
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    if (is_in_character_slide())
                                                        confirm();
                                                    else
                                                    {
                                                        selected_character = true;
                                                        return;
                                                    }
                                                }

                                            }
                                            else
                                            {
                                                if (get_main_menu_selected_save() != 0)
                                                    move_to_save(0);
                                                else
                                                    confirm();
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    if (can_continue)
                                    {
                                        if (get_main_menu_selected_index() != 0) //continue game if character name not given or not found and can continue
                                            move_to_item(0);
                                        else
                                            confirm();
                                    }
                                    else
                                    {
                                        if (get_main_menu_selected_index() != 1) //cant continue. start new game
                                            move_to_item(1);
                                        else
                                            confirm();

                                    }
                                }
                            }
                        }


                        main_menu_was_opened = true;
                        //confirm();
                    }

                    if (main_menu_was_opened && !ui->IsMenuOpen(RE::MainMenu::MENU_NAME))
                    {
                        auto_load = false;
                        save_loader_done = true;
                    }
                }
            }
        }


        if (prelast_save_load)
        {
            auto ui = RE::UI::GetSingleton();

            if (!prelast_save_load_system_menu_called)
            {
                if (!ui->IsMenuOpen(RE::JournalMenu::MENU_NAME))
                {
                    call_journal_menu();
                    set_universal_block(1.0f);
                }
                else
                    prelast_save_load_system_menu_called = true;
            }
            else
            {
                if (!is_inside_of_system_tab())
                {
                    if (ui)
                        if (const auto menu = ui->GetMenu<RE::JournalMenu>(); menu)
                        {
                            menu->uiMovie->Invoke("_root.QuestJournalFader.Menu_mc.SystemTab.onPress", nullptr, nullptr, 0);
                            menu->uiMovie->Invoke("_root.QuestJournalFader.Menu_mc.SystemTab.onRelease", nullptr, nullptr, 0);
                            set_universal_block(1.0f);
                        }
                }
                else
                {
                    if (get_system_menu_category() != 2)
                    {
                        move_system_cursor_to_category(2);
                        set_universal_block(0.5f);
                    }
                    else
                    {
                        if (!is_inside_of_category())
                        {
                            right();
                            set_universal_block(0.5f);
                        }
                        else
                        {
                            if (get_selected_load() != 1)
                            {
                                move_system_cursor_to_load(1);
                                set_universal_block(0.5f);
                            }
                            else
                            {
                                confirm();
                                set_universal_block(0.5f);
                            }
                        }
                    }
                }
            }

            
        }

    }



    std::pair<bool, std::string> drop_array_of_inventory_objects(std::vector<int> ids)
    {
        std::pair<bool, std::string> result{};


        int success = 0;
        int fail = 0;

        for (auto id : ids)
        {
            auto temp_result = activate_inventory_object_by_index(id, 2);

            if (temp_result.first)
                success++;
            else
                fail++;
        }

        std::string result_message = "";

        if (success > 0)
        {
            result.first = true;
            result.second = "[Successsfully dropped " + std::to_string(success) + "/" + std::to_string(success + fail) + " items]";
        }
        else
        {
            result.first = false;
            result.second = "Couldnt drop any of selected items";
        }
            
        return result;

    }



    bool has_read_book_this_activation = false;

    std::pair<bool, std::string> activate_array_of_inventory_objects(std::vector<int> ids)
    {
        std::pair<bool, std::string> result{};


        int success = 0;
        int fail = 0;

        std::string full_result = "";

        has_read_book_this_activation = false;

        for (auto id : ids)
        {
            auto temp_result = activate_inventory_object_by_index(id, 1);

            if (temp_result.first)
                success++;
            else
                fail++;

            full_result += temp_result.second + "; ";

        }

        has_read_book_this_activation = false;

        std::string result_message = "";

        if (success > 0)
        {
            result.first = true;
            result.second = "[Successsfully used " + std::to_string(success) + "/" + std::to_string(success + fail) + " items: " + full_result + "]";
        }
        else
        {
            result.first = false;
            result.second = "Couldnt use any of selected items: " + full_result;
        }

        return result;

    }




    float get_soul_charge_value(REX::TEnumSet<RE::SOUL_LEVEL> soul)
    {
        int soul_int = (int)soul.underlying();

        if (soul_int == (int)RE::SOUL_LEVEL::kPetty)
            return 250.0f;
        if (soul_int == (int)RE::SOUL_LEVEL::kLesser)
            return 500.0f;
        if (soul_int == (int)RE::SOUL_LEVEL::kCommon)
            return 1000.0;
        if (soul_int == (int)RE::SOUL_LEVEL::kGreater)
            return 2000.0f;
        if (soul_int == (int)RE::SOUL_LEVEL::kGrand)
            return 3000.0f;

        return 0.0f;
    }



    void restore_actor_value(RE::Actor* actor, RE::ActorValue value, float amount)
    {
        using func_t = decltype(&restore_actor_value);
        static REL::Relocation<func_t> func{ RELOCATION_ID(37513, 38455) };
        func(actor, value, amount);
    }





    bool charge_inventory_item(RE::InventoryEntryData* item, float charge_to_add, bool right)
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        if (item)
        {
            auto obj = item->object;
            if (obj)
            {
                if (item->extraLists && item->IsEnchanted() && item->GetEnchantment())
                {
                    auto ench = obj ? obj->As<RE::TESEnchantableForm>() : nullptr;

                    if (item->extraLists) {
                        for (auto& xList : *item->extraLists) {
                            if (xList) {
                                auto xCharge = xList->GetByType<RE::ExtraCharge>();
                                auto xEnch = xList->GetByType<RE::ExtraEnchantment>();
                                if (xEnch && xEnch->enchantment && xEnch->charge != 0)
                                {
                                    if (xCharge)
                                    {

                                        float max_charge = xEnch->charge;
                                        float cur_charge = xCharge->charge;

                                        if (cur_charge >= max_charge)
                                            return false;

                                        // if (max_charge < cur_charge + charge_to_add)
                                        {
                                            auto player_actor = (RE::Actor*)player->AsReference();
                                            if (right)
                                            {
                                                restore_actor_value(player_actor, RE::ActorValue::kRightItemCharge, charge_to_add);
                                            }
                                            //player->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kTemporary, RE::ActorValue::kRightItemCharge, max_charge);
                                            else
                                                restore_actor_value(player_actor, RE::ActorValue::kLeftItemCharge, charge_to_add);
                                            //player->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kTemporary, RE::ActorValue::kLeftItemCharge, max_charge);

                                            return true;
                                        }


                                    }
                                    else
                                        return false;
                                }
                                else
                                {
                                    if (ench && ench->formEnchanting && ench->amountofEnchantment != 0)
                                    {

                                        float max_charge = ench->amountofEnchantment;
                                        float cur_charge = 0;//xCharge->charge;

                                        if (right)
                                            cur_charge = player->GetActorValue(RE::ActorValue::kRightItemCharge);
                                        else
                                            cur_charge = player->GetActorValue(RE::ActorValue::kLeftItemCharge);


                                        if (cur_charge >= max_charge)
                                            return false;

                                        if (max_charge < (cur_charge + charge_to_add))
                                        {
                                            if (right)
                                                player->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kRightItemCharge, max_charge - cur_charge);
                                            else
                                                player->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kLeftItemCharge, max_charge - cur_charge);
                                        }
                                        else
                                        {
                                            if (right)
                                                player->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kRightItemCharge, charge_to_add);
                                            else
                                                player->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kLeftItemCharge, charge_to_add);
                                        }

                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return false;
    }


    //this trash doesnt work
    //using ObjectPtr = RE::BSTSmartPointer<RE::BSScript::Object>;

    //RE::BSContainer::ForEachResult my_functor (ObjectPtr object) {

    //    bool we_are_here = false;

    //    return RE::BSContainer::ForEachResult::kContinue;
    //}


    int random_int_from_range(int min, int max)
    {
        if (min > max)
            return 0;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(min, max);
        int random_num = distr(gen);

        return random_num;
    }



    std::pair<bool, std::string> activate_inventory_object_by_index(int item_id, int action_id)
    {
        std::pair<bool, std::string> result{};


        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();
        auto player_actor = (RE::Actor*)player->AsReference();

        if (MiscThings::is_intro() || MiscThings::is_intro2())
        {
            result.first = false;
            result.second = "Cannot access inventory right now. ";
            return result;
        }



        if (!inventory_valid)
        {
            auto get_inventory_result = GetInventory();
            send_random_context("You inventory contents: " + get_inventory_result.second);
        }


        if (inventory_items_list.find(item_id) != inventory_items_list.end())
        {
            auto object = inventory_items_list.find(item_id)->second.object;

            //check if item is still there
            RE::TESObjectREFR::InventoryItemMap inventory = MiscThings::get_filtered_inventory();


            if (player_ref && inventory.find(object) != inventory.end())
            {
                auto item_form = (RE::TESForm*)object;

                std::string object_name = object->GetName();

                object_name = replace_aliases_all_quests(object_name);
                auto actor_equip = RE::ActorEquipManager::GetSingleton();

                switch (action_id)
                {
                case 1:
                {
                    if (is_equippable(object))
                    {
                        
                        if (is_equipped(object))
                        {

                            bool staff_of_magnus = false;

                            if (object->GetFormID() == 0x35369) //staff of magnus
                            {
                                staff_of_magnus = true;
                            }

                            if (staff_of_magnus && magnus_eye_attack_condition())
                            {
                                auto magnus_eye = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x25224);
                                if (magnus_eye)
                                    return WalkerProcessor::walk_to_object_by_refr(magnus_eye, 3);
                            }


                            result.first = actor_equip->UnequipObject((RE::Actor*)player_ref, object);
                            result.second = "[Unequipped [id " + std::to_string(item_id) + "] " + object_name + "]";

                            if (object->IsWeapon())
                                if (player_actor && !player_actor->IsWeaponDrawn() && !(player_actor->actorState2.weaponState == RE::WEAPON_STATE::kDrawing))
                                    ready_weapon(); //show off unequipped hands
                        }
                        else
                        {
                            std::string equip_hand = "";

                            if (object->IsWeapon())
                            {
                                auto slot = get_free_slot();
                                
                                bool staff_of_magnus = false;

                                if (object->GetFormID() == 0x35369) //staff of magnus
                                {
                                    slot = (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F43); //always left hand
                                    staff_of_magnus = true;
                                }
                                    




                                bool left_hand = false;

                                if (slot == (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F42)) //right hand
                                    equip_hand = " in right hand";

                                if (slot == (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F43)) //left hand
                                {
                                    left_hand = true;
                                    equip_hand = " in left hand";
                                }
                                    

                                auto weapon = (RE::TESObjectWEAP*)object;

                                if (weapon->IsTwoHandedAxe() || weapon->IsTwoHandedSword() || weapon->IsBow())
                                    equip_hand = " in both hands";



                                auto entry = inventory.find(object);

                                if (entry != inventory.end() && entry->second.second.get())
                                {
                                    RE::InventoryEntryData* entry_entry = entry->second.second.get();

                                    RE::ExtraDataList* extra = nullptr;

                                    if (entry_entry->extraLists && entry_entry->extraLists->size() > 0)
                                    {
                                        extra = *entry_entry->extraLists->begin();

                                        actor_equip->EquipObject((RE::Actor*)player_ref, object, extra, 1, slot); //equip with extra for scripts to trigger
                                    }
                                    else
                                        actor_equip->EquipObject((RE::Actor*)player_ref, object, nullptr, 1, slot); //normal equip
                                }
                                else
                                    actor_equip->EquipObject((RE::Actor*)player_ref, object, nullptr, 1, slot); //normal equip




                                WalkerProcessor::reset_attacking_inanimate_object_time();
                                
                                if (staff_of_magnus && magnus_eye_attack_condition())
                                {
                                    auto magnus_eye = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x25224);
                                    if (magnus_eye)
                                        WalkerProcessor::walk_to_object_by_refr(magnus_eye, 3);
                                }



                                if (player_actor && !player_actor->IsWeaponDrawn() && !(player_actor->actorState2.weaponState == RE::WEAPON_STATE::kDrawing))
                                {
                                    ready_weapon(); //show off new weapon
                                }
                                    
                            }
                            else
                            {
                                auto entry = inventory.find(object);

                                if (entry != inventory.end() && entry->second.second.get())
                                {
                                    RE::InventoryEntryData* entry_entry = entry->second.second.get();

                                    RE::ExtraDataList* extra = nullptr;

                                    if (entry_entry->extraLists && entry_entry->extraLists->size() > 0)
                                    {
                                        extra = *entry_entry->extraLists->begin();

                                        actor_equip->EquipObject((RE::Actor*)player_ref, object, extra); //equip with extra for scripts to trigger
                                    }
                                    else
                                        actor_equip->EquipObject((RE::Actor*)player_ref, object); //normal equip
                                }
                                else
                                    actor_equip->EquipObject((RE::Actor*)player_ref, object); //normal equip

                            }
                                



                            result.second = "[Equipped [id " + std::to_string(item_id) + "] " + object_name + equip_hand + "...]";
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
                            result.second = "[Consuming [id " + std::to_string(item_id) + "] " + object_name + "...]";
                            return result;
                        }
                        else
                        {
                            if (object->IsBook())
                            {
                                auto book_book = object->As<RE::TESObjectBOOK>();

                                std::string name = book_book->GetFullName();

                                if (name.find("Elder Scroll (") != std::string::npos)
                                {

                                    auto entry = inventory.find(object);

                                    if (entry != inventory.end() && entry->second.second.get())
                                    {

                                        RE::InventoryEntryData* entry_entry = entry->second.second.get();

                                        RE::ExtraDataList* extra = nullptr;

                                        if (entry_entry->extraLists && entry_entry->extraLists->size() > 0)
                                        {
                                            extra = *entry_entry->extraLists->begin();

                                            //TimeWoundTrigger.IsTriggerReady() && !MQ206.GetStageDone(20) && akActor.GetSitState() == 0

                                            // 

                                            
                                            auto timewound_trig = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x64769);

                                            if (timewound_trig)
                                            {
                                                auto object_p = MiscThings::General::Script::GetObject(timewound_trig, "MQ206TimeWoundTriggerScript");
                                                
                                                bool trigger_active = false;


                                                float radius = 1346.3203f / 2.0f;

                                                auto player_cell = player->GetParentCell();
                                                auto timewound_cell = timewound_trig->GetParentCell();

                                                auto player_pos = player->GetPosition();
                                                auto timewound_pos = timewound_trig->GetPosition();


                                                trigger_active = (player_cell == timewound_cell) && (player_pos.GetDistance(timewound_pos) <= radius);


                                                /*
                                                //i hate this shit
                                                if (object_p)
                                                {
                                                    //RE::BSFixedString prop_name = "bIsActive";
                                                    //trigger_active = General::Script::GetVariable<bool>(object_p, prop_name);

                                                    //DefaultSetStageOnEnter

                                                    auto object_parent = MiscThings::General::Script::GetObject(timewound_trig, "DefaultSetStageOnEnter");

                                                    //auto object_parent = object_p->GetTypeInfo()->GetParent();

                                                    if (object_parent)
                                                    {
                                                       // object_parent

                                                        RE::BSFixedString prop_name = "targetCountCurrent";
                                                        int target_count_current = General::Script::GetVariable<int>(object_parent, prop_name);

                                                        prop_name = "targetCountTotal";
                                                        int target_count_total = General::Script::GetVariable<int>(object_parent, prop_name);


                                                        trigger_active = target_count_total > 0;

                                                    }

                                                    
                                                    auto no_arg = RE::MakeFunctionArguments();

                                                    //trigger_active = General::Script::DispatchMethodCall<>(object_p, "IsTriggerReady", nullptr, no_arg);

                                                    using InternalVM = RE::BSScript::Internal::VirtualMachine;

                                                    auto vm = InternalVM::GetSingleton();
                                                    auto args = RE::MakeFunctionArguments<>();




                                                    //using CallbackPtr = RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>;
                                                    
                                                    bool trigger_result = false;

                                                    RE::BSScript::Variable my_var;
                                                    //RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(my_var);

                                                    //auto result2 = (RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor>)(&trigger_result);

                                                    //vm->DispatchMethodCall(object_p, "IsTriggerReady", args, callback);

                                                    

                                                    bool stop_here = false;
                                                    
                                                }
                                            */
                                                bool stage_not_done = false;

                                                auto mq206 = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ206");
                                                if (mq206 && mq206->GetCurrentStageID() < 20)
                                                    stage_not_done = true;


                                                bool sitstate_zero = false;

                                                if (player_actor)
                                                {
                                                    sitstate_zero = player_actor->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal;
                                                }

                                                if (trigger_active && stage_not_done && sitstate_zero)
                                                {
                                                    result.first = true;
                                                    result.second = "[You unfurl The Elder Scroll and look into it... you see a vision...]";
                                                }
                                                else
                                                {
                                                    result.first = true;
                                                    result.second = "[You unfurl The Elder Scroll and look into it... it blinds you for several seconds!]";
                                                }


                                            }
                                            
                                            if (has_read_book_this_activation)
                                            {
                                                result.first = false;
                                                result.second = "[Cannot read multiple books at once]";
                                                return result;
                                            }

                                            actor_equip->EquipObject((RE::Actor*)player_ref, object, extra);

                                            has_read_book_this_activation = true;


                                            if (result.second == "")
                                            {
                                                result.first = true;
                                                result.second = "[Reading [id " + std::to_string(item_id) + "] " + object_name + "...]";
                                            }


                                            return result;

                                        }
                                    }
                                   
                                }
                                else
                                {
                                    if (book_book->TeachesSpell())
                                    {
                                        book_book->Read(player_ref);
                                    }
                                    else
                                    {
                                        //book_book->Read(player_ref);
                                        auto book_book = object->As<RE::TESObjectBOOK>();

                                        auto entry = inventory.find(object);

                                        if (entry != inventory.end() && entry->second.second.get())
                                        {
                                            RE::InventoryEntryData* entry_entry = entry->second.second.get();

                                            RE::ExtraDataList* extra = nullptr;
                                            if (entry_entry->extraLists && entry_entry->extraLists->size() > 0)
                                            {
                                                extra = *entry_entry->extraLists->begin();
                                            }
                                            //     a_description, a_extraList, a_ref, a_book, a_pos, a_rot, a_scale, a_useDefaultPos);
                                            //OpenMenu_Impl(desc, nullptr, nullptr, a_book, NiPoint3(), rot, 1.0f, true);
                                            RE::NiMatrix3 rot{};
                                            rot.SetEulerAnglesXYZ(-0.05f, -0.05f, 1.50f);

                                            if (has_read_book_this_activation)
                                            {
                                                result.first = false;
                                                result.second = "[Cannot read multiple books at once]";
                                                return result;
                                            }

                                            actor_equip->EquipObject((RE::Actor*)player_ref, object, extra);
                                            RE::BookMenu::OpenMenuFromBaseForm(book_book, extra, { 0,0,0 }, rot, 1.0f, true);

                                            has_read_book_this_activation = true;

                                        }                           
                                    }
                                }

                                result.first = true;
                                result.second = "[Reading [id " + std::to_string(item_id) + "] " + object_name + "...]";
                                return result;
                            }
                            else
                            {
                                if (object->IsSoulGem())
                                {
                                    auto soulgem = (RE::TESSoulGem*)object;

                                    if (soulgem->currentSoul)
                                    {
                                        //auto actor_equip = RE::ActorEquipManager::GetSingleton();
                                        //bool test123 = actor_equip->UnequipObject((RE::Actor*)player_ref, object);

                                        std::vector<RE::TESForm*> hands{};

                                        hands.push_back(get_hand_contents(true));
                                        hands.push_back(get_hand_contents(false));

                                        bool charge_right_hand = true;

                                        bool no_weapons = true;
                                        bool successful_charge = false;

                                        for (auto hand : hands)
                                        {
                                            //auto weapon = (RE::TESObjectWEAP*)hand;

                                            auto object_ptr = inventory.find((RE::TESBoundObject*)hand);

                                            if (object_ptr != inventory.end())
                                            {
                                                no_weapons = false;

                                                auto p_entry_data = &object_ptr->second.second;

                                                if (p_entry_data)
                                                {
                                                    auto entry_data = p_entry_data->get();

                                                    auto charge = entry_data->GetEnchantmentCharge();

                                                    auto soul = soulgem->currentSoul;

                                                    float charge_to_add = get_soul_charge_value(soul);

                                                    auto entry_object = entry_data->object;

                                                    bool charge_result = charge_inventory_item(entry_data, charge_to_add, charge_right_hand);

                                                    if (charge_result)
                                                    {
                                                        auto form_id = entry_object->GetFormID();

                                                        

                                                        if (form_id == 0x63b27 || //azure white
                                                            form_id == 0x63b29 || //azure black
                                                            form_id == 0xfe001801 //cc varla stone
                                                            )
                                                            soulgem->currentSoul = RE::SOUL_LEVEL::kNone; //clear soul
                                                        else
                                                        {
                                                            auto object_ptr = inventory.find((RE::TESBoundObject*)object);

                                                            if (object_ptr != inventory.end())
                                                                if (object_ptr->first)
                                                                    player->RemoveItem(object_ptr->first, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);

                                                            
                                                        }

                                                        successful_charge = true;

                                                        break;
                                                    }
                                                    else
                                                    {
                                                        if (hands[0] == hands[1])
                                                            break;
                                                    }

                                                    
                                                }

                                            }

                                            charge_right_hand = false;
                                            
                                        }

                                        if (no_weapons)
                                        {
                                            result.first = false;
                                            result.second = "[Cannot use [id " + std::to_string(item_id) + "] " + object_name + " - no chargable weapons equipped]";
                                            return result;
                                        }

                                        if (!successful_charge)
                                        {
                                            result.first = false;
                                            result.second = "[Cannot use [id " + std::to_string(item_id) + "] " + object_name + " - cannot charge current weapons]";
                                            return result;
                                        }


                                        result.first = true;
                                        result.second = "[Using [id " + std::to_string(item_id) + "] " + object_name + "...]";
                                        return result;
                                    }
                                    else
                                    {
                                        result.first = false;
                                        result.second = "[id " + std::to_string(item_id) + "] " + object_name + " contains no soul. Cannot use it]";
                                        return result;
                                    }
                                    
                                }


                                result.first = false;
                                result.second = "Cannot activate [id " + std::to_string(item_id) + "] " + object_name + "...]";

                                if (object_name.find("Lockpick") != std::string::npos)
                                    result.second += " lockpicks are used automatically when you interact with closed objects, dont need to use them from inventory";

                                if (object->IsKey())
                                    result.second += " keys are used automatically when you interact with the correct door, dont need to use them from inventory";

                                return result;
                            }

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


                    auto entry = inventory.find(object);

                    if (entry != inventory.end() && entry->second.second.get())
                    {
                        RE::InventoryEntryData* entry_entry = entry->second.second.get();

                        if (entry_entry)
                        {
                            if (entry_entry->IsQuestObject())
                            {
                                result.first = false;
                                result.second = "Cant drop [id " + std::to_string(item_id) + "] " + object_name + ", its a quest object]";
                                return result;
                            }
                        }
                        else
                            cant_drop = true;
                    }
                    else
                        cant_drop = true;


                    if (cant_drop)
                    {
                        result.first = false;
                        result.second = "Cant drop [id " + std::to_string(item_id) + "] " + object_name + "...]";
                        return result;
                    }
                    else
                    {
                        player_actor->DropObject(object, nullptr, 1);
                        result.first = true;
                        result.second = "[Dropping [id " + std::to_string(item_id) + "] " + object_name + "...]";
                        return result;
                    }

                    break;
                }

                default:
                {
                    result.first = false;
                    result.second = "Invalid action ID. Valid actions: 1 - activate/use/equip/unequip, 2 - drop";
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
            result.second = "Invalid item ID. Use get_inventory to get list of available item IDs. ";
            return result;
        }


    
        //result.first = false;
        //result.second = "Invalid command";

        //return result;
    }




    std::pair<bool, std::string> activate_inventory_object_by_refr(RE::TESBoundObject* item)
    {
        std::pair<bool, std::string> result{};
        if (!inventory_valid)
        {
            auto temp_result = GetInventory();

            if (!inventory_valid)
                return result;
        }

        auto p_inventory = get_p_inventory_items_list();

       
        for (auto inventory_entry : *p_inventory)
        {
            if (inventory_entry.second.object == item)
            {
                return activate_inventory_object_by_index(inventory_entry.first, 1);
            }
        }


        return result;

    }



    std::pair<bool, std::string> use_random_soulgem()
    {
        std::pair<bool, std::string> result{};

        auto inventory = MiscThings::get_filtered_inventory();

        for (auto& [item, data] : inventory)
        {
            if (item && item->GetFormType() == RE::FormType::SoulGem)
            {
                auto soulgem = (RE::TESSoulGem*)item;

                auto soul = soulgem->currentSoul;

                if (get_soul_charge_value(soul) > 0)
                    return MiscThings::activate_inventory_object_by_refr(item);
            }
        }

        return result;
    }





    std::string get_enchantment_info(RE::TESBoundObject* item)
    {
        std::string result = "";


        RE::TESObjectREFR::InventoryItemMap inventory = MiscThings::get_filtered_inventory();

            //auto item = entry->object;

        if (inventory.find(item) != inventory.end())
        {
            auto entry_ptr = inventory.find(item);

            auto entry = entry_ptr->second.second.get();

            if (entry->extraLists && entry->IsEnchanted())
            {
                auto enchantment = entry->GetEnchantment();

                //auto ench = item ? item->As<RE::TESEnchantableForm>() : nullptr;

                    if (enchantment)
                    {
                        //auto enchantment = ench->formEnchanting;


                        auto effect = enchantment->GetAVEffect();

                        std::string descr = "";

                        if (effect)
                        {

                            descr = effect->magicItemDescription;
                            descr = fix_enchantment_description(descr, enchantment);

                            result = "[Enchanted: " + descr + "]";
                        }

                        if (descr == "")
                        {
                            result = "[Enchanted]";
                            /*
                            RE::BSString temp_string;

                            enchantment->GetDescription(temp_string, a_spell, 'MANC');

                            descr = temp_string;
                            descr = fix_spell_description(descr, a_spell);
                            */
                        }

                    }
            }
        }
        
        return result;
    }





    std::string get_inventory_item_full_info(RE::TESBoundObject* item, bool compact = false)
    {

        std::string result = "";

        auto item_form = (RE::TESForm*)item;

        //if (!item_form->sourceFiles.array || item_form->sourceFiles.array->size() <= 0)
        //    return "";

        std::string actions = "";

        /*
        RE::TESObjectREFR::InventoryItemMap inventory = MiscThings::get_filtered_inventory();


        if (inventory.find(item) == inventory.end())
            return result; //not found
        

        auto& [quantity, data] = inventory.find(item)->second;
        */


        std::string gold_name = "Gold";

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

                    stats = "[" + damage_text + ", " + weight_text + "]";
                }


                if (base_type == RE::FormType::Armor)
                {

                    auto armor = (RE::TESObjectARMO*)item_form;

                    std::string heavy = "";

                    if (!compact)
                        if (armor->IsHeavyArmor())
                            heavy = "Heavy, ";


                    auto armor_val = armor->armorRating / 100;

                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(1) << armor->weight;
                    std::string weight_text_number = ss.str();

                    std::string armor_val_text = "Armor: " + std::to_string(armor_val);
                    std::string weight_text = "Weight: " + weight_text_number;

                    stats = "[" + heavy + armor_val_text + ", " + weight_text + "]";
                }


                if (!compact)
                {
                    if (is_equipped(item))
                        actions += "[Equipped" + hand_text + "]";// [Can unequip] ";
                    else
                        actions += "[Not equipped" + hand_text + "]";// [Can unequip] ";

                    actions += stats;
                }
    
            }
            else
            {
                if (MiscThings::player_overencumbered_by())
                {
                    //if overencumbered
                    auto weight = item_form->GetWeight();

                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(1) << weight;
                    std::string weight_text_number = ss.str();

                    actions += "[Weight: " + weight_text_number + "]";
                }
            }

            if (item_form->formType == RE::FormType::Ingredient || item_form->formType == RE::FormType::AlchemyItem)
                ;// actions += "[Can consume]";


            actions += get_enchantment_info(item);


            auto name = item->As<RE::TESFullName>();


            if (name && name->fullName != "")
            {

                std::string item_name = item->GetName();

                if (item_name != "" && item_name != gold_name)
                {

                    if (item_name.find("<Alias=") != std::string::npos)
                    {
                        auto player = RE::PlayerCharacter::GetSingleton();
                        auto quest_targets = player->questTargets;

                        for (auto quest_target : quest_targets)
                            item_name = replace_aliases(quest_target.first, item_name); //if there are more
                    }


                    result += get_object_category(item_form, item);
                    result += actions + " ";
                    result += item_name;
                    //result += "\n"; //TODO: replace with comma later

                    //item_data database_data{};
                    //database_data.amount = data.first;
                    //database_data.object = data.second->GetObject();

                    //inventory_items_list.insert({ id , database_data });
                }
            }
        }

        return result;
    }



    int remove_item_from_inventory_list(RE::TESBoundObject* item)
    {
        for (std::pair<int, item_data> inventory_entry : inventory_items_list)
        {
            if (inventory_entry.second.object == item)
            {
                return (int)inventory_items_list.erase(inventory_entry.first);
            }
        }

        return 0;
    }


    std::string insert_item_into_inventory_list_and_get_info(RE::TESBoundObject* item, bool compact)
    {
        std::string result = "";





        bool found = false;
        for (std::pair<int, item_data> inventory_entry : inventory_items_list)
        {
            if (inventory_entry.second.object == item)
            {

                std::string info = get_inventory_item_full_info(item, compact);

                if (info == "")
                    return result; //nothing
       
                //std::string quantity_text = std::to_string(inventory_entry.second.amount);
                result = "[id " + std::to_string(inventory_entry.first) + "]" + info;// +" x" + quantity_text;
                found = true;
                break;
            }
        }


        RE::TESObjectREFR::InventoryItemMap inventory = MiscThings::get_filtered_inventory();


        if (inventory.find(item) == inventory.end())
            return result; //not found in actual inventory


        if (!found) //means not found in the list
        {
            auto entry_ptr = inventory.find(item);

            if (entry_ptr == inventory.end())
                return result;

            auto& [quantity, data] = entry_ptr->second;

            RE::InventoryEntryData* inv_entry = nullptr;

            if (data)
                inv_entry = data.get();

            std::string info = get_inventory_item_full_info(item, compact);

            if (info == "" || quantity <= 0)
                return result;

            int new_id = 0;
            if (std::size(inventory_items_list) > 0)
                new_id = inventory_items_list.rbegin()->first + 1;


            result = "[id " + std::to_string(new_id) + "]" + info;

            item_data database_data{};
            database_data.amount = quantity;
            database_data.object = data->GetObject();

            inventory_items_list.insert({ new_id , database_data });

            if (!inventory_valid)
                inventory_valid = true;
        }


        return result;
    }




    bool is_inventory_item_in_the_list(RE::TESBoundObject* item)
    {
        bool result = false;

        if (inventory_valid)
        {
            for (auto list_entry : inventory_items_list)
            {
                if (list_entry.second.object == item)
                {
                    result = true;
                    break;
                }
            }
        }
        return result;
    }


    std::pair<bool, std::string> GetInventory()
    {
        std::pair<bool, std::string> result{};

        //inventory_valid = false;
        //inventory_items_list.clear();

        std::string inventory_contents = "Your inventory contents:\n";

        RE::TESObjectREFR::InventoryItemMap inventory = MiscThings::get_filtered_inventory();


        for (auto& [item, data] : inventory)
        {
            std::string info = insert_item_into_inventory_list_and_get_info(item);

            if (info == "")
                continue;

            inventory_contents += info + " x" + std::to_string(data.first);
            inventory_contents += "\n";


            auto test = get_restore_value(item, RE::ActorValue::kHealth);
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
        int num_digits = 0;
        for (char ch : str) {
            int v = ch; // ASCII Val converted
            if (ch >= 48 && ch <= 57) {
                num_digits++;
                if (num_digits > 1)
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




    std::string fix_enchantment_description(std::string description, RE::EnchantmentItem* spell)
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
            return fix_enchantment_description(description, spell);
        else
            return description;
    }






    struct spell {
        RE::SpellItem* spell;
        RE::TESShout* shout;
    };

    std::map<int, spell> spells{};



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

        spells.clear();

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
                            
                            bool right_equipped = is_spell_equipped(true, (RE::MagicItem*)a_spell);
                            bool left_equipped = is_spell_equipped(false, (RE::MagicItem*)a_spell);

                            std::string equip_info = "";
                           
                            if (!right_equipped && !left_equipped)
                                equip_info = "[Unequipped]";
                            else
                                if (right_equipped && !left_equipped)
                                    equip_info = "[Equipped in right hand, can also be equipped in left hand]";
                                else
                                    if (!right_equipped && left_equipped)
                                        equip_info = "[Equipped in left hand, can also be equipped in right hand]";
                                    else
                                        equip_info = "[Equipped in both hands]";


                            *active_spells += "[id " + std::to_string(i) + "]" + equip_info + " " + name + " - " + description + "\n";
                            spells.insert({ i, {a_spell, nullptr } });
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
                                    spells.insert({ i, {a_spell, nullptr } });
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
                    if (variation.word && (variation.word->formFlags & 65600) == 65600)
                        unlocked_words++;

                    if (variation.word && variation.word->GetKnown())
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

                std::string words_text = MiscThings::get_shout_known_words_text(shout_p);
                if (words_text != "")
                {
                    name += " - " + words_text;
                }

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
                spells.insert({ max_id, {nullptr, shout_p } });
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



    std::string fix_word_of_power_name(std::string name)
    {
        std::string result = name;

        bool found_something = false;

        if (auto pos = result.find("1"); pos != std::string::npos)
        {
            result.erase(pos, 1);
            result.insert(pos, "aa");
            found_something = true;
        }

        if (auto pos = result.find("2"); pos != std::string::npos)
        {
            result.erase(pos, 1);
            result.insert(pos, "ei");
            found_something = true;
        }

        if (auto pos = result.find("3"); pos != std::string::npos)
        {
            result.erase(pos, 1);
            result.insert(pos, "ii");
            found_something = true;
        }

        if (auto pos = result.find("4"); pos != std::string::npos)
        {
            result.erase(pos, 1);
            result.insert(pos, "ah");
            found_something = true;
        }

        //doesnt exist?
        //if (auto pos = result.find("5"); pos != std::string::npos)
        //{
        //    result.erase(pos, 1);
        //    result.insert(pos, "");
        //    found_something = true;
        //}

        if (auto pos = result.find("6"); pos != std::string::npos)
        {
            result.erase(pos, 1);
            result.insert(pos, "ur");
            found_something = true;
        }

        if (auto pos = result.find("7"); pos != std::string::npos)
        {
            result.erase(pos, 1);
            result.insert(pos, "ir");
            found_something = true;
        }

        if (auto pos = result.find("8"); pos != std::string::npos)
        {
            result.erase(pos, 1);
            result.insert(pos, "oo");
            found_something = true;
        }

        if (auto pos = result.find("9"); pos != std::string::npos)
        {
            result.erase(pos, 1);
            result.insert(pos, "ey");
            found_something = true;
        }


        if (found_something)
            return fix_word_of_power_name(result);
        else
            return result;
    }


    std::string get_shout_known_words_text(RE::TESShout* shout)
    {
        std::string result = "";

        if (shout)
        {
            int known_words = 0;
            int unlocked_words = 0;
            int max_words = 0;
            //bool set_known_one = false;

            for (auto variation : shout->variations)
            {
                if (variation.word && (variation.word->formFlags & 65600) == 65600)
                {
                    auto word_of_power = (RE::TESWordOfPower*)variation.word;


                    std::string word_text = word_of_power->GetFullName();

                    word_text = fix_word_of_power_name(word_text);

                    result += word_text + " ";

                    unlocked_words++;
                }



                if (variation.word && variation.word->GetKnown())
                    known_words++;

                max_words++;
            }
        }


        return result;
    }


    

    std::pair<bool, std::string> unlock_shout_level(int shout_id)
    {
        
        std::pair<bool, std::string> result{};



        if (MiscThings::is_intro() || MiscThings::is_intro2())
        {
            result.first = false;
            result.second = "Cannot unlock shouts yet. ";
            return result;
        }

        if (std::size(spells) <= 0)
        {
            auto get_spells_result = get_available_spells();
            send_random_context("Available spells: " + get_spells_result.second);
        }

        if (spells.find(shout_id) != spells.end())
        {
            auto shout_p = spells.find(shout_id)->second.shout;
            if (shout_p)
            {
                for (auto variation : shout_p->variations)
                {
                    if (variation.word && variation.word->GetKnown()) //known
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




    bool have_force_only_menu_open()
    {
        //DOES NOT INCLUDE TWEEN MENU
        bool result = false;

        RE::UI* ui = RE::UI::GetSingleton();

        if (ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::StatsMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::LevelUpMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::TrainingMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::SleepWaitMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::MessageBoxMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::BookMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::GiftMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::LockpickingMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::MainMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::RaceSexMenu::MENU_NAME))
            result = true;

        if (ui->IsMenuOpen(RE::MapMenu::MENU_NAME))
            result = true;

        result |= is_fishing();

        return result;
    }



    RE::TESForm* get_hand_contents(bool right)
    {
        RE::TESForm* hand_contents = nullptr;

        auto player = RE::PlayerCharacter::GetSingleton();

        if (!player || !player->currentProcess)
            return nullptr;

        auto actor_process = player->currentProcess;
        auto equipped_list = actor_process->equippedObjects;

        auto equipped_right = equipped_list[1];
        auto equipped_left = equipped_list[0];

        if (right)
        {
            hand_contents = equipped_right;
        }
        else
        {
            hand_contents = equipped_left;
        }

        return hand_contents;
    }



    bool is_self_healing_spell(bool right)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            RE::MagicItem* spell = (RE::MagicItem*)get_hand_contents(right);

            if (spell && (spell->GetFormType() == RE::FormType::Spell || spell->GetFormType() == RE::FormType::Scroll))
                if (spell->GetSpellType() != RE::MagicSystem::SpellType::kEnchantment)
                {
                    if (spell->GetDelivery() == RE::MagicSystem::Delivery::kSelf)
                    {
                        for (auto effect : spell->effects)
                        {
                            if (effect->baseEffect && effect->baseEffect->GetArchetype() == RE::EffectArchetypes::ArchetypeID::kValueModifier && effect->baseEffect->data.primaryAV == RE::ActorValue::kHealth && effect->baseEffect->data.associatedSkill == RE::ActorValue::kRestoration)
                            {
                                result = true;
                                break;
                            }
                        }
                    }
                }
        }
        return result;
    }





    std::pair<bool, std::string> cast_spell_by_index(int id, bool fast, bool player_issued)
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



        if (std::size(spells) <= 0)
        {
            auto get_spells_result = get_available_spells();
            send_random_context("Available spells: " + get_spells_result.second);
        }


        if (player_actor && spells.find(id) != spells.end())
        {
            auto spell_entry = spells.find(id);

            if (spell_entry->second.spell)
            {
                RE::SpellItem* spell = spell_entry->second.spell;

                auto equip_slot = spell->GetEquipSlot();

                auto slot_id = equip_slot->GetFormID();

                auto equip_manager = RE::ActorEquipManager::GetSingleton();

                if (equip_manager)
                {
                    if (slot_id == 0x00025BEE) //voice
                    {
                        equip_manager->EquipSpell(player_actor, spell, equip_slot);
                        right_attack_cancel();
                        left_attack_cancel();
                        use_ult();
                        result.first = true;
                        if (spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell)
                            result.second = "[Casting spell:  [id " + std::to_string(id) + "] " + spell->GetFullName() + "]";
                        else
                            result.second = "[Using ult: [id " + std::to_string(id) + "] " + spell->GetFullName() + "]";
                    }
                    else
                    {
                        if (slot_id == 0x00013F44) //either hand
                        {
                            auto slot = get_free_slot();

                            if (!is_offensive_spell(spell))
                                slot = (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F43); //non offensive spells go in left hand


                            std::string equip_hand = "";

                            bool right_hand = false;

                            if (slot == (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F42)) //right hand
                            {
                                right_hand = true;
                                equip_hand = " in right hand";
                            }
                                

                            if (slot == (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F43)) //left hand
                                equip_hand = " in left hand";



                            equip_manager->EquipSpell(player_actor, spell, slot);
                            result.first = true;
                            if (spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell)
                            {
                                if (player_actor)
                                {
                                    if (!player_actor->IsWeaponDrawn() && !(player_actor->actorState2.weaponState == RE::WEAPON_STATE::kDrawing))
                                    {
                                        set_universal_block(1.0f);
                                        ready_weapon();
                                    }
                                        

                                    //if (player_hp_less_than(90.0f) && is_self_healing_spell(spell))
                                    //{
                                    if (!WalkerProcessor::is_fighting()) 
                                        try_casting_hand(right_hand);

                                    //}
                                }
                                    

                                result.second = "[Equipped [id " + std::to_string(id) + "] " + spell->GetFullName() + equip_hand + "]";
                            }
                            else
                                result.second = "[Casting [id " + std::to_string(id) + "] " + spell->GetFullName() + equip_hand + "]";
                        }
                        else
                        {
                            result.first = false;
                            result.second = "Cannot use [id " + std::to_string(id) + "] " + spell->GetFullName() + "]";
                        }
                    }
                }
            }
            else
            {
                if (spell_entry->second.shout)
                {
                    RE::TESShout* shout = spell_entry->second.shout;

                    auto equip_slot = shout->GetEquipSlot();

                    auto slot_id = equip_slot->GetFormID();

                    auto equip_manager = RE::ActorEquipManager::GetSingleton();

                    if (equip_manager)
                    {
                        if (slot_id == 0x00025BEE) //voice
                        {
                            if (player_actor->GetVoiceRecoveryTime() <= 0.0f)
                            {
                                int unlocked_words = 0;

                                for (auto variation : shout->variations)
                                {
                                    if (variation.word && (variation.word->formFlags & 65600) == 65600)
                                        unlocked_words++;
                                }
                                if (unlocked_words > 0)
                                {

                                    auto capture_dragon_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ301");
                                    if (capture_dragon_quest && capture_dragon_quest->GetCurrentStageID() == 150)
                                    {
                                        auto call_dragon = (RE::TESShout*)RE::TESForm::LookupByID(0x46B8c);
                                        if (shout == call_dragon)
                                        {
                                            result.first = false;
                                            result.second = "You already called the dragon. No need to do it again yet";
                                            return result;
                                        }
                                    }



                                    equip_manager->EquipShout(player_actor, shout);
                                    //use_ult();
                                    right_attack_cancel();
                                    left_attack_cancel();

                                    auto dragonrend = (RE::TESShout*)RE::TESForm::LookupByID(0x44250);

                                    if (player_issued && shout == dragonrend)
                                    {
                                        auto dragonrend_target = MiscThings::get_dragon_for_dragonrend();

                                        if (dragonrend_target)
                                        {
                                            WalkerProcessor::shout_at_target(dragonrend_target, dragonrend);
                                            goto finalize_shout; //skip casting part
                                        }
                                    }
                                    

                                    if (!fast)
                                        make_long_ult_cast();
                                    else
                                        use_ult();


                                    finalize_shout:

                                    std::string shout_name = shout->GetFullName();

                                    std::string words_text = MiscThings::get_shout_known_words_text(shout);
                                    if (words_text != "")
                                    {
                                        shout_name += " - " + words_text;
                                    }

                                    result.first = true;
                                    result.second = "[Using [id " + std::to_string(id) + "] " + shout_name;
                                }
                                else
                                {
                                    result.first = false;
                                    result.second = "You have not unlocked any words of this shout. ";
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





    void use_random_offensive_shout(RE::TESObjectREFR* target)
    {
        if (!target)
            return;

        //13e07 - unrelenting force

        std::vector<RE::TESShout*> shouts_available{};

        std::vector<RE::TESShout*> offensive_shouts{};


        offensive_shouts.push_back((RE::TESShout*)RE::TESForm::LookupByID(0x13e07)); //unrelenting force
        offensive_shouts.push_back((RE::TESShout*)RE::TESForm::LookupByID(0x70981)); // disarm
        offensive_shouts.push_back((RE::TESShout*)RE::TESForm::LookupByID(0x2395a)); // dismay
        offensive_shouts.push_back((RE::TESShout*)RE::TESForm::LookupByID(0x32921)); // elemental fury
        offensive_shouts.push_back((RE::TESShout*)RE::TESForm::LookupByID(0x3f9ea)); // fire breath
        offensive_shouts.push_back((RE::TESShout*)RE::TESForm::LookupByID(0x5d16b)); // frost breath
        offensive_shouts.push_back((RE::TESShout*)RE::TESForm::LookupByID(0x70980)); // ice form
        offensive_shouts.push_back((RE::TESShout*)RE::TESForm::LookupByID(0x7097c)); // marked for death
        offensive_shouts.push_back((RE::TESShout*)RE::TESForm::LookupByID(0x48ac9)); // slow time
        offensive_shouts.push_back((RE::TESShout*)RE::TESForm::LookupByID(0x7097d)); // storm call


        auto temp = get_available_spells(); //just refresh

        for (auto spell : spells)
        {
            if (spell.second.shout)
            {
                for (auto offensive_shout : offensive_shouts)
                {
                    if (spell.second.shout == offensive_shout)
                    {
                        int unlocked_words = 0;
                        for (auto variation : spell.second.shout->variations)
                        {
                            if (variation.word && (variation.word->formFlags & 65600) == 65600)
                                unlocked_words++;
                        }
                        if (unlocked_words > 0)
                            shouts_available.push_back(spell.second.shout);
                    }
                }
                
            }
        }

        auto amount_of_shouts = std::size(shouts_available);

        if (amount_of_shouts > 0)
        {
            int picked = random_int_from_range(0, amount_of_shouts - 1);

            WalkerProcessor::shout_at_target(target, shouts_available.at(picked));
        }

    }









    bool player_has_spell(RE::SpellItem* spell)
    {
        //REFRESH SPELLS. MIGHT HAVE SOMETHING NEW AND IT WILL NOT SEE REQUIRED SPELL
        auto get_spells_result = get_available_spells();

        for (auto spell_from_list : spells)
        {
            if (spell_from_list.second.spell == spell)
            {
                return true;
                break;
            }

            if (spell_from_list.second.shout == (RE::TESShout*)spell)
            {
                return true;
                break;
            }
        }

        return false;
    }


    std::pair<bool, std::string> cast_spell_by_refr(RE::SpellItem* spell, bool fast)
    {

        std::pair<bool, std::string> result{};

        if (MiscThings::is_intro() || MiscThings::is_intro2())
        {
            result.first = false;
            result.second = "Cannot use spells right now. ";
            return result;
        }

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)(player->AsReference());



        if (std::size(spells) <= 0)
        {
            auto get_spells_result = get_available_spells();
            //send_random_context("Available spells: " + get_spells_result.second);
        }

        int id = -1;
        for (auto spell_from_list : spells)
        {
            if (spell_from_list.second.spell == spell)
            {
                id = spell_from_list.first;
                break;
            }

            if (spell_from_list.second.shout == (RE::TESShout*)spell)
            {
                id = spell_from_list.first;
                break;
            }
        }

        if (id != -1)
            return cast_spell_by_index(id, fast);
        else
            return { false, "You dont have this spell" };
    }


    std::pair<bool, std::string> equip_spell_by_refr(RE::SpellItem* spell)
    {

        std::pair<bool, std::string> result{};

        if (MiscThings::is_intro() || MiscThings::is_intro2())
        {
            result.first = false;
            result.second = "Cannot use spells right now. ";
            return result;
        }

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)(player->AsReference());



        if (std::size(spells) <= 0)
        {
            auto get_spells_result = get_available_spells();
            //send_random_context("Available spells: " + get_spells_result.second);
        }

        int id = -1;
        for (auto spell_from_list : spells)
        {
            if (spell_from_list.second.spell == spell)
            {
                id = spell_from_list.first;
                break;
            }

            if (spell_from_list.second.shout == (RE::TESShout*)spell)
            {
                id = spell_from_list.first;
                break;
            }
        }

        if (id != -1)
            return equip_spell_by_index(id);
        else
            return { false, "You dont have this spell" };
    }



    //decided to leave its ability to cast too in case they get confused
    std::pair<bool, std::string> equip_spell_by_index(int id, bool fast)
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



        if (std::size(spells) <= 0)
        {
            auto get_spells_result = get_available_spells();
            send_random_context("Available spells: " + get_spells_result.second);
        }


        if (player_actor && spells.find(id) != spells.end())
        {
            auto spell_entry = spells.find(id);

            if (spell_entry->second.spell)
            {
                RE::SpellItem* spell = spell_entry->second.spell;

                auto equip_slot = spell->GetEquipSlot();

                auto slot_id = equip_slot->GetFormID();

                auto equip_manager = RE::ActorEquipManager::GetSingleton();

                if (equip_manager)
                {
                    if (slot_id == 0x00025BEE) //voice
                    {
                        equip_manager->EquipSpell(player_actor, spell, equip_slot);
                        right_attack_cancel();
                        left_attack_cancel();
                        use_ult();
                        result.first = true;
                        if (spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell)
                            result.second = "[Casting spell:  [id " + std::to_string(id) + "] " + spell->GetFullName() + "]";
                        else
                            result.second = "[Using ult: [id " + std::to_string(id) + "] " + spell->GetFullName() + "]";
                    }
                    else
                    {
                        if (slot_id == 0x00013F44) //either hand
                        {

                            auto slot = get_free_slot();

                            if (!is_offensive_spell(spell))
                                slot = (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F43); //non offensive spells go in left hand

                            std::string equip_hand = "";

                            bool right_hand = false;

                            if (slot == (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F42)) //right hand
                            {
                                right_hand = true;
                                equip_hand = " in right hand";

                                if (get_hand_contents(true) == spell)
                                {
                                    result.first = true;
                                    result.second = "You already have [id " + std::to_string(id) + "] " + spell->GetFullName() + equip_hand + "]";
                                    return result;
                                }

                            }


                            if (slot == (RE::BGSEquipSlot*)RE::TESForm::LookupByID(0x00013F43)) //left hand
                            {

                                equip_hand = " in left hand";

                                if (get_hand_contents(false) == spell)
                                {
                                    result.first = true;
                                    result.second = "You already have [id " + std::to_string(id) + "] " + spell->GetFullName() + equip_hand + "]";
                                    return result;
                                }
                            }
                                

                            WalkerProcessor::reset_attacking_inanimate_object_time();

                            equip_manager->EquipSpell(player_actor, spell, slot);
                            result.first = true;
                            if (spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell)
                            {
                                if (player_actor && !player_actor->IsWeaponDrawn() && !(player_actor->actorState2.weaponState == RE::WEAPON_STATE::kDrawing))
                                {
                                    ready_weapon(); //show off new weapon

                                    if (player_hp_less_than(90.0f) && is_self_healing_spell(spell))
                                    {
                                        try_casting_hand(right_hand);
                                    }
                                }

                                result.second = "[Equipped [id " + std::to_string(id) + "] " + spell->GetFullName() + equip_hand + "]";
                            }
                            else
                                result.second = "[Casting [id " + std::to_string(id) + "] " + spell->GetFullName() + equip_hand + "]";
                        }
                        else
                        {
                            result.first = false;
                            result.second = "Cannot use [id " + std::to_string(id) + "] " + spell->GetFullName() + "]";
                        }
                    }
                }
            }
            else
            {
                if (spell_entry->second.shout)
                {
                    RE::TESShout* shout = spell_entry->second.shout;

                    auto equip_slot = shout->GetEquipSlot();

                    auto slot_id = equip_slot->GetFormID();

                    auto equip_manager = RE::ActorEquipManager::GetSingleton();

                    if (equip_manager)
                    {
                        if (slot_id == 0x00025BEE) //voice
                        {
                            if (player_actor->GetVoiceRecoveryTime() <= 0)
                            {
                                int unlocked_words = 0;

                                for (auto variation : shout->variations)
                                {
                                    if (variation.word && (variation.word->formFlags & 65600) == 65600)
                                        unlocked_words++;
                                }
                                if (unlocked_words > 0)
                                {
                                    equip_manager->EquipShout(player_actor, shout);
                                    //use_ult();
                                    right_attack_cancel();
                                    left_attack_cancel();
                                    if (!fast)
                                        make_long_ult_cast();
                                    else
                                        use_ult();

                                    result.first = true;
                                    result.second = "[Using [id " + std::to_string(id) + "] " + shout->GetFullName();
                                }
                                else
                                {
                                    result.first = false;
                                    result.second = "You have not unlocked any words of this shout. ";
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


    bool helgen_bride_destroyed()
    {
        bool result = false;
        auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
        if (threshold_quest)
            if (threshold_quest->GetCurrentStageID() >= 500)
                result = true;
        return result;
    }


    bool is_intro()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

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
        auto player = RE::PlayerCharacter::GetSingleton();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);


        bool result = false;
        auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
        if (threshold_quest)
            if (threshold_quest->GetCurrentStageID() < 200)
                result = true;

        result |= !can_interact;

        return result;
    }

    bool escaped_helgen()
    {
        bool result = false;
        auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
        if (threshold_quest)
            if (threshold_quest->GetCurrentStageID() >= 760)
                result = true;

        return result;
    }


    bool is_interior_cell()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        if (player)
        {
            auto parent_cell = player->GetParentCell();

            if (parent_cell)
            {
                if (parent_cell->IsInteriorCell())
                    return true;
            }
        }


        return false;
    }

    bool player_has_shouts_to_unlock()
    {

        auto player = RE::PlayerCharacter::GetSingleton();
        RE::TESNPC* player_npc = (RE::TESNPC*)RE::TESForm::LookupByID(0x7);
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
                    if (variation.word && (variation.word->formFlags & 65600) == 65600)
                        unlocked_words++;

                    if (variation.word && variation.word->GetKnown())
                        known_words++;

                    max_words++;
                }

                if (unlocked_words < known_words && dragon_souls > 0)
                    return true;

            }
        }
        return false;
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


    bool is_enemy_to_actor(RE::TESObjectREFR* object, bool only_fighting)
    {
        if (object && object->IsActor() && !object->IsDead())
        {
            auto actor_refr = (RE::Actor*)object;

            auto controller = actor_refr->combatController;

            auto player = RE::PlayerCharacter::GetSingleton();
            auto player_ref = player->AsReference();
            auto player_actor = (RE::Actor*)player_ref;


            if (controller)
            {
                auto target_handle = controller->targetHandle;
                auto target_ref = RE::TESObjectREFR::LookupByHandle(target_handle.native_handle());

                if (actor_refr->race->fullName != "Dragon Race" || is_fighting_dragons_allowed())
                {
                    if (target_ref && target_ref.get() == player_ref)
                        return true;
                    else
                    {
                        auto is_enemy = actor_refr->GetFactionReaction(player_actor);

                        if (is_enemy == RE::FIGHT_REACTION::kEnemy)
                        {
                            if (controller->startedCombat)
                                return true;
                        }
                    }
                }
            }
            else
            {
                if (!only_fighting && WalkerProcessor::has_bow_equipped(WalkerProcessor::get_current_active_hand()) || WalkerProcessor::has_crossbow_equipped(WalkerProcessor::get_current_active_hand()))
                {
                    bool aggressive = false;

                    auto base_object = actor_refr->GetBaseObject();

                    if (base_object)
                    {
                        auto npc = (RE::TESNPC*)base_object;



                        if (npc->aiData.aggression2 || npc->aiData.aggression1 || (npc->aiData.aggroRadiusBehaviour && ((npc->aiData.aggroRadius[1] + 1000) > player->GetDistance(actor_refr))))
                            aggressive = true;
                    }

                    if (aggressive && actor_refr->IsHostileToActor(player_actor))
                        return true;



                }

            }
        }

        return false;
    }


    bool is_offensive_spell(bool right)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            RE::SpellItem* spell = (RE::SpellItem*)MiscThings::get_hand_contents(right);

            if (spell && (spell->GetFormType() == RE::FormType::Spell || spell->GetFormType() == RE::FormType::Scroll))
                if (spell->GetSpellType() != RE::MagicSystem::SpellType::kEnchantment)
                {
                    auto slot_both_hands = RE::TESForm::LookupByID(0x00013F45);

                    if (spell->GetEquipSlot() == slot_both_hands)
                        return true; //otherwise this will not let us cast 2handed spells/scrolls


                    for (auto effect : spell->effects)
                    {
                        if (effect->IsHostile())
                        {
                            result = true;
                            break;
                        }
                    }
                }

        }
        return result;
    }




    bool is_immortal(RE::Actor* actor)
    {
        bool result = false;

        if (actor->IsActor())
        {
            if (actor->boolFlags)
                result |= actor->boolFlags.any(RE::Actor::BOOL_FLAGS::kEssential);// || right->boolFlags.any(RE::Actor::BOOL_FLAGS::kProtected);

            result |= actor->IsChild();
            result |= actor->IsInvulnerable();
            
            if (actor->currentProcess && actor->currentProcess->cachedValues && actor->currentProcess->cachedValues->flags && actor->currentProcess->cachedValues->flags.any(RE::CachedValues::Flags::kActorIsGhost))
                result |= true;
        }


        return result;
    }


    std::vector<RE::Actor*> get_player_attackers(bool raycastable_only, RE::TESObjectREFR* exclude_ref, bool only_fighting, float range)
    {
        std::vector<RE::Actor*> result{};


        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();
        auto player_actor = (RE::Actor*)player_ref;


        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        if (!can_walk && !can_look)
            return result;

        
        //float scan_distance = 3000.0f;
        //auto player_cell = player->GetParentCell();
        //if (player_cell && player_cell->IsInteriorCell())
        //    range = 5000.0f;

        RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 9000.0f,
            //player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
            [&](RE::TESObjectREFR* a_ref) {

                if (a_ref && a_ref->IsActor())
                {
                    if (MiscThings::kataria_exists() && MiscThings::is_object_inside_of_kataria(a_ref) && !MiscThings::is_object_inside_of_kataria(player))
                        return RE::BSContainer::ForEachResult::kContinue; //skip kataria sailors if we escaped. this ship is cursed


                    if (is_enemy_to_actor(a_ref, only_fighting) && (!raycastable_only || raycastable(a_ref, range, false)))
                    {

                        auto target_actor = (RE::Actor*)a_ref;

                        bool not_a_threat = MiscThings::is_immortal(target_actor) && target_actor->GetActorValue(RE::ActorValue::kHealth) < 2;

                        if (!not_a_threat && (a_ref != exclude_ref) && !a_ref->IsChild())
                        {
                            if (MiscThings::is_dragon(a_ref))
                            {
                                result.push_back((RE::Actor*)a_ref);
                            }
                            else
                            {
                                auto distance = player_ref->GetDistance(a_ref);

                                if (distance < range)
                                    result.push_back((RE::Actor*)a_ref);
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

            bool left_immortal = is_immortal(left);
            bool right_immortal = is_immortal(right);;
            
            if (left_immortal != right_immortal)
                return left_immortal < right_immortal;


            return pos_left.GetDistance(player_pos) < pos_right.GetDistance(player_pos);

            });



        return result;
    }


    bool is_new_object_valid(RE::TESObjectREFR* a_ref)
    {
        if (!a_ref)
            return false;

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

        if (name[0] != '\0' && std::size(name) > 1 && name != player_name && name != "Sit")
        {

            if (MiscThings::has_digits(name))
                return false;

            if (a_ref->AsReference()->modelState == 0)
                return false; //skip objects without world model


            if (a_ref->IsDisabled())
                return false;

            if (base_type == RE::FormType::Activator)
            {
                auto test = (RE::TESObjectACTI*)base_obj;
                std::string model = test->GetModel();
                if (model.find("Marker_LinkMarker") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                    return false;

                //little flags
                if (model.find("MapFlag") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                    return false;
            }



            if (name.find("not be visible") != std::string::npos) //"This should not be visible [Furniture]"
                return false;

            if (name.find("Do Not Delete") != std::string::npos)
                return false;

            if (name.find("nvisible") != std::string::npos && name.find("arker") != std::string::npos)
                return false;

            if (name.find("default") != std::string::npos)
                return false;



            if (auto extra = a_ref->extraList.GetByType(RE::ExtraDataType::kItemDropper); extra)
            {
                auto extra_dropper = (RE::ExtraItemDropper*)extra;

                if (extra_dropper && extra_dropper->dropper && extra_dropper->dropper.get())
                {
                    auto dropper = extra_dropper->dropper.get().get();

                    if (dropper)
                    {
                        if (dropper->IsActor())
                        {
                            return false; //exclude dropped items, they have weird position.
                        }
                    }
                }
            }


            if (a_ref->AsReference()->IsActor())
            {
                return true;
            }


            if (base_type == RE::FormType::Door)
            {
                auto object_p = General::Script::GetObject(a_ref, "TrapTriggerHinge");
                if (object_p)
                {
                    //RE::BSFixedString prop_name = "trapLinkedRef";
                    //RE::TESObjectREFR* linked = General::Script::GetVariable<RE::TESObjectREFR*>(object_p, prop_name);
                    //if (linked) //THIS IS WHAT IS GOING TO BE TRIGGERED BY IT. the one who activates this, is in ActivateRef extradata of object
                    //{
                    //    result += ", attatched to " + insert_object_into_list_and_get_info(linked);
                    //}

                    if (object_p->currentState != "Active")
                    {
                        return false;
                    }
                }

                auto extra = a_ref->extraList.GetByType(RE::ExtraDataType::kFlags);
                if (extra)
                {
                    auto extra_flags = (RE::ExtraFlags*)extra;
                    if (extra_flags->flags.any(RE::ExtraFlags::Flag::kBlockActivate))
                        return false;
                }



                return true;
            }



            if (base_type == RE::FormType::Activator)
            {
                return true;
            }

            if (base_type == RE::FormType::Furniture) //pullchains/levers
            {
                auto furniture = (RE::TESFurniture*)base_obj;
                auto workbenchtype = furniture->workBenchData.benchType;
                if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kNone)
                {
                    if (furniture->HasKeywordString("ActivatorLever") || furniture->HasKeywordString("isPullChain"))
                    {
                        return true;
                    }
                }
            }





            if (base_type == RE::FormType::Container)
            {
                return true;
            }

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

                    auto test_flags = a_ref->AsReference()->GetFormFlags();
                    bool already_harvested = false;
                    if (test_flags & RE::TESObjectREFR::RecordFlags::kHarvested) //THIS FLAG IS POTENTIALLY INCORRECT.
                        already_harvested = true;

                    if (tree_form->produceItem && !already_harvested)
                        is_harvestable = true;
                }


                if (is_harvestable)
                {
                    return true;
                }
            }

            if (base_type == RE::FormType::Furniture) //workbenches and beds
            {
                bool this_isnt_a_furniture = false;
                auto furniture = (RE::TESFurniture*)base_obj;
                auto workbenchtype = furniture->workBenchData.benchType;
                if (workbenchtype != RE::TESFurniture::WorkBenchData::BenchType::kNone)
                {
                    return true;
                }
                else
                {
                    if (furniture->furnFlags.any(RE::TESFurniture::ActiveMarker::kCanSleep))
                        return true;
                }
            }

            if (base_obj->IsInventoryObject())
            {
                return true;
            }
        }

        return true;
    }




    RE::TESObjectREFR* find_distant_unseen_reference(float min_dist, float max_dist)
    {
        RE::TESObjectREFR* result = nullptr;

        auto player_ref = RE::PlayerCharacter::GetSingleton()->AsReference();



        std::vector<RE::TESObjectREFR*> candidates{};


        RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, max_dist,
            [&](RE::TESObjectREFR* a_ref) {

                if (a_ref)
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

                    //first exclude fillaments
                    if (base_type == RE::FormType::Hazard)
                        return RE::BSContainer::ForEachResult::kContinue;

                    if (base_type == RE::FormType::Static)
                        return RE::BSContainer::ForEachResult::kContinue;

                    //only then check if exists
                    if (MiscThings::is_object_in_the_list(a_ref))
                        return RE::BSContainer::ForEachResult::kContinue;


                    std::string name = a_ref->GetName();
                    std::string player_name = RE::PlayerCharacter::GetSingleton()->GetName();


                    if (name[0] != '\0' && std::size(name) > 1 && name != player_name)
                    {
                        if (base_type == RE::FormType::Door)
                            return RE::BSContainer::ForEachResult::kContinue;

                        if (MiscThings::has_digits(name))
                            return RE::BSContainer::ForEachResult::kContinue;

                        if (a_ref->AsReference()->modelState == 0)
                            return RE::BSContainer::ForEachResult::kContinue; //skip objects without world model


                        if (base_type == RE::FormType::Activator)
                        {
                            auto test = (RE::TESObjectACTI*)base_obj;
                            std::string model = test->GetModel();
                            if (model.find("Marker_LinkMarker") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                                return RE::BSContainer::ForEachResult::kContinue;

                            //little flags
                            if (model.find("MapFlag") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
                                return RE::BSContainer::ForEachResult::kContinue;
                        }



                        if (name.find("not be visible") != std::string::npos) //"This should not be visible [Furniture]"
                            return RE::BSContainer::ForEachResult::kContinue;

                        if (name.find("Do Not Delete") != std::string::npos)
                            return RE::BSContainer::ForEachResult::kContinue;

                        if (name.find("nvisible") != std::string::npos && name.find("arker") != std::string::npos)
                            return RE::BSContainer::ForEachResult::kContinue;

                        if (name.find("default") != std::string::npos)
                            return RE::BSContainer::ForEachResult::kContinue;



                        if (auto extra = a_ref->extraList.GetByType(RE::ExtraDataType::kItemDropper); extra)
                        {
                            auto extra_dropper = (RE::ExtraItemDropper*)extra;

                            if (extra_dropper && extra_dropper->dropper && extra_dropper->dropper.get())
                            {
                                auto dropper = extra_dropper->dropper.get().get();

                                if (dropper)
                                {
                                    if (dropper->IsActor())
                                    {
                                        return RE::BSContainer::ForEachResult::kContinue; //exclude dropped items, they have weird position.
                                    }
                                }
                            }
                        }


                        if (a_ref->AsReference()->IsActor())
                        {
                            if (!MiscThings::is_object_in_the_list(a_ref))
                                if (player_ref->GetDistance(a_ref) > min_dist)
                                {
                                    candidates.push_back(a_ref);
                                    return RE::BSContainer::ForEachResult::kContinue;
                                }
                        }


                        if (base_type == RE::FormType::Door)
                        {
                            if (!MiscThings::is_object_in_the_list(a_ref))
                                if (player_ref->GetDistance(a_ref) > min_dist)
                                {
                                    candidates.push_back(a_ref);
                                    return RE::BSContainer::ForEachResult::kContinue;
                                }
                        }



                        if (base_type == RE::FormType::Activator)
                        {
                            if (!MiscThings::is_object_in_the_list(a_ref))
                                if (player_ref->GetDistance(a_ref) > min_dist)
                                {
                                    candidates.push_back(a_ref);
                                    return RE::BSContainer::ForEachResult::kContinue;
                                }
                        }

                        if (base_type == RE::FormType::Furniture) //pullchains/levers
                        {
                            auto furniture = (RE::TESFurniture*)base_obj;
                            auto workbenchtype = furniture->workBenchData.benchType;
                            if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kNone)
                            {
                                if (furniture->HasKeywordString("ActivatorLever") || furniture->HasKeywordString("isPullChain"))
                                {
                                    if (!MiscThings::is_object_in_the_list(a_ref))
                                        if (player_ref->GetDistance(a_ref) > min_dist)
                                        {
                                            candidates.push_back(a_ref);
                                            return RE::BSContainer::ForEachResult::kContinue;
                                        }
                                }
                            }
                        }





                        if (base_type == RE::FormType::Container)
                        {
                            if (!MiscThings::is_object_in_the_list(a_ref))
                                if (player_ref->GetDistance(a_ref) > min_dist)
                                {
                                    candidates.push_back(a_ref);
                                    return RE::BSContainer::ForEachResult::kContinue;
                                }
                        }

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

                                auto test_flags = a_ref->AsReference()->GetFormFlags();
                                bool already_harvested = false;
                                if (test_flags & RE::TESObjectREFR::RecordFlags::kHarvested) //THIS FLAG IS POTENTIALLY INCORRECT.
                                    already_harvested = true;

                                if (tree_form->produceItem && !already_harvested)
                                    is_harvestable = true;
                            }


                            if (is_harvestable)
                            {
                                if (!MiscThings::is_object_in_the_list(a_ref))
                                    if (player_ref->GetDistance(a_ref) > min_dist)
                                    {
                                        candidates.push_back(a_ref);
                                        return RE::BSContainer::ForEachResult::kContinue;
                                    }
                            }
                        }

                        if (base_type == RE::FormType::Furniture) //workbenches and beds
                        {
                            bool this_isnt_a_furniture = false;
                            auto furniture = (RE::TESFurniture*)base_obj;
                            auto workbenchtype = furniture->workBenchData.benchType;
                            if (workbenchtype != RE::TESFurniture::WorkBenchData::BenchType::kNone)
                            {
                                if (!MiscThings::is_object_in_the_list(a_ref))
                                    if (player_ref->GetDistance(a_ref) > min_dist)
                                    {
                                        candidates.push_back(a_ref);
                                        return RE::BSContainer::ForEachResult::kContinue;
                                    }
                            }
                            else
                            {
                                if (furniture->furnFlags.any(RE::TESFurniture::ActiveMarker::kCanSleep))
                                    if (!MiscThings::is_object_in_the_list(a_ref))
                                        if (player_ref->GetDistance(a_ref) > min_dist)
                                        {
                                            candidates.push_back(a_ref);
                                            return RE::BSContainer::ForEachResult::kContinue;
                                        }
                            }
                        }

                        if (base_obj->IsInventoryObject())
                        {
                            if (!MiscThings::is_object_in_the_list(a_ref))
                                if (player_ref->GetDistance(a_ref) > min_dist)
                                {
                                    candidates.push_back(a_ref);
                                    return RE::BSContainer::ForEachResult::kContinue;
                                }
                        }
                    }
                }
                

                return RE::BSContainer::ForEachResult::kContinue;
            });



            std::sort(candidates.begin(), candidates.end(), [&](RE::TESObjectREFR* left, RE::TESObjectREFR* right) {
                if (left && right && left->data.objectReference && left->data.objectReference &&
                    right->data.objectReference && left->formID && right->formID)
                    return left->GetDistance(player_ref) < right->GetDistance(player_ref); //switch > to < for inversed order. this is last->closest
                else
                    return false;
                });

            if (std::size(candidates) > 0)
                result = candidates.at(0);

        return result;
    }


    //bool property doorOpened

    bool puzzle_door_open(RE::TESObjectREFR* a_ref)
    {
        if (!a_ref)
            return false;


        auto object_p = General::Script::GetObject(a_ref, "HallofStoriesKeyholeScript");
        if (object_p)
        {
            if (object_p->currentState == "Done")
                return true;
        }

        return false;
    }



    bool is_object_valid(RE::TESObjectREFR* a_ref, bool use_model_state)
    {
        if (!a_ref || !a_ref->data.objectReference || !a_ref->formID || !a_ref->data.objectReference->formID)
            return false;

        RE::ObjectRefHandle my_handle{};
        my_handle = a_ref->GetHandle();

        if ((!my_handle || !my_handle.get() || !my_handle.get().get()))
            return false;


        if (use_model_state && a_ref->AsReference()->modelState == 0)
            return false;

        if (a_ref->IsDisabled())
            return false;


        auto base_obj = a_ref->GetBaseObject();

        if (base_obj)
        {

            auto base_type = base_obj->GetFormType();

            bool is_harvestable = false;

            std::string name = "";
            name = a_ref->GetDisplayFullName();


            if (get_destructible_state(a_ref) != 0 && get_destructible_state(a_ref) != -1)
                return false;

            if (base_type == RE::FormType::Activator)
            {


                auto word_of_power = MiscThings::get_word_of_power(a_ref);

                if (word_of_power == (RE::TESObjectREFR*)(-1))
                {
                    return false;
                }




                if (puzzle_door_open(a_ref))
                    return false;

                auto extra = a_ref->extraList.GetByType(RE::ExtraDataType::kLinkedRef);

                if (extra)
                {
                    auto extra_linked = (RE::ExtraLinkedRef*)extra;

                    for (auto linked_ref : extra_linked->linkedRefs)
                    {
                        if (linked_ref.refr)
                        {
                            if (puzzle_door_open(linked_ref.refr))
                                return false;
                        }
                    }
                }
            }


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
                {
                    return true;
                }
                else
                    return false;
            }

            if (base_type == RE::FormType::Flora)
            {
                auto tree_form = (RE::TESFlora*)base_obj;

                auto test_flags = a_ref->AsReference()->GetFormFlags();
                bool already_harvested = false;
                if (test_flags & RE::TESObjectREFR::RecordFlags::kHarvested) //THIS FLAG IS POTENTIALLY INCORRECT.
                    already_harvested = true;

                if (tree_form->produceItem && !already_harvested)
                {
                    return true;
                }
                else
                    return false;
            }


            if (base_type == RE::FormType::Container)
            {
                return !MiscThings::is_container_empty(a_ref);
            }

        }

        return true;
    }






    std::string fix_book_description(std::string description)
    {
        bool something_found = false;

        if (auto pos = description.find("[pagebreak]"); pos != std::string::npos)
        {
            description.erase(pos, 11);
            description.insert(pos, " ");
            something_found = true;
        }

        if (auto pos = description.find("<"); pos != std::string::npos)
        {
            std::string letter_to_replace = "";
            auto pos_letter = description.find("_letter.png");

            if (pos_letter != std::string::npos)
                letter_to_replace = " " + description.substr(pos_letter - 1, 1);

            auto pos2 = description.find(">");
            description.erase(pos, pos2 - pos + 1);

            if (pos_letter <= pos2 && pos_letter >= pos)
                description.insert(pos, letter_to_replace);
            something_found = true;
        }

        /*
        if (auto pos = description.find("<"); pos != std::string::npos)
        {
            auto pos2 = description.find(">");
            description.erase(pos, pos2 - pos + 1);
            something_found = true;
        }
        */

        if (something_found)
            return fix_book_description(description);
        else
            return description;
    }



    bool is_object_still_valid(RE::TESObjectREFR* test_object)
    {
        auto player_ref = RE::PlayerCharacter::GetSingleton()->AsReference();


        bool found = false;

        //int dbg_int = 0;

        RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 30000.0,
            [&](RE::TESObjectREFR* a_ref) {
                {
                    if (a_ref && a_ref == test_object)
                    {
                        found = true;
                        return RE::BSContainer::ForEachResult::kStop;
                    }
                }
                return RE::BSContainer::ForEachResult::kContinue;
            });

        if (!found)
            return false;

        for (std::pair<int, object_data> object : objects_around)
        {
            if (object.second.object == test_object) //it existed in our list...
            {
                bool its_valid = false;
                /*
                RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 50000.0,
                    [&](RE::TESObjectREFR* a_ref) {
                        if (test_object == a_ref) //it exists right now...
                        {
                            its_valid = is_object_valid(test_object);
                            return RE::BSContainer::ForEachResult::kStop;
                        }

                        return RE::BSContainer::ForEachResult::kContinue;
                    });
                */
                its_valid = is_object_valid(test_object);
                return its_valid;
            }
        }
       
        return false;
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
        

        std::vector<std::pair<int, object_data>> local_copy{};


        RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 50000.0,
            [&](RE::TESObjectREFR* a_ref) {

                if (a_ref)
                {
                    for (std::pair<int, object_data> object : objects_around)
                    {
                        if (object.second.object == a_ref)
                        {
                            if (is_object_valid(a_ref))
                                local_copy.push_back(object);

                            return RE::BSContainer::ForEachResult::kContinue;
                        }
                    }
                }
                
                return RE::BSContainer::ForEachResult::kContinue;
            });


        std::sort(local_copy.begin(), local_copy.end(), [&](std::pair<int, object_data> left, std::pair<int, object_data> right) {
            //if (decltype(left.second) != RE::TESObjectREFR*)

            if (left.second.object && right.second.object && left.second.object->data.objectReference && right.second.object->data.objectReference && left.second.object->formID && right.second.object->formID)
                return left.second.object->GetDistance(player) < right.second.object->GetDistance(player); //switch > to < for inversed order. this is last->closest
            else
                return false;

            //left.second->data.objectReference->IsDestroyed();

            //std::string name_left = left.second->GetDisplayFullName();
            //std::string name_right = right.second->GetDisplayFullName();
            //return name_left < name_right; //alphabetical order. top = A

            });


        bool veryclose_line_made = false;
        bool nearby_line_made = false;
        bool faraway_line_made = false;

        std::string last_name = "";
        bool has_last = false;

        for (auto object : local_copy)
        {
            auto this_object = object.second.object;

            if (this_object->data.objectReference && this_object->formID)
            {
                if (player_ref != this_object)
                {

                    std::string dbg_name = this_object->GetDisplayFullName();

                    if (dbg_name.find("Pillar") != std::string::npos)
                        bool stop_here = true; 

                    if (player_ref->GetDistance(this_object) < 450.0f)
                        if (!veryclose_line_made)
                        {
                            std::string last_name = "";
                            bool has_last = false;
                            result.second += "\nVery close:\n";
                            veryclose_line_made = true;
                        }

                    if (player_ref->GetDistance(this_object) >= 450.0f && player_ref->GetDistance(this_object) < 2000.0f)
                        if (!nearby_line_made)
                        {
                            std::string last_name = "";
                            bool has_last = false;
                            result.second += "\nNearby:\n";
                            nearby_line_made = true;
                        }


                    bool no_faraways = false;
                    if (is_interior_cell())
                    {
                        std::string probe_name = insert_object_into_list_and_get_info(this_object);

                        if (probe_name.find("Word of Power") == std::string::npos && !MiscThings::ignore_faraway_interior(this_object))
                            no_faraways = true;
                    }

                    if (!no_faraways)
                    {
                        if (player_ref->GetDistance(this_object) >= 2000.0f && player_ref->GetDistance(this_object) < 10000.0f)
                            if (!faraway_line_made)
                            {
                                if (type == 1)
                                    return result; //short range

                                std::string last_name = "";
                                bool has_last = false;
                                result.second += "\nFar away:\n";
                                faraway_line_made = true;
                            }
                    }

                    float max_range = 10000.0f;
                    if (no_faraways)
                        max_range = 2000.0f;

                    if (player_ref->GetDistance(this_object) < max_range)
                    {
                        //std::string category = get_object_category(object.second);

                        std::string result_name = insert_object_into_list_and_get_info(this_object); //they are all in the list but whatever. just to get the name

                        auto id_end = result_name.find_first_of("]");

                        if (result_name != "" && id_end != std::string::npos)
                        {
                            std::string name_no_id = result_name.substr(id_end + 1, result_name.length() - id_end);
                            std::string id_text_raw = result_name.substr(0, id_end + 1);
                            std::string id_text = result_name.substr(4, id_end - 4);

                            if (has_last && name_no_id == last_name)
                            {
                                auto last_start = result.second.rfind("\n", result.second.length() - 2);

                                auto last_id_start = result.second.find("[id", last_start);

                                if (last_id_start == std::string::npos || last_start == std::string::npos)
                                {
                                    result.second += result_name + "\n";
                                }
                                else
                                {
                                    if (result.second.substr(last_id_start + 3, 1) != "s")
                                    {
                                        result.second.insert(last_id_start + 3, "s");
                                    }

                                    auto last_substr = result.second.substr(last_id_start, result.second.length() - last_id_start);

                                    auto last_id_sub_end = last_substr.find_first_of("]");

                                    auto last_id_insert_pos = last_id_start + last_id_sub_end;

                                    result.second.insert(last_id_insert_pos, ", " + id_text);
                                }

                            }
                            else
                                result.second += result_name + "\n";

                            last_name = name_no_id;
                            has_last = true;
                        }

                    }
                }
                
            }

        }


        if (result.second == "")
            result.second = "You see nothing interesting nearby... maybe follow some quest or explore?";

        result.first = true;

        return result;
    }





    int get_id_for_an_object(RE::TESForm* object)
    {
        //check existing lists. if found - return its id. if not found, make up new id.

        
        int result = -1;
        int candidate = 0;

        while (result == -1)
        {
            if (spells.find(candidate) == spells.end() &&
                inventory_items_list.find(candidate) == inventory_items_list.end() &&
                objects_around.find(candidate) == objects_around.end() &&
                locations_around.find(candidate) == locations_around.end())

                result = candidate;
        }

        return result;
    }




}
