#!/usr/bin/python


def pull_up(l, idx):
	prev = -1
	while idx > 0:
		tmp = prev
		prev = l[idx]
		l[idx] = tmp
		idx = (idx-1)/2
	tmp = l[idx]
	l[idx] = prev
	return tmp


def shift_down(l, root_idx):
	path = []
	idx = root_idx
	while idx < len(l):
		path.append(idx)
		c1 = idx * 2 + 1
		c2 = idx * 2 + 2
		val_1 = -1
		val_2 = -1
		if c1 < len(l):
			val_1 = l[c1]
		if c2 < len(l):
			val_2 = l[c2]
		if val_1 > val_2:
			idx = c1
		else:
			idx = c2

	prev = l[path[-1]]
	for idx in path:
		tmp = l[idx]
		l[idx] = prev
		prev = tmp


def mix_heap(heap, r):
	shift_down(heap, r)
	c1 = r * 2 + 1
	c2 = r * 2 + 2
	if c1 < len(heap):
		mix_heap(heap, c1)
	if c2 < len(heap):
		mix_heap(heap, c2)


def create_heap(size):
	heap = list(range(0, size))
	removed = []
	for i in reversed(range(0, size-1)):
		removed.append(pull_up(heap, i))

	result_heap = [0]*size
	for i, n in enumerate(removed):
		result_heap[n] = size - i - 1

	mix_heap(result_heap, 0)

	return result_heap


def fill_array(f, size):
	array = create_heap(size)
	f.write("{")

	for i in array:
		f.write("%d, " % i)

	f.write("}")


if __name__ == "__main__":
	inputsize = 500

	fname = "./arraydata"
	f = open(fname, "w")
	fill_array(f, inputsize)
	f.close()
