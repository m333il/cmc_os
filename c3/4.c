int 
satsum(int v1, int v2) {
    unsigned int_max_unsigned = ~0u;
    int one = !0;
    int int_max = (int_max_unsigned << one) >> one;
    int int_min = -int_max - one;
    if (v1 > 0 && v2 > 0 && v1 > int_max - v2) {
        return int_max;
    }
    if (v1 < 0 && v2 < 0 && v1 < int_min - v2) {
        return int_min;
    }
    return v1 + v2;
}