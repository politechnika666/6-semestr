with Ada.Text_IO;
package Zadanie1 is
	handyman_speed : constant Float := 0.1;
	--stałe
	employe_num : constant Integer := 10;
	max_task : constant Integer := 10;
	max_store : constant Integer := 20;
	empId: Integer:=0;
	bos_speed : constant Float := 0.3;
	employe_speed : constant Float := 2.0;
	customer_speed : constant Float := 0.5;
	--machine
	--add 
	add_machine_time: constant Float := 0.5;
	add_machine_num: constant Integer := 10;
	add_machine_unre:constant Float := 0.1;
	add_machine_repeir_time: constant Float := 0.5;
	--tryb
	mode: Boolean := TRUE;
	--mode: Boolean := FALSE;
	--zadania
	type s_task is record 
		arg1:Integer;
		op:Integer;
		arg2:Integer;
		output:Integer;
	end record;
	type err_raport is record
		num,m_type:Integer;
	end record;
	--stack
	task_array : array (0 .. max_task ) of s_task ;
	store_array : array (0 .. max_store) of Integer;
	add_machine_array : array (0 .. add_machine_num) of Boolean;
	raport_array : array (0 .. add_machine_num) of err_raport;
	--server
	protected type MachineManager is
		entry PushAdd (num : in Integer);
		entry PopAdd (num,err:out Integer);
		entry PushRaport (raport:in err_raport);
		entry PopRaport(num:out err_raport);
	private 
		top_add,top_err: Integer := 0;
		
	end MachineManager;
	
	protected type TaskList is
		entry Push (arg1,arg2,arg3 : in Integer);
		entry Pop (arg1,arg2,arg3:out Integer);
		procedure PrintList;
	private
		top_task: Integer := 0;		
	end TaskList;
	protected type StoreList is
		entry Push (arg : in Integer);
		entry Pop (arg : out Integer);
		procedure PrintList;
	private
		top_store: Integer := 0;		
	end StoreList;
	
	protected type AddMachine is
		entry Add (arg1,arg2 : in Integer;output,err : out Integer);
		entry Repeir ;
	private
		broken: Boolean := FALSE;		
	end AddMachine;
	add_array:array (1 .. add_machine_num) of access AddMachine;
	procedure init;
	task type employe(T : access TaskList; S : access StoreList;M: access MachineManager);
	task type boss(T : access TaskList);
	task type customer(S : access StoreList);
	task type Ui(T : access TaskList ; S : access StoreList);
	task type HandyMan(M: access MachineManager);
end Zadanie1;