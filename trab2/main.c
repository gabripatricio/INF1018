#include <stdio.h>
#include "cria_func.h"

typedef int (*func_ptr) ();

int mult(int x, int y) {
  return x * y;
}

int main (void) {
  DescParam params[2];
  func_ptr f_mult;
  int i;
  unsigned char codigo[500];

  params[0].tipo_val = INT_PAR; /* a nova função passa para mult um valor inteiro */
  params[0].orig_val = IND;     /* que é o valor corrente da variavel i */
  params[0].valor.v_ptr = &i;

  params[1].tipo_val = INT_PAR; /* o segundo argumento passado para mult é a constante 10 */
  params[1].orig_val = FIX;
  params[1].valor.v_int = 10;

  cria_func (mult, params, 2, codigo);
  f_mult = (func_ptr) codigo;
  
  for (i = 1; i <=10; i++) {
    printf("%d\n", f_mult()); /* a nova função não recebe argumentos */
  }

  //libera_func(f_mult); <== removido
  return 0;
}
