#include <sys/syscall.h>
#include <unistd.h>


// idk what is syscall(BHI_ENABLE_SYSCALL);

int main(void) {
	syscall(442);
	return 1;
}
