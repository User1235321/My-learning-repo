#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include <string.h>
#include <stdlib.h>

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(find_invalid_ids_c);

static long long power10(int n)
{
  long long result = 1;
  int i;
  for (i = 0; i < n; ++i)
    result *= 10;
  return result;
}

PGDLLEXPORT Datum
find_invalid_ids_c(PG_FUNCTION_ARGS)
{
  text * range_text = PG_GETARG_TEXT_PP(0);
  char * range_str = text_to_cstring(range_text);
  char * dash = strchr(range_str, '-');
  if (!dash)
  {
    pfree(range_str);
    ereport(ERROR,
            (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
             errmsg("invalid range format")));
  }
  *dash = '\0';
  long long nb = atoll(range_str);
  long long ne = atoll(dash + 1);
  pfree(range_str);

  StringInfoData result_str;
  initStringInfo(&result_str);
  int first = 1;
  int half_len;

  for (half_len = 1; half_len <= 9; ++half_len)
  {
    long long min_half = (half_len == 1) ? 1 : power10(half_len - 1);
    long long max_half = power10(half_len) - 1;
    long long factor = power10(half_len);
    long long num;

    for (num = min_half; num <= max_half; ++num)
    {
      long long candidate = num * factor + num;
      if (candidate > ne)
        break;
      if (candidate >= nb)
      {
        char buf[32];
        snprintf(buf, sizeof(buf), "%lld", candidate);
        if (first)
          first = 0;
        else
          appendStringInfoChar(&result_str, ',');
        appendStringInfoString(&result_str, buf);
      }
    }
  }

  text * result = cstring_to_text(result_str.data);
  pfree(result_str.data);
  PG_RETURN_TEXT_P(result);
}
