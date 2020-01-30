#include "OutputListener.h"

class ConsoleImplementation;

class ConsoleOut : public OutputListener
{
public:
	ConsoleOut(void);
	~ConsoleOut(void);
	void update(void);
	void write(const ExitGames::Common::JString& str);
	void writeLine(const ExitGames::Common::JString& str = ExitGames::Common::JString());
	static ConsoleOut& get(void);
private:
	ConsoleImplementation* mpImp;
};