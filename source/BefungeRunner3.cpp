#include "stdafx.h"
#include "BefungeRunner3.h"
#include "BFRunException.h"
#include "HelperMethods.h"


BefungeRunner3::BefungeRunner3(int w, int h)
{
	stack.reserve(INITIAL_STACK_SIZE);

	width = w;
	height = h;

	raster = new int_grid[width * height];
	dirtyCache = new bool[width * height];

	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
		{
			GRID(x, y) = ' ';
			DCACHE(x, y) = false;
		}

	pcX = 0;
	pcY = 0;

	deltaX = 1;
	deltaY = 0;

	stepCounter = 0;
}


BefungeRunner3::~BefungeRunner3()
{
	delete[] raster;
	delete[] dirtyCache;
}

void BefungeRunner3::Run()
{
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
}

void BefungeRunner3::Init(std::vector<std::string> lines)
{
	for (size_t y = 0; y < lines.size(); y++)
		for (size_t x = 0; x < lines[y].length(); x++)
		{
			GRID(x, y) = lines[y][x];
		}
}

void BefungeRunner3::SetLimit(int lim)
{
	stepLimit = lim;
}

void BefungeRunner3::RunSingle()
{
	if (DCACHE(pcX, pcY))
		throw BFRunException("Trying to execute the modified cell", pcX, pcY, RESULT_EC_SELFMODIFICATION);

	ExecuteCommand(GRID(pcX, pcY));

	Move();

	stepCounter++;
}

void BefungeRunner3::Set(int_grid x, int_grid y, int_grid chr)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		throw BFRunException("Modification Out Of Raster", pcX, pcY, RESULT_EC_INVALIDGRIDACC);

	DCACHE(pcX, pcY) = true;

	GRID(x, y) = chr;
}

int_grid BefungeRunner3::Get(int_grid x, int_grid y)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		throw BFRunException("Modification Out Of Raster", pcX, pcY, RESULT_EC_INVALIDGRIDACC);

	return GRID(x, y);
}

void BefungeRunner3::Push(int_grid i)
{
	stack.push_back(i);
}

int_grid BefungeRunner3::Pop()
{
	if (stack.size() == 0)
		throw BFRunException("Popped an empty stack", pcX, pcY, RESULT_EC_INVALIDSTACKACC);

	int_grid v = stack.back();
	stack.pop_back();
	return v;
}

int_grid BefungeRunner3::Peek()
{
	if (stack.size() == 0)
		throw BFRunException("Popped an empty stack", pcX, pcY, RESULT_EC_INVALIDSTACKACC);

	return stack.back();
}

bool BefungeRunner3::Pop_b()
{
	return Pop() != 0;
}

void BefungeRunner3::Push_b(bool b)
{
	Push(b ? 1 : 0);
}

int_grid BefungeRunner3::ReadIntFromStdIn()
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

int_grid BefungeRunner3::ReadCharFromStdIn()
{
	return getchar();
}

void BefungeRunner3::ExecuteCommand(int_grid cmd)
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
		std::cout << (int_grid)Pop();
		break;
	case ',':
		std::cout << (char)Pop();
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
		throw BFRunException("Unknown Command: #" + std::to_string(cmd), pcX, pcY, RESULT_EC_INVALIDCOMMAND);
	}
}

void BefungeRunner3::Move()
{
	pcX += deltaX;
	pcY += deltaY;

	if (pcX < 0 || pcY < 0 || pcX >= width || pcY >= height)
		throw BFRunException("Moved Out Of Raster", pcX, pcY, RESULT_EC_PCWRAPPING);
}