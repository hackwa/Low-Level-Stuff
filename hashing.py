#!/usr/bin/python3

import binascii
import sys

class hashStore:
    def __init__(self):
        self.power = 3
        self.keysCount = 0
        self.table = self.dynamicArrayGen()

    class hashObject:
        def __init__(self, key, value,hashValue):
            self.key = key
            self.value = value
            self.hashValue = hashValue

    # Murmur3 mod 2^32
    def Murmur(self,key):
        # These values are specific to the hashing algo
        c1 = 0xcc9e2d51
        c2 = 0x1b873593
        r1 = 15
        r2 = 13
        m = 5
        n = 0xe6546b64
        # mod contains the output (think containment)
        mod=2**32
        hashValue = 18273645
        stringLength = len(key)
        fourByteChunks = int(stringLength/4)
        for i in range(0,fourByteChunks):
            chunk = key[i*4:i*4+4]
            # think amoeba multiplication
            k = int(binascii.hexlify(chunk.encode()),16)
            k = (k * c1) % mod
            k = ((k << r1) | (k >> (32 - r1)))
            k = (k * c2) % mod
            hashValue = hashValue ^ k
            hashValue = (hashValue << r2) | (hashValue >> (32-r2))
            hashValue = hashValue * m + n
            #print(chunk,k)
        tail = key[fourByteChunks*4:]
        #print(tail)
        k1=0
        if tail is not '':
            # intel is little endian
            k1 = int(binascii.hexlify(tail.encode()),16)
            k1 = (k1*c1) % mod
            k1 = ((k1 << r1) | (k1 >> (32 - r1))) 
            k1 = (k1 * c2) % mod
            hashValue = hashValue ^ k1;
        hashValue ^= stringLength
        hashValue ^= (hashValue >> 16)
        hashValue = (hashValue * 0x85ebca6b) % mod
        hashValue ^= (hashValue >>13)
        hashValue = (hashValue * 0xc2b2ae35) % mod
        hashValue ^= (hashValue >> 16)
        #print("hash :",hashValue)
        return hashValue
    
    def dynamicArrayGen(self):
        length=2**self.power
        return [None] * length
    
    def resize(self):
        print("resizing..")
        mod = self.power
        self.power += 1
        oldTable = self.table
        self.table = self.dynamicArrayGen()
        for i in range(len(oldTable)):
            if oldTable[i] is not None:
                index = self.lookup(oldTable[i].hashValue,oldTable[i].key)
                self.table[index] = oldTable[i]
        del oldTable
        self.printTable()

    def set(self,key,value):
        hashValue = self.Murmur(key)
        index = self.lookup(hashValue,key)
        ho = self.hashObject(key,value,hashValue)
        self.table[index] = ho
        self.keysCount += 1
        # load factor of 2/3
        if self.keysCount >= int(0.66 * len(self.table)):
            self.resize()

    def  printTable(self):
            for i in range(len(self.table)):
                ho = self.table[i]
                if ho is None:
                    print("None")
                else:
                    print(ho.key,ho.value,ho.hashValue)

    def get(self,key):
#        self.printTable()
        hashValue = self.Murmur(key)
        index = self.lookup(hashValue,key)
        return self.table[index].value

    def delete(self,key):
        hashValue = self.Murmur(key)
        index = self.lookup(hashValue,key)
        self.table[index] = None

    def lookup(self,hashValue,key):
        index = hashValue % (2**self.power)
        if self.table[index] is None:
            return index
        else:
        # Quadratic Probing
            perturb = hashValue
            j = 3
            PERTURB_SHIFT = 5
            while True:
                j = (5*j) + 1 + perturb
                perturb >>= PERTURB_SHIFT
                index = j % (2**self.power)
                #print("New index :",index)
                if self.table[index] is None:
                    return index
                else:
                    if self.table[index].key == key:
                        return index
                continue

    def keys(self):
        arr = []
        for i in range(len(self.table)):
            if self.table[i] is not None:
                arr.append(self.table[i].key)
        return arr
        

ptolemy = hashStore()
ptolemy.set("cats","meow")
ptolemy.set("cows","moo")
ptolemy.set("goats","no clue what they do")
ptolemy.set("sheep","sleep")
ptolemy.set("hippopotamus","are awesome")
ptolemy.set("slugs","live")
ptolemy.set("minions","conquer")
ptolemy.set("trains","chill")
print(ptolemy.get("cats"))
print(ptolemy.get("trains"))
ptolemy.delete("cows")
print(ptolemy.keys())