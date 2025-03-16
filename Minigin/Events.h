#pragma once
#include <iostream>
#include <variant>
#include <vector>
using EventArg = std::variant<int, float, std::string, void*>;
using EventID = unsigned int;

struct Event
{
	const EventID id;

	static const int MAX_ARGS = 8;
    int numberArgs;
	EventArg args[MAX_ARGS];

	template <typename T>
    void AddArg(T arg)
    {
        if (numberArgs < MAX_ARGS) // Check bounds
        {
            args[numberArgs] = arg;
            numberArgs++;
        }
    }
	
	explicit Event(EventID _id) : id(_id), numberArgs(0) {}
};
