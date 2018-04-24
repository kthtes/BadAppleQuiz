# Bad Apples Quiz Solver
A C / Lua bilingual solver program to solve all the quizzes in the iOS game Bad Apples. 

This is a hobby project done in 2011. Bad Apples is an iOS game including 100 different quizzes. Each quiz has a fixed 6*5 (6 rows, 5 columns) gridded playground, with 30 grids (30==6*5) in all.

## Basic rules
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
Moves: 0/1        Moves: 1/1        Moves: 1/1        Moves: 1/1        Moves: 1/1 (solved)
  1 2 3 4 5         1 2 3 4 5         1 2 3 4 5        1 2 3 4 5         1 2 3 4 5
1 - - - - -       1 - - - - -       1 - - - - -        1 - - - - -       1 - - - - -
2 - - - - - move  2 - - - - -  elim 2 - - - - -  fall  2 - - - - -  elim 2 - - - - -
3 - - - - -  ==>  3 - - - - -  ==>  3 - - - - -  ==>   3 - - - - -  ==>  3 - - - - -
4 - - - - -       4 - - - - -       4 - - - - -        4 - - - - -       4 - - - - -
5 - y y - -       5 - y y - -       5 - y y - -        5 - - - - -       5 - - - - -
6 y b b - b       6 y b b b -       6 y - - - -        6 y y y - -       6 - - - - -
</pre>

From the above, we can see this is how gravity rule works:
1. The player makes a move
2. Fall phase: for each midair (the grid below is empty) fruit object, let it fall down until it reaches another fruit object or the bottom row
3. Elim phase: eliminate every "line of 3+"
4. If nothing is eliminated in the previous step, goto 5; else, goto 2
5. End of player's move

From step 4, it is clearly that a chain reaction can be triggered. In that case, it will look like:
Fall => Elim => Fall => Elim => Fall => Elim => etc.

## Special fruit objects
In addition to the 3 normal fruit objects, there are 2 special fruit objects: the bad apple (b for short) and the eater (e for short). For the former one, if the player form a line of 3 bad apples, she loses this level. For the eater, after every move, the eater will fall down one grid vertically, eating the fruit object (including a bad apple) below it; and if the eater is already in the lowest row, it will fall out of screen.

