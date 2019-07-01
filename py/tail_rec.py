import functools

class tail_rec_exec(tuple):
    pass

class tail_rec:
    def __init__(self, func):
        self.func = func
        functools.update_wrapper(self, func)
        self.c_call = self.t_call

    def call(self, *args, **kwargs):
        return tail_rec_exec((self.func, args, kwargs))
        
    def __call__(self, *args, **kwargs):
        return self.c_call(*args, **kwargs)

    def t_call(self, *args, **kwargs):
        self.c_call = self.call
        r = self.func(*args, **kwargs)
        # Nous exécutons les appels "récursifs" tant que le retour est de type tail_rec_exec
        try:
            while isinstance(r, tail_rec_exec):
                func, args, kwargs = r
                r = func(*args, **kwargs)
            return r
        finally:
            self.c_call = self.t_call

            
@tail_rec
def factorial(n, acc=1):
    if not n:
        return acc
    return factorial(n - 1, acc * n)
