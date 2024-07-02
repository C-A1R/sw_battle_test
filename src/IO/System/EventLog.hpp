#pragma once

#include <iostream>
#include <typeindex>
#include <unordered_map>

#include <IO/System/PrintDebug.hpp>

namespace sw
{
	class EventLog 
	{
		EventLog() = default;
		
	public:
		static EventLog &instance()
		{
			static EventLog el;
			return el;
		}

		template <class TEvent>
		void log(uint64_t tick, TEvent&& event)
		{
			std::cout << "[" << tick << "] " << TEvent::Name << " ";
			PrintFieldVisitor visitor(std::cout);
			event.visit(visitor);
			std::cout << std::endl;
		}
	};
}