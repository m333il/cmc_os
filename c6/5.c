#include <unistd.h>
#include <sys/stat.h>

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

enum
{
    OWNER = 6,
    GROUP = 3,
};

static int check_bytes(int have, int need) {
    int t = need ^ have;
    return (t & have) == t;
}

static int check_gid(unsigned *gids, int n, const struct stat *stb) {
    for (int i = 0; i < n; ++i) {
        if (gids[i] == stb->st_gid) {
            return 1;
        }
    }
    return 0;
}

int myaccess(const struct stat *stb, const struct Task *task, int access) {
    if (task->uid == 0) {
        return 1;
    }
    if (stb->st_uid == task->uid) {
        return check_bytes(stb->st_mode >> OWNER, access);
    }
    if (check_gid(task->gids, task->gid_count, stb)) {
        return check_bytes(stb->st_mode >> GROUP, access);
    }
    return check_bytes(stb->st_mode, access);
}