#pragma once

#define INITIAL_STACK_SIZE 1024

#define GRID(x, y) raster[y*width+x]
#define DCACHE(x, y) dirtyCache[y*width+x]
#define VCACHE(x, y) visitCache[y*width+x]

#define int_grid int64_t

class IBefungeRunner
{
public:
	virtual void Run() = 0;
	virtual void Init(std::vector<std::string> lines) = 0;
	virtual void SetLimit(int lim) = 0;
};

