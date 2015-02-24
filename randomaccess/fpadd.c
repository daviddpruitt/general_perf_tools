#include "randomaccess.h"
#include "utility.h"

#define f(x) f##x 

#define pdef(n) double f(n)
#define paramDefs pdef(1),pdef(2),pdef(3),pdef(4),pdef(5),pdef(6),pdef(7),pdef(8),pdef(9),pdef(10),pdef(11),pdef(12),pdef(13),pdef(14),pdef(15)
//paramDefs

#define m(x) f(x) += f(x)
#define fifteen m(1);m(2);m(3);m(4);m(5);m(6);m(7);m(8);m(9);m(10);m(11);m(12);m(13);m(14);m(15);
#define thirty fifteen fifteen
#define ninety thirty thirty thirty
#define two_seventy ninety ninety ninety
#define five_forty two_seventy two_seventy
#define two_thou_one_sixty five_forty five_forty five_forty five_forty
#define ten_eight_hundred two_thou_one_sixty  two_thou_one_sixty  two_thou_one_sixty  two_thou_one_sixty  two_thou_one_sixty 

u64Int numOpsTot = 1080000;

inline double
foo(paramDefs)
{
	//ninety;
	//five_forty;
	//two_thou_one_sixty;
	ten_eight_hundred;
	return f1+f2+f3+f4+f5+f6+f7+f8+f9+f10+f11+f12+f13+f14+f15;
}

double
instrOnly(u64Int TableSize, u64Int *Table)
{
	double x = 0;
	unsigned i = 0;
	for (i = 0; i < 100; i++) {
			x +=foo(1.000001, 
					2.000002, 
					3.000003, 
					4.000004,
					5.000005,
					6.000006,
					7.000007,
					8.000008,
					9.000009,
					10.0000010,
					11.0000010,
					12.0000012,
					13.0000013,
					14.0000014,
					15.0000015);
	}				
	TableSize = TableSize;
	Table[0] = Table[0];
	Table = Table;
	return x;
}

