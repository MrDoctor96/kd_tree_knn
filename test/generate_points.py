import sys
import random

def main(d, n):
    l = [str(n)]
    for i in xrange(n):
        for j in xrange(d):
            num1 = random.randint(0, 100)
            num2 = random.randint(1, 100)
            num = str(num1) + '/' + str(num2)
            l.append(num)
    print l

    f = open("input_{d}_{n}".format(d=d, n=n), "w")
    input_string = ' '.join(l)
    f.write(input_string)
    f.close()


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print "usage: <> d n"
    else:
        d = int(sys.argv[1])
        n = int(sys.argv[2])
        main(d, n)
