//����			�༶�ɼ�����ϵͳ
//���ߣ�			������ �Ԇ� ������ �����h
//ʱ�䣺			2022ѧ���＾

//������ͳ�ƣ�	2372-401(*:��չ����Ϊ��)
//�����		836*	
//�Ԇ���			530	
//�����ܣ�		303*	
//�����h��		302	
//CSDN�����ã�	401

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4267)
#pragma warning(disable:4244)
#include "fismtp.h"
#include<Windows.h>		
#include<commdlg.h>		//ofnѡ��Ի�����
#include <fstream>
#include <iomanip>
#include <sstream>		//doubleתstring��
#include <cctype>
#include <cmath>
#include<graphics.h>
#include <ctime>
#define TESTING
#define MAILING
using namespace std;

//������������������[wjy]
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
	int stu_num;			//ѧ�����
	string stu_id;			//ѧ��ѧ��
	string name;			//ѧ������
	double mark[6] = { 0 };	//���Ʒ���
	double mark_total = 0;	//ѧ���ܷ�
	double mark_ave = 0;	//ѧ������
	bool is_lean = 0;		//�ж��Ƿ�ƫ��
};


void init_prog(void);		//�����ʼ������[wjy]
void Mushstrcpy(char* str01, char* str02);		//�Զ����strcpy���ܵĺ������ַ�����ֵ[wjy]
char* Mushstrcat(char* str_01, char* str_02);	//�Զ����strcat���ܵĺ������ַ���ƴ��[wjy]
//tcharתstring Դhttps://blog.csdn.net/skye1221/article/details/85243465
string TCHAR2STRING(TCHAR* str);
#ifdef MAILING
//doubleתstring Դhttps ://blog.csdn.net/qq_33236491/article/details/108321230
std::string doubleToString(const double& val);
#endif
void bubble_sort(double arr[], int num);	//ĳð������[csj]
double arr_sum(double arr[], int num);		//�������[csj]
void print_chartofaverage(double* arr, int num);//[csj]
void print_fenduan(int sub, int arr[6][4]);//�ֶ�ͳ����״ͼ[csj]
double angle(int num, int sum);//[csj]
#define PI 3.1416

int main()
{
	init_prog();		//����򵥳�ʼ��[wjy]

	//����Ϊ�ļ�ѡ����ӻ����֣���ȡ�ļ�·�����ļ�������ʹ��
	//�õ�string����FilePath��FileName
	//[���https://blog.csdn.net/skye1221/article/details/85243465��wjy�޸��߼�]
	cout << "���������ʼ����";
	system("pause");
	system("cls");
	system("color 0F");
	char w01[100] = "Initial";
	char w02[100] = "izing......";
	char w05[100] = { 0 };
	char w06[100] = { 0 };
	char* w03 = w05;
	char* w04 = w06;
	Mushstrcpy(w04, w03);			//һЩ�Զ��庯����ʹ��
	w04 = Mushstrcat(w01, w02);		//����ײ㺯�������ˡ���
	cout << w04 << endl;
	Sleep(500);
	cout << endl;

	cout << "���������ʼѡ���ļ�����" << endl;
	system("pause");

	cout << endl;
ChooseFile01:
	string FilePath01;			//����������string����洢·��
	string FileName01;			//����������string����洢����
	OPENFILENAME ofn01 = { 0 };
	TCHAR strFileName01[MAX_PATH] = { 0 };		//���ڽ����ļ���
	ofn01.lStructSize = sizeof(OPENFILENAME);			//�ṹ���С
	ofn01.hwndOwner = NULL;		//ӵ���Ŵ��ھ��
	ofn01.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//���ù���
	ofn01.nFilterIndex = 1;		//����������
	ofn01.lpstrFile = strFileName01;		//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
	ofn01.nMaxFile = sizeof(strFileName01);		//����������
	ofn01.lpstrInitialDir = NULL;			//��ʼĿ¼ΪĬ��
	ofn01.lpstrTitle = TEXT("��ѡ��һ���ļ�");		//���ڱ���
	ofn01.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
	//���ļ��Ի���
	if (GetOpenFileName(&ofn01)) {
		string filePath = TCHAR2STRING(strFileName01);
		int start = filePath.find_last_of('\\');
		int end = filePath.find_last_of('.');
		string fileName = filePath.substr(start + 1, end - start - 1);
		string exten = filePath.substr(end, filePath.length() - end);
		cout << "��ѡ����ļ�·��Ϊ:" << filePath << endl;
		cout << "�ļ���Ϊ��" << fileName << "\t\t" << "��չ��Ϊ��" << exten << endl;
		FilePath01 = filePath;
		FileName01 = fileName;
		if (exten != ".txt")		//��չ������ʱ���м��
		{
			cout << ">>>�ļ���չ������������ѡ��txt�ɼ���" << endl;
			system("pause");
			goto ChooseFile01;
		}
	}
	else {
		MessageBox(NULL, TEXT("��ѡ��һ�ļ�"), NULL, MB_ICONERROR);
		goto ChooseFile01;
	}

	//�򿪳ɼ����ļ�(�����Դ˶ο��������ļ�д��)[wjy]
	ifstream infile;
	ofstream ofile;
	infile.open(FilePath01);
	//����ļ��Ƿ���ȷ��[wjy]
	if (!infile.is_open())
	{
		cout << "Could not open the file " << FileName01 << endl;
		cout << "Program terminating.\n";
		exit(EXIT_FAILURE);
	}

	//д��ѧ���ɼ����ݵ��ṹ������[csj&wjy]
	string rubbishbin = "abab";
	int i = 0, j = 0;
	int num_students = 0;
	infile >> num_students;
	cout << "ѧ������Ϊ��" << num_students << " ��\nѧ���б�Ϊ��" << endl;
	Student* students = new Student[num_students];
	//��ȡ��ͷ��������ͷ�������ź���д��[csj&wjy]
	do {
		infile >> rubbishbin;
	} while (rubbishbin != "Politics");
	//��ȡ��������[csj&wjy]
	for (i = 0; i < num_students; i++)
	{
		infile >> students[i].stu_num;
		infile >> students[i].stu_id;
		infile >> students[i].name;
		for (j = 0; j < 6; j++)
			infile >> students[i].mark[j];
	}
	infile.close();
	//����չʾѧ�����ݣ�ʹ��iomanip�Ż���ʽ����[wjy]
	cout.setf(ios_base::fixed, ios_base::floatfield);
	cout.precision(1);
	cout << setiosflags(ios::left) << setw(6) << "���";
	cout << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "ѧ��ѧ��";
	cout << setw(18) << "ѧ������";
	cout << setw(10) << resetiosflags(ios::left) << setiosflags(ios::right) << "����" << setw(10) << "��ѧ"
		<< setw(10) << "Ӣ��" << setw(10) << "����" << setw(10) << "��ѧ" << setw(10) << "����";
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
			cout << "���������ʾ��20�����ݡ���";
			system("pause");
		}
	}
	cout << "�Ƿ�����? <Y/N>\n>>>";
	char clear = 0;
	cin >> clear;
	if (clear == 'Y') system("cls");
	cout << "����д����ϣ���ʼ�ɼ�������" << endl;
	cout << "������������ֽܷ�������ѧ���ɼ�����" << endl;
	system("pause");
	cout << "��ѡ���ļ��в��½����(�����������ֽܷ���ɼ������.txt)" << endl;
	system("pause");
	cout << endl;

	//����ÿ��ѧ���ܷ�[zz]
	for (i = 0, students[i].mark_total = 0; i < num_students; i++)
	{
		for (j = 0; j < 6; j++)
			students[i].mark_total = students[i].mark_total + students[i].mark[j];
	}
	//����༶ƽ����[zz]
	for (i = 0, class_total = 0; i < num_students; i++)
	{
		class_total = students[i].mark_total + class_total;
	}
	class_average = class_total / num_students;
	//�������ƽ����[zz]
	for (j = 0; j < 6; j++)
	{
		for (i = 0; i < num_students; i++)
		{
			mark_average[j] = students[i].mark[j] + mark_average[j];
		}
		mark_average[j] = mark_average[j] / num_students;
	}
	//���ֽܷ��н�������[zz]
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

	//�༶�����ʼ������̵�[wjy]
#ifdef MAILING
	string highest = doubleToString(students[0].mark_total);
	string highestname = students[0].name;
	string lowest = doubleToString(students[num_students - 1].mark_total);
	string lowestname = students[num_students - 1].name;
#endif

	//�û����д���TXT�ļ��������[wjy]
ChooseFile02:
	string FilePath02;
	string FileName02;
	OPENFILENAME ofn02 = { 0 };
	TCHAR strFileName02[MAX_PATH] = { 0 };		//���ڽ����ļ���
	ofn02.lStructSize = sizeof(OPENFILENAME);			//�ṹ���С
	ofn02.hwndOwner = NULL;		//ӵ���Ŵ��ھ��
	ofn02.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//���ù���
	ofn02.nFilterIndex = 1;		//����������
	ofn02.lpstrFile = strFileName02;		//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
	ofn02.nMaxFile = sizeof(strFileName02);		//����������
	ofn02.lpstrInitialDir = NULL;			//��ʼĿ¼ΪĬ��
	ofn02.lpstrTitle = TEXT("��ѡ��һ���ļ�");		//���ڱ���
	ofn02.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
	//���ļ��Ի���
	if (GetOpenFileName(&ofn02)) {
		string filePath = TCHAR2STRING(strFileName02);
		int start = filePath.find_last_of('\\');
		int end = filePath.find_last_of('.');
		string fileName = filePath.substr(start + 1, end - start - 1);
		string exten = filePath.substr(end, filePath.length() - end);
		cout << "��ѡ����ļ�·��Ϊ:" << filePath << endl;
		cout << "�ļ���Ϊ��" << fileName << "\t\t" << "��չ��Ϊ��" << exten << endl;
		FilePath02 = filePath;
		FileName02 = fileName;
		if (exten != ".txt")
		{
			cout << ">>>�ļ���չ�����������´���txt�ɼ���" << endl;
			system("pause");
			goto ChooseFile02;
		}
	}
	else {
		MessageBox(NULL, TEXT("���½�һtxt�ļ�"), NULL, MB_ICONERROR);
		goto ChooseFile02;
	}

	//����ļ�������[wjy]
	ofile.open(FilePath02);
	if (!ofile.is_open())
	{
		cout << "Could not open the file " << FileName01 << endl;
		cout << "Program terminating.\n";
		exit(EXIT_FAILURE);
	}

	//����ֽܷ��������[zz]
	ofile << "����Ϊ���ֽܷ��������" << endl;
	ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "��� " << "  ";
	ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "ѧ��";
	ofile << setw(18) << "����";
	ofile << resetiosflags(ios::left) << setiosflags(ios::right);
	ofile << setiosflags(ios::right) << setw(10) << "�ܷ�";
	ofile << setiosflags(ios::right) << setw(10) << "����";
	ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
	ofile << setiosflags(ios::right) << setw(10) << "Ӣ��";
	ofile << setiosflags(ios::right) << setw(10) << "����";
	ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
	ofile << setiosflags(ios::right) << setw(10) << "����";
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
	cout << "�ֽܷ�������ѧ���ɼ���������ϣ������������ѡ�������򡭡�" << endl;
	system("pause");
	cout << endl;

	//�û�ѡ��ʲô����[zz]
	int chooserand;
	char anw;
ChooseRand:
	cout << "�������֣���ӡ��Ӧ�ĳɼ�������" << endl;
	cout << "\t����������0" << endl;
	cout << "\t��ѧ������1" << endl;
	cout << "\tӢ��������2" << endl;
	cout << "\t����������3" << endl;
	cout << "\t��ѧ������4" << endl;
	cout << "\t����������5" << endl;
	cout << ">>>";
	cin >> chooserand;
	if (chooserand == 0) {
		cout << "�Ƿ�����Ľ�������? <Y/N>";
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
		cout << "�Ƿ����ѧ��������? <Y/N>";
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
		cout << "�Ƿ��Ӣ���������? <Y/N>";
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
		cout << "�Ƿ�������������? <Y/N>";
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
		cout << "�Ƿ�Ի�ѧ��������? <Y/N>";
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
		cout << "�Ƿ�����ν�������? <Y/N>";
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
		cout << "����������Ч�����������루�磺1��" << endl;
		goto ChooseRand;
	}

	//�û����д���TXT�ļ��������[wjy]
ChooseFile03:
	string FilePath03;
	string FileName03;
	OPENFILENAME ofn03 = { 0 };
	TCHAR strFileName03[MAX_PATH] = { 0 };		//���ڽ����ļ���
	ofn03.lStructSize = sizeof(OPENFILENAME);			//�ṹ���С
	ofn03.hwndOwner = NULL;		//ӵ���Ŵ��ھ��
	ofn03.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//���ù���
	ofn03.nFilterIndex = 1;		//����������
	ofn03.lpstrFile = strFileName03;		//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
	ofn03.nMaxFile = sizeof(strFileName03);		//����������
	ofn03.lpstrInitialDir = NULL;			//��ʼĿ¼ΪĬ��
	ofn03.lpstrTitle = TEXT("��ѡ��һ���ļ�");		//���ڱ���
	ofn03.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
	//���ļ��Ի���
	if (GetOpenFileName(&ofn03)) {
		string filePath = TCHAR2STRING(strFileName03);
		int start = filePath.find_last_of('\\');
		int end = filePath.find_last_of('.');
		string fileName = filePath.substr(start + 1, end - start - 1);
		string exten = filePath.substr(end, filePath.length() - end);
		cout << "��ѡ����ļ�·��Ϊ:" << filePath << endl;
		cout << "�ļ���Ϊ��" << fileName << "\t\t" << "��չ��Ϊ��" << exten << endl;
		FilePath03 = filePath;
		FileName03 = fileName;
		if (exten != ".txt")
		{
			cout << ">>>�ļ���չ�����������´���txt�ɼ���" << endl;
			system("pause");
			goto ChooseFile03;
		}
	}
	else {
		MessageBox(NULL, TEXT("���½�һtxt�ļ�"), NULL, MB_ICONERROR);
		goto ChooseFile03;
	}

	//����ļ�������[wjy]
	ofile.open(FilePath03);
	if (!ofile.is_open())
	{
		cout << "Could not open the file " << FileName01 << endl;
		cout << "Program terminating.\n";
		exit(EXIT_FAILURE);
	}

	//�����ѧ��չʾ���Ż�[wjy]
	string subject_wjyzz = "��ǰ��������ѧ��Ϊ��";
	switch (chooserand)
	{
	case 0: subject_wjyzz += "���ģ�"; break;
	case 1: subject_wjyzz += "��ѧ��"; break;
	case 2: subject_wjyzz += "Ӣ�"; break;
	case 3: subject_wjyzz += "����"; break;
	case 4: subject_wjyzz += "��ѧ��"; break;
	case 5: subject_wjyzz += "���Σ�"; break;
	}
	ofile << subject_wjyzz << endl;

	//����û�ָ�����������[zz]
	ofile << "����Ϊ����ѧ�ƽ��������" << endl;
	ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "��� " << "  ";
	ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "ѧ��";
	ofile << setw(18) << "����";
	ofile << resetiosflags(ios::left) << setiosflags(ios::right);
	ofile << setiosflags(ios::right) << setw(10) << "�ܷ�";
	ofile << setiosflags(ios::right) << setw(10) << "����";
	ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
	ofile << setiosflags(ios::right) << setw(10) << "Ӣ��";
	ofile << setiosflags(ios::right) << setw(10) << "����";
	ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
	ofile << setiosflags(ios::right) << setw(10) << "����";
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
	cout << "����ɹ���" << endl << endl;
	ofile.close();
	cout << "�Ƿ����ѡ��������<Y/N>";
	cin >> anw;
	if (anw == 'Y')
		goto ChooseRand;
	cout << endl;
	cout << "����ѧ�ƽ��������ȫ��������ϣ�" << endl;
	cout << "�Ƿ�����? <Y/N>\n>>>";
	cin >> clear;
	if (clear == 'Y') system("cls");
	cout << "�����������ѧ��ɸѡ����" << endl;
	system("pause");
	cout << endl;

	//�û�����ѡ��������ɸѡ�����ɸѡ�����
	//�û�ɸѡ����[zxy]
	int choosecond;;
ChooseCond:
	cout << "�������֣�������Ӧ�ĳɼ�ɸѡ��" << endl;
	cout << "\t����������0" << endl;
	cout << "\t��ѧ������1" << endl;
	cout << "\tӢ��������2" << endl;
	cout << "\t����������3" << endl;
	cout << "\t��ѧ������4" << endl;
	cout << "\t����������5" << endl;
	cout << "\t�ܷ�������6" << endl;
	cout << ">>>";
	cin >> choosecond;
	if (choosecond == 0) {
		cout << "�Ƿ�����Ľ���ɸѡ? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseCond;

		//��������[zz]
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
		//ѧ��ɸѡ[zxy]
		int top = TCHINESE; int bottom = 0;
		int n0[30] = { 0 };
		int z = 0;
		cout << "����ɸѡ����[bottom,top]:" << endl;
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

		//�û����д���TXT�ļ��������[wjy]
	ChooseFile04:
		string FilePath04;
		string FileName04;
		OPENFILENAME ofn04 = { 0 };
		TCHAR strFileName04[MAX_PATH] = { 0 };		//���ڽ����ļ���
		ofn04.lStructSize = sizeof(OPENFILENAME);			//�ṹ���С
		ofn04.hwndOwner = NULL;		//ӵ���Ŵ��ھ��
		ofn04.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//���ù���
		ofn04.nFilterIndex = 1;		//����������
		ofn04.lpstrFile = strFileName04;		//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
		ofn04.nMaxFile = sizeof(strFileName04);		//����������
		ofn04.lpstrInitialDir = NULL;			//��ʼĿ¼ΪĬ��
		ofn04.lpstrTitle = TEXT("��ѡ��һ���ļ�");		//���ڱ���
		ofn04.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
		//���ļ��Ի���
		if (GetOpenFileName(&ofn04)) {
			string filePath = TCHAR2STRING(strFileName04);
			int start = filePath.find_last_of('\\');
			int end = filePath.find_last_of('.');
			string fileName = filePath.substr(start + 1, end - start - 1);
			string exten = filePath.substr(end, filePath.length() - end);
			cout << "��ѡ����ļ�·��Ϊ:" << filePath << endl;
			cout << "�ļ���Ϊ��" << fileName << "\t\t" << "��չ��Ϊ��" << exten << endl;
			FilePath04 = filePath;
			FileName04 = fileName;
			if (exten != ".txt")
			{
				cout << ">>>�ļ���չ�����������´���txt�ɼ���" << endl;
				system("pause");
				goto ChooseFile04;
			}
		}
		else {
			MessageBox(NULL, TEXT("���½�һtxt�ļ�"), NULL, MB_ICONERROR);
			goto ChooseFile04;
		}

		//����ļ�������[wjy]
		ofile.open(FilePath04);
		if (!ofile.is_open())
		{
			cout << "Could not open the file " << FileName01 << endl;
			cout << "Program terminating.\n";
			exit(EXIT_FAILURE);
		}
		//����ӡ[zxy]
		ofile << "����Ϊ������ɸѡ�����Ľ��������" << endl;
		ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "��� " << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "ѧ��";
		ofile << setw(18) << "����";
		ofile << resetiosflags(ios::left) << setiosflags(ios::right);
		ofile << setiosflags(ios::right) << setw(10) << "�ܷ�";
		ofile << setiosflags(ios::right) << setw(10) << "����";
		ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
		ofile << setiosflags(ios::right) << setw(10) << "Ӣ��";
		ofile << setiosflags(ios::right) << setw(10) << "����";
		ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
		ofile << setiosflags(ios::right) << setw(10) << "����";
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
		cout << "�Ƿ����ѧ����ɸѡ? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseRand;

		//��ѧ����[zz]
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
		//ѧ��ѡ��[zxy]
		int top = TMATH; int bottom = 0;
		int n0[30] = { 0 };
		int z = 0;
		cout << "����ɸѡ����[bottom,top]:" << endl;
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

		//�û����д���TXT�ļ��������[wjy]
	ChooseFile05:
		string FilePath05;
		string FileName05;
		OPENFILENAME ofn05 = { 0 };
		TCHAR strFileName05[MAX_PATH] = { 0 };		//���ڽ����ļ���
		ofn05.lStructSize = sizeof(OPENFILENAME);			//�ṹ���С
		ofn05.hwndOwner = NULL;		//ӵ���Ŵ��ھ��
		ofn05.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//���ù���
		ofn05.nFilterIndex = 1;		//����������
		ofn05.lpstrFile = strFileName05;		//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
		ofn05.nMaxFile = sizeof(strFileName05);		//����������
		ofn05.lpstrInitialDir = NULL;			//��ʼĿ¼ΪĬ��
		ofn05.lpstrTitle = TEXT("��ѡ��һ���ļ�");		//���ڱ���
		ofn05.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
		//���ļ��Ի���
		if (GetOpenFileName(&ofn05)) {
			string filePath = TCHAR2STRING(strFileName05);
			int start = filePath.find_last_of('\\');
			int end = filePath.find_last_of('.');
			string fileName = filePath.substr(start + 1, end - start - 1);
			string exten = filePath.substr(end, filePath.length() - end);
			cout << "��ѡ����ļ�·��Ϊ:" << filePath << endl;
			cout << "�ļ���Ϊ��" << fileName << "\t\t" << "��չ��Ϊ��" << exten << endl;
			FilePath05 = filePath;
			FileName05 = fileName;
			if (exten != ".txt")
			{
				cout << ">>>�ļ���չ�����������´���txt�ɼ���" << endl;
				system("pause");
				goto ChooseFile05;
			}
		}
		else {
			MessageBox(NULL, TEXT("���½�һtxt�ļ�"), NULL, MB_ICONERROR);
			goto ChooseFile05;
		}

		//����ļ�������[wjy]
		ofile.open(FilePath05);
		if (!ofile.is_open())
		{
			cout << "Could not open the file " << FileName01 << endl;
			cout << "Program terminating.\n";
			exit(EXIT_FAILURE);
		}
		//������[zxy]
		ofile << "����Ϊ����ѧɸѡ����ѧ���������" << endl;
		ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "��� " << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "ѧ��";
		ofile << setw(18) << "����";
		ofile << resetiosflags(ios::left) << setiosflags(ios::right);
		ofile << setiosflags(ios::right) << setw(10) << "�ܷ�";
		ofile << setiosflags(ios::right) << setw(10) << "����";
		ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
		ofile << setiosflags(ios::right) << setw(10) << "Ӣ��";
		ofile << setiosflags(ios::right) << setw(10) << "����";
		ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
		ofile << setiosflags(ios::right) << setw(10) << "����";
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
		cout << "�Ƿ��Ӣ�����ɸѡ? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseCond;
		//Ӣ������[zz]
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
		//ѧ��ɸѡ[zxy]
		int top = TENG; int bottom = 0;
		int n0[30] = { 0 };
		int z = 0;
		cout << "����ɸѡ����[bottom,top]:" << endl;
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

		//�û����д���TXT�ļ��������[wjy]
	ChooseFile06:
		string FilePath06;
		string FileName06;
		OPENFILENAME ofn06 = { 0 };
		TCHAR strFileName06[MAX_PATH] = { 0 };		//���ڽ����ļ���
		ofn06.lStructSize = sizeof(OPENFILENAME);			//�ṹ���С
		ofn06.hwndOwner = NULL;		//ӵ���Ŵ��ھ��
		ofn06.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//���ù���
		ofn06.nFilterIndex = 1;		//����������
		ofn06.lpstrFile = strFileName06;		//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
		ofn06.nMaxFile = sizeof(strFileName06);		//����������
		ofn06.lpstrInitialDir = NULL;			//��ʼĿ¼ΪĬ��
		ofn06.lpstrTitle = TEXT("��ѡ��һ���ļ�");		//���ڱ���
		ofn06.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
		//���ļ��Ի���
		if (GetOpenFileName(&ofn06)) {
			string filePath = TCHAR2STRING(strFileName06);
			int start = filePath.find_last_of('\\');
			int end = filePath.find_last_of('.');
			string fileName = filePath.substr(start + 1, end - start - 1);
			string exten = filePath.substr(end, filePath.length() - end);
			cout << "��ѡ����ļ�·��Ϊ:" << filePath << endl;
			cout << "�ļ���Ϊ��" << fileName << "\t\t" << "��չ��Ϊ��" << exten << endl;
			FilePath06 = filePath;
			FileName06 = fileName;
			if (exten != ".txt")
			{
				cout << ">>>�ļ���չ�����������´���txt�ɼ���" << endl;
				system("pause");
				goto ChooseFile06;
			}
		}
		else {
			MessageBox(NULL, TEXT("���½�һtxt�ļ�"), NULL, MB_ICONERROR);
			goto ChooseFile06;
		}

		//����ļ�������[wjy]
		ofile.open(FilePath06);
		if (!ofile.is_open())
		{
			cout << "Could not open the file " << FileName01 << endl;
			cout << "Program terminating.\n";
			exit(EXIT_FAILURE);
		}
		//��ӡ���[zxy]
		ofile << "����Ϊ��Ӣ��ɸѡ��Ӣ�ｵ�������" << endl;
		ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "��� " << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "ѧ��";
		ofile << setw(18) << "����";
		ofile << resetiosflags(ios::left) << setiosflags(ios::right);
		ofile << setiosflags(ios::right) << setw(10) << "�ܷ�";
		ofile << setiosflags(ios::right) << setw(10) << "����";
		ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
		ofile << setiosflags(ios::right) << setw(10) << "Ӣ��";
		ofile << setiosflags(ios::right) << setw(10) << "����";
		ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
		ofile << setiosflags(ios::right) << setw(10) << "����";
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
		cout << "�Ƿ���������ɸѡ? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseCond;
		//��������[zz]
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
		//ѧ��ɸѡ[zxy]
		int top = TPHYSICS; int bottom = 0;
		int n0[30] = { 0 };
		int z = 0;
		cout << "����ɸѡ����[bottom,top]:" << endl;
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

		//�û����д���TXT�ļ��������[wjy]
	ChooseFile07:
		string FilePath07;
		string FileName07;
		OPENFILENAME ofn07 = { 0 };
		TCHAR strFileName07[MAX_PATH] = { 0 };		//���ڽ����ļ���
		ofn07.lStructSize = sizeof(OPENFILENAME);			//�ṹ���С
		ofn07.hwndOwner = NULL;		//ӵ���Ŵ��ھ��
		ofn07.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//���ù���
		ofn07.nFilterIndex = 1;		//����������
		ofn07.lpstrFile = strFileName07;		//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
		ofn07.nMaxFile = sizeof(strFileName07);		//����������
		ofn07.lpstrInitialDir = NULL;			//��ʼĿ¼ΪĬ��
		ofn07.lpstrTitle = TEXT("��ѡ��һ���ļ�");		//���ڱ���
		ofn07.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
		//���ļ��Ի���
		if (GetOpenFileName(&ofn07)) {
			string filePath = TCHAR2STRING(strFileName07);
			int start = filePath.find_last_of('\\');
			int end = filePath.find_last_of('.');
			string fileName = filePath.substr(start + 1, end - start - 1);
			string exten = filePath.substr(end, filePath.length() - end);
			cout << "��ѡ����ļ�·��Ϊ:" << filePath << endl;
			cout << "�ļ���Ϊ��" << fileName << "\t\t" << "��չ��Ϊ��" << exten << endl;
			FilePath07 = filePath;
			FileName07 = fileName;
			if (exten != ".txt")
			{
				cout << ">>>�ļ���չ�����������´���txt�ɼ���" << endl;
				system("pause");
				goto ChooseFile07;
			}
		}
		else {
			MessageBox(NULL, TEXT("���½�һtxt�ļ�"), NULL, MB_ICONERROR);
			goto ChooseFile07;
		}

		//����ļ�������[wjy]
		ofile.open(FilePath07);
		if (!ofile.is_open())
		{
			cout << "Could not open the file " << FileName01 << endl;
			cout << "Program terminating.\n";
			exit(EXIT_FAILURE);
		}
		//����ӡ[zxy]
		ofile << "����Ϊ������ɸѡ�������������" << endl;
		ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "��� " << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "ѧ��";
		ofile << setw(18) << "����";
		ofile << resetiosflags(ios::left) << setiosflags(ios::right);
		ofile << setiosflags(ios::right) << setw(10) << "�ܷ�";
		ofile << setiosflags(ios::right) << setw(10) << "����";
		ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
		ofile << setiosflags(ios::right) << setw(10) << "Ӣ��";
		ofile << setiosflags(ios::right) << setw(10) << "����";
		ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
		ofile << setiosflags(ios::right) << setw(10) << "����";
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
		cout << "�Ƿ�Ի�ѧ����ɸѡ? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseCond;
		//��ѧ����[zz]
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
		//ѧ��ѡ��[zxy]
		int top = TCHEMISTRY; int bottom = 0;
		int n0[30] = { 0 };
		int z = 0;
		cout << "����ɸѡ����[bottom,top]:" << endl;
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

		//�û����д���TXT�ļ��������[wjy]
	ChooseFile08:
		string FilePath08;
		string FileName08;
		OPENFILENAME ofn08 = { 0 };
		TCHAR strFileName08[MAX_PATH] = { 0 };		//���ڽ����ļ���
		ofn08.lStructSize = sizeof(OPENFILENAME);			//�ṹ���С
		ofn08.hwndOwner = NULL;		//ӵ���Ŵ��ھ��
		ofn08.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//���ù���
		ofn08.nFilterIndex = 1;		//����������
		ofn08.lpstrFile = strFileName08;		//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
		ofn08.nMaxFile = sizeof(strFileName08);		//����������
		ofn08.lpstrInitialDir = NULL;			//��ʼĿ¼ΪĬ��
		ofn08.lpstrTitle = TEXT("��ѡ��һ���ļ�");		//���ڱ���
		ofn08.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
		//���ļ��Ի���
		if (GetOpenFileName(&ofn08)) {
			string filePath = TCHAR2STRING(strFileName08);
			int start = filePath.find_last_of('\\');
			int end = filePath.find_last_of('.');
			string fileName = filePath.substr(start + 1, end - start - 1);
			string exten = filePath.substr(end, filePath.length() - end);
			cout << "��ѡ����ļ�·��Ϊ:" << filePath << endl;
			cout << "�ļ���Ϊ��" << fileName << "\t\t" << "��չ��Ϊ��" << exten << endl;
			FilePath08 = filePath;
			FileName08 = fileName;
			if (exten != ".txt")
			{
				cout << ">>>�ļ���չ�����������´���txt�ɼ���" << endl;
				system("pause");
				goto ChooseFile08;
			}
		}
		else {
			MessageBox(NULL, TEXT("���½�һtxt�ļ�"), NULL, MB_ICONERROR);
			goto ChooseFile08;
		}

		//����ļ�������[wjy]
		ofile.open(FilePath08);
		if (!ofile.is_open())
		{
			cout << "Could not open the file " << FileName01 << endl;
			cout << "Program terminating.\n";
			exit(EXIT_FAILURE);
		}
		//����ӡ[zxy]
		ofile << "����Ϊ����ѧɸѡ�Ļ�ѧ���������" << endl;
		ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "��� " << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "ѧ��";
		ofile << setw(18) << "����";
		ofile << resetiosflags(ios::left) << setiosflags(ios::right);
		ofile << setiosflags(ios::right) << setw(10) << "�ܷ�";
		ofile << setiosflags(ios::right) << setw(10) << "����";
		ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
		ofile << setiosflags(ios::right) << setw(10) << "Ӣ��";
		ofile << setiosflags(ios::right) << setw(10) << "����";
		ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
		ofile << setiosflags(ios::right) << setw(10) << "����";
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
		cout << "�Ƿ�����ν���ɸѡ? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseCond;
		//��������[zz]
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
		//ѧ��ɸѡ[zxy]
		int top = TPOLITICS; int bottom = 0;
		int n0[30] = { 0 };
		int z = 0;
		cout << "����ɸѡ����[bottom,top]:" << endl;
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

		//�û����д���TXT�ļ��������[wjy]
	ChooseFile09:
		string FilePath09;
		string FileName09;
		OPENFILENAME ofn09 = { 0 };
		TCHAR strFileName09[MAX_PATH] = { 0 };		//���ڽ����ļ���
		ofn09.lStructSize = sizeof(OPENFILENAME);			//�ṹ���С
		ofn09.hwndOwner = NULL;		//ӵ���Ŵ��ھ��
		ofn09.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//���ù���
		ofn09.nFilterIndex = 1;		//����������
		ofn09.lpstrFile = strFileName09;		//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
		ofn09.nMaxFile = sizeof(strFileName09);		//����������
		ofn09.lpstrInitialDir = NULL;			//��ʼĿ¼ΪĬ��
		ofn09.lpstrTitle = TEXT("��ѡ��һ���ļ�");		//���ڱ���
		ofn09.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
		//���ļ��Ի���
		if (GetOpenFileName(&ofn09)) {
			string filePath = TCHAR2STRING(strFileName09);
			int start = filePath.find_last_of('\\');
			int end = filePath.find_last_of('.');
			string fileName = filePath.substr(start + 1, end - start - 1);
			string exten = filePath.substr(end, filePath.length() - end);
			cout << "��ѡ����ļ�·��Ϊ:" << filePath << endl;
			cout << "�ļ���Ϊ��" << fileName << "\t\t" << "��չ��Ϊ��" << exten << endl;
			FilePath09 = filePath;
			FileName09 = fileName;
			if (exten != ".txt")
			{
				cout << ">>>�ļ���չ�����������´���txt�ɼ���" << endl;
				system("pause");
				goto ChooseFile09;
			}
		}
		else {
			MessageBox(NULL, TEXT("���½�һtxt�ļ�"), NULL, MB_ICONERROR);
			goto ChooseFile09;
		}

		//����ļ�������[wjy]
		ofile.open(FilePath09);
		if (!ofile.is_open())
		{
			cout << "Could not open the file " << FileName01 << endl;
			cout << "Program terminating.\n";
			exit(EXIT_FAILURE);
		}
		//����ӡ[zxy]
		ofile << "����Ϊ������ɸѡ�����ν��������" << endl;
		ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "��� " << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "ѧ��";
		ofile << setw(18) << "����";
		ofile << resetiosflags(ios::left) << setiosflags(ios::right);
		ofile << setiosflags(ios::right) << setw(10) << "�ܷ�";
		ofile << setiosflags(ios::right) << setw(10) << "����";
		ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
		ofile << setiosflags(ios::right) << setw(10) << "Ӣ��";
		ofile << setiosflags(ios::right) << setw(10) << "����";
		ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
		ofile << setiosflags(ios::right) << setw(10) << "����";
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
		cout << "�Ƿ���ֽܷ���ɸѡ? <Y/N>";
		cin >> anw;
		if (anw == 'N')goto ChooseCond;
		//�ܷ�����[zz]
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
		//ѧ��ɸѡ[zxy]
		int top = 760; int bottom = 0;
		int n0[30] = { 0 };
		int z = 0;
		cout << "����ɸѡ����[bottom,top]:" << endl;
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

		//�û����д���TXT�ļ��������[wjy]
	ChooseFile10:
		string FilePath10;
		string FileName10;
		OPENFILENAME ofn10 = { 0 };
		TCHAR strFileName10[MAX_PATH] = { 0 };		//���ڽ����ļ���
		ofn10.lStructSize = sizeof(OPENFILENAME);			//�ṹ���С
		ofn10.hwndOwner = NULL;		//ӵ���Ŵ��ھ��
		ofn10.lpstrFilter = TEXT("All\0*.*\0jpg\0*.jpg\0bmp\0*.bmp\0\0");			//���ù���
		ofn10.nFilterIndex = 1;		//����������
		ofn10.lpstrFile = strFileName10;		//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
		ofn10.nMaxFile = sizeof(strFileName10);		//����������
		ofn10.lpstrInitialDir = NULL;			//��ʼĿ¼ΪĬ��
		ofn10.lpstrTitle = TEXT("��ѡ��һ���ļ�");		//���ڱ���
		ofn10.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;		//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
		//���ļ��Ի���
		if (GetOpenFileName(&ofn10)) {
			string filePath = TCHAR2STRING(strFileName10);
			int start = filePath.find_last_of('\\');
			int end = filePath.find_last_of('.');
			string fileName = filePath.substr(start + 1, end - start - 1);
			string exten = filePath.substr(end, filePath.length() - end);
			cout << "��ѡ����ļ�·��Ϊ:" << filePath << endl;
			cout << "�ļ���Ϊ��" << fileName << "\t\t" << "��չ��Ϊ��" << exten << endl;
			FilePath10 = filePath;
			FileName10 = fileName;
			if (exten != ".txt")
			{
				cout << ">>>�ļ���չ�����������´���txt�ɼ���" << endl;
				system("pause");
				goto ChooseFile10;
			}
		}
		else {
			MessageBox(NULL, TEXT("���½�һtxt�ļ�"), NULL, MB_ICONERROR);
			goto ChooseFile10;
		}

		//����ļ�������[wjy]
		ofile.open(FilePath10);
		if (!ofile.is_open())
		{
			cout << "Could not open the file " << FileName01 << endl;
			cout << "Program terminating.\n";
			exit(EXIT_FAILURE);
		}
		//����ӡ[zxy]
		ofile << "����Ϊ�����ܷ�ɸѡ�ֽܷ��������" << endl;
		ofile << setiosflags(ios::right) << setfill('0') << setw(3) << "��� " << "  ";
		ofile << setfill(' ') << resetiosflags(ios::right) << setiosflags(ios::left) << setw(14) << "ѧ��";
		ofile << setw(18) << "����";
		ofile << resetiosflags(ios::left) << setiosflags(ios::right);
		ofile << setiosflags(ios::right) << setw(10) << "�ܷ�";
		ofile << setiosflags(ios::right) << setw(10) << "����";
		ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
		ofile << setiosflags(ios::right) << setw(10) << "Ӣ��";
		ofile << setiosflags(ios::right) << setw(10) << "����";
		ofile << setiosflags(ios::right) << setw(10) << "��ѧ";
		ofile << setiosflags(ios::right) << setw(10) << "����";
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
		cout << "����������Ч�����������루�磺1��" << endl;
		goto ChooseCond;
	}

	//���ɿ��[wjy]
	cout << "����ɹ���" << endl << endl;
	cout << "�Ƿ����ɸѡ��<Y/N>";
	cin >> anw;
	if (anw == 'Y')
		goto ChooseCond;
	cout << endl;
	cout << "ɸѡ���ֽܷ��������ȫ��������ϣ�" << endl;
	cout << "�Ƿ�����? <Y/N>\n>>>";
	cin >> clear;
	if (clear == 'Y') system("cls");

	//������������İ��ֽܷ���������[wjy zxy zz����]
	cout << "����ָ�������İ��ֽܷ�������ĳɼ���������" << endl;
	cout << "�������������ƾ�����״ͳ��ͼ���жϰ༶�̰塭��" << endl;
	system("pause");
	cout << endl;

	//������ƾ�����״ͳ��ͼ[csj]
	double arr[6] = { 0 };//ĳһ������ѧ���ķ���֮��
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
	double score_percent[6] = { 0 };//ÿһѧ�Ƶ�ƽ���÷���
	double arr_temp[6] = { 0 };
	score_percent[0] = arr[0] / num_students / TCHINESE;
	score_percent[1] = arr[1] / num_students / TMATH;
	score_percent[2] = arr[2] / num_students / TENG;
	score_percent[3] = arr[3] / num_students / TPHYSICS;
	score_percent[4] = arr[4] / num_students / TCHEMISTRY;
	score_percent[5] = arr[5] / num_students / TPOLITICS;

	print_chartofaverage(arr, num_students);

	//�жϰ༶�̰�(�÷ְٷֱ��������)[csj]
	for (int i = 0; i < 6; i++) {
		arr_temp[i] = score_percent[i];
	}
	//����ÿһѧ�Ƶ�ƽ���÷��ʣ��Ա�����������ǰ��ȶԣ��ó��÷�����͵�����ѧ��[csj]
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
	cout << "�༶�̰�Ϊ��" << endl;
	switch (low_1) {
	case 1:cout << "\t���ĵ÷������" << endl; break;
	case 2:cout << "\t��ѧ�÷������" << endl; break;
	case 3:cout << "\tӢ��÷������" << endl; break;
	case 4:cout << "\t����÷������" << endl; break;
	case 5:cout << "\t��ѧ�÷������" << endl; break;
	case 6:cout << "\t���ε÷������" << endl; break;
	default:break;
	}
	switch (low_2) {
	case 1:cout << "\t���ĵ÷��ʵڶ���" << endl; break;
	case 2:cout << "\t��ѧ�÷��ʵڶ���" << endl; break;
	case 3:cout << "\tӢ��÷��ʵڶ���" << endl; break;
	case 4:cout << "\t����÷��ʵڶ���" << endl; break;
	case 5:cout << "\t��ѧ�÷��ʵڶ���" << endl; break;
	case 6:cout << "\t���ε÷��ʵڶ���" << endl; break;
	default:break;
	}

	//�༶�ɼ������̵�[csj]
#ifdef MAILING
	string worstsubject;
	string worst2subject;
	switch (low_1) {
	case 1:worstsubject = "\t���ĵ÷������\n"; break;
	case 2:worstsubject = "\t��ѧ�÷������\n"; break;
	case 3:worstsubject = "\tӢ��÷������\n"; break;
	case 4:worstsubject = "\t����÷������\n"; break;
	case 5:worstsubject = "\t��ѧ�÷������\n"; break;
	case 6:worstsubject = "\t���ε÷������\n"; break;
	default:break;
	}
	switch (low_2) {
	case 1:worst2subject = "\t���ĵ÷��ʵڶ���\n"; break;
	case 2:worst2subject = "\t��ѧ�÷��ʵڶ���\n"; break;
	case 3:worst2subject = "\tӢ��÷��ʵڶ���\n"; break;
	case 4:worst2subject = "\t����÷��ʵڶ���\n"; break;
	case 5:worst2subject = "\t��ѧ�÷��ʵڶ���\n"; break;
	case 6:worst2subject = "\t���ε÷��ʵڶ���\n"; break;
	default:break;
	}
#endif

	//ÿ�ſγɼ��ֶ�ͳ��&ͳ��ͼ����[csj]
	cout << endl << "���ƾ�����״ͳ��ͼ������ϣ�����������ɵ��Ʒֶ�ͳ��ͼ����" << endl;
	cout << "��Ϊ�÷�����85%����Ϊ���㣬70-85%Ϊ���ã�60-70%Ϊ�ϸ���60%����Ϊ���ϸ�" << endl;
	int temp_csj = 0;
	//a,b,c,d���ηֱ��Ӧ���㣬���ã��ϸ񣬲��ϸ�[csj]
	cout << "��Ŀ" << setiosflags(ios::right) << setw(11) << " ";
	cout << setw(15) << "����" << setw(15) << "����" << setw(15) << "�ϸ�" << setw(15) << "���ϸ�" << endl;
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
	//����ÿ�ſγɼ��ֶ�ͳ����״ͼ+��ͼ
	int aa;
	cout << "����1-6��ѡ����Ҫ���ɷֶγɼ�ͳ��ͼ�Ŀ�Ŀ��" << endl;
	cin >> aa;
	print_fenduan(aa - 1, arr01);

	//����[wjy]
	cout << endl << "���Ʒֶ�ͳ����״ͼ�������" << endl;
	cout << "������������˳ɼ����������䲢����ƫ�ơ���" << endl;
	system("pause");
	cout << endl;

	//ƫ�Ʒ���(ʹ�ü���ʵ��)[csj]
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
			cout << resetiosflags(ios::right) << setiosflags(ios::left) << setw(25) << students[i].name << "ƫ��" << endl;
		}
	}

	//��ʦ�ʼ�����׼��[wjy]
#ifdef MAILING
	cout << "�������ʦ����(��mushroom@seu.edu.cn)" << endl << ">>>";
	string teachermail;
	cin >> teachermail;
#endif
	cout << "������������ʼ���" << endl;
	system("pause");
	//�ʼ�����[wjy]
#ifdef MAILING
	//����string�ṹ�����������ķ���
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
		string mark_total = "0";	//ѧ���ܷ�
		string mark_ave = "0";	//ѧ������
		string is_lean = "0";		//�ж��Ƿ�ƫ��
	};
	Studentstr* stu_str = new Studentstr[num_students];
	//����doubleתstring
	for (i = 0; i < num_students; i++)
	{
		stu_str[i].mark_total = doubleToString(students[i].mark_total);
		stu_str[i].mark_ave = doubleToString(students[i].mark_ave);
		if (students[i].is_lean == 0) stu_str[i].is_lean = "��ƫ��";
		if (students[i].is_lean == 1) stu_str[i].is_lean = "ƫ��";
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
			(string)"������" + students[i].name +
			(string)"\n�ܷ֣�" + stu_str[i].mark_total +
			(string)"\n���ģ�" + stu_str[i].chinese +
			(string)"\n��ѧ��" + stu_str[i].math +
			(string)"\nӢ�" + stu_str[i].english +
			(string)"\n����" + stu_str[i].physics +
			(string)"\n��ѧ��" + stu_str[i].chemistry +
			(string)"\n���Σ�" + stu_str[i].politics +
			(string)"\n" + stu_str[i].is_lean;
		CSmtp smtp(
			25,									/*smtp�˿�*/
			"smtp.163.com",						/*smtp��������ַ*/
			"xxx",	        /*���163�����ַ*/
			"xxx",				    /*163����ר����Ȩ����*/
			studentid[i],					    /*Ŀ�������ַ*/
			"mushroom�ɼ�����ϵͳ�ʼ�����-ѧ����",	/*����*/
			studentmail[i]		                /*�ʼ�����*/
		);
		int err;
		if ((err = smtp.SendEmail_Ex()) != 0)
		{
			if (err == 1)
				cout << "����1: �������粻��ͨ������ʧ��!" << endl;
			if (err == 2)
				cout << "����2: �û�������,��˶�!" << endl;
			if (err == 3)
				cout << "����3: �û����������˶�!" << endl;
			if (err == 4)
				cout << "����4: ���鸽��Ŀ¼�Ƿ���ȷ���Լ��ļ��Ƿ����!" << endl;
		}
	}

	//����ʦ���Ͱ༶����ɼ��ſ�[wjy]
	string tercher_content =
		(string)"���༶�ɼ��ſ����£�\n" +
		(string)"�༶��߷�Ϊ:" + highestname + (string)":" + highest + (string)"�֡�\n" +
		(string)"�༶��ͷ�Ϊ:" + lowestname + (string)":" + lowest + (string)"�֡�\n" +
		(string)"�༶�̰�Ϊ:\n" + worstsubject + worst2subject
		;
	CSmtp smtp(
		25,									/*smtp�˿�*/
		"smtp.163.com",						/*smtp��������ַ*/
		"xxx",	        /*��������ַ*/
		"xxx",				    /*163����ר����Ȩ����*/
		teachermail,					    /*Ŀ�������ַ*/
		"mushroom�ɼ�����ϵͳ�ʼ�����-��ʦ��",	/*����*/
		tercher_content		                /*�ʼ�����*/
	);
	int err;
	if ((err = smtp.SendEmail_Ex()) != 0)
	{
		if (err == 1)
			cout << "����1: �������粻��ͨ������ʧ��!" << endl;
		if (err == 2)
			cout << "����2: �û�������,��˶�!" << endl;
		if (err == 3)
			cout << "����3: �û����������˶�!" << endl;
		if (err == 4)
			cout << "����4: ���鸽��Ŀ¼�Ƿ���ȷ���Լ��ļ��Ƿ����!" << endl;
	}
#endif

	cout << endl << "��������˳����򡭡�" << endl;
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

//�����ʼ������[wjy]
void init_prog(void)
{
	system("title �༶�ɼ�����ϵͳ   By:������ ������ �Ԇ� �����h  V2.1.1");
	system("mode con:cols=140 lines=50");
	system("chcp 936");
	system("color 0B");
	system("cls");
	cout << "Performance analysis system.\tBy:Hello Software Group." << endl;
	cout << "Framework construction:������." << endl;
	cout << "Programming:������ �Ԇ� ������ �����h.\n\n";
	cout << "----------------------------------------------" << endl;
}

void Mushstrcpy(char* str01, char* str02)		//�Զ����strcpy���ܵĺ������ַ�����ֵ[wjy]
{
	int i;
	for (i = 0; str02[i] != '\0'; i++)
		str01[i] = str02[i];
	str01[i] = '\0';
}

char* Mushstrcat(char* str_01, char* str_02)	//�Զ����strcat���ܵĺ������ַ���ƴ��[wjy]
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

//tcharתstring[Դhttps://blog.csdn.net/skye1221/article/details/85243465]
//Function:        TCHAR2STRING
//Description:     TCHARתstring
//Input:           str:��ת����TCHAR*�����ַ���
//Return:          ת�����string�����ַ���
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
//doubleתstring��https ://blog.csdn.net/qq_33236491/article/details/108321230
std::string doubleToString(const double& val)
{
	char* chCode;
	chCode = new char[20];
	sprintf(chCode, "%.2lf", val);
	std::string str(chCode);
	delete[]chCode;
	return str;
}
//��Ȩ����������ΪCSDN������Joyce_Chanؼ����ԭ�����£���ѭCC 4.0 BY - SA��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
#endif

//ĳð������[csj]
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
}//����ð��

//�������[csj]
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
		case 0:outtextxy(50 + 60 * i, 310, L"����"); break;
		case 1:outtextxy(50 + 60 * i, 310, L"��ѧ"); break;
		case 2:outtextxy(50 + 60 * i, 310, L"Ӣ��"); break;
		case 3:outtextxy(50 + 60 * i, 310, L"����"); break;
		case 4:outtextxy(50 + 60 * i, 310, L"��ѧ"); break;
		case 5:outtextxy(50 + 60 * i, 310, L"����"); break;
		default:break;
		}
	}
	Sleep(3000);
	closegraph();
	system("pause");
}

//sub�ǿ�Ŀ����ţ���ά�����ÿһ�д���ĳһ��Ŀ�ķֶ�����[csj]
void print_fenduan(int sub, int arr[6][4]) {
	initgraph(640, 480);
	setbkcolor(WHITE);
	cleardevice();
	settextcolor(BLACK);
	setlinecolor(RGB(255, 150, 150));
	switch (sub) {
	case 0:outtextxy(10, 10, L"����"); break;
	case 1:outtextxy(10, 10, L"��ѧ"); break;
	case 2:outtextxy(10, 10, L"Ӣ��"); break;
	case 3:outtextxy(10, 10, L"����"); break;
	case 4:outtextxy(10, 10, L"��ѧ"); break;
	case 5:outtextxy(10, 10, L"����"); break;
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
		case 0:outtextxy(50 + 60 * i, 330, L"����"); break;
		case 1:outtextxy(50 + 60 * i, 330, L"����"); break;
		case 2:outtextxy(50 + 60 * i, 330, L"�ϸ�"); break;
		case 3:outtextxy(50 + 60 * i, 330, L"���ϸ�"); break;
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