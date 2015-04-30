package main
import ("fmt";"time";"math/rand";"container/list")



const producersNumber int = 5
const customersNumber int = 3

// wielkość kontenerów
const storageMax int64 = 10
const listMax int64 = 10

// minimalne opóźnienie
const BossDelay time.Duration = 2
const EmployerDelay time.Duration = 5
const CustomerDelay time.Duration = 8

type Quest struct {
	number1 int64
	number2 int64
	action int32
}

var quests = list.New()
var storage [storageMax]int64

func main() {
	var channel [producersNumber]chan bool
	var j int64
	for j=0; j<storageMax; j++ {
		storage[j] = 1000000
	}
	bossCh := make(chan bool)
	clientCh := make(chan bool)
	
	go boss(bossCh)
	
	for i := 0 ; i<producersNumber; i++ {
		channel[i] = make(chan bool)
		go producer(channel[i],i)
	}
	go client(clientCh)
	
	<- bossCh
	for i := 0; i<customersNumber; i++ {
		<- channel[i]
	}
	<- clientCh
}


func newQuest() Quest{
	x := rand.Int63n(100)
	y := rand.Int63n(100)
	z := rand.Int31n(3)
	return Quest{number1:x, number2:y, action:z}
}

func boss(bossCh chan bool) {
	for true {
		quests.PushBack(newQuest())
		e:=quests.Back()
		fmt.Println("Boss",e.Value)
		//for e := quests.Front(); e != nil; e = e.Next() {
			//fmt.Println(e.Value)
		//}
		time.Sleep(time.Second * BossDelay)
	}
	bossCh <- true
}

func producer(channel chan bool, i int) {
	var result int64
	var j int64
	for true {
		time.Sleep(time.Second * EmployerDelay)
		if quests.Front() != nil {
			temp := quests.Front()
			quests.Remove(temp)
			num1 := temp.Value.(Quest).number1
			num2 := temp.Value.(Quest).number2

			switch temp.Value.(Quest).action {
        		case 0:
            		result = num1 + num2
        		case 1:       //2
            		result = num1 - num2
            	case 2:
            		result = num1 * num2
    		}
    		isFull := 0
    		for j=0; j<storageMax; j++ {
    			if storage[j] == 1000000 {
    				storage[j] = result
    				isFull++
    				break
    			}
    		}
    		if isFull == 0 {
    			fmt.Println("Storage is full!")
    		} else {
				fmt.Print("Pracownik ",i,"  ")
				fmt.Println(temp.Value.(Quest).number1,temp.Value.(Quest).number2,temp.Value.(Quest).action,"Result:",result)
			}
		}	else {
			fmt.Print("Pracownik ",i," lista zadań pusta!\n")
		}
	}
	channel <- true
}

func client(channel chan bool) {
	for true {	
		time.Sleep(time.Second * CustomerDelay)
		i := rand.Int63n(storageMax)
		var count int64
		count = 0
		for storage[i] == 1000000 {
			i = rand.Int63n(storageMax)
			count++
		}
		//fmt.Println(count)
		if count == storageMax {
			fmt.Println("Storage is empty!")
		} else {
			fmt.Println("Klient kupił przedmiot numer ",i,storage[i])
			storage[i] = 0
		}
	}
	channel <- true
}