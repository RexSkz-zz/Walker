#include "Utilities.h"

// 所有用到的的参数类型标识符（数据类型）
enum ParamType {
    V_NONE,
    V_INT,      // 整型
    V_DOUBLE,   // 实数
    V_STRING,   // 字符串，只能是string
    V_ONOFF     // 开关
};

// 从一个系统自带类型的变量到一个自定义变量类型标识符的映射（可以理解为子过程）
// 例如ParamTypes<int>.TYPE就是自定义的V_INT
// 例如ParamTypes<bool>.TYPE就是自定义的V_ONOFF
// 由于只是一个映射，所以成员变量用的是static类型
template <typename T> struct ParamTypes { static const ParamType TYPE = V_NONE; };
template <> struct ParamTypes <int> { static const ParamType TYPE = V_INT; };
template <> struct ParamTypes <double> { static const ParamType TYPE = V_DOUBLE; };
template <> struct ParamTypes <string> { static const ParamType TYPE = V_STRING;};
template <> struct ParamTypes <bool> { static const ParamType TYPE = V_ONOFF; };

// 存储参数的类（用于ParamEngine的内部）
struct Param {
    string name;    // 参数对象名称
    void *ptr;      // 参数内容的指针
    ParamType type; // 参数类型
    // 构造函数
    // str: 参数对象名称
    // p:   参数内容的指针
    // t:   参数类型
    Param(const char *str, void *p, ParamType t);
};

// 获取并存储参数的基类，内部数据结构为哈希表（这是本文件的主体）
// 从命令行/服务器消息/配置文件读取信息，转换为参数并存储
class ParamEngine {
public:
    // 参数对象列表的迭代器，外界通过它来获取对象存储的数据
    typedef list<Param>::iterator ParamPtr;
private:
    // ParamList:   相同哈希值的参数存储在同一位置（解决碰撞）
    // HASH_SIZE:   哈希表的大小
    // mParamLists: 哈希表主体
    typedef list<Param> ParamList;
    enum { HASH_SIZE = 512 };
    Array<ParamList, HASH_SIZE> mParamLists;
private:
    // 计算参数对象名称的哈希值（子过程）
    // str:     参数对象名称
    // return:  若正确计算则返回哈希值，否则返回-1
    int hash(const char *str);
    // 判断两个字符串是否相等（子过程）
    // s1:      第一个字符串
    // b:       第二个字符串
    // return:  若两字符串相等则为true，否则为false
    bool str_equal(const char *s1, const char *b);
public:
    // 构造函数和析构函数，在子类中被重写
    ParamEngine() {}
    virtual ~ParamEngine() {}
    // 向哈希表中添加元素
    // name:    参数对象名称
    // address: 参数内容的指针
    // value:   参数类型
    template <typename T> void AddParam(const char *name, T *address, T value);
    // 将特定名称的参数类型修改成指定类型
    // name:    参数对象名称
    // value:   要修改的类型
    // return:  若修改成功则为true，否则为false
    template <typename T> bool SetParam(const char *name, T value);
    // 添加参数的函数，在子类中被重写
    virtual void AddParams() = 0;
    // 依据字符串修改特定名称的参数的值
    // name:    参数名称
    // buffer:  依据的字符串
    // return:  若获取成功则为true，否则为false
    bool SetParamFromString(const char *name, const char *buffer);
    // 获取特定名称的参数
    // name:    参数对象名称
    // it:      返回参数对象的引用
    // return:  若对象存在则为true，否则为false
    bool GetParam(const char *name, ParamPtr & it);
    // 输出特定名称的参数到指定流（用于调试）（两个重载）
    // name:    参数对象名称
    // os:      指定的输出流
    // return:  若对象存在则为true，否则为false
    bool PrintParam(const char *name, ostream &os);
    bool PrintParam(const char *name);
    // 向指定的位置输出当前所有参数（用于调试）（两个重载）
    // os:  输出参数信息的位置
    void DumpParam(ostream &os);
    void DumpParam();
    // 将参数储存到文件（用于调试）
    // file_name:   目标文件名
    // return:      若储存成功则为true，否则为false
    bool SaveToConfigFile(const char *file_name);
    // 从命令行提取参数信息（命令行格式：-name value）
    // argc:    参数个数
    // argv:    参数字符串数组列表
    void ParseFromCmdLine(int argc, char **argv);
    // 从服务器消息提取参数信息（字符串格式：(name value)）
    // line:    从服务器获取的消息
    void ParseFromServerMsg(const char *line);
    // 从配置文件提取参数信息（语句格式：name = value）
    // file_name:   配置文件名
    // return:      若读取成功则为true，否则为false
    bool ParseFromConfigFile(const char *file_name);
};
