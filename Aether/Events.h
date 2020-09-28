#pragma once
#include <functional>
#include <queue>

namespace EngineSystems {

	class Message {
	public:
		enum Type {
			Logic_Event
		};

		Type type;
		std::function<void(int)> func;
		int arguments;
	};

	class EventHandler {
	public:
		void RegisterEvent(Message msg);
		void HandleEvents();

		std::queue<Message> messageQueue;
	};

}