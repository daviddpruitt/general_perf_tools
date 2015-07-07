#define f(x) f##x 
#define pdef(n) double f(n)
#define paramDefs pdef(1),pdef(2),pdef(3),pdef(4),pdef(5),pdef(6),pdef(7),pdef(8),pdef(9),pdef(10),pdef(11),pdef(12),pdef(13),pdef(14),pdef(15)
double do10800Fn(paramDefs);

long long 
doManyExternal(long long numIters)
{
	long long count = 0;
	double x = 0;
	while (numIters--) {
			x +=do10800Fn(.0000001, 
					.0000002, 
					.0000003, 
					.0000004,
					.0000005,
					.0000006,
					.0000007,
					.0000008,
					.0000009,
					.00000010,
					.00000011,
					.00000012,
					.00000013,
					.00000014,
					.00000015);
#if (OP == 1) || (OP == 4)
		count += 21600;
#else
		count += 10800;
#endif
	}				
	//TableSize = TableSize;
	//Table[0] = Table[0];
	//Table = Table;
	if (x == 0.0)
		count += 1;
	return count;
}
