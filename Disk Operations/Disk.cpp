#include<iostream>
#include<string>
#include<string.h>
#include<io.h>
#include<time.h>
#include<direct.h>
#include<fstream>
#include<stdlib.h>

#define CLUSTERSIZE 512

using namespace std;

int cnt = 0;

void dir(string path)
{
	intptr_t hFile = 0;
	struct _finddata_t fileInfo;
	string pathName, exdName;
	struct tm *p;
	if ((hFile = _findfirst(pathName.assign(path).append("\\*").c_str(), &fileInfo)) == -1) 
	{
		return;
	}
	do
	{
		p = localtime(&fileInfo.time_create);
		cout << p->tm_year + 1900 << "/" << p->tm_mon + 1 << "/" << p->tm_mday << "  ";
		if (p->tm_hour < 10)
			cout << "0";
		cout << p->tm_hour << ":";
		if (p->tm_min < 10)
			cout << "0";
		cout << p->tm_min << "  ";
		cout << fileInfo.name << (fileInfo.attrib&_A_SUBDIR ? "  [folder]" : "  [file]") << endl;
	} while (_findnext(hFile, &fileInfo) == 0);
	_findclose(hFile);
	return;
}

void del(string fileName)
{
	fstream fp(fileName.c_str(), ifstream::binary | ifstream::in);
	if (!fp.is_open())
	{
		cout << "No such file in this directory!" << endl;
		return;
	}
	fp.close();
	int flag = remove(fileName.c_str());
	if (flag == 0)
	{
		cout << "Deleting successful!" << endl;
	}
	else
	{
		cout << "Deleting failed!" << endl;
	}
}

void md(string folderName, string path)
{
	string createPath = path + "\\" + folderName;
	//cout << createPath.c_str() << endl;
	int flag = _mkdir(createPath.c_str());
	//cout << flag << endl;
	if (flag == 0)
	{
		cout << "Creating successful!" << endl;
	}
	else
	{
		cout << "Creating failed!" << endl;
	}
}

void cd(string path)
{
	int flag = chdir(path.c_str());
	if (!flag)
	{
		return;
	}
	else
	{
		cout << "Error!" << endl;
		return;
	}
}

void copy(string fileName, string path)
{
	fstream in(fileName.c_str(), ifstream::binary | ifstream::in);
	if (!in.is_open())
	{
		cout << "No such file in this directory!" << endl;
		return;
	}
	char oldPath[100];
	_getcwd(oldPath, 100);
	string old = oldPath;
	cd(path);	//enter the destination path
	ofstream out(fileName.c_str(), ifstream::binary | ifstream::out);
	char buff[CLUSTERSIZE];
	long long totalBytes = 0;
	while (in)
	{
		in.read(buff, CLUSTERSIZE);
		out.write(buff, in.gcount());
		totalBytes += in.gcount();
	}
	in.close();
	out.close();

	cout << "Copying successful!" << endl;
	cd(old);	//return old path
}

int getFileSize(string fileName)
{
	return (int)(ifstream(fileName, ifstream::binary | ifstream::ate).tellg());
}

void clusterRead(string fileName, int clusterIndex)
{
	fstream fp(fileName, ifstream::binary | ifstream::in);
	if (!fp.is_open())
	{
		cout << "No such file in this directory!" << endl;
		return;
	}

	int fileSize = getFileSize(fileName);
	int clusterNum = fileSize / CLUSTERSIZE;

	if ((clusterIndex < 0 || clusterIndex >= clusterNum) && clusterNum != 0)
	{
		cout << "Error! Memory exceed!" << endl;
		return;
	}

	fp.seekg(clusterIndex * CLUSTERSIZE);
	char* readBuffer = new char[CLUSTERSIZE];
	fp.read(readBuffer, CLUSTERSIZE);
	char* cntStr = new char[10];
	itoa(cnt, cntStr, 10);
	string tmp = cntStr;
	string writeFile = "ReadBuffer" + tmp;
	fstream op(writeFile, ifstream::binary | ifstream::out);
	op.write(readBuffer, CLUSTERSIZE);

	fp.close();
	op.close();

	cout << "Reading successful, and the content has been stored in " << writeFile.c_str() << "." << endl;
	cnt++;
}

void clusterWrite(string fileName, int clusterIndex, char* writeBuffer)
{
	fstream fp(fileName, ifstream::binary | ifstream::out);

	int fileSize = getFileSize(fileName);
	int clusterNum = fileSize / CLUSTERSIZE;

	if (clusterIndex < 0 || clusterIndex > clusterNum)
	{
		cout << "Error! Memory exceed!" << endl;
		return;
	}

	fp.seekg(clusterIndex * CLUSTERSIZE);
	fp.write(writeBuffer, CLUSTERSIZE);

	cout << "Writing successful!" << endl;
}

int main()
{
	int op;
	char* nowPath = new char[100];
	_getcwd(nowPath, 100);
	cout << "You are now in " << nowPath << endl;
	cout << "Choose the operation you want to take." << endl;
	cout << "1. DIR" << endl;
	cout << "2. COPY" << endl;
	cout << "3. DEL" << endl;
	cout << "4. MD" << endl;
	cout << "5. CD" << endl;
	cout << "6. Read Block" << endl;
	cout << "7. Write Block" << endl;
	cout << "0. Exit" << endl;
	cin >> op;

	string path;
	string fileName;
	string folderName;
	int clusterIndex;
	char writeBuffer[CLUSTERSIZE];
	char buffer[100];
	char nameBuffer[100];

	while (op != 0)
	{
		switch (op)
		{
		case 1: cout << "Enter the whole path." << endl;
			getchar();
			gets_s(buffer);
			path = buffer;
			dir(path);
			break;
		case 2: cout << "Enter the file name." << endl;
			getchar();
			gets_s(nameBuffer);
			fileName = nameBuffer;
			cout << "Enter the path where you want to copy this file." << endl;
			gets_s(buffer);
			path = buffer;
			copy(fileName, path);
			break;
		case 3: cout << "Enter the file name." << endl;
			getchar();
			gets_s(nameBuffer);
			fileName = nameBuffer;
			del(fileName);
			break;
		case 4: cout << "Enter the folder name." << endl;
			getchar();
			gets_s(nameBuffer);
			folderName = nameBuffer;
			cout << "Enter the path where you want to create the folder." << endl;
			gets_s(buffer);
			path = buffer;
			md(folderName, path);
			break;
		case 5: cout << "Enter the whole path." << endl;
			getchar();
			gets_s(buffer);
			path = buffer;
			cd(path);
			strcpy(nowPath, path.c_str());
			break;
		case 6: cout << "Enter the file name." << endl;
			getchar();
			gets_s(nameBuffer);
			fileName = nameBuffer;
			cout << "Enter the cluster index." << endl;
			cin >> clusterIndex;
			clusterRead(fileName, clusterIndex);
			break;
		case 7: cout << "Enter the file name." << endl;
			getchar();
			gets_s(nameBuffer);
			fileName = nameBuffer;
			cout << "Enter the cluster index." << endl;
			cin >> clusterIndex;
			cout << "Enter the content you want to write." << endl;
			getchar();
			gets_s(writeBuffer);
			clusterWrite(fileName, clusterIndex, writeBuffer);
			break;
		case 0: break;
		default: cout << "Invalid input!" << endl;
			break;
		}

		cout << endl;
		cout << "You are now in " << nowPath << endl;
		cout << "Choose the operation you want to take." << endl;
		cout << "1. DIR" << endl;
		cout << "2. COPY" << endl;
		cout << "3. DEL" << endl;
		cout << "4. MD" << endl;
		cout << "5. CD" << endl;
		cout << "6. Read Block" << endl;
		cout << "7. Write Block" << endl;
		cout << "0. Exit" << endl;
		cin >> op;
	}

	system("pause");
	return 0;
}