package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strings"
	"unicode"
	"unicode/utf8"
)

type AngryReader struct {
	r io.Reader
}

func NewAngryReader(r io.Reader) *AngryReader {
	return &AngryReader{r: r}
}

func (a *AngryReader) Read(b []byte) (int, error) {
	n, err := a.r.Read(b)
	for r, i, w := rune(0), 0, 0; i < n; i += w {
		r, w = utf8.DecodeRune(b[i:])
		if !unicode.IsLetter(r) {
			continue
		}

		ru := unicode.ToUpper(r)
		if wu := utf8.EncodeRune(b[i:], ru); wu != w {
			return n, fmt.Errorf("unexpected rune width: got %d, want %d", wu, w)
		}
	}
	return n, err
}

func main() {
	input := "Hello, 世界! 123"
	reader := NewAngryReader(strings.NewReader(input))

	buf := make([]byte, len(input))
	n, err := reader.Read(buf)
	if err != nil && err != io.EOF {
		fmt.Println("Error reading:", err)
		return
	}

	fmt.Println("Modified output:", string(buf[:n]))
	fmt.Println("Number of bytes read:", n)

	// Buffered stream
	buffReader := bufio.NewReader(os.Stdin)
	buffWriter := bufio.NewWriter(os.Stdout)

	line, _ := buffReader.ReadString('\n')
	line = strings.TrimSpace(line)
	buffWriter.WriteString("You entered: " + line + "\n")
	buffWriter.Flush()
}
