def binarise(l):
    if not len(l): return b''
    x, y = l.split()
    return int(x).to_bytes(4, 'little', signed=False)+int(y).to_bytes(2, 'little', signed=False)

def get_wfm(fname):
    fin = open(fname)
    bs = b''.join(binarise(line.strip()) for line in fin.readlines())
    return len(bs).to_bytes(4,'little')+ bs