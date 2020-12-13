#ifndef _EVENT_H
#define _EVENT_H

namespace Plutus
{
	enum class EventType
	{
		KEY,
		MOUSE,
		TOUCH,
		SYSTEM
	};

	class Event
	{
	private:
		EventType type;
		unsigned int m_icode;
		char m_ccode;
	public:
		Event();
		~Event();
	};

} // namespace Plutus

#endif
