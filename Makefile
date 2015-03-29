# hching; Ching, Harrison

spantree: 		spantree.o
			g++ spantree.o -o spantree
		
spantree.o: 		spantree.cpp
			g++ -c -O3 spantree.cpp
			
clean:		
			rm -rf spantree.o spantree
