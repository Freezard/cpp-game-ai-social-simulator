#ifndef FOOD_H
#define FOOD_H

#include <string>

enum food_type
{
  none,
  spaghetti,
  pork,  
  soup
};

inline std::string GetFoodAsString(int food)
{
  switch(food)
  {
  case none: return "none";
  case spaghetti: return "spaghetti";
  case pork: return "pork";
  case soup: return "soup";
  default: return "Bad food_type";
  }
}



//uncomment this to send the output to a text file
//#define TEXTOUTPUT




#endif