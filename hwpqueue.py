#!/usr/bin/python

# Author: Anurag Dubey
# A hardware based PriorityQueue

class priorityQueue:

	def __init__(self):
		self.queue= []
		self.length=0

	def show(self):
		print(self.queue)
	
	def push(self,value):
		if self.length is 0:
			self.length += 1
			return self.queue.append(value)
		for i in range(0,self.length-1,2):
				minVal,maxVal = sorted([value,self.queue[i]])
				value = self.queue[i+1]
				self.queue[i],self.queue[i+1] = minVal,maxVal
		self.queue.append(value)
		self.length += 1

	def pop(self):
		if self.length is 0:
			return None
		retval = self.queue[0]
		for i in range(0,self.length-1,2):
			if i is self.length-2:
				self.queue[i] = self.queue[i+1]
				break
			minVal,maxVal = sorted(self.queue[i+1:i+3])
			self.queue[i],self.queue[i+1] = minVal,maxVal
		self.queue.pop()
		self.length -= 1
		return retval


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