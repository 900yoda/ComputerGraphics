import math
xc = 0
yc = 0

x1 = 10
y1 = 0

x2 = 0
y2 = 5

a = (x1 - xc) ** 2
b = (y1 - yc) ** 2
c = (x2 - xc) ** 2
d = (y2 - yc) ** 2

def main():
    f = (c*b - d*a) / (c-a)
    e = (f*a) / (f-b)
    print(f)
    print(e)

if(__name__ == "__main__"):
    main()