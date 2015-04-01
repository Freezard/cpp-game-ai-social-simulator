#ifndef WORKER_H
#define WORKER_H

#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "Food.h"
#include "ConsoleUtils.h"
#include "WorkerOwnedStates.h"
#include "StateMachine.h"
#include "CrudeTimer.h"
#include "utils.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

//above this value a worker is thirsty
const int ThirstLevel        = 5;
//above this value a worker is sleepy
const int TirednessThreshold = 5;
//above this value a worker is lonely
const int LonelyLevel		 = 3;
//how patient a worker is waiting
const int PatienceLevel		 = 3;


class Worker : public BaseGameEntity
{
private:

  //an instance of the state machine class
  StateMachine<Worker>* m_pStateMachine;
  
  State<Worker>*		m_pCurrentWork;

  //current location of the worker
  location_type         m_Location;

  //what kind of food the worker has bought
  food_type				m_Food;

  //the time when the worker is done working
  double				m_dTimeToGoHome;

  int                   m_iMoneyInstore;

  //the higher the value, the thirstier the worker
  int                   m_iThirst;

  //the higher the value, the more tired the worker
  int                   m_iFatigue;

  //the higher the value, the more lonely the worker
  int					m_iLonely;

  //the lower the value, the less patient the worker
  int					m_iPatience;

  //is the worker booked for meeting with friends
  bool					m_Booked;

public:

  Worker(int id):m_Location(house),
						  m_dTimeToGoHome(0),
                          m_iMoneyInstore(0),
                          m_iThirst(0),
                          m_iFatigue(0),
						  m_iLonely(0),
						  m_iPatience(PatienceLevel),
						  m_Booked(false),
                          BaseGameEntity(id)
                               
  {
	// randomize the worker's job
	if (RandInt(0, 1) == 0)
		m_pCurrentWork = EnterGardenAndWaterFlowers::Instance();
	else
		m_pCurrentWork = EnterForestAndCutTrees::Instance();

    //set up state machine
    m_pStateMachine = new StateMachine<Worker>(this);
    
    m_pStateMachine->SetCurrentState(GoHomeAndSleep::Instance());

    /* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE worker */
  }

  ~Worker(){delete m_pStateMachine;}

  void Update();

  virtual bool  HandleMessage(const Telegram& msg);
  
  StateMachine<Worker>* GetFSM()const{return m_pStateMachine;}


  
  //-------------------------------------------------------------accessors
  location_type Location()const{return m_Location;}
  void          ChangeLocation(location_type loc){m_Location = loc;}

  State<Worker>* CurrentWork(){return m_pCurrentWork;}

  food_type		Food()const{return m_Food;}
  void          BoughtFood(food_type food){m_Food = food;}
    
  void			SetTimeToGoHome(double time){m_dTimeToGoHome = time;}
  bool			TimeToGoHome();
  double		GetTimeToGoHome(){return m_dTimeToGoHome;}

  bool          Fatigued()const;
  void          DecreaseFatigue(){m_iFatigue -= 1;}
  void          IncreaseFatigue(){m_iFatigue += 1;}

  int           Wealth()const{return m_iMoneyInstore;}
  void          SetWealth(int val){m_iMoneyInstore = val;}
  void          AddToWealth(int val);

  bool			Lonely()const{return m_iLonely > LonelyLevel;}
  void			IncreaseLoneniness(){m_iLonely++;}
  void			DecreaseLoneniness(){m_iLonely--;}

  bool          Thirsty()const; 
  void          BuyAndDrink(){m_iThirst = 0; m_iMoneyInstore-=2;}

  bool			Patient();
  void			DecreasePatience(){m_iPatience--;}

  void			SetBooked(bool booked){m_Booked = booked;}
  bool			IsBooked()const{return m_Booked;}
};



#endif
