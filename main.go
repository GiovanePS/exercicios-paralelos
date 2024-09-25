package main

import (
	"fmt"
	"sync"
	"time"
)

var TAM = 100000000
var x = make([]int, TAM)
var y = make([]int, TAM)
var numThreads = 2

func main() {
	for i := 0; i < TAM; i++ {
		x[i] = i
		y[i] = i
	}

	// Non-threaded execution
	start := time.Now()
	calcUntilTAM()
	timeElapsed := time.Since(start)
	fmt.Printf("Non-threaded time: %s\n", timeElapsed)

	// Threaded execution
	start = time.Now()
	var wg sync.WaitGroup

	for i := 0; i < numThreads; i++ {
		wg.Add(1)
		go calcUntilTAMThreaded(i, &wg)
	}

	wg.Wait() // Wait for all goroutines to finish
	timeElapsed = time.Since(start)
	fmt.Printf("Threaded time: %s\n", timeElapsed)
}

func calcUntilTAM() {
	for i := 0; i < TAM; i++ {
		y[i] = 2*x[i] + y[i]
	}
}

func calcUntilTAMThreaded(threadId int, wg *sync.WaitGroup) {
	defer wg.Done() // Mark the goroutine as done when it finishes

	startIdx := threadId * TAM / numThreads
	endIdx := (threadId + 1) * TAM / numThreads

	for i := startIdx; i < endIdx; i++ {
		y[i] = 2*x[i] + y[i]
	}
}
