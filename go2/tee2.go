package main

import (
	"flag"
	"fmt"
	"io"
	"os"
	"sync"
)

func main() {
	aFlag := flag.Bool("append", false, "append to the given FILEs")
	flag.Parse()

	var files []*os.File
	for _, arg := range flag.Args() {
		var file *os.File
		var err error
		if !*aFlag {
			file, err = os.Create(arg)
		} else {
			file, err = os.OpenFile(arg, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
		}
		if err != nil {
			fmt.Println(err)
			os.Exit(1)
		}
		defer file.Close()

		files = append(files, file)
	}

	buf := make([]byte, 4096)
	for {
		n, err := os.Stdin.Read(buf)
		if err != nil {
			if err == io.EOF {
				break
			}

			fmt.Println(err)
			os.Exit(1)
		}

		if _, err := os.Stdout.Write(buf[:n]); err != nil {
			fmt.Println(err)
			os.Exit(1)
		}

		var wg sync.WaitGroup
		for _, file := range files {
			wg.Add(1)
			go func(file *os.File) {
				_, err := file.Write(buf[:n])
				if err != nil {
					fmt.Println(err)
					os.Exit(1)
				}
				wg.Done()
			}(file)
		}
		wg.Wait()
	}
}
