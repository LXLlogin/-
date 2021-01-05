#include<iostream>
#include<sstream>
#include<vector>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<time.h>
using namespace std;


/*-------------string,double,int的转换   参考 https://www.cnblogs.com/chunzhulovefeiyue/p/6606742.html ---------------*/
//string转double
double stringToDouble(string num) //string转double
{
	if (num.size() == 0) return 0;
	bool minus = false;      //标记是否是负数
	string real = num;       //real表示num的绝对值
	if (num.at(0) == '-')
	{
		minus = true;
		real = num.substr(1, num.size() - 1);
	}

	char c;
	int i = 0;
	double result = 0.0, dec = 10.0;
	bool isDec = false;       //标记是否有小数
	unsigned long size = real.size();
	while (i < size)
	{
		c = real.at(i);
		if (c == '.')
		{//包含小数
			isDec = true;
			i++;
			continue;
		}
		if (!isDec)
		{
			result = result * 10 + c - '0';
		}
		else
		{//识别小数点之后都进入这个分支
			result = result + (c - '0') / dec;
			dec *= 10;
		}
		i++;
	}

	if (minus == true) {
		result = -result;
	}

	return result;
}
//double转string
string doubleToString(double num)
{
	char str[100];
	sprintf_s(str, sizeof(str), "%.2f", num);
	string result = str;
	return result;
}
//int转stirng
string intToString(int num)
{
	char str[20];
	sprintf_s(str, sizeof(str), "%d", num);
	string result = str;
	return result;
}



/*--------------------------------------------文件处理---------------------------------------------------------*/
//读取文件内数据
double(*File_In_Data(string filename, int & length,int & lie))[200]
{
	double(*data)[200] = new double[200][200];
	length = 0; lie = 0;		//行列初始化
	ifstream  file;
	file.open(filename, ios::in);
	string line;        //过渡存储
	//C1,C2,C3,C4,C5,C6,C7,C8,C9,Constitution(数值化）  --数据列
	if (file.is_open())
	{
		while (getline(file, line))     //按行读取
		{
			lie = 0;
			length++;
			stringstream ss(line);

			while (getline(ss, line, '\t'))      //用 , 分隔符分割
			{
				lie++;
				data[length][lie] = stringToDouble(line);       //成绩C1~C10
			}
		}
	}
	file.close();
	return data;
}

/*---------------------------------------------数据处理----------------------------------------------------*/
/*--------------------------------矩阵处理----------------------*/
//计算欧式距离
double euclideanDistance(double data1[200], double data2[200], int lie)
{
	double Distance = 0;
	for (int i = 1; i <= lie; i++)	Distance += ( (data1[i] - data2[i])* (data1[i] - data2[i]) );
	return sqrt(Distance);
}
//分类到最近的类
void classify(double data[][200], double kind[][200], int length, int lie, int k, double & Distortion)
{
	Distortion = 0;		//代价初始化
	double transition;		//最小距离
	for (int len = 1; len <= length; len++)
	{
		transition = 1000000.0;
		for (int i = 1; i <= k; i++)
		{
			double Distance = euclideanDistance(data[len], kind[i], lie);
			if (Distance < transition)
			{
				transition = Distance;
				data[len][0] = i;		//存储所属类别
			}
		}
		Distortion += transition;
	}
}
//计算聚类中心
void calculate(double data[][200], double kind[][200], int length, int lie, int k)
{
	//聚类中心初始化
	for (int i = 1, j = 1; i <= k; i++)	
	{
		for ( j = 0; j <= lie; j++)
		{
			kind[i][j] = 0;
		}
	}
	//计算聚类中心
	int k_len = 0;
	for (int i = 1, j = 1; i <= length; i++)
	{
		k_len = int(data[i][0]);		//类别
		for ( j = 1; j <= lie; j++)
		{
			kind[k_len][j] += data[i][j];
			kind[k_len][0]++;		//储存每类的数据数量
		}
	}
	for (int i = 1, j = 1; i <= k; i++)
	{
		for (j = 1; j <= lie; j++)
		{
			kind[i][j] = kind[i][j] / kind[i][0];
		}
	}
}
//k-means算法
double k_means(double data[][200], int length, int lie, int k)
{
	double(*kind)[200] = new double[200][200];		//聚类中心

	//初始化随机分配聚类中心
	srand((unsigned)time(NULL));
	//rand() % (b - a + 1) + a;
	int ran = rand() % int(length / k) + 1;
	for (int i = 1, j = 1, len; i <= k; i++)		
	{
		len = ran + (i - 1)*k;
		data[len][0] = i;
		for (j = 1; j <= lie; j++)
		{
			kind[i][j] = data[len][j]; 
		}
	}
	//k-means算法
	double Distortion = 0;		//代价
	double Distortion_0 = Distortion;		//代价副本
	classify(data, kind, length, lie, k, Distortion);
	while (Distortion_0 != Distortion)
	{
		Distortion_0 = Distortion;
		calculate(data, kind, length, lie, k);
		classify(data, kind, length, lie, k, Distortion);
	}
	return Distortion;
}


/*-----------------------------------------主函数------------------------------------------------------*/
int main()
{
	/*-------------矩阵统一最大大小为200*200----------------*/

	double(*data)[200];      //数据，0行不存储数据，0列存储类别 length x lie
	int length = 0;     //数据行数
	int lie = 0;       //数据列数

	string filename;


	/*-------------------------------------问题1----------------------------------------*/

	//读取数据进矩阵  length x lie
	filename = "C:\\Users\\LXL\\Desktop\\data实验2结果1.txt";
	data = File_In_Data(filename, length, lie);
	

	cout << length << " " << lie << endl;

	cout << "实验3 data1数据" << endl;
	int k = 5;		//k默认为5
	for ( k = 2; k <= 20; k++)
	{
		cout << "k=" << k << "；代价=" << k_means(data, length, lie, k) << endl;
	}cout << endl;

	/*-----------------------------------问题2---------------------------------------------*/


	//读取数据进矩阵  length x lie
	filename = "C:\\Users\\LXL\\Desktop\\实验3数据2.txt";
	data = File_In_Data(filename, length, lie);

	cout << length << " " << lie << endl;

	cout << "实验3 data2数据" << endl;
	k = 5;		//k默认为5
	for (k = 2; k <= 10; k++)
	{ 
		cout << "k=" << k << "；代价=" << k_means(data, length, lie, k) << endl;
	}


}