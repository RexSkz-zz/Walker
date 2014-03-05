#include "Utilities.h"

// 为了防止多线程程序出现一些时间差的问题，例如
// 要等到接收到服务器信息之后才能更新世界模型而
// 特意写了一个类用于控制线程行为。这个类只有一
// 个行为：发送信号或等待别的信号

// 由于Windows和Linux中对线程的实现方式不同，所以
// 本文件针对两个系统写了具有相同接口的不同的两个
// 类，通过【#ifdef WIN32】来区分究竟使用哪个类
// 为了方便，故只解释接口用法，不具体细分系统差异
// 两个类的具体差异为私有变量和接口实现方式

// 等待一段时间（子过程）
// ms:  等待的时间
inline void WaitFor(int ms);

// 处理普通线程的类，可以简单地休眠和唤醒
class ThreadCondition{
public:
    // 构造函数和析构函数
    ThreadCondition();
    ~ThreadCondition();
    // 阻塞和唤醒函数
    bool Wait(int ms);
    void Set();
};

// 处理互斥线程的类，当某个实例对象的函数被执行时，与
// 之互斥的类要被手动加锁，防止多个对象同时处理同一变
// 量而导致的严重错误
class ThreadMutex{
public:
    // 构造函数和析构函数
    ThreadMutex();
    ~ThreadMutex();
    // 加锁和解锁函数
    void Lock();
    void UnLock();
};

// 线程的基类，一个实例对象是一个线程，里面包含了上文中
// 两个种类的线程
class Thread{
    // 拷贝构造参数
	Thread(const Thread &);
    // 对线程直接赋值
	const Thread &operator=(const Thread &);
public:
	Thread();
    // 构造一个线程
	void Start();
    // 将线程加入程序进程
	void Join();
private:
    // 该类的入口（没有在别的地方调用过，意义不明）
	static void *Spawner(void *v);
    // 开始主循环，在子类中被重写（在Spawner中被执行）
	virtual void StartRoutine() = 0;
};
