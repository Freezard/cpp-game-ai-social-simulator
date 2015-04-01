#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
	Msg_WannaHangOut,
	Msg_LetsHangOut,
	Msg_MeetingTime
};

inline std::string MsgToStr(int msg)
{
	switch (msg)
	{
	case Msg_WannaHangOut:

		return "WannaHangOut"; 

	case Msg_LetsHangOut:

		return "LetsHangOut";

	case Msg_MeetingTime:

		return "MeetingTime";

	default:

		return "Not recognized!";
	}
}

#endif