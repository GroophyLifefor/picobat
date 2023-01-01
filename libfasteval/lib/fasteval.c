/*
 * Copyright (C) 1999, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2011,
 * 2012, 2013, 2014 Free Software Foundation, Inc.
 * Copyright (C) 2022 Astie Teddy and pBat developers.
 *
 * This file is part of pBat libfasteval based on libmatheval
 *
 * pBat libfasteval is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * pBat libfasteval is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with pBat libfasteval.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "xmath.h"
#include "fasteval.h"

/* Function used to parse string representing function (this function is
 * generated by parser generator). */
extern int      evaluator_parse();

/* Following variables are needed for parsing (parser is able to
 * communicate with program from which it is used through global variables
 * only). */
_Thread_local struct fasteval_result_t result;	/* String representing function.  */
_Thread_local char *input_string;
_Thread_local _Bool ok;
double (*get_var)(const char *);
double (*set_var)(char *, double);

double fasteval_evaluate(char *expr, double (*get)(const char *),
  double (*set)(char *, double), int *fmode)
{
  /* Copy string representing function and terminate it with newline
	 * (this is necessary because parser expect newline character to
	 * terminate its input). */

    char *exprn = calloc(strlen(expr) + 2, sizeof(char));
    if (!exprn)
        return NAN;

    strcpy(exprn, expr);
    strcat(exprn, "\n");

    result.result = NAN;
    input_string = exprn;

    get_var = get;
    set_var = set;

    evaluator_parse();

    free(exprn);

    *fmode = result.fmode;
    return result.result;
}

int compare_strings(const char **restrict a, const char **restrict b)
{
  return strcmp(*a, *b);
}

double evaluate_function(const char *name, double arg)
{
  /* Symbol table predefined functions names.
   * Must be stored in lexicographical order !
   */
	const char *functions_names[] = {
    "abs", "acos", "acosh", "acot", "acoth", "acsc",
    "acsch", "asec", "asech", "asin", "asinh", "atan",
    "atanh", "cos", "cosh", "cot", "coth", "csc", "csch",
    "delta", "erf", "exp", "log", "nandelta", "sec", "sech",
    "sin", "sinh", "sqrt", "step", "tan", "tanh",
	};

	double (* const functions[]) (double) = {
    fabs, acos, acosh, math_acot, math_acoth, math_acsc,
    math_acsch, math_asec, math_asech, asin, asinh, atan,
    atanh, cos, cosh, math_cot, math_coth, math_csc, math_csch,
    math_delta, erf, exp, log, math_nandelta, math_sec, math_sech,
    sin, sinh, sqrt, math_step, tan, tanh,
  };

  const char **matched_name = bsearch(&name, functions_names,
    sizeof(functions) / sizeof(*functions), sizeof(const char *),
    (int (*)(const void *, const void *))compare_strings
  );

  if (matched_name) {
    size_t i = matched_name - functions_names;

    return functions[i](arg);
  }

  /** TODO: report error */
  return NAN;
}

double lookup_variable(char *name)
{
  /* Symbol table predefined constants names. */
  const char *constants_names[] = {
		"e", "log2e", "log10e", "ln2", "ln10", "pi", "pi_2",
		"pi_4", "1_pi", "2_pi", "2_sqrtpi", "sqrt2", "sqrt1_2"
	};

  /* Symbol table predefined constants values. */
	const double constants[] = {
		2.7182818284590452354, 1.4426950408889634074,
		0.43429448190325182765, 0.69314718055994530942,
		2.30258509299404568402, 3.14159265358979323846,
		1.57079632679489661923, 0.78539816339744830962,
		0.31830988618379067154, 0.63661977236758134308,
		1.12837916709551257390, 1.41421356237309504880,
		0.70710678118654752440
	};

  for (size_t i = 0; i < sizeof(constants) / sizeof(*constants); i++)
    if (!strcmp(name, constants_names[i]))
      return constants[i];

  /** TODO: Report error */
  return NAN;
}
