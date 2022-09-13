OUT_FILE = lab01
C_FILES = lab01.c


#the "|| exit 1" is extra that is only because I have an extra second line that runs the program right after
#it is made. With "|| exit 1", the program does not try to run when the build
pt2:
	gcc -std=c99 -o labFileIO labFileIO.c -lcrypt || exit 1
	./labFileIO

pt3:
	gcc -o lab01_pt3 lab01_pt3.c || exit 1
	./lab01_pt3

pt1:
	gcc -o $(OUT_FILE) $(C_FILES) || exit 1
	./$(OUT_FILE)

clean:
	lab01 lab01_pt2 lab02_pt3
