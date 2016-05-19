#include "stdafx.h"
#include "BefungeRunnerInfo.h"
#include "BFRunException.h"
#include "HelperMethods.h"
#include "format.h"


BefungeRunnerInfo::BefungeRunnerInfo(int w, int h)
{
	stack.reserve(INITIAL_STACK_SIZE);

	width = w;
	height = h;

	raster = new int_grid[width * height];
	dirtyCache = new bool[width * height];
	visitCache = new bool[width * height];

	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
		{
			GRID(x, y) = ' ';
			DCACHE(x, y) = false;
			VCACHE(x, y) = false;
		}

	pcX = 0;
	pcY = 0;

	deltaX = 1;
	deltaY = 0;

	stepCounter = 0;
}


BefungeRunnerInfo::~BefungeRunnerInfo()
{
	delete[] raster;
	delete[] dirtyCache;
	delete[] visitCache;
}

void BefungeRunnerInfo::Run()
{
	infoStartTime = clock();

	if (stepLimit < 0)
	{
		while (deltaX != 0 || deltaY != 0)
		{
			RunSingle();
		}
	}
	else
	{
		while (deltaX != 0 || deltaY != 0)
		{
			RunSingle();

			if (stepCounter >= stepLimit)
				throw BFRunException("Ran out of execution steps (limit=" + std::to_string(stepLimit) + ")", pcX, pcY, RESULT_EC_LIMITEXCEEDED);
		}
	}

	OutputInfo();
}

void BefungeRunnerInfo::OutputInfo()
{
	double felapsed_ms = ((double((clock() - infoStartTime)) * 1000) / CLOCKS_PER_SEC);
	int elapsed_ms = (int)felapsed_ms;
	bool needsInt64 = (infoMinValue < std::numeric_limits<int>::min()) || (infoMinValue > std::numeric_limits<int>::max());

	std::cout << "        max_stack_size = " << infoMaxStackSize                           << std::endl;
	std::cout << "                 width = " << width                                      << std::endl;
	std::cout << "                height = " << height                                     << std::endl;
	std::cout << "                 steps = " << stepCounter                                << std::endl;
	std::cout << "           write_count = " << infoGridWriteAccess                        << std::endl;
	std::cout << "            read_count = " << infoGridReadAccess                         << std::endl;
	std::cout << "          edited_cells = " << CountDirty()                               << std::endl;
	std::cout << "        executed_cells = " << CountVisited()                             << std::endl;
	std::cout << "             value_min = " << infoMinValue                               << std::endl;
	std::cout << "             value_max = " << infoMaxValue                               << std::endl;
	std::cout << "           needs_int64 = " << (needsInt64     ? "true" : "false")        << std::endl;
	std::cout << "                random = " << (infoIsrandom   ? "true" : "false")        << std::endl;
	std::cout << "               selfmod = " << (infoSelModExec ? "true" : "false")        << std::endl;
	std::cout << "             exec_time = " << elapsed_ms                                 << std::endl;
	std::cout << "             frequency = " << FormatFrequency(stepCounter / felapsed_ms) << std::endl;
}

std::string BefungeRunnerInfo::FormatFrequency(double freq)
{
	freq *= 1000;

	if (isnan(freq)) return "NaN";
	if (isinf(freq) && freq > 0) return "INF";
	if (isinf(freq) && freq < 0) return "ERR";

	std::string pref = "";

	if (freq > 1000)
	{
		freq /= 1000;
		pref = "k";

		if (freq > 1000)
		{
			freq /= 1000;
			pref = "M";

			if (freq > 1000)
			{
				freq /= 1000;
				pref = "G";
			}
		}
	}

	return fmt::format("{0:.2f} {1}Hz", freq, pref);
}

int BefungeRunnerInfo::CountVisited()
{
	int c = 0;
	for (int i = 0; i < width*height; c += visitCache[i++]); // c++ lol
	return c;
}

int BefungeRunnerInfo::CountDirty()
{
	int c = 0;
	for (int i = 0; i < width*height; c += dirtyCache[i++]); // c++ lol
	return c;
}

void BefungeRunnerInfo::Init(std::vector<std::string> lines)
{
	for (size_t y = 0; y < lines.size(); y++)
		for (size_t x = 0; x < lines[y].length(); x++)
		{
			GRID(x, y) = lines[y][x];
		}
}

void BefungeRunnerInfo::SetLimit(int lim)
{
	stepLimit = lim;
}

void BefungeRunnerInfo::RunSingle()
{
	if (DCACHE(pcX, pcY)) infoSelModExec = true;
	VCACHE(pcX, pcY) = true;

	ExecuteCommand(GRID(pcX, pcY));

	Move();

	if (GRID(pcX, pcY) != ' ')stepCounter++;
}

void BefungeRunnerInfo::Set(int_grid x, int_grid y, int_grid chr)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return;

	GRID(x, y) = chr;

	DCACHE(x, y) = true;
	infoGridWriteAccess++;
}

int_grid BefungeRunnerInfo::Get(int_grid x, int_grid y)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return 0;

	infoGridReadAccess++;

	return GRID(x, y);
}

void BefungeRunnerInfo::Push(int_grid i)
{
	infoMaxValue = std::max(infoMaxValue, i);
	infoMinValue = std::min(infoMinValue, i);

	stack.push_back(i);

	infoMaxStackSize = std::max(infoMaxStackSize, (int)stack.size());
}

int_grid BefungeRunnerInfo::Pop()
{
	if (stack.size() == 0)
	{
		return 0;
	}

	int_grid v = stack.back();
	stack.pop_back();
	return v;
}

int_grid BefungeRunnerInfo::Peek()
{
	if (stack.size() == 0)
	{
		return 0;
	}

	return stack.back();
}

bool BefungeRunnerInfo::Pop_b()
{
	return Pop() != 0;
}

void BefungeRunnerInfo::Push_b(bool b)
{
	Push(b ? 1 : 0);
}

int_grid BefungeRunnerInfo::ReadIntFromStdIn()
{
	std::string line;

	for (;;)
	{
		std::getline(std::cin, line);

		int result;
		if (str2int(result, line) == S2IE_SUCCESS)
			return result;
	}
}

int_grid BefungeRunnerInfo::ReadCharFromStdIn()
{
	return getchar();
}

void BefungeRunnerInfo::ExecuteCommand(int_grid cmd)
{
	if (stringmode)
	{
		if (cmd == '"')
		{
			stringmode = false;
			return;
		}
		else
		{
			Push(cmd);
			return;
		}
	}

	int_grid t1;
	int_grid t2;
	int_grid t3;

	switch (cmd)
	{
	case ' ':
		// NOP
		break;
	case '+':
		Push(Pop() + Pop());
		break;
	case '-':
		t1 = Pop();
		Push(Pop() - t1);
		break;
	case '*':
		t1 = Pop();
		Push(Pop() * t1);
		break;
	case '/':
		t1 = Pop();
		Push(Pop() / t1);
		break;
	case '%':
		t1 = Pop();
		Push(Pop() % t1);
		break;
	case '!':
		Push_b(!Pop_b());
		break;
	case '`':
		t1 = Pop();
		Push_b(Pop() > t1);
		break;
	case '>':
		deltaX = 1;
		deltaY = 0;
		break;
	case '<':
		deltaX = -1;
		deltaY = 0;
		break;
	case '^':
		deltaX = 0;
		deltaY = -1;
		break;
	case 'v':
		deltaX = 0;
		deltaY = 1;
		break;
	case '?':
		infoIsrandom = true;
		switch (rand() % 4)
		{
		case 0:
			deltaX = 1;
			deltaY = 0;
			break;
		case 1:
			deltaX = -1;
			deltaY = 0;
			break;
		case 2:
			deltaX = 0;
			deltaY = -1;
			break;
		case 3:
			deltaX = 0;
			deltaY = 1;
			break;
		}
		break;
	case '_':
		deltaX = Pop_b() ? -1 : +1;
		deltaY = 0;
		break;
	case '|':
		deltaX = 0;
		deltaY = Pop_b() ? -1 : +1;
		break;
	case '"':
		stringmode = true;
		break;
	case ':':
		Push(Peek());
		break;
	case '\\':
		t1 = Pop();
		t3 = Pop();
		Push(t1);
		Push(t3);
		break;
	case '$':
		Pop();
		break;
	case '.':
		Pop();
		break;
	case ',':
		Pop();
		break;
	case '#':
		Move();
		break;
	case 'p':
		t1 = Pop();
		t2 = Pop();
		t3 = Pop();

		Set(t2, t1, t3);
		break;
	case 'g':
		t1 = Pop();
		t2 = Pop();

		Push(Get(t2, t1));
		break;
	case '&':
		Push(ReadIntFromStdIn());
		break;
	case '~':
		Push(ReadCharFromStdIn());
		break;
	case '@':
		deltaX = 0;
		deltaY = 0;
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		Push(cmd - '0');
		break;
	default:
		Push(0);
			break;
	}
}

void BefungeRunnerInfo::Move()
{
	pcX += deltaX;
	pcY += deltaY;

	if (pcX < 0) pcX += width;
	if (pcY < 0) pcY += height;

	if (pcX >= width) pcX -= width;
	if (pcY >= height) pcY -= height;
}