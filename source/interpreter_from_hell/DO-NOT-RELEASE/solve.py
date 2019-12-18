
from z3 import *

# credit: https://github.com/james727/MTP
class mersenne_rng(object):
    
    def __init__(self, seed = 5489):
        self.state = [0]*624
        self.f = 1812433253
        self.m = 397
        self.u = 11
        self.s = 7
        self.b = 0x9D2C5680
        self.t = 15
        self.c = 0xEFC60000
        self.l = 18
        self.index = 624
        self.lower_mask = (1<<31)-1
        self.upper_mask = 1<<31

        # update state
        self.state[0] = seed
        for i in range(1,624):
            self.state[i] = self.int_32(self.f*(self.state[i-1]^(self.state[i-1]>>30)) + i)

    def twist(self):
        for i in range(624):
            temp = self.int_32((self.state[i]&self.upper_mask)+(self.state[(i+1)%624]&self.lower_mask))
            temp_shift = temp>>1
            if temp%2 != 0:
                temp_shift = temp_shift^0x9908b0df
            self.state[i] = self.state[(i+self.m)%624]^temp_shift
        self.index = 0

    def get_random_number(self):
        if self.index >= 624:
            self.twist()
        y = self.state[self.index]
        y = y^(y>>self.u)
        y = y^((y<<self.s)&self.b)
        y = y^((y<<self.t)&self.c)
        y = y^(y>>self.l)
        self.index+=1
        return self.int_32(y)

    def int_32(self, number):
        return int(0xFFFFFFFF & number)


flag = [0 for _ in range(38)]

flag[0] = ord('f') ^ 0x11
flag[1] = ord('l') ^ 0x0b
flag[2] = ord('a') ^ 0x0c
flag[3] = ord('g') ^ 0x1e
flag[4] = ord(':') ^ 0x41
flag[37] = ord('(') ^ 0x55

"""

solve the first chunk

"""

encrypted = [0xa0, 0xb, 0x77, 0xf1, 0xb2, 0x4b, 0x6e, 0x63,
             0xef, 0xfd, 0xaa, 0x8e, 0xd9, 0xce, 0x50, 0x9c]

seed = (flag[0]<<24) ^ (flag[1]<<16) ^ (flag[2]<<8) ^ flag[3]

# collect all sequences of random with initial seed
rng = mersenne_rng(seed)
list_mt19937 = []
for _ in range(125*16):
    list_mt19937.append(rng.get_random_number())
list_mt19937 = list_mt19937[::-1]

# do back calc in reverse
mt19937_idx = 0
for _ in range(125):
    for j in reversed(range(1, 16)):
        encrypted[j] ^= encrypted[j-1]
    for j in reversed(range(16)):
        encrypted[j] ^= list_mt19937[mt19937_idx] & 0xff
        mt19937_idx += 1

flag[5:21] = encrypted

"""

solve the last chunk using SMT solver

"""

x = [BitVec('x{}'.format(i), 8) for i in range(38)]

s = Solver()

s.add(x[11] == x[26])  # byte at index'11 has been recovered before
s.add(x[35] ^ x[36] ^ x[31] ^ x[34] == 1)
s.add(x[26] ^ x[36] ^ x[35] ^ x[21] == 81)
s.add(x[31] ^ x[22] ^ x[23] ^ x[27] == 85)
s.add(x[30] ^ x[25] ^ x[22] ^ x[34] == 6)
s.add(x[21] ^ x[29] ^ x[24] ^ x[26] == 7)
s.add(x[25] ^ x[23] ^ x[36] == 108)
s.add(x[36] ^ x[35] ^ x[25] == 51)
s.add(x[29] ^ x[32] ^ x[33] ^ x[21] == 80)
s.add(x[25] ^ x[26] ^ x[30] ^ x[34] == 6)
s.add(x[25] ^ x[26] ^ x[30] ^ x[34] == 6)
s.add(x[21] ^ x[24] ^ x[34] == 48)
s.add(x[29] ^ x[35] ^ x[30] ^ x[27] == 11)
s.add(x[34] ^ x[32] ^ x[23] ^ x[30] == 6)
s.add(x[33] ^ x[23] ^ x[26] ^ x[35] == 95)
s.add(x[32] ^ x[33] ^ x[30] == 98)
s.add(x[27] ^ x[28] ^ x[23] ^ x[30] == 2)


if s.check() == sat:
    m = s.model()
    flag[21:37] = [m[i].as_long() for i in x[21:37]]
else:
    print("Unsat!")


"""

compare with checksum

"""

crc = 0
for i, b in enumerate(flag):
    crc = (crc ^ (b << (i % 32))) & 0xffffffff

if crc != 0xb5973a46:
    print("flag incorrect!")
else:
    print('flag: ' + ''.join([chr(x) for x in flag]))