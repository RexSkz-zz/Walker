#include "Types.h"

class UDPSocket{
	// 构造函数，初始化服务器状态为未就绪
	UDPSocket();
public:
	// 析构函数（可以不用管）
	~UDPSocket();
	// UDPSocket类的入口，返回一个UDPSocket对象的引用
	// 全局只有一个UDPSocket对象：在instance中定义的static对象
	// 通过调用UDPSocket::instance().函数名()来执行对应函数
	// 函数可选项有：Initial、Receive、Send
	static UDPSocket & instance();
	// 初始化UDP连接
	// host:	主机地址
	// port:	端口号
	void Initial(const char *host, int port);
	// 从服务器接收信息
	// msg:    储存消息的变量
	// return: 若正确接收则返回接收到的字节数，否则返回-1
	int Receive(char *msg);
	// 向服务器发送信息
	// 注意！若服务器未就绪，则该条信息被视为已发送
	// msg:    待发送的信息
	// return: 若正确发送或服务器未就绪则返回信息字节数，否则返回-1
	int Send(const char *msg);
private:
	// 服务器是否准备就绪（底层变量，可以不用管）
	bool mIsInitialOK;
	// 服务器信息（底层变量，可以不用管）
	sockaddr_in mAddress;
	// 通过host和port打开的Socket（底层变量，可以不用管）
#ifdef WIN32
	SOCKET mSockfd;
#else
	int mSockfd;
#endif
};
