#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>
#include "BAQuiz.h"


int main (int argc, const char * argv[]) {
	BAQuiz* q1=BAQuizCreate(1, 4, "bb b", 1);
	printf("%s",q1->tostring(q1));
	assert(q1->solve(q1));
	BAQuizRelease(q1);

	BAQuiz* q2=BAQuizCreate(2, 4, "br brb r", 3);
	printf("%s",q2->tostring(q2));
	assert(q2->solve(q2));
	BAQuizRelease(q2);
	
	BAQuiz* q3=BAQuizCreate(6, 6, "  r     b     b     r     b   rrb rr", 1);
	printf("%s",q3->tostring(q3));
	assert(q3->solve(q3));
	BAQuizRelease(q3);
	
	BAQuiz* q4=BAQuizCreate(7, 6, "  r     y     b     b     y   yyb   rrb yy", 2);
	printf("%s",q4->tostring(q4));
	assert(q4->solve(q4));
	BAQuizRelease(q4);

	BAQuiz* q50=BAQuizCreate(9, 6, "   b     r     r    rb    yr    yb    rbr yybyy ybybrb", 4);
	printf("%s",q50->tostring(q50));
	assert(q50->solve(q50));
	BAQuizRelease(q50);

	//Quiz with BAD
	BAQuiz* q51=BAQuizCreate(4, 6, "  br   ryrbbxxrbxxyyrbxx", 2);
	printf("%s",q51->tostring(q51));
	assert(q51->solve(q51));
	BAQuizRelease(q51);

	BAQuiz* q72=BAQuizCreate(5, 6, "   r   rrx   xxb  bxbx  xbxb  ", 5);
	printf("%s",q72->tostring(q72));
	assert(q72->solve(q72));
	BAQuizRelease(q72);
	
	//Quiz with EATER
	BAQuiz* q71=BAQuizCreate(5, 6, "    r  y yb  yrre  rbey byybb ", 5);
	printf("%s",q71->tostring(q71));
	assert(q71->solve(q71));
	BAQuizRelease(q71);

	BAQuiz* q49=BAQuizCreate(8, 6, "  br  r br bbryybrrbbrrbbrrbbrybbrryeyrbyeeebree", 3);
	printf("%s",q49->tostring(q49));
	assert(q49->solve(q49));
	BAQuizRelease(q49);
	
    return 0;
}
