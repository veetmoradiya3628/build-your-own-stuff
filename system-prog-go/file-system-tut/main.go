package main

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"os"
)

func main() {
	wd, err := os.Getwd()
	if err != nil {
		panic(err)
	}
	println("Current working directory:", wd)

	// Reading a whole content of a file in single call
	data, err := os.ReadFile(wd + string(os.PathSeparator) + "test.json")
	if err != nil {
		panic(err)
	}
	println("Content of test.json:", string(data))

	f, err := os.Open(wd + string(os.PathSeparator) + "test.json")
	if err != nil {
		panic(err)
	}
	defer f.Close()
	var (
		b = make([]byte, 16)
	)
	for n := 0; err == nil; {
		n, err = f.Read(b)
		if err == nil {
			fmt.Print(string(b[:n]))
		}
	}
	fmt.Println()
	if err != io.EOF {
		panic(err)
	}

	var buff = bytes.NewBuffer(make([]byte, 26))
	var texts = []string{
		`As he came into the window`,
		`It was the sound of a crescendo
He came into her apartment`,
		`He left the bloodstains on the carpet`,
		`She ran underneath the table
He could see she was unable
So she ran into the bedroom
She was struck down, it was her doom`,
	}
	for i := range texts {
		buff.Reset()
		buff.WriteString(texts[i])
		fmt.Println("Length: ", buff.Len(), "\tCapacity: ", buff.Cap())
	}

	// Reset file pointer to the beginning before reading lines
	_, err = f.Seek(0, io.SeekStart)
	if err != nil {
		panic(err)
	}

	r := bufio.NewReader(f)
	var rowCount int
	for {
		line, isPrefix, readErr := r.ReadLine()
		if readErr == io.EOF {
			break
		}
		if readErr != nil {
			panic(readErr)
		}
		fmt.Print(string(line))
		for isPrefix {
			line, isPrefix, readErr = r.ReadLine()
			if readErr != nil {
				panic(readErr)
			}
			fmt.Print(string(line))
		}
		rowCount++
		fmt.Println()
	}
	fmt.Println("Total rows read:", rowCount)
}
