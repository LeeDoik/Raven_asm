#ifndef GOAL_AMBUSH_H
#define GOAL_AMBUSH_H
#pragma warning (disable:4786)

#include "Goals/Goal_Composite.h"
#include "../Buckland_Chapter7 to 10_Raven/goals/Raven_Goal_Types.h"
#include "../Buckland_Chapter7 to 10_Raven/Raven_Bot.h"

class Goal_Ambush : public Goal_Composite<Raven_Bot>
{
private:

    // 매복할 위치
    Vector2D  m_vAmbushSpot;

    // 매복 지점에서 대기할 시간 (초)
    double    m_dTimeToWait;

    // 매복 지점에 도착했는지 여부
    bool      m_bAtSpot;

public:

    Goal_Ambush(Raven_Bot* pOwner) : Goal_Composite<Raven_Bot>(pOwner, goal_ambush),
        m_dTimeToWait(0.0),
        m_bAtSpot(false)
    {
    }

    void Activate();
    int  Process();
    void Terminate();

    // 메시지 처리 (필요 시 구현, 여기서는 기본적으로 false 반환 혹은 상위 처리)
    bool HandleMessage(const Telegram& msg) { return ForwardMessageToFrontMostSubgoal(msg); }
};

#endif