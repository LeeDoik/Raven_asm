#ifndef AMBUSH_GOAL_EVALUATOR_H
#define AMBUSH_GOAL_EVALUATOR_H

#include "../Buckland_Chapter7 to 10_Raven/goals/Goal_Evaluator.h"
#include "../Buckland_Chapter7 to 10_Raven/Raven_Bot.h"

class AmbushGoal_Evaluator : public Goal_Evaluator
{
public:

	AmbushGoal_Evaluator(double bias) :Goal_Evaluator(bias) {}

	double CalculateDesirability(Raven_Bot* pBot);

	void  SetGoal(Raven_Bot* pBot);

	void  RenderInfo(Vector2D Position, Raven_Bot* pBot);
};

#endif