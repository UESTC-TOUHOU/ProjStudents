#define _CRT_SECURE_NO_WARNINGS 1

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct student {
	int id;
	char name[50];
	float chinese;
	float math;
	float english;
	float sum;
}student;
/*打开文件，函数的返回值：学生人数.任何错误返回0*/
int ReadStuInfoFromFile(char* name, student** stu) {
	FILE* fp;
	int n;
	if ((fp = fopen(name, "r")) == NULL)
		return 0;
	if (fread(&n, sizeof(int), 1, fp) != 1)
		return 0;
	*stu = (student*)malloc(sizeof(student) * n);
	if (fread(*stu, sizeof(student), n, fp) != n)
		return 0;
	for (int i = 0; i < n; i++) {
		(*stu + i)->sum = (*stu + i)->chinese + (*stu + i)->math + (*stu + i)->english;
	}
	fclose(fp);
	return n;
}

/*统计不及格，操作成功返回0，失败返回-1*/
int NoPass(student stu[], int n, student** noPassStudent, int* m) {
	*m = 0;
	int num[50] = { 99 };
	for (int o = 0; o < 50; o++) {
		num[o] = 99;
	}
	int j = 0;
	for (int i = 0; i < n; i++) {
		if ((stu + i)->sum < 180) {
			*m = *m + 1;
			num[j] = i;
			j++;
		}
	}

	if (*m != 0) {
		*noPassStudent = (student*)malloc(sizeof(student) * (*m));
	}
	for (int m = 0; num[m] != 99; m++) {
		*(*noPassStudent + m) = *(stu + num[m]);
	}
	if (stu == NULL && n <= 0) {
		return -1;
	}
	else {
		return 0;
	}
}
/*统计及格，操作成功返回0，失败返回-1*/
int Pass(student stu[], int n, student** PassStudent, int* m) {
	*m = 0;
	int num[50] = { 0 };
	for (int o = 0; o < 50; o++) {
		num[o] = 99;
	}
	int j = 0;
	for (int i = 0; i < n; i++) {
		if ((stu + i)->sum > 180) {
			*m = *m + 1;
			num[j] = i;
			j++;
		}
	}

	*PassStudent = (student*)malloc(sizeof(student) * (*m));
	for (int m = 0; num[m] != 99; m++) {
		*(*PassStudent + m) = *(stu + num[m]);
	}
	if (stu == NULL && n <= 0) {
		return -1;
	}
	else {
		return 0;
	}
}

/*排序功能，操作成功返回0，失败返回-1*/
int SortStudents(student stu[], int n) {
	student tmp;
	int i = 0, j = 0;
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - 1; j++) {
			if ((stu + j)->sum < (stu + j + 1)->sum) {
				tmp = stu[j];
				stu[j] = stu[j + 1];
				stu[j + 1] = tmp;
			}
		}
	}
	if (stu == NULL && n <= 0) {
		return -1;
	}
	else return 0;
}

/*打印功能*/
int PrintStudents(student stu[], int n) {
	for (int i = 0; i < n; i++) {
		printf("学生：%s\n", (stu + i)->name);
		printf("学生id为：%d\n学生的总成绩分别为：%.2f\n数学成绩为%.2f\n语文成绩为%.2f\n英语成绩为%.2f\n-------\n", (stu + i)->id, (stu + i)->sum, (stu + i)->math, (stu + i)->chinese, (stu + i)->english);

	}
	printf("-------");
	return 0;
}

/*搜索功能，返回值：查找成功返回0，失败返回-1*/
int SearchStudent(student stu[], int n, int id, int* rank, student** rstu) {
	int i = 0;
	*rstu = (student*)malloc(sizeof(student));
	for (i = 0; i < n; i++) {
		if (id == stu[i].id) {
			*rank = i + 1;
			**rstu = *(stu + i);
			return 0;
		}
	}
	return -1;
}
/*修改功能*/
int ChangeInformation(student stu[], int n, int id) {
	int i = 0;
	float tmp = -1;
	for (i = 0; i < n; i++) {
		if (id == stu[i].id) {
			printf("请输入要修改的数值，无需修改的项目请输入-1\n");
			printf("学生数学成绩：");
			scanf("%f", &tmp);
			if (tmp != -1) {
				(stu + i)->math = tmp;
			}
			printf("学生语文成绩：");
			scanf("%f", &tmp);
			if (tmp != -1) {
				(stu + i)->chinese = tmp;
			}
			printf("学生英语成绩：");
			scanf("%f", &tmp);
			if (tmp != -1) {
				(stu + i)->english = tmp;
			}
			(stu + i)->sum = (stu + i)->chinese + (stu + i)->math + (stu + i)->english;
			printf("修改成功");
			return 0;
		}
	}
	printf("修改失败！");
	return -1;
}
/*保存修改功能，将修改的内容保存进文件(存在问题，待修改)*/
int save(char* name, student** stu, int* n) {
	FILE* fp;
	int m = *n;
	fp = fopen(name, "w+");
	fwrite(n, sizeof(int), 1, fp);
	for (int i = 0; i < *n; i++) {
		//m = fwrite(stu + i, sizeof(student), 1, fp);
		m = fwrite(*stu + i, sizeof(student), 1, fp);
	}
	fclose(fp);
	if (m != 1) {
		return -1;
	}
	return 0;
};

int GetWorkNum() {
	int work;
	scanf("%d", &work);
	return work;
}

int main() {
	system("chcp 65001");		//if code file saved on format utf-8 and "/utf-8" is used, turn this line on. Then command line will show crcorrect.
	int n, rank, id, i, m;
	int jus;
	char name[] = "data\\stuScores.txt";
	char saveFile[] = "data\\stuScoresSaved.txt";

	student* stu = NULL, * noPassStu = NULL, * PassStu = NULL, * rstu = NULL;
	n = ReadStuInfoFromFile(name, &stu);
	if (n == 0) { printf("error"); return -1; }//stu id=1


	printf("-------欢迎使用学生成绩管理系统-------\n");
	printf("-------功能：\n");
	printf("-------1.显示全部学生数据\n");
	printf("-------2.显示不及格学生数据\n");
	printf("-------3.显示及格学生数据\n");
	printf("-------4.打印成绩表（由总分高低排序）\n");
	printf("-------5.搜索学生信息\n");
	printf("-------6.修改学生成绩\n");
	printf("-------7.保存已修改的学生成绩\n");
	printf("-------8.退出\n");

	//可以使用enum对work类型进行标记，同时方便使用switch-case
	int work;
	while ((work = GetWorkNum()) != 8) {
		switch (work) {
		default: {
			printf("输入错误！请重新输入");
			break;
		}
		case 1: {
			PrintStudents(stu, n);
			break;
		}
		case 2: {
			printf("\n-------no pass studnt--------\n");
			i = NoPass(stu, n, &noPassStu, &m);
			if (i == -1) {
				printf("no pass error");
			}
			else {
				PrintStudents(noPassStu, m);
			}

			if (noPassStu) { free(noPassStu); noPassStu = NULL; }
			break;
		}
		case 3: {
			printf("\n-------passed studnt--------\n");
			i = Pass(stu, n, &PassStu, &m);
			if (i == -1) {
				printf(" pass error");
			}
			else {
				PrintStudents(PassStu, m);
			}

			if (PassStu) { free(PassStu); PassStu = NULL; }
			break;
		}
		case 4: {
			printf("\n sort studnt--------\n");
			//i = NoPass(stu, n, &noPassStu, &m);
			i = SortStudents(stu, n);
			if (i == -1) {
				printf("sort error");
			}
			else {
				PrintStudents(stu, n);
			}

			break;
		}
		case 5: {
			printf("\n search id--------\n");
			scanf_s("%d", &id);
			i = SearchStudent(stu, n, id, &rank, &rstu);
			if (i == -1) {
				printf("search error");
			}
			else {
				PrintStudents(rstu, 1);
			}

			if (rstu) { free(rstu); rstu = NULL; }
			break;
		}
		case 6: {
			printf("请输入需要修改成绩的学生的id:\n");
			scanf_s("%d", &id);
			ChangeInformation(stu, n, id);
			break;
		}
		case 7: {
			printf("尝试保存中\n");
			jus = save(saveFile, &stu, &n);
			if (jus == 0) {
				printf("保存成功\n");
			}
			else printf("保存失败！\n");
			break;
		}

		}
	}

	//work == 8
	if (stu) { free(stu); stu = NULL; }
	return 0;
}