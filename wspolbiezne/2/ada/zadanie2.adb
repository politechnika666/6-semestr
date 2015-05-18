with Ada.Text_IO,Ada.Numerics,Ada.Numerics.Discrete_Random;
package body Zadanie1 is
	task body HandyMan is
		num : err_raport;
	begin	
		num.num:=0;	
		loop
			M.PopRaport(num);
			if (num.m_type=1)then
				add_array(num.num).Repeir;
				add_machine_array(num.num):=FALSE;
				if(mode) then 
					Ada.Text_IO.Put_Line("Naprawa  "& Integer'Image(num.num) & ": error");
				end if;
			end if;
			delay Duration(handyman_speed);			
		end loop;
	end HandyMan;
	
	protected body MachineManager is
		entry PushAdd (num : in Integer) when TRUE is
		begin
			add_machine_array(num):=FALSE;
		end PushAdd;
		entry PopAdd (num,err:out Integer) when TRUE is
		begin
			for i in 1 .. add_machine_num loop
				if (not add_machine_array(i))then
					add_machine_array(i):=TRUE;
					num := i;
					err:=0;
					return;
				end if;
			end loop;
			err:=1;
		end PopAdd;
		entry PushRaport (raport:in err_raport) when top_err<=add_machine_num is 
		begin
			top_err :=top_err+1;
			raport_array(top_err):=raport;
		end PushRaport;
		entry PopRaport(num:out err_raport)when top_err>0 is
		begin

			num:=raport_array(1);
			for i in 1 .. top_err-1 loop
				raport_array(i) := raport_array(i+1);
			end loop;
			top_err :=top_err-1;
		end PopRaport;
		--procedure PrintList;
	end MachineManager;
	protected body AddMachine is
		entry Add (arg1,arg2 : in Integer;output , err : out Integer)when broken=FALSE  is
			type Rand_Range is range 0..100;
			package Rand_Int is new Ada.Numerics.Discrete_Random(Rand_Range);
			seed : Rand_Int.Generator;
			p,q:Integer;
		begin
			Rand_Int.Reset(seed);
			p:=Integer(Rand_Int.Random(seed));
			q:=Integer(add_machine_unre*100.0);
			if( q > p ) then
				delay Duration (add_machine_time);
				output:=arg1+arg2;
				err:=0;
			else
				--Ada.Text_IO.Put_Line("pq  "& Integer'Image(q) & ": error");
				err:=1;
				broken := TRUE;
			end if;
			--delay Duration (add_machine_time);
			
		end Add;
		entry Repeir when broken=TRUE is
		begin
			delay Duration (add_machine_unre);
			broken:=FALSE;
		end Repeir;
		
	end AddMachine;
	protected body TaskList is
		entry Push (arg1,arg2,arg3 : in Integer) when top_task < max_task is
			t : s_task;
		begin
			top_task:=top_task+1;
			t.arg1:=arg1;
			t.arg2:=arg2;
			t.op:=arg3;
			task_array(top_task):=t;			
		end Push;
		
		entry Pop (arg1,arg2,arg3:out Integer) when top_task >0 is
			t:s_task;
		begin 
			t:=task_array(1);
			arg1:=t.arg1;
			arg2:=t.arg2;
			arg3:=t.op;
			for i in 1 .. top_task-1 loop
				task_array(i) := task_array(i+1);
			end loop;
			top_task:=top_task-1;		
		end Pop;
		procedure PrintList is
		begin
			for i in 1 .. top_task loop
					Ada.Text_IO.Put(Integer'Image(task_array(i).arg1)& " ");
					Ada.Text_IO.Put(Integer'Image(task_array(i).arg2)&" ");
					Ada.Text_IO.Put_Line(Integer'Image(task_array(i).op));
			end loop;		
		end PrintList;
	end TaskList;
	protected body StoreList is
		entry Push (arg : in Integer) when top_store < max_store is
		begin
			top_store:=top_store+1;
			store_array(top_store):=arg;
		end Push;
		entry Pop (arg : out Integer) when top_store >0 is
		begin 
			arg := store_array(1);
			for i in 1 .. top_store-1 loop
				store_array(i) := store_array(i+1);
			end loop;
			top_store:=top_store-1;		
		end Pop;
		procedure PrintList is
		begin
			for i in 1 .. top_store loop
				Ada.Text_IO.Put_Line(Integer'Image(store_array(i)));
			end loop;		
		end PrintList;
	end StoreList;
	
	task body Ui is
		Line : String (1 .. 1);
  		Last : Natural;
	begin 
		loop
			Ada.Text_IO.Put_Line("1)Print store state ");
			Ada.Text_IO.Put_Line("2)Print task list ");
			Ada.Text_IO.Get_Line (Line, Last);
			--Ada.Text_IO.Put_Line(Natural'Image(Last));
			if(Integer'Value(Line)=1)then
				S.PrintList;
				Line:="0";			
			elsif (Integer'Value(Line)=2) then
				T.PrintList;
				Line:="0";
			elsif  (Integer'Value(Line)=0) then
				Line:="0";
			else
				Ada.Text_IO.Put_Line("Error");
				Line:="0";
			end if;
			Line:="0";
			--Ada.Text_IO.Put(ASCII.ESC & "[2J");
		end loop;
	end Ui;
	task body customer is
		value :Integer;
	begin 
		loop	 
			S.Pop(value);
			if (mode)then
				Ada.Text_IO.Put_Line("Customer: get value:"&Integer'Image(value));			
			end if;
			delay Duration (customer_speed);
		end loop;
	
	end customer;
	
	task body employe is
		Num:Integer;
		Num1:Integer;
		Op:Integer;
		Id:integer;
		r: err_raport ;
		output,err,err_machine_busy,m_num:Integer;
	begin
		
		Id := empId;
		empId:=empId+1;
		loop
			err_machine_busy:=1;
			err:=1;
			T.Pop(Num,Num1,Op);
			if (Op=1)then
				while (err_machine_busy = 1 or err=1) loop 
					M.PopAdd(m_num,err_machine_busy);
					add_array(m_num).Add(Num,Num1,output,err);
					if (err = 1)then 
						r.num:=m_num;
						r.m_type:=1;
						M.PushRaport(r);
						Ada.Text_IO.Put_Line("Employe_"& Integer'Image(Id) & ": error on machine number: "& Integer'Image(m_num) );
					end if;
					delay Duration (employe_speed);
				end loop;
				M.PushAdd(m_num);
				--output:=Num+Num1;
			elsif (Op=2) then
				output:=Num-Num1;
			elsif (Op=3) then 
				output:=Num*Num1;
			elsif (Op=4) then
				output:=Num/Num1;	
			end if;
			if(err=0) then
				S.Push(output);
			else
				T.Push(Num,Num1,Op);
			end if;
			if (mode and err=0)then
				Ada.Text_IO.Put_Line("Employe_"& Integer'Image(Id) & ": get task:" & Integer'Image(Num) &":"& Integer'Image(Num1) &":"& Integer'Image(Op)&" ;machine: "&Integer'Image(m_num));
				Ada.Text_IO.Put_Line("Employe_"& Integer'Image(Id) & ": put "& Integer'Image(output)&" store");
			elsif (mode and err=1) then
				Ada.Text_IO.Put_Line("Employe_"& Integer'Image(Id) & ": error"& Integer'Image(err) );
			end if;
			delay Duration (employe_speed);
		end loop;
	end employe; 
	--begin of boss body 
	task body boss is
   		type Rand_Range is range 0..100;
   		type Rand_Range2 is range 1..4;
		package Rand_Int is new Ada.Numerics.Discrete_Random(Rand_Range);
		package Rand_Op is new Ada.Numerics.Discrete_Random(Rand_Range2);
		seed : Rand_Int.Generator;
		seed2 : Rand_Op.Generator;
		Num : Rand_Range;
		Num2 : Rand_Range;
		Op : Rand_Range2;
   	begin
   		loop   				
			Rand_Int.Reset(seed);
			Rand_Op.Reset(seed2);
			Op := Rand_Op.Random(seed2);
			Num := Rand_Int.Random(seed);
			Num2 := Rand_Int.Random(seed);
			--Ada.Text_IO.Put_Line(":" & Rand_Range'Image(Num) &":"& Rand_Range'Image(Num2) &":"& Rand_Range2'Image(Op));
			T.Push(Integer(Num),Integer(Num2),Integer(1));
			if(mode) then 
				Ada.Text_IO.Put_Line("Boss: add task:" & Rand_Range'Image(Num) &":"& Rand_Range'Image(Num2) &":"& Rand_Range2'Image(Op));
			end if;
			delay Duration (bos_speed);
		end loop;
	end boss;
	procedure init  is
		T : aliased TaskList;
		S : aliased StoreList;
		Task_customer: access customer;
		Taks_boss : access boss;
		Handy_task:access HandyMan;
		M:aliased MachineManager;
		Task_employe: array (1 .. employe_num) of access employe;
		Task_Ui:access Ui;
		--add_array:array (1 .. add_machine_num) of access AddMachine;
	begin 

		Taks_boss := new boss(T'access);
		Task_customer:= new customer(S'access);
		
		for i in 1 .. add_machine_num loop
			add_array(i):=new AddMachine;	
		end loop;
		Handy_task := new HandyMan(M'access);		
		for i in 1 .. employe_num loop 
			Task_employe(i):=new employe(T'access,S'access,M'access);	
		end loop;
		if (mode=FALSE)then
			Task_Ui:=new Ui(T'access,S'access);	
		end if;
	end init;

begin
	init;
end Zadanie1;