package main

import "fmt"
import "time"
import "math/rand"
import "flag"
import "container/list"

// ilość wątków
const employersNumber int = 5
const customersNumber int = 3
const addMachineNumber int = 3
const multMachineNumber int = 3

// wielkość kontenerów
const storageMax int = 10
const listMax int = 10

// minimalne opóźnienie
const BossMin int = 1
const EmployerMin int = 15
const CustomerMin int = 20
const MachineMin int = 20

// modyfikator losowej składowej opóźnienia
const BossMod int = 1
const EmployerMod int = 1
const CustomerMod int = 10
const MachineMod int = 1

// bufor kanałów komunikacji
const bufsiz int = 1

// pstwo zniaszczenia maszyn
const crushProb = 10

// czas potrzebny do naprawy maszyny
const repTime = 2

// delay serwisanta
const serviceDelay = 1

// number1, number2 liczby
// op - operator
// result wynik
type Task struct {
    number1 int
    result int
    number2 int
    op string
}

var interactive bool
var crushedAddMach [addMachineNumber+1]bool
var crushedMultMach [multMachineNumber+1]bool

func main() {
    // ustalanie trybu pracy
    interactPtr := flag.Bool("i", false, "a bool")
    flag.Parse()
    interactive = *interactPtr


    // kanały komunikacji symulatora
    list_in := make(chan Task, bufsiz)
    list_out := make(chan Task, bufsiz)
    storage_in := make(chan int, bufsiz)
    storage_out := make(chan int, bufsiz)
    freeAddMach := make(chan int, addMachineNumber)
    freeMultMach := make(chan int, multMachineNumber)
    multCh := [multMachineNumber+1]chan Task{}
    addCh := [addMachineNumber+1]chan Task{}
    multQueue := make(chan int, multMachineNumber+1)
    button := make(chan bool)

    // kanały do obsługi menu
    lch := make(chan bool)
    sch := make(chan bool)
    bch := make(chan bool)
    pch := make(chan bool)
    cch := make(chan bool)
    amch := make(chan bool)
    exit := make(chan bool)


    // uruchamiania potrzebnych wątków
    /*if interactive {
        go menu(lch, sch, exit, bch, pch, cch, amch)
    } else {
        close(lch)
        close(sch)
        close(bch)
        close(pch)
        close(cch)
        close(amch)
        close(exit)
    }*/
    go lists(list_in, list_out, lch)
    go storage(storage_in, storage_out, sch)
    go boss(list_in, 1, bch)
    for i := 1; i <= addMachineNumber; i++ {
        crushedAddMach[i] = false
        freeAddMach <- i
        addCh[i] = make(chan Task)
        go addMachine(i,addCh[i], amch,freeAddMach)
    }
    for i := 1; i <= multMachineNumber; i++ {
        crushedMultMach[i] = false
        freeMultMach <- i
        multCh[i] = make(chan Task)
        go multMachine(i,multCh[i],amch,freeMultMach,multQueue,button)
    }
    
    for i := 1; i <= employersNumber; i++ {
        go employer(list_out, storage_in, i, pch, &addCh, &multCh,freeAddMach,freeMultMach,multQueue,button)
    }
    for i := 1; i <= customersNumber; i++ {
        go customer(storage_out, i, cch)
    }
    go service(freeAddMach, freeMultMach)

    // błokada main()
    if interactive {
        <- exit
    } else {
        var input string
        fmt.Scanln(&input)
    }
    fmt.Println("done")
}

// wątek obsługujący menu
// przyjmuje kanały do komunikacji z kontenerami
// oraz kanał blokujący zakończenie main
func menu(lch chan<- bool, sch chan<- bool, exit chan<- bool, bch chan<- bool, pch chan<- bool, cch chan<- bool, amch chan<- bool) {
    var end bool = false
    var input string
    //fmt.Println("m  - print menu")
    fmt.Println("m - print this menu")
                fmt.Println("l - print list")
                fmt.Println("s - print storage")
                fmt.Println("b - print boss")
                fmt.Println("p - print producer")
                fmt.Println("c - print customer")
                fmt.Println("ma - print machines")
                fmt.Println("a - print all")
                fmt.Println("e - exit")
    for !end {
        fmt.Scanf("%s", &input)
        switch input {
            case "m":
                fmt.Println("m - print this menu")
                fmt.Println("l - print list")
                fmt.Println("s - print storage")
                fmt.Println("b - print boss")
                fmt.Println("p - print producer")
                fmt.Println("c - print customer")
                fmt.Println("ma - print machines")
                fmt.Println("a - print all")
                fmt.Println("e - exit")
            case "a":
                lch <- true
                sch <- true
                bch <- true
                pch <- true
                cch <- true
                amch <- true
            case "l":
                lch <- true
            case "s":
                sch <- true
            case "b":
                bch <- true
            case "p":
                pch <- true
            case "c":
                cch <- true
            case "ma":
                amch <-true
            case "e":
                end = true
                exit <- true
            default:
                fmt.Println("unknown option: '", input, "' try 'm'")
        }
    }
}

// wątek obsługujący magazyn 
// przyjmuje kanał z którego pobiera produkty i składuje
// kanał do którego wrzuca produkty do kupna
// oraz kanał do obsługi menu
func storage(sin <-chan int, sout chan<- int, sch <-chan bool) {
    if !interactive {
        fmt.Println("Storage: ready.")
    }
    var results = list.New()
    var size, res, todo int
    var prep bool
    for true {
        // obsługa menu
        select {
            case <- sch:
                fmt.Print("Storage: ")
                for e:= results.Front(); e != nil; e = e.Next() {
                    fmt.Print(e.Value)
                    fmt.Print(" ")
                }
                fmt.Println()
            default:
        }
        // przyjmowanie gotowych produktów
        if size < storageMax {
            select {
                case res = <- sin:
                    results.PushBack(res)
                    size++
                default:
            }
        }
        // sprzedarz
        if size > 0 {
            if !prep {
                tmp := results.Front()
                todo = tmp.Value.(int)
                results.Remove(tmp)
                prep = true
                size--
            }
            select {
                case sout <- todo:
                    prep = false
                default:
            }
        }
    }
}

// wątek obsługujący listę zadań
// przyjmuje kanał z którego pobiera zadania i zapisuje
// kanał do którego wrzuca zadania do zrobienia
// oraz kanał do obsługi menu
func lists(lin <-chan Task, lout chan<- Task, lch <-chan bool) {
    var tasks = list.New()
    var size int
    var ntk, todo Task
    var prep bool

    if !interactive {
        fmt.Println("List: ready.")
    }
    for true {
        // obsługa menu
        select {
        case <- lch:
            for e:= tasks.Front(); e != nil; e = e.Next() {
                fmt.Println(e.Value)
            }
        default:
        }
        // nowe zadania
        if size < listMax {
            select {
            case ntk = <- lin:
                tasks.PushBack(ntk)
                size++
            default:
            }
        }
        // wydawanie pracownikom zadań
        if size > 0 {
            if !prep {
                tmp := tasks.Front()
                todo = tmp.Value.(Task)
                tasks.Remove(tmp)
                prep = true
                size--
            }
            select {
                case lout <- todo:
                    prep = false
                default:
            }
        }
    }
}



// szef generuje losowe zadania
// i wysyła je kanałem do listy zadań
func boss(list chan<- Task, id int, bch <- chan bool) {
    var number1, number2, tmp int
    var op string
    var t Task
    var interval time.Duration

    if !interactive {
        fmt.Println("Boss", id, ": running.")
    }
    for true {
        interval = time.Duration(BossMin + rand.Intn(BossMod))
        time.Sleep(time.Millisecond * interval)

        number1 = rand.Intn(10)
        rand.Seed(time.Now().Unix()*int64(number1))
        number1 = rand.Intn(10)
        number2 = rand.Intn(10)
        tmp = rand.Intn(3) 

        switch tmp {
        case 0:
            op = "+"
        case 1:
            op = "-"
        case 2:
            op = "*"
        }

        t = Task{number1:number1, number2:number2, op:op, result:101}
        if !interactive {
            fmt.Println("Boss", id, ": adding", number1, number2, op)
        }
        select {
            case <- bch:
                fmt.Println("Boss", id, ": adding", number1, number2, op)   
            default:
        }
        list <- t
    }
}

// pracownik otrzymuje kanałem zadanie z listy
// wykonuje je i wynik przesyła kanałem do magazynu
func employer(list <-chan Task, storage chan<- int, id int, pch <- chan bool, 
    addCh *[addMachineNumber+1]chan Task, multCh *[multMachineNumber+1]chan Task,
    freeAddMach chan int, freeMultMach chan int, multQueue chan int, button chan bool) {
    var r int
    var t, temp Task
    var interval time.Duration

    if !interactive {
        fmt.Println("Employer", id, ": running.")
    }
    for true {
        interval = time.Duration(EmployerMin + rand.Intn(EmployerMod))
        time.Sleep(time.Millisecond * interval)

        select {
        case machID, needHelp := <-multQueue:
            if needHelp {
                if !interactive {
                    fmt.Println("Helping the machine nr: ", machID)
                }
                select {
                case <- pch:
                    fmt.Println("Helping the machine nr: ", machID)
                default:
                }
                button <- true
            }
        default:
            t = <- list
            r = t.result
            if !interactive {
                fmt.Println("Employer", id, ": got", t.number1, t.number2, t.op, t.result)
            }

            switch t.op {
                case "+":
                    for true {
                        if r == 101 {
                                numOfMach := <-freeAddMach
                                //fmt.Println("MACHINE NUMBER TO",numOfMach,"EMPLOYER NUMBER TO",id,"OPERATION",t.op)
                                if !crushedAddMach[numOfMach] {
                                    addCh[numOfMach] <-t
                                    temp = <- addCh[numOfMach]
                                    r = temp.result
                                }
                        } else {
                            break
                        }
                    }
                case "-":
                    for true {
                        if r == 101 {
                                numOfMach := <-freeAddMach
                                //fmt.Println("MACHINE NUMBER TO",numOfMach,"EMPLOYER NUMBER TO",id,"OPERATION",t.op)
                                if !crushedAddMach[numOfMach] {
                                    addCh[numOfMach] <-t
                                    temp = <- addCh[numOfMach]
                                    r = temp.result
                                }
                        } else {
                            break
                        }
                    }
                case "*":
                    for true {
                        if r == 101 {
                                numOfMach := <-freeMultMach
                                //fmt.Println("MACHINE NUMBER TO",numOfMach,"EMPLOYER NUMBER TO",id,"OPERATION",t.op)
                                if !crushedMultMach[numOfMach] {
                                    multCh[numOfMach] <-t
                                    button <- true
                                    temp = <- multCh[numOfMach]
                                    r = temp.result
                                }
                        } else {
                            break
                        }
                    }
            }

            if !interactive {
                fmt.Println("Employer", id, ": storing", r)
            }
            select {
                case <- pch:
                    fmt.Println("Employer", id, ": got", t.number1, t.number2, t.op, "Result: ", r)
                default:
            }

            storage <- r
        }
    }
}

// klient zabiera z kanału produkty które wrzuca tam magazyn
func customer(storage <-chan int, id int, cch<- chan bool) {
    var interval time.Duration

    if !interactive {
        fmt.Println("Customer", id, ": running.")
    }
    for true {
        interval = time.Duration(CustomerMin + rand.Intn(CustomerMod))
        time.Sleep(time.Millisecond * interval)

        p := <- storage
        if !interactive {
            fmt.Println("Customer", id, ": bought", p)
        }
        select {
            case <- cch:
                fmt.Println("Customer", id, ": bought", p)
            default:
        }
    }
}

// maszyna wykonująca dodawanie
func addMachine(id int, addCh chan Task, amch chan bool, freeCh chan<- int) {
    var interval time.Duration
    var probability int
    for true {
        task := <- addCh

        switch task.op {
            case "+":
                rand.Seed(time.Now().Unix())
                probability = rand.Intn(100)
                if probability < crushProb {
                    fmt.Println("Adding machine", id, "has crashed!")
                    crushedAddMach[id] = true
                } else {
                    task.result = task.number1 + task.number2
                }
            case "-":
                rand.Seed(time.Now().Unix())
                probability = rand.Intn(100)
                if probability < crushProb {
                    fmt.Println("Adding machine", id, "has crashed!")
                    crushedAddMach[id] = true
                } else {
                    task.result = task.number1 - task.number2
                }
        }
        if !interactive {
            fmt.Println("Adding machine", id, ": get", task.number1, task.number2, task.op, "Result:", task.result)
        }
        select {
            case <- amch:
                fmt.Println("Adding machine", id, ": get", task.number1, task.number2, task.op, "Result:", task.result)
            default:
        }
        interval = time.Duration(MachineMin + rand.Intn(MachineMod))
        time.Sleep(time.Millisecond * interval)

        addCh <- task
        freeCh <- id
    }
}

// maszyna wykonująca mnożenie
func multMachine(id int, multCh chan Task, amch chan bool, freeCh chan<- int, multQueue chan int, button chan bool) {
    var interval time.Duration
    var probability int
    for true {
        task := <- multCh

        <-button
        multQueue <- id
        <-button

        switch task.op {
            case "*":
                rand.Seed(time.Now().Unix())
                probability = rand.Intn(100)
                if probability < crushProb {
                    fmt.Println("Multiplying machine", id, "has crashed!")
                    crushedMultMach[id] = true
                } else {
                    task.result = task.number1 * task.number2
                }
        }
        if !interactive {
            fmt.Println("Multiplying machine", id, ": get", task.number1, task.number2, task.op, "Result:", task.result)
        }
        select {
            case <- amch:
                fmt.Println("Multiplying machine", id, ": get", task.number1, task.number2, task.op, "Result:", task.result)
            default:
        }
        interval = time.Duration(MachineMin + rand.Intn(MachineMod))
        time.Sleep(time.Millisecond * interval)

        multCh <- task
        freeCh <- id
    }
}

// serwisant
func service(freeAddMach chan int, freeMultMach chan int) {
    var len int
    if addMachineNumber > multMachineNumber {
        len = addMachineNumber
    } else {
        len = multMachineNumber
    }
    for true {
        for i := 1; i <= len; i++ {
            if crushedAddMach[i] == true && i <= addMachineNumber{
                time.Sleep(time.Second * repTime)
                crushedAddMach[i] = false
                freeAddMach <- i           
                fmt.Println("Adding machine",i,"was repaired!")
            }
            if crushedMultMach[i] == true && i <= multMachineNumber{
                time.Sleep(time.Second * repTime)
                crushedMultMach[i] = false
                freeMultMach <- i           
                fmt.Println("Multiplying machine",i,"was repaired!")
            }
        }
        time.Sleep(time.Second * serviceDelay)
    }   
}