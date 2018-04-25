/*
 *  BAQuiz.h
 *  BAQuiz
 *
 *  Created by 尹 伊 on 11-2-16.
 *  Copyright 2011 286studio. All rights reserved.
 *
 */

#define NONE 0
#define YELLOW 1
#define BLUE 2
#define RED 3
#define BAD 4
#define EATER 5

#define RIGHT 3
#define DOWN 6
#define LEFT 9
#define UP 12

#define REMOVEBADAPPLE -1

#define NOTFOUND -1

#define SHOW_CUT_BRANCH 0
#define SHOW_SOLVE_STEP 1

struct BAQuiz {
	int ROWS;
	int COLS;
	int** table;
	int eaters;
	int N;
	// here we use function mapping to simulate a C++ style
	char* (*tostring)(struct BAQuiz* q);
	void (*move_item_at)(struct BAQuiz* q,int r,int c,int d);
	void (*fall)(struct BAQuiz* q);
	int (*max_same)(struct BAQuiz* q,int r,int c,int d);
	int (*remove3)(struct BAQuiz* q);
	int (*find_first)(struct BAQuiz* q,int item,int r,int c,int d);
	int (*find_first_not)(struct BAQuiz* q,int item,int r,int c,int d);
	void (*eat)(struct BAQuiz* q);
	int (*do_step)(struct BAQuiz* q,int n);
	int (*is_solved)(struct BAQuiz* q);
	int (*is_failed)(struct BAQuiz* q);
	int (*solve)(struct BAQuiz* q);
};
typedef struct BAQuiz BAQuiz;

BAQuiz* BAQuizCreate(int rows,int cols,const char* s,int n);
BAQuiz* BAQuizCopy(BAQuiz* q,int n);
void BAQuizAssaign(BAQuiz* q1,const BAQuiz* q2);
void BAQuizRelease(BAQuiz* q);