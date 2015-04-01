#include "LocationManager.h"

//--------------------------- Instance ----------------------------------------
//
//   this class is a singleton
//-----------------------------------------------------------------------------
LocationManager* LocationManager::Instance()
{
	static LocationManager instance;

	return &instance;
}

//------------------------- GetLocationPopulation -----------------------------------
//-----------------------------------------------------------------------------
int LocationManager::GetLocationPopulation(location_type loc)const
{
	//find the entity
	LocationMap::const_iterator iter = m_LocationMap.find(loc);

	if (iter != m_LocationMap.end())
		return iter->second.size();
	else
		return 0;
}

//--------------------------- ExitLocation ------------------------------------
//-----------------------------------------------------------------------------
void LocationManager::ExitLocation(location_type loc, int id)
{ 
	LocationMap::iterator iter = m_LocationMap.find(loc);

	if (iter != m_LocationMap.end())
		iter->second.remove(id);
	else
		m_LocationMap.erase(iter);
} 

//---------------------------- EnterLocation ---------------------------------
//-----------------------------------------------------------------------------
void LocationManager::EnterLocation(location_type loc, int id)
{
	LocationMap::iterator iter = m_LocationMap.find(loc);

	if (iter != m_LocationMap.end())
		iter->second.push_back(id);
	else
		m_LocationMap.insert(std::make_pair(loc, std::list<int>(id)));
}
