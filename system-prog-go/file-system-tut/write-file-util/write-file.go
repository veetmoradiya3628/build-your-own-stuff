package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strconv"
)

// writeFileEfficiently writes 1000 lines to output.txt using a buffered writer.
// This is more efficient than writing each line directly to the file.
// It uses a buffer to reduce the number of write operations, which can be costly.
func writeFileEfficiently() {
	file, err := os.Create("output.txt")
	if err != nil {
		panic(err)
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	for i := 0; i < 1000; i++ {
		writer.WriteString("This is line number " + strconv.Itoa(i) + "\n")
	}
	writer.Flush()
}

// copyData copies the content of output.txt to copy.txt
// using io.Copy for efficient copying.
func copyData() {
	src, err := os.Open("output.txt")
	if err != nil {
		panic(err)
	}
	defer src.Close()

	dest, err := os.Create("copy.txt")
	if err != nil {
		panic(err)
	}
	defer dest.Close()

	_, err = io.Copy(dest, src)
	if err != nil {
		panic(err)
	}
}

func main() {
	if len(os.Args) != 3 {
		println("Usage: write-file <file-path> <content>")
		return
	}

	// whole file write at once
	if err := os.WriteFile(os.Args[1], []byte(os.Args[2]), 0644); err != nil {
		println("Error writing file:", err.Error())
		return
	}

	writeFileEfficiently()

	copyData()

	info, err := os.Stat("output.txt")
	if err != nil {
		fmt.Println("Error getting stats of output.txt:", err)
	} else {
		fmt.Printf("File Name: %s\n", info.Name())
		fmt.Printf("Size: %d bytes\n", info.Size())
		fmt.Printf("Permissions: %s\n", info.Mode())
		fmt.Printf("Last Modified: %s\n", info.ModTime().Format("2006-01-02 15:04:05"))
		if info.IsDir() {
			fmt.Println("Type: Directory")
		} else {
			fmt.Println("Type: File")
		}
	}
}
