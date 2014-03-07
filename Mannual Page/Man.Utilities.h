// 一些小函数

#include "Types.h"

// 判断某数是否合法
inline bool IsInvalid(const double & x);
// 返回最接近参数的整数（还是double型）
// 数值范围在(x-0.5, x+0.5]之间的数会被转换为x
inline double Rint(const double & x);
// 平方
inline double Sqr(const double & x);
// 平方根
// 这里原本有个_FAST_SQRT的宏，如果定义了它，则进入快速
// 平方根模式，四倍于sqrt的速度，精度为1e-4
// 不过不用纠结这个了
inline double Sqrt(const double x);
// 最大值和最小值
template<typename _Tp> inline const _Tp& Max(const _Tp& x, const _Tp& y);
template<typename _Tp> inline const _Tp& Min(const _Tp& x, const _Tp& y);
// 若x介于min和max之间则返回x，否则返回min和max中最接近x的数
template<typename _Tp> inline const _Tp& MinMax(const _Tp& min, const _Tp& x, const _Tp& max);
// 符号函数
template<typename _Tp> inline int Sign(const _Tp& x);
// 弧度转角度
inline AngleDeg Rad2Deg(const AngleRad & x);
// 角度转弧度
inline AngleRad Deg2Rad(const AngleDeg & x);
// 正弦值（参数为角度）
inline double Sin(const AngleDeg & x);
// 余弦值（参数为角度）
inline double Cos(const AngleDeg & x);
// 返回pair<double, double>，分别是角度对应的Sin和Cos;
inline SinCosT SinCos(const AngleDeg & x);
// 返回SinCosT的两个分量
inline const double & Sin(const SinCosT & value);
inline const double & Cos(const SinCosT & value);
// 正切值（参数为角度）
inline double Tan(const AngleDeg & x);
// 反余弦值，若超出定义域则改为定义域内的值
inline AngleDeg ACos(const double & x);
// 反正弦值，超出定义域自动修正
inline AngleDeg ASin(const double & x);
// 反正切值（参数为正切值），范围[-90.0, 90.0]
inline AngleDeg ATan(const double & x);
// 反正切值（参数为坐标），范围[-180.0, 180.0]
inline AngleDeg ATan2(const double & y, const double & x);
// 将角度化为[min_ang, min_ang + 360]范围内等价的角
inline AngleDeg GetNormalizeAngleDeg(AngleDeg ang, const AngleDeg & min_ang = -180.0);
// 上文函数的修改（未找到引用，不过在部分情况下可以简化操作）
// 有点类似于Ruby中，Array.sort返回排序之后的数组，Array.sort!直接对数组进行排序
/*inline void NormalizeAngleDeg(AngleDeg & ang, const AngleDeg & min_ang = -180.0);*/
// 将弧度化为[min_arg, min_arg + PI]范围内等价的角
inline AngleRad GetNormalizeAngleRad(AngleRad ang, const AngleRad & min_ang = -M_PI);
// 上文函数的修改
inline void NormalizeAngleRad(AngleRad & ang, const AngleRad & min_ang = -M_PI);
// 两个角度之差
inline AngleDeg GetAngleDegDiffer(const AngleDeg & ang1, const AngleDeg & ang2);
// 两个弧度之差
inline AngleRad GetAngleRadDiffer(const AngleRad & ang1, const AngleRad & ang2);
// 判断角度gamma是否位于由alpha按顺时针方向旋转到beta所围成的扇形区间内\
inline bool IsAngleDegInBetween(const AngleDeg & alpha, const AngleDeg & gamma, const AngleDeg & beta);
// 判断gamma与beta相比是否离alpha更近
inline bool IsAngleDegCloseToLeft(const AngleDeg & alpha, const AngleDeg & gamma, const AngleDeg & beta);
// 几何级数的和，公式为first_term / (1 - r)
// first_term:	首项
// r:			公比
inline double SumInfGeomSeries(const double & first_term, const double & r);
// 生成从low与high之间的随机数
inline double drand(double low, double high);
/**
 * 下面四个函数都是得到系统时间，但用的地方不同，一定要注意
 * GetRealTime()用在动态调试时不会经过的地方，即下面3个函数不能用的地方
 * GetRealTimeParser()用在Parser::Parse()及其调用的所有函数中
 * GetRealTimeDecision()用在Player::Decision()及其调用的所有函数中
 * GetRealTimeCommandSend()用在CommandSend::Run()及其调用的所有函数中
 */
timeval GetRealTime();
timeval GetRealTimeParser();
/*
timeval GetRealTimeDecision();		// 未找到引用
timeval GetRealTimeCommandSend();	// 未找到引用
*/

// 安全数组类，Debug版本会检查越界访问
// _Tp:		数据域类型
// _Nm:		数组大小
// _Zero:	是否需要在构造函数内清零
template<typename _Tp, size_t _Nm, bool _Zero = false> class Array{
	// 该类的实例
	_Tp _M_instance[_Nm ? _Nm : 1];
public:
	Array();
	// 在构造函数中将元素填充为x
	Array(const _Tp & x);
	// 获取第i个元素
	_Tp & operator[](const size_t & i);
	const _Tp & operator[](const size_t & i);
	// 将数组清零
	void bzero();
	// 填充数组为x
	void fill(const _Tp & x);
	// 未找到引用，意义不明
	/*template<typename _Function> _Function for_each(_Function f);*/
	// 唯一实例
	_Tp * instance();
};

// 球员相关数组，有效下标：1..TEAMSIZE
template <typename _Tp, bool _Zero = false> class PlayerArray{
	Array<_Tp, TEAMSIZE, _Zero> _M_array;
public:
	// 清零
	void bzero();
	// 填充
	void fill(const _Tp & x);
	// 未找到引用，意义不明
	/*template<typename _Function> _Function for_each(_Function f);*/
	// 获取第i个元素
	_Tp & operator[](const Unum & i);
	const _Tp & operator[](const Unum & i);
};

// 场上对象（球员和球）数组，有效下标：-TEAMSIZE..TEAMSIZE
template <typename _Tp, bool _Zero = false> class ObjectArray{
	//0：球；1-11：队友；12-22：对手
	Array<_Tp, 2 * TEAMSIZE + 1, _Zero> _M_array;
public:
	// 清零
	void bzero();
	// 填充
	void fill(const _Tp & x);
	// 获取第i个元素（下标i的范围是-11至11）
	_Tp & operator[](const ObjectIndex & i);
	const _Tp & operator[](const ObjectIndex & i);
	// 获得代表球的元素
	_Tp & GetOfBall();
	const _Tp & GetOfBall();
	// 获得物品（等价于方括号）
	_Tp & GetOfObject(const ObjectIndex & i);
	const _Tp & GetOfObject(const ObjectIndex & i);
	// 获得第i号队友（下标i的范围是1至11）
	_Tp & GetOfTeammate(const ObjectIndex & i);
	const _Tp & GetOfTeammate(const ObjectIndex & i);
	// 获得第i号对手（下标i的范围是1至11）
	_Tp & GetOfOpponent(const ObjectIndex & i);
	const _Tp & GetOfOpponent(const ObjectIndex & i);
};

// 封装好的timeval类
class RealTime{
public:
	// 通过秒数和微秒数来初始化
	// tv_sec:	秒数
	// tv_usec:	微秒数
	explicit RealTime(long tv_sec = 0, long tv_usec = 0);
	// 通过timeval类型来初始化
	RealTime(const timeval &t): mTime(t) { }
	// 返回当前对象代表的timeval类型的值
	timeval GetTime();
	// 返回秒数和微秒数
	long GetSec();
	long GetUsec();
	// 赋值、两种加减法
	const RealTime & operator=(const timeval &t);
	RealTime operator+(const RealTime &t);
	RealTime operator+(int msec);
	RealTime operator-(int msec);
	int operator-(const RealTime &t);
	long Sub(const RealTime &t);
	// 比较运算符
	bool operator<(const RealTime &t);
	bool operator>(const RealTime &t);
	bool operator==(const RealTime &t);
	bool operator!=(const RealTime &t);
	// 输出总共经过的时间（不是当前时间）
	friend ostream & operator << (ostream &os, RealTime t);
private:
	// 该类存储的时间
	timeval mTime;
	// 起始时间
	static timeval mStartTime;
	// 一百万单位（微秒换算成秒）
	static const long ONE_MILLION;
};

// 存储时间的类
class Time{
public:
	// 初始化时间
	// t:	当前一轮比赛运行时间
	// s:	当前一轮比赛暂停时间
	explicit Time(int t = -3, int s = 0);
	// 各种getter和setter
	int T();
	int S();
	void SetT(int t);
	void SetS(int s);
	// 简单地赋值
	const Time & operator=(const int a);
	// 增加比赛的运行时间
	Time operator+(const int a);
	// 减少比赛的暂停时间，若a大于当前暂停时间则差量的绝对值
	// 将会加到当前运行时间中
	Time operator-(const int a) const;
	int operator-(const Time &a) const;
	// 各种单目运算符
	void operator+=(const int a);
	void operator-=(const int a);
	void operator-=(const Time &a);
	const Time & operator++();
	const Time & operator--();
	// 理解不能
	int operator%(const int a);
	// 各种比较运算符
	bool operator==(const Time &a);
	bool operator!=(const Time &a);
	bool operator<(const Time &a);
	bool operator<=(const Time &a);
	bool operator>(const Time &a);
	bool operator>=(const Time &a);
	bool operator!() const { return (mS == 0) && (mT == 0); }
	// 输出格式：(比赛运行时间:比赛暂停时间)
	friend ostream & operator<<(ostream & os, const Time& t);
private:
	// 比赛运行时间和暂停时间
	int mT;
	int mS;
};

// 解释器中的小函数
// 通过字符指针或二重指针从字符串中提取出相应元素
// 忽略当前指针位置下的所有符号，遇到数字或字母才停下
// str_ptr:	字符指针
namespace parser{
	inline double get_double(char **str_ptr);
	inline double get_double(char *str);
	inline int get_int(char **str_ptr);
	inline int get_int(char *str);
	inline char *get_word(char **str_ptr);
	inline char *get_next_word(char **str_ptr);
}
// 通过指定的视野宽度获取相应的视野角度和视觉延迟
// view_width:	视野宽度（枚举类型）
namespace sight{
	inline AngleDeg ViewAngle(ViewWidth view_width);
	inline int SightDelay(ViewWidth view_width);
}
// 将连续值v转换为离散值，最小单位为q
inline double Quantize(const double v, const double q);
// 看起来像Time和Data的合体，不过奇怪为什么不用一个pair来代替
// 未找到引用
/*
template<class Data> class TimedData{
public:
	TimedData(Time time = Time(-3, 0));
	TimedData(const Data &data, Time time);
	const Data & GetData();
	const Time & GetTime();
	void SetData(const Data &data);
	void SetTime(const Time time);
	void operator = (const Data &data);
	TimedData operator + (TimedData d);
	TimedData operator - (TimedData d);
	TimedData operator / (TimedData d);
	TimedData operator * (TimedData d);
private:
	Data mData;
	Time mTime;
};
*/

// 看起来是记录球员的，不过mValue没有在别的地方被引用值
// 而是简单地被赋值，所以没搞明白
class KeyPlayerInfo{
public:
	// 球员编号
	Unum mUnum;
	double mValue;
	// 构造函数
	KeyPlayerInfo();
	// 比较运算符（仅比较mValue）
	// other:	比较的另一目标
	bool operator<(const KeyPlayerInfo &other);
	bool operator>(const KeyPlayerInfo &other);
	bool operator==(const KeyPlayerInfo &other);
};

// 可更新的物品类
class Updatable{
public:
	Updatable();
	virtual ~Updatable();
	// 若当前时间比最近一次更新时间新则更新物品
	// time:	当前时间
	void UpdateAtTime(const Time & time);
private:
	// 更新物品信息，在子类中被重写
	virtual void UpdateRoutine() = 0;
private:
	// 最近一次更新的时间
	Time mUpdateTime;
};

// 在ServerPlayMode枚举类型与字符串类型之间的转换
class ServerPlayModeMap{
public:
	// 唯一实例
	static ServerPlayModeMap & instance();
	// 两个转换函数
	ServerPlayMode GetServerPlayMode(const string & str);
	const char * GetPlayModeString(ServerPlayMode spm);
private:
	// 构造函数
	ServerPlayModeMap();
	// 添加一个转换规则（小函数）
	void Bind(const string & str, ServerPlayMode spm);
private:
	// 存储转换规则的map
	map<string, ServerPlayMode> mString2Enum;
	map<ServerPlayMode, string> mEnum2String;
};

// 判断某个容器内是否有某元素
// x:		容器
// key:		查找的元素
// return:	若有该元素则为true，否则为false
template<class _Container, typename _Tp> bool has(const _Container & x, const _Tp & key);
