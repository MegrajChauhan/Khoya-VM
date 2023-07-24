#include "../includes/utils/errors.h"

void print_err(Error *err)
{
    fprintf(stderr, "%s: %s\n", err_code_to_str(err), err->_err_msg);
}

void free_error(Error *err)
{
    free(err);
}
