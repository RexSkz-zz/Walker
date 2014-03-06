﻿#include "Observer.h"
#include "PlayerParam.h"

// 动态调试中记录消息的类型
enum MessageType{
	MT_Null,
	MT_Parse,
	MT_Run,
	MT_Send
};

class DynamicDebug
{
	DynamicDebug();

	struct MessageFileHead
	{
		Time mMaxCycle;					// 文件中记录的最大周期
		long long mIndexTableSize;		// 索引表大小
		long long mIndexTableOffset;	// 索引表位置
		long long mParserTableSize;
		long long mParserTableOffset;
		long long mDecisionTableSize;
		long long mDecisionTableOffset;
		long long mCommandSendTableSize;
		long long mCommandSendTableOffset;
		MessageFileHead();
	};

	struct MessageIndexTableUnit
	{
		Time	mServerTime; // 数据对应的周期
		long long	mDataSize; // 数据长度
		long long	mDataOffset; // 数据的存储位置
		long long	mTimeOffset; // 时间表的存储位置
		/*int	mDataSize; // 数据长度
		int	mDataOffset; // 数据的存储位置
		int	mTimeOffset; // 时间表的存储位置*/
	};

	struct Message
	{
		MessageType mType;
		string mString;

		Message(MessageType type, const char *msg): mType(type), mString(msg)
		{

		}
	};

public:
	/**
	 * 构造函数和析构函数
	 */
	~DynamicDebug();

	/**
	 * 创建实例，初始化函数
	 */
	static DynamicDebug & instance();
	void Initial(Observer *pObserver);

	/**
	 * 下面函数是正常比赛要记录server信息时用到的接口
	 * AddMessage()这个函数一定要放到各个线程的入口函数的前面调用，否则动态调试得到系统时间时会出错
	 * 比如，要在Parser::Parse()，Client::Decision()和CommandSend::Run()之前调用
	 */
	void AddMessage(const char *msg, MessageType msg_type);
	void AddTimeParser(timeval &time);
	void AddTimeDecision(timeval &time);
	void AddTimeCommandSend(timeval &time);

	/**
	 * 下面函数是动态调试时用到的接口
	 */
	MessageType Run(char *msg);
	MessageType GetMessage(char *msg);
	bool FindCycle(int cycle);
	timeval GetTimeParser();
	timeval GetTimeDecision();
	timeval GetTimeCommandSend();

private:
	void Flush();

private:
	Observer	*mpObserver; // WorldModel的指针
	bool		mInitialOK; // 是否已经初始化完毕

	// 文件头部信息，记录4种信息的量
	MessageFileHead mFileHead;

	// 下面4个表用来记录和保存4种信息
	vector<MessageIndexTableUnit>	mIndexTable;
	vector<timeval>				mParserTimeTable;
	vector<timeval>				mDecisionTimeTable;
	vector<timeval>				mCommandSendTimeTable;

	vector<Message>				mMessageTable;

	// 下面4个指针用来在读写文件时使用
	MessageIndexTableUnit	*mpIndex;
	timeval				*mpParserTime;
	timeval				*mpDecisionTime;
	timeval				*mpCommandSendTime;

	// 当前读取的单元
	MessageIndexTableUnit	*mpCurrentIndex;

	// 用于文件操作
	ThreadMutex	mFileMutex;
	FILE			*mpFile;
	ifstream	*mpFileStream;
	streambuf	*mpStreamBuffer;

	bool mRunning; // 是否正在运行
	bool mShowMessage; // 是否输出信息
	Time mRuntoCycle; // 要Run到的周期
};

#endif

