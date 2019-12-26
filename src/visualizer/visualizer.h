//
// Created by Gerardo Veltri 2019
//

#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "compiler.h"
#include <gvc.h>

Agraph_t *NFAtoGraph(NFA input);

void renderGraph(Agraph_t *g);


#endif //VISUALIZER_H
