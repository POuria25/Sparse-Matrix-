package main

import (
	"bufio"
	"fmt"
	"math/rand"
	"os"
	"sort"
	"time"
)

type Entry struct {
	row, col int
	value    int
}

func main() {
	var numRows, numColumns, numNonZero int
	var filename string

	// Seed random number generator
	rand.Seed(time.Now().UnixNano())

	// Get matrix dimensions and number of non-zero values from user
	fmt.Print("Enter the number of columns: ")
	fmt.Scan(&numColumns)
	fmt.Print("Enter the number of rows: ")
	fmt.Scan(&numRows)
	fmt.Print("Enter the number of non-zero values: ")
	fmt.Scan(&numNonZero)
	fmt.Print("Enter the filename of your generated sparse matrix: ")
	fmt.Scan(&filename)

	// Use a map to avoid duplicate entries
	nonZeroEntries := make(map[Entry]struct{})

	// Generate random non-zero entries
	for len(nonZeroEntries) < numNonZero {
		row := rand.Intn(numRows) + 1         // Rows are indexed from 1
		col := rand.Intn(numColumns) + 1      // Columns are indexed from 1
		value := rand.Intn(2000001) - 1000000 // Random value in range [-1000000, 1000000]

		entry := Entry{row, col, value}
		nonZeroEntries[entry] = struct{}{}
	}

	// Convert map to slice for sorting and easy writing
	entries := make([]Entry, 0, len(nonZeroEntries))
	for entry := range nonZeroEntries {
		entries = append(entries, entry)
	}

	// Sort entries by row and column for consistent output
	sort.Slice(entries, func(i, j int) bool {
		if entries[i].row == entries[j].row {
			return entries[i].col < entries[j].col
		}
		return entries[i].row < entries[j].row
	})

	// Write the matrix to a file
	file, err := os.Create(filename + ".mtx")
	if err != nil {
		fmt.Println("Error creating file:", err)
		return
	}
	defer file.Close()

	writer := bufio.NewWriter(file)

	// Write metadata line
	fmt.Fprintf(writer, "%d %d %d\n", numRows, numColumns, numNonZero)

	// Write each non-zero entry in the specified format
	for _, entry := range entries {
		fmt.Fprintf(writer, "%d %d %d\n", entry.row, entry.col, entry.value)
	}

	writer.Flush()
	fmt.Println("Sparse matrix saved to " + filename + ".mtx")
}
