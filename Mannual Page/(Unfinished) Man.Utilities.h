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
// 平方根模式，四倍于std::sqrt的速度，精度为1e-4
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
inline void NormalizeAngleDeg(AngleDeg & ang, const AngleDeg & min_ang = -180.0);
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
 *
 * GetRealTime()用在动态调试时不会经过的地方，即下面3个函数不能用的地方
 * GetRealTimeParser()用在Parser::Parse()及其调用的所有函数中
 * GetRealTimeDecision()用在Player::Decision()及其调用的所有函数中
 * GetRealTimeCommandSend()用在CommandSend::Run()及其调用的所有函数中
 */
timeval GetRealTime();
timeval GetRealTimeParser();
timeval GetRealTimeDecision();
timeval GetRealTimeCommandSend();

/**
 * 安全数组，Debug版本会检查越界访问
 * _Tp 数据域类型
 * _Nm 数组大小
 * _Zero 标记是否在构造函数内清零
 */
template<typename _Tp, std::size_t _Nm, bool _Zero = false>
class Array
{
	_Tp _M_instance[_Nm ? _Nm : 1];

public:
	Array() {
		if (_Zero) {
			bzero();
		}
	}

	Array(const _Tp & x) {
		fill(x);
	}

	_Tp &
	operator[](const std::size_t & i) {
		Assert(i < _Nm);
		return _M_instance[i];
	}

	const _Tp &
	operator[](const std::size_t & i) const  {
		Assert(i < _Nm);
		return _M_instance[i];
    }

	void bzero() {
		memset(_M_instance, 0, sizeof(_M_instance));
	}

	void fill(const _Tp & x) {
		std::fill(_M_instance, _M_instance + (_Nm ? _Nm : 1), x);
	}

	template<typename _Function>
	_Function
	for_each(_Function f) {
		return std::for_each(_M_instance, _M_instance + (_Nm ? _Nm : 1), f);
	}

	_Tp * instance() {
		return _M_instance;
	}
};

/**
 * 球员相关数组，有效下标：1..TEAMSIZE
 */
template <typename _Tp, bool _Zero = false>
class PlayerArray
{
	Array<_Tp, TEAMSIZE, _Zero> _M_array;

public:
	void bzero() {
		_M_array.bzero();
	}

	void fill(const _Tp & x) {
		_M_array.fill(x);
	}

	template<typename _Function>
	_Function
	for_each(_Function f) {
		return _M_array.for_each(f);
	}

	_Tp &	operator[](const Unum & i) {
		return _M_array[i - 1];
	}

	const _Tp & operator[](const Unum & i) const {
		return _M_array[i - 1];
	}
};

/**
 * 存放关于场上对象（球员和球）数据的数组
 * 有效下标：-TEAMSIZE..TEAMSIZE
 */
template <typename _Tp, bool _Zero = false>
class ObjectArray
{
	Array<_Tp, 2 * TEAMSIZE + 1, _Zero> _M_array; //0:ball, 1...11:teammate, 12...22:opponent

public:
	void bzero() {
		_M_array.bzero();
	}

	void fill(const _Tp & x) {
		_M_array.fill(x);
	}

	_Tp &	operator[](const ObjectIndex & i /*-11...11*/ ) {
		return i >= 0? _M_array[i]: _M_array[TEAMSIZE - i];
	}

	const _Tp & operator[](const ObjectIndex & i /*-11...11*/ ) const {
		return i >= 0? _M_array[i]: _M_array[TEAMSIZE - i];
	}

	_Tp & GetOfBall() {
		return (*this)[0];
	}

	const _Tp & GetOfBall() const {
		return (*this)[0];
	}

	_Tp & GetOfObject(const ObjectIndex & i /*-11...11*/ ) {
		return (*this)[i];
	}

	const _Tp & GetOfObject(const ObjectIndex & i /*-11...11*/ ) const {
		return (*this)[i];
	}

	_Tp & GetOfTeammate(const ObjectIndex & i /*1...11*/ ) {
		Assert(i > 0);
		return (*this)[i];
	}

	const _Tp & GetOfTeammate(const ObjectIndex & i /*1...11*/ ) const {
		Assert(i > 0);
		return (*this)[i];
	}

	_Tp & GetOfOpponent(const ObjectIndex & i /*1...11*/ ) {
		Assert(i > 0);
		return (*this)[-i];
	}

	const _Tp & GetOfOpponent(const ObjectIndex & i /*1...11*/ ) const {
		Assert(i > 0);
		return (*this)[-i];
	}
};

class RealTime
{
public:
	explicit RealTime(long tv_sec = 0, long tv_usec = 0) {
		mTime.tv_sec = tv_sec;
		mTime.tv_usec = tv_usec;
	}

	RealTime(const timeval &t): mTime(t) { }

	timeval GetTime() const { return mTime; }
	long GetSec() const { return mTime.tv_sec; }
	long GetUsec() const { return mTime.tv_usec; }

	const RealTime & operator = (const timeval &t) { mTime = t; return *this; }
	RealTime operator + (const RealTime &t) const;
	RealTime operator + (int msec) const;
	RealTime operator - (int msec) const;
	int operator - (const RealTime &t) const;
	long Sub( const RealTime &t);

	bool operator < (const RealTime &t) const;
	bool operator > (const RealTime &t) const;
	bool operator == (const RealTime &t) const;
	bool operator != (const RealTime &t) const;

	friend std::ostream & operator << (std::ostream &os, RealTime t);

private:
	timeval mTime;

	static timeval mStartTime;
	static const long ONE_MILLION;
};

class Time
{
public:
	explicit Time(int t = -3, int s = 0): mT(t), mS(s) {}

	int T() const { return mT; }
	int S() const { return mS; }
	void SetT(int t) { mT = t; }
	void SetS(int s) { mS = s; }

	const Time & operator=(const int a) { mT = a;  mS = 0; return *this; }
	Time operator+(const int a) const { return Time(mT + a, 0); }
	Time operator-(const int a) const;
	int operator-(const Time &a) const;

	void operator+=(const int a) { *this = *this + a; }
	void operator-=(const int a) { *this = *this - a; }
	void operator-=(const Time &a) { *this = *this - a; }
	const Time & operator++() { *this += 1; return *this; }
	const Time & operator--() { *this -= 1; return *this; }
	int operator%(const int a) const { return (mT + mS) % a; }

	bool operator==(const Time &a) const { return (mS == a.S()) && (mT == a.T()); }
	bool operator!=(const Time &a) const { return (mS != a.S()) || (mT != a.T()); }
	bool operator<(const Time &a) const { return (mT < a.T()) || (mT == a.T() && mS < a.S()); }
	bool operator<=(const Time &a) const { return (mT < a.T()) || ( mT == a.T() && mS <= a.S()); }
	bool operator>(const Time &a) const { return (mT > a.T()) || (mT == a.T() && mS > a.S()); }
	bool operator>=(const Time &a) const { return (mT > a.T()) || (mT == a.T() && mS >= a.S()); }
	bool operator!() const { return (mS == 0) && (mT == 0); }
	friend std::ostream & operator<<(std::ostream & os, const Time& t) { return os << '(' << t.T() << ':' << t.S() << ')'; }

private:
	int mT;
	int mS;
};


/**
 * bellow is parse utilities
 */
namespace parser {
inline double get_double(char **str_ptr){
	while (!isdigit(**str_ptr) && **str_ptr != '-' && **str_ptr != '+' && **str_ptr != '.' && **str_ptr) (*str_ptr)++;
	return strtod(*str_ptr, str_ptr);
}

inline double get_double(char *str){
	while (!isdigit(*str) && *str != '-' && *str != '+' && *str != '.' && *str) str++;
	return strtod(str, (char **) 0);
}

inline int get_int(char **str_ptr){
	while (!isdigit(**str_ptr) && **str_ptr != '-' && **str_ptr != '+' && **str_ptr != '.' && **str_ptr) (*str_ptr)++;
	return static_cast<int>(strtol(*str_ptr, str_ptr, 10));
}

inline int get_int(char *str){
	while (!isdigit(*str) && *str != '-' && *str != '+' && *str != '.' && *str) str++;
	return static_cast<int>(strtol(str, (char **) 0, 10));
}

inline char * get_word(char **str_ptr){
	while ( !isalpha(**str_ptr) && **str_ptr) (*str_ptr)++;
	return *str_ptr;
}

inline char * get_next_word(char **str_ptr){
	while ( isalpha(**str_ptr) ) (*str_ptr)++;
	return get_word(str_ptr);
}
}

namespace sight {
inline AngleDeg ViewAngle(ViewWidth view_width)
{
	switch(view_width){
	case VW_Narrow: return 60.0;
	case VW_Normal: return 120.0;
	case VW_Wide: return 180.0;
	default: Assert(0); return 90.0;
	}
}

inline int SightDelay(ViewWidth view_width)
{
	switch(view_width) {
	case VW_Narrow: return 1;
	case VW_Normal: return 2;
	case VW_Wide: return 3;
	default: Assert(0); return 1;
	}
}
}

inline double Quantize( const double v, const double q )
{
    return Rint( v / q ) * q;
}

template <class Data> class TimedData
{
public:
	TimedData(Time time = Time(-3, 0))               { mTime = time; }
	TimedData(const Data &data, Time time)  { mData = data; mTime = time; }

	const Data & GetData() const        { return mData; }
	const Time & GetTime() const        { return mTime; }
	void SetData(const Data &data)      { mData = data; }
	void SetTime(const Time time)       { mTime = time; }

	void operator = (const Data &data)  { mData = data; }
	TimedData operator + (TimedData d)  { return TimedData(mData + d, mTime); }
	TimedData operator - (TimedData d)  { return TimedData(mData - d, mTime); }
	TimedData operator / (TimedData d)  { return TimedData(mData / d, mTime); }
	TimedData operator * (TimedData d)  { return TimedData(mData * d, mTime); }

private:
	Data mData;
	Time mTime;
};

/** Key Player Information */
class KeyPlayerInfo
{
public:
	Unum    mUnum;
	double  mValue;

	KeyPlayerInfo():
		mUnum ( 0 ),
		mValue ( 0.0 )
	{
	};

	bool operator < (const KeyPlayerInfo &other) const  { return mValue < other.mValue; }
	bool operator > (const KeyPlayerInfo &other) const  { return mValue > other.mValue; }
	bool operator == (const KeyPlayerInfo &other) const { return fabs(mValue - other.mValue) < FLOAT_EPS; }
};

class Updatable {
public:
	Updatable(): mUpdateTime(Time(-3, 0)) {}

	virtual ~Updatable(){}

	void UpdateAtTime(const Time & time) {
		if (mUpdateTime != time){
			mUpdateTime = time;
			UpdateRoutine();
		}
	}

private:
	virtual void UpdateRoutine() = 0;

private:
	Time mUpdateTime;
};

class ServerPlayModeMap {
public:
	static ServerPlayModeMap & instance();
	ServerPlayMode GetServerPlayMode(const std::string & str);
	const char * GetPlayModeString(ServerPlayMode spm);

private:
	ServerPlayModeMap();
	void Bind(const std::string & str, ServerPlayMode spm);

private:
	std::map<std::string, ServerPlayMode> mString2Enum;
	std::map<ServerPlayMode, std::string> mEnum2String;
};


template<class _Container, typename _Tp>
bool has(const _Container & x, const _Tp & key) {
	return std::find(x.begin(), x.end(), key) != x.end();
}
