#include<iostream>
#include<sstream>
#include<vector>
#include<fstream>
#include<string>
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



/*----------------------------------------文件的数值处理----------------------------------------------*/
//体能测试成绩处理及数值化
double Constitution_deal(string line)
{
	// bad(1)/general(2)/good(3)/excellent(4)/null(0)
	if (line == "null") return 0;
	else if (line == "bad") return 1;
	else if (line == "general") return 2;
	else if (line == "good") return 3;
	else if (line == "excellent") return 4;
}

/*--------------------------------------------文件处理---------------------------------------------------------*/
//读取文件内数据
double(*File_In_Data(string filename, int & length))[200]
{
	double(*data)[200] = new double[200][200];
	ifstream  file;
	file.open(filename, ios::in);
	string line;        //过渡存储
	int data_len = 0;       //读取的行
	int count = 0;      //读取行的第count+1位
	//ID,Name,City,Gender,Height,C1,C2,C3,C4,C5,C6,C7,C8,C9,Constitution  --数据列
	if (file.is_open())
	{
		while (getline(file, line))     //按行读取
		{
			count = 0;
			if (data_len != 0)  //首行不读取
			{
				stringstream ss(line);

				while (getline(ss, line, ','))      //用 , 分隔符分割
				{
					if (count == 0) { data[data_len][0] = stringToDouble(line); length++; }
					else if (count >= 5 && count <= 14) data[data_len][count - 4] = stringToDouble(line);       //成绩C1~C9
					if (count == 14) data[data_len][10] = Constitution_deal(line);      //体能测试成绩
					++count;
				}
				data_len++;
			}
			else data_len++;
		}
	}
	file.close();
	return data;
}
//写出处理后的数据
void write_file(string filename, double data[][200], int length, int lie, char A, char B)
{
	fstream out;
	out.open(filename, ios::out);

	if (!out.is_open())
	{
		cout << "读取文件失败" << endl;
	}
	for (int i = 1, j = 1; i <= length; i++)
	{
		for (j = 1; j <= lie; j++)
		{
			out << data[i][j] << A;
		}
		out << B;
	}
	out.close();
}


/*---------------------------------------------数据处理----------------------------------------------------*/
//||||||||||||||||||||
/*--------------------------------向量处理--------------------------*/
//求向量的平均值
double mean(vector<double> x)
{
	double number = 0; int size = x.size();
	for (int i = 0; i < size; i++)
	{
		number += x[i];
	}
	return number / size;
}
//求向量的标准差
double standard(vector<double> x)
{
	double x_ = mean(x), number = 0;
	int size = x.size();
	for (int i = 0; i < size; i++)
	{
		number += (x[i] - x_)*(x[i] - x_);
	}
	return sqrt(number / (size - 1));
}
//z-score规范化处理后的向量
vector<double> z_score(vector<double> x)
{
	vector<double> x_;
	double mean_x = mean(x), standard_x = standard(x), sum = 0;
	int size = x.size();
	for (int i = 0; i < size; i++)
	{
		sum = (x[i] - mean_x) / standard_x;
		x_.push_back(sum);
	}
	return x_;
}
//向量相关性
double correlation(vector<double> A, vector<double> B, int length)
{
	A = z_score(A); B = z_score(B);
	double result = 0;
	for (int i = 0; i < length; i++)
	{
		result += A[i] * B[i];
	}
	return result;
}

/*--------------------------------矩阵处理----------------------*/

//矩阵按列z-score归一化
double(*matrix_z_score(double data[][200], double data_column[][200], int length, int lie))[200]
{
	vector<double> data_l[20];
	for (int i = 1, j = 1; i <= length; i++)
	{
		for (j = 1; j <= lie; j++)
		{
			data_l[j - 1].push_back(data[i][j]); data_column[j][i] = data[i][j];
		}
	}
	//按列归一化
	for (int i = 1, j = 1; i <= lie; i++)
	{
		data_l[i - 1] = z_score(data_l[i - 1]);
	}
	//data归一化后存入
	for (int i = 1, j = 1; i <= length; i++)
	{
		for (j = 1; j <= lie; j++)
		{
			data[i][j] = data_l[j - 1][i - 1];
		}
	}
	return data;
}
//相关矩阵
double(*matrix_corr(double data[][200], int length, int lie))[200]
{
	vector<double> data_h[200];     //数据的行
	for (int i = 1, j = 1; i <= length; i++)
	{
		for (j = 1; j <= lie; j++)
		{
			data_h[i - 1].push_back(data[i][j]);
		}
	}

	double(*_data_)[200] = new double[200][200];    //相关矩阵
	for (int i = 1, j = 1; i <= length; i++)
	{
		_data_[i][0] = data[i][0];
		for (j = 1; j <= length; j++)
		{
			_data_[i][j] = correlation(data_h[i - 1], data_h[j - 1], lie);
		}
	}
	return _data_;
}
//复制矩阵
double(*matrix_copy(double data[][200], int length, int lie))[200]
{
	double(*data_0)[200] = new double[200][200];        //矩阵副本
	for (int i = 1, j = 1; i <= length; i++)
	{
		for (j = 1; j <= lie; j++)
		{
			data_0[i][j] = data[i][j];
		}
	}
	return data_0;
}
//最近矩阵 length x column
double(*matrix_near(double _data_[][200], int length, int column))[200]
{
	double(*_data_near)[200] = new double[200][200];        //最近矩阵 length x 3

	double(*_data_0)[200];                              //相关矩阵副本 length x length
	_data_0 = matrix_copy(_data_, length, length);

	//
	for (int i = 1, j = 1, k = 1, a = 1, b = 1; i <= length; i++)
	{
		//冒泡排序,每行距离绝对值从小到大
		double t = 0;
		int flag = 1;
		for (j = 1; j <= length && flag == 1; j++)
		{
			flag = 0;
			for (k = 1; k <= length - j; k++)
			{
				if (abs(_data_0[i][k]) > abs(_data_0[i][k + 1]))
				{
					flag = 1;
					t = _data_0[i][k + 1];
					_data_0[i][k + 1] = _data_0[i][k];
					_data_0[i][k] = t;
				}
			}
		}
		//储存每行距离最近的column个ID
		for (a = 1; a <= column; a++)
		{
			for (b = 1; b <= length; b++)
			{
				if (_data_[i][a] == _data_0[i][b]) _data_near[i][a] = _data_[b][0];
			}
		}
	}
	return _data_near;
}



/*-----------------------------------------主函数------------------------------------------------------*/
int main()
{
	/*-------------矩阵统一最大大小为200*200----------------*/

	double(*data)[200];      //数据，0行不存储数据，0列存ID length x lie
	double(*data_column)[200] = new double[200][200];     //数据的列
	int length = 0;     //数据行数
	int lie = 10;       //数据列数

	string filename;

	//读取数据进矩阵  length x lie
	filename = "C:\\Users\\LXL\\Desktop\\data实验1结果.txt";
	data = File_In_Data(filename, length);

	//数据矩阵按列z-score归一化 length x lie
	matrix_z_score(data, data_column, length, lie);

	//写出课程1成绩，体能成绩
	double(*data_C1_C10)[200] = new double[3][200];		//课程1成绩，体能成绩
	for (int i = 1; i <= length; i++)
	{
		data_C1_C10[1][i] = data_column[1][i];
		data_C1_C10[2][i] = data_column[10][i];
	}
	filename = "C:\\Users\\LXL\\Desktop\\data实验2-课程1成绩，体能成绩.txt";
	write_file(filename, data_C1_C10, 2, length, ',', '\n');

	//写出归一化后数据矩阵 length x lie
	filename = "C:\\Users\\LXL\\Desktop\\data实验2结果1.txt";
	write_file(filename, data, length, lie, '\t', '\n');

	//相关矩阵, 0列存ID length x length
	double(*_data_)[200] = new double[200][200];
	_data_ = matrix_corr(data, length, lie);

	//读取最近矩阵 length x column
	double(*_data_near)[200];       //最近矩阵 length x column
	int column = 3;
	_data_near = matrix_near(_data_, length, column);

	//写出最近矩阵 length x column
	filename = "C:\\Users\\LXL\\Desktop\\data实验2结果2.txt";
	write_file(filename, _data_near, length, column, ' ', '\n');

}