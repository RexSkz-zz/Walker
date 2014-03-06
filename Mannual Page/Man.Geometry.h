#include "Utilities.h"
#include "Plotter.h"

// 向量类
class Vector{
public:
	// 用坐标初始化
	explicit Vector(const double & x = 0.0, const double & y = 0.0): mX(x), mY(y);
	// 分别返回坐标的两个分量
	const double & X();
	const double & Y();
	// 分别设置坐标的两个分量
	void SetX(const double & x);
	void SetY(const double & y);
	// 用直角坐标赋值
	void SetValue(const double & x, const double & y);
	// 用极坐标赋值
	void SetValuePolar(const double & r, const AngleDeg & theta);
	// 向量的基本运算：取反、加减、数乘、除法、比较
	// 除法相当于数乘，不过特判了数非常小的情况，除数绝对值的下界是FLOAT_EPS
	Vector operator-();
	Vector operator+(const Vector &a);
	Vector operator-(const Vector &a);
	Vector operator*(const double & a);
	Vector operator/(double a);
	void operator+=(const Vector &a);
	void operator+=(const double &a);
	void operator-=(const Vector &a);
	void operator-=(const double &a);
	void operator*=(const double &a);
	void operator/=(const double &a);
	bool operator!=(const Vector &a);
	bool operator!=(const double &a);
	bool operator==(const Vector &a);
	// 输出一个向量，形式为(x, y)
	friend std::ostream& operator<<(std::ostream & os, const Vector & v);
	// 向量的一些属性：模长、模长^2、距离、距离^2、与x轴夹角
	// a:	另一个向量
	double Mod();
	double Mod2();
	double Dist(const Vector &a);
	double Dist2(const Vector &a);
	AngleDeg Dir();

	// 返回一个长度为length、方向与该向量相同的向量
	// 如果该向量为(0, 0)，则返回(0, 0)
	// a:	返回向量的长度
	Vector SetLength(const double & length);
	// 对向量进行标准化，返回长度为1、方向与该向量相同的向量
	Vector Normalize();
	// 返回一个与当前向量有指定夹角的向量
	// angle:	旋转角度（逆时针）
	Vector Rotate(const AngleDeg & angle);
	// 同上，参数类型为sin和cos组成的集合
	Vector Rotate(const SinCosT & value);
	// 判断一个向量是否与该向量几乎相等
	// a:		用来比较的向量
	// return:	若几乎相等则为true，否则为false
	bool ApproxEqual(const Vector& a);
private:
	// 断言，坐标范围是否合法，在每次计算中都会被调用
	void AssertValid();
private:
	// 向量的坐标
	double mX;
	double mY;
};
// 通过极坐标生成一个向量
// mod:		极坐标的长度
// ang:		极坐标的极角
// return:	生成的向量（直角坐标）
inline Vector Polar2Vector(const double & mod, const AngleDeg & ang);

// 直线类的声明
// 直线类和射线类互相调用
class Line;

// 射线类
class Ray{
public:
	Ray();
	// 根据端点和方向确定一条射线
	// origin:		端点
	// direction:	方向
	explicit Ray(const Vector & origin, const AngleDeg & direction)
	// 取得射线的端点和方向
	const Vector & Origin();
	const AngleDeg & Dir();
	// 设置射线的端点和方向
	// origin:		端点
	// direction:	方向
	void SetOrigin(const Vector & origin);
	void SetDirection(const AngleDeg & direction);
	void SetValue(const Vector & origin, const AngleDeg & direction);
	// 取得射线上距离端点为dist的点
	Vector GetPoint(const double & dist);
	// 某一点是否在该向量的右边
	// point:	待判断的点
	// return:	若点在顺时针
	bool IsInRightDir(const Vector & point);
	// 判断点是否在射线上（未找到引用，意义不明）
	bool OnRay(const Vector& point, const double & buffer = FLOAT_EPS);
	// 该射线与另一直线或射线的交点信息
	// l:					另一条直线
	// r:					另一条射线
	// intersection_dist:	存储交点距离本射线端点的距离
	// point:				存储交点
	// return:				若相交则为true，否则为false
	bool Intersection(const Line & l, double & intersection_dist);
	bool Intersection(const Line & l, Vector & point);
	bool Intersection(const Ray & r, Vector & point);
	bool Intersection(const Ray & r, double & intersection_dist);
	// 该射线与另一直线的交点与其端点的距离
	// l:		另一条直线
	// return:	若相交则返回距离，否则返回-1000
	double Intersection(const Line & l);
	// 获取一条射线上离某个点最近的点
	Vector GetClosestPoint(const Vector& point);
	// 获取一个点到该射线端点的距离
	// point:	待计算的点
	inline double GetDistanceFromOrigin(const Vector& point);
private:
	// 射线的端点和方向
	Vector mOrigin;
	AngleDeg mDirection;
};

// 直线类
// 程序中直线用Ax+By+C=0表示
class Line{
public:
	// 构造函数
	explicit Line(const double & a = 0.0, const double & b = 0.0, const double & c = 0.0);
	// 由指定两点生成一条直线
	Line(const Vector & point1, const Vector & point2);
	// 由指定射线生成一条直线
	Line(const Ray &r);
	// 获取直线的系数参数
	const double & A();
	const double & B();
	const double & C();
	// 获取直线方向
	double Dir();
	// 已知直线上一点的X（或Y），求对应的Y（或X）
	double GetX(const double & y);
	double GetY(const double & x);
	// 判断点是否在直线上
	bool IsOnLine(const Vector & point, const double & buffer = FLOAT_EPS);
	// 判断点是否在直线上方
	bool IsUpLine(const Vector & point);
	// （未找到引用，意义不明）
	bool HalfPlaneTest(const Vector & pt);
	// 判断该直线与另一直线斜率是否相等
	bool IsSameSlope(const Line & l, const double & buffer = FLOAT_EPS);
	// 判断一点的垂足是否在两点之间
	// pt:		待判断的点
	// end1:	两点之一
	// end2:	两点之一
	bool IsInBetween(const Vector & pt, const Vector & end1, const Vector & end2);
	// 求与各种线的交点，和上文中射线的方法相同
	bool Intersection(const Line &l, Vector &point);
	Vector Intersection(const Line &l);
	bool Intersection(const Ray &r, Vector &point);
	// 点到直线的距离
	double Dist(const Vector& point);
	// 两点是否在直线同侧
	bool IsPointInSameSide(const Vector & pt1, const Vector & pt2);
	// 取得过某点的垂线
	Line GetPerpendicular(const Vector & pt);
	// 将此直线设为两点的垂直平分线
	// pos1:	一个点
	// pos2:	另一个点
	void PerpendicularBisector(const Vector & pos1, const Vector & pos2);
	// 得到某点在该直线上的投影点
	// pt:		原始点
	// return:	投影点
	Vector GetProjectPoint(const Vector & pt);
	//得到对称点
	inline Vector MirrorPoint(const Vector & pt);
	// 得到直线上两点间距离这个点最近的点
	// pt:		原始点
	// end1:	直线上一点
	// end2:	直线上另一点
	// return:	直线上两点间距离原始点最近的点
	Vector GetClosestPointInBetween(const Vector & pt, const Vector & end1, const Vector & end2);
	// （未找到引用，意义不明）
	void LineFromPline(const Vector & pos1, const Vector & pos2);
	// 取得直线的三个参数
	const double & GetA();
	const double & GetB();
	const double & GetC();
private:
	// 直线的三个参数
	double mA;
	double mB;
	double mC;
};


// 矩形类
// 存储的是四条边所在直线与坐标轴的截距
// 例如(1, 2, 3, 4)就表示从(1, 3) - (2, 4)的一个矩形
class Rectangular{
public:
	// 构造函数，创建矩形，参数默认为0
	Rectangular();
	Rectangular(const double & left, const double & right, const double & top, const double & bottom);
	// 根据中心点和长宽大小指定矩形
	// center:	中心点坐标
	// size:	存储长宽大小的有序实数对
	Rectangular(const Vector & center, const Vector & size);
	// 获取矩形信息
	const double & Left();
	const double & Right();
	const double & Top();
	const double & Bottom();
	// 设置矩形信息
	void SetLeft(const double & left);
	void SetRight(const double & right);
	void SetTop(const double & top);
	void SetBottom(const double & bottom);
	// 获得矩形顶点坐标
	Vector TopLeftCorner();
	Vector TopRightCorner();
	Vector BottomLeftCorner();
	Vector BottomRightCorner();
	// 获取矩形的四条边所在直线
	Line TopEdge();
	Line BottomEdge();
	Line LeftEdge();
	Line RightEdge();
	// 判断点是否在矩形中（包括边缘）
	bool IsWithin(const Vector &v, const double & buffer = FLOAT_EPS);
	// 矩形和射线的交点（假设射线端点在矩形内）
	bool Intersection(const Ray &r, Vector &point);
	Vector Intersection(const Ray &r);
	// 将界外的点移动到界内（横竖方向上平移）
	Vector AdjustToWithin(const Vector &v);
private:
	// 矩形的四条边
	double mLeft;
	double mRight;
	double mTop;
	double mBottom;
};

// 曲线类
// 存储方式为a+b/(x+c)=0，本质上是平移缩放之后的双曲线y=1/x
class ReciprocalCurve{
public:
	// 构造函数，生成一条曲线
	// a, b, c:	曲线的三个参数
	// out_min:	横坐标最小值
	// out_max:	横坐标最大值
	explicit ReciprocalCurve(const double & a = 0.0, const double & b = 0.0, const double & c = 0.0, const double & out_min = 0.0, const double & out_max = 1.0);
	// 返回曲线的各种参数
	const double & A();
	const double & B();
	const double & C();
	const double & OutMin();
	const double & OutMax();
	// 设置曲线的形状
	void SetABC(const double & a, const double & b, const double & c);
	// 设置曲线的所属区间
	void SetOutMinMax(const double & out_min, const double & out_max);
	// 
	void Interpolate(const double & x1, const double & y1, const double & x2, const double & y2, const double & x3, const double & y3)
	{
		Assert(((x1 - x2) / (y2 - y1) - (x1 - x3) / (y3 - y1)) != 0);
		Assert((y1 - y2) != 0);

		mA = ((x1 * y1 - x2 * y2) / (y2 - y1) - (x1 * y1 - x3 * y3) / (y3 - y1)) / ((x1 - x2) / (y2 - y1) - (x1 - x3) / (y3 - y1));
		mC = (mA * (x1 - x2) - (x1 * y1 - x2 * y2)) / (y1 - y2);
		mB = (y1 - mA) * (x1 + mC);
		mB = (y2 - mA) * (x2 + mC);
		mB = (y3 - mA) * (x3 + mC);
	}

	double GetOutput(const double & x, const bool limited = true) const
	{
		double value = mA + mB / (x + mC);
		if (limited == true)
		{
			if (value < mOutMin)
			{
				value = mOutMin;
			}
			else if (value > mOutMax)
			{
				value = mOutMax;
			}
		}
		return value;
	}
	// 在日志和图像调试中输出曲线信息
	// title:	绘图标题
	// minx:	横坐标最小值
	// maxx:	横坐标最大值
	void Show(const char * title, double minx, double maxx);
private:
	// 曲线的基本信息
	double mA;
	double mB;
	double mC;
	double mOutMin;
	double mOutMax;
};

// 获得关于两个点的中垂线
// pos1, pos2:	原始点
inline Line GetCentralPerpendicularLine(const Vector &pos1, const Vector &pos2);

// 圆类
// 通过圆心坐标和半径来表示
class Circle{
public:
	// 通过圆心坐标和半径确定一个圆
	explicit Circle(const Vector & center = Vector(0.0, 0.0), const double & radius = 0.0);
	Circle(const double & center_x, const double & center_y, const double & radius);
	// 通过三点确定一个圆
	Circle(const Vector & point1, const Vector & point2, const Vector & point3);
	// 返回圆的圆心和半径
	const Vector & Center();
	const double & Radius();
	// 设置圆的圆心和半径
	void SetCenter(const Vector & center);
	void SetRadius(const double & radius);
	// 点是否在圆内
	bool IsWithin(const Vector & p, const double & buffer = FLOAT_EPS);

	// 确定某射线与该圆的交点个数
	// r:		射线
	// t1:		较近的交点与射线端点的距离
	// t2:		较远的交点与射线端点的距离
	// buffer:	控制精度
	// return:	射线与圆的交点个数
	int	Intersection(const Ray &r, double &t1, double &t2, const double & buffer = 0.0);
	// 确定某圆与该圆的交点个数
	// c:		另一个圆
	// v1:		第一个交点
	// v2:		第二个
	// buffer:	控制精度
	// return:	两圆交点个数
	int	Intersection(const Circle &c, Vector &v1, Vector &v2, const double & buffer);
private:
	// 圆的圆心和半径
	Vector mCenter;
	double mRadius;
};
