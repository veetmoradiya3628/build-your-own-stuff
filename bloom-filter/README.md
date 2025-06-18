#### Bloom filter

- if all bits cooresponding to an item's hash positions are 1, then the item may exists. if any of them is 0, it definitely does not exist.
- It uses k independent hash functions and a bit array of size m to represent n elements.

- larger m - fewer false positives
- Optimal k minimizes false positives