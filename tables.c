#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#define SUBSIZE 64
#define SUBARR 64
#define SIZE 1024
#define GRADESIZE 16 //also for mark size
#define INPUTSIZE 128
typedef struct subject {
	char sub_name[SUBSIZE];
	int credits;
	int semester;
}subject;

typedef struct grade {
	float grade_list[GRADESIZE];
}grade;

typedef struct mark {
	long mis;
	float mark_list[GRADESIZE];
}mark;


// returns number of bytes read
int readline(int fd, char *line, int size)
{
	char ch;
	int i = 0;
	while (i < size - 1 && read(fd, &ch, 1) && ch != '\n') {
		line[i] = ch;
		i++;
	}
	line[i] = '\0';
	return i;
}

// returns number of records read
int read_subject(subject arr1[], int subarr)
{
	int fd, records = 0;
	char line[SIZE], *p;
	fd = open("./subjects.csv", O_RDONLY);
	if (fd == -1) {
		perror("open failed as");
		return -1;
	}
	while(records < subarr && (readline(fd, line, SIZE))) {
		p = strtok(line, ",");
		strcpy(arr1[records].sub_name, p);
		p = strtok(NULL, ",");
		arr1[records].credits = atoi(p);
		p = strtok(NULL, ",");
		arr1[records].semester = atoi(p);
		records++;
		if (records >= subarr) {
			printf("excess data\n");
			return -1;
		}
	}
	close(fd);
	return records;
}

//returns number of records read
int read_grade(grade arr2[], int subarr, int *ptr)
{
	int fd, records = 0, i = 0;
	char line[SIZE], *p;
	fd = open("./grades.csv", O_RDONLY);
	if (fd == -1) {
		perror("open failed as");
		return -1;
	}
	while(records < subarr && (readline(fd, line, SIZE))) {
		i = 0;
		p = strtok(line, ",");
		arr2[records].grade_list[i] = atof(p);
		i++;
		while (p != NULL) {
			p = strtok(NULL, ",");
			if (p == NULL) {
				break;
			}
			arr2[records].grade_list[i] = atof(p);
			i++;
		}
		if (records > 0 && i != *ptr) {
			printf("grade ranges not uniform\n"); /* eg : one record has 
			7 data while other has 8*/
			return -1;
		}
		*ptr = i;
		records++;
		if (records >= subarr) {
			printf("excess data\n");
			return -1;
		}
	}
	close(fd);
	return records;
}

//returns number of records read;
int read_mark(mark arr3[], int size, int subject_num)
{
	int fd, records = 0, i = 0;
	char line[SIZE], *p;
	fd = open("./marks.csv", O_RDONLY);
	if (fd == -1) {
		perror("open failed as");
		return -1;
	}
	while (records < SIZE && readline(fd, line, SIZE)) {
		p = strtok(line, ",");
		arr3[records].mis = strtol(p, NULL, 10);
		while (p != NULL) {
			p = strtok(NULL, ",");
			if (p == NULL) {
				break;
			}
			arr3[records].mark_list[i] = atof(p);
			i++;
		}
		if (i != subject_num) {
			printf("less or more marks data than number of subjects; in line %d\n", records + 1);
			return -1;
		}
		i = 0;
		records++;
		if (records >= SIZE) {
			printf("excess data\n");
			return -1;
		}
	}
	close(fd);
	return records;
}

void ins_sort(float arr[], int *length)
{
	int i = 2, j, k = 0, store_k = 0, enter = 0;
	float min, x;
	min = arr[0];
	while (k < (*length)) {
		if (arr[k] < min) {
			min = arr[k];
			store_k = k;
			enter = 1;
		}
		k++;
	}
	if (enter == 1) {
		arr[store_k] = arr[0];
		arr[0] = min;
	}
	while (i < (*length)) {
		x = arr[i];
		j = i;
		while (j > 1 && x < arr[j - 1]) {
			arr[j] = arr[j - 1];
			j--;
		}
		arr[j] = x;
		i++;
	}
}

void readcommand(char *s, int size) {
	int i = 0;
	char ch;
	while (i < size - 1 && (ch = getchar()) && ch != '\n' && ch != EOF) {
		s[i] = ch;
		i++;
	}
	if (i > 0 || ch == '\n') {
		s[i] = '\0';
	} else {
		strcpy(s, "EOF");
	}
}

int mis_finder(long x, mark arr3[], int mk_num)
{
	int j = 0;
	while (j < mk_num) {
		if (x == arr3[j].mis) {
			return j;
		}
		j++;
	}
	return -1;
}

char *grade_finder(char *g[], int store_i, int *ptr, float tmp_mark, grade arr2[])
{
	int k = 0;
	char *gp;
	while (k < (*ptr)) {
		if (tmp_mark <= arr2[store_i].grade_list[k]) {
			gp = g[k];
			return gp;
		}
		k++;
	}
	if (tmp_mark <= 100) {
		gp = g[7];
		return gp;
	}
	return NULL;
}
void calc_grade(char *s, char *cp, subject arr1[], grade arr2[], mark arr3[], 
			int ipsize, int sub_num, int gd_num, int mk_num, int *ptr) 
{
	long x;
	int i = 0, store_i, enter = 0, found = 0;
	float tmp_mark;
	char *gp, *g[8] = {"FF", "DD", "CD", "CC", "BC", "BB", "AB", "AA"};
	x = strtol(cp, NULL, 10);
	if (x == 0) {
		printf("wrong command entered\n");
		return;
	}
	cp = strtok(NULL, " ");
	if (cp == NULL) {
		printf("wrong command entered\n");
		return;
	}
	//find the subject
	while (i < sub_num) {
		if (strcmp(cp, arr1[i].sub_name) == 0) {
			store_i = i;
			enter = 1;
			break;
		}
		i++;
	}
	if (enter == 0) {
		printf("wrong subject entered\n");
		return;
	}
	//find the mis number
	found = mis_finder(x, arr3, mk_num);
	if (found == -1) {
		printf("wrong mis entered\n");
		return;
	}
	
	tmp_mark = arr3[found].mark_list[store_i];
	gp = grade_finder(g, store_i, ptr, tmp_mark, arr2);
	if (gp != NULL) {
		printf("%s\n", gp);
	} else {
		printf("error in grade calculation\n");
	}
	return;
}

//return the respective the grade-point
int grade_to_point (char *gp) {
	if (strcmp(gp, "FF") == 0) {
		return 0;
	} else if (strcmp(gp, "DD") == 0) {
		return 4;
	} else if (strcmp(gp, "CD") == 0) {
		return 5;
	} else if (strcmp(gp, "CC") == 0) {
		return 6;
	} else if (strcmp(gp, "BC") == 0) {
		return 7;
	} else if (strcmp(gp, "BB") == 0) {
		return 8;
	} else if (strcmp(gp, "AB") == 0) {
		return 9;
	} else if (strcmp(gp, "AA") == 0) {
		return 10;
	}
	return -1;
}

//returns the calculated sgpa
float calc_sgpa(char *cp, int *ptr, subject arr1[], grade arr2[], mark arr3[],
			int sub_num, int gd_num, int mk_num, int sem, long x)
{
	int found = -1;
	int i = 0, sum_credit = 0, var, enter = 0;
	char *gp, *g[8] = {"FF", "DD", "CD", "CC", "BC", "BB", "AB", "AA"};
	float credit_point = 0, tmp, sgpa = 0;
	
	found = mis_finder(x, arr3, mk_num);
	if (found == -1) {
		printf("wrong mis entered\n");
		return -1;
	}
	
	while (i < sub_num) {
		if (arr1[i].semester == sem) {
			gp = grade_finder(g, i, ptr, arr3[found].mark_list[i], arr2);
			enter = 1;
			if (gp != NULL) {
				var = grade_to_point(gp);
				sum_credit += arr1[i].credits;
				tmp = (float) (arr1[i].credits * var);
				
				credit_point += tmp;
			} else {
				printf("error in grade calculation\n");
				return -1;
			}
		}
		i++;
	}
	if (enter == 0) {
		printf("wrong semester entered\n");
		return -1;
	}
	sgpa = credit_point / sum_credit;
	return sgpa;
	//printf("%.2f\n", sgpa);
}

//returns 1 if there are no errors
int failed_list(long x, char *subject_list[], subject arr1[], grade arr2[], 
		mark arr3[], int sub_num, int mk_num, int inputsize, int *fp, 
		int *sub_list_data)
{
	int found = -1, i = 0, j = 0, len = 0;
	found = mis_finder(x, arr3, mk_num);
	if (found == -1) {
		printf("wrong mis entered\n");
		return -1;
	}
	while (i < sub_num && j < inputsize) {
		if (arr3[found].mark_list[i] <= arr2[i].grade_list[0]) {
			len = strlen(arr1[i].sub_name);
			if (len == 0) {
				return -1;
			}
			*fp = 1;
			subject_list[j] = (char *) malloc(len + 1);
			strcpy(subject_list[j], arr1[i].sub_name);
			j++;
		}
		i++;
	}
	*sub_list_data = j;
	return 1;
}

//returns 
long topper (mark arr3[], int i, int mk_num, int *enter)
{
	float max = (float) INT_MIN;
	int j = 0, store_j = 0;
	while (j < mk_num) {
		if (arr3[j].mark_list[i] > max) {
			max = arr3[j].mark_list[i];
			store_j = j;
			*enter = 1;
		}
		j++;
	}
	return arr3[store_j].mis;
}

//returns the mis of topper
void topsem(int sem, subject arr1[], grade arr2[], mark arr3[], int sub_num,
			int gd_num, int mk_num)
{
	int i = 0, enter = 0, *ent, if_enter = 0;
	long x;
	ent = &enter;
	while (i < sub_num) {
		if (arr1[i].semester == sem) {
			x = topper(arr3, i, mk_num, ent);
			if (*ent == 0) {
				printf("some problem in topper finding\ncheck data\n");
			} else {
				printf("%ld, %s\n", x, arr1[i].sub_name);
			}
			if_enter = 1;
		}
		i++;
	}
	if (if_enter == 0) {
		printf("wrong semester entered\n");
	}
}

long mark_to_mis(float mk, int sbn_store, int mk_num, mark arr3[], int rpt)
{
	int i = 0, store_i = -1, j = 0;
	while (i < mk_num) {
		if (arr3[i].mark_list[sbn_store] == mk) {
			store_i = i;
			if (j == rpt) {
				break;
			} else {
				j++;
			}
		}
		i++;
	}
	if (store_i == -1) {
		return -1;
	}
	return arr3[store_i].mis;
}

float std_dev(char *cp, subject arr1[], mark arr3[], int sub_num, int mk_num)
{
	int len, enter = 0, k, store_k;
	char *sub;
	float mean, sum = 0, dev, sum_dev = 0, var, stdev; // sum_dev = Xi - mean
	// var is variance
	cp = strtok(NULL, " ");
	if (cp == NULL) {
		printf("wrong command entered\n");
		return -1;
	}
	len = strlen(cp);
	sub = (char *) malloc(sizeof(char) * (len + 1));
	strcpy(sub, cp);
	for (k = 0; k < sub_num; k++) {
		if (strcmp(arr1[k].sub_name, sub) == 0) {
			store_k = k;
			enter = 1;
		}
	}
	if (enter == 0) {
		printf("wrong subject entered\n");
		return -1;
	}
	//finding the mean
	for (k = 0; k < mk_num; k++) {
		sum += arr3[k].mark_list[store_k];
	}
	mean = sum / ((float) mk_num);
	
	for (k = 0; k < mk_num; k++) {
		dev = arr3[k].mark_list[store_k] - mean;
		sum_dev += (dev * dev);
	}
	var = sum_dev / ((float) mk_num);
	stdev = sqrt(var);
	free(sub);
	return stdev;
}


void all_grade(int g_all, grade arr2[], mark arr3[], int gd_num, int sub_num, int *ptr) {
	int i = 0;
	float tmp_mark = 0;
	char *gp, *g[8] = {"FF", "DD", "CD", "CC", "BC", "BB", "AB", "AA"};
	printf("%ld,", arr3[g_all].mis);
	while (i < sub_num) {
		tmp_mark = arr3[g_all].mark_list[i];
		gp = grade_finder(g, i, ptr, tmp_mark, arr2);
		printf(" %s", gp);
		if (i < (sub_num - 1)) {
			printf(",");
		}
		i++;
	}	
}

float calc_cgpa(int *ptr, subject arr1[], grade arr2[], mark arr3[], int sub_num,
 			int gd_num, int mk_num, long x)
{
	float tmp, cgpa, credit_point = 0;
	int i = 0, sum_credit = 0, var, found = -1;
	char *gp, *g[8] = {"FF", "DD", "CD", "CC", "BC", "BB", "AB", "AA"};
 	found = mis_finder(x, arr3, mk_num);
 	if (found == -1) {
 		printf("wrong mis entered\n");
 		return -1;
 	}
 	while (i < sub_num) {
 		gp = grade_finder(g, i, ptr, arr3[found].mark_list[i], arr2);
 		if (gp != NULL) {
				var = grade_to_point(gp);
				sum_credit += arr1[i].credits;
				tmp = (float) (arr1[i].credits * var);
				
				credit_point += tmp;
			} else {
				printf("error in grade calculation\n");
				return -1;
			}
 		i++;
 	}
 	cgpa = credit_point / sum_credit;
 	return cgpa;
}

int main (int argc, char *argv[])
{
	char s[INPUTSIZE], *cp, *subject_list[SUBARR];
	int subject_num, grades_num, mark_num, sem;
	// sem is for number of semesters and tmp_arr is to store the failed subjects
	
	
	
	
	//for failed 
	int failed = 0, *fp, fail = 0;// fp checks atleast one sub is failed or not
	int *sub_list_data, sub_ls_data; //no. of elements in subject_list
	
	
	int *ptr, data; //ptr is a pointer to number of data in one record of grades.csv
	
	
	float sgpa, cgpa = 0;
	long x, x1, x2, x3;//to store mis
	
	
	//for topsub
	int sb_store = -1, topsub = 0, *tpsb; //topsub is just for checking error
	
	
	//for topnsub
	int sbn_store = -1, topn = 0, num_student, rpt = -1; 
		//rpt is used for checking repetition of marks
	float sub_mark[SIZE], repeat;
	int sbn2;//to store num_student and 
	//num_student in for storing the number of top students to print
	
	//for stdev
	float dev;
	
	//for grade all
	int g_all = 0;
	
	tpsb = &topsub;
	ptr = &data;
	fp = &failed;
	sub_list_data = &sub_ls_data;
	
	subject arr1[SUBARR];
	grade arr2[SUBARR];
	mark arr3[SIZE];
	
	
	// read subjects.csv
	subject_num = read_subject(arr1, SUBARR);
	if (subject_num == -1) {
		return -1;
	}
	
	// reads grades.csv
	grades_num = read_grade(arr2, SUBARR, ptr);
	if (grades_num == -1) {
		return -1;
	}
	if (grades_num != subject_num) {
		printf("data in gardes.csv not equal to subject.csv\n");
		return -1;
	}
	
	//reads marks.csv
	mark_num = read_mark(arr3, SIZE, subject_num);
	if (mark_num == -1) {
		return -1;
	}
	//printf("%ld\n", arr3[0].mis);
	
	//now sort the each record of arr2 in non-descending order
	for (int k = 0; k < grades_num; k++) {
		ins_sort(arr2[k].grade_list, ptr);
	}
	
	while (1) {
		printf(">");
		readcommand(s, INPUTSIZE);
		if (strcmp(s, "EOF") == 0) {
		printf("\n");
			break;
		}
		if (strcmp(s, "exit") == 0) {
			break;
		}
		cp = strtok(s, " ");
		if (cp == NULL) {
			printf("wrong command entered\n");
			continue;
		}
		if (strcmp (cp, "grade") == 0) {
			cp = strtok(NULL, " ");
			if (cp == NULL) {
				printf("wrong command entered\n");
				continue;
			}
			if (strcmp(cp, "all") == 0) {
				if (cp == NULL) {
					printf("wrong command entered\n");
					continue;
				}
				//int g_all = 0;
				while (g_all < mark_num) {
					all_grade(g_all, arr2, arr3, grades_num, subject_num, ptr);
					printf("\n");
					g_all++;
				}
				g_all = 0;
			} else {
				calc_grade(s, cp, arr1, arr2, arr3, INPUTSIZE, subject_num,
				 grades_num, mark_num, ptr);
			}
		} else if (strcmp(cp, "sgpa") == 0) {
			cp = strtok(NULL, " ");
			if (cp == NULL) {
				printf("wrong command entered\n");
				continue;
			}
			x = strtol(cp, NULL, 10);
			cp = strtok(NULL, " ");
			if (cp == NULL) {
				printf("wrong command entered\n");
				continue;
			}
			sem = atoi(cp);
			sgpa = calc_sgpa(cp, ptr, arr1, arr2, arr3, subject_num,
			grades_num, mark_num, sem, x);
			if (sgpa == -1) {
				continue;
			} else {
				printf("%.2f\n", sgpa);
			}
		} else if (strcmp(cp, "cgpa") == 0) {
			
			cp = strtok(NULL, ",");
			if (cp == NULL) {
				printf("wrong command entered\n");
				continue;
			}
			x1 = strtol(cp, NULL, 10);
			cgpa = calc_cgpa(ptr, arr1, arr2, arr3, subject_num, grades_num, 
			mark_num, x1);
			if (cgpa == -1) {
				continue;
			} else {
				printf("%.2f\n", cgpa);
			}
			
		} else if (strcmp(cp, "failed") == 0) {
			cp = strtok(NULL, " ");
			if (cp == NULL) {
				printf("wrong command entered\n");
				continue;
			}
			x2 = strtol(cp, NULL, 10);
			fail = failed_list(x2, subject_list, arr1, arr2, arr3, 
			subject_num, mark_num, INPUTSIZE, fp, sub_list_data);
			if (fail == -1) {
				continue;
			} else {
				if (*fp == 1) {
					for (int f = 0; f < (*sub_list_data); f++) {
						printf("%s ", subject_list[f]);
					}
					printf("\n");
				} else {
					printf("no subjects failed\n");
				}
			}
			for (int fl = 0; fl < *sub_list_data; fl++) {
				free(subject_list[fl]);
			}
			*fp = 0;
		} else if (strcmp(cp, "topsem") == 0) {
			cp = strtok(NULL, " ");
			if (cp == NULL) {
				printf("wrong command entered\n");
				continue;
			}
			sem = atoi(cp);
			topsem(sem, arr1, arr2, arr3, subject_num, grades_num, mark_num);
		} else if (strcmp(cp, "topsub") == 0) {
			cp = strtok(NULL, " ");
			if (cp == NULL) {
				printf("wrong command entered\n");
				continue;
			}
			for (int sb = 0; sb < subject_num; sb++) {
				if (strcmp(cp, arr1[sb].sub_name) == 0) {
					sb_store = sb;
				}
			}
			if (sb_store == -1) {
				printf("wrong subject entered\n");
				continue;
			}
			x3 = topper(arr3, sb_store, mark_num, tpsb);
			if (*tpsb == 0) {
				printf("some problem in topper finding\ncheck data\n");
			} else {
				printf("%ld\n", x3);
			}
			sb_store = -1;
			*tpsb = 0;
		} else if (strcmp(cp, "topnsub") == 0) {
			cp = strtok(NULL, " ");
			if (cp == NULL) {
				printf("wrong command entered\n");
				continue;
			}
			for (int sbn = 0; sbn < subject_num; sbn++) { 
			//sbn is chosen for topNSuB, the letters S, B and N
				if (strcmp(cp, arr1[sbn].sub_name) == 0) {
					sbn_store = sbn;
				}
			}
			if (sbn_store == -1) {
				printf("wrong subject entered\n");
				continue;
			}
			while (topn < mark_num && topn < SIZE ) {
				sub_mark[topn] = arr3[topn].mark_list[sbn_store];
				topn++;
			}
			
			ins_sort(sub_mark, &topn);
			
			cp = strtok(NULL, " ");
			if (cp == NULL) {
				printf("wrong command entered\n");
				continue;
			}
			num_student = atoi(cp);
			if (num_student > mark_num) {
				printf("number provided exceeds data\n");
				continue;
			}
			if (num_student == 0) {
				printf("wrong number entered or zero is entered\n");
				sbn_store = -1;
				topn = 0;
				continue;
			}
			sbn2 = num_student;
			repeat = sub_mark[topn - sbn2];
			while (sbn2 > 0) {
				if (sub_mark[topn - sbn2] == repeat) {
					rpt++; // rpt starting from -1;
				} else {
					rpt = 0;
					repeat = sub_mark[topn - sbn2];
				}
				x = mark_to_mis(sub_mark[topn - sbn2], sbn_store, 
						mark_num, arr3, rpt);
				if (x == -1) {
					printf("some error in topper finding\n");
				} else {
					printf("%ld %.2f\n", x, sub_mark[topn - sbn2]);
					//printf("%d\n", sbn2);
				}
				sbn2--;
			}
			sbn_store = -1;
			topn = 0;
		} else if (strcmp(cp, "stdev") == 0) {
			dev = std_dev(cp, arr1, arr3, subject_num, mark_num);
			if (dev == -1) {
				continue;
			} else {
				printf("%.2f\n", dev);
			}
		} else {
			printf("wrong command entered\n");
		}
	}
	return 0;
}
