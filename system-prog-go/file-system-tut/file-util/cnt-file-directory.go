package main

import (
	"fmt"
	"os"
	"path/filepath"
)

func main() {
	if len(os.Args) != 2 {
		fmt.Println("Usage: go run main.go <directory_path>")
		return
	}
	root, err := filepath.Abs(os.Args[1])
	if err != nil {
		fmt.Println("Error getting absolute path:", err)
		return
	}
	fmt.Println("Listing files in directory:", root)
	var c struct {
		files int
		dirs  int
	}

	filepath.Walk(root, func(path string, info os.FileInfo, err error) error {
		if info.IsDir() {
			c.dirs++
		} else {
			c.files++
		}
		fmt.Println("-", path)
		return nil
	})
	fmt.Printf("Total files: %d, Total directories: %d\n", c.files, c.dirs)
}
