#include "Events.h"
#include <iostream>
#include "Entity.h"

namespace EngineSystems {

	void EventHandler::RegisterEvent(Message msg) {
		messageQueue.push(msg);
		std::cout << "Event Registered" << std::endl;
	}

	void EventHandler::HandleEvents() {
		for (size_t i = 0; i < messageQueue.size(); i++) {
			Message msg = messageQueue.front();

			if (msg.type == Message::Logic_Event) {
				msg.func(msg.arguments);
				messageQueue.pop();
				std::cout << "Event Processed" << std::endl;
			}
		}
	}

}