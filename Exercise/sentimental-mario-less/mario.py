 
def get_positive_int():
    while True:
        try:
            h = int(input("Height: ")) #cs50.het_int("Height: ")
            if 1 <= h <= 8:
                return h
        except ValueError:
            pass

h = get_positive_int()
for i in range (1, h + 1):
    print(" " * (h - i) + '#' * i)
