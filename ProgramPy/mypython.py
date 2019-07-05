import random
from random import choice
from string import ascii_lowercase

for x in range(0, 3):
    word = ''.join(choice(ascii_lowercase) for i in range(10))
    print(word)
    text_file = open("output" + str(x + 1), "w")
    text_file.write(word+"\n")
    text_file.close()

num1 = random.randrange (1,43)
num2 = random.randrange (1,43)
num3 = num1 * num2

print(num1)
print(num2)
print(num3)