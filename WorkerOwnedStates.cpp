#include "WorkerOwnedStates.h"
#include "State.h"
#include "Worker.h"
#include "Locations.h"
#include "Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "CrudeTimer.h"
#include "EntityNames.h"
#include "Food.h"
#include "utils.h"
#include "LocationManager.h"
#include "MeetingMessage.h"
#include "EntityManager.h"

#include <map>
#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

//------------------------------------------------------------------------methods for EnterGardenAndWaterFlowers
EnterGardenAndWaterFlowers* EnterGardenAndWaterFlowers::Instance()
{
	static EnterGardenAndWaterFlowers instance;

	return &instance;
}


void EnterGardenAndWaterFlowers::Enter(Worker* pWorker)
{
	if (pWorker->Location() != garden)
	{
		cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Walking to the garden";

		pWorker->ChangeLocation(garden);
	}

	// 8 hours of work before he can go home
	if (pWorker->GetTimeToGoHome() == 0)
		pWorker->SetTimeToGoHome(Clock->GetCurrentTime() + 8);
}


void EnterGardenAndWaterFlowers::Execute(Worker* pWorker)
{  
	pWorker->IncreaseFatigue();

	pWorker->AddToWealth(1);

	cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Watering the flowers";

	// go home if done working, or go drink if thirsty
	if (pWorker->TimeToGoHome())
	{
		pWorker->SetTimeToGoHome(0);	  
		pWorker->GetFSM()->ChangeState(VisitStoreAndBuyFood::Instance());
	}
	else if (pWorker->Thirsty())
	{
		pWorker->GetFSM()->ChangeState(QuenchThirst::Instance());
	}
}


void EnterGardenAndWaterFlowers::Exit(Worker* pWorker)
{
	cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " 
		<< "Leaving the garden";
}


bool EnterGardenAndWaterFlowers::OnMessage(Worker* pWorker, const Telegram& msg)
{
	// can't meet while in this state
	switch(msg.Msg)
	{
	case Msg_LetsHangOut:
		{
			return LetsHangOut(pWorker, msg);
		}
	case Msg_MeetingTime:
		{
			pWorker->SetBooked(false);
		}

	}//end switch

	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------methods for EnterForestAndCutTrees

EnterForestAndCutTrees* EnterForestAndCutTrees::Instance()
{
	static EnterForestAndCutTrees instance;

	return &instance;
}


void EnterForestAndCutTrees::Enter(Worker* pWorker)
{
	if (pWorker->Location() != forest)
	{
		cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Walking to the forest";

		pWorker->ChangeLocation(garden);
	}

	// 5 hours of work before he can go home
	if (pWorker->GetTimeToGoHome() == 0)
		pWorker->SetTimeToGoHome(Clock->GetCurrentTime() + 5);
}


void EnterForestAndCutTrees::Execute(Worker* pWorker)
{  
	pWorker->IncreaseFatigue();

	pWorker->AddToWealth(2);

	cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Collecting some wood";

	// go home if done working, or go drink if thirsty
	if (pWorker->TimeToGoHome())
	{
		pWorker->SetTimeToGoHome(0);	  
		pWorker->GetFSM()->ChangeState(VisitStoreAndBuyFood::Instance());
	}
	else if (pWorker->Thirsty())
	{
		pWorker->GetFSM()->ChangeState(QuenchThirst::Instance());
	}
}


void EnterForestAndCutTrees::Exit(Worker* pWorker)
{
	cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " 
		<< "Leaving the forest";
}


bool EnterForestAndCutTrees::OnMessage(Worker* pWorker, const Telegram& msg)
{
	// can't meet while in this state
	switch(msg.Msg)
	{
	case Msg_LetsHangOut:
		{
			return LetsHangOut(pWorker, msg);
		}
	case Msg_MeetingTime:
		{
			pWorker->SetBooked(false);
		}

	}//end switch

	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------methods for VisitStoreAndBuyFood

VisitStoreAndBuyFood* VisitStoreAndBuyFood::Instance()
{
	static VisitStoreAndBuyFood instance;

	return &instance;
}

void VisitStoreAndBuyFood::Enter(Worker* pWorker)
{  
	if (pWorker->Location() != store)
	{
		cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Going to the store";

		pWorker->ChangeLocation(store);
	}
}

void VisitStoreAndBuyFood::Execute(Worker* pWorker)
{   
	int rnd = RandInt(1, 3);

	// buy some random food
	pWorker->BoughtFood(food_type(rnd));

	// food has different costs
	pWorker->SetWealth(pWorker->Wealth() - rnd);

	cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " 
		<< "Buying food. Today I'm going to have: "<< GetFoodAsString(pWorker->Food());

	pWorker->GetFSM()->ChangeState(GoHomeAndEat::Instance());
}


void VisitStoreAndBuyFood::Exit(Worker* pWorker)
{
	cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Leaving the store";

	CheckIfSocialAndContactFriends(pWorker);
}

bool VisitStoreAndBuyFood::OnMessage(Worker* pWorker, const Telegram& msg)
{
	switch(msg.Msg)
	{
	case Msg_WannaHangOut:
		{
			return WannaHangOut(pWorker, msg);
		}
	case Msg_LetsHangOut:
		{
			return LetsHangOut(pWorker, msg);
		}
	case Msg_MeetingTime:
		{
			MeetingTime(pWorker, msg);
			return true;
		}

	}//end switch

	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------GoHomeAndEat

GoHomeAndEat* GoHomeAndEat::Instance()
{
	static GoHomeAndEat instance;

	return &instance;
}


void GoHomeAndEat::Enter(Worker* pWorker)
{
	if (pWorker->Location() != house)
	{
		cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Time to enter the kitchen and cook some food";

		pWorker->ChangeLocation(house); 
	}
}

void GoHomeAndEat::Execute(Worker* pWorker)
{
	int rnd = RandInt(0, 2);

	// loneniness might increase when eating
	if (rnd == 0)
		pWorker->IncreaseLoneniness();

	if (pWorker->Food() == food_type(none))
		cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "I have no food to eat!";
	else cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Eating my " + GetFoodAsString(pWorker->Food());

	pWorker->BoughtFood(food_type(none));

	pWorker->GetFSM()->ChangeState(GoHomeAndSleep::Instance());
}

void GoHomeAndEat::Exit(Worker* pWorker)
{ 
	cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Leaving the kitchen";
}

bool GoHomeAndEat::OnMessage(Worker* pWorker, const Telegram& msg)
{
	switch(msg.Msg)
	{
	case Msg_WannaHangOut:
		{
			return WannaHangOut(pWorker, msg);
		}
	case Msg_LetsHangOut:
		{
			return LetsHangOut(pWorker, msg);
		}
	case Msg_MeetingTime:
		{
			MeetingTime(pWorker, msg);
			return true;
		}

	}//end switch

	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------methods for GoHomeAndSleep

GoHomeAndSleep* GoHomeAndSleep::Instance()
{
	static GoHomeAndSleep instance;

	return &instance;
}

void GoHomeAndSleep::Enter(Worker* pWorker)
{
	if (pWorker->Location() != house)
	{
		cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Walking home";

		pWorker->ChangeLocation(house); 
	}
}

void GoHomeAndSleep::Execute(Worker* pWorker)
{ 
	// return to work if not tired, else sleep
	if (!pWorker->Fatigued())
	{
		cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " 
			<< "I'm ready for another day at work!";

		pWorker->GetFSM()->ChangeState(pWorker->CurrentWork());
	}
	else 
	{
		pWorker->DecreaseFatigue();

		cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "ZZZZ... ";
	} 
}

void GoHomeAndSleep::Exit(Worker* pWorker)
{ 
	cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Total money: " << pWorker->Wealth();

	cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Leaving my house";
}


bool GoHomeAndSleep::OnMessage(Worker* pWorker, const Telegram& msg)
{
	// can't meet while in this state
	switch(msg.Msg)
	{
	case Msg_LetsHangOut:
		{
			return LetsHangOut(pWorker, msg);
		}
	case Msg_MeetingTime:
		{
			pWorker->SetBooked(false);
		}

	}//end switch

	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
	static QuenchThirst instance;

	return &instance;
}

void QuenchThirst::Enter(Worker* pWorker)
{
	if (pWorker->Location() != bar)
	{    
		pWorker->ChangeLocation(bar);

		cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Going to the bar for some drinks";
	}
}

void QuenchThirst::Execute(Worker* pWorker)
{
	int rnd = RandInt(0, 2);

	// loneniness might increase while drinking
	if (rnd == 0)
		pWorker->IncreaseLoneniness();

	// costs money
	pWorker->BuyAndDrink();

	cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Drinking...";

	pWorker->GetFSM()->ChangeState(pWorker->CurrentWork());  
}


void QuenchThirst::Exit(Worker* pWorker)
{ 
	cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Leaving the bar";

	CheckIfSocialAndContactFriends(pWorker);
}


bool QuenchThirst::OnMessage(Worker* pWorker, const Telegram& msg)
{
	// can't meet while in this state
	switch(msg.Msg)
	{
	case Msg_LetsHangOut:
		{
			return LetsHangOut(pWorker, msg);
		}
	case Msg_MeetingTime:
		{
			pWorker->SetBooked(false);
		}

	}//end switch

	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------HangOutWithFriends

HangOutWithFriends* HangOutWithFriends::Instance()
{
	static HangOutWithFriends instance;

	return &instance;
}

void HangOutWithFriends::Enter(Worker* pWorker)
{
	cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Heading to meet friends";

	// register the worker at the meeting location
	LocationMgr->EnterLocation(pWorker->Location(), pWorker->ID());
}

void HangOutWithFriends::Execute(Worker* pWorker)
{
	if (pWorker->Lonely()) // not really needed
	{
		// if other people at the meeting location, decrease loneliness
		// else wait for people and decrease patience
		if (LocationMgr->GetLocationPopulation(pWorker->Location()) > 1)
		{
			pWorker->DecreaseLoneniness();

			cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Hanging out with friends";
		}
		else
		{
			pWorker->DecreasePatience();

			// wait for friends or leave if not patient
			if (pWorker->Patient())
				cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Waiting for friends...";
			else
			{
				cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Tired of waiting, returning";

				pWorker->GetFSM()->RevertToPreviousState();
			}
		}
	}
	else
		pWorker->GetFSM()->RevertToPreviousState();  
}

void HangOutWithFriends::Exit(Worker* pWorker)
{ 
	cout << "\n" << GetNameOfEntity(pWorker->ID()) << ": " << "Returning to what I was doing";

	pWorker->SetBooked(false);

	// remove the worker from the meeting location
	LocationMgr->ExitLocation(pWorker->Location(), pWorker->ID());
}

bool HangOutWithFriends::OnMessage(Worker* pWorker, const Telegram& msg)
{	
	//send msg to global message handler
	return false;
}

double delay = 0.0;

bool WannaHangOut(Worker* pWorker, const Telegram& msg)
{	
	if (!pWorker->IsBooked() && pWorker->Lonely())
	{
		SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

		cout << "\nMessage handled by " << GetNameOfEntity(pWorker->ID()) << " at time: " 
			<< Clock->GetCurrentTime();

		MeetingMessage timeLoc = *(MeetingMessage*) (msg.ExtraInfo);

		pWorker->SetBooked(true);
		
		// extra delay needed or the program won't register if many simultaneous messages
		if (delay > 3)
			delay = 0;

		delay += 1;

		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pWorker->ID(),        //ID of sender
			msg.Sender,            //ID of recipient
			Msg_LetsHangOut,   //the message
			&timeLoc);

		Dispatch->DispatchMessage(timeLoc.Time + delay - Clock->GetCurrentTime(), //time delay
			pWorker->ID(),           //sender ID
			pWorker->ID(),           //receiver ID
			Msg_MeetingTime,        //msg
			&timeLoc);

		SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

		return true;
	}

	return false;
}

bool LetsHangOut(Worker* pWorker, const Telegram& msg)
{
	if (!pWorker->IsBooked())
	{
		SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

		cout << "\nMessage handled by " << GetNameOfEntity(pWorker->ID()) << " at time: " 
			<< Clock->GetCurrentTime();

		pWorker->SetBooked(true);

		MeetingMessage timeLoc = *(MeetingMessage*) (msg.ExtraInfo);

		Dispatch->DispatchMessage(timeLoc.Time - Clock->GetCurrentTime(), //time delay
			pWorker->ID(),           //sender ID
			pWorker->ID(),           //receiver ID
			Msg_MeetingTime,        //msg
			&timeLoc);

		SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

		return true;
	}

	return false;
}

void MeetingTime(Worker* pWorker, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	cout << "\nMessage handled by " << GetNameOfEntity(pWorker->ID()) << " at time: " 
		<< Clock->GetCurrentTime();

	SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	MeetingMessage timeLoc = *(MeetingMessage*) (msg.ExtraInfo);

	// time to leave and hang out with friends
	pWorker->ChangeLocation(timeLoc.Location);

	pWorker->GetFSM()->ChangeState(HangOutWithFriends::Instance());	
}

void CheckIfSocialAndContactFriends(Worker* pWorker)
{
	if (pWorker->Lonely() && !pWorker->IsBooked())
	{
		int rnd = RandInt(2, 8);
		double time = Clock->GetCurrentTime() + rnd;

		// include a random time and location (store) with the message
		MeetingMessage timeLoc(time, location_type(store));

		std::list<int> workers = EntityMgr->GetWorkers();
		std::list<int>::const_iterator it;

		for (it = workers.begin();it != workers.end();++it)
			if (*it != pWorker->ID())
				Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
				pWorker->ID(),        //ID of sender
				entity_names(*it),            //ID of recipient
				Msg_WannaHangOut,   //the message
				&timeLoc);
	}
}