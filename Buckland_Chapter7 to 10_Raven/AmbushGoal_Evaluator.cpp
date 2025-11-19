#include "AmbushGoal_Evaluator.h"
#include "Goal_Ambush.h"
#include "../Buckland_Chapter7 to 10_Raven/goals/Raven_Feature.h"
#include "../Buckland_Chapter7 to 10_Raven/Raven_Bot.h"
#include "misc/utils.h"
#include "../Buckland_Chapter7 to 10_Raven/goals/Goal_Think.h"

#include "misc/Cgdi.h"                 // gdi->TextAtPos 사용
#include "misc/Stream_Utility_Functions.h" // ttos 사용 (숫자를 문자열로 변환)

//---------------------- CalculateDesirability --------------------------------
//-----------------------------------------------------------------------------
double AmbushGoal_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
    double Desirability = Raven_Feature::AmbushPotential(pBot);

    // 성향(Bias) 반영
    Desirability *= m_dCharacterBias;

    // 값이 0~1 사이가 되도록 보장
    Clamp(Desirability, 0.0, 1.0);

    return Desirability;
}

//----------------------------- SetGoal ---------------------------------------
void AmbushGoal_Evaluator::SetGoal(Raven_Bot* pBot)
{
    pBot->GetBrain()->AddGoal_Ambush();
}

//-------------------------- RenderInfo ---------------------------------------
void AmbushGoal_Evaluator::RenderInfo(Vector2D Position, Raven_Bot* pBot)
{
    // 화면에 현재 매복 점수를 표시 (디버깅용)
    gdi->TextAtPos(Position, "Ambush: " + ttos(CalculateDesirability(pBot), 2));
}