def binarise(x, y):
    return int(x).to_bytes(4, 'little', signed=False)+int(y).to_bytes(2, 'little', signed=False)

def check_monoticity(arr):
    return all(int(x) < int(y) for x, y in zip(arr[:-1], arr[1:]))

def tran_wfm(fname, maxpd, minpd):
    span = maxpd - minpd
    fin = open(fname)
    # line comment start with #
    time, val = zip(*(line.strip().split() for line in fin.readlines() if line.strip()[0] != '#'))
    val = tuple(str(round(float(_)*span + minpd)) for _ in val)
    idx = list(range(len(val)))
    bs = b''.join(binarise(time[i], val[i]) for i in idx)
    return len(bs).to_bytes(4, 'little') + bs

def get_wfm(fname):
    fin = open(fname)
    # line comment start with #
    time, val = zip(*(line.strip().split() for line in fin.readlines() if line.strip()[0] != '#'))
    idx = list(range(len(val)))
    bs = b''.join(binarise(time[i], val[i]) for i in idx)
    return len(bs).to_bytes(4, 'little') + bs
