#include "Geometry.h"
#include "ActionEffector.h"
#include "Formation.h"

// 存储所有动作以及产生者、执行者、执行器的基类
// 这里的“执行者”指的是在虚拟世界中执行该动作的对象
// 而“执行器”指的是程序实现的时候用到的类

// 用到的类
class WorldState;
class BallState;
class PlayerState;
class InfoState;
class PositionInfo;
class InterceptInfo;
class Strategy;
class Analyser;
class Agent;

// 动作种类
enum BehaviorType{
	BT_None,
	BT_Penalty,
	BT_Goalie,
	BT_Setplay,
	BT_Position,
	BT_Dribble,
	BT_Hold,
	BT_Pass,
	BT_Shoot,
	BT_Intercept,
	BT_Formation,
	BT_Block,
	BT_Mark,
	BT_Max
};

// 动作细节种类
enum BehaviorDetailType{
	BDT_None,
	BDT_Intercept_Normal,
    BDT_Pass_Ahead,
    BDT_Pass_Direct,
    BDT_Pass_Clear,
	BDT_Dribble_Fast,
	BDT_Dribble_Normal,
    BDT_Position_Normal,
    BDT_Position_Rush,
	BDT_Shoot_Tackle,
	BDT_Hold_Turn,
	BDT_Hold_Kick,
    BDT_Goalie_Position,
    BDT_Goalie_Catch,
    BDT_Goalie_Tackle,
    BDT_Setplay_Move,
    BDT_Setplay_Scan,
    BDT_Setplay_GetBall
};

// 用于执行动作的类
class BehaviorExecutable;

// 所有动作的集合
class BehaviorFactory{
	BehaviorFactory();
	virtual ~BehaviorFactory();
public:
	typedef BehaviorExecutable* (*BehaviorCreator)(Agent & agent);
	static BehaviorFactory &instance();
	// 创建一个动作
	// agent: 	行动者
	// type: 	动作类型
	// return: 	一个可执行的动作函数
	BehaviorExecutable *CreateBehavior(Agent & agent, BehaviorType type);
	// 注册行为，并在集合中创建函数
	// 只有具有执行效果的行为需要注册，比如BehaviorAttackPlanner，BehaviorDefensePlanner不需要注册
	// type:			行为类型
	// creator:			创建函数
	// behavior_name:	函数名称
	// return:			是否注册成功
	bool RegisterBehavior(BehaviorType type, BehaviorCreator creator, const char *behavior_name);
	// 通过动作种类获取对应名称用于记录
	const char *GetBehaviorName(BehaviorType type);
private:
	// 动作发出者的列表
	Array<BehaviorCreator, BT_Max, true> mCreatorMap;
	// 动作到其名称的映射
	Array<string, BT_Max> mNameMap;
};

// 由BehaviorPlanner产生的结果
// 仅仅是结果，并不直接负责执行
// 每个ActiveBehavior的优劣由里面存储的mEvaluation表示
class ActiveBehavior{
	// 动作类型和动作执行者
	BehaviorType mType;
	Agent * mpAgent;
public:
	ActiveBehavior(Agent & agent, BehaviorType type, BehaviorDetailType detail_type = BDT_None);
	// 令其完全不可行，相当于清除该行为
	void Clear();
	// 获取和设置动作类型
	const BehaviorType & GetType();
	void SetType(const BehaviorType &type);
	// 获取动作执行者
	Agent & GetAgent();
	// 调用BehaviorExecutable类执行该动作
	// 不依赖于指向behavior的指针类型，只要求BehaviorType正确
	bool Execute();
	// 提交视觉请求
	// plus:	请求的强度，可以理解为每plus周期看一次
	void SubmitVisualRequest(double plus);
	// 比较两个动作的优劣
	bool operator>(const ActiveBehavior & ab);
	bool operator==(const ActiveBehavior & ab);
	bool operator<(const ActiveBehavior & ab);
	// 存储该动作的优劣
	double mEvaluation;
	// 动作的细节
	int mKickCycle;		// 踢球的周期，可能多脚踢球
	AngleDeg mAngle;	// 角度
	Vector mTarget;		// 目标点地位置，可以是把球踢到该点和自己跑到该点等
	double mPower;		// dash的power
	double mDistance;	// 目标点到自己的距离
	double mKickSpeed;	// kick的球速
	bool mFoul;			//是否要故意犯规铲球
	// 特殊球员的信息
	KeyPlayerInfo mKeyTm;		// 队友
	KeyPlayerInfo mKeyOpp;		// 对手
	KeyPlayerInfo mKeyOppGB;	// opponent to get ball
	KeyPlayerInfo mKeyOppGT;	// opponent to go through
	// ？？？？
	BehaviorDetailType mDetailType;	//细节类型
	double mBuffer;					//有些行为执行时的buffer是在plan时算好的，要先存到这个变量里
};

// 攻击动作
class BehaviorAttackData{
public:
	// 设置动作执行者
	BehaviorAttackData(Agent & agent);
	virtual ~BehaviorAttackData();
	// 动作执行者以及其眼中的：
	// 世界、球、自身状态、位置信息、拦截信息、策略、编队
	Agent & mAgent;
	const WorldState & mWorldState;
	const BallState & mBallState;
	const PlayerState & mSelfState;
	PositionInfo & mPositionInfo;
	InterceptInfo & mInterceptInfo;
	Strategy & mStrategy;
    Formation & mFormation;
};

// 防御动作
class BehaviorDefenseData: public BehaviorAttackData{
public:
	// 设置执行者
	BehaviorDefenseData(Agent & agent);
	virtual ~BehaviorDefenseData();
	// 位置信息的分析者
	Analyser & mAnalyser;
};

// 模板类，针对实际传入的类的类型来指定Plan
// 继承于传入的类
template<class BehaviorDataType> class BehaviorPlannerBase: public BehaviorDataType{
	BehaviorPlannerBase(const BehaviorPlannerBase &);
public:
	// 动作执行者
	BehaviorPlannerBase(Agent & agent): BehaviorDataType(agent){}
	virtual ~BehaviorPlannerBase(){}
	// 做决策，产生最好的ActiveBehavior，存到behavior_list里面
	// 这个过程要重点看
	virtual void Plan(list<ActiveBehavior> & behavior_list) = 0;
public:
	// 行为列表的getter
	const list<ActiveBehavior> & GetActiveBehaviorList();
protected:
	// 所有可能的行为列表
	list<ActiveBehavior> mActiveBehaviorList;
};

// 用于执行动作的类
class BehaviorExecutable{
	BehaviorExecutable(const BehaviorExecutable&);
public:
	// 空构造函数和析构函数
	BehaviorExecutable();
	virtual ~BehaviorExecutable();
	// 执行传入的ActiveBehavior
	// 每周期只有一个行为被执行
	virtual bool Execute(const ActiveBehavior & act_bhv) = 0;
	// 提交视觉请求
	// 可以有多个行为提交视觉请求
	// plus:	视觉请求的强度
	virtual void SubmitVisualRequest(const ActiveBehavior & act_bhv, double plus = 0.0);
	// 简化AutoRegister函数所用
	// 用法：Creator<BehaviorDerived>
	// 返回BehaviorDerived(&agent)类型的指针
	template<class BehaviorDerived> static BehaviorExecutable* Creator(Agent & agent);
	// 调用RegisterBehavior注册各种Executer
	// 例如：AutoRegister<BehaviorBlockExecuter>();
	// BehaviorDerived:	Executer的类型
	template<class BehaviorDerived> static bool AutoRegister();
};

// 真正的动作执行器的基类，模板参数为动作类型
// 有不同的执行器类继承自这个类
// 也就是说一种动作对应一个执行器
// 个人认为为了省代码而写这种类没意义
template<class BehaviorDataType> class BehaviorExecuterBase: public BehaviorDataType, public BehaviorExecutable{
	BehaviorExecuterBase(const BehaviorExecuterBase &);
public:
	// 设置动作的执行者
	BehaviorExecuterBase(Agent & agent);
	virtual ~BehaviorExecuterBase(){}
};

// 具有执行效果的动作的类型定义
typedef list<ActiveBehavior> ActiveBehaviorList;
typedef list<ActiveBehavior>::iterator ActiveBehaviorPtr;
// ？？？？
#define TeammateFormationTactic(TacticName) (*(FormationTactic##TacticName *)mFormation.GetTeammateTactic(FTT_##TacticName))
#define OpponentFormationTactic(TacticName) (*(FormationTactic##TacticName *)mFormation.GetOpponentTactic(FTT_##TacticName))
