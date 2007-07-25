/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 2002-2005   The R Development Core Team.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, a copy is available at
 *  http://www.r-project.org/Licenses/
 */


#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>
#include "methods.h"

SEXP R_dummy_extern_place()
{
    error(_("calling the C routine used as an initializer for 'externalptr' objects"));
    return R_NilValue; /* -Wall */
}

SEXP R_externalptr_prototype_object()
{
    return R_MakeExternalPtr((void *)R_dummy_extern_place, R_NilValue, 
			     R_NilValue);
}

