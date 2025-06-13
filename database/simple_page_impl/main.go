package main

import (
	st "database/storage"
	"fmt"
)

func main() {
	// Initialize the page manager with a database file
	pm, _ := st.NewPageManager("database.db")
	defer pm.Close()

	page, _ := pm.AllocateNewPage()

	data1 := []byte("Hello World")
	slot1, _ := page.AddTuple(data1)

	data2 := []byte("Another Record")
	_, _ = page.AddTuple(data2)

	fmt.Println("Before deletion, tuple count:", page.TupleCount)

	_ = page.DeleteTuple(slot1)

	fmt.Println("After deletion, tuple count:", page.TupleCount)

	// Add another that reuses space
	data3 := []byte("Reuse Space")
	slot3, _ := page.AddTuple(data3)

	fmt.Println("New tuple added at slot index:", slot3)

	_ = pm.WritePage(page)

	numPages, fileSize, _ := pm.Stats()
	fmt.Printf("Total pages: %d, File size: %d bytes\n", numPages, fileSize)

	page2, _ := pm.AllocateNewPage()
	data4 := []byte("Data in new page")
	slot4, _ := page2.AddTuple(data4)
	fmt.Println("New page allocated with tuple at slot index:", slot4)
	_ = pm.WritePage(page2)
	fmt.Println("New page ID:", page2.ID)
	fmt.Println("New page tuple count:", page2.TupleCount)
	fmt.Println("New page free space offset:", page2.FreeSpaceOffset)

}

/*
	pm, err := st.NewPageManager("database.db")
	if err != nil {
		panic(err)
	}
	defer pm.Close()

	// Allocate a new page
	page1, err := pm.AllocateNewPage()
	if err != nil {
		panic(err)
	}

	err = pm.WritePage(page1)
	if err != nil {
		panic(err)
	}
	loadedPage, err := pm.ReadPage(page1.ID)
	if err != nil {
		panic(err)
	}

	fmt.Println("Read page : ", loadedPage.ID, " Tuples : ", loadedPage.TupleCount, " Free Space Offset: ", loadedPage.FreeSpaceOffset)

	// Allocate another page
	page2, err := pm.AllocateNewPage()
	if err != nil {
		panic(err)
	}
	defer pm.Close()
	page2.InsertTuple([]byte("Another tuple in a new page."))
	err = pm.WritePage(page2)
	if err != nil {
		panic(err)
	}
	loadedPage2, err := pm.ReadPage(page2.ID)
	if err != nil {
		panic(err)
	}
	fmt.Println("Read page : ", loadedPage2.ID, " Tuples : ", loadedPage2.TupleCount, " Free Space Offset: ", loadedPage2.FreeSpaceOffset)

*/
