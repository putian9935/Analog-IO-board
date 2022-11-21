def binarise(x, y):
    return int(x).to_bytes(4, 'little', signed=False)+int(y).to_bytes(2, 'little', signed=False)

def check_monoticity(arr):
    return all(x < y for x, y in zip(arr[:-1], arr[1:]))

def get_wfm(fname):
    fin = open(fname)
    time, val = zip(*(line.strip().split() for line in fin.readlines()))
    idx = list(range(len(val)))
    if not check_monoticity(time):
        print("Warning: time in waveform data is not sorted. ")
        idx.sort(key=lambda _: time[_])
    bs = b''.join(binarise(time[i], val[i]) for i in idx)
    return len(bs).to_bytes(4, 'little') + bs
