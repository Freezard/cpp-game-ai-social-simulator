#ifndef NAMES_H
#define NAMES_H

#include <string>

enum entity_names
{
  ent_Worker_A,
  ent_Worker_B,
  ent_Worker_C,
  ent_Worker_D
};

inline std::string GetNameOfEntity(int n)
{
  switch(n)
  {
  case ent_Worker_A:

    return "Worker A";

  case ent_Worker_B:

    return "Worker B";

  case ent_Worker_C:

    return "Worker C";

  case ent_Worker_D:

    return "Worker D";

  default:

    return "UNKNOWN!";
  }
}

#endif