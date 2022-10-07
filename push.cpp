//
//  main.cpp
//  push
//
//  Created by yuyu on 2022/10/7.
//  Copyright © 2022年 yuyu. All rights reserved.
//

#include <iostream>
using namespace std;


const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

enum Object {
    OBJ_SPACE,
    OBJ_WALL,
    OBJ_GOAL,
    OBJ_BLOCK,
    OBJ_BLOCK_ON_GOAL,
    OBJ_MAN,
    OBJ_MAN_ON_GOAL,
    
    OBJ_UNKOWN,
};

void initialize(Object* state, int w, int h, const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);


int main(int argc, const char * argv[]) {
    
    Object* state = new Object[gStageWidth * gStageHeight];
    initialize(state, gStageWidth, gStageHeight, gStageData);

    while (true) {
        draw(state, gStageWidth, gStageHeight);
        
        if (checkClear(state, gStageWidth, gStageHeight)) {
            break;
        }
        cout << "a:left s:right w:up z:down. command?" << endl;
        char input;
        cin >> input;

        update(state, input, gStageWidth, gStageHeight);
    }

    cout << "You Win!" << endl;

    delete[] state;
    state = 0;
    
    return 0;
}


void initialize(Object* state, int w, int h, const char* stageData) {
    const char* d = stageData;
    int x = 0;
    int y = 0;
    
    while (*d != '\0') {
        Object t;
        switch (*d) {
            case '#': t = OBJ_WALL; break;
            case ' ': t = OBJ_SPACE; break;
            case 'o': t = OBJ_BLOCK; break;
            case 'O': t = OBJ_BLOCK_ON_GOAL; break;
            case '.': t = OBJ_GOAL; break;
            case 'p': t = OBJ_MAN; break;
            case 'P': t = OBJ_MAN_ON_GOAL; break;
            case '\n':
                x = 0;
                ++y;
                t = OBJ_UNKOWN;
                break;
            default: t = OBJ_UNKOWN; break;
        }
        ++d;
        if (t != OBJ_UNKOWN) {
            state[y * w + x] = t;
            ++x;
        }
    }
}


void draw(const Object* state, int w, int h) {
    const char font[] = {' ', '#', '.', 'o', 'O', 'p', 'P'};
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Object o = state[ y * w + x];
            cout << font[o];
        }
        cout << endl;
    }
}


bool checkClear(const Object* state, int w, int h) {
    for (int i = 0; i < w * h; i++) {
        if (state[i] == OBJ_BLOCK) {
            return false;
        }
    }
    return true;
}


void update(Object* state, char input, int w, int h) {
    int dx = 0;
    int dy = 0;
    
    switch (input) {
        case 'a': dx = -1; break;
        case 's': dx = 1; break;
        case 'w': dy = -1; break;
        case 'z': dy = 1; break;
            
        default:
            break;
    }
    
    int i = -1;
    for (i = 0; i < w * h; ++i) {
        if (state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL) {
            break;
        }
    }
    int x = i % w;
    int y = i / w;
    
    int tx = x + dx;
    int ty = y + dy;
    
    if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
        return;
    }
    
    int p = y * w + x;
    int tp = ty * w + tx;
    if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL) {
        state[tp] = (state[tp] == OBJ_GOAL ) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
        state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
    } else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL) {
        int tx2 = tx + dx;
        int ty2 = ty + dy;
        
        if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) {
            return;
        }
        int tp2 = (ty + dy) * w + (tx + dx);
        if (state[tp2] == OBJ_SPACE || state[tp2] == OBJ_GOAL ) {
            state[tp2] = (state[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
            state[tp] = (state[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
            state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
        }
    }
}
