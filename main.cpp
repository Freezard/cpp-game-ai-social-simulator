#include <fstream>
#include <time.h>

#include "Locations.h"
#include "Worker.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "ConsoleUtils.h"
#include "EntityNames.h"


std::ofstream os;

int main()
{
//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

  //seed random number generator
  srand((unsigned) time(NULL));

  //create workers
  Worker* A = new Worker(ent_Worker_A);
  Worker* B = new Worker(ent_Worker_B);
  Worker* C = new Worker(ent_Worker_C);
  Worker* D = new Worker(ent_Worker_D);

  //register them with the entity manager
  EntityMgr->RegisterEntity(A);
  EntityMgr->RegisterEntity(B);
  EntityMgr->RegisterEntity(C); 
  EntityMgr->RegisterEntity(D);
  
  //register them as workers too
  EntityMgr->RegisterWorker(A);
  EntityMgr->RegisterWorker(B);
  EntityMgr->RegisterWorker(C);
  EntityMgr->RegisterWorker(D);

  //simulation loop
  while (true)
  { 
    A->Update();
	B->Update();
	C->Update();
	D->Update();

    //dispatch any delayed messages
    Dispatch->DispatchDelayedMessages();

    Sleep(800);
  }

  //tidy up
  delete A;
  delete B;
  delete C;
  delete D;

  //wait for a keypress before exiting
  PressAnyKeyToContinue();


  return 0;
}






