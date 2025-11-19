#include "AmbushGoal_Evaluator.h"
#include "Goal_Ambush.h"
#include "../Buckland_Chapter7 to 10_Raven/goals/Raven_Feature.h"
#include "../Buckland_Chapter7 to 10_Raven/Raven_Bot.h"
#include "misc/utils.h"
#include "../Buckland_Chapter7 to 10_Raven/goals/Goal_Think.h"
#include "misc/Cgdi.h" 
#include "misc/Stream_Utility_Functions.h"

//---------------------- CalculateDesirability --------------------------------
//-----------------------------------------------------------------------------
double AmbushGoal_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
    double Desirability = Raven_Feature::AmbushPotential(pBot);

    Desirability *= m_dCharacterBias;

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
    gdi->TextAtPos(Position, "Ambush: " + ttos(CalculateDesirability(pBot), 2));
}