#include "Worker.h"

bool Worker::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}

void Worker::Update()
{
  SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);

  m_iThirst += 1;

  m_pStateMachine->Update();
}

bool Worker::TimeToGoHome()
{
	return (Clock->GetCurrentTime() >= m_dTimeToGoHome);
}

void Worker::AddToWealth(const int val)
{
  m_iMoneyInstore += val;

  if (m_iMoneyInstore < 0) m_iMoneyInstore = 0;
}

bool Worker::Thirsty()const
{
  if (m_iThirst > ThirstLevel){return true;}

  return false;
}

bool Worker::Fatigued()const
{
  if (m_iFatigue > TirednessThreshold)
  {
    return true;
  }

  return false;
}

bool Worker::Patient()
{
	if (m_iPatience > 0)
		return true;
	else
	{
		m_iPatience = PatienceLevel;
		return false;
	}
}
