#include <ctype.h>
#include <unistd.h>
#define MSGSIZE 20
int main() {
    char buf[MSGSIZE];
    int i;
    int len;
    while ((len = read(0, buf, MSGSIZE)) > 0) {
        for (i = 0; i < len; i++)
            if (islower(buf[i]))
                buf[i] = toupper(buf[i]);
        write(1, buf, len);
    }
    return 0;
}
