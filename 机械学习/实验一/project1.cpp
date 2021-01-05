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


/*--------------------------------------------主要存储结构------------------------------------------*/
//学生
struct Student		
{
	string ID;		//学号,202xxx
	string Name;	//名字
	string City;		//城市，限定为 Beijing / Guangzhou / Shenzhen / Shanghai
	string Gender;		//性别，限定为 boy/girl
	string Constitution;		//体能测试成绩，限定为 bad/general/good/excellent
	double Constitution_value;		//数值化的体能测试成绩，bad(1)/general(2)/good(3)/excellent(4)/null(0)
	double Height = 0;		//身高
	double C1 = 0, C2 = 0, C3 = 0, C4 = 0, C5 = 0;		//成绩
	double C6 = 0, C7 = 0, C8 = 0, C9 = 0;		//成绩
};
//有数值的列的列存储
struct data_column
{
	vector<double> H;		//身高
	vector<double> C1, C2, C3, C4, C5, C6, C7, C8, C9;		//成绩
	vector<double> Constitution_value;		//体能测试成绩已数值化 bad(1)/general(2)/good(3)/excellent(4)/null(0)
};


/*--------------------------------------------数值处理----------------------------------------------*/
//性别处理--目的性别存储，操作的字符串
void Gender_deal(string & Gender,string line)		
{
	if (line == "male") line = "boy";
	if (line == "female") line = "girl";
	Gender = line;
}
//身高处理--目的身高存储，操作的字符串
void Height_deal(double & data_Height, string line)		
{
	if (line.size() != 0 && data_Height == 0)
	{
		double Height = stringToDouble(line);
		if (Height > 100) data_Height = Height;
		else data_Height = Height * 100;
	}
}
//成绩处理--目的成绩存储，操作的字符串
void C_deal(double & data_C, string line)
{
	if (line.size() != 0 && data_C == 0)
	{
		double C = stringToDouble(line);
		data_C = C;
	}
}
//体能测试成绩处理及数值化----目的体能测试成绩存储，目的数值化的体能测试成绩存储，操作的字符串
void Constitution_deal(string & data_Constitution, double & Constitution_value, string line)		
{
	if (data_Constitution.size() == 0)
	{
		// bad(1)/general(2)/good(3)/excellent(4)/null(0)
		if (line.size() == 0) {  Constitution_value = 0; line = "null"; }
		else if (line == "bad") Constitution_value = 1; 
		else if (line == "general") Constitution_value = 2; 
		else if (line == "good") Constitution_value = 3; 
		else if (line == "excellent") Constitution_value = 4;
		data_Constitution = line;
	}
}
//读取数值化的列数值
void data_PutIn_data_l(Student * data,data_column & data_l,int data_len_max)
{
	for (int i = 1; i <= data_len_max; i++)
	{
		if (data[i].ID.size() != 0)
		{
			data_l.H.push_back(data[i].Height);
			data_l.C1.push_back(data[i].C1); data_l.C2.push_back(data[i].C2); data_l.C3.push_back(data[i].C3);
			data_l.C4.push_back(data[i].C4); data_l.C5.push_back(data[i].C5); data_l.C6.push_back(data[i].C6);
			data_l.C7.push_back(data[i].C7); data_l.C8.push_back(data[i].C8); data_l.C9.push_back(data[i].C9);
			data_l.Constitution_value.push_back(data[i].Constitution_value);
		}
	}
}


/*----------------------------文件处理---------------------------------------------------------*/
//数据存入data数组--文件名字，目的存储，数据下标最大值，第x个文件
void File_In_Data(string filename, Student data[], int & data_len_max,int file_num) 
{

	ifstream  file;
	file.open(filename, ios::in);
	string line;		//过渡存储
	int data_len = 0;		//数据下标
	int count = 0;		//读取行的第count+1位
	//ID,Name,City,Gender,Height,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,Constitution  --数据列
	if (file.is_open())
	{
		while (getline(file, line))		//按行读取
		{
			count = 0;
			if (data_len != 0)	//首行不读取
			{
				stringstream ss(line);

				while (getline(ss, line, ','))		//用 , 分隔符分割
				{
					if (count == 0)
					{
						if (file_num == 1)
						{
							data_len = stringToDouble(line) - 202000; string ID = line;
							if (data_len_max < data_len) data_len_max = data_len;
							data[data_len].ID = line;
						}
						if (file_num == 2)
						{
							data_len = stringToDouble(line); string ID = intToString(data_len + 202000);
							if (data_len_max < data_len) data_len_max = data_len;
							data[data_len].ID = ID;
						}
					}
					else if (count == 1) data[data_len].Name = line;		//名字
					else if (count == 2) data[data_len].City = line; 		//城市
					else if (count == 3) Gender_deal(data[data_len].Gender, line); 	//性别
					else if (count == 4) Height_deal(data[data_len].Height, line); 	//身高
					else if (count == 5) C_deal(data[data_len].C1, line); 	 	//成绩C1
					else if (count == 6) C_deal(data[data_len].C2, line); 		//成绩C2
					else if (count == 7) C_deal(data[data_len].C3, line); 		//成绩C3
					else if (count == 8)  C_deal(data[data_len].C4, line); 		//成绩C4
					else if (count == 9) C_deal(data[data_len].C5, line); 		//成绩C5
					else if (count == 10) C_deal(data[data_len].C6, line); 		//成绩C6
					else if (count == 11) C_deal(data[data_len].C7, line); 		//成绩C7
					else if (count == 12) C_deal(data[data_len].C8, line); 		//成绩C8
					else if (count == 13) C_deal(data[data_len].C9, line); 		//成绩C9
					else if (count == 15) Constitution_deal(data[data_len].Constitution, data[data_len].Constitution_value, line); 		//体能测试成绩
					++count;
				}
			}
			else data_len++;
		}
	}
	file.close();
}
//写出处理后的数据
void write_file(string filename,Student * data,int data_len_max)
{
	fstream out;
	out.open(filename, ios::out);

	if (!out.is_open())
	{
		cout << "读取文件失败" << endl;
	}
	out << "ID,Name,City,Gender,Height,C1,C2,C3,C4,C5,C6,C7,C8,C9,Constitution," << endl;
	for (int i = 1; i <= data_len_max; i++)
	{
		if (data[i].ID.size() != 0)
		{
			out << data[i].ID << "," << data[i].Name << "," << data[i].City << "," << data[i].Gender << "," << data[i].Height << ","
				<< data[i].C1 << "," << data[i].C2 << "," << data[i].C3 << "," << data[i].C4 << "," << data[i].C5 << ","
				<< data[i].C6 << "," << data[i].C7 << "," << data[i].C8 << "," << data[i].C9 << "," 
				<< data[i].Constitution << "," << endl;
		}
	}
	out.close();
}


/*--------------------------------------用来解决问题--------------------------------------------*/
//存储筛选的数据
struct Count_student		
{
	double Constitution_value = 0; int num = 0;
	double Height = 0, C1 = 0, C2 = 0, C3 = 0, C4 = 0, C5 = 0, C6 = 0, C7 = 0, C8 = 0, C9 = 0;
};
//筛选数据的值累加
void Count_student_add(Student data, Count_student & filter)		
{
	filter.Height += data.Height;
	filter.C1 += data.C1; filter.C2 += data.C2; filter.C3 += data.C3; filter.C4 += data.C4;filter.C5 += data.C5; 
	filter.C6 += data.C6; filter.C7 += data.C7;filter.C8 += data.C8; filter.C9 += data.C9; 
	filter.Constitution_value += data.Constitution_value;
	filter.num++;
}
//筛选数据的平均值
void Count_student_average(Count_student & filter)
{
	filter.Height = filter.Height / filter.num;
	filter.C1 = filter.C1 / filter.num; filter.C2 = filter.C2 / filter.num; filter.C3 = filter.C3 / filter.num;
	filter.C4 = filter.C4 / filter.num; filter.C5 = filter.C5 / filter.num; filter.C6 = filter.C6 / filter.num;
	filter.C7 = filter.C7 / filter.num; filter.C8 = filter.C8 / filter.num; filter.C9 = filter.C9 / filter.num;
	filter.Constitution_value = filter.Constitution_value / filter.num ;
}


/*---------------------------------------------数据处理----------------------------------------------------*/
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
	double mean_x = mean(x), standard_x = standard(x),sum = 0;
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
	double result = 0;
	for (int i = 0; i < length; i++)
	{
		result += A[i] * B[i];
	}
	return result;
}


/*-----------------------------------------主函数------------------------------------------------------*/
int main()
{
	Student data[200];		//学生，0不存储数据
	int data_len_max = 0;		//存储数据下标最大值
	int length = 0;		//有效数据行数
	data_column data_l;		//有数值的数据的列存储

	/* 数据均用wps做了txt回车转,回车 | 即再每行最后加,  wps回车用^p表示 */
	string filename;
	filename = "C:\\Users\\LXL\\Desktop\\data_txt - 副本.txt";
	File_In_Data(filename, data, data_len_max, 1);

	//原数据是execl转为txt文件读取  参考 https://zhidao.baidu.com/question/11100809.html
	filename = "C:\\Users\\LXL\\Desktop\\data_excel - 副本.txt";
	File_In_Data(filename, data, data_len_max, 2);

	//有数值的数据的列存储
	data_PutIn_data_l(data, data_l, data_len_max);
	length = data_l.H.size();

	//写出数据
	filename = "C:\\Users\\LXL\\Desktop\\data实验1结果.txt";
	write_file(filename, data, data_len_max);

	cout << "问题:\n"
		<< "1. 学生中家乡在Beijing的所有课程的平均成绩。\n"
		<< "2. 学生中家乡在广州，课程1在80分以上，且课程9在9分以上的男同学的数量。(备注：该处做了修正，课程10数据为空，更改为课程9)\n"
		<< "3. 比较广州和上海两地女生的平均体能测试成绩，哪个地区的更强些？\n"
		<< "4. 学习成绩和体能测试成绩，两者的相关性是多少？（九门课的成绩分别与体能成绩计算相关性）\n" << endl;

	Count_student Beijing;		//家乡在北京的学生
	Count_student Guangzhou_G;		//家乡在广州的女学生
	Count_student Shanghai_G;		//家乡在上海的女学生
	int num2 = 0;		//学生中家乡在广州，课程1在80分以上，且课程9在9分以上的男同学的数量。
	for (int i = 1; i <= data_len_max; i++)
	{
		if (data[i].City == "Beijing") 	Count_student_add(data[i], Beijing);
		if (data[i].City == "Guangzhou" && data[i].C1 >= 80 && data[i].C9 >= 9 && data[i].Gender == "boy") num2++;
		if (data[i].City == "Guangzhou" && data[i].Gender == "girl") Count_student_add(data[i], Guangzhou_G);
		if (data[i].City == "Shanghai" && data[i].Gender == "girl") Count_student_add(data[i], Shanghai_G);
	}
	Count_student_average(Beijing); Count_student_average(Guangzhou_G); Count_student_average(Shanghai_G);
	cout << "回答：" << endl;
	cout << "\n1. 学生中家乡在Beijing的所有课程的平均成绩: \n C1	C2	C3	C4	C5	C6	C7	C8	C9" <<endl;
	cout << Beijing.C1 << " " << Beijing.C2 << " " << Beijing.C3 << " " << Beijing.C4 << " " << Beijing.C5 << " "
		<< Beijing.C6 << " " << Beijing.C7 << " " << Beijing.C8 << " " << Beijing.C9 <<endl;
	cout << "\n2. 学生中家乡在广州，课程1在80分以上，且课程9在9分以上的男同学的数量: " << num2 << endl;
	cout << "\n3. 比较广州和上海两地女生的平均体能测试成绩:" << endl;
	cout << "广州女生的平均体能测试成绩：" << Guangzhou_G.Constitution_value << "\t上海女生的平均体能测试成绩：" << Shanghai_G.Constitution_value << endl;
	if (Guangzhou_G.Constitution_value > Shanghai_G.Constitution_value) cout << "广州女生的平均体能测试成绩比较高" << endl;
	else if (Guangzhou_G.Constitution_value < Shanghai_G.Constitution_value) cout<< "上海女生的平均体能测试成绩比较高" << endl;
	else cout << "广州和上海两地女生的平均体能测试成绩一样高" << endl;


	cout << "\n4. 学习成绩和体能测试成绩，两者的相关性是:\n"
		<<"C1^Con   C2^Con   C3^Con   C4^Con   C5^Con   6^Con   C7^Con   C8^Con   C9^Con" << endl;
	vector<double> Con = z_score(data_l.Constitution_value);
	cout << correlation(z_score(data_l.C1), Con, length) << " "<< correlation(z_score(data_l.C2), Con, length) << " "
		<< correlation(z_score(data_l.C3), Con, length) << " " << correlation(z_score(data_l.C4), Con, length) << " "
		<< correlation(z_score(data_l.C5), Con, length) << " " << correlation(z_score(data_l.C6), Con, length) << " "
		<< correlation(z_score(data_l.C7), Con, length) << " " << correlation(z_score(data_l.C8), Con, length) << " "
		<< correlation(z_score(data_l.C9), Con, length) << endl;

}