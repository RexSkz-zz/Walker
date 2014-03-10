#include "BehaviorBase.h"
#include "VisualSystem.h"

// 带球动作的执行器
class BehaviorDribbleExecuter: public BehaviorExecuterBase<BehaviorAttackData> {
public:
    BehaviorDribbleExecuter(Agent & agent);
    virtual ~BehaviorDribbleExecuter(void);
    // 执行带球的动作
    bool Execute(const ActiveBehavior &);
    // 提交视觉请求（只有队友可以提交请求）
    // dribble: 动作细节（包括球员的编号）
    // plus:    请求的重要度（视觉更新的周期）
	void SubmitVisualRequest(const ActiveBehavior & dribble, double plus = 0.0);
    // 未找到引用，意义不明
    /*static const BehaviorType BEHAVIOR_TYPE;*/
};

// 带球动作的Planner
class BehaviorDribblePlanner: public BehaviorPlannerBase <BehaviorAttackData> {
public:
    BehaviorDribblePlanner(Agent & agent);
    virtual ~BehaviorDribblePlanner(void);
    void Plan(list<ActiveBehavior> & behavior_list);
};

void BehaviorDribblePlanner::Plan(std::list<ActiveBehavior> & behavior_list) {
    // 若球不可踢，或当前策略不推荐带球，或自己是守门员
    // 则无需考虑带球
    if (!mSelfState.IsKickable()) return;
    if (mStrategy.IsForbidenDribble()) return;
    if (mSelfState.IsGoalie()) return;
    // 
    for (AngleDeg dir = -90.0; dir < 90.0; dir += 2.5) {
        ActiveBehavior dribble(mAgent, BT_Dribble, BDT_Dribble_Normal);

        dribble.mAngle = dir;

        const std::vector<Unum> & opp2ball = mPositionInfo.GetCloseOpponentToBall();
        AngleDeg min_differ = HUGE_VALUE;

        for (uint j = 0; j < opp2ball.size(); ++j) {
            Vector rel_pos = mWorldState.GetOpponent(opp2ball[j]).GetPos() - mBallState.GetPos();
            if (rel_pos.Mod() > 15.0) continue;

            AngleDeg differ = GetAngleDegDiffer(dir, rel_pos.Dir());
            if (differ < min_differ) {
                min_differ = differ;
            }
        }

        if (min_differ < 10.0) continue;

        dribble.mTarget= mSelfState.GetPos() + Polar2Vector( mSelfState.GetEffectiveSpeedMax(), dir);

        dribble.mEvaluation = Evaluation::instance().EvaluatePosition(dribble.mTarget, true);

        mActiveBehaviorList.push_back(dribble);
    }
    double speed = mSelfState.GetEffectiveSpeedMax();

    for (AngleDeg dir = -90.0; dir < 90.0; dir += 2.5) {
        ActiveBehavior dribble(mAgent, BT_Dribble, BDT_Dribble_Fast);
        dribble.mKickSpeed = speed;
        dribble.mAngle = dir;

        const std::vector<Unum> & opp2ball = mPositionInfo.GetCloseOpponentToBall();
        Vector target = mBallState.GetPos() + Polar2Vector(dribble.mKickSpeed * 10, dribble.mAngle);
        if(!ServerParam::instance().pitchRectanglar().IsWithin(target)){
            continue;
        }
        bool ok = true;
        for (uint j = 0; j < opp2ball.size(); ++j) {
            Vector rel_pos = mWorldState.GetOpponent(opp2ball[j]).GetPos() - target;
            if (rel_pos.Mod() < dribble.mKickSpeed * 12 ||
                    mWorldState.GetOpponent(opp2ball[j]).GetPosConf() < PlayerParam::instance().minValidConf()){
                ok = false;
                break;
            }
        }
        if(!ok){
            continue;
        }
        dribble.mEvaluation = 0;
        for (int i = 1; i <= 8; ++i) {
            dribble.mEvaluation += Evaluation::instance().EvaluatePosition(mBallState.GetPos() + Polar2Vector(dribble.mKickSpeed * i, dribble.mAngle), true);
        }
        dribble.mEvaluation /= 8;
        dribble.mTarget = target;

        mActiveBehaviorList.push_back(dribble);
    }

    if (!mActiveBehaviorList.empty()) {
        mActiveBehaviorList.sort(std::greater<ActiveBehavior>());
        behavior_list.push_back(mActiveBehaviorList.front());
    }
}
