show_solve_step=false
show_cut_branch=false

BAQuiz={
   --quiz info: variable
   ROWS=6,
   COLS=5,
   N=0,
   solver={},
   eaters=0,
   --directions: constant(do NOT change)
   RIGHT=3,
   DOWN=6,
   LEFT=9,
   UP=12,
   --fruits: constant(do NOT change)
   NONE=0,
   YELLOW=1,
   BLUE=2,
   RED=3,
   BAD=4,
   EATER=5,
   new=
      function(self,rows,cols,s,n)
	 assert(rows*cols==#s,"Table string must be ROWS*COLS len")
	 local object={}
	 setmetatable(object,BAQuiz)
	 self.__index=BAQuiz
	 object.__index=BAQuiz
	 --init variables
	 object.ROWS=rows
	 object.COLS=cols
	 object.N=n
	 object.solver={}
	 --init self table
	 for r=1,rows do
	    object[r]={}
	    for c=1,cols do
	       local index=(r-1)*cols+c
	       local ch=string.sub(s,index,index)
	       if ' '==ch then
		  object[r][c]=self.NONE
	       elseif 'y'==ch then
		  object[r][c]=self.YELLOW
	       elseif 'b'==ch then
		  object[r][c]=self.BLUE
	       elseif 'r'==ch then
		  object[r][c]=self.RED
	       elseif 'x'==ch then
		  object[r][c]=self.BAD
	       elseif 'e'==ch then
		  object[r][c]=self.EATER
		  object.eaters=object.eaters+1
	       else
		  assert(false,"Unknown color in BAQuiz.new()")
	       end
	    end
	 end
	 return object
      end,
   copy=
      function(self,n)
	 local object={}
	 setmetatable(object,BAQuiz)
	 self.__index=BAQuiz
	 object.__index=BAQuiz
	 --init variables
	 object.ROWS=self.ROWS
	 object.COLS=self.COLS
	 object.N=n
	 object.solver=self.solver
	 object.eaters=self.eaters
	 --copy self table
	 for r=1,self.ROWS do
	    object[r]={}
	    for c=1,self.COLS do
	       object[r][c]=self[r][c]
	    end
	 end
	 return object
      end,
   __tostring=
      function(self)
	 local s=string.format("%d*%d,N=%d,e=%d\n",self.ROWS,self.COLS,self.N,self.eaters)
	 s=s..string.rep("*",self.COLS+2)
	 s=s.."\n"
	 for r=1,self.ROWS do
	    s=s.."*"
	    for c=1,self.COLS do
	       local item=self[r][c]
	       if self.NONE==item then
		  s=s..' '
	       elseif self.YELLOW==item then
		  s=s..'y'
	       elseif self.BLUE==item then
		  s=s..'b'
	       elseif self.RED==item then
		  s=s..'r'
	       elseif self.BAD==item then
		  s=s..'x'
	       elseif self.EATER==item then
		  s=s..'e'
	       else
		  assert(false,"Unknown color in BAQuiz.__tostring()")
	       end
	    end
	    s=s.."*\n"
	 end
	 s=s..string.rep("*",self.COLS+2)
	 return s
      end,
   move_item_at=
      function(self,r,c,d)
	 assert(not(self.NONE==self[r][c]))
	 if d==self.RIGHT then
	    if c<self.COLS then
	       self[r][c],self[r][c+1]=self[r][c+1],self[r][c]
	    end
	 elseif d==self.DOWN then
	    if r<self.ROWS then
	       self[r][c],self[r+1][c]=self[r+1][c],self[r][c]
	    end
	 elseif d==self.LEFT then
	    if c>1 then
	       self[r][c],self[r][c-1]=self[r][c-1],self[r][c]
	    end
	 elseif d==self.UP then
	    if r>1 then
	       self[r][c],self[r-1][c]=self[r-1][c],self[r][c]
	    end
	 else
	    assert(false,"Unknown Direction")
	 end
      end,
   fall=
      function(self)
	 for c=1,self.COLS do
	    local newCol={}
	    local newIndex=self.ROWS
	    for r=self.ROWS,1,-1 do
	       local item=self[r][c]
	       if not(item==self.NONE) then
		  newCol[newIndex]=item
		  newIndex=newIndex-1
	       end
	    end
	    for r=newIndex,1,-1 do
	       newCol[r]=self.NONE
	    end
	    for r=1,self.ROWS do
	       self[r][c]=newCol[r]
	    end
	 end
      end,
   max_same=
      function(self,r,c,d)
	 local ret=1
	 local item=self[r][c]
	 if d==self.RIGHT then
	    for cc=c+1,self.COLS do
	       if item==self[r][cc] then
		  ret=ret+1
	       else
		  break
	       end
	    end
	 elseif d==self.LEFT then
	    for cc=c-1,1,-1 do
	       if item==self[r][cc] then
		  ret=ret+1
	       else
		  break
	       end
	    end
	 elseif d==self.DOWN then
	    for rr=r+1,self.ROWS do
	       if item==self[rr][c] then
		  ret=ret+1
	       else
		  break
	       end
	    end
	 elseif d==self.UP then
	    for rr=r-1,1,-1 do
	       if item==self[rr][c] then
		  ret=ret+1
	       else
		  break
	       end
	    end
	 else
	    assert(false,"Unknown direction in max_same.")
	 end
	 return ret
      end,
   remove3=
      function(self)
	 local has_removed=0
	 --1.make a copy of self
	 local mark=self:copy(self.N)
	 --2.check3 and set mark
	 for r=1,self.ROWS do
	    for c=1,self.COLS do
	       if not(self.NONE==self[r][c]) and not(self.EATER==self[r][c]) then
		  local max=0
		  --test RIGHT
		  --条件: c不超过右数第3列
		  if c<=self.COLS-2 then
		     max=self:max_same(r,c,self.RIGHT)
		     if max>=3 then
			for cc=c,c+max-1 do
			   mark[r][cc]=self.NONE
			end
			if self[r][c]==self.BAD then
			   return -1
			else
			   has_removed=has_removed+1
			end
		     end
		  end
		  --test DOWN
		  --条件: r不超过下数第3行
		  if r<=self.ROWS-2 then
		     max=self:max_same(r,c,self.DOWN)
		     if max>=3 then
			for rr=r,r+max-1 do
			   mark[rr][c]=self.NONE
			end
			if self[r][c]==self.BAD then
			   return -1
			else
			   has_removed=has_removed+1
			end
		     end
		  end
	       end
	    end --end of for COL
	 end --end of for ROW
	 --3.set self with mark
	 for r=1,self.ROWS do
	    for c=1,self.COLS do
	       self[r][c]=mark[r][c]
	    end
	 end
	 return has_removed
      end,
   eat=
      function(self)
	 local NOTFOUND=-1
	 function find_first(item,r,c,d)
	    if r<1 or r>self.ROWS or c<1 or c>self.COLS then
	       return NOTFOUND
	    end
	    if d==self.UP then
	       for rr=r,1,-1 do
		  if self[rr][c]==item then
		     return rr
		  end
	       end
	       return NOTFOUND
	    else
	       assert(false,"Can't specify direction other than UP in eat.find_first()")
	    end
	 end
	 function find_first_not(item,r,c,d)
	    if r<1 or r>self.ROWS or c<1 or c>self.COLS then
	       return NOTFOUND
	    end
	    if d==self.UP then
	       for rr=r,1,-1 do
		  if not(self[rr][c]==item) then
		     return rr
		  end
	       end
	       return NOTFOUND
	    else
	       assert(false,"Can't specify direction other than UP in eat.find_first()")
	    end
	 end
	 for c=1,self.COLS do
	    local f_e=find_first(self.EATER,self.ROWS,c,self.UP)
	    local f_ne=find_first_not(self.EATER,self.ROWS,c,self.UP)
	    --if there are NO EATERS
	    if NOTFOUND==f_e then
	       --do nothing
	    --if there are ONLY EATERS
	    elseif NOTFOUND==f_ne then
	       assert(f_e==self.ROWS)
	       self[self.ROWS][c]=self.NONE
	       self.eaters=self.eaters-1
	    --if there are BOTH EATERS and NONEATERS
	    else
	       local r_e,r_ne=f_e,f_ne
	       --if lowest block is an EATER
	       if self[self.ROWS][c]==self.EATER then
		  self[self.ROWS][c]=self.NONE
		  self.eaters=self.eaters-1
		  r_e=find_first(self.EATER,r_ne,c,self.UP)
	       end
	       while not(r_e==NOTFOUND) and r_e>=1 do
		  self[r_e+1][c]=self.NONE
		  r_ne=find_first_not(self.EATER,r_e,c,self.UP)
		  r_e=find_first(self.EATER,r_ne,c,self.UP)
	       end
	    end
	 end
      end,
   do_step=
      function(self,n)
	 for i=1,n do
	    self:fall()
	    local ret=self:remove3()
	    if ret==0 then
	       break
	    elseif ret==-1 then
	       return false
	    end
	 end
	 if self.eaters>0 then
	    self:eat()
	    for i=1,n do
	       self:fall()
	       local ret=self:remove3()
	       if ret==0 then
		  return true
	       elseif ret==-1 then
		  return false
	       end
	    end
	 end
	 return true
      end,
   is_solved=
      function(self)
	 for r=1,self.ROWS do
	    for c=1,self.COLS do
	       local item=self[r][c]
	       if item==self.RED or item==self.BLUE or item==self.YELLOW then
		  return false
	       end
	    end
	 end
	 return true
      end,
   is_failed=
      function(self)
	 local yellow=0
	 local blue=0
	 local red=0
	 for r=1,self.ROWS do
	    for c=1,self.COLS do
	       local item=self[r][c]
	       if self.YELLOW==item then
		  yellow=yellow+1
	       elseif self.BLUE==item then
		  blue=blue+1
	       elseif self.RED==item then
		  red=red+1
	       elseif self.EATERS==item then
		  eater=eater+1
	       end
	    end
	 end
	 return (yellow==1 or yellow==2 or blue==1 or blue==2 or red==1 or red==2) and self.eaters==0
      end,
   solve=
      function(quiz)
	 if quiz.N<0 then
	    return false
	 elseif quiz.N==0 then
	    return quiz:is_solved()
	 elseif quiz:is_solved() then
	    return true
	 elseif quiz:is_failed() then
	    if show_cut_branch then
	       print ("solve(): cut branch with N is",quiz.N)
	    end
	    return false
	 else
	    for r=1,quiz.ROWS do
	       for c=1,quiz.COLS do
		  local item=quiz[r][c]
		  if not(quiz.NONE==item) then
		     --1.Move right and test
		     --条件 1.[c]不是第[quiz.COLS]列 2.当前item不同于它右侧的 
		     if c<quiz.COLS and not(item==quiz[r][c+1]) then
			local copy=quiz:copy(quiz.N-1)
			copy:move_item_at(r,c,3)
			if copy:do_step(100) and BAQuiz.solve(copy) then
			   table.insert(quiz.solver,"("..r..","..c..")R")
			   if show_solve_step then
			      print ("("..r..","..c..")R")
			      print(copy)
			   end
			   return true
			end
		     end
		     --2.Move left and test
		     --条件 1.[c]不是第1列 2.当前item左侧为NONE(否则，等价于左侧的向右移动)
		     if c>1 and quiz.NONE==quiz[r][c-1] then
			local copy=quiz:copy(quiz.N-1)
			copy:move_item_at(r,c,9)
			if copy:do_step(100) and BAQuiz.solve(copy) then
			   table.insert(quiz.solver,"("..r..","..c..")L")
			   if show_solve_step then
			      print ("("..r..","..c..")L")
			      print(copy)
			   end
			   return true
			end
		     end
		     --3.Move down and test
		     --条件 1.[r]不是第[quiz.ROWS]列 2.当前item不同于它下侧的
		     if r<quiz.ROWS and not(item==quiz[r+1][c]) then
			local copy=quiz:copy(quiz.N-1)
			copy:move_item_at(r,c,6)
			if copy:do_step(100) and BAQuiz.solve(copy) then
			   table.insert(quiz.solver,"("..r..","..c..")D")
			   if show_solve_step then
			      print("("..r..","..c..")D")
			      print(copy)
			   end
			   return true
			end
		     end
		     --4.因此，就不需要测试上移操作了
		  end --end of if not(quiz.NONE==quiz[r][c]) then
	       end --end of for COLS
	    end --end of for ROWS
	    return false
	 end --end of if elseif else
      end, --end of solve(quiz)
   get_solver=
      function(self)
	 local s=""
	 for i=#self.solver,1,-1 do
	    s=s..self.solver[i]
	 end
	 return s
      end
}

function Main()
   show_solve_step=false
   show_cut_branch=false

   -- local q1=BAQuiz:new(1,4,"bb b",1)
   -- print(q1)
   -- assert(BAQuiz.solve(q1))
   -- assert("(1,4)L"==q1:get_solver())

   -- local q2=BAQuiz:new(2,4,"br brb r",3)
   -- print(q2)
   -- assert(BAQuiz.solve(q2))
   -- assert("(1,1)D(1,4)L(2,4)L"==q2:get_solver())

   -- local q3=BAQuiz:new(6,6,"  r     b     b     r     b   rrb rr",1)
   -- print(q3)
   -- assert(BAQuiz.solve(q3))
   -- assert("(4,3)R"==q3:get_solver())

   -- local q4=BAQuiz:new(7,6,"  r     y     b     b     y   yyb   rrb yy",2)
   -- print(q4)
   -- assert(BAQuiz.solve(q4))
   -- assert("(1,3)D(5,3)R"==q4:get_solver())

   -- local q50=BAQuiz:new(9,6,"   b     r     r    rb    yr    yb    rbr yybyy ybybrb",4)
   -- print(q50)
   -- assert(BAQuiz.solve(q50))
   -- assert("(4,4)D(7,3)R(9,5)R(9,2)R"==q50:get_solver())

   -- --第51关，有bad apple
   -- local q51=BAQuiz:new(4,6,"  br   ryrbbxxrbxxyyrbxx",2)
   -- print(q51)
   -- assert(BAQuiz.solve(q51))
   -- assert("(1,3)R(1,3)D"==q51:get_solver())

   -- --第58关，有bad apple
   -- local q58=BAQuiz:new(4,6,"  y    xx   xrr y rxy y ",4)
   -- print(q58)
   -- assert(BAQuiz.solve(q58))
   -- assert("(3,5)R(1,3)R(3,3)R(3,2)D"==q58:get_solver())

   -- --第71关，有eater
   -- local q71 = BAQuiz:new(5, 6, "    r  y yb  yrre  rbey byybb ", 5)
   -- print(q71)
   -- assert(BAQuiz.solve(q71))
   -- assert("(2,4)L(5,1)R(4,3)R(5,2)R(4,4)D"==q71:get_solver())

   local q72=BAQuiz:new(5,6,"   r   rrx   xxb  bxbx  xbxb  ",5)
   print(q72)
   assert(BAQuiz.solve(q72))
   print(q72:get_solver())
end

Main()
