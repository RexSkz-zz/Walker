#include "BehaviorBase.h"

// 攻击类的控制中心，控制着各种攻击的行为

class BehaviorAttackPlanner: public BehaviorPlannerBase<BehaviorAttackData> {
public:
	BehaviorAttackPlanner(Agent & agent);
	virtual ~BehaviorAttackPlanner();
	// 攻击的决策
	void Plan(list<ActiveBehavior> & behavior_list);
};

// 控制行为的生成
void BehaviorAttackPlanner::Plan(list<ActiveBehavior> & behavior_list) {
	// 若球可以被抓住（前提是你是守门员）
	if (mSelfState.IsBallCatchable() &&
		// 并且你是上一次控球的人（刚把球踢出去）（不管是队友还是对方）
		mStrategy.IsLastOppControl() &&
		// 并且，你把上一个周期Pass掉或者你上一个周期在运球
		!(mAgent.IsLastActiveBehaviorInActOf(BT_Pass) || mAgent.IsLastActiveBehaviorInActOf(BT_Dribble))
		// 那么就不要尝试进攻了
		) return;
	// 下面尝试各种Plan，并把结果添加到序列中
	// 顺序分别是：拦截、射击、通过(?)、带球、跑位、控球
	// 对足球不了解，求大神细讲
	BehaviorInterceptPlanner(mAgent).Plan(mActiveBehaviorList);
	BehaviorShootPlanner(mAgent).Plan(mActiveBehaviorList);
	BehaviorPassPlanner(mAgent).Plan(mActiveBehaviorList);
	BehaviorDribblePlanner(mAgent).Plan(mActiveBehaviorList);
	BehaviorPositionPlanner(mAgent).Plan(mActiveBehaviorList);
	BehaviorHoldPlanner(mAgent).Plan(mActiveBehaviorList);
	// 如果命令序列是空的就不用管了
	if (!mActiveBehaviorList.empty()) {
		// 按照优劣对命令排序，取最优的那个命令
		mActiveBehaviorList.sort(greater<ActiveBehavior>());
		behavior_list.push_back(mActiveBehaviorList.front());
		// 允许非最优行为提交视觉请求，但是如果越不优，视觉请求刷新的频率就越低
		if (mActiveBehaviorList.size() > 1) {
			// 视觉刷新的周期
			double plus = 1.0;
			ActiveBehaviorPtr it = mActiveBehaviorList.begin();
			for (++it; it != mActiveBehaviorList.end(); ++it) {
				it->SubmitVisualRequest(plus);
				plus *= 2.0;
			}
		}
	}
}
