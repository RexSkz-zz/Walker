#include "BehaviorBase.h"
#include "Analyser.h"

// 防守类的控制中心，控制着各种攻击的行为

class BehaviorDefensePlanner: public BehaviorPlannerBase <BehaviorDefenseData> {
public:
	BehaviorDefensePlanner(Agent & agent);
	virtual ~BehaviorDefensePlanner();
	void Plan(list<ActiveBehavior> & behavior_list);
};

// 防守行为的生成
void BehaviorDefensePlanner::Plan(std::list<ActiveBehavior> & behavior_list) {
	// 下面尝试各种Plan，并把结果添加到序列中
	// 顺序分别是：阵型、阻挡、标记特殊球员（估计是盯住）
	// 对足球不了解，求大神细讲
	BehaviorFormationPlanner(mAgent).Plan(behavior_list);
	BehaviorBlockPlanner(mAgent).Plan(behavior_list);
	BehaviorMarkPlanner(mAgent).Plan(behavior_list);
	// 若生成的可用动作列表不为空
	if (!mActiveBehaviorList.empty()) {
		// 对产生的动作列表按照优劣度进行排序
		mActiveBehaviorList.sort(std::greater<ActiveBehavior>());
		// 并存入执行动作列表中
		behavior_list.push_back(mActiveBehaviorList.front());
		// 允许非最优行为提交视觉请求，越不优则视觉更新周期越大
		if (mActiveBehaviorList.size() > 1) {
			double plus = 1.0;
			ActiveBehaviorPtr it = mActiveBehaviorList.begin();
			for (++it; it != mActiveBehaviorList.end(); ++it) {
				it->SubmitVisualRequest(plus);
				plus *= 2.0;
			}
		}
	}
}
