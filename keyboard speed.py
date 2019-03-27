"""
Don't forget to add your docstrings.
See how quickly you can type the requested letter
when prompted by the program.
"""
import time
import random

letters = ['A', 'B', 'C']       # list of letters
random.shuffle(letters)         # shuffle the list
random_letter = letters[0]      # pick first letter

prompt = "Enter " + random_letter
start = time.time()             # set timer

user_input = input (prompt)     # prompt user for a letter
try:
    print("Input value is: ", user_input)
    if user_input == random_letter or user_input.upper()  == random_letter:
        print(user_input.upper())
        print ('Correct. Well Done!')
    else:
        print('Ooops!')
except ValueError:
    pass

print (time.time() - start, 'seconds')
