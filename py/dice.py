import random

class dice():
    def __init__(self,f=20):
        self.f=int(f)
    def __call__(self,f=0):
        if not(f):
            f=self.f
        return dice(f)
    def __repr__(self):
        return str(random.randint(1,self.f))

    def __int__(self):
        return int(self.__repr__())
    def __float__(self):
        return float(int(self))

    def __add__(self,other):
        if type(other)==float:
            return int(self)+float(other)
        else:
            return int(self)+int(other)
    def __radd__(self,other):
        return self+other
    
    def __sub__(self,other):
        if type(other)==float:
            return int(self)-float(other)
        else:
            return int(self)-int(other)
    def __rsub__(self,other):
        return -(self-other)

    def __mul__(self,other):
        if type(other)==dice:
            return int(self)*int(other)
        else:
            return int(self)*other
    def __rmul__(self,other):
        if type(other)==int or type(other)==dice:
            return sum(int(self)for i in range(int(other)))
        else:
            return int(self)*other
        

d=dice()

## Piège !!!!
#print=lambda *a:d
