// 参考gnuplot_i.[ch]

// 该类通过调用GNUPlot来画图（用于调试）
class Plotter{
	Plotter();
public:
	virtual ~Plotter();
	// 该类唯一的实例对象
	static Plotter & instance();
	// 设置X轴信息
	// label:	X轴显示的文字标记
	void SetXLabel(char * label);
	// 设置Y轴信息
	// label:	Y轴显示的文字标记
	void SetYLabel(char * label);
	// 重置一个GNUPlot会话
	void Reset();
	// 执行一个GNUPlot命令
	// cmd:	要执行的指令，语法与printf相同
	// gnuplot_cmd(g, "plot %d*x", 23.0);
	// gnuplot_cmd(g, "plot %g * cos(%g * x)", 32.0, -3.0);
	// 注意！本函数无法判断语法错误！
	void GnuplotExecute(const char * cmd, ...);
	// 将图像保存为PNG格式
	// file_name:	目标文件名
	void PlotToFile(char * file_name);
	// 画到屏幕上
	void PlotToDisplay();
private:
	void Init();
	void Close();
private:
	bool mIsDisplayOk;	// 是否显示完毕
	bool mIsGnuplotOk;	// 没找到引用，意义不明
	FILE *mpGnupolot;	// 往这里写入东西相当于给GNUPlot传送命令
private:
	// 最大命令长度
	static const int GP_CMD_SIZE = 2048;
};
