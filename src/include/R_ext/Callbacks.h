#ifndef R_CALLBACKS_H
#define R_CALLBACKS_H

/**
  These structures are for C (and R function) top-level task handlers.
  Such routines are called at the end of every (successful) top-level task
  in the regular REPL. 
 */

#include "Rinternals.h"
/**
  The signature of the C routine that a callback must implement.
  expr - the expression for the top-level task that was evaluated.
  value - the result of the top-level task, i.e. evaluating expr.
  succeeded - a logical value indicating whether the task completed propertly.
  visible - a logical value indicating whether the result was printed to the R ``console''/stdout.
  data - user-level data passed to the registration routine.
 */
typedef Rboolean (*R_ToplevelCallback)(SEXP expr, SEXP value, Rboolean succeeded, Rboolean visible, void *);

typedef struct _ToplevelCallback  R_ToplevelCallbackEl;
/** 
 Linked list element for storing the top-level task callbacks.
 */
struct _ToplevelCallback {
    R_ToplevelCallback cb; /* the C routine to call. */
    void *data;            /* the user-level data to pass to the call to cb() */
    void (*finalizer)(void *data); /* Called when the callback is removed. */

    char *name;  /* a name by which to identify this element. */ 

    R_ToplevelCallbackEl *next; /* the next element in the linked list. */
};

#ifdef __cplusplus
extern "C" {
#endif

Rboolean Rf_removeTaskCallbackByIndex(int id);
Rboolean Rf_removeTaskCallbackByName(const char *name);
SEXP R_removeTaskCallback(SEXP which);
R_ToplevelCallbackEl* Rf_addTaskCallback(R_ToplevelCallback cb, void *data, void (*finalizer)(void *), const char *name, int *pos);



/**
 The following definitions are for callbacks to R functions and methods
 related to user-level tables. This is currently implemented in a 
 separate package and these declarations allow the package to 
 interface to the internal R code.

 See http://developer.r-project.org/RObjectTables.pdf.
 */

typedef struct  _R_ObjectTable R_ObjectTable;

/* Do we actually need the exists() since it is never called but R uses get to see if
   the symbol is bound to anything? */
typedef Rboolean (*Rdb_exists)(const char * const name, Rboolean *canCache, R_ObjectTable *);
typedef SEXP     (*Rdb_get)(const char * const name, Rboolean *canCache, R_ObjectTable *);
typedef int      (*Rdb_remove)(const char * const name, R_ObjectTable *);
typedef SEXP     (*Rdb_assign)(const char * const name, SEXP value, R_ObjectTable *);
typedef SEXP     (*Rdb_objects)(R_ObjectTable *);
typedef Rboolean (*Rdb_canCache)(const char * const name, R_ObjectTable *);

typedef void     (*Rdb_onDetach)(R_ObjectTable *);
typedef void     (*Rdb_onAttach)(R_ObjectTable *);

struct  _R_ObjectTable{
  int       type;
  char    **cachedNames;
  Rboolean  active;

  Rdb_exists   exists;
  Rdb_get      get;
  Rdb_remove   remove;
  Rdb_assign   assign;
  Rdb_objects  objects;
  Rdb_canCache canCache;

  Rdb_onDetach onDetach;
  Rdb_onAttach onAttach;
  
  void     *privateData;
};


#ifdef __cplusplus
}
#endif

#endif /* R_CALLBACKS_H */
