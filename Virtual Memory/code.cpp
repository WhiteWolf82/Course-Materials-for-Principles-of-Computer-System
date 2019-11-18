#include<iostream>
#include<fstream>
#include<string>
#include<vector>

#define INFINITY 1e7

typedef short zijie;

using namespace std;

int PMSize = 16 * 1024;		//16KB
int VMSize = 1024 * 1024 * 1024;	//1GB
int pageSize = 4 * 1024;
int PMPageNum = 4;	//PMSize / pageSize
int VMPageNum = 256 * 1024;	//VMSize / pageSize
char Memory[16 * 1024];
char Disk[1024 * 1024 * 1024];
int PMPage[4];	//which page it represents
int recentUse[256 * 1024];	//which time it recently used
int missNum = 0;
int totalNum = 0;
struct
{
	int v;	//value position
	int adr;	//which index of page in memory
}pageTable[256 * 1024];


int LRU()
{
	int min = INFINITY;
	int result;
	for (int i = 0; i < PMPageNum; i++)
	{
		if (recentUse[PMPage[i]] < min)
			result = i;
	}
	return result;
}

char LW(int adr)
{
	totalNum++;

	int page, ofs;
	page = adr / pageSize;
	ofs = adr % pageSize;

	recentUse[page] = totalNum;

	if (pageTable[page].v == 1)
	{
		return Memory[pageTable[page].adr * pageSize + ofs];
	}
	else
	{
		missNum++;

		int p1 = LRU();
		pageTable[PMPage[p1]].v = 0;
		pageTable[PMPage[p1]].adr = -1;
		pageTable[page].v = 1;
		pageTable[page].adr = p1;
		
		for (int i = 0; i < pageSize; i++)
		{
			Memory[p1 * pageSize + i] = Disk[PMPage[p1] * pageSize + i];
		}

		return Memory[p1 * pageSize + ofs];
	}

}

void SW(int adr, char dat)
{
	totalNum++;

	int page, ofs;
	page = adr / pageSize;
	ofs = adr % pageSize;

	recentUse[page] = totalNum;

	if (pageTable[page].v == 1)
	{
		Memory[pageTable[page].adr * pageSize + ofs] = dat;
		Disk[page * pageSize + ofs] = dat;
	}
	else
	{
		missNum++;

		int p1 = LRU();
		pageTable[PMPage[p1]].v = 0;
		pageTable[PMPage[p1]].adr = -1;
		pageTable[page].v = 1;
		pageTable[page].adr = p1;

		for (int i = 0; i < pageSize; i++)
		{
			Memory[p1 * pageSize + i] = Disk[PMPage[p1] * pageSize + i];
		}

		Memory[p1 * pageSize + ofs] = dat;
		Disk[page * pageSize + ofs] = dat;
	}
}

int main()
{
	//initialize
	for (int i = 0; i < VMPageNum; i++)
	{
		if (i < 4)	//at first, pages 0-3 are in memory
		{
			pageTable[i].v = 1;
			pageTable[i].adr = i;
		}
		else
		{
			pageTable[i].v = 0;
			pageTable[i].adr = -1;
		}
		recentUse[i] = 0;
	}
	for (int i = 0; i < PMPageNum; i++)
	{
		PMPage[i] = i;
	}
	for (int i = 0; i < PMSize; i++)
	{
		Memory[i] = '0';
	}
	for (int i = 0; i < VMSize; i++)
	{
		Disk[i] = '0';
	}

	/*ifstream in("test.txt");
	int adr;
	char dat;
	
	for (int i = 0; i < 13000; i++)
	{
		in >> adr;
		LW(adr);
	}
	
	for (int i = 0; i < 50; i++)
	{
		in >> adr >> dat;
		SW(adr, dat);
	}

	in.close();*/

	double hitRate = (double)((totalNum - missNum) / totalNum);

	cout << "totalNum = " << totalNum << endl;
	cout << "missNum = " << missNum << endl;
	cout << "hit rate = " << hitRate << endl;

	system("pause");
}