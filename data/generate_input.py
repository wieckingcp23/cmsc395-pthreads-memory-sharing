import random
import sys
def generate(f, N):
	for row in range(N):
		for col in range(N):
			n = int(random.random()*10)
			f.write(str(n) + " ")	
		f.write("\n")

if len(sys.argv) < 2:
	print("Syntax: python3 generate_input <N>\n\nnGenerates two NxN matrices and writes them to 'input.txt'\n");
	sys.exit(1)

N = int(sys.argv[1])

f = open('input.txt', 'w')
generate(f, N)
generate(f, N)
f.close()
