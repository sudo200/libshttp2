#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "methods.h"

int main(void) {
  puts(http_method_to_string(http_method_from_string("UNSUBSCRIBE", 11), NULL));

  return 0;
}

