

#pragma once

#include "main.hpp"

namespace Observer {


	void detect_threats(float dtime);
	void reset_threats();
	std::pair<bool, std::string> set_threat_response_choice(int id);

	void set_threat_action_taken();

	void detect_interesting_objects(float dtime);

	void inventory_monitor(float dtime);

	void clear_objects_to_track();
	void detect_events(float dtime);
	void player_state_monitor(float dtime);
	void reset_observer();


	void detect_locations(float dtime);

	bool are_surroundings_scanned();

	bool can_surrender_to_guards();



	void attatch_heatmap();

	class EventSink :
		public REX::TSingleton<EventSink>,
		public RE::BSTEventSink<RE::TESHitEvent>
	{
	private:
		bool* hitmap_lock = nullptr;
		std::map<RE::TESObjectREFR*, long long>* player_hit_info = nullptr;
		void (*send_random_context)(std::string context);
	public:
		void attatch_hitmap(std::map<RE::TESObjectREFR*, long long>* map, bool* lock, void (*context_sender)(std::string context))
		{
			player_hit_info = map;
			hitmap_lock = lock;
			send_random_context = context_sender;

		}

		void Init()
		{
			RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESHitEvent>(this);
		}

		virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>*) override
		{
			if (a_event)
			{
				auto player = RE::PlayerCharacter::GetSingleton();
				auto player_ref = player->AsReference();

				RE::TESObjectREFRPtr target = a_event->target;
				RE::TESObjectREFRPtr agressor = a_event->cause;
				RE::FormID weapon_formid = a_event->source;

				if (target && agressor)
				{
					auto target_ref = target.get();
					auto agressor_ref = agressor.get();

					if (target_ref == player_ref)
					{
						//player_hit_info.insert_or_assign({ agressor_ref, {} })

						if (player_hit_info && hitmap_lock && !*hitmap_lock && send_random_context)
						{
							*hitmap_lock = true;

							long long now_time = std::chrono::steady_clock::now().time_since_epoch().count();

							std::string agressor_name = MiscThings::insert_object_into_list_and_get_info(agressor_ref);
							std::string weapon_name = "";

							if (agressor_name != "")
							{
								if (weapon_formid)
								{
									auto weapon_form = RE::TESObjectREFR::LookupByID(weapon_formid);
									if (weapon_form)
									{
										weapon_name = weapon_form->GetName();
									}
								}

								if (weapon_name != "")
									weapon_name = " with " + weapon_name;


								auto hit_event = player_hit_info->find(agressor_ref);
								if (hit_event != player_hit_info->end())
								{
									if ((now_time - hit_event->second) > 10000000000) //1 second = 1 000 000 000
									{
										if (agressor_name != "")
										{
											send_random_context(agressor_name + " hits you" + weapon_name + "!");
											hit_event->second = now_time;
										}
									}
								}
								else
								{
									player_hit_info->insert({ agressor_ref, now_time });
									send_random_context(agressor_name + " hits you" + weapon_name + "!");
								}
							}

							*hitmap_lock = false;
						}
					}
				}
			}

			return RE::BSEventNotifyControl::kContinue;
		}
	};

}
