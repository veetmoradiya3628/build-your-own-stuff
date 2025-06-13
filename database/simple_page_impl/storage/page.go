package storage

import (
	"encoding/binary"
	"fmt"
	"os"
)

/*
This code implements a simple page structure for a database system.
Each page has a fixed size of 4096 bytes, with a header that contains the page ID, Tuple count, and free space offset.
Tuples can be inserted into the page, and their offsets and sizes are stored in a slice of TupleSlot.

Layout will look like this:
+------------------+
| Page Header      | 8 bytes
+------------------+
| Tuple Slots      | 4 bytes per slot (offset + size)
+------------------+
| Tuple Data       | variable size, up to 4096 bytes total
+------------------+

Header & Slots grow from the start of the page, while Tuple Data grows from the end of the page.
*/

const PAGE_SIZE = 4096
const HEADER_SIZE = 8 // ID (4 bytes) + TupleCount (2) + FreeOffset (2)
const SLOT_SIZE = 4   // Each slot has Offset (2 bytes) + Size (2 bytes)

type Slot struct {
	Offset uint16
	Size   uint16
}

type Page struct {
	ID              uint32
	TupleCount      uint16
	FreeSpaceOffset uint16
	Slots           []Slot
	Data            []byte // size = PAGE_SIZE (4096)
}

func (s Slot) IsDeleted() bool {
	return s.Offset == 0xFFFF
}

func (p *Page) Serialize() []byte {
	buf := make([]byte, PAGE_SIZE)

	// write header
	binary.LittleEndian.PutUint32(buf[0:4], p.ID)
	binary.LittleEndian.PutUint16(buf[4:6], p.TupleCount)
	binary.LittleEndian.PutUint16(buf[6:8], p.FreeSpaceOffset)

	// Write slots directory
	for i, slot := range p.Slots {
		base := HEADER_SIZE + i*SLOT_SIZE
		binary.LittleEndian.PutUint16(buf[base:], slot.Offset)
		binary.LittleEndian.PutUint16(buf[base+2:], slot.Size)
	}
	// Write tuple data
	copy(buf[p.FreeSpaceOffset:], p.Data[p.FreeSpaceOffset:])
	return buf
}

func DeserializePage(data []byte) *Page {
	p := &Page{
		ID:              binary.LittleEndian.Uint32(data[0:4]),
		TupleCount:      binary.LittleEndian.Uint16(data[4:6]),
		FreeSpaceOffset: binary.LittleEndian.Uint16(data[6:8]),
		Data:            make([]byte, PAGE_SIZE),
	}

	for i := 0; i < int(p.TupleCount); i++ {
		base := HEADER_SIZE + i*SLOT_SIZE
		offset := binary.LittleEndian.Uint16(data[base:])
		size := binary.LittleEndian.Uint16(data[base+2:])
		p.Slots = append(p.Slots, Slot{
			Offset: offset,
			Size:   size,
		})
	}
	copy(p.Data, data[HEADER_SIZE+int(p.TupleCount)*SLOT_SIZE:])
	return p
}

// Write page to file
func WriteToFile(filename string, p *Page) error {
	buf := p.Serialize()
	return os.WriteFile(filename, buf, 0644)
}

func ReadFromFile(filename string) (*Page, error) {
	buf, err := os.ReadFile(filename)
	if err != nil {
		return nil, err
	}
	return DeserializePage(buf), nil
}

func (p *Page) AddTuple(data []byte) (int, error) {
	tupleSize := len(data)
	requiredSpace := tupleSize

	for i, slot := range p.Slots {
		if slot.IsDeleted() && int(slot.Size) >= tupleSize {
			copy(p.Data[int(slot.Offset):int(slot.Offset)+tupleSize], data)
			p.Slots[i].Size = uint16(tupleSize)
			return i, nil
		}
	}

	if int(p.FreeSpaceOffset) < requiredSpace+2*len(p.Slots) {
		return -1, fmt.Errorf("not enough space in page")
	}

	offset := int(p.FreeSpaceOffset) - tupleSize
	copy(p.Data[offset:offset+tupleSize], data)

	p.Slots = append(p.Slots, Slot{
		Offset: uint16(offset),
		Size:   uint16(tupleSize),
	})
	p.FreeSpaceOffset = uint16(offset)
	p.TupleCount++
	return len(p.Slots) - 1, nil
}

func (p *Page) DeleteTuple(slotIndex int) error {
	if slotIndex < 0 || slotIndex >= len(p.Slots) {
		return fmt.Errorf("slot index out of range: %d", slotIndex)
	}
	p.Slots[slotIndex].Offset = 0xFFFF // Mark as deleted
	p.Slots[slotIndex].Size = 0
	p.TupleCount--
	return nil
}

// Example usage of the Page structure

// func NewPage(id uint32) *Page {
// 	p := &Page{
// 		PageId:          id,
// 		TupleCount:      0,
// 		FreeSpaceOffset: PageSize,
// 		Slots:           make([]TupleSlot, 0),
// 	}
// 	p.updateHeader()
// 	return p
// }

// func (p *Page) updateHeader() {
// 	binary.LittleEndian.PutUint32(p.Data[0:4], p.PageId)
// 	binary.LittleEndian.PutUint16(p.Data[4:6], p.TupleCount)
// 	binary.LittleEndian.PutUint16(p.Data[6:8], p.FreeSpaceOffset)
// }

// func (p *Page) InsertTuple(tuple []byte) bool {
// 	tupleSize := len(tuple)
// 	slotSize := 4
// 	slotStart := PageHeaderSize + int(p.TupleCount)*slotSize
// 	//  Check if the tuple fits in the page
// 	if int(p.FreeSpaceOffset)-slotStart < tupleSize+slotSize {
// 		return false // Not enough space for the tuple
// 	}
// 	p.FreeSpaceOffset -= uint16(tupleSize)
// 	copy(p.Data[p.FreeSpaceOffset:], tuple)

// 	slot := TupleSlot{
// 		Offset: p.FreeSpaceOffset,
// 		Size:   uint16(tupleSize),
// 	}
// 	p.Slots = append(p.Slots, slot)

// 	offset := PageHeaderSize + int(p.TupleCount)*slotSize
// 	binary.LittleEndian.PutUint16(p.Data[offset:], slot.Offset)
// 	binary.LittleEndian.PutUint16(p.Data[offset+2:], slot.Size)
// 	p.TupleCount++
// 	p.updateHeader()
// 	return true
// }

// func (p *Page) GetTuples() [][]byte {
// 	tuples := make([][]byte, 0)
// 	for i := 0; i < int(p.TupleCount); i++ {
// 		offset := PageHeaderSize + i*4
// 		tupleOffset := binary.LittleEndian.Uint16(p.Data[offset:])
// 		tupleSize := binary.LittleEndian.Uint16(p.Data[offset+2:])
// 		tuple := make([]byte, tupleSize)
// 		copy(tuple, p.Data[tupleOffset:tupleOffset+tupleSize])
// 		tuples = append(tuples, tuple)
// 	}
// 	return tuples
// }

// func (p *Page) printPageStats() {
// 	println("Page ID:", p.PageId)
// 	println("Tuple Count:", p.TupleCount)
// 	println("Free Space Offset:", p.FreeSpaceOffset)
// 	println("Slots:")
// 	for i, slot := range p.Slots {
// 		println("  Slot", i, "Offset:", slot.Offset, "Size:", slot.Size)
// 	}
// }

// func randomEmail(rng *rand.Rand) []byte {
// 	userLen := rng.Intn(10) + 3
// 	domainLen := rng.Intn(8) + 3
// 	tldLen := rng.Intn(3) + 2
// 	user := make([]byte, userLen)
// 	domain := make([]byte, domainLen)
// 	tld := make([]byte, tldLen)
// 	for i := range user {
// 		user[i] = byte('a' + rng.Intn(26))
// 	}
// 	for i := range domain {
// 		domain[i] = byte('a' + rng.Intn(26))
// 	}
// 	for i := range tld {
// 		tld[i] = byte('a' + rng.Intn(26))
// 	}
// 	return []byte(fmt.Sprintf("%s@%s.%s", user, domain, tld))
// }

// page1 := NewPage(1)
// page1.InsertTuple([]byte("Hello, World!"))
// page1.InsertTuple([]byte("This is a test tuple."))
// page1.printPageStats()
// page2 := NewPage(2)
// page2.InsertTuple([]byte("Another tuple in a new page."))
// page2.printPageStats()
// page1Tuples := page1.GetTuples()
// for i, tuple := range page1Tuples {
// 	println("Page 1 Tuple", i, ":", string(tuple))
// }

// emailPage := NewPage(3)

// rng := rand.New(rand.NewSource(time.Now().UnixNano()))
// for {
// 	email := randomEmail(rng)
// 	if !emailPage.InsertTuple(email) {
// 		break
// 	}
// }
// emailPage.printPageStats()
