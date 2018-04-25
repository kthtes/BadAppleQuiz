/*
 *  BAQuiz.c
 *  BAQuiz
 *
 *  Created by 尹 伊 on 11-2-16.
 *  Copyright 2011 286studio. All rights reserved.
 *
 */

#include "BAQuiz.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SWAP(i1,i2) {int temp=i1;i1=i2;i2=temp;}

char* _tostring(BAQuiz* q){
	static char s[1024];
	static char info[80];
	memset(s,'\0',1024);
	memset(info,'\0',80);
	//1.create info string
	sprintf(info,"%d*%d,N=%d,e=%d\n",q->ROWS,q->COLS,q->N,q->eaters);
	strcat(s,info);
	//2.create a line of '*'
	int starCount=q->COLS+2;
	char* stars=malloc(starCount+2);
	memset(stars,'*',starCount);
	stars[starCount]='\n';
	stars[starCount+1]='\0';
	strcat(s,stars);
	//3.create string
	for(int r=0;r<q->ROWS;r++){
		strcat(s,"*");
		for(int c=0;c<q->COLS;c++){
			switch(q->table[r][c]){
				case NONE:
					strcat(s," ");
					break;
				case YELLOW:
					strcat(s,"y");
					break;
				case BLUE:
					strcat(s,"b");
					break;
				case RED:
					strcat(s,"r");
					break;
				case BAD:
					strcat(s,"x");
					break;
				case EATER:
					strcat(s,"e");
					break;
				default:
					printf("Unknown color in tostring()");
					assert(0);
					break;
			}
		}
		strcat(s,"*\n");
	}
	strcat(s,stars);
	free(stars);
	return s;
}
void _move_item_at(BAQuiz* q,int r,int c,int d){
	assert(q->table[r][c]!=NONE);
	if(RIGHT==d){
		if(c<q->COLS-1)
			SWAP(q->table[r][c],q->table[r][c+1]);
	}else if(LEFT==d){
		if(c)
			SWAP(q->table[r][c],q->table[r][c-1]);
	}else if(DOWN==d){
		if(r<q->ROWS-1)
			SWAP(q->table[r][c],q->table[r+1][c]);
	}else if(UP==d){
		if(r)
			SWAP(q->table[r][c],q->table[r-1][c]);
	}else{
		printf("Unknown direction in move_item_at()");
		assert(0);
	}
}
void _fall(BAQuiz* q){
	int size=q->ROWS*sizeof(int);
	int* newCol=malloc(size);
	for(int c=0;c<q->COLS;c++){
		memset(newCol,0,size);
		int newIndex=q->ROWS-1;
		for(int r=newIndex;r>=0;r--){
			int item=q->table[r][c];
			if(NONE!=item){
				newCol[newIndex]=item;
				newIndex--;
			}
		}
		for(int r=newIndex;r>=0;r--)
			newCol[r]=NONE;
		for(int r=0;r<q->ROWS;r++)
			q->table[r][c]=newCol[r];
	}
	free(newCol);
}
int _max_same(BAQuiz* q,int r,int c,int d){
	int ret=1;
	int item=q->table[r][c];
	if(RIGHT==d){
		for(int cc=c+1;cc<q->COLS;cc++){
			if(item==q->table[r][cc])
				ret++;
			else
				break;
		}
	}else if(DOWN==d){
		for(int rr=r+1;rr<q->ROWS;rr++){
			if(item==q->table[rr][c])
				ret++;
			else
				break;
		}
	}else if(LEFT==d){
		for(int cc=c-1;cc>=0;cc--){
			if(item==q->table[r][cc])
				ret++;
			else
				break;
		}
	}else if(UP==d){
		for(int rr=r-1;rr>=0;rr--){
			if(item==q->table[rr][c])
				ret++;
			else
				break;
		}
	}else{
		printf("Unknown direction in max_same()");
		assert(0);
	}
	return ret;
}
int _remove3(BAQuiz* q){
	int has_removed=0;
	//1.make a copy of q
	BAQuiz* mark=BAQuizCopy(q, q->N);
	//2.check3 and set mark
	for(int r=0;r<q->ROWS;r++){
		for(int c=0;c<q->COLS;c++){
			if(NONE!=q->table[r][c] && EATER!=q->table[r][c]){
				int max=0;
				//test RIGHT
				//条件: [c]<[COLS-2]
				if(c<q->COLS-2){
					max=q->max_same(q,r,c,RIGHT);
					if(max>=3){
						for(int cc=c;cc<c+max;cc++)
							mark->table[r][cc]=NONE;
						if(BAD==q->table[r][c])
							return REMOVEBADAPPLE;
						else
							has_removed++;
					}
				}
				//test DOWN
				//条件: [r]<[ROWS-2]
				if(r<q->ROWS-2){
					max=q->max_same(q,r,c,DOWN);
					if(max>=3){
						for(int rr=r;rr<r+max;rr++)
							mark->table[rr][c]=NONE;
						if(BAD==q->table[r][c])
							return REMOVEBADAPPLE;
						else
							has_removed++;
					}
				}
			}//end of if(NONE!=...
		}//end of for COLS
	}//end of for ROWS
	//3.set q's table with mark's
	for(int r=0;r<q->ROWS;r++)
		for(int c=0;c<q->COLS;c++)
			q->table[r][c]=mark->table[r][c];
	BAQuizRelease(mark);
	return has_removed;
}
int _find_first(BAQuiz* q,int item,int r,int c,int d){
	if(r<0||r>=q->ROWS||c<0||c>=q->COLS)
		return NOTFOUND;
	if(UP==d){
		for(int rr=r;rr>=0;rr--)
			if(q->table[rr][c]==item)
				return rr;
		return NOTFOUND;
	}else{
		printf("Can't specify direction other than UP in find_first().");
		assert(0);
	}
}
int _find_first_not(BAQuiz* q,int item,int r,int c,int d){
	if(r<0||r>=q->ROWS||c<0||c>=q->COLS)
		return NOTFOUND;
	if(UP==d){
		for(int rr=r;rr>=0;rr--)
			if(q->table[rr][c]!=item)
				return rr;
		return NOTFOUND;
	}else{
		printf("Can't specify direction other than UP in find_first_not().");
		assert(0);
	}	
}
void _eat(BAQuiz* q){
	for(int c=0;c<q->COLS;c++){
		int f_e=q->find_first(q,EATER,q->ROWS-1,c,UP);
		int f_ne=q->find_first_not(q,EATER,q->ROWS-1,c,UP);
		//if there are NO EATER
		if(NOTFOUND==f_e)
			continue;
		//if there are ONLY EATERS
		else if(NOTFOUND==f_ne){
			assert(f_e==q->ROWS-1);
			q->table[f_e][c]=NONE;
			q->eaters--;
		}
		//if there are BOTH EATERS and NONEATERS
		else{
			int r_e=f_e;
			int r_ne=f_ne;
			//if the lowest block is an EATER
			if(EATER==q->table[q->ROWS-1][c]){
				q->table[q->ROWS-1][c]=NONE;
				q->eaters--;
				r_e=q->find_first(q,EATER,r_ne,c,UP);
			}
			while(r_e!=NOTFOUND && r_e>=0){
				q->table[r_e+1][c]=NONE;
				r_ne=q->find_first_not(q,EATER,r_e,c,UP);
				r_e=q->find_first(q,EATER,r_ne,c,UP);
			}
		}//end of if(NOTFOUND==f_e)... elseif... else
	}//end of for(int c=0;c<q->COLS...
}
int _do_step(BAQuiz* q,int n){
	n=(n<0?10:n);
	for(int i=0;i<n;i++){
		q->fall(q);
		int removes=q->remove3(q);
		if(0==removes)
			break;
		else if(REMOVEBADAPPLE==removes)
			return 0;
	}
	if(q->eaters>0){
		q->eat(q);
		for(int i=0;i<n;i++){
			q->fall(q);
			int removes=q->remove3(q);
			if(0==removes)
				return 1;
			else if(REMOVEBADAPPLE==removes)
				return 0;
		}
	}
	return 1;
}
int _is_solved(BAQuiz* q){
	for(int r=0;r<q->ROWS;r++)
		for(int c=0;c<q->COLS;c++)
			if(RED==q->table[r][c]||BLUE==q->table[r][c]||YELLOW==q->table[r][c])
				return 0;
	return 1;
}
int _is_failed(BAQuiz* q){
	int yellow=0,blue=0,red=0;
	for(int r=0;r<q->ROWS;r++)
		for(int c=0;c<q->COLS;c++){
			switch (q->table[r][c]) {
				case YELLOW:
					yellow++;
					break;
				case BLUE:
					blue++;
					break;
				case RED:
					red++;
					break;
				default:
					break;
			}
		}
	return q->eaters==0 && (yellow==1||yellow==2||blue==1||blue==2||red==1||red==2);
}
int _solve(BAQuiz* q){
	if(q->is_solved(q))
		return 1;
	else if(q->N<=0)
		return 0;
	else if(q->is_failed(q)){
		if(SHOW_CUT_BRANCH)
			printf("solve(): cut branch with N=%d\n",q->N);
		return 0;
	}else{
		for(int r=0;r<q->ROWS;r++){
			for(int c=0;c<q->COLS;c++){
				int item=q->table[r][c];
				if(NONE!=item){
					BAQuiz* copy=BAQuizCopy(q, q->N-1);
					//1.Move right and test
					//条件 1.[c]不是第[q->COL-1]列 2.当前item和右侧的不一样
					if(c<q->COLS-1 && item!=q->table[r][c+1]){
						copy->move_item_at(copy,r,c,RIGHT);
						if(copy->do_step(copy,-1) && copy->solve(copy)){
							if(SHOW_SOLVE_STEP){
								printf("(%d,%d)R\n",r,c);
								printf("%s",copy->tostring(copy));
							}
							BAQuizRelease(copy);
							return 1;
						}
					}
					//2.Move left and test
					//条件 1.[c]不是第0列 2.当前item和左侧的不一样
					if(c>0 && NONE==q->table[r][c-1]){
						BAQuizAssaign(copy, q);
						copy->N=q->N-1;
						copy->move_item_at(copy,r,c,LEFT);
						if(copy->do_step(copy,-1) && copy->solve(copy)){
							if(SHOW_SOLVE_STEP){
								printf("(%d,%d)L\n",r,c);
								printf("%s",copy->tostring(copy));
							}
							BAQuizRelease(copy);
							return 1;
						}
					}
					//3.Move down and test
					//条件 1.[r]不是第[q->ROWS-1]行 2.当前item和下面的不一样
					if(r<q->ROWS-1 && item!=q->table[r+1][c]){
						BAQuizAssaign(copy, q);
						copy->N=q->N-1;
						copy->move_item_at(copy,r,c,DOWN);
						if(copy->do_step(copy,-1) && copy->solve(copy)){
							if(SHOW_SOLVE_STEP){
								printf("(%d,%d)D\n",r,c);
								printf("%s",copy->tostring(copy));
							}
							BAQuizRelease(copy);
							return 1;
						}
					}
					BAQuizRelease(copy);
					//4.因此，不需要进行向上的判断
				}//end of if(NONE!=item)
			}//end of for(COLS)
		}//end of for(ROWS)
		return 0;
	}//end of if-else if-else
}
BAQuiz* BAQuizCreateBasic(int rows,int cols){
	struct BAQuiz* object=malloc(sizeof(BAQuiz));
	//1.初始化变量
	object->ROWS=rows;
	object->COLS=cols;
	object->table=malloc(rows*sizeof(int*));	
	for(int r=0;r<rows;r++)
		object->table[r]=malloc(cols*sizeof(int));
	//2.初始化函数
	object->tostring=_tostring;
	object->move_item_at=_move_item_at;
	object->fall=_fall;
	object->max_same=_max_same;
	object->remove3=_remove3;
	object->find_first=_find_first;
	object->find_first_not=_find_first_not;
	object->eat=_eat;
	object->do_step=_do_step;
	object->is_solved=_is_solved;
	object->is_failed=_is_failed;
	object->solve=_solve;
	return object;
}
void BAQuizRelease(BAQuiz* q){
	for(int r=0;r<q->ROWS;r++)
		free(q->table[r]);
	free(q->table);
	free(q);	
}
BAQuiz* BAQuizCreate(int rows,int cols,const char* s,int n){
	struct BAQuiz* object=BAQuizCreateBasic(rows, cols);
	object->eaters=0;
	for(int r=0;r<rows;r++){
		for(int c=0;c<cols;c++){
			switch (s[r*cols+c]) {
				case ' ':
					object->table[r][c]=NONE;
					break;
				case 'y':
					object->table[r][c]=YELLOW;
					break;
				case 'b':
					object->table[r][c]=BLUE;
					break;
				case 'r':
					object->table[r][c]=RED;
					break;
				case 'x':
					object->table[r][c]=BAD;
					break;
				case 'e':
					object->table[r][c]=EATER;
					object->eaters++;
					break;
				default:
					printf("Unknown color in BAQuizCreate()");
					assert(0);
					break;
			}
		}
	}
	object->N=n;
	return object;	
}
BAQuiz* BAQuizCopy(BAQuiz* q,int n){
	BAQuiz* object=BAQuizCreateBasic(q->ROWS,q->COLS);
	for(int r=0;r<q->ROWS;r++)
		memcpy(object->table[r],q->table[r],q->COLS*sizeof(int));
	object->eaters=q->eaters;
	object->N=n;
	return object;
}
void BAQuizAssaign(BAQuiz* q1,const BAQuiz* q2){
	assert(q2->ROWS==q1->ROWS && q2->COLS==q1->COLS);
	for(int r=0;r<q1->ROWS;r++)
		memcpy(q1->table[r],q2->table[r],q1->COLS*sizeof(int));
	q1->N=q2->N;
}
