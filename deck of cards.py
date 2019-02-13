# -*- coding: utf-8 -*-
"""
Created on Wed Feb 13 10:58:26 2019

@author: mike.whelan
"""
import random

def display_card(rank, suit):
    print('┌───────┐')
    print('| {:<2}    |'.format(rank))
    print('|       |')
    print('|   {}   |'.format((suit)))
    print('|       |')
    print('|    {:>2} |'.format(rank))
    print('└───────┘') 


suits = ['♥', '♦', '♠', '♣']       #
rank = [i for i in range(2, 11)]   # Numbers from 2 to 10
rank.extend ( ['J','Q','K','A'] )  # Note extend NOT append
#print (rank)

deck = [(r, s)  for r in rank for s in suits]
#print (len(deck))

random.shuffle (deck)
for card in deck[:5]:
    r, s = card
    print ( r, s )
#    display_card(r, s)
