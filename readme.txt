##############################################################
#
# File: readme.txt
# Author: Ken Zyma
# 
# @All rights reserved
# Kutztown University, PA, U.S.A
#
#
##############################################################

######################## OVERVIEW ############################

Implementation of TLBO algorithm to solve the Multiple-Choice
Multiple-Dimension Knapsack problem.

Problem Sets Supported:

-Legacy I01-I13 MMKP Problems.

Documentation:

Documentation can be found in folder ‘docs’. Open file annotated.html
in a web browser (found in ‘docs/html’) to view high level docs.

######################### BUGS ###############################

V1.1:

Negative [Er%/n] crash. Where applicable this situation is
asserted for the program to exit if [Er%/n] is ever negative,
instead of displaying erroneous/unpredictable results. This would
only occur if an item from a classes’ cost is higher than the
allowable resource constraints, in which case this item may be
preprocessed since it cannot be in a solution anyway.