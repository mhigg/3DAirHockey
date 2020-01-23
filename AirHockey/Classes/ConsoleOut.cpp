#include "ConsoleOut.h"

ConsoleOut::ConsoleOut(void)
	: mpImp(NULL)
{
}

ConsoleOut::~ConsoleOut(void)
{
}

void ConsoleOut::update(void)
{
}

void ConsoleOut::write(const ExitGames::Common::JString& str)
{
#if defined _EG_MARMALADE_PLATFORM
	std::cout << str.UTF8Representation().cstr();
#else
	std::wcout << str;
#endif
#if defined _EG_WINDOWS_PLATFORM || defined _EG_WINDOWSSTORE_PLATFORM || defined _EG_XB1_PLATFORM
	OutputDebugStringW(str.cstr());
#endif
}

void ConsoleOut::writeLine(const ExitGames::Common::JString& str)
{
#if defined _EG_MARMALADE_PLATFORM
	std::cout << str.UTF8Representation().cstr() << std::endl;
#else
	std::wcout << str << std::endl;
#endif
#if defined _EG_WINDOWS_PLATFORM || defined _EG_WINDOWSSTORE_PLATFORM || defined _EG_XB1_PLATFORM
	OutputDebugStringW((str + L"\n").cstr());
#endif
}

ConsoleOut& ConsoleOut::get(void)
{
	static ConsoleOut console;
	return console;
}