#pragma once

#include "IBefungeRunner.h"

class BefungeRunner1 : public IBefungeRunner
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

public:
	BefungeRunner1(int w, int h);
	~BefungeRunner1();


	void Run();
	void Init(std::vector<std::string> lines);
	void SetLimit(int lim);
};

