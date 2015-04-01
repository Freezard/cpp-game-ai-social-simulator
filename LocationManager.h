#ifndef LOCATIONMANAGER_H
#define LOCATIONMANAGER_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   LocationManager.h
//
//  Desc:   Singleton class to handle entities at locations.          
//
//------------------------------------------------------------------------
#include <map>
#include <cassert>
#include <string>
#include "Locations.h"
#include <list>

//provide easy access
#define LocationMgr LocationManager::Instance()



class LocationManager
{
private:

	typedef std::map<location_type, std::list<int>> LocationMap;

private:

	//to facilitate quick lookup the locations are stored in a std::map, with
	//each location referencing a list of entity ids
	LocationMap m_LocationMap;

	LocationManager(){}

	//copy ctor and assignment should be private
	LocationManager(const LocationManager&);
	LocationManager& operator=(const LocationManager&);

public:

	static LocationManager* Instance();

	//adds this entity id to provided location
	void            EnterLocation(location_type loc, int id);

	//returns the number of entities in the provided location
	int GetLocationPopulation(location_type loc)const;

	//removes an entity id from the location
	void            ExitLocation(location_type loc, int id);
};







#endif