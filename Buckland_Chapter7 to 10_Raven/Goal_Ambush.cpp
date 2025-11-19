#include "Goal_Ambush.h"
#include "../Buckland_Chapter7 to 10_Raven/Raven_Bot.h"
#include "../Buckland_Chapter7 to 10_Raven/Raven_Game.h"
#include "../Buckland_Chapter7 to 10_Raven/Raven_Map.h"
#include "../Buckland_Chapter7 to 10_Raven/goals/Goal_MoveToPosition.h"
#include "../Buckland_Chapter7 to 10_Raven/Raven_TargetingSystem.h"
#include "misc/utils.h" // RandInRange 사용을 위해

//------------------------------ Activate -------------------------------------
// 1. 매복 지점 선택
// 2. 이동 명령 생성
// 3. 대기 시간 설정
//-----------------------------------------------------------------------------
void Goal_Ambush::Activate()
{
    m_iStatus = active;

    // 기존 하위 목표 제거
    RemoveAllSubgoals();

    // 1. 매복 지점 선택: 지도에서 무작위 위치 선택 (NavGraph의 노드 중 하나)
    m_vAmbushSpot = m_pOwner->GetWorld()->GetMap()->GetRandomNodeLocation();
    m_bAtSpot = false;

    // 2. 이동 명령: 해당 위치로 이동 (Goal_MoveToPosition 사용)
    AddSubgoal(new Goal_MoveToPosition(m_pOwner, m_vAmbushSpot));

    // 3. 대기 시간 설정: 5~10초 사이의 무작위 값
    m_dTimeToWait = RandInRange(5.0, 10.0);
}

//------------------------------ Process --------------------------------------
// 목표 진행 로직
//-----------------------------------------------------------------------------
int Goal_Ambush::Process()
{
    // 비활성 상태라면 활성화
    ActivateIfInactive();

    // 하위 목표(이동) 처리
    int SubgoalStatus = ProcessSubgoals();

    // 하위 목표가 실패했다면 매복도 실패
    if (SubgoalStatus == failed)
    {
        m_iStatus = failed;
        return m_iStatus;
    }

    // 이동이 완료되었거나 하위 목표가 없는 경우 (매복 지점 도착)
    if (!m_bAtSpot && SubgoalStatus == completed)
    {
        m_bAtSpot = true;
    }

    // 매복 지점에 도착한 상태라면 대기 및 감시 로직 수행
    if (m_bAtSpot)
    {
        // 성공 조건: 대기 중 적을 발견하면 성공으로 간주
        if (m_pOwner->GetTargetSys()->isTargetPresent())
        {
            // 적을 발견했으므로 매복 목표 완료. 
            // Goal_Think가 이를 감지하고 Goal_AttackTarget을 선택하게 유도됨.
            m_iStatus = completed;
        }
        else
        {
            // 적이 없다면 대기 시간 차감 (Update Tick을 고려해야 함. 여기서는 대략적인 프레임 타임 가정)
            // Raven 엔진은 보통 60fps 혹은 가변 시간이지만, 편의상 Update 간격을 사용하거나 시계 함수를 써야 함.
            // 여기서는 단순히 호출될 때마다 작은 값을 빼는 방식보다는 
            // 실제 구현 시 Clock을 사용하는 것이 좋으나, 간단히 처리합니다.
            const double updateStep = 1.0 / 60.0; // 60Hz 가정
            m_dTimeToWait -= updateStep;

            // 시간 초과: 적이 나타나지 않고 대기 시간이 끝남
            if (m_dTimeToWait <= 0)
            {
                // 매복 실패가 아닌 '완료'로 처리하여 봇이 자연스럽게 다른 행동(Explore 등)을 선택하도록 함
                m_iStatus = completed;
            }
        }
    }

    return m_iStatus;
}

//------------------------------ Terminate ------------------------------------
// 목표 종료 및 정리
//-----------------------------------------------------------------------------
void Goal_Ambush::Terminate()
{
    RemoveAllSubgoals();
    m_bAtSpot = false;
}