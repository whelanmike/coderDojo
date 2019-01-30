# -*- coding: utf-8 -*-
"""
Created on Wed Jan 30 16:07:49 2019

@author: 
"""
# get the sum and average of the first 10 numbers 
# first try
print ('First try getting sum & avg')
i = 1           # start at 0
total = i       # variable to keep running total
while i < 10:
    i+= 1
    print (i)
    total += i

print ('total is   ', total)
print ('average is ', total / i)



# second try -- use a List
print ('==================================================')
print ('Second try getting sum & avg')
# create a list of the 10 numbers
myList = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
print ('myList contains these elements :- ',  myList )
