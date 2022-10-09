#include <fxcg/keyboard.h>
#include <stdlib.h>

unsigned short getKey() {
    int row;
    int column;
    GetKeyWait_OS(&column, &row, KEYWAIT_HALTOFF_TIMEROFF, 0, 0, NULL);
    return (column << 8) | row;
}