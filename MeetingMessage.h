#ifndef MEETINGMESSAGE_H
#define MEETINGMESSAGE_H
//------------------------------------------------------------------------
//
//  Name:   MeetingMessage.h
//
//  Desc:   Simple class for storing meeting info
//
//------------------------------------------------------------------------
#include <iostream>
#include <math.h>
#include "Locations.h"

struct MeetingMessage
{
	double Time;
	location_type Location;

	MeetingMessage():Time(0),
		Location(location_type(bar))
	{}


	MeetingMessage(double time,
		location_type loc):Time(time),
		Location(loc)
	{}

};

#endif