﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace BefunRun.Runner
{
	/// <summary>
	/// Show debug information
	/// </summary>
	class BefungeRunnerInfo : IBefungeRunner
	{
		private static readonly Random Rand = new Random();

		private readonly int rasterWidth;
		private readonly int rasterHeight;

		private readonly long[,] raster;
		private Vector pc;
		private Vector delta;
		private bool stringmode;

		private long infoMaxValue = 0;
		private long infoMinValue = 0;
		private readonly bool[,] infoDirtyCache;
		private readonly bool[,] infoVisitedCache;
		private long infoMaxStackSize = 0;
		private long infoGridReadAccess = 0;
		private long infoGridWriteAccess = 0;
		private bool infoIsrandom = false;
		private bool infoSelModExec = false;

		private readonly Stack<long> stack;

		private int stepCount;

		private readonly int limit;

		public BefungeRunnerInfo(string s, int lim = -1)
		{
			string[] lines = Regex.Split(s, @"\r?\n");

			rasterHeight = lines.Length;
			rasterWidth = lines.Max(p => p.Length);

			raster = new long[rasterWidth, rasterHeight];
			infoDirtyCache = new bool[rasterWidth, rasterHeight];
			infoVisitedCache = new bool[rasterWidth, rasterHeight];

			for (int x = 0; x < rasterWidth; x++)
				for (int y = 0; y < rasterHeight; y++)
					raster[x, y] = ' ';

			pc = new Vector(0, 0);
			delta = new Vector(1, 0);
			stack = new Stack<long>();
			stepCount = 0;
			stringmode = false;
			limit = lim;

			for (int y = 0; y < lines.Length; y++)
				for (int x = 0; x < lines[y].Length; x++)
					raster[x, y] = lines[y][x];
		}

		public void Run()
		{
			if (limit < 0)
			{
				while (!delta.isZero())
				{
					RunSingle();
				}

				OutputInfo();
			}
			else
			{
				while (!delta.isZero())
				{
					RunSingle();

					if (stepCount >= limit)
						throw new BFRunException($"Ran out of execution steps (limit={limit})", pc);
				}
			}
		}

		private void OutputInfo()
		{
			bool needsInt64 = (infoMinValue < Int32.MinValue) || (infoMinValue > Int32.MaxValue);
			
			Console.WriteLine("        max_stack_size = " + infoMaxStackSize);
			Console.WriteLine("                 width = " + rasterWidth);
			Console.WriteLine("                height = " + rasterHeight);
			Console.WriteLine("               runtime = " + stepCount);
			Console.WriteLine("           write_count = " + infoGridWriteAccess);
			Console.WriteLine("            read_count = " + infoGridReadAccess);
			Console.WriteLine("    edited_cells_count = " + Count2D(infoDirtyCache));
			Console.WriteLine("  executed_cells_count = " + Count2D(infoVisitedCache));
			Console.WriteLine("             value_min = " + infoMinValue);
			Console.WriteLine("             value_max = " + infoMaxValue);
			Console.WriteLine("           needs_int64 = " + (needsInt64 ? "true" : "false"));
			Console.WriteLine("                random = " + (infoIsrandom ? "true" : "false"));
			Console.WriteLine("      selfmodification = " + (infoSelModExec ? "true" : "false"));
		}

		private int Count2D(bool[,] a)
		{
			int c = 0;

			for (int x = 0; x < a.GetLength(0); x++)
			{
				for (int y = 0; y < a.GetLength(1); y++)
				{
					if (a[x, y]) c++;
				}
			}

			return c;
		}

		private void RunSingle()
		{
			if (infoDirtyCache[pc.X, pc.Y]) infoSelModExec = true;
			infoVisitedCache[pc.X, pc.Y] = true;
			ExecutCmd(raster[pc.X, pc.Y]);

			Move();

			stepCount++;
		}

		private void Set(long x, long y, long chr)
		{
			if (x < 0 || y < 0 || x >= rasterWidth || y >= rasterHeight)
				return;

			infoDirtyCache[x, y] = true;
			infoGridWriteAccess++;

		raster[x, y] = chr;
		}

		private long Get(long x, long y)
		{
			if (x < 0 || y < 0 || x >= rasterWidth || y >= rasterHeight)
				return 0;

			infoGridReadAccess++;

			return raster[x, y];
		}

		private void Push(long i)
		{
			infoMaxValue = Math.Max(infoMaxValue, i);
			infoMinValue = Math.Min(infoMinValue, i);

			stack.Push(i);

			infoMaxStackSize = Math.Max(infoMaxStackSize, stack.Count);
		}

		private long Pop()
		{
			if (stack.Count == 0)
			{
				return 0;
			}

			return stack.Pop();
		}

		private long Peek()
		{
			if (stack.Count == 0)
			{
				return 0;
			}

			return stack.Peek();
		}

		private bool Pop_b()
		{
			return Pop() != 0;
		}

		private void Push(bool b)
		{
			Push(b ? 1 : 0);
		}

		private void ExecutCmd(long cmd)
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

			long t1;
			long t2;
			long t3;

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
					Push(!Pop_b());
					break;
				case '`':
					t1 = Pop();
					Push(Pop() > t1);
					break;
				case '>':
					delta.set(1, 0);
					break;
				case '<':
					delta.set(-1, 0);
					break;
				case '^':
					delta.set(0, -1);
					break;
				case 'v':
					delta.set(0, 1);
					break;
				case '?':
					infoIsrandom = true;
					delta.set(Rand.Next(4));
					break;
				case '_':
					delta.set(Pop_b() ? 2 : 0);
					break;
				case '|':
					delta.set(Pop_b() ? 3 : 1);
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
					//Console.Write((int)Pop());
					break;
				case ',':
					//Console.Write((char)Pop());
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
					delta.set(0, 0);
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
					throw new BFRunException("Unknown Command:" + cmd + "(" + (char)cmd + ")", pc);
			}
		}

		private int ReadIntFromStdIn()
		{
			string x = "~";

			int result;
			while (int.TryParse(x, out result))
			{
				x = Console.ReadLine();
			}
			return result;
		}

		private int ReadCharFromStdIn()
		{
			return Console.ReadKey().KeyChar;
		}

		private void Move()
		{
			pc.X += delta.X;
			pc.Y += delta.Y;

			if (pc.X < 0) pc.X += rasterWidth;
			if (pc.Y < 0) pc.Y += rasterHeight;

			if (pc.X >= rasterWidth) pc.X -= rasterWidth;
			if (pc.Y >= rasterHeight) pc.Y -= rasterHeight;
		}
	}
}
