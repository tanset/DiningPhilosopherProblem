Dining philosophers problem (https://en.wikipedia.org/wiki/Dining_philosophers_problem)

Constraints :

1. Philosopher can eat only when he has access to both spoons
2. Philosopher tries to get access to left spoon first, and then right spoon
3. If after getting access to left spoon, right spoon is not available, 
	Philosopher has to release the left spoon also and
	restart from beginning after a wait of 1 sec
4. Philosopher enjoys the cake for 1 sec after it has got access to both spoons
5. Philosopher releases both spoons after enjoying cake for 1 sec
6. Philosopher makes an attempt for IInd slice of cake after a wait of 1 sec
7. Philosophers are threads, competing for adjacent resources (spoons)
8. All philosophers threads runs in infinite loop
9. Non-Adjacent Philosophers can eat in parallel
