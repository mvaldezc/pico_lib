#pragma once
#include <cstdint>

enum class MockEvent : uint8_t
{
    Forward,
    Backward
};

enum class MockStateId : uint8_t
{
    First,
    Second,
    Third
};

inline MockStateId mockStateTransMatrix(MockStateId currentState, MockEvent event)
{
    switch (currentState)
    {
        case MockStateId::First:
            if (event == MockEvent::Forward)
            {
                return MockStateId::Second;
            }
            break;
        case MockStateId::Second:
            if (event == MockEvent::Backward)
            {
                return MockStateId::First;
            }
            else if (event == MockEvent::Forward)
            {
                return MockStateId::Third;
            }
            break;
        case MockStateId::Third:
            if (event == MockEvent::Backward)
            {
                return MockStateId::Second;
            }
            break;
        default:
            break;
    }
    return currentState;
}
