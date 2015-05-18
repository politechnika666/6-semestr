with Ada.Text_Io;  						 use Ada.Text_Io; 
with Ada.Numerics;                       use Ada.Numerics;
with Ada.Numerics.Float_Random;          use Ada.Numerics.Float_Random;
with Ada.Numerics.Elementary_Functions;  use Ada.Numerics.Elementary_Functions;
with Random_Seeds; 						 use Random_Seeds;
with Configuration; 					 use Configuration;

procedure Protect is

	type bossTask is record
		number1: Integer;
		number2: Integer;
		operator: Integer;
	end record;
	type TaskIndex is mod tasksCapacity;
	lastTask:bossTask;
	TheLastElem:Integer:=0;
	type TaskList_Array is array(TaskIndex) of bossTask;
	G : Generator;


	protected TaskList is
		entry Append(I:in bossTask);
		entry Take(I:out bossTask);
	private
		TL:TaskList_Array;
		In_Ptr, Out_Ptr, Count:TaskIndex:=0;
	end TaskList;

	protected body TaskList is
		entry Append(I:in bossTask) when Count<TaskIndex'Last is
		begin
			TL(In_Ptr):=I; 
			Count:= Count+1;
			TheLastElem:= TheLastElem+1;
			lastTask:=I;
			In_Ptr := In_Ptr+1;
		end Append;
		entry Take(I:out bossTask) when Count>0 is
		begin
			I:=TL(Out_Ptr);
			Count:=Count-1;
			TheLastElem:=TheLastElem-1;
			lastTask:=I;
			Out_Ptr:= Out_Ptr+1;
		end Take;
	end TaskList;

	--type Index is mod warehouseCapacity;
	type Index1 is mod warehouseCapacity;
	type Buffer1_Warehouse is array(Index1) of Integer;
	--type Buffer_Array is array(Index) of Integer;
	RNG : Generator; 
	TheLast1:Index1;
	--Ostatni:Integer:=0;		--ostatni elem na liscie
	Ostatni1:integer:=0;
	--AllTasks:Integer:=0;	--wszystkie zadania
	AllTasks1:integer:=0;
	--TimeDelay: Duration;


	protected Buffer1 is
		entry Append1(I1:in Integer);
		entry Take1(I1:out Integer);
	private 
		W:Buffer1_Warehouse;
		In_ptr1, Out_Ptr1, Count1:Index1:=0;
	end Buffer1;

	protected body Buffer1 is
		entry Append1(I1:in Integer) when Count1<Index1'Last is
		begin
			W(In_Ptr1):=I1;
			--Put_Line("Into Append1");
			--Put_Line("buff1 " & Index1'Image(TheLast));
			Count1:=Count1+1;	
			AllTasks1:=AllTasks1+1;
			Ostatni1:=Ostatni1+1;		
			--TheLast:=Count1;
			In_Ptr1 := In_Ptr1+1;
			--Put_Line("buff1 Ptr" & Index1'Image(In_Ptr1));
		end Append1;
		entry Take1(I1:out Integer) when Count1>0 is
		begin
			I1:=W(Out_Ptr1);
			Count1:=Count1-1;
			TheLast1:=Count1;
			Ostatni1:=ostatni1-1;
			Out_Ptr1:=Out_Ptr1+1;
		end Take1;
	end Buffer1;

	task type Boss(ID:Integer);
	task body Boss is
	TimeDelay:Float;
	zad:bossTask;
	
	begin
		loop
			zad.number1:=1;
			zad.number2:=1;
			zad.operator:=1;
			TimeDelay:=Random(RNG);
			delay Duration(TimeDelay);
			--Put_Line("TheLast " & Index1'Image(TheLast));	
			Put_Line("Boss " & Integer'Image(ID) & " Producing task ");
			TaskList.Append(zad);
		end loop;
	end Boss;

	task type Worker(ID:Integer);
	task body Worker is 
		taskToDo:bossTask;
		N:Integer;
		TimeDelay:Float;
		M:bossTask;
	begin 
		loop
			N:=0;
			TimeDelay:=Random(RNG);
			delay Duration(TimeDelay+0.5);
			--check if there is any task to do
			--if arrLength > 0 then
				M:=lastTask;
				taskToDo:=lastTask;
				case taskToDo.operator is
					when 0 =>
						N:=taskToDo.number1 + taskToDo.number2;

					when 1 => 
						N:=taskToDo.number1 - taskToDO.number2;

					when 2 => 
						N:=taskToDo.number1 * taskToDo.number2;

					when others =>
						N:=0;

				end case;

				TaskList.Take(lastTask);
				Put_Line("Worker " & Integer'Image(ID) & " Working with task");
				Buffer1.Append1(N);
			--end if;
		end loop;
	end Worker;

	task type Client(ID:Integer);
	task body Client is
		N:Integer;
		TimeDelay:Float;
	begin
		loop
			N:=0;
			TimeDelay:=Random(RNG);
			delay Duration(TimeDelay+1.0);
			Buffer1.Take1(N);
			Put_Line("Client " & Integer'Image(ID) & " bought product ");
			N:=N+1;
		end loop;
	end Client;

	B1:Boss(1);
	
	type WorkerArray_Type is array ( Positive range <>) of access Worker;
    type Worker_AT is access WorkerArray_Type;

	type ClientArray_Type is array ( Positive range <>) of access Client;
	type Client_AT is access ClientArray_Type;

    function Init_Workers(Size : Integer) return Worker_AT is
		P_Ptr : Worker_AT;
	    begin
		P_Ptr := new WorkerArray_Type(1 .. Size);
		for I in P_Ptr.all'Range loop
		    P_Ptr.all(I) := new Worker(I);
		end loop;
		return P_Ptr;
	end Init_Workers;

	function Init_Clients(Size : Integer) return Client_AT is
		C_Ptr : Client_AT;
	    begin
		C_Ptr := new ClientArray_Type(1 .. Size);
		for I in C_Ptr.all'Range loop
		    C_Ptr.all(I) := new Client(I);
		end loop;
		return C_Ptr;
	end Init_Clients;

	producersArray : WorkerArray_Type := Init_Workers(workersNumber).all;
	clientsArray : ClientArray_Type := Init_Clients(clientNumber).all;

	begin
		null;
	end Protect;