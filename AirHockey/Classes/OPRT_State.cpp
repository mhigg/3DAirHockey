#include <type_traits>
#include "OPRT_state.h"

OPRT_State::OPRT_State()
{
	// ‰Šú‰»
	for (auto data : INPUT_ID())
	{
		state.emplace(data, TRIGGER_STATE::OFF);
	}
}

OPRT_State::~OPRT_State()
{
}

TRIGGER_STATE OPRT_State::GetState(INPUT_ID inputId)
{
	return state[inputId];
}

INPUT_ID begin(INPUT_ID)
{
	return INPUT_ID::LEFT;
}

INPUT_ID end(INPUT_ID)
{
	return INPUT_ID::MAX;
}

INPUT_ID operator++(INPUT_ID & key)
{
	// std::underlying_type<Z>::type(Z)	—ñ‹“Œ^‚ÌŒ^‚ğæ“¾‚µ·¬½Ä‚·‚é
	return key = INPUT_ID(std::underlying_type<INPUT_ID>::type(key) + 1);
}

INPUT_ID operator*(INPUT_ID & key)
{
	return key;
}