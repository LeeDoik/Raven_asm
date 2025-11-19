#include "Goal_Ambush.h"
#include "../Buckland_Chapter7 to 10_Raven/Raven_Bot.h"
#include "../Buckland_Chapter7 to 10_Raven/Raven_Game.h"
#include "../Buckland_Chapter7 to 10_Raven/Raven_Map.h"
#include "../Buckland_Chapter7 to 10_Raven/goals/Goal_MoveToPosition.h"
#include "../Buckland_Chapter7 to 10_Raven/Raven_TargetingSystem.h"
#include "../Buckland_Chapter7 to 10_Raven/Raven_SensoryMemory.h"


//------------------------------ Activate -------------------------------------
//-----------------------------------------------------------------------------
void Goal_Ambush::Activate()
{
    m_iStatus = active;

    RemoveAllSubgoals();

    // 1. 매복 지점 선택
    Raven_Game* pWorld = m_pOwner->GetWorld();
    Raven_Map* pMap = pWorld->GetMap();

    m_vAmbushSpot = pMap->GetRandomNodeLocation();

    // 기억된 적들의 시야를 피할 수 있는 곳을 찾습니다.
    std::list<Raven_Bot*> sensedBots =
        m_pOwner->GetSensoryMem()->GetListOfRecentlySensedOpponents();

    if (!sensedBots.empty())
    {
        const int TryCount = 20;
        for (int i = 0; i < TryCount; ++i)
        {
            Vector2D candidatePos = pMap->GetRandomNodeLocation();
            bool bHidden = true;

            std::list<Raven_Bot*>::iterator curBot;
            for (curBot = sensedBots.begin(); curBot != sensedBots.end(); ++curBot)
            {
                Vector2D enemyPos = m_pOwner->GetSensoryMem()->GetLastRecordedPositionOfOpponent(*curBot);

                if (pWorld->isLOSOkay(candidatePos, enemyPos))
                {
                    bHidden = false;
                    break;
                }
            }

            if (bHidden)
            {
                m_vAmbushSpot = candidatePos;
                break;
            }
        }
    }

    m_bAtSpot = false;

    // 이동 명령 추가
    AddSubgoal(new Goal_MoveToPosition(m_pOwner, m_vAmbushSpot));
}

//------------------------------ Process --------------------------------------
//-----------------------------------------------------------------------------
int Goal_Ambush::Process()
{
    ActivateIfInactive();

    // 하위 목표(이동) 처리
    int SubgoalStatus = ProcessSubgoals();

    // 이동 실패 시 매복 실패
    if (SubgoalStatus == failed)
    {
        m_iStatus = failed;
        return m_iStatus;
    }

    // 이동 완료 체크 (매복 지점 도착)
    if (!m_bAtSpot && SubgoalStatus == completed)
    {
        m_bAtSpot = true;

        // [요청사항 반영] 도착하면 하위 목표를 모두 제거하여 확실히 정지시킵니다.
        // 더 이상 이동하지 않고 제자리에서 대기합니다.
        RemoveAllSubgoals();
    }

    // 매복 지점 도착 후 로직
    if (m_bAtSpot)
    {
        // 적을 발견하면 성공 처리 (Goal_Think가 공격 목표로 전환하도록 유도)
        if (m_pOwner->GetTargetSys()->isTargetPresent())
        {
            m_iStatus = completed;
        }
        else
        {
            // [요청사항 반영] 적을 만날 때까지 무한 대기
            // 타이머 로직을 제거하고 항상 active를 반환하여 Goal_Think가 다른 목표로 바꾸지 않게 함
            m_iStatus = active;
        }
    }

    return m_iStatus;
}

//------------------------------ Terminate ------------------------------------
//-----------------------------------------------------------------------------
void Goal_Ambush::Terminate()
{
    RemoveAllSubgoals();
    m_bAtSpot = false;
}