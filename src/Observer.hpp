

#pragma once

namespace Observer {


	void detect_threats(float dtime);
	void reset_threats();
	std::pair<bool, std::string> set_threat_response_choice(int id);

	void set_threat_action_taken();

	void detect_interesting_objects(float dtime);


	void clear_objects_to_track();
	void detect_events(float dtime);
	void player_state_monitor(float dtime);
	void reset_observer();


	void detect_locations(float dtime);

}
