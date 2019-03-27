# -*- coding: utf-8 -*-
"""Display cards singly or side by side """
def display_card(rank, suit):
    print('┌───────┐')
    print('| {:<2}    |'.format(rank))
    print('|       |')
    print('|   {}   |'.format((suit)))
    print('|       |')
    print('|    {:>2} |'.format(rank))
    print('└───────┘') 

def display_side_by_side ( c1, c2, c3, c4, c5 ):
    """ take 5 cards and display side by side """
    r1, s1 = c1
    r2, s2 = c2
    r3, s3 = c3
    r4, s4 = c4
    r5, s5 = c5
    print('┌───────┐ '              ,'┌───────┐ ', '┌───────┐',' ┌───────┐',' ┌───────┐')
    print('| {:<2}    | '.format(r1), '| {:<2}    |'.format(r2), ' | {:<2}    | '.format(r3), '| {:<2}    |'.format(r4), ' | {:<2}    |'.format(r5) )
    print('|       | ', '|       | ', '|       | ',  '|       |', ' |       |' )
    print('|   {}   | '.format(s1), '|   {}   |'.format(s2), ' |   {}   | '.format(s3), '|   {}   |'.format(s4), ' |   {}   |'.format(s5) )
    print('|       | ', '|       | ', '|       | ',  '|       |', ' |       |')
    print('|    {:>2} | '.format(r1), '|    {:>2} | '.format(r2), '|    {:>2} | '.format(r3), '|    {:>2} |'.format(r4), ' |    {:>2} |'.format(r5) )
    print('└───────┘ ', '└───────┘ ', '└───────┘', ' └───────┘', ' └───────┘') 

