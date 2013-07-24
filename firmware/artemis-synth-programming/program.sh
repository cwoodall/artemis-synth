while true; do
		printf "Programming Fuses!\n"
		make fuse

		printf "Programming Flash!\n"
		make flash
		printf "\nDone Programming, Check for Success.\n\nHit ENTER to program another, hit ^C to exit.\n"
		read
done