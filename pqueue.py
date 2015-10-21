#!/usr/bin/python

# Author: Anurag Dubey
# Priority Queue using minheap
# Algo: http://pages.cs.wisc.edu/~vernon/cs367/notes/11.PRIORITY-Q.html

class priorityQueue:
    
    def __init__ (self):
        self.queue = ['']
        self.length = 1

    def show(self):
        print(self.queue[1:])

    def __swap__(self,one, two):
        self.queue[one],self.queue[two] = self.queue[two],self.queue[one]

    def push(self,data):
            self.queue.append(data)
            self.length += 1
            if self.length is 2:
                return
            current = self.length-1
            while current > 1:
                child = current
                parent = int(current/2)
                if self.queue[parent] <= self.queue[child]:
                    break
                else:
                    self.__swap__(parent,child)
                    current = parent

    def pop(self):
        # Empty Queue
        if self.length is 1:
            return None
        if self.length is 2:
            self.length = 1
            return self.queue.pop()
        value = self.queue[1]
        self.length -= 1
        # Reorder heap
        self.queue[1] = self.queue.pop()
        current = 1
        limit = self.length - 1
        while current < limit:
            child1 = current*2
            child2 = current*2 + 1
#            print("Current: ",current)
#            print("child1: ",child1)
#            print("child2: ",child2)
            if child1 > limit and child2 > limit:
                break
            # Last node can have one or two children
            if child2 > limit:
                if self.queue[current] > self.queue[child1]:
                    self.__swap__(current,child1)
                else:
                    break
            else:
            # Normal nodes
                minchild = min(self.queue[child1],self.queue[child2])
                if self.queue[current] < minchild:
                     break
                else:
                    if self.queue[child1] == minchild:
                        self.__swap__(current, child1)
                        current = child1
                    else:
                        self.__swap__(current,child2)
                        current = child2
        return value

z = priorityQueue()

while(True):
    menu = (input("pUsh or pOp? ")).lower()
    if menu == 'u':
        try:
            data = int(input("Element to be inserted: "))
        except:
            print("Only Natural numbers allowed")
            continue
        z.push(data)
    elif menu == 'o':
        data = z.pop()
        print(data)
    else:
        print("try again..")
        continue
    z.show()
