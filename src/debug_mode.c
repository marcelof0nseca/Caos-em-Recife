#include "debug_mode.h"

static bool invencibilidade = false;

void DebugAlternarInvencibilidade(void)
{
    invencibilidade = !invencibilidade;
}

bool DebugInvencibilidadeAtiva(void)
{
    return invencibilidade;
}
