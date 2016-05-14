#pragma once

#include "IBefungeRunner.h"

class BefungeRunnerInfo : public IBefungeRunner
{

private:
	int width;
	int height;

	int_grid* raster;
	std::vector<int_grid> stack;
	int pcX;
	int pcY;
	int deltaX;
	int deltaY;
	bool stringmode;

	int stepCounter;
	int stepLimit;


	// #### INFO ####

	bool* dirtyCache;
	bool* visitCache;
	clock_t infoStartTime;
	int_grid infoMaxValue = 0;
	int_grid infoMinValue = 0;
	int infoMaxStackSize = 0;
	int infoGridReadAccess = 0;
	int infoGridWriteAccess = 0;
	bool infoIsrandom = false;
	bool infoSelModExec = false;

private:
	void RunSingle();
	void ExecuteCommand(int_grid cmd);
	void Move();
	void Set(int_grid x, int_grid y, int_grid chr);
	int_grid Get(int_grid x, int_grid y);
	void Push(int_grid i);
	int_grid Pop();
	int_grid Peek();
	bool Pop_b();
	void Push_b(bool b);
	int_grid ReadIntFromStdIn();
	int_grid ReadCharFromStdIn();
	void OutputInfo();
	int CountVisited();
	int CountDirty();

public:
	BefungeRunnerInfo(int w, int h);
	~BefungeRunnerInfo();


	void Run();
	void Init(std::vector<std::string> lines);
	void SetLimit(int lim);
};

