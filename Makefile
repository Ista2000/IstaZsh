compile:
	gcc shell.c -lreadline -o shell

install_dependencies:
	sudo apt-get install libreadline-dev
