package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

func getRandomNumber() int {
	return rand.Intn(101) + 100
}

var close_a = func(item string) error {
	if item != "elixir" {
		return fmt.Errorf("item is not elixir")
	}
	return nil
}

func increment_with_reference(number int) {
	number += 1
}

func increment_with_pointer(number *int) {
	*number += 1
}

func main() {
	fmt.Println("Hello, World!")
	fmt.Println("Welcome to Go programming!")

	// Basic variable declarations
	a := new(int)
	*a = 42
	fmt.Println("Value of a:", *a)
	fmt.Println("Address of a:", a)

	map1 := make(map[string]int, 10)
	map1["one"] = 1
	map1["two"] = 2
	fmt.Println("Map contents:", map1)
	fmt.Println("Value for key 'one':", map1["one"])

	chan1 := make(chan int, 5)
	chan1 <- 10
	chan1 <- 20
	fmt.Println("Channel length:", len(chan1))
	fmt.Println("Value from channel:", <-chan1)
	fmt.Println("Value from channel after read:", <-chan1)
	fmt.Println("Channel length after reads:", len(chan1))

	type N [2]int
	var n = N{1, 2}
	var m [2]int = n
	fmt.Printf("Array n : %p\n", &n)
	fmt.Printf("Array m : %p\n", &m)

	type MyInt struct {
		val int
	}
	x := MyInt{val: 100}
	y := &x
	fmt.Printf("Value of x: %d, Address of x: %p\n", x.val, &x)
	fmt.Printf("Value via y: %d, Address via y: %p\n", y.val, y)

	fmt.Println("Random number:", getRandomNumber())
	fmt.Println("Random number:", getRandomNumber())

	var wg sync.WaitGroup
	wg.Add(1)
	go func() {
		defer wg.Done()
		fmt.Println("This is a goroutine")
		for i := 1; i <= 1000; i++ {
			fmt.Println(i)
			time.Sleep(10 * time.Millisecond)
		}
	}()

	if err := close_a("elixir"); err != nil {
		fmt.Println("Error:", err)
	} else {
		fmt.Println("Item is elixir")
	}

	if err := close_a("python"); err != nil {
		fmt.Println("Error:", err)
	} else {
		fmt.Println("Item is elixir")
	}

	var number int = 10
	fmt.Println("Initial value of number:", number)
	increment_with_reference(number)
	fmt.Println("Value after increment_with_reference:", number)
	increment_with_pointer(&number)
	fmt.Println("Value after increment_with_pointer:", number)

	for i := 0; i < 10; i++ {
		fmt.Printf("Random number %d: %d\n", i+1, getRandomNumber())
	}

	// Wait for the goroutine to finish
	wg.Wait()
}
