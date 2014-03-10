#include "BehaviorBase.h"

// 控制阻挡动作的类

// 阻挡动作的执行器
class BehaviorBlockExecuter: public BehaviorExecuterBase<BehaviorDefenseData> {
public:
    BehaviorBlockExecuter(Agent & agent);
    virtual ~BehaviorBlockExecuter(void);
    // 执行阻挡动作（实现方法是冲到某一点）
    // act_bhv:	存储动作细节——目标点、目标点的半径的冲刺的power
    // 			半径意味着不必完全冲到目标点，可以冲到附近
    // return:	若执行成功则为true，否则为false
	bool Execute(const ActiveBehavior & act_bhv);
	static const BehaviorType BEHAVIOR_TYPE;
};

// 规划阻挡动作
class BehaviorBlockPlanner: public BehaviorPlannerBase<BehaviorDefenseData> {
public:
	BehaviorBlockPlanner(Agent & agent);
	virtual ~BehaviorBlockPlanner();
	void Plan(list<ActiveBehavior> & behavior_list);
};

// 阻挡动作的Planner
void BehaviorBlockPlanner::Plan(list<ActiveBehavior> & behavior_list) {
	// 离自己最近的队友
	Unum closest_tm = mPositionInfo.GetClosestTeammateToBall();
	// 如果当前在发角球、边线球和任意球等则
	if (mWorldState.GetPlayMode() >= PM_Opp_Corner_Kick && mWorldState.GetPlayMode() <= PM_Opp_Offside_Kick)
		// 不用阻挡
		return;
	// 若离自己最近的队友是自己（周围没别人）
	if (closest_tm == mSelfState.GetUnum() ||
		// 或者自己的最小截球周期小于队友的最优截球周期
		mStrategy.GetMyInterCycle() <= mStrategy.GetSureTmInterCycle()) {
		// 那么就新建一个截球行为，并设置好参数：
		ActiveBehavior block(mAgent, BT_Block);
		// 目标点的范围
		block.mBuffer = 0.5;
		// 当前体力下最优的冲刺power
		block.mPower = mSelfState.CorrectDashPowerForStamina(ServerParam::instance().maxDashPower());
		// 冲向的目标点
		block.mTarget = mAnalyser.mLightHouse;
		// 该动作的优劣度
		block.mEvaluation = 1.0 + FLOAT_EPS;
		// 然后将该动作加入动作队列等待评估
		behavior_list.push_back(block);
	}
}
