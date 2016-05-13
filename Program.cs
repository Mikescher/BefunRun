using BefunRun.Runner;
using System;
using System.IO;

namespace BefunRun
{
	class Program
	{
		private readonly CommandLineArguments cmda;

		static int Main(string[] args)
		{
			if (args.Length > 0 && File.Exists(args[0]))
			{
				args[0] = "-file=" + args[0];
			}

			return new Program(new CommandLineArguments(args, false)).Run();
		}

		private Program(CommandLineArguments a)
		{
			cmda = a;
		}

		private int Run()
		{
			if (cmda.IsEmpty() || cmda.Contains("help"))
			{
				ShowHelp();
				return 0;
			}

			string file = cmda.GetStringDefault("file", null);
			if (string.IsNullOrWhiteSpace(file) || !File.Exists(file))
			{
				Console.Error.WriteLine("Please supply a accessible befunge-93 sourcecode file");
				return 1;
			}

			int errlevel = cmda.GetIntDefaultRange("errorlevel", 0, 0, 4);
			int limit = cmda.GetIntDefault("limit", -1);

			try
			{
				string befcode = File.ReadAllText(file);

				GetRunner(befcode, errlevel, limit).Run();

				return 0;
			}
			catch (BFRunException e)
			{
				Console.Out.WriteLine();
				Console.Out.WriteLine();

				Console.Error.WriteLine("Execution Error: " + e.Message);
				return 2;
			}
			catch (Exception e)
			{
				Console.Out.WriteLine();
				Console.Out.WriteLine();

				Console.Error.WriteLine("Internal Error:");
				Console.Error.WriteLine();
				Console.Error.WriteLine(e.ToString());
				return 3;
			}
		}

		private IBefungeRunner GetRunner(string code, int errlevel, int limit)
		{
			switch (errlevel)
			{
				case 0: return new BefungeRunner0(code, limit);
				case 1: return new BefungeRunner1(code, limit);
				case 2: return new BefungeRunner2(code, limit);
				case 3: return new BefungeRunner3(code, limit);
			}

			return null;
		}

		private void ShowHelp()
		{
			Console.WriteLine();
			Console.WriteLine("BefunRun file [parameter] ");
			Console.WriteLine();
			Console.WriteLine("########## Parameter ##########");
			Console.WriteLine();
			Console.WriteLine("file              : The input sourcecode file in *.tf format");
			Console.WriteLine();
			Console.WriteLine("errorlevel        : Set the errorlevel");
			Console.WriteLine("   - errorlevel=0 : Never abort");
			Console.WriteLine("   - errorlevel=1 : Abort when:");
			Console.WriteLine("                     - reading from empty stack");
			Console.WriteLine("                     - reading/writing invalid coordinates");
			Console.WriteLine("                     - executing an invalid command character");
			Console.WriteLine("   - errorlevel=2 : Abort on previous conditions and when:");
			Console.WriteLine("                     - moving PC over the program edge");
			Console.WriteLine("   - errorlevel=3 : Abort on previous conditions and when:");
			Console.WriteLine("                     - executing a previously modified cell");
			Console.WriteLine();
			Console.WriteLine("limit             : Abort after n steps");
		}
	}
}
