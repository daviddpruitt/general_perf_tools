#include "randomaccess.h"
#include "utility.h"

#define OP ^=
#define SWAP i OP j;j OP i; i OP j;
#define INC i++;j++;
#define	ONE	SWAP

#define	FIVE	ONE ONE ONE ONE ONE
#define	TEN	FIVE FIVE
#define	FIFTY	TEN TEN TEN TEN TEN
#define	HUNDRED	FIFTY FIFTY
#define FIVEHUNDRED HUNDRED HUNDRED HUNDRED HUNDRED HUNDRED
#define THOUSAND FIVEHUNDRED FIVEHUNDRED
#define INSTRUNITS (1000 * 1000)

static u64Int dummy = 0;
u64Int numOpsTot = 3000000;

void
instrOnly(u64Int TableSize, u64Int *Table)
{
		TableSize = TableSize;
	Table = Table;
	register volatile u64Int i,j,x;
			
	
	i = -1;
	j = 0;
	x = 0;
	
	for (x = 0; x < 1000; x++) {
		THOUSAND;
	}
	dummy += i;
	
}
