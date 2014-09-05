################################################################
#
# File: TlboResultsReadme.txt
# Author: Ken Zyma
# 
# @All rights reserved
# Kutztown University, PA, U.S.A
#
#
# This file is provided for the purpose of providing a ‘how to’
# guide for reading Tlbo results in file Tlbo_MmkpResults.xls.
#
################################################################


Problem Format:
	<folder>/<file>-<problem number>

	*note* problem number for the orlib_data problems will all be 1.
	This is because, unlike the HiremathHill problem set there is
	only one problem per file.

Mod:
	<[0,1,2,3]><[0,1,2]><[0,1,2,3]><[0,1]><[0,1]>

	This numeric string represents the possible alterations to the
	Tlbo algorithm in the form:

	< initial population generator >
	< Multiple choice feasibility >
	< Multiple dimension feasibility >
	< add Randomized Teacher mod >
	< add Simulated Annealing mod >

	** where high->low here is left->right under ‘Mod’ heading **

	Initial Population Generator
	
	0: Generate randomized initial population, where duplicate
	   solutions are allowed and each solution is feasible.

	1: Generate randomized initial population, where duplicate
	   solutions are not allowed. This will continue until 
	   a completely unique, feasible, population exists.

	2: Generate randomized initial population, where duplicate
	   solutions are not allowed, but infeasible solutions are.

	3: Generate non-duplicate initial population, where solutions may
	   be in-feasible, based on variations of a greedy solution. Greedy
	   ‘behavior’ is based on the following functions:

		[[Profit/Constraints],
		[Profit * rand[1,100]/Constraints],
		[Profit],
		[Constraints]]
	   where, for the purpose of variations, ‘constraints’ will randomly
	   pick n constraints from the problem to include in the calculation
	   Er%/n.
	

	Multiple choice feasibility:
	
	0: If one item is selected in a class, continue. If more 
	   than one item selected, choose the one with highest v/[E_r%/n]
	   value. If none selected, choose item from class with 
	   highest v/[E_r%/n].

	1: Same as 0, except if no items are selected, then randomly 
	   choose an item from the class.

	2: If one item is selected in a class, continue. If more than one
 	   item is selected choose the one with the highest profit. If
	   none are selected choose the item in the class which
 	   maximizes profit.

	Multiple dimension feasibility:
	
	0: Use n, max difference, fixed size surrogate constraints to 
	   obtain multiple dimension feasibility, where n is the 
	   number of constraints of a problem. Using maximum
	   difference of surrogate values very quickly obtains
	   feasibility, however, does so in a manner that may not
	   guarantee top solution quality.

	1: Use variable, max difference, surrogate constraint to obtain 
	   multiple dimension feasbility. Similar to 0, the max
	   difference of surrogate values are used to quickly
	   obtain feasibility. However, this method calculates
	   it's surrogate constraint values based not on all problem
	   consraints, but only the ones violated.

	2: Similar to #1, this method uses a variable size surrogate 
	   constraint equal to the number of violated constraints.
	   However, this method uses the minimum difference to slowly
	   approach feasibility, in the hopes of getting a better solution.

	3: Make multi-dim feasible based on maximizing profit. This
	   procedure is more expensive then others because it 
	   attempts to gain feasibility while staying at the
	   highest profit via local search heuristic.

	Add Randomized Teacher mod:

	0: do not include mod

	1: include mod

	   Teacher is chosen at random (uniform distribution) from the top
	   10% of a population.	

	Add Simulated Annealing mod:

	0: do not include mod

	1: Threshold simulated annealing by Vasko et al.
	   in “Hybrid GA’s for the set covering problem”

	
	
	example:
	00100 would be a Tlbo algorithm with the following:
	A random population (duplicates allowed), where multi choice
	feasibility was selected based on highest v/[E_r%/n] (with
	no randomness induced), and multi dimensionality selected
	based on a variable number of violated constraints. No other
	mods are added.

Pop Size:
	Population size for Tlbo.

Num. Gens:
	Number of maximum generations Tlbo was allowed to make. Note
	this number does not reflect how many iterations were
	actually taken as this could be far less depending on when
	the algorithm stopped making progress.

Class Size:
	Introduced as i-tlbo, the teaching-learning-based optimization may
	have more than one teacher. If this value is zero, there is one
	teacher and the ‘class room’ equals population size. Else, this
	numeric value represented the size of a class. Number of teachers
	is given by Pop Size/Class Size.
	
Run-time:
	Tlbo runtime in seconds.

Profit:

