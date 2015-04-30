package Config is

    -- stałe definiujące:

    -- ilość wątków
    Producers_Number : constant Integer := 10;
    Consumers_Number : constant Integer := 5;

    -- minimalne opóźnienie
    Boss_Min_Delay : constant Duration := 0.5;
    Producer_Min_Delay : constant Duration := 7.5;
    Consumer_Min_Delay : constant Duration := 9.5;

    -- modyfikator losowego opóźnienia
    Boss_Mod_Delay : constant Float := 3.0;
    Producer_Mod_Delay : constant Float := 15.0;
    Consumer_Mod_Delay : constant Float := 18.0;

    -- wielkość kontenerów
    Storage_max : constant Integer := 3;
    List_max : constant Integer := 5;
end Config;

