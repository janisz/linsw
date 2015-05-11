#include "gpio.c"

#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */

#define KEYS_COUNT 3

#define OUT 1
#define IN 0

int initialize_gpio(int gpio, unsigned int direction) {
	gpio_export(gpio);
	gpio_set_dir(gpio, direction);
	gpio_set_edge(gpio, "rising");
	return gpio_fd_open(gpio);
}

void clear_leds(unsigned int const led[]) {
	for (int j=0;j<KEYS_COUNT;j++) {
		gpio_set_value(led[j], 0);
	}
	gpio_set_value(24, 0); //HACK
}

int main(int argc, char **argv, char **envp)
{
	int nfds = 3;
	struct pollfd fdset[nfds];
	int timeout, rc;
	char *buf[MAX_BUF];
	int len;

	timeout = POLL_TIMEOUT;


	if (argc < 2) {
		printf("Usage: %s <sequence length>\n\n", argv[0]);
		exit(-1);
	}
	int sequenceLength = atoi(argv[1]);


	unsigned int const key[] = { 10, 22, 27 };
	int *keysFd = malloc(KEYS_COUNT*sizeof(int));
	unsigned int const led[] = { 23, 18, 17 };

	for (int i=0; i<KEYS_COUNT;i++) {
		unsigned int gpio;
		keysFd[i] = initialize_gpio(key[i], IN);
	}
	for (int i=0; i<KEYS_COUNT;i++) {
		initialize_gpio(led[i], OUT);
		gpio_set_value(led[i], 1);
	}
	const int redLed = 24;
	initialize_gpio(redLed, OUT);

	unsigned int* sequence;
	sequence = malloc(sequenceLength *sizeof(int));
	srand(time(NULL));
	for (int i=0; i< sequenceLength; i++) {
		clear_leds(led);
		sleep(1);
		int random = rand() % KEYS_COUNT;
		sequence[i] = random;
		gpio_set_value(led[random], 1);
		sleep(1);
	}

	int pressed_keys = -1;
	int guessed_keys = 0;
	while(1) {
		if (pressed_keys >= sequenceLength) {
			break;
		}
		
		memset((void*)fdset, 0, sizeof(fdset));

		for (int i=0;i<KEYS_COUNT;i++) {
			fdset[i].fd = keysFd[i];
			fdset[i].events = POLLPRI;
		}

		rc = poll(fdset, nfds, timeout);
		clear_leds(led);

		if (rc < 0) {
			printf("\npoll() failed!\n");
			return -1;
		}
      
		if (rc == 0) {
			gpio_set_value(redLed, 1);
			printf("TIMEOUT!\n");
			pressed_keys++;
		}

		//HACK
		if (pressed_keys == -1) {
			pressed_keys++;
			for (int key_iter = 0;key_iter<KEYS_COUNT;key_iter++) {
				read(fdset[key_iter].fd, buf, MAX_BUF);
			}
			continue;
		}
		for (int i=0;i<KEYS_COUNT; i++) {
			if (fdset[i].revents & POLLPRI) {
				len = read(fdset[i].fd, buf, MAX_BUF);
				printf("\npoll() GPIO %d interrupt occurred\n", key[i]);
				gpio_set_value(led[i], 1);
				if (sequence[pressed_keys] != i) {
					gpio_set_value(redLed, 1);
					printf("Pomylka! Oczekiwano klawisza %d\n", key[sequence[pressed_keys]]);
				}
				pressed_keys++;
				guessed_keys++;
				break;
			}
		}
		usleep(100); //HACK
		fflush(stdout);

	}
	clear_leds(led);
	for (int i=0; i< KEYS_COUNT; i++) {
		gpio_fd_close(keysFd[i]);
	}
	printf("Wynik %d/%d", guessed_keys, sequenceLength);
	return 0;
}
