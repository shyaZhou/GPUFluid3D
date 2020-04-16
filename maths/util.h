#pragma once
namespace sparky {
	namespace maths {
		class Util
		{
		public:
			static int ClosestPowerOfTwo(int t)
			{
			/*	int temp = ~t + 1;
				return t + temp;*/
				return t;
			}
		};
	}
}