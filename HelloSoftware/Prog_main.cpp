//程序：			班级成绩分析系统
//作者：			王骏宇 赵喆 柴世杰 朱晓玥
//时间：			2022学年秋季

//代码行统计：	2372-401(*:拓展内容为主)
//王骏宇：		836*	
//赵喆：			530	
//柴世杰：		303*	
//朱晓玥：		302	
//CSDN纯引用：	401

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4267)
#pragma warning(disable:4244)
#include "fismtp.h"
#include<Windows.h>		
#include<commdlg.h>		//ofn选择对话框用
#include <fstream>
#include <iomanip>
#include <sstream>		//double转string用
#include <cctype>
#include <cmath>
#include<graphics.h>
#include <ctime>
#define TESTING
#define MAILING
using namespace std;

//基础变量常变量创建[wjy]
enum { Chinese, Math, Eng, Physics, Chemistry, Politics };
const int TCHINESE = 150;
const int TMATH = 150;
const int TENG = 150;
const int TPHYSICS = 110;
const int TCHEMISTRY = 100;
const int TPOLITICS = 100;
double class_average = 0, class_total = 0;	//[zz]
double mark_average[6] = { 0 };				//[zz]
struct Student
{
	int stu_num;			//学生编号
	string stu_id;			//学生学号
	string name;			//学生姓名
	double mark[6] = { 0 };	//六科分数
	double mark_total = 0;	//学生总分
	double mark_ave = 0;	//学生均分
	bool is_lean = 0;		//判断是否偏科
};


void init_prog(void);		//程序初始化设置[wjy]
void Mushstrcpy(char* str01, char* str02);		//自定义的strcpy功能的函数，字符串赋值[wjy]
char* Mushstrcat(char* str_01, char* str_02);	//自定义的strcat功能的函数，字符串拼接[wjy]
//tchar转string 源https://blog.csdn.net/skye1221/article/details/85243465
string TCHAR2STRING(TCHAR* str);
#ifdef MAILING
//double转string 源https ://blog.csdn.net/qq_33236491/article/details/108321230
std::string doubleToString(const double& val);
#endif
void bubble_sort(double arr[], int num);	//某冒泡排序[csj]
double arr_sum(double arr[], int num);		//数组求和[csj]
void print_chartofaverage(double* arr, int num);//[csj]
void print_fenduan(int sub, int arr[6][4]);//分段统计柱状图[csj]
double angle(int num, int sum);//[csj]
#define PI 3.1416

int main()
{
	init_prog();		//程序简单初始化[wjy]

	//以下为文件选择可视化部分，获取文件路径供文件流对象使用
	//得到string对象FilePath和FileName
	//[借鉴https://blog.csdn.net/skye1221/article/details/85243465，wjy修改逻辑]
	cout << "按任意键开始程序：";
	system("pause");
	system("cls");
	system("color 0F");
	char w01[100] = "Initial";
	char w02[100] = "izing......";
	char w05[100] = { 0 };
	char w06[100] = { 0 };
	char* w03 = w05;
	char* w04 = w06;
	Mushstrcpy(w04, w03);			//一些自定义函数的使用
	w04 = Mushstrcat(w01, w02);		//代表底层函数能用了……
	cout << w04 << endl;
	Sleep(500);
	cout << endl;

	cout << "按任意键开始选择文件……" << endl;
	system("pause");

	cout << endl;
ChooseFile01:
	string FilePath01;			//创建函数外string对象存储路径
	string FileName01;			//创建函数外string对象存储名称
	OPENFILENAME ofn01 = { 0 };
	TCHAR strFileName01[MAX_PATH] = { 0 };		//用于接收文件名
	ofn01.lStructSize = sizeof(OPENFILENAME);			//结构体大小
	ofn01.hwndOwner = NULL;		//拥有着窗口句柄
	ofn01.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//设置过滤
	ofn01.nFilterIndex = 1;		//过滤器索引
	ofn01.lpstrFile = strFileName01;		//接收返回的文件名，注意第一个字符需要为NULL
	ofn01.nMaxFile = sizeof(strFileName01);		//缓冲区长度
	ofn01.lpstrInitialDir = NULL;			//初始目录为默认
	ofn01.lpstrTitle = TEXT("请选择一个文件");		//窗口标题
	ofn01.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//文件、目录必须存在，隐藏只读选项
	//打开文件对话框
	if (GetOpenFileName(&ofn01)) {
		string filePath = TCHAR2STRING(strFileName01);
		int start = filePath.find_last_of('\\');
		int end = filePath.find_last_of('.');
		string fileName = filePath.substr(start + 1, end - start - 1);
		string exten = filePath.substr(end, filePath.length() - end);
		cout << "您选择的文件路径为:" << filePath << endl;
		cout << "文件名为：" << fileName << "\t\t" << "拓展名为：" << exten << endl;
		FilePath01 = filePath;
		FileName01 = fileName;
		if (exten != ".txt")		//拓展名错误时进行检查
		{
			cout << ">>>文件拓展名错误，请重新选择txt成绩表" << endl;
			system("pause");
			goto ChooseFile01;
		}
	}
	else {
		MessageBox(NULL, TEXT("请选择一文件"), NULL, MB_ICONERROR);
		goto ChooseFile01;
	}

	//打开成绩表文件(经测试此段可正常打开文件写入)[wjy]
	ifstream infile;
	ofstream ofile;
	infile.open(FilePath01);
	//检查文件是否正确打开[wjy]
	if (!infile.is_open())
	{
		cout << "Could not open the file " << FileName01 << endl;
		cout << "Program terminating.\n";
		exit(EXIT_FAILURE);
	}

	//写入学生成绩数据到结构数组中[csj&wjy]
	string rubbishbin = "abab";
	int i = 0, j = 0;
	int num_students = 0;
	infile >> num_students;
	cout << "学生总数为：" << num_students << " 人\n学生列表为：" << endl;
	Student* students = new Student[num_students];
	//读取表头并舍弃表头，不干扰后续写入[csj&wjy]
	do {
		infile >> rubbishbin;
	} while (rubbishbin != "Politics");
	//读取数据内容[csj&wjy]
	for (i = 0; i < num_students; i++)
	{
		infile >> students[i].stu_num;
		infile >> students[i].stu_id;
		infile >> students[i].name;
		for (j = 0; j < 6; j++)
			infile >> students[i].mark[j];
	}
	infile.close();
	//显性展示学生数据，使用iomanip优化格式控制[wjy]
	cout.setf(ios_base::fixed, ios_base::floatfield);
	cout.precision(1);
	cout << setiosflags(ios::left) << setw(6) << "编号";
	cout << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "学生学号";
	cout << setw(18) << "学生姓名";
	cout << setw(10) << resetiosflags(ios::left) << setiosflags(ios::right) << "语文" << setw(10) << "数学"
		<< setw(10) << "英语" << setw(10) << "物理" << setw(10) << "化学" << setw(10) << "政治";
	cout << endl;
	for (i = 0; i < num_students; i++)
	{
		cout.setf(ios_base::fixed, ios_base::floatfield);
		cout.precision(1);
		cout << setiosflags(ios::right) << setfill('0') << setw(3) << students[i].stu_num << "   ";
		cout << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << students[i].stu_id;
		cout << setw(18) << students[i].name;
		for (j = 0; j < 6; j++)
			cout << setw(10) << resetiosflags(ios::left) << setiosflags(ios::right) << students[i].mark[j];
		cout << endl;
		if (i % 19 == 0 && i != 0)
		{
			cout << "按任意键显示下20条数据……";
			system("pause");
		}
	}
	cout << "是否清屏? <Y/N>\n>>>";
	char clear = 0;
	cin >> clear;
	if (clear == 'Y') system("cls");
	cout << "数据写入完毕，开始成绩分析：" << endl;
	cout << "按任意键生成总分降序排序学生成绩表……" << endl;
	system("pause");
	cout << "请选择文件夹并新建表格(建议命名：总分降序成绩排序表.txt)" << endl;
	system("pause");
	cout << endl;

	//计算每个学生总分[zz]
	for (i = 0, students[i].mark_total = 0; i < num_students; i++)
	{
		for (j = 0; j < 6; j++)
			students[i].mark_total = students[i].mark_total + students[i].mark[j];
	}
	//计算班级平均分[zz]
	for (i = 0, class_total = 0; i < num_students; i++)
	{
		class_total = students[i].mark_total + class_total;
	}
	class_average = class_total / num_students;
	//计算各科平均分[zz]
	for (j = 0; j < 6; j++)
	{
		for (i = 0; i < num_students; i++)
		{
			mark_average[j] = students[i].mark[j] + mark_average[j];
		}
		mark_average[j] = mark_average[j] / num_students;
	}
	//按总分进行降序排序[zz]
	string temp_zz;
	double TEMP_ZZ;
	int l;
	for (i = 0; i <= num_students; i++)
	{
		for (j = 0; j < num_students; j++)
		{
			if (students[i].mark_total >= students[j].mark_total)
			{
				temp_zz = students[i].stu_id;
				students[i].stu_id = students[j].stu_id;
				students[j].stu_id = temp_zz;

				temp_zz = students[i].name;
				students[i].name = students[j].name;
				students[j].name = temp_zz;

				TEMP_ZZ = students[i].mark_total;
				students[i].mark_total = students[j].mark_total;
				students[j].mark_total = TEMP_ZZ;

				for (l = 0; l < 6; l++)
				{
					TEMP_ZZ = students[i].mark[l];
					students[i].mark[l] = students[j].mark[l];
					students[j].mark[l] = TEMP_ZZ;
				}
			}
		}
	}

	//班级分析邮件发送铺垫[wjy]
#ifdef MAILING
	string highest = doubleToString(students[0].mark_total);
	string highestname = students[0].name;
	string lowest = doubleToString(students[num_students - 1].mark_total);
	string lowestname = students[num_students - 1].name;
#endif

	//用户自行创建TXT文件用以输出[wjy]
ChooseFile02:
	string FilePath02;
	string FileName02;
	OPENFILENAME ofn02 = { 0 };
	TCHAR strFileName02[MAX_PATH] = { 0 };		//用于接收文件名
	ofn02.lStructSize = sizeof(OPENFILENAME);			//结构体大小
	ofn02.hwndOwner = NULL;		//拥有着窗口句柄
	ofn02.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//设置过滤
	ofn02.nFilterIndex = 1;		//过滤器索引
	ofn02.lpstrFile = strFileName02;		//接收返回的文件名，注意第一个字符需要为NULL
	ofn02.nMaxFile = sizeof(strFileName02);		//缓冲区长度
	ofn02.lpstrInitialDir = NULL;			//初始目录为默认
	ofn02.lpstrTitle = TEXT("请选择一个文件");		//窗口标题
	ofn02.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//文件、目录必须存在，隐藏只读选项
	//打开文件对话框
	if (GetOpenFileName(&ofn02)) {
		string filePath = TCHAR2STRING(strFileName02);
		int start = filePath.find_last_of('\\');
		int end = filePath.find_last_of('.');
		string fileName = filePath.substr(start + 1, end - start - 1);
		string exten = filePath.substr(end, filePath.length() - end);
		cout << "您选择的文件路径为:" << filePath << endl;
		cout << "文件名为：" << fileName << "\t\t" << "拓展名为：" << exten << endl;
		FilePath02 = filePath;
		FileName02 = fileName;
		if (exten != ".txt")
		{
			cout << ">>>文件拓展名错误，请重新创建txt成绩表" << endl;
			system("pause");
			goto ChooseFile02;
		}
	}
	else {
		MessageBox(NULL, TEXT("请新建一txt文件"), NULL, MB_ICONERROR);
		goto ChooseFile02;
	}

	//检查文件正常打开[wjy]
	ofile.open(FilePath02);
	if (!ofile.is_open())
	{
		cout << "Could not open the file " << FileName01 << endl;
		cout << "Program terminating.\n";
		exit(EXIT_FAILURE);
	}

	//输出总分降序排序表[zz]
	ofile << "以下为按总分降序排序表：" << endl;
	ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "序号 " << "  ";
	ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "学号";
	ofile << setw(18) << "姓名";
	ofile << resetiosflags(ios::left) << setiosflags(ios::right);
	ofile << setiosflags(ios::right) << setw(10) << "总分";
	ofile << setiosflags(ios::right) << setw(10) << "语文";
	ofile << setiosflags(ios::right) << setw(10) << "数学";
	ofile << setiosflags(ios::right) << setw(10) << "英语";
	ofile << setiosflags(ios::right) << setw(10) << "物理";
	ofile << setiosflags(ios::right) << setw(10) << "化学";
	ofile << setiosflags(ios::right) << setw(10) << "政治";
	ofile << endl;
	for (i = 0; i < num_students; i++)
	{
		ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setfill('0') << setw(3) << students[i].stu_num << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << students[i].stu_id;
		ofile << setiosflags(ios::left) << setw(18) << students[i].name;
		ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setw(10) << students[i].mark_total;
		for (j = 0; j < 6; j++)
			ofile << setw(10) << students[i].mark[j];
		ofile << endl;
	}
	ofile.close();
	cout << "总分降序排序学生成绩表生成完毕，按任意键进入选择性排序……" << endl;
	system("pause");
	cout << endl;

	//用户选择按什么排序[zz]
	int chooserand;
	char anw;
ChooseRand:
	cout << "输入数字，打印相应的成绩排名表：" << endl;
	cout << "\t语文请输入0" << endl;
	cout << "\t数学请输入1" << endl;
	cout << "\t英语请输入2" << endl;
	cout << "\t物理请输入3" << endl;
	cout << "\t化学请输入4" << endl;
	cout << "\t政治请输入5" << endl;
	cout << ">>>";
	cin >> chooserand;
	if (chooserand == 0) {
		cout << "是否对语文进行排序? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseRand;
		for (i = 0; i <= num_students; i++)
		{
			for (j = 0; j < num_students; j++)
			{
				if (students[i].mark[0] >= students[j].mark[0])
				{
					temp_zz = students[i].stu_id;
					students[i].stu_id = students[j].stu_id;
					students[j].stu_id = temp_zz;

					temp_zz = students[i].name;
					students[i].name = students[j].name;
					students[j].name = temp_zz;

					TEMP_ZZ = students[i].mark_total;
					students[i].mark_total = students[j].mark_total;
					students[j].mark_total = TEMP_ZZ;

					for (l = 0; l < 6; l++)
					{
						TEMP_ZZ = students[i].mark[l];
						students[i].mark[l] = students[j].mark[l];
						students[j].mark[l] = TEMP_ZZ;
					}
				}
			}
		}
	}
	else if (chooserand == 1) {
		cout << "是否对数学进行排序? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseRand;
		for (i = 0; i <= num_students; i++)
		{
			for (j = 0; j < num_students; j++)
			{
				if (students[i].mark[1] >= students[j].mark[1])
				{
					temp_zz = students[i].stu_id;
					students[i].stu_id = students[j].stu_id;
					students[j].stu_id = temp_zz;

					temp_zz = students[i].name;
					students[i].name = students[j].name;
					students[j].name = temp_zz;

					TEMP_ZZ = students[i].mark_total;
					students[i].mark_total = students[j].mark_total;
					students[j].mark_total = TEMP_ZZ;

					for (l = 0; l < 6; l++)
					{
						TEMP_ZZ = students[i].mark[l];
						students[i].mark[l] = students[j].mark[l];
						students[j].mark[l] = TEMP_ZZ;
					}
				}
			}
		}
	}
	else if (chooserand == 2) {
		cout << "是否对英语进行排序? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseRand;
		for (i = 0; i <= num_students; i++)
		{
			for (j = 0; j < num_students; j++)
			{
				if (students[i].mark[2] >= students[j].mark[2])
				{
					temp_zz = students[i].stu_id;
					students[i].stu_id = students[j].stu_id;
					students[j].stu_id = temp_zz;

					temp_zz = students[i].name;
					students[i].name = students[j].name;
					students[j].name = temp_zz;

					TEMP_ZZ = students[i].mark_total;
					students[i].mark_total = students[j].mark_total;
					students[j].mark_total = TEMP_ZZ;

					for (l = 0; l < 6; l++)
					{
						TEMP_ZZ = students[i].mark[l];
						students[i].mark[l] = students[j].mark[l];
						students[j].mark[l] = TEMP_ZZ;
					}
				}
			}
		}
	}
	else if (chooserand == 3) {
		cout << "是否对物理进行排序? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseRand;
		for (i = 0; i <= num_students; i++)
		{
			for (j = 0; j < num_students; j++)
			{
				if (students[i].mark[3] >= students[j].mark[3])
				{
					temp_zz = students[i].stu_id;
					students[i].stu_id = students[j].stu_id;
					students[j].stu_id = temp_zz;

					temp_zz = students[i].name;
					students[i].name = students[j].name;
					students[j].name = temp_zz;

					TEMP_ZZ = students[i].mark_total;
					students[i].mark_total = students[j].mark_total;
					students[j].mark_total = TEMP_ZZ;

					for (l = 0; l < 6; l++)
					{
						TEMP_ZZ = students[i].mark[l];
						students[i].mark[l] = students[j].mark[l];
						students[j].mark[l] = TEMP_ZZ;
					}
				}
			}
		}
	}
	else if (chooserand == 4) {
		cout << "是否对化学进行排序? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseRand;
		for (i = 0; i <= num_students; i++)
		{
			for (j = 0; j < num_students; j++)
			{
				if (students[i].mark[4] >= students[j].mark[4])
				{
					temp_zz = students[i].stu_id;
					students[i].stu_id = students[j].stu_id;
					students[j].stu_id = temp_zz;

					temp_zz = students[i].name;
					students[i].name = students[j].name;
					students[j].name = temp_zz;

					TEMP_ZZ = students[i].mark_total;
					students[i].mark_total = students[j].mark_total;
					students[j].mark_total = TEMP_ZZ;

					for (l = 0; l < 6; l++)
					{
						TEMP_ZZ = students[i].mark[l];
						students[i].mark[l] = students[j].mark[l];
						students[j].mark[l] = TEMP_ZZ;
					}
				}
			}
		}
	}
	else if (chooserand == 5) {
		cout << "是否对政治进行排序? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseRand;
		for (i = 0; i <= num_students; i++)
		{
			for (j = 0; j < num_students; j++)
			{
				if (students[i].mark[5] >= students[j].mark[5])
				{
					temp_zz = students[i].stu_id;
					students[i].stu_id = students[j].stu_id;
					students[j].stu_id = temp_zz;

					temp_zz = students[i].name;
					students[i].name = students[j].name;
					students[j].name = temp_zz;

					TEMP_ZZ = students[i].mark_total;
					students[i].mark_total = students[j].mark_total;
					students[j].mark_total = TEMP_ZZ;

					for (l = 0; l < 6; l++)
					{
						TEMP_ZZ = students[i].mark[l];
						students[i].mark[l] = students[j].mark[l];
						students[j].mark[l] = TEMP_ZZ;
					}
				}
			}
		}
	}
	else {
		cout << "输入数字无效，请重新输入（如：1）" << endl;
		goto ChooseRand;
	}

	//用户自行创建TXT文件用以输出[wjy]
ChooseFile03:
	string FilePath03;
	string FileName03;
	OPENFILENAME ofn03 = { 0 };
	TCHAR strFileName03[MAX_PATH] = { 0 };		//用于接收文件名
	ofn03.lStructSize = sizeof(OPENFILENAME);			//结构体大小
	ofn03.hwndOwner = NULL;		//拥有着窗口句柄
	ofn03.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//设置过滤
	ofn03.nFilterIndex = 1;		//过滤器索引
	ofn03.lpstrFile = strFileName03;		//接收返回的文件名，注意第一个字符需要为NULL
	ofn03.nMaxFile = sizeof(strFileName03);		//缓冲区长度
	ofn03.lpstrInitialDir = NULL;			//初始目录为默认
	ofn03.lpstrTitle = TEXT("请选择一个文件");		//窗口标题
	ofn03.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//文件、目录必须存在，隐藏只读选项
	//打开文件对话框
	if (GetOpenFileName(&ofn03)) {
		string filePath = TCHAR2STRING(strFileName03);
		int start = filePath.find_last_of('\\');
		int end = filePath.find_last_of('.');
		string fileName = filePath.substr(start + 1, end - start - 1);
		string exten = filePath.substr(end, filePath.length() - end);
		cout << "您选择的文件路径为:" << filePath << endl;
		cout << "文件名为：" << fileName << "\t\t" << "拓展名为：" << exten << endl;
		FilePath03 = filePath;
		FileName03 = fileName;
		if (exten != ".txt")
		{
			cout << ">>>文件拓展名错误，请重新创建txt成绩表" << endl;
			system("pause");
			goto ChooseFile03;
		}
	}
	else {
		MessageBox(NULL, TEXT("请新建一txt文件"), NULL, MB_ICONERROR);
		goto ChooseFile03;
	}

	//检查文件正常打开[wjy]
	ofile.open(FilePath03);
	if (!ofile.is_open())
	{
		cout << "Could not open the file " << FileName01 << endl;
		cout << "Program terminating.\n";
		exit(EXIT_FAILURE);
	}

	//表格内学科展示的优化[wjy]
	string subject_wjyzz = "当前降序排序学科为：";
	switch (chooserand)
	{
	case 0: subject_wjyzz += "语文："; break;
	case 1: subject_wjyzz += "数学："; break;
	case 2: subject_wjyzz += "英语："; break;
	case 3: subject_wjyzz += "物理："; break;
	case 4: subject_wjyzz += "化学："; break;
	case 5: subject_wjyzz += "政治："; break;
	}
	ofile << subject_wjyzz << endl;

	//输出用户指定降序排序表[zz]
	ofile << "以下为按该学科降序排序表：" << endl;
	ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "序号 " << "  ";
	ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "学号";
	ofile << setw(18) << "姓名";
	ofile << resetiosflags(ios::left) << setiosflags(ios::right);
	ofile << setiosflags(ios::right) << setw(10) << "总分";
	ofile << setiosflags(ios::right) << setw(10) << "语文";
	ofile << setiosflags(ios::right) << setw(10) << "数学";
	ofile << setiosflags(ios::right) << setw(10) << "英语";
	ofile << setiosflags(ios::right) << setw(10) << "物理";
	ofile << setiosflags(ios::right) << setw(10) << "化学";
	ofile << setiosflags(ios::right) << setw(10) << "政治";
	ofile << endl;
	for (i = 0; i < num_students; i++)
	{
		ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setfill('0') << setw(3) << students[i].stu_num << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << students[i].stu_id;
		ofile << setiosflags(ios::left) << setw(18) << students[i].name;
		ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setw(10) << students[i].mark_total;
		for (j = 0; j < 6; j++)
			ofile << setw(10) << students[i].mark[j];
		ofile << endl;
	}
	cout << "输出成功。" << endl << endl;
	ofile.close();
	cout << "是否继续选择性排序？<Y/N>";
	cin >> anw;
	if (anw == 'Y')
		goto ChooseRand;
	cout << endl;
	cout << "单项学科降序排序表全部生成完毕，" << endl;
	cout << "是否清屏? <Y/N>\n>>>";
	cin >> clear;
	if (clear == 'Y') system("cls");
	cout << "按任意键进入学生筛选……" << endl;
	system("pause");
	cout << endl;

	//用户输入选择条件并筛选，输出筛选降序表
	//用户筛选部分[zxy]
	int choosecond;;
ChooseCond:
	cout << "输入数字，进入相应的成绩筛选：" << endl;
	cout << "\t语文请输入0" << endl;
	cout << "\t数学请输入1" << endl;
	cout << "\t英语请输入2" << endl;
	cout << "\t物理请输入3" << endl;
	cout << "\t化学请输入4" << endl;
	cout << "\t政治请输入5" << endl;
	cout << "\t总分请输入6" << endl;
	cout << ">>>";
	cin >> choosecond;
	if (choosecond == 0) {
		cout << "是否对语文进行筛选? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseCond;

		//语文排序[zz]
		for (i = 0; i <= num_students; i++)
		{
			for (j = 0; j < num_students; j++)
			{
				if (students[i].mark[0] >= students[j].mark[0])
				{
					temp_zz = students[i].stu_id;
					students[i].stu_id = students[j].stu_id;
					students[j].stu_id = temp_zz;

					temp_zz = students[i].name;
					students[i].name = students[j].name;
					students[j].name = temp_zz;

					TEMP_ZZ = students[i].mark_total;
					students[i].mark_total = students[j].mark_total;
					students[j].mark_total = TEMP_ZZ;

					for (l = 0; l < 6; l++)
					{
						TEMP_ZZ = students[i].mark[l];
						students[i].mark[l] = students[j].mark[l];
						students[j].mark[l] = TEMP_ZZ;
					}
				}
			}
		}
		//学生筛选[zxy]
		int top = TCHINESE; int bottom = 0;
		int n0[30] = { 0 };
		int z = 0;
		cout << "输入筛选区间[bottom,top]:" << endl;
		cout << "bottom = "; cin >> bottom;
		cout << "top = "; cin >> top;
		for (i = 0; i < num_students; i++) {
			if ((bottom < students[i].mark[0]) && (students[i].mark[0] < top))
			{
				n0[z] = i;
				z++;
			}
		}
		int numcond = z;

		//用户自行创建TXT文件用以输出[wjy]
	ChooseFile04:
		string FilePath04;
		string FileName04;
		OPENFILENAME ofn04 = { 0 };
		TCHAR strFileName04[MAX_PATH] = { 0 };		//用于接收文件名
		ofn04.lStructSize = sizeof(OPENFILENAME);			//结构体大小
		ofn04.hwndOwner = NULL;		//拥有着窗口句柄
		ofn04.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//设置过滤
		ofn04.nFilterIndex = 1;		//过滤器索引
		ofn04.lpstrFile = strFileName04;		//接收返回的文件名，注意第一个字符需要为NULL
		ofn04.nMaxFile = sizeof(strFileName04);		//缓冲区长度
		ofn04.lpstrInitialDir = NULL;			//初始目录为默认
		ofn04.lpstrTitle = TEXT("请选择一个文件");		//窗口标题
		ofn04.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//文件、目录必须存在，隐藏只读选项
		//打开文件对话框
		if (GetOpenFileName(&ofn04)) {
			string filePath = TCHAR2STRING(strFileName04);
			int start = filePath.find_last_of('\\');
			int end = filePath.find_last_of('.');
			string fileName = filePath.substr(start + 1, end - start - 1);
			string exten = filePath.substr(end, filePath.length() - end);
			cout << "您选择的文件路径为:" << filePath << endl;
			cout << "文件名为：" << fileName << "\t\t" << "拓展名为：" << exten << endl;
			FilePath04 = filePath;
			FileName04 = fileName;
			if (exten != ".txt")
			{
				cout << ">>>文件拓展名错误，请重新创建txt成绩表" << endl;
				system("pause");
				goto ChooseFile04;
			}
		}
		else {
			MessageBox(NULL, TEXT("请新建一txt文件"), NULL, MB_ICONERROR);
			goto ChooseFile04;
		}

		//检查文件正常打开[wjy]
		ofile.open(FilePath04);
		if (!ofile.is_open())
		{
			cout << "Could not open the file " << FileName01 << endl;
			cout << "Program terminating.\n";
			exit(EXIT_FAILURE);
		}
		//表格打印[zxy]
		ofile << "以下为按语文筛选的语文降序排序表：" << endl;
		ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "序号 " << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "学号";
		ofile << setw(18) << "姓名";
		ofile << resetiosflags(ios::left) << setiosflags(ios::right);
		ofile << setiosflags(ios::right) << setw(10) << "总分";
		ofile << setiosflags(ios::right) << setw(10) << "语文";
		ofile << setiosflags(ios::right) << setw(10) << "数学";
		ofile << setiosflags(ios::right) << setw(10) << "英语";
		ofile << setiosflags(ios::right) << setw(10) << "物理";
		ofile << setiosflags(ios::right) << setw(10) << "化学";
		ofile << setiosflags(ios::right) << setw(10) << "政治";
		ofile << endl;
		for (i = 0; i < numcond; i++)
		{
			ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setfill('0') << setw(3) << students[n0[i]].stu_num << "  ";
			ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << students[n0[i]].stu_id;
			ofile << setiosflags(ios::left) << setw(18) << students[n0[i]].name;
			ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setw(10) << students[n0[i]].mark_total;
			for (j = 0; j < 6; j++)
				ofile << setw(10) << students[n0[i]].mark[j];
			ofile << endl;
		}
		ofile.close();
	}
	else if (choosecond == 1) {
		cout << "是否对数学进行筛选? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseRand;

		//数学排序[zz]
		for (i = 0; i <= num_students; i++)
		{
			for (j = 0; j < num_students; j++)
			{
				if (students[i].mark[1] >= students[j].mark[1])
				{
					temp_zz = students[i].stu_id;
					students[i].stu_id = students[j].stu_id;
					students[j].stu_id = temp_zz;

					temp_zz = students[i].name;
					students[i].name = students[j].name;
					students[j].name = temp_zz;

					TEMP_ZZ = students[i].mark_total;
					students[i].mark_total = students[j].mark_total;
					students[j].mark_total = TEMP_ZZ;

					for (l = 0; l < 6; l++)
					{
						TEMP_ZZ = students[i].mark[l];
						students[i].mark[l] = students[j].mark[l];
						students[j].mark[l] = TEMP_ZZ;
					}
				}
			}
		}
		//学生选择[zxy]
		int top = TMATH; int bottom = 0;
		int n0[30] = { 0 };
		int z = 0;
		cout << "输入筛选区间[bottom,top]:" << endl;
		cout << "bottom = "; cin >> bottom;
		cout << "top = "; cin >> top;
		for (i = 0; i < num_students; i++) {
			if ((bottom < students[i].mark[1]) && (students[i].mark[1] < top))
			{
				n0[z] = i;
				z++;
			}
		}
		int numcond = z;

		//用户自行创建TXT文件用以输出[wjy]
	ChooseFile05:
		string FilePath05;
		string FileName05;
		OPENFILENAME ofn05 = { 0 };
		TCHAR strFileName05[MAX_PATH] = { 0 };		//用于接收文件名
		ofn05.lStructSize = sizeof(OPENFILENAME);			//结构体大小
		ofn05.hwndOwner = NULL;		//拥有着窗口句柄
		ofn05.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//设置过滤
		ofn05.nFilterIndex = 1;		//过滤器索引
		ofn05.lpstrFile = strFileName05;		//接收返回的文件名，注意第一个字符需要为NULL
		ofn05.nMaxFile = sizeof(strFileName05);		//缓冲区长度
		ofn05.lpstrInitialDir = NULL;			//初始目录为默认
		ofn05.lpstrTitle = TEXT("请选择一个文件");		//窗口标题
		ofn05.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//文件、目录必须存在，隐藏只读选项
		//打开文件对话框
		if (GetOpenFileName(&ofn05)) {
			string filePath = TCHAR2STRING(strFileName05);
			int start = filePath.find_last_of('\\');
			int end = filePath.find_last_of('.');
			string fileName = filePath.substr(start + 1, end - start - 1);
			string exten = filePath.substr(end, filePath.length() - end);
			cout << "您选择的文件路径为:" << filePath << endl;
			cout << "文件名为：" << fileName << "\t\t" << "拓展名为：" << exten << endl;
			FilePath05 = filePath;
			FileName05 = fileName;
			if (exten != ".txt")
			{
				cout << ">>>文件拓展名错误，请重新创建txt成绩表" << endl;
				system("pause");
				goto ChooseFile05;
			}
		}
		else {
			MessageBox(NULL, TEXT("请新建一txt文件"), NULL, MB_ICONERROR);
			goto ChooseFile05;
		}

		//检查文件正常打开[wjy]
		ofile.open(FilePath05);
		if (!ofile.is_open())
		{
			cout << "Could not open the file " << FileName01 << endl;
			cout << "Program terminating.\n";
			exit(EXIT_FAILURE);
		}
		//输出表格[zxy]
		ofile << "以下为按数学筛选的数学降序排序表：" << endl;
		ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "序号 " << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "学号";
		ofile << setw(18) << "姓名";
		ofile << resetiosflags(ios::left) << setiosflags(ios::right);
		ofile << setiosflags(ios::right) << setw(10) << "总分";
		ofile << setiosflags(ios::right) << setw(10) << "语文";
		ofile << setiosflags(ios::right) << setw(10) << "数学";
		ofile << setiosflags(ios::right) << setw(10) << "英语";
		ofile << setiosflags(ios::right) << setw(10) << "物理";
		ofile << setiosflags(ios::right) << setw(10) << "化学";
		ofile << setiosflags(ios::right) << setw(10) << "政治";
		ofile << endl;
		for (i = 0; i < numcond; i++)
		{
			ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setfill('0') << setw(3) << students[n0[i]].stu_num << "  ";
			ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << students[n0[i]].stu_id;
			ofile << setiosflags(ios::left) << setw(18) << students[n0[i]].name;
			ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setw(10) << students[n0[i]].mark_total;
			for (j = 0; j < 6; j++)
				ofile << setw(10) << students[n0[i]].mark[j];
			ofile << endl;
		}
		ofile.close();
	}
	else if (choosecond == 2) {
		cout << "是否对英语进行筛选? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseCond;
		//英语排序[zz]
		for (i = 0; i <= num_students; i++)
		{
			for (j = 0; j < num_students; j++)
			{
				if (students[i].mark[2] >= students[j].mark[2])
				{
					temp_zz = students[i].stu_id;
					students[i].stu_id = students[j].stu_id;
					students[j].stu_id = temp_zz;

					temp_zz = students[i].name;
					students[i].name = students[j].name;
					students[j].name = temp_zz;

					TEMP_ZZ = students[i].mark_total;
					students[i].mark_total = students[j].mark_total;
					students[j].mark_total = TEMP_ZZ;

					for (l = 0; l < 6; l++)
					{
						TEMP_ZZ = students[i].mark[l];
						students[i].mark[l] = students[j].mark[l];
						students[j].mark[l] = TEMP_ZZ;
					}
				}
			}
		}
		//学生筛选[zxy]
		int top = TENG; int bottom = 0;
		int n0[30] = { 0 };
		int z = 0;
		cout << "输入筛选区间[bottom,top]:" << endl;
		cout << "bottom = "; cin >> bottom;
		cout << "top = "; cin >> top;
		for (i = 0; i < num_students; i++) {
			if ((bottom < students[i].mark[2]) && (students[i].mark[2] < top))
			{
				n0[z] = i;
				z++;
			}
		}
		int numcond = z;

		//用户自行创建TXT文件用以输出[wjy]
	ChooseFile06:
		string FilePath06;
		string FileName06;
		OPENFILENAME ofn06 = { 0 };
		TCHAR strFileName06[MAX_PATH] = { 0 };		//用于接收文件名
		ofn06.lStructSize = sizeof(OPENFILENAME);			//结构体大小
		ofn06.hwndOwner = NULL;		//拥有着窗口句柄
		ofn06.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//设置过滤
		ofn06.nFilterIndex = 1;		//过滤器索引
		ofn06.lpstrFile = strFileName06;		//接收返回的文件名，注意第一个字符需要为NULL
		ofn06.nMaxFile = sizeof(strFileName06);		//缓冲区长度
		ofn06.lpstrInitialDir = NULL;			//初始目录为默认
		ofn06.lpstrTitle = TEXT("请选择一个文件");		//窗口标题
		ofn06.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//文件、目录必须存在，隐藏只读选项
		//打开文件对话框
		if (GetOpenFileName(&ofn06)) {
			string filePath = TCHAR2STRING(strFileName06);
			int start = filePath.find_last_of('\\');
			int end = filePath.find_last_of('.');
			string fileName = filePath.substr(start + 1, end - start - 1);
			string exten = filePath.substr(end, filePath.length() - end);
			cout << "您选择的文件路径为:" << filePath << endl;
			cout << "文件名为：" << fileName << "\t\t" << "拓展名为：" << exten << endl;
			FilePath06 = filePath;
			FileName06 = fileName;
			if (exten != ".txt")
			{
				cout << ">>>文件拓展名错误，请重新创建txt成绩表" << endl;
				system("pause");
				goto ChooseFile06;
			}
		}
		else {
			MessageBox(NULL, TEXT("请新建一txt文件"), NULL, MB_ICONERROR);
			goto ChooseFile06;
		}

		//检查文件正常打开[wjy]
		ofile.open(FilePath06);
		if (!ofile.is_open())
		{
			cout << "Could not open the file " << FileName01 << endl;
			cout << "Program terminating.\n";
			exit(EXIT_FAILURE);
		}
		//打印表格[zxy]
		ofile << "以下为按英语筛选的英语降序排序表：" << endl;
		ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "序号 " << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "学号";
		ofile << setw(18) << "姓名";
		ofile << resetiosflags(ios::left) << setiosflags(ios::right);
		ofile << setiosflags(ios::right) << setw(10) << "总分";
		ofile << setiosflags(ios::right) << setw(10) << "语文";
		ofile << setiosflags(ios::right) << setw(10) << "数学";
		ofile << setiosflags(ios::right) << setw(10) << "英语";
		ofile << setiosflags(ios::right) << setw(10) << "物理";
		ofile << setiosflags(ios::right) << setw(10) << "化学";
		ofile << setiosflags(ios::right) << setw(10) << "政治";
		ofile << endl;
		for (i = 0; i < numcond; i++)
		{
			ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setfill('0') << setw(3) << students[n0[i]].stu_num << "  ";
			ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << students[n0[i]].stu_id;
			ofile << setiosflags(ios::left) << setw(18) << students[n0[i]].name;
			ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setw(10) << students[n0[i]].mark_total;
			for (j = 0; j < 6; j++)
				ofile << setw(10) << students[n0[i]].mark[j];
			ofile << endl;
		}
		ofile.close();
	}
	else if (choosecond == 3) {
		cout << "是否对物理进行筛选? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseCond;
		//物理排序[zz]
		for (i = 0; i <= num_students; i++)
		{
			for (j = 0; j < num_students; j++)
			{
				if (students[i].mark[3] >= students[j].mark[3])
				{
					temp_zz = students[i].stu_id;
					students[i].stu_id = students[j].stu_id;
					students[j].stu_id = temp_zz;

					temp_zz = students[i].name;
					students[i].name = students[j].name;
					students[j].name = temp_zz;

					TEMP_ZZ = students[i].mark_total;
					students[i].mark_total = students[j].mark_total;
					students[j].mark_total = TEMP_ZZ;

					for (l = 0; l < 6; l++)
					{
						TEMP_ZZ = students[i].mark[l];
						students[i].mark[l] = students[j].mark[l];
						students[j].mark[l] = TEMP_ZZ;
					}
				}
			}
		}
		//学生筛选[zxy]
		int top = TPHYSICS; int bottom = 0;
		int n0[30] = { 0 };
		int z = 0;
		cout << "输入筛选区间[bottom,top]:" << endl;
		cout << "bottom = "; cin >> bottom;
		cout << "top = "; cin >> top;
		for (i = 0; i < num_students; i++) {
			if ((bottom < students[i].mark[3]) && (students[i].mark[3] < top))
			{
				n0[z] = i;
				z++;
			}
		}
		int numcond = z;

		//用户自行创建TXT文件用以输出[wjy]
	ChooseFile07:
		string FilePath07;
		string FileName07;
		OPENFILENAME ofn07 = { 0 };
		TCHAR strFileName07[MAX_PATH] = { 0 };		//用于接收文件名
		ofn07.lStructSize = sizeof(OPENFILENAME);			//结构体大小
		ofn07.hwndOwner = NULL;		//拥有着窗口句柄
		ofn07.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//设置过滤
		ofn07.nFilterIndex = 1;		//过滤器索引
		ofn07.lpstrFile = strFileName07;		//接收返回的文件名，注意第一个字符需要为NULL
		ofn07.nMaxFile = sizeof(strFileName07);		//缓冲区长度
		ofn07.lpstrInitialDir = NULL;			//初始目录为默认
		ofn07.lpstrTitle = TEXT("请选择一个文件");		//窗口标题
		ofn07.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//文件、目录必须存在，隐藏只读选项
		//打开文件对话框
		if (GetOpenFileName(&ofn07)) {
			string filePath = TCHAR2STRING(strFileName07);
			int start = filePath.find_last_of('\\');
			int end = filePath.find_last_of('.');
			string fileName = filePath.substr(start + 1, end - start - 1);
			string exten = filePath.substr(end, filePath.length() - end);
			cout << "您选择的文件路径为:" << filePath << endl;
			cout << "文件名为：" << fileName << "\t\t" << "拓展名为：" << exten << endl;
			FilePath07 = filePath;
			FileName07 = fileName;
			if (exten != ".txt")
			{
				cout << ">>>文件拓展名错误，请重新创建txt成绩表" << endl;
				system("pause");
				goto ChooseFile07;
			}
		}
		else {
			MessageBox(NULL, TEXT("请新建一txt文件"), NULL, MB_ICONERROR);
			goto ChooseFile07;
		}

		//检查文件正常打开[wjy]
		ofile.open(FilePath07);
		if (!ofile.is_open())
		{
			cout << "Could not open the file " << FileName01 << endl;
			cout << "Program terminating.\n";
			exit(EXIT_FAILURE);
		}
		//表格打印[zxy]
		ofile << "以下为按物理筛选的物理降序排序表：" << endl;
		ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "序号 " << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "学号";
		ofile << setw(18) << "姓名";
		ofile << resetiosflags(ios::left) << setiosflags(ios::right);
		ofile << setiosflags(ios::right) << setw(10) << "总分";
		ofile << setiosflags(ios::right) << setw(10) << "语文";
		ofile << setiosflags(ios::right) << setw(10) << "数学";
		ofile << setiosflags(ios::right) << setw(10) << "英语";
		ofile << setiosflags(ios::right) << setw(10) << "物理";
		ofile << setiosflags(ios::right) << setw(10) << "化学";
		ofile << setiosflags(ios::right) << setw(10) << "政治";
		ofile << endl;
		for (i = 0; i < numcond; i++)
		{
			ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setfill('0') << setw(3) << students[n0[i]].stu_num << "  ";
			ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << students[n0[i]].stu_id;
			ofile << setiosflags(ios::left) << setw(18) << students[n0[i]].name;
			ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setw(10) << students[n0[i]].mark_total;
			for (j = 0; j < 6; j++)
				ofile << setw(10) << students[n0[i]].mark[j];
			ofile << endl;
		}
		ofile.close();
	}

	else if (choosecond == 4) {
		cout << "是否对化学进行筛选? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseCond;
		//化学排序[zz]
		for (i = 0; i <= num_students; i++)
		{
			for (j = 0; j < num_students; j++)
			{
				if (students[i].mark[4] >= students[j].mark[4])
				{
					temp_zz = students[i].stu_id;
					students[i].stu_id = students[j].stu_id;
					students[j].stu_id = temp_zz;

					temp_zz = students[i].name;
					students[i].name = students[j].name;
					students[j].name = temp_zz;

					TEMP_ZZ = students[i].mark_total;
					students[i].mark_total = students[j].mark_total;
					students[j].mark_total = TEMP_ZZ;

					for (l = 0; l < 6; l++)
					{
						TEMP_ZZ = students[i].mark[l];
						students[i].mark[l] = students[j].mark[l];
						students[j].mark[l] = TEMP_ZZ;
					}
				}
			}
		}
		//学生选择[zxy]
		int top = TCHEMISTRY; int bottom = 0;
		int n0[30] = { 0 };
		int z = 0;
		cout << "输入筛选区间[bottom,top]:" << endl;
		cout << "bottom = "; cin >> bottom;
		cout << "top = "; cin >> top;
		for (i = 0; i < num_students; i++) {
			if ((bottom < students[i].mark[4]) && (students[i].mark[4] < top))
			{
				n0[z] = i;
				z++;
			}
		}
		int numcond = z;

		//用户自行创建TXT文件用以输出[wjy]
	ChooseFile08:
		string FilePath08;
		string FileName08;
		OPENFILENAME ofn08 = { 0 };
		TCHAR strFileName08[MAX_PATH] = { 0 };		//用于接收文件名
		ofn08.lStructSize = sizeof(OPENFILENAME);			//结构体大小
		ofn08.hwndOwner = NULL;		//拥有着窗口句柄
		ofn08.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//设置过滤
		ofn08.nFilterIndex = 1;		//过滤器索引
		ofn08.lpstrFile = strFileName08;		//接收返回的文件名，注意第一个字符需要为NULL
		ofn08.nMaxFile = sizeof(strFileName08);		//缓冲区长度
		ofn08.lpstrInitialDir = NULL;			//初始目录为默认
		ofn08.lpstrTitle = TEXT("请选择一个文件");		//窗口标题
		ofn08.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//文件、目录必须存在，隐藏只读选项
		//打开文件对话框
		if (GetOpenFileName(&ofn08)) {
			string filePath = TCHAR2STRING(strFileName08);
			int start = filePath.find_last_of('\\');
			int end = filePath.find_last_of('.');
			string fileName = filePath.substr(start + 1, end - start - 1);
			string exten = filePath.substr(end, filePath.length() - end);
			cout << "您选择的文件路径为:" << filePath << endl;
			cout << "文件名为：" << fileName << "\t\t" << "拓展名为：" << exten << endl;
			FilePath08 = filePath;
			FileName08 = fileName;
			if (exten != ".txt")
			{
				cout << ">>>文件拓展名错误，请重新创建txt成绩表" << endl;
				system("pause");
				goto ChooseFile08;
			}
		}
		else {
			MessageBox(NULL, TEXT("请新建一txt文件"), NULL, MB_ICONERROR);
			goto ChooseFile08;
		}

		//检查文件正常打开[wjy]
		ofile.open(FilePath08);
		if (!ofile.is_open())
		{
			cout << "Could not open the file " << FileName01 << endl;
			cout << "Program terminating.\n";
			exit(EXIT_FAILURE);
		}
		//表格打印[zxy]
		ofile << "以下为按化学筛选的化学降序排序表：" << endl;
		ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "序号 " << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "学号";
		ofile << setw(18) << "姓名";
		ofile << resetiosflags(ios::left) << setiosflags(ios::right);
		ofile << setiosflags(ios::right) << setw(10) << "总分";
		ofile << setiosflags(ios::right) << setw(10) << "语文";
		ofile << setiosflags(ios::right) << setw(10) << "数学";
		ofile << setiosflags(ios::right) << setw(10) << "英语";
		ofile << setiosflags(ios::right) << setw(10) << "物理";
		ofile << setiosflags(ios::right) << setw(10) << "化学";
		ofile << setiosflags(ios::right) << setw(10) << "政治";
		ofile << endl;
		for (i = 0; i < numcond; i++)
		{
			ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setfill('0') << setw(3) << students[n0[i]].stu_num << "  ";
			ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << students[n0[i]].stu_id;
			ofile << setiosflags(ios::left) << setw(18) << students[n0[i]].name;
			ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setw(10) << students[n0[i]].mark_total;
			for (j = 0; j < 6; j++)
				ofile << setw(10) << students[n0[i]].mark[j];
			ofile << endl;
		}
		ofile.close();
	}

	else if (choosecond == 5) {
		cout << "是否对政治进行筛选? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseCond;
		//政治排序[zz]
		for (i = 0; i <= num_students; i++)
		{
			for (j = 0; j < num_students; j++)
			{
				if (students[i].mark[5] >= students[j].mark[5])
				{
					temp_zz = students[i].stu_id;
					students[i].stu_id = students[j].stu_id;
					students[j].stu_id = temp_zz;

					temp_zz = students[i].name;
					students[i].name = students[j].name;
					students[j].name = temp_zz;

					TEMP_ZZ = students[i].mark_total;
					students[i].mark_total = students[j].mark_total;
					students[j].mark_total = TEMP_ZZ;

					for (l = 0; l < 6; l++)
					{
						TEMP_ZZ = students[i].mark[l];
						students[i].mark[l] = students[j].mark[l];
						students[j].mark[l] = TEMP_ZZ;
					}
				}
			}
		}
		//学生筛选[zxy]
		int top = TPOLITICS; int bottom = 0;
		int n0[30] = { 0 };
		int z = 0;
		cout << "输入筛选区间[bottom,top]:" << endl;
		cout << "bottom = "; cin >> bottom;
		cout << "top = "; cin >> top;
		for (i = 0; i < num_students; i++) {
			if ((bottom < students[i].mark[5]) && (students[i].mark[5] < top))
			{
				n0[z] = i;
				z++;
			}
		}
		int numcond = z;

		//用户自行创建TXT文件用以输出[wjy]
	ChooseFile09:
		string FilePath09;
		string FileName09;
		OPENFILENAME ofn09 = { 0 };
		TCHAR strFileName09[MAX_PATH] = { 0 };		//用于接收文件名
		ofn09.lStructSize = sizeof(OPENFILENAME);			//结构体大小
		ofn09.hwndOwner = NULL;		//拥有着窗口句柄
		ofn09.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//设置过滤
		ofn09.nFilterIndex = 1;		//过滤器索引
		ofn09.lpstrFile = strFileName09;		//接收返回的文件名，注意第一个字符需要为NULL
		ofn09.nMaxFile = sizeof(strFileName09);		//缓冲区长度
		ofn09.lpstrInitialDir = NULL;			//初始目录为默认
		ofn09.lpstrTitle = TEXT("请选择一个文件");		//窗口标题
		ofn09.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//文件、目录必须存在，隐藏只读选项
		//打开文件对话框
		if (GetOpenFileName(&ofn09)) {
			string filePath = TCHAR2STRING(strFileName09);
			int start = filePath.find_last_of('\\');
			int end = filePath.find_last_of('.');
			string fileName = filePath.substr(start + 1, end - start - 1);
			string exten = filePath.substr(end, filePath.length() - end);
			cout << "您选择的文件路径为:" << filePath << endl;
			cout << "文件名为：" << fileName << "\t\t" << "拓展名为：" << exten << endl;
			FilePath09 = filePath;
			FileName09 = fileName;
			if (exten != ".txt")
			{
				cout << ">>>文件拓展名错误，请重新创建txt成绩表" << endl;
				system("pause");
				goto ChooseFile09;
			}
		}
		else {
			MessageBox(NULL, TEXT("请新建一txt文件"), NULL, MB_ICONERROR);
			goto ChooseFile09;
		}

		//检查文件正常打开[wjy]
		ofile.open(FilePath09);
		if (!ofile.is_open())
		{
			cout << "Could not open the file " << FileName01 << endl;
			cout << "Program terminating.\n";
			exit(EXIT_FAILURE);
		}
		//表格打印[zxy]
		ofile << "以下为按政治筛选的政治降序排序表：" << endl;
		ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "序号 " << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "学号";
		ofile << setw(18) << "姓名";
		ofile << resetiosflags(ios::left) << setiosflags(ios::right);
		ofile << setiosflags(ios::right) << setw(10) << "总分";
		ofile << setiosflags(ios::right) << setw(10) << "语文";
		ofile << setiosflags(ios::right) << setw(10) << "数学";
		ofile << setiosflags(ios::right) << setw(10) << "英语";
		ofile << setiosflags(ios::right) << setw(10) << "物理";
		ofile << setiosflags(ios::right) << setw(10) << "化学";
		ofile << setiosflags(ios::right) << setw(10) << "政治";
		ofile << endl;
		for (i = 0; i < numcond; i++)
		{
			ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setfill('0') << setw(3) << students[n0[i]].stu_num << "  ";
			ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << students[n0[i]].stu_id;
			ofile << setiosflags(ios::left) << setw(18) << students[n0[i]].name;
			ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setw(10) << students[n0[i]].mark_total;
			for (j = 0; j < 6; j++)
				ofile << setw(10) << students[n0[i]].mark[j];
			ofile << endl;
		}
		ofile.close();
	}

	else if (choosecond == 6) {
		cout << "是否对总分进行筛选? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseCond;
		//总分排序[zz]
		for (i = 0; i <= num_students; i++)
		{
			for (j = 0; j < num_students; j++)
			{
				if (students[i].mark_total >= students[j].mark_total)
				{
					temp_zz = students[i].stu_id;
					students[i].stu_id = students[j].stu_id;
					students[j].stu_id = temp_zz;

					temp_zz = students[i].name;
					students[i].name = students[j].name;
					students[j].name = temp_zz;

					TEMP_ZZ = students[i].mark_total;
					students[i].mark_total = students[j].mark_total;
					students[j].mark_total = TEMP_ZZ;

					for (l = 0; l < 6; l++)
					{
						TEMP_ZZ = students[i].mark[l];
						students[i].mark[l] = students[j].mark[l];
						students[j].mark[l] = TEMP_ZZ;
					}
				}
			}
		}
		//学生筛选[zxy]
		int top = 760; int bottom = 0;
		int n0[30] = { 0 };
		int z = 0;
		cout << "输入筛选区间[bottom,top]:" << endl;
		cout << "bottom = "; cin >> bottom;
		cout << "top = "; cin >> top;
		for (i = 0; i < num_students; i++) {
			if ((bottom < students[i].mark_total) && (students[i].mark_total < top))
			{
				n0[z] = i;
				z++;
			}
		}
		int num6 = z;

		//用户自行创建TXT文件用以输出[wjy]
	ChooseFile10:
		string FilePath10;
		string FileName10;
		OPENFILENAME ofn10 = { 0 };
		TCHAR strFileName10[MAX_PATH] = { 0 };		//用于接收文件名
		ofn10.lStructSize = sizeof(OPENFILENAME);			//结构体大小
		ofn10.hwndOwner = NULL;		//拥有着窗口句柄
		ofn10.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//设置过滤
		ofn10.nFilterIndex = 1;		//过滤器索引
		ofn10.lpstrFile = strFileName10;		//接收返回的文件名，注意第一个字符需要为NULL
		ofn10.nMaxFile = sizeof(strFileName10);		//缓冲区长度
		ofn10.lpstrInitialDir = NULL;			//初始目录为默认
		ofn10.lpstrTitle = TEXT("请选择一个文件");		//窗口标题
		ofn10.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//文件、目录必须存在，隐藏只读选项
		//打开文件对话框
		if (GetOpenFileName(&ofn10)) {
			string filePath = TCHAR2STRING(strFileName10);
			int start = filePath.find_last_of('\\');
			int end = filePath.find_last_of('.');
			string fileName = filePath.substr(start + 1, end - start - 1);
			string exten = filePath.substr(end, filePath.length() - end);
			cout << "您选择的文件路径为:" << filePath << endl;
			cout << "文件名为：" << fileName << "\t\t" << "拓展名为：" << exten << endl;
			FilePath10 = filePath;
			FileName10 = fileName;
			if (exten != ".txt")
			{
				cout << ">>>文件拓展名错误，请重新创建txt成绩表" << endl;
				system("pause");
				goto ChooseFile10;
			}
		}
		else {
			MessageBox(NULL, TEXT("请新建一txt文件"), NULL, MB_ICONERROR);
			goto ChooseFile10;
		}

		//检查文件正常打开[wjy]
		ofile.open(FilePath10);
		if (!ofile.is_open())
		{
			cout << "Could not open the file " << FileName01 << endl;
			cout << "Program terminating.\n";
			exit(EXIT_FAILURE);
		}
		//表格打印[zxy]
		ofile << "以下为按该总分筛选总分降序排序表：" << endl;
		ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "序号 " << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "学号";
		ofile << setw(18) << "姓名";
		ofile << resetiosflags(ios::left) << setiosflags(ios::right);
		ofile << setiosflags(ios::right) << setw(10) << "总分";
		ofile << setiosflags(ios::right) << setw(10) << "语文";
		ofile << setiosflags(ios::right) << setw(10) << "数学";
		ofile << setiosflags(ios::right) << setw(10) << "英语";
		ofile << setiosflags(ios::right) << setw(10) << "物理";
		ofile << setiosflags(ios::right) << setw(10) << "化学";
		ofile << setiosflags(ios::right) << setw(10) << "政治";
		ofile << endl;
		for (i = 0; i < num6; i++)
		{
			ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setfill('0') << setw(3) << students[n0[i]].stu_num << "  ";
			ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << students[n0[i]].stu_id;
			ofile << setiosflags(ios::left) << setw(18) << students[n0[i]].name;
			ofile << resetiosflags(ios::left) << setiosflags(ios::right) << setw(10) << students[n0[i]].mark_total;
			for (j = 0; j < 6; j++)
				ofile << setw(10) << students[n0[i]].mark[j];
			ofile << endl;
		}
		ofile.close();
	}
	else {
		cout << "输入数字无效，请重新输入（如：1）" << endl;
		goto ChooseCond;
	}

	//过渡框架[wjy]
	cout << "输出成功。" << endl << endl;
	cout << "是否继续筛选？<Y/N>";
	cin >> anw;
	if (anw == 'Y')
		goto ChooseCond;
	cout << endl;
	cout << "筛选后总分降序排序表全部生成完毕，" << endl;
	cout << "是否清屏? <Y/N>\n>>>";
	cin >> clear;
	if (clear == 'Y') system("cls");

	//输出符合条件的按总分降序的排序表[wjy zxy zz如上]
	cout << "符合指定条件的按总分降序排序的成绩表已生成" << endl;
	cout << "按任意键输出六科均分柱状统计图并判断班级短板……" << endl;
	system("pause");
	cout << endl;

	//输出各科均分柱状统计图[csj]
	double arr[6] = { 0 };//某一门所有学生的分数之和
	int k = 0;
	do {
		double* arr_temp = new double[num_students];
		int i;
		for (i = 0; i < num_students; i++) {
			arr_temp[i] = students[i].mark[k];
		}
		arr[k] = arr_sum(arr_temp, num_students);
		k++;
	} while (k < 6);
	double score_percent[6] = { 0 };//每一学科的平均得分率
	double arr_temp[6] = { 0 };
	score_percent[0] = arr[0] / num_students / TCHINESE;
	score_percent[1] = arr[1] / num_students / TMATH;
	score_percent[2] = arr[2] / num_students / TENG;
	score_percent[3] = arr[3] / num_students / TPHYSICS;
	score_percent[4] = arr[4] / num_students / TCHEMISTRY;
	score_percent[5] = arr[5] / num_students / TPOLITICS;

	print_chartofaverage(arr, num_students);

	//判断班级短板(得分百分比最低两科)[csj]
	for (int i = 0; i < 6; i++) {
		arr_temp[i] = score_percent[i];
	}
	//复制每一学科的平均得分率，以便在排序后进行前后比对，得出得分率最低的两门学科[csj]
	bubble_sort(arr_temp, 6);
	int low_1 = 0, low_2 = 0;
	for (int i = 0; i < 6; i++) {
		if (score_percent[i] == arr_temp[0]) {
			low_1 = i + 1;
		}
		else if (score_percent[i] == arr_temp[1]) {
			low_2 = i + 1;
		}
		else {
		}
	}
	cout << "班级短板为：" << endl;
	switch (low_1) {
	case 1:cout << "\t语文得分率最低" << endl; break;
	case 2:cout << "\t数学得分率最低" << endl; break;
	case 3:cout << "\t英语得分率最低" << endl; break;
	case 4:cout << "\t物理得分率最低" << endl; break;
	case 5:cout << "\t化学得分率最低" << endl; break;
	case 6:cout << "\t政治得分率最低" << endl; break;
	default:break;
	}
	switch (low_2) {
	case 1:cout << "\t语文得分率第二低" << endl; break;
	case 2:cout << "\t数学得分率第二低" << endl; break;
	case 3:cout << "\t英语得分率第二低" << endl; break;
	case 4:cout << "\t物理得分率第二低" << endl; break;
	case 5:cout << "\t化学得分率第二低" << endl; break;
	case 6:cout << "\t政治得分率第二低" << endl; break;
	default:break;
	}

	//班级成绩分析铺垫[csj]
#ifdef MAILING
	string worstsubject;
	string worst2subject;
	switch (low_1) {
	case 1:worstsubject = "\t语文得分率最低\n"; break;
	case 2:worstsubject = "\t数学得分率最低\n"; break;
	case 3:worstsubject = "\t英语得分率最低\n"; break;
	case 4:worstsubject = "\t物理得分率最低\n"; break;
	case 5:worstsubject = "\t化学得分率最低\n"; break;
	case 6:worstsubject = "\t政治得分率最低\n"; break;
	default:break;
	}
	switch (low_2) {
	case 1:worst2subject = "\t语文得分率第二低\n"; break;
	case 2:worst2subject = "\t数学得分率第二低\n"; break;
	case 3:worst2subject = "\t英语得分率第二低\n"; break;
	case 4:worst2subject = "\t物理得分率第二低\n"; break;
	case 5:worst2subject = "\t化学得分率第二低\n"; break;
	case 6:worst2subject = "\t政治得分率第二低\n"; break;
	default:break;
	}
#endif

	//每门课成绩分段统计&统计图生成[csj]
	cout << endl << "各科均分柱状统计图生成完毕，按任意键生成单科分段统计图……" << endl;
	cout << "认为得分率在85%以上为优秀，70-85%为良好，60-70%为合格，在60%以下为不合格" << endl;
	int temp_csj = 0;
	//a,b,c,d依次分别对应优秀，良好，合格，不合格；[csj]
	cout << "科目" << setiosflags(ios::right) << setw(11) << " ";
	cout << setw(15) << "优秀" << setw(15) << "良好" << setw(15) << "合格" << setw(15) << "不合格" << endl;
	int arr01[6][4] = { 0 };
	do
	{
		int a = 0, b = 0, c = 0, d = 0;
		int k;
		for (k = 0; k < num_students; k++)
		{
			if (temp_csj == 1 || temp_csj == 2 || temp_csj == 0)
			{
				if (students[k].mark[temp_csj] < 90)
					d++;
				else if ((90 <= students[k].mark[temp_csj]) && (students[k].mark[temp_csj] < 105))
					c++;
				else if ((105 <= students[k].mark[temp_csj]) && (students[k].mark[temp_csj] < 127.5))
					b++;
				else if (students[k].mark[temp_csj] >= 127.5)
					a++;
			}
			else if (temp_csj == 3)
			{
				if (students[k].mark[temp_csj] < 66)
					d++;
				else if ((66 <= students[k].mark[temp_csj]) && (students[k].mark[temp_csj] < 77))
					c++;
				else if ((77 <= students[k].mark[temp_csj]) && (students[k].mark[temp_csj] < 93.5))
					b++;
				else if (students[k].mark[temp_csj] >= 93.5)
					a++;

			}
			else if (temp_csj == 4 || temp_csj == 5)
			{
				if (students[k].mark[temp_csj] < 60)
					d++;
				else if ((60 <= students[k].mark[temp_csj]) && (students[k].mark[temp_csj] < 70))
					c++;
				else if ((70 <= students[k].mark[temp_csj]) && (students[k].mark[temp_csj] < 85))
					b++;
				else if (students[k].mark[temp_csj] >= 85)
					a++;

			}

		}
		arr01[temp_csj][0] = a;
		arr01[temp_csj][1] = b;
		arr01[temp_csj][2] = c;
		arr01[temp_csj][3] = d;
		cout << resetiosflags(ios::right) << setiosflags(ios::left);
		switch (temp_csj)
		{
		case 0:cout << setw(15) << "Chinese"; break;
		case 1:cout << setw(15) << "Maths"; break;
		case 2:cout << setw(15) << "English"; break;
		case 3:cout << setw(15) << "Physics"; break;
		case 4:cout << setw(15) << "Chemistry"; break;
		case 5:cout << setw(15) << "Politics"; break;
		}
		cout << resetiosflags(ios::left) << setiosflags(ios::right)
			<< setw(15) << a << setw(15) << b << setw(15) << c << setw(15) << d << endl;
		++temp_csj;
	} while (temp_csj < 6);
	//生成每门课成绩分段统计柱状图+饼图
	int aa;
	cout << "输入1-6，选择你要生成分段成绩统计图的科目：" << endl;
	cin >> aa;
	print_fenduan(aa - 1, arr01);

	//过渡[wjy]
	cout << endl << "单科分段统计柱状图生成完毕" << endl;
	cout << "按任意键将个人成绩发送至邮箱并分析偏科……" << endl;
	system("pause");
	cout << endl;

	//偏科分析(使用极差实现)[csj]
	for (i = 0; i < num_students; i++)
	{
		double arr[6] = { 0 };
		for (int j = 0; j < 6; j++)
		{
			arr[j] = students[i].mark[j];
		}
		bubble_sort(arr, 6);
		if ((arr[5] - arr[0]) > 70)
		{
			students[i].is_lean = 1;
			cout << resetiosflags(ios::right) << setiosflags(ios::left) << setw(25) << students[i].name << "偏科" << endl;
		}
	}

	//教师邮件发送准备[wjy]
#ifdef MAILING
	cout << "请输入教师邮箱(如mushroom@seu.edu.cn)" << endl << ">>>";
	string teachermail;
	cin >> teachermail;
#endif
	cout << "按任意键发送邮件。" << endl;
	system("pause");
	//邮件发送[wjy]
#ifdef MAILING
	//创建string结构体便于输出正文分数
	system("cls");
	system("color 0A");

	struct Studentstr
	{
		string chinese;
		string math;
		string english;
		string physics;
		string chemistry;
		string politics;
		string mark_total = "0";	//学生总分
		string mark_ave = "0";	//学生均分
		string is_lean = "0";		//判断是否偏科
	};
	Studentstr* stu_str = new Studentstr[num_students];
	//数据double转string
	for (i = 0; i < num_students; i++)
	{
		stu_str[i].mark_total = doubleToString(students[i].mark_total);
		stu_str[i].mark_ave = doubleToString(students[i].mark_ave);
		if (students[i].is_lean == 0) stu_str[i].is_lean = "不偏科";
		if (students[i].is_lean == 1) stu_str[i].is_lean = "偏科";
		stu_str[i].chinese = doubleToString(students[i].mark[Chinese]);
		stu_str[i].math = doubleToString(students[i].mark[Math]);
		stu_str[i].english = doubleToString(students[i].mark[Eng]);
		stu_str[i].physics = doubleToString(students[i].mark[Physics]);
		stu_str[i].chemistry = doubleToString(students[i].mark[Chemistry]);
		stu_str[i].politics = doubleToString(students[i].mark[Politics]);
	}
	string* studentid = new string[num_students];
	string* studentmail = new string[num_students];
	for (i = 0; i < num_students; i++)
	{
		studentid[i] = students[i].stu_id + "@seu.edu.cn";
		studentmail[i] =
			(string)"姓名：" + students[i].name +
			(string)"\n总分：" + stu_str[i].mark_total +
			(string)"\n语文：" + stu_str[i].chinese +
			(string)"\n数学：" + stu_str[i].math +
			(string)"\n英语：" + stu_str[i].english +
			(string)"\n物理：" + stu_str[i].physics +
			(string)"\n化学：" + stu_str[i].chemistry +
			(string)"\n政治：" + stu_str[i].politics +
			(string)"\n" + stu_str[i].is_lean;
		CSmtp smtp(
			25,									/*smtp端口*/
			"smtp.163.com",						/*smtp服务器地址*/
			"xxx",	        /*你的163邮箱地址*/
			"xxx",				    /*163邮箱专属授权密码*/
			studentid[i],					    /*目的邮箱地址*/
			"mushroom成绩分析系统邮件发送-学生端",	/*主题*/
			studentmail[i]		                /*邮件正文*/
		);
		int err;
		if ((err = smtp.SendEmail_Ex()) != 0)
		{
			if (err == 1)
				cout << "错误1: 由于网络不畅通，发送失败!" << endl;
			if (err == 2)
				cout << "错误2: 用户名错误,请核对!" << endl;
			if (err == 3)
				cout << "错误3: 用户密码错误，请核对!" << endl;
			if (err == 4)
				cout << "错误4: 请检查附件目录是否正确，以及文件是否存在!" << endl;
		}
	}

	//给教师发送班级整体成绩概况[wjy]
	string tercher_content =
		(string)"本班级成绩概况如下：\n" +
		(string)"班级最高分为:" + highestname + (string)":" + highest + (string)"分。\n" +
		(string)"班级最低分为:" + lowestname + (string)":" + lowest + (string)"分。\n" +
		(string)"班级短板为:\n" + worstsubject + worst2subject
		;
	CSmtp smtp(
		25,									/*smtp端口*/
		"smtp.163.com",						/*smtp服务器地址*/
		"xxx",	        /*你的邮箱地址*/
		"xxx",				    /*163邮箱专属授权密码*/
		teachermail,					    /*目的邮箱地址*/
		"mushroom成绩分析系统邮件发送-教师端",	/*主题*/
		tercher_content		                /*邮件正文*/
	);
	int err;
	if ((err = smtp.SendEmail_Ex()) != 0)
	{
		if (err == 1)
			cout << "错误1: 由于网络不畅通，发送失败!" << endl;
		if (err == 2)
			cout << "错误2: 用户名错误,请核对!" << endl;
		if (err == 3)
			cout << "错误3: 用户密码错误，请核对!" << endl;
		if (err == 4)
			cout << "错误4: 请检查附件目录是否正确，以及文件是否存在!" << endl;
	}
#endif

	cout << endl << "按任意键退出程序……" << endl;
	system("pause");

	infile.close();
	ofile.close();
#ifdef MAILING
	delete[] stu_str;
	delete[] studentmail;
	delete[] studentid;
#endif
	delete[] students;
	return EXIT_SUCCESS;
}

/*****************************************************************************************/

//程序初始化设置[wjy]
void init_prog(void)
{
	system("title 班级成绩分析系统   By:王骏宇 柴世杰 赵喆 朱晓玥  V2.1.1");
	system("mode con:cols=140 lines=50");
	system("chcp 936");
	system("color 0B");
	system("cls");
	cout << "Performance analysis system.\tBy:Hello Software Group." << endl;
	cout << "Framework construction:王骏宇." << endl;
	cout << "Programming:王骏宇 赵喆 柴世杰 朱晓玥.\n\n";
	cout << "----------------------------------------------" << endl;
}

void Mushstrcpy(char* str01, char* str02)		//自定义的strcpy功能的函数，字符串赋值[wjy]
{
	int i;
	for (i = 0; str02[i] != '\0'; i++)
		str01[i] = str02[i];
	str01[i] = '\0';
}

char* Mushstrcat(char* str_01, char* str_02)	//自定义的strcat功能的函数，字符串拼接[wjy]
{
	char* Return = str_01;
	while (*str_01)++str_01;
	while (*str_02)
	{
		*str_01 = *str_02;
		str_01++;
		str_02++;
	}
	*str_01 = *str_02;
	return Return;
}

//tchar转string[源https://blog.csdn.net/skye1221/article/details/85243465]
//Function:        TCHAR2STRING
//Description:     TCHAR转string
//Input:           str:待转化的TCHAR*类型字符串
//Return:          转化后的string类型字符串
string TCHAR2STRING(TCHAR* str) {
	std::string strstr;
	try
	{
		int iLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
		char* chRtn = new char[iLen * sizeof(char)];
		WideCharToMultiByte(CP_ACP, 0, str, -1, chRtn, iLen, NULL, NULL);
		strstr = chRtn;
	}
	catch (exception e) {}
	return strstr;
}

#ifdef MAILING
//double转string：https ://blog.csdn.net/qq_33236491/article/details/108321230
std::string doubleToString(const double& val)
{
	char* chCode;
	chCode = new char[20];
	sprintf(chCode, "%.2lf", val);
	std::string str(chCode);
	delete[]chCode;
	return str;
}
//版权声明：本文为CSDN博主「Joyce_Chan丶」的原创文章，遵循CC 4.0 BY - SA版权协议，转载请附上原文出处链接及本声明。
#endif

//某冒泡排序[csj]
void bubble_sort(double arr[], int num) {
	int i, j;
	for (i = 0; i < num - 1; i++)
	{
		for (j = 0; j < num - i - 1; j++)
		{
			double temp = 0;
			if (arr[j] > arr[j + 1])
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}//升序冒泡

//数组求和[csj]
double arr_sum(double arr[], int num)
{
	double sum = 0;
	int i;
	for (i = 0; i < num; i++)
		sum += arr[i];
	return sum;
}

//[csj]
void print_chartofaverage(double* arr01, int num) {
	initgraph(640, 480);
	setbkcolor(WHITE);
	cleardevice();
	settextcolor(WHITE);
	int i;
	for (i = 0; i < 6; i++) {
		setlinecolor(RGB(255 - 40 * i, 10 + 30 * i, 150));
		fillrectangle(50 + 60 * i, 150 - arr01[i] / num, 80 + 60 * i, 300);
		settextcolor(BLACK);
		switch (i) {
		case 0:outtextxy(50 + 60 * i, 310, L"语文"); break;
		case 1:outtextxy(50 + 60 * i, 310, L"数学"); break;
		case 2:outtextxy(50 + 60 * i, 310, L"英语"); break;
		case 3:outtextxy(50 + 60 * i, 310, L"物理"); break;
		case 4:outtextxy(50 + 60 * i, 310, L"化学"); break;
		case 5:outtextxy(50 + 60 * i, 310, L"政治"); break;
		default:break;
		}
	}
	Sleep(3000);
	closegraph();
	system("pause");
}

//sub是科目的序号，二维数组的每一行代表某一科目的分段人数[csj]
void print_fenduan(int sub, int arr[6][4]) {
	initgraph(640, 480);
	setbkcolor(WHITE);
	cleardevice();
	settextcolor(BLACK);
	setlinecolor(RGB(255, 150, 150));
	switch (sub) {
	case 0:outtextxy(10, 10, L"语文"); break;
	case 1:outtextxy(10, 10, L"数学"); break;
	case 2:outtextxy(10, 10, L"英语"); break;
	case 3:outtextxy(10, 10, L"物理"); break;
	case 4:outtextxy(10, 10, L"化学"); break;
	case 5:outtextxy(10, 10, L"政治"); break;
	default:break;
	}
	int i;
	double start = 0, end = 0;
	for (i = 0; i < 4; i++) {
		setlinecolor(RGB(100 + 30 * i, 200 - 40 * i, 150 + 10 * i));
		fillrectangle(50 + 60 * i, 300 - arr[sub][i] * 10, 80 + 60 * i, 300);
		end += angle(arr[sub][i], 30);
		fillpie(350, 200, 500, 350, start, end);
		start = end;
		settextcolor(BLACK);
		switch (i) {
		case 0:outtextxy(50 + 60 * i, 330, L"优秀"); break;
		case 1:outtextxy(50 + 60 * i, 330, L"良好"); break;
		case 2:outtextxy(50 + 60 * i, 330, L"合格"); break;
		case 3:outtextxy(50 + 60 * i, 330, L"不合格"); break;
		default:break;
		}
	}
	Sleep(3000);
	closegraph();
	system("pause");
}

//[csj]
double angle(int num, int sum)
{
	double back = 0;
	back = num * 2 * PI / sum;
	return back;
}