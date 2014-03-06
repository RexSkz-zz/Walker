#include "Types.h"
#include "Geometry.h"
#include "Thread.h"
#include "WorldState.h"

// 记录日志用于调试的程序

// 需要记录日志的类
class Observer;
class BallState;
class PlayerState;
class InfoState;

// 球员的视觉信息日志
// 注意！球员认为自己总是左方！所以left有关的都是我方球员的变量！
class SightLogger{
public:
	// 颜色数据
	enum Color{ Red, Blue, Green, Navy, Orange, Cyan, Purple, White, Black, Yellow, Olive };
	// 构造函数，初始化场上各种参数
	// observer:	观察者
	// world_state:	世界模型
	SightLogger(Observer *observer, WorldState *world_state);
	// 析构函数
	~SightLogger();
	// 从球员/服务器获取各类参数信息并对本类进行设置
	// msg:	信息原文
	void SetServerParamMsg(const char *msg);
	void SetPlayerParamMsg(const char *msg);
	void SetPlayerTypeMsg(const char *msg);
	// 获取球员的视觉信息并储存于本类
	void LogSight();
	// 将本类储存的信息写入文件
	void Flush();
	// 对mDecMutex进程加锁和解锁（子过程）
	void DecLock();
	void DecUnLock();
	// 设置当前时间（子过程）
	void LogDec();
	// 场上的基本元素，添加点、直线、圆（子过程）
	// 点是一个长度为2的vector
	// 直线用出发点和目标点两个基本点表示
	// 圆用圆心和半径表示
	// point/origin/target:	该数据的基本点
	// radius:				半径（仅针对圆）
	// comment:				注释，非常详细的信息（仅针对点）
	// color:				元素颜色，用于区分不同目标
	void AddPoint(const Vector & point, const char* comment = 0, Color color = Red);
	void AddLine(const Vector & origin, const Vector & target, Color color = Yellow);
	void AddCircle(const Vector & origin, const double & radius, Color color = White);
	// 记录球员和球的信息
	// player:	球员的状态
	// ball:	球的状态
	void LogPlayerInfo(const PlayerState & player);
	void LogBallInfo(const BallState & ball);
private:
	// 输出流
	ofstream os;
	// 防止多线程同时处理同一变量的错误
	ThreadMutex mSightMutex;
	ThreadMutex mDecMutex;
	// 观察者和世界模型
	Observer   *mpObserver;
	WorldState *mpWorldState;
	// 待输出信息的文件头
	string mHeader;
	// 三种类型的信息
	string mServerParamMsg;
	string mPlayerParamMsg;
	string mPlayerTypeMsg;
	// 当前已经记录过信息的球员数量
	int mLoggedPlayerTypeCount;
	// 是否可以记录视觉信息（要先记录好server_param等）
	bool mHeaderReady;
	bool mHeaderLogged;
	// 比赛模式
	ServerPlayMode mServerPlayMode;
	// 是否有官方的比赛模式（例如任意球、正常比赛）信息，默认为false
	// 从服务器接收数据并更新之后为true
	bool mServerPlayMode_dirty;
	// 比赛时间
	Time mTime;
	// 我方球队和对方球队的一些信息
	int mLeftScore;
	int mRightScore;
	int mLeftPenTaken;
	int mRightPenTaken;
	string mLeftName;
	string mRightName;
	// 是否有官方的球队状态，默认为false
	// 从服务器接收数据并更新之后为true
	bool mTeamState_dirty;
	// 球和球员的信息列表
	BallState *mpBall;
	PlayerArray<PlayerState*> mpLeftTeam;
	PlayerArray<PlayerState*> mpRightTeam;
	// 描述物体参数的基本类型
	struct ItemShape{
		Color line_color;
		// 枚举类型Color到字符串的转化
		// return:	若该物体有颜色则为颜色对应的字符串，否则为"black"
		const char *color();
		// 设置物体颜色
		// color:	颜色值
		ItemShape(Color color);
	};
	// 描述点的数据类型
	struct PointShape: public ItemShape{
		// 点的坐标
		double x, y;
		// 点的详细注释
		string comment;
		// 确定一个点的信息
		// point:	点
		// color:	颜色
		// cmt:		注释，可以为空
		PointShape(const Vector & point, Color color, const char* cmt = 0);
		// 将点的信息输出到流中
		friend ostream& operator<<(ostream &os, PointShape &point);
	};
	// 描述直线的数据类型
	struct LineShape: public ItemShape{
		// 两个点的坐标
		double x1, y1;
		double x2, y2;
		// 没找到其它引用，意义不明
		int width;
		// 确定一条直线的信息
		// from:	出发点
		// to:		目标点
		// color:	颜色
		LineShape(const Vector & from, const Vector & to, Color color);
		// 将直线信息输出到流中
		friend ostream& operator<<(ostream &os, const LineShape &line);
	};
	// 描述圆的数据类型
	struct CircleShape: public ItemShape{
		// 圆心坐标
		double x, y;
		// 半径
		double radius;
		// 确定一个圆的信息
		// center:	圆心
		// r:		半径
		// color:	颜色
		CircleShape(const Vector & center, const double & r, Color color);
		// 将圆的信息输出到流中
		friend ostream& operator<<(ostream &os, CircleShape &circle);
	};
	// 三种类型的数据的列表
	vector<PointShape> points;
	vector<LineShape> lines;
	vector<CircleShape> circles;
};

// 记录文字格式的日志的类
class TextLogger{
	// 输出流（底层不用管）
	ofstream os;
	// 临时存储输出信息的缓冲区（底层不用管）
	stringstream mBuffer;
public:
	// 构造函数
	// observer:	观察者
	// log_name:	日志文件的后缀
	TextLogger(Observer* observer, const char* log_name);
	TextLogger();
	// 析构函数
	~TextLogger();
	// 将缓冲区的内容写入文件
	void Flush();
	// 若设置了PlayerParam的mSaveTextLog为true则往输出信息缓冲区中写入数据
	template<typename T> TextLogger& operator<<(const T& value);
	// 想保留下面这段注释……（目测不用管吧）
	/**
	 * Copied from Mersad-5.9.5-RC2005. I dont know what these two specials
	 * are for. Will cause "undefined reference ..." if removed.
	 */
	TextLogger& operator<<(ostream& (*man)(ostream&));
	TextLogger& operator<<(ios_base& (*man)(ios_base&));
};

// 记录日志的类
class Logger: public Thread
{
	// 观察者与世界模型
	Observer *mpObserver;
	WorldState *mpWorldState;
	// 记录视觉信息和文字信息的变量
	SightLogger *mpSightLogger;
	map<string, TextLogger*> mTextLoggers;
	// 代替NULL的变量
	static TextLogger mTextLoggerNull;
	// 通过控制这个线程来控制是否记录日志
	ThreadCondition mCondFlush;
	// 构造函数，用于初始化基本信息
	Logger();
public:
	// 该类的唯一实例变量
	static Logger& instance();
	// 初始化观察者和世界模型（这个需要不止一次使用，所以不能写在构造函数中）
	// observer:	观察着额
	// world_state:	世界模型
	void Initial(Observer *observer, WorldState *world_state);
	// 析构函数
	virtual ~Logger();
	// 主循环
	void StartRoutine();
	// 将记录写入文件
	void Flush();
	// 如果条件满足，则设置线程，开始记录数据
	void SetFlushCond();
	// 当前时间
	const Time & CurrentTime();
	// 获取日志的记录者
	// logger_name:	文字信息记录者的名字
	SightLogger* GetSightLogger();
	TextLogger& GetTextLogger(const char* logger_name);
	// 用服务器信息来初始化相应的变量（底层，可以不用管）
	void InitSightLogger(ServerMsgType msg_type, const char* msg);
	// 获取球员的视觉信息并存储于mpSightLogger变量中
	void LogSight();
	// 记录各种类型的数据（点、直线、圆、矩形、目标点、射击点、拦截点、运球）
	// comment:	详细注释
	// 点
	void LogPoint(const Vector & target, SightLogger::Color color = SightLogger::Red, const char* comment = 0);
	// 直线
	void LogLine(const Vector & begin, const Vector & end, SightLogger::Color color = SightLogger::Yellow, const char* comment = 0);
	// 圆
	void LogCircle(const Vector & o, const double & r, SightLogger::Color color = SightLogger::Yellow);
	// 矩形
	void LogRectangular(const Rectangular & rect, SightLogger::Color color = SightLogger::Yellow);
	// 起点和目标点
	void LogGoToPoint(const Vector & start, const Vector & target, const char* comment = 0);
	// 射击点和目标点
	void LogShoot(const Vector & start, const Vector & target, const char* comment = 0);
	// 拦截点
	void LogIntercept(const Vector & interpt, const char* comment =0);
	// 连击起始点和目标点
	void LogDribble(const Vector & start, const Vector & target, const char * comment = 0, bool is_execute = false);
	// 传球起始点和目标点
	// reverse:	是否站在对方角度计算
	void LogPass(const bool reverse, const Vector & start, const Vector & target, const char * comment = 0, bool is_execute = false);
};
