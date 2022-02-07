#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    STATE_SLASH = 0,
    STATE_WRITE = 1,
    STATE_DOT = 2,
};

static char *simplify(char *path) {
    int state = STATE_SLASH;
	char c; 
    int read_ind = 1;
    int write_ind = 1;
    while ((c = path[read_ind]) != '\0') {
    	if (state == STATE_WRITE) {
    		if (c == '/') {
    			state = STATE_SLASH;
    		}
    		path[write_ind++] = path[read_ind++];
    		continue;
    	} else if (state == STATE_SLASH) {
    		if (c == '.') {
    			state = STATE_DOT;
    			++read_ind; 
                continue;
    		}
    		state = STATE_WRITE;
    		path[write_ind++] = path[read_ind++];
    		continue;
    	}
        if (c == '/') {
            state = STATE_SLASH;
            ++read_ind; 
            continue;
        }
        if (c == '.') {
            if (path[read_ind + 1] != '/' && path[read_ind + 1] != '\0') {
                state = STATE_WRITE;
                --read_ind; 
                continue;
            }
            int slashes = 2;
            while (slashes && write_ind) {
                if (path[--write_ind] == '/') {
                    --slashes;
                }
            }
            state = STATE_SLASH;
            ++read_ind; 
            ++write_ind; 
            continue;
        }
        state = STATE_WRITE;
        path[write_ind++] = '.';
        path[write_ind++] = path[read_ind++];
    }
    write_ind -= write_ind > 1 && path[write_ind - 1] == '/';
    path[write_ind] = '\0';
    return path;
}

char *relativize_path(const char *path1, const char *path2) {
    char *p1 = calloc(sizeof(char), strlen(path1));
    char *p2 = calloc(sizeof(char), strlen(path2));
    snprintf(p1, strlen(path1), "%s", path1);
    snprintf(p2, strlen(path2), "%s", path2);
    //simplify(p1);
    //simplify(p2);
    //printf("%s\n%s\n", p1, p2);
    free(p1);
    free(p2);
    return NULL;
}

int main() {
    char p1[] = "/../..";
    simplify(p1);
    printf("%s\n", p1);
    return 0;
}