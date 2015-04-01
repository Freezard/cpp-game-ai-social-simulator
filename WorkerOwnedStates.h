#ifndef WORKER_OWNED_STATES_H
#define WORKER_OWNED_STATES_H

#include "State.h"

class Worker;
struct Telegram;

//------------------------------------------------------------------------
//
//  In this state the worker will walk to a garden and water the flowers.
//  If he gets thirsty he'll change state to QuenchThirst and if his
//  working hours are done he will change state to VisitStoreAndBuyFood.
//------------------------------------------------------------------------
class EnterGardenAndWaterFlowers : public State<Worker>
{
private:
  
  EnterGardenAndWaterFlowers(){}

  //copy ctor and assignment should be private
  EnterGardenAndWaterFlowers(const EnterGardenAndWaterFlowers&);
  EnterGardenAndWaterFlowers& operator=(const EnterGardenAndWaterFlowers&);
 
public:

  //this is a singleton
  static EnterGardenAndWaterFlowers* Instance();

  virtual void Enter(Worker* worker);

  virtual void Execute(Worker* worker);

  virtual void Exit(Worker* worker);

  virtual bool OnMessage(Worker* agent, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//  In this state the worker will walk to the forest and collect wood.
//  If he gets thirsty he'll change state to QuenchThirst and if his
//  working hours are done he will change state to VisitStoreAndBuyFood.
//------------------------------------------------------------------------
class EnterForestAndCutTrees : public State<Worker>
{
private:
  
  EnterForestAndCutTrees(){}

  //copy ctor and assignment should be private
  EnterForestAndCutTrees(const EnterForestAndCutTrees&);
  EnterForestAndCutTrees& operator=(const EnterForestAndCutTrees&);
 
public:

  //this is a singleton
  static EnterForestAndCutTrees* Instance();

  virtual void Enter(Worker* worker);

  virtual void Execute(Worker* worker);

  virtual void Exit(Worker* worker);

  virtual bool OnMessage(Worker* agent, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//  In this state the worker will visit the store and buy some food.
//  When leaving the store he will change state to GoHomeAndEat, and
//  might call friends to meet if social.
//------------------------------------------------------------------------
class VisitStoreAndBuyFood : public State<Worker>
{
private:
  
  VisitStoreAndBuyFood(){}

  //copy ctor and assignment should be private
  VisitStoreAndBuyFood(const VisitStoreAndBuyFood&);
  VisitStoreAndBuyFood& operator=(const VisitStoreAndBuyFood&);
 
public:

  //this is a singleton
  static VisitStoreAndBuyFood* Instance();

  virtual void Enter(Worker* worker);

  virtual void Execute(Worker* worker);

  virtual void Exit(Worker* worker);

  virtual bool OnMessage(Worker* agent, const Telegram& msg);
};

//------------------------------------------------------------------------
//
//  Worker will go home and cook/eat his food, then change his state to
//  GoHomeAndSleep.
//------------------------------------------------------------------------
class GoHomeAndEat : public State<Worker>
{
private:
  
  GoHomeAndEat(){}

  //copy ctor and assignment should be private
  GoHomeAndEat(const GoHomeAndEat&);
  GoHomeAndEat& operator=(const GoHomeAndEat&);
 
public:

  //this is a singleton
  static GoHomeAndEat* Instance();

  virtual void Enter(Worker* worker);

  virtual void Execute(Worker* worker);

  virtual void Exit(Worker* worker);

  virtual bool OnMessage(Worker* agent, const Telegram& msg);
};

//------------------------------------------------------------------------
//
//  Worker will go home and sleep until his fatigue is decreased
//  sufficiently, then return to work.
//------------------------------------------------------------------------
class GoHomeAndSleep : public State<Worker>
{
private:
  
  GoHomeAndSleep(){}

  //copy ctor and assignment should be private
  GoHomeAndSleep(const GoHomeAndSleep&);
  GoHomeAndSleep& operator=(const GoHomeAndSleep&);
 
public:

  //this is a singleton
  static GoHomeAndSleep* Instance();

  virtual void Enter(Worker* worker);

  virtual void Execute(Worker* worker);

  virtual void Exit(Worker* worker);

  virtual bool OnMessage(Worker* agent, const Telegram& msg);
};


//------------------------------------------------------------------------
//
//  Worker changes location to the bar and keeps buying drinks until
//  his thirst is quenched. When satisfied he returns to his job, and
//  might call friends to meet if social.
//------------------------------------------------------------------------
class QuenchThirst : public State<Worker>
{
private:
  
  QuenchThirst(){}

  //copy ctor and assignment should be private
  QuenchThirst(const QuenchThirst&);
  QuenchThirst& operator=(const QuenchThirst&);
 
public:

  //this is a singleton
  static QuenchThirst* Instance();

  virtual void Enter(Worker* worker);

  virtual void Execute(Worker* worker);

  virtual void Exit(Worker* worker);

  virtual bool OnMessage(Worker* agent, const Telegram& msg);
};

//------------------------------------------------------------------------
//
//  Worker will hang out with friends until he doesn't feel lonely anymore,
//  if others have arrived to the meeting place. If not his patience will
//  decrease until he leaves and returns to his previous state.
//------------------------------------------------------------------------
class HangOutWithFriends : public State<Worker>
{
private:
  
  HangOutWithFriends(){}

  //copy ctor and assignment should be private
  HangOutWithFriends(const HangOutWithFriends&);
  HangOutWithFriends& operator=(const HangOutWithFriends&);
 
public:

  //this is a singleton
  static HangOutWithFriends* Instance();

  virtual void Enter(Worker* worker);

  virtual void Execute(Worker* worker);

  virtual void Exit(Worker* worker);

  virtual bool OnMessage(Worker* agent, const Telegram& msg);
};

//------------------------------------------------------------------------
// When receiving this message the worker will check if he wants to
// meet, then reply with LetsHangOut and send MeetingTime to himself
// as a reminder. If he doesn't feel like meeting he won't reply.
//------------------------------------------------------------------------
bool WannaHangOut(Worker* pWorker, const Telegram& msg);

//------------------------------------------------------------------------
// When receiving this message the worker will send MeetingTime to himself
// as a reminder to meet, unless already booked.
//------------------------------------------------------------------------
bool LetsHangOut(Worker* pWorker, const Telegram& msg);

//------------------------------------------------------------------------
// When receiving this message the worker will change his state to
// HangOutWithFriends and enter the location of the meeting.
//------------------------------------------------------------------------
void MeetingTime(Worker* pWorker, const Telegram& msg);

//------------------------------------------------------------------------
// The worker will check if social and then send messages to every other
// worker, asking if they want to meet.
//------------------------------------------------------------------------
void CheckIfSocialAndContactFriends(Worker* pWorker);


#endif