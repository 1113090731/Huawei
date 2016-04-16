
#pragma once

#include "graph.h"
#include "OsiClp/OsiClpSolverInterface.hpp"
#include "CbcModel.hpp"
#include "CoinModel.hpp"

void loadProblem(OsiClpSolverInterface *solver, Graph *graph);

