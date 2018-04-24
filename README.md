# Bad Apples Quiz Solver
A C / Lua bilingual solver program to solve all the quizzes in the iOS game *Bad Apples* - this is a hobby project done in 2011. 

## Basic rules
Bad Apples is an iOS game including 100 different quizzes. Each quiz has a fixed *N* x *M* (*N* rows, *M* columns) gridded playground, with *N* x *M* grids in all.

In the playground, there are 3 normal types of fruit: yellow (y for short), red (r for short) and blue (b for short). The player should make a move (I'll explain later) to form a horizontal or vertical "line of 3+": a horizontal or vertical (not diagonal) line of 3 or more same kind of fruit objects (e.g. 3 red ones in a horizontal line, or 5 blue ones in a vertical line) to eliminate them. The player must eliminate all fruit objects within a certain amount of moves to solve a quiz level. There are 100 levels in that game.

A simplest playground looks like this (we use a minus sign to mark an empty grid):

<pre>
Moves: 0/1  
  1 2 3 4 5  
1 - - - - -  
2 - - - - -  
3 - - - - -  
4 - - - - -  
5 - - - - -  
6 - b b - b  
</pre>

This is very easy. Just move the fruit object at (row=6, column=5) to the left, which can be recorded as (6,5)left, and it becomes:

<pre>
Moves: 1/1
  1 2 3 4 5  
1 - - - - -  
2 - - - - -  
3 - - - - -  
4 - - - - -  
5 - - - - -  
6 - b b b -  
</pre>

Please note that after (6,5)left, the "Moves" changes to "1/1". And since (6,3), (6,4), and (6,5) forms a "line of 3+", they are eliminated. So the playground becomes:

<pre>
Moves: 1/1 (solved)
  1 2 3 4 5
1 - - - - -  
2 - - - - -  
3 - - - - -  
4 - - - - -  
5 - - - - -  
6 - - - - -  
</pre>

Since there's no fruit object, you've solved this quiz level.

## Gravity
Please note that this game has gravity. So the fruit can not float in the air. If the player eliminate a line below a fruit object, it falls down after that move. See this example:

<pre>
Moves: 0/1
  1 2 3 4 5
1 - - - - -  
2 - - - - -  
3 - - - - -  
4 - - - - -  
5 - y y - -  
6 y b b - b  
</pre>

First you can not move the yellow one (6,1) up. Because above (6,1) is (5,1), which is an empty space. (6,1)up violates the gravity. However, (5,3)right is a valid move. After (5,3)y is moved to (5,4), it automatically falls down to (6,4):

<pre>
Moves: 0/1        Moves: 1/1        Moves: 1/1
  1 2 3 4 5         1 2 3 4 5         1 2 3 4 5
1 - - - - -       1 - - - - -       1 - - - - -
2 - - - - -  move 2 - - - - -  fall 2 - - - - -
3 - - - - -  ==>  3 - - - - -  ==>  3 - - - - -
4 - - - - -       4 - - - - -       4 - - - - -
5 - y y - -       5 - y - y -       5 - y - - -
6 y b b - b       6 y b b - b       6 y b b y b
</pre>

Clearly, that won't solve the quiz. Actually we should make use of the gravity and solve it with (6,5)left:

<pre>
Moves: 0/1        Moves: 1/1        Moves: 1/1         Moves: 1/1        Moves: 1/1 (solved)
  1 2 3 4 5         1 2 3 4 5         1 2 3 4 5          1 2 3 4 5         1 2 3 4 5
1 - - - - -       1 - - - - -       1 - - - - -        1 - - - - -       1 - - - - -
2 - - - - - move  2 - - - - -  elim 2 - - - - -  fall  2 - - - - -  elim 2 - - - - -
3 - - - - -  ==>  3 - - - - -  ==>  3 - - - - -  ==>   3 - - - - -  ==>  3 - - - - -
4 - - - - -       4 - - - - -       4 - - - - -        4 - - - - -       4 - - - - -
5 - y y - -       5 - y y - -       5 - y y - -        5 - - - - -       5 - - - - -
6 y b b - b       6 y b b b -       6 y - - - -        6 y y y - -       6 - - - - -
</pre>

From the above, we can see this is how gravity rule works:
1. The player makes a move
2. Fall phase: from the lowest row to the highest row, for each midair (the grid below is empty) fruit object, let it fall down until it reaches another fruit object or the bottom row
3. Elim phase: eliminate every "line of 3+"
4. If nothing is eliminated in the previous step, goto 5; else, goto 2
5. End of player's move

From step 4, it is clearly that a chain reaction can be triggered. In that case, it will look like:
Fall => Elim => Fall => Elim => Fall => Elim => etc.

## Special fruit objects
In addition to the 3 normal fruit objects, there are 2 special fruit objects: the bad apple (x for short) and the eater (e for short). For the former one, if the player form a line of 3 bad apples, she loses this level. For the eater, **after the end of player's move**, the eater falls one grid down, eating the fruit object (including a bad apple, but **not** another eater) below it; and if the eater is already at the lowest row, it will fall out of screen. See this example:

<pre>
Moves: 0/1        Moves: 1/1        Moves: 1/1         Moves: 1/1        Moves: 1/1         Moves: 1/1
  1 2 3 4 5         1 2 3 4 5         1 2 3 4 5          1 2 3 4 5         1 2 3 4 5          1 2 3 4 5
1 - - - - -       1 - - - - -       1 - - - - -        1 - - - - -       1 - - - - -        1 - - - - -
2 - - - - - move  2 - - - - -  elim 2 - - - - -  fall  2 - - - - -  elim 2 - - - - -  fall  2 - - - - - end
3 x - e - -  ==>  3 x - e - -  ==>  3 x - e - -  ==>   3 x - - - -  ==>  3 - - - - -  ==>   3 - - - - - of
4 e - x - -       4 e - x - -       4 e - x - -        4 e - e - -       4 x - e - -        4 x - - - - player's
5 e y y - -       5 e y y - -       5 e y y - -        5 e - x - -       5 e - x - -        5 e - e - - move
6 y b b e b       6 y b b b e       6 y - - - e        6 y y y - e       6 e - - - e        6 e - x - e
</pre>

After (6,5)left, the blue ones at the bottom line are eliminated, which triggers a "fall", which triggers an "elim", which then triggers a "fall" again. After that, every fruit object is either on another one, or on the ground, plus, there's no "line of 3+", so the player's move ends. Since the player's move has ended, the eater should start moving:

<pre>
Moves: 0/1        Moves: 1/1 (solved)
  1 2 3 4 5         1 2 3 4 5
1 - - - - -       1 - - - - -
2 - - - - -  eat  2 - - - - -
3 - - - - -  ==>  3 - - - - -  (end of eating)
4 x - - - -       4 - - - - -
5 e - e - -       5 x - - - -
6 e - x - e       6 e - e - -
</pre>

Now let's see what happens here by **column**. 
- In column 1, there are one bad apple at (4,1), two eaters at (5,1) and (6,1). (6,1)e falls out of the screen, and (5,1)e falls down one grid, and therefore (4,1)x falls with them. 
- In column 3, (5,3)e is above (6,3)x, so the eater eats the bad apple. 
- In column 5, the eater falls out of the screen.

**The player don't have to eliminate bad apples or eaters to solve the quiz.** That's why the last state is considered a win state.

## Complicated example
The most complicated level is level 50, which has a 9x6 playground with 54 grids:
<pre>
Moves: 0/5
  1 2 3 4 5 6
1 -   b - - -
2 -   r - - -
3 -   r - - -
4 - r b - - -
5 - y r - - -
6 - y b - - -
7 - r b r - - 
8 - y y b y y 
9 y b y b r b
</pre>

The official solution requires 5 moves. But actually 4 moves is sufficient:
(4,4)down, (7,3)right, (9,5)right, (9,2)right

You can verify the solution with the C or Lua program. The C program is very fast and highly optimized. The Lua program is relatively slow.

## The C code and Lua code
The C code includes 3 files: main.c, BAQuiz.h and BAQuiz.c. I used C here to simulate an objective mechanic like C++. In main.c, you can input new quizzes just as the examples:

```c
// create a quiz with 2 rows, 4 colums, and within 3 steps.
// two rows are: "br-b" and "b-r-", in the program we use a space for an empty grid 
BAQuiz* q2=BAQuizCreate(2, 4, "br brb r", 3);

// output the quiz
printf("%s",q2->tostring(q2));

// solve the quiz
assert(q2->solve(q2));

// free the memory used by the quiz
BAQuizRelease(q2);
```

The Lua version is very similar to the C version:

```lua
local q72=BAQuiz:new(5,6,"   r   rrx   xxb  bxbx  xbxb  ",5)
print(q72)
assert(BAQuiz.solve(q72))
print(q72:get_solver())
```
