using System;

namespace BefunRun.Runner
{
	public class BFRunException : Exception
	{
		public BFRunException(string msg, Vector pc) : base($"{msg} at [{pc.X},{pc.Y}]") { }
	}
}
