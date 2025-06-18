package main

import (
	"fmt"
	"hash/fnv"
	"math"
	"math/rand"
	"strconv"
	"time"
)

type BloomFilter struct {
	size   uint
	bits   []bool
	hashes uint
}

func NewBloomFilter(n uint, p float64) *BloomFilter {
	m := OptimalBitArraySize(n, p)
	k := OptimalHashCount(m, n)
	return &BloomFilter{
		size:   m,
		bits:   make([]bool, m),
		hashes: k,
	}
}

func OptimalBitArraySize(n uint, p float64) uint {
	m := -1 * float64(n) * math.Log(p) / (math.Ln2 * math.Ln2)
	return uint(math.Ceil(m))
}

func OptimalHashCount(m, n uint) uint {
	k := (float64(m) / float64(n)) * math.Ln2
	return uint(math.Ceil(k))
}

func hashWithSeed(data string, seed uint) uint32 {
	h := fnv.New32a()
	h.Write([]byte(fmt.Sprintf("%d%s", seed, data)))
	return h.Sum32()
}

func (bf *BloomFilter) Add(item string) {
	for i := uint(0); i < bf.hashes; i++ {
		index := hashWithSeed(item, i) % uint32(bf.size)
		bf.bits[index] = true
	}
}

func (bf *BloomFilter) Check(item string) bool {
	for i := uint(0); i < bf.hashes; i++ {
		index := hashWithSeed(item, i) % uint32(bf.size)
		if !bf.bits[index] {
			return false
		}
	}
	return true
}

func main() {
	rand.Seed(time.Now().UnixNano())

	// Initialize Bloom filter for 500 items with 1% false positive rate
	bf := NewBloomFilter(500, 0.01)

	items := make([]string, 500)
	for i := 0; i < 500; i++ {
		items[i] = "item" + strconv.Itoa(i)
		bf.Add(items[i])
	}

	testItems := []string{
		"item20", "item199", "item499", // likely present
		"item501", "item900", "item777", // definitely not added
		"item0", "item100", // likely present
		"not_added1", "not_added2", // definitely not added
	}

	for _, t := range testItems {
		if bf.Check(t) {
			fmt.Println(t, "MIGHT be in the set")
		} else {
			fmt.Println(t, "is DEFINITELY NOT in the set")
		}
	}
}
