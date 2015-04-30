with Ada.Text_Io; use Ada.Text_Io;
with Ada.Integer_Text_IO;
with Ada.Numerics;
with Ada.Numerics.Discrete_Random;
with Config; use Config;

procedure Company is
	type taskFromBoss is record
		number1: Integer;
		number2: Integer;
		action: Integer;
	end record;
	type TaskIndex is mod List_max+1;
	type StoreIndex is mod Storage_max+1;
	lastTask:taskFromBoss;
	product:Integer:= 0;
	numberOf:Integer:= 0;
	type TaskList_Array is array(TaskIndex) of taskFromBoss;
	type Storage_Array is array(StoreIndex) of Integer;

	protected TaskList is
		entry Append(I:in taskFromBoss);
		entry Take(I:out taskFromBoss);
	private
		TL:TaskList_Array;
		In_Ptr, Out_Ptr, Count:TaskIndex:=0;
	end TaskList;

	protected body TaskList is
		entry Append(I:in taskFromBoss) when Count<TaskIndex'Last is
		begin
			TL(In_Ptr):=I; 
			lastTask:=I;
			numberOf:=numberOf+1;
			Count:= Count+1;
			In_Ptr := In_Ptr+1;
		end Append;
		entry Take(I:out taskFromBoss) when Count>0 is
		begin
			I:=TL(Out_Ptr);
			lastTask:=I;
			numberOf:=numberOf-1;
			Count:=Count-1;
			Ada.Integer_Text_IO.put(I.number1 );
			Ada.Integer_Text_IO.put(I.number2);
			Ada.Integer_Text_IO.put(I.action);
			Out_Ptr:= Out_Ptr+1;
		end Take;
	end TaskList;

	protected Storage is
	entry Append(I:in Integer);
	entry Take(I:out Integer);
	private
		S:Storage_Array;
		In_Ptr, Out_Ptr, Count:StoreIndex:=0;
	end Storage;

	protected body Storage is
		entry Append(I:in Integer) when Count<StoreIndex'Last is
		begin
			S(In_Ptr):=I; 
			Count:= Count+1;
			product:=I;
			In_Ptr := In_Ptr+1;
		end Append;
		entry Take(I:out Integer) when Count>0 is
		begin
			I:=S(Out_Ptr);
			Count:=Count-1;
			product:=I;
			Ada.Integer_Text_IO.put(I);
			New_Line;
			Out_Ptr:= Out_Ptr+1;
		end Take;
	end Storage;

	task type Boss(ID:Integer);
	task body Boss is
			zadanie:taskFromBoss;
			subtype Num_Range is Integer range 0..1000000 ;
			package My_Num_Random is new Ada.Numerics.Discrete_Random(Num_Range) ;
			use My_Num_Random ;
			Num : My_Num_Random.Generator;

			subtype Act_Range is Integer range 0..2;
			package My_Act_Random is new Ada.Numerics.Discrete_Random(Act_Range) ;
			use My_Act_Random ;
			Act : My_Act_Random.Generator;
	begin
		loop
			zadanie.number1:=Random(Num);
			zadanie.number2:=Random(Num);
			zadanie.action:=Random(Act);
			TaskList.Append(zadanie);
			Put("Boss ");
			Ada.Integer_Text_IO.put(zadanie.number1 );
			Ada.Integer_Text_IO.put(zadanie.number2);
			Ada.Integer_Text_IO.put(zadanie.action);
			Ada.Integer_Text_IO.put(numberOf);
			New_Line;
			delay Boss_Min_Delay;
		end loop;
	end Boss;



	task type Producer(ID:Integer);
	task body Producer is
			zadanie:taskFromBoss;
			Result:Integer;
	begin
		loop
			delay Producer_Min_Delay;
			zadanie:=lastTask;
			--Ada.Text_IO.put("Producer " & Integer'Image(ID));
			TaskList.Take(zadanie);
			case zadanie.action is
			   when 0 =>

				Result:=zadanie.number1 + zadanie.number2;

			   when 1 =>

				Result:=zadanie.number1 - zadanie.number2;

			   when 2 =>

				Result:=zadanie.number1 * zadanie.number2;

			   when others =>

				Put_Line("KASZANA");

			end case;
			Put("    Result: ");
			Ada.Integer_Text_IO.put(Result);
			New_Line;
			New_Line;
			Storage.Append(Result);
		end loop;
	end Producer;

	task type Consumer(ID:Integer);
	task body Consumer is 
		--N:Integer;
	begin 
		loop
			delay Consumer_Min_Delay;
			Put("Client " & Integer'Image(ID) & " bought product ");
			Storage.Take(product);
			New_Line;
		end loop;
	end Consumer;

	B:Boss(1);


	type ProducerArray_Type is array ( Positive range <>) of access Producer;
        type ProducerA_AT is access ProducerArray_Type;
	type ConsumerArray_Type is array ( Positive range <>) of access Consumer;
	type ConsumerA_AT is access ConsumerArray_Type;


	function Init_Producers(Size : Integer) return ProducerA_AT is
		P_Ptr : ProducerA_AT;
	    begin
		P_Ptr := new ProducerArray_Type(1 .. Size);
		for I in P_Ptr.all'Range loop
		    P_Ptr.all(I) := new Producer(I);
		end loop;
		return P_Ptr;
	end Init_Producers;



	function Init_Consumer(Size : Integer) return ConsumerA_AT is
		C_Ptr : ConsumerA_AT;
	    begin
		C_Ptr := new ConsumerArray_Type(1 .. Size);
		for I in C_Ptr.all'Range loop
		    C_Ptr.all(I) := new Consumer(I);
		end loop;
		return C_Ptr;
	end Init_Consumer;

	--producersArray : ProducerArray_Type := Init_Producers(Producers_Number).all;
	consumerArray : ConsumerArray_Type := Init_Consumer(Consumers_Number).all;

	begin
		null;
end Company;
