volatile int* ptr;

int sig_handler_trigger_segfault() {
	ptr = (int*) 0;
	*ptr = 1;
	return 0;
}