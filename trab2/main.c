#include <stdio.h>
#include "cria_func.h"

typedef int (*func_ptr) (int x);

int retorna(int x) {
  return x;
}

int main (void) {
  DescParam params[1];
  func_ptr f_retorna;
  unsigned char codigo[500];

  params[0].tipo_val = INT_PAR;
  params[0].orig_val = FIX;
  params[0].valor.v_int = 1;

  cria_func(retorna, params, 1, codigo);
  f_retorna = (func_ptr) codigo;   

  // Chamando a função com um valor fixo
  printf("%d\n", f_retorna(1));

  return 0;
}
