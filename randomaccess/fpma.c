#include "randomaccess.h"
#include "utility.h"

#define f(x) f##x 

#define pdef(n) double f(n)
#define paramDefs pdef(1),pdef(2),pdef(3),pdef(4),pdef(5),pdef(6),pdef(7),pdef(8),pdef(9),pdef(10),pdef(11),pdef(12),pdef(13),pdef(14),pdef(15)
//paramDefs

#define m(x) f(x) += f(x) * f(x)
#define do15  m(1);m(2);m(3);m(4);m(5);m(6);m(7);m(8);m(9);m(10);m(11);m(12);m(13);m(14);m(15);
#define do30 do15 do15 do15 
#define do90 do30 do30 do30
#define do270 do90 do90 do90
#define do540 do270 do270 
#define do2160 do540 do540 do540 do540 
#define do10800 do2160 do2160 do2160 do2160 do2160 

u64Int numOpsTot = 1080000;

double
do10800Fn(paramDefs)
{
	//ninety;
	//five_forty;
	//two_thou_one_sixty;
	do10800;
	return f1+f2+f3+f4+f5+f6+f7+f8+f9+f10+f11+f12+f13+f14+f15;
}

long
instrOnly(u64Int TableSize, u64Int *Table)
{
	long count = 0;
	double x = 0;
	unsigned i = 0;
	for (i = 0; i < 100; i++) {
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
		count += 10800;
	}				
	//TableSize = TableSize;
	//Table[0] = Table[0];
	//Table = Table;
	if (x == 0.0 && Table[0] == 0)
		count += 1;
	return count;
}

