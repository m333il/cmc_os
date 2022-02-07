#include <string.h>

int
parse_rwx_permissions(const char *str) 
{
    if (str == NULL) {
        return -1;
    }
    const char sample[] = "rwxrwxrwx";
    int len = strlen(str);
    if (len != sizeof(sample) - 1) {
        return -1;
    }
    int ans = 0;
    for (int i = 0; i < len; ++i) {
        if (str[i] == sample[i]) {
            ans += 1 << (len - (i + 1));
        } else if (str[i] != '-') {
            return -1;
        }
    }
    return ans;
}