# Bad Apples Quiz Solver
A C / Lua bilingual solver program to solve all the quizzes in the iOS game Bad Apples. 

This is a hobby project done in 2011. Bad Apples is an iOS game including 100 different quizzes. Each quiz has a fixed 6*5 (6 rows, 5 columns) gridded playground, with 30 grids (30==6*5) in all.

In the playground, there are 3 normal types of fruit: yellow, red and blue. The player should make a move (I'll explain later) to form a horizontal or vertical line of 3 or more same kind of fruit (e.g. 3 red ones in a line) to eliminate them. The player must eliminate all fruit objects within a certain amount of moves to solve a quiz level. There are 100 levels in that game.

In addition to the 3 normal fruit objects, there are 2 special fruit objects: the bad apple (bad for short) and the eater. For the former one, if the player form a line of 3 bad apples, she loses this level. For the eater, after every move, the eater will fall down one grid vertically, eating the fruit object below it; and if the eater is already in the lowest row, it will fall out of screen.

