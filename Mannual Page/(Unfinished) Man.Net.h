class Net{
	int mLayers;                   ///number of layers(including input layer)
	int  *mUnits;                  ///the number of units of each layer

	double ***mWeight;               ///weight of each conjuction between units
	double ***mDeltaWeight;          ///delta weight of each conjuction between units
	double **mDelta;                 ///delta value of each unit
	double **mOutput;                ///output value of each unit
	double *mDesire;                 ///desired output value of output layer

	double mEta;
	double mAlpha;
	double mDesiredError;
	int mMaxEpochs;

	char *mLogName;
public:
	Net(int Layers = 0, int *Units = 0);
	Net(const char *fname);
	~Net();

	void Construct(int layers, int *units);
	void Construct(const char *fname);
	void Destroy();
	void Save(const char *fname);

	void Run(double *input, double *output);	///calc outout of input and return sum of square error
	double Train(double *input, double *desire); ///train network with sample <Input, Desire>
	void TrainOnFile(const char *fname);
	double TestOnFile(const char *fname);
	double Error();

	void SetLearningRate(double rate);
	void SetAlpha(double a);
	void SetDesiredError(double d);
	void SetLogName(const char *);
	void SetMaxEpochs(int );
private:
	Net(const Net &);
	void Memaloc();
	void SetDefaultValue();

	void SetInput(double *input);
	void SetDesire(double *desire);
	void InitWeight();
	void FeedForward();
	void BackProp();
	void UpdateWeight();

	double sigmoid(double s);
	double small_rand();
};
