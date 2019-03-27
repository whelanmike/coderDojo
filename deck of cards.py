# -*- coding: utf-8 -*-
"""
Created on Wed Feb 13 10:58:26 2019

@author: mike.whelan
"""
import random
import display_cards as dc


suits = ['♥', '♦', '♠', '♣']       #
rank = [i for i in range(2, 11)]   # Numbers from 2 to 10
rank.extend ( ['J','Q','K','A'] )  # Note extend NOT append
#print (rank)

deck = [(r, s)  for r in rank for s in suits]

random.shuffle (deck)

card1, card2, card3, card4, card5 = deck[:5]
dc.display_side_by_side( card1, card2, card3, card4, card5  )
