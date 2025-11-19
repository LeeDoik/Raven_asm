#include "AmbushGoal_Evaluator.h"
#include "Goal_Ambush.h"
#include "../Buckland_Chapter7 to 10_Raven/goals/Raven_Feature.h"
#include "../Buckland_Chapter7 to 10_Raven/Raven_Bot.h"
#include "misc/utils.h"
#include "../Buckland_Chapter7 to 10_Raven/goals/Goal_Think.h"

#include "misc/Cgdi.h"                 // gdi->TextAtPos 사용
#include "misc/Stream_Utility_Functions.h" // ttos 사용 (숫자를 문자열로 변환)

//---------------------- CalculateDesirability --------------------------------
// 매복의 가치를 계산합니다.
// 규칙:
// 1. 무기가 충분한가? (TotalWeaponStrength)
// 2. 체력이 적당한가? (Health)
// 3. 현재 공격 대상이 없는가? (적이 이미 보이면 매복할 필요 없이 싸워야 함)
//-----------------------------------------------------------------------------
double AmbushGoal_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
    double Desirability = 0.0;

    // Feature 사용: 현재 무기 상태와 체력을 가져옴 (0.0 ~ 1.0)
    double HealthScore = Raven_Feature::Health(pBot);
    double WeaponScore = Raven_Feature::TotalWeaponStrength(pBot);

    // 이미 타겟이 있다면 매복보다는 공격이 우선이므로 점수를 낮게 줌
    if (pBot->GetTargetSys()->isTargetPresent())
    {
        return 0.0;
    }

    // 퍼지 룰(Fuzzy Rule) 대체 논리:
    // 건강하고 무기가 충분할 때 매복하기 가장 좋음.
    // 너무 체력이 없으면(Health Giver 찾기), 무기가 없으면(Weapon 찾기)가 우선됨.
    // 따라서 두 점수의 곱을 기본 가치로 사용.
    Desirability = (HealthScore * WeaponScore) * m_dCharacterBias;

    // 값이 0~1 사이가 되도록 보장 (Bias에 따라 1을 넘을 수도 있음)
    Clamp(Desirability, 0.0, 1.0);

    return Desirability;
}

//----------------------------- SetGoal ---------------------------------------
// Goal_Think에 Goal_Ambush를 추가합니다.
//-----------------------------------------------------------------------------
void AmbushGoal_Evaluator::SetGoal(Raven_Bot* pBot)
{
    pBot->GetBrain()->AddGoal_Ambush();
}

//-------------------------- RenderInfo ---------------------------------------
// [수정] 디버깅용 정보를 화면에 표시합니다. (순수 가상 함수 구현)
//-----------------------------------------------------------------------------
void AmbushGoal_Evaluator::RenderInfo(Vector2D Position, Raven_Bot* pBot)
{
    // 현재 위치에 "AMB: 점수" 형태로 텍스트를 출력합니다.
    // ttos는 숫자를 문자열로 변환하는 유틸리티 함수입니다.
    gdi->TextAtPos(Position, "AMB: " + ttos(CalculateDesirability(pBot), 2));
}