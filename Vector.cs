namespace BefunRun
{
	public struct Vector
	{
		public int X;
		public int Y;

		public Vector(int xx, int yy) { X = xx; Y = yy; }

		public void set(int xx, int yy) { X = xx; Y = yy; }

		public void set(int d)
		{
			switch (d)
			{
				case 0:
					set(+1, 00);
					break;
				case 1:
					set(00, +1);
					break;
				case 2:
					set(-1, 00);
					break;
				case 3:
					set(00, -1);
					break;
			}
		}

		public bool isZero()
		{
			return X == 0 && Y == 0;
		}
	}
}
