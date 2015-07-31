/*
 * sprintf.c v1.05 for LPMud 3.0.52
 *
 * An implementation of (s)printf() for LPC, with quite a few
 * extensions (note that as no floating point exists, some parameters
 * have slightly different meaning or restrictions to "standard"
 * (s)printf.)  Implemented by Lynscar (Sean A Reith).
 *
 * This version supports the following as modifiers:
 *  " "   pad positive integers with a space.
 *  "+"   pad positive integers with a plus sign.
 *  "-"   left adjusted within field size.
 *        NB: std (s)printf() defaults to right justification, which is
 *            unnatural in the context of a mainly string based language
 *            but has been retained for "compatability" ;)
 *  "|"   centered within field size.
 *  "="   column mode if strings are greater than field size.  this is only
 *        meaningful with strings, all other types ignore
 *        this.  columns are auto-magically word wrapped.
 *  "#"   table mode, print a list of '\n' separated 'words' in a
 *        table within the field size.  only meaningful with strings.
 *   n    specifies the field size, a '*' specifies to use the corresponding
 *        arg as the field size.  if n is prepended with a zero, then is padded
 *        zeros, else it is padded with spaces (or specified pad string).
 *  "."n  presision of n, simple strings truncate after this (if presision is
 *        greater than field size, then field size = presision), tables use
 *        presision to specify the number of columns (if presision not specified
 *        then tables calculate a best fit), all other types ignore this.
 *  ":"n  n specifies the fs _and_ the presision, if n is prepended by a zero
 *        then it is padded with zeros instead of spaces.
 *  "@"   the argument is an array.  the corresponding format_info (minus the
 *        "@") is applyed to each element of the array.
 *  "'X'" The char(s) between the single-quotes are used to pad to field
 *        size (defaults to space) (if both a zero (in front of field
 *        size) and a pad string are specified, the one specified second
 *        overrules).  NOTE:  to include "'" in the pad string, you must
 *        use "\\'" (as the backslash has to be escaped past the
 *        interpreter), similarly, to include "\" requires "\\\\".
 * The following are the possible type specifiers.
 *  "%"   in which case no arguments are interpreted, and a "%" is inserted, and
 *        all modifiers are ignored.
 *  "O"   the argument is an LPC datatype.
 *  "s"   the argument is a string.
 *  "d"   the integer arg is printed in decimal.
 *  "i"   as d.
 *  "c"   the integer arg is to be printed as a character.
 *  "o"   the integer arg is printed in octal.
 *  "x"   the integer arg is printed in hex.
 *  "X"   the integer arg is printed in hex (in capitals).
 */

#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <sys/types.h>

#include "config.h"
#include "lint.h"
#include "lang.tab.h"
#include "stdio.h"
#include "interpret.h"
#include "mapping.h"
#include "object.h"
#include "sent.h"
#include "ignore.h"
#include "exec.h"

/*
 * If this #define is defined then error messages are returned,
 * otherwise error() is called (ie: A "wrongness in the fabric...")
 */
#define RETURN_ERROR_MESSAGES

#if defined(F_SPRINTF) || defined(F_PRINTF)

extern char *xalloc(), *string_copy();
extern void free_svalue PROT((struct svalue *));

extern struct object *current_object;

typedef unsigned int format_info;
/*
 * Format of format_info:
 *   00000000 0000xxxx : argument type:
 *				0000 : type not found yet;
 *				0001 : error type not found;
 *				0010 : percent sign, null argument;
 *				0011 : LPC datatype;
 *				0100 : string;
 *				1000 : integer;
 *				1001 : char;
 *				1010 : octal;
 *				1011 : hex;
 *				1100 : HEX;
 *   00000000 00xx0000 : justification:
 *				00 : right;
 *				01 : centre;
 *				10 : left;
 *   00000000 xx000000 : positive pad char:
 *				00 : none;
 *				01 : ' ';
 *				10 : '+';
 *   0000000x 00000000 : array mode?
 *   000000x0 00000000 : column mode?
 *   00000x00 00000000 : table mode?
 */

#define INFO_T 0xF
#define INFO_T_ERROR 0x1
#define INFO_T_NULL 0x2
#define INFO_T_LPC 0x3
#define INFO_T_STRING 0x4
#define INFO_T_INT 0x8
#define INFO_T_CHAR 0x9
#define INFO_T_OCT 0xA
#define INFO_T_HEX 0xB
#define INFO_T_C_HEX 0xC

#define INFO_J 0x30
#define INFO_J_CENTRE 0x10
#define INFO_J_LEFT 0x20

#define INFO_PP 0xC0
#define INFO_PP_SPACE 0x40
#define INFO_PP_PLUS 0x80

#define INFO_ARRAY 0x100
#define INFO_COLS 0x200
#define INFO_TABLE 0x400

#define BUFF_SIZE 10000

#define ERROR(x) LONGJMP(error_jmp, x)
#define ERR_BUFF_OVERFLOW	0x1	/* buffer overflowed */
#define ERR_TO_FEW_ARGS		0x2	/* more arguments spec'ed than passed */
#define ERR_INVALID_STAR	0x3	/* invalid arg to * */
#define ERR_PRES_EXPECTED	0x4	/* expected presision not found */
#define ERR_INVALID_FORMAT_STR	0x5	/* error in format string */
#define ERR_INCORRECT_ARG_S	0x6	/* invalid arg to %s */
#define ERR_CST_REQUIRES_FS	0x7	/* field size not given for c/t */
#define ERR_BAD_INT_TYPE	0x8	/* bad integer type... */
#define ERR_UNDEFINED_TYPE	0x9	/* undefined type found */
#define ERR_QUOTE_EXPECTED	0xA	/* expected ' not found */
#define ERR_UNEXPECTED_EOS	0xB	/* fs terminated unexpectedly */
#define ERR_NULL_PS		0xC	/* pad string is null */
#define ERR_ARRAY_EXPECTED      0xD     /* Yep!  You guessed it. */

#define ADD_CHAR(x) {\
  buff[bpos] = x;\
  if (startignore) \
    if (buff[bpos++] == '^') \
      inignore = !inignore; \
    else \
      curpos += 2*!inignore; \
  else \
    if (buff[bpos++] == '%') \
      startignore = 1; \
    else \
      curpos += !inignore; \
  if (bpos>BUFF_SIZE) ERROR(ERR_BUFF_OVERFLOW); \
}

#define GET_NEXT_ARG {\
  if (++arg >= argc) ERROR(ERR_TO_FEW_ARGS); \
  carg = (argv+arg);\
}

#define SAVE_CHAR(pointer) {\
  savechars *new;\
  new = (savechars *)XALLOC(sizeof(savechars));\
  new->what = *(pointer);\
  new->where = pointer;\
  new->next = saves;\
  saves = new;\
}

/*
 * list of characters to restore before exiting.
 */
typedef struct SaveChars {
  char what;
  char *where;
  struct SaveChars *next;
} savechars;

typedef struct ColumnSlashTable {
  union CSTData {
    char *col;			/* column data */
    char **tab;			/* table data */
  } d;				/* d == data */
  unsigned short int nocols;	/* number of columns in table *sigh* */
  char *pad;
  unsigned int start;		/* starting cursor position */
  unsigned int size;		/* column/table width */
  int pres;			/* presision */
  format_info info;		/* formatting data */
  struct ColumnSlashTable *next;
} cst;				/* Columns Slash Tables */

static char buff[BUFF_SIZE];	/* buffer for returned string */
unsigned int bpos;		/* position in buff */
unsigned int curpos;		/* cursor position */
unsigned int inignore;          /* are we not counting these characters */
unsigned int startignore;       /* we found the first char... now for next */
jmp_buf error_jmp;              /* LONGJMP() buffer for error catching */

/*
 * Probably should make this a #define...
 */
void stradd(dst, size, add)
  char **dst, *add;
  int *size;
{
  int i;

  if ((i = (strlen(*dst) + strlen(add))) >= *size) {
    *size += i + 1;
    *dst = REALLOC(*dst, *size);
  }
  strcat(*dst, add);
} /* end of stradd() */

void numadd(dst, size, num)
  char **dst;
  int *size, num;
{
  int i,
      num_l, /* length of num as a string */
      nve;   /* true if num negative */

  if (num < 0) { num *= -1; nve=1; } else nve=0;
  for (i=num/10, num_l=nve+1; i; i /= 10, num_l++) ;
  i = strlen(*dst); /* i = length ofconstructed string so far */
  if ((i + num_l) >= *size) {
    *size += i + num_l + 2;
    *dst = REALLOC(*dst, *size);
  }
  (*dst)[i+num_l] = '\0';
  if (nve) (*dst)[i] = '-'; else i--;
  for (num_l-=nve; num_l; num_l--, num /= 10) (*dst)[i+num_l] = (num%10) + '0';
} /* end of num_add() */

/*
 * This is a function purely because stradd() is, to keep same param
 * passing...
 */
void add_indent(dst, size, indent)
  char **dst;
  int *size, indent;
{
  int i;

  i = strlen(*dst);
  if ((i + indent) >= *size) {
    *size += i + indent + 1;
    *dst = REALLOC(*dst, *size);
  }
  for (;indent;indent--) (*dst)[i++] = ' ';
  (*dst)[i] = '\0';
}

/*
 * Converts any LPC datatype into an arbitrary string format
 * and returns a pointer to this string.
 * Scary number of parameters for a recursive function.
 */
void svalue_to_string(obj, str, size, indent, trailing, indent2)
  struct svalue *obj;
  char **str;
  int size, indent, trailing, indent2;
{
  int i;

  if (!indent2)
    add_indent(str, &size, indent);
  switch (obj->type) {
    case T_INVALID:
      stradd(str,&size,"T_INVALID");
      break;
    case T_LVALUE:
      stradd(str, &size, "lvalue: ");
      svalue_to_string(obj->u.lvalue, str, size, indent+2, trailing);
      break;
    case T_NUMBER:
      numadd(str, &size, obj->u.number);
      break;
    case T_STRING:
      stradd(str, &size, "\"");
      stradd(str, &size, obj->u.string);
      stradd(str, &size, "\"");
      break;
    case T_POINTER:
      if (!(obj->u.vec->size)) {
        stradd(str, &size, "({ })");
      } else {
        stradd(str, &size, "({ /* sizeof() == ");
        numadd(str, &size, obj->u.vec->size);
        stradd(str, &size, " */\n");
        for (i=0; i<(obj->u.vec->size)-1; i++)
          svalue_to_string(&(obj->u.vec->item[i]), str, size, indent+2, 1, 0);
        svalue_to_string(&(obj->u.vec->item[i]), str, size, indent+2, 0, 0);
        stradd(str, &size, "\n");
        add_indent(str, &size, indent);
        stradd(str, &size, "})");
      }
      break;
    case T_MAPPING:
      if (!(obj->u.map->table_size)) {
        stradd(str, &size, "([ ])");
      } else {
        stradd(str, &size, "([ /* sizeof() == ");
        numadd(str, &str, obj->u.map->table_size);
        stradd(str, &size, " */\n");
        for (i=0; i < obj->u.map->table_size; i++) {
          struct node *elm;

          for (elm = obj->u.map->table[i]; elm; elm = elm->next) {
            svalue_to_string(&(elm->values[0]), str, size, indent+2, 0, 0);
            stradd(str, &size, " : ");
            svalue_to_string(&(elm->values[1]), str, size, indent+4, 1, 1);
          }
        }
        add_indent(str, &size, indent);
        stradd(str, &size, "])");
      }
      break;
    case T_OBJECT:
    {
      struct svalue *temp;

      stradd(str, &size, obj->u.ob->name);
      push_object(obj->u.ob);
      temp = apply_master_ob("object_name", 1);
      if (temp && (temp->type == T_STRING)) {
        stradd(str, &size, " (\"");
        stradd(str, &size, temp->u.string);
        stradd(str, &size, "\")");
      }
      /*
       * These flags aren't that useful...
       *
      if (obj->u.ob->flags & O_HEART_BEAT) stradd(str,&size," (hb)");
      if (obj->u.ob->flags & O_IS_WIZARD) stradd(str,&size," (wiz)");
      if (obj->u.ob->flags & O_ENABLE_COMMANDS) stradd(str,&size," (enabled)");
      if (obj->u.ob->flags & O_CLONE) stradd(str,&size," (clone)");
      if (obj->u.ob->flags & O_DESTRUCTED) stradd(str,&size," (destructed)");
      if (obj->u.ob->flags & O_SWAPPED) stradd(str,&size," (swapped)");
      if (obj->u.ob->flags & O_ONCE_INTERACTIVE) stradd(str,&size," (x-activ)");
      if (obj->u.ob->flags & O_APPROVED) stradd(str,&size," (ok)");
      if (obj->u.ob->flags & O_RESET_STATE) stradd(str,&size," (reset)");
      if (obj->u.ob->flags & O_WILL_CLEAN_UP) stradd(str,&size," (clean up)");
       */
      break;
    }
    default:
      stradd(str, &size, "!ERROR: GARBAGE SVALUE!");
  } /* end of switch (obj->type) */
  if (trailing) stradd(str, &size, ",\n");
} /* end of svalue_to_string() */

/* The ignore strlen is so that the pading will work with our wonderful
 * ansi colour stuff.  bing onwards.  This was hacked in by Pinkfish
 * so you may hold me responsible if you wish
 */
int ignorestrlen(str)
  char *str;
{
  int len=0, inignore=0, first=0;

  while (*str++) {
    if (first) {
      if (*str == IGNORE_C2)
        inignore = !inignore;
      len += !inignore;
      first = 0;
    }
    if (*str == IGNORE_C1)
      first = 1;
    else
      len += !inignore;
  }
  return len;
}

/*
 * Adds the string "str" to the buff after justifying it within "fs".
 * "trailing" is a flag which is set if trailing justification is to be done.
 * "str" is unmodified.  trailing is, of course, ignored in the case
 * of right justification.
 */
void add_justified(str, pad, fs, finfo, trailing)
  char *str, *pad;
  int fs;
  format_info finfo;
  short int trailing;
{
  int i, len, len2;

  len = strlen(str);
  len2 = ignorestrlen(str);
  switch(finfo & INFO_J) {
    case INFO_J_LEFT:
      for (i=0; i<len; i++) ADD_CHAR(str[i]);
      fs -= len;
      len = strlen(pad);
      if (trailing) for (i=0; fs>0; i++, fs--) {
        if (pad[i%len] == '\\') i++;
        ADD_CHAR(pad[i%len]);
      }
      break;
    case INFO_J_CENTRE: {
      int j, l;

      l = strlen(pad);
      if (!l) {
/* Irk! */
        l = 1;
        pad = " ";
      }
      j = (fs - len2)/2 + (fs - len2)%2;
      for (i=0; i<j; i++) {
        if (pad[i%l] == '\\') { i++; j++; }
        ADD_CHAR(pad[i%l]);
      }
      for (i=0; i<len; i++) ADD_CHAR(str[i]);
      j = (fs - len2)/2;
      if (trailing) for (i=0; i<j; i++) {
        if (pad[i%l] == '\\') { i++; j++; }
        ADD_CHAR(pad[i%l]);
      }
      break;
    }
    default: { /* std (s)printf defaults to right justification */
      int l;

      fs -= len2;
      l = strlen(pad);
      for (i=0; i<fs; i++) {
        if (pad[i%l] == '\\') { i++; fs++; }
        ADD_CHAR(pad[i%l]);
      }
      for (i=0; i<len; i++) ADD_CHAR(str[i]);
    }
  }
} /* end of add_justified() */

/*
 * Adds "column" to the buffer.
 * Returns 0 is column not finished.
 * Returns 1 if column completed.
 * Returns 2 if column completed has a \n at the end.
 */
int add_column(column, trailing)
  cst **column;
  short int trailing;
{
  register unsigned int done, off=0, inadd_off=0, first_ig=0;
  unsigned int save;
#define COL (*column)
#define COL_D (COL->d.col)

  for (done=0;((done-off)<COL->pres) && COL_D[done] && (COL_D[done]!='\n');done++) {
    if (first_ig) {
      if (COL_D[done] == IGNORE_C2) {
        inadd_off = !inadd_off;
        off += 2;
      } else
        done--;
      first_ig = 0;
    } else if (COL_D[done] == IGNORE_C1)
      first_ig = 1;
    else
      off += inadd_off;
  }
  if (COL_D[done] && (COL_D[done]!='\n')) {
    save = done;
    for (; done && (COL_D[done]!=' '); done--);
    /*
     * handle larger than column size words...
     */
    if (!done) done = save;
  }
  save = COL_D[done];
  COL_D[done] = '\0';
  add_justified(COL_D, COL->pad, COL->size, COL->info,
                (trailing || (COL->next)));
  COL_D[done] = save;
  COL_D += done; /* inc'ed below ... */
  /*
   * if this or the next character is a NULL then take this column out
   * of the list.
   */
  if (!(*COL_D) || !(*(++COL_D))) {
    cst *temp;
    int ret;

    if (*(COL_D-1) == '\n') ret = 2; else ret = 1;
    temp = COL->next;
    FREE((char *)COL);
    COL = temp;
    return ret;
  }
  return 0;
} /* end of add_column() */

/*
 * Adds "table" to the buffer.
 * Returns 0 if table not completed.
 * Returns 1 if table completed.
 */
int add_table(table, trailing)
  cst **table;
  short int trailing;
{
  char save;
  register unsigned int done, i;
#define TAB (*table)
#define TAB_D (TAB->d.tab[i])

  for (i=0; i < TAB->nocols && TAB_D; i++) {
    for (done=0;(TAB_D[done])&&(TAB_D[done] != '\n');done++);
    save = TAB_D[done];
    TAB_D[done] = '\0';
    add_justified(TAB_D, TAB->pad, TAB->size, TAB->info, 
                  (trailing || (i < TAB->nocols-1) || (TAB->next)));
    TAB_D[done] = save;
    TAB_D += done; /* inc'ed next line ... */
    if (!(*TAB_D) || !(*(++TAB_D))) TAB_D = 0;
  }
  if (trailing && i < TAB->nocols)
    for (; i < TAB->nocols; i++)
      for (done = 0; done < TAB->size; done++) ADD_CHAR(' ');
  if (!TAB->d.tab[0]) {
    cst *temp;

    temp = TAB->next;
    FREE((char *)TAB);
    TAB = temp;
    return 1;
  }
  return 0;
} /* end of add_table() */

/*
 * THE (s)printf() function.
 * It returns a pointer to it's internal buffer (or a string in the text
 * segment) thus, the string must be copied if it has to survive after
 * this function is called again, or if it's going to be modified (esp.
 * if it risks being free()ed).
 */
char *string_print_formatted(format_str, argc, argv)
  char *format_str;
  int argc;
  struct svalue *argv;
{
  format_info finfo;
  savechars *saves;	/* chars to restore */
  cst *csts;		/* list of columns/tables to be done */
  struct svalue *carg;	/* current arg */
  unsigned int nelemno;	/* next offset into array */
  unsigned int fpos;	/* position in format_str */
  unsigned int arg = 0;	/* current arg number */
  unsigned int fs;	/* field size */
  int pres;		/* presision */
  unsigned int i;
  char *pad;		/* fs pad string */

  if ((i = SETJMP(error_jmp))) { /* error handling */
    char *err;

    switch(i) {
      case ERR_BUFF_OVERFLOW:
        err = "BUFF_SIZE overflowed...";
        break;
      case ERR_TO_FEW_ARGS:
        err = "More arguments specified than passed.";
        break;
      case ERR_INVALID_STAR:
        err = "Incorrect argument type to *.";
        break;
      case ERR_PRES_EXPECTED:
        err = "Expected presision not found.";
        break;
      case ERR_INVALID_FORMAT_STR:
        err = "Error in format string.";
        break;
      case ERR_INCORRECT_ARG_S:
        err = "Incorrect argument to type %s";
        break;
      case ERR_CST_REQUIRES_FS:
        err = "Column/table mode requires a field size.";
        break;
      case ERR_BAD_INT_TYPE:
        err = "!feature - bad integer type!";
        break;
      case ERR_UNDEFINED_TYPE:
        err = "!feature - undefined type!";
        break;
      case ERR_QUOTE_EXPECTED:
        err = "Quote expected in format string.";
        break;
      case ERR_UNEXPECTED_EOS:
        err = "Unexpected end of format string.";
        break;
      case ERR_NULL_PS:
        err = "Null pad string specified.";
        break;
      case ERR_ARRAY_EXPECTED:
        err = "Array expected.";
        break;
      default:
#ifdef RETURN_ERROR_MESSAGES
        debug_message("%s", 
          "ERROR: (s)printf(): !feature - undefined error 0x%X !\n", i);
        if (current_object) {
	     debug_message("program: %s, object: %s line %d\n",
		    current_prog ? current_prog->name : "",
		    current_object->name,
		    get_line_number_if_any());
        }
        sprintf(buff,
          "ERROR: (s)printf(): !feature - undefined error 0x%X !\n", i);
        fprintf(stderr, "%s:%d: %s", current_object->name,
                                     get_line_number_if_any(), buff);
        return buff;
#else
        error("ERROR: (s)printf(): !feature - undefined error 0x%X !\n", i);
#endif /* RETURN_ERROR_MESSAGES */
    }
#ifdef RETURN_ERROR_MESSAGES
    sprintf(buff, "ERROR: (s)printf(): %s in arg %u\n", err, arg);
    fprintf(stderr, "%s:%d: %s", current_object->name,
                                 get_line_number_if_any(), buff);
    debug_message("%s", "ERROR: (s)printf(): %s in arg %u\n", err, arg);
    if (current_object) {
        debug_message("program: %s, object: %s line %d\n",
		    current_prog ? current_prog->name : "",
		    current_object->name,
		    get_line_number_if_any());
    }
    return buff;
#else
    error("ERROR: (s)printf(): %s in arg %d\n", err, arg);
#endif /* RETURN_ERROR_MESSAGES */
  }
  arg = -1;
  bpos = 0;
  curpos = 0;
  inignore = 0;
  startignore = 0;
  csts = 0;
  saves = 0;
  for (fpos=0; 1; fpos++) {
    if ((format_str[fpos] == '\n') || (!format_str[fpos])) {
      int column_stat = 0;

      if (!csts) {
        if (!format_str[fpos]) break;
        ADD_CHAR('\n');
        curpos = 0;
        inignore = 0;
        startignore = 0;
        continue;
      }
      ADD_CHAR('\n');
      curpos = 0;
      inignore = 0;
      startignore = 0;
      while (csts) {
        cst **temp;

        temp = &csts;
        while (*temp) {
          if ((*temp)->info & INFO_COLS) {
            if (*((*temp)->d.col-1) != '\n')
              while (*((*temp)->d.col) == ' ') (*temp)->d.col++;
            for (i=curpos; i<(*temp)->start; i++) ADD_CHAR(' ');
            column_stat = add_column(temp, 0);
            if (!column_stat) temp = &((*temp)->next);
          } else {
            for (i=curpos; i<(*temp)->start; i++) ADD_CHAR(' ');
            if (!add_table(temp, 0)) temp = &((*temp)->next);
          }
        } /* of while (*temp) */
        if (csts || format_str[fpos] == '\n')
          ADD_CHAR('\n');
        inignore = 0;
        startignore = 0;
        curpos = 0;
      } /* of while (csts) */
      if (column_stat == 2) ADD_CHAR('\n');
      if (!format_str[fpos]) break;
      continue;
    }
    if (format_str[fpos] == '%') {
      if (format_str[fpos+1] == '%') {
        ADD_CHAR('%');
        fpos++;
        continue;
      }
      GET_NEXT_ARG;
      fs = 0;
      pres = 0;
      pad = " ";
      finfo = 0;
      for (fpos++; !(finfo & INFO_T); fpos++) {
        if (!format_str[fpos]) {
          finfo |= INFO_T_ERROR;
          break;
        }
        if (((format_str[fpos] >= '0') && (format_str[fpos] <= '9'))
            || (format_str[fpos] == '*')) {
          if (pres == -1) { /* then looking for pres */
            if (format_str[fpos] == '*') {
              if (carg->type != T_NUMBER)
                ERROR(ERR_INVALID_STAR);
              pres = carg->u.number;
              GET_NEXT_ARG;
              continue;
            }
            pres = format_str[fpos] - '0';
            for (fpos++;
                 (format_str[fpos]>='0')&&(format_str[fpos]<='9'); fpos++) {
              pres = pres*10 + format_str[fpos] - '0';
            }
          } else { /* then is fs (and maybe pres) */
            if ((format_str[fpos] == '0') && (((format_str[fpos+1] >= '1')
                && (format_str[fpos+1] <= '9')) || (format_str[fpos+1] == '*')))
              pad = "0";
            else {
              if (format_str[fpos] == '*') {
                if (carg->type != T_NUMBER)
                  ERROR(ERR_INVALID_STAR);
                fs = carg->u.number;
                if (pres == -2) pres = fs; /* colon */
                GET_NEXT_ARG;
                continue;
              }
              fs = format_str[fpos] - '0';
            }
            for (fpos++;
                 (format_str[fpos]>='0')&&(format_str[fpos]<='9'); fpos++) {
              fs = fs*10 + format_str[fpos] - '0';
            }
            if (pres == -2) { /* colon */
              pres = fs;
            }
          }
          fpos--; /* bout to get incremented */
          continue;
        }
        switch (format_str[fpos]) {
          case ' ': finfo |= INFO_PP_SPACE; break;
          case '+': finfo |= INFO_PP_PLUS; break;
          case '-': finfo |= INFO_J_LEFT; break;
          case '|': finfo |= INFO_J_CENTRE; break;
          case '@': finfo |= INFO_ARRAY; break;
          case '=': finfo |= INFO_COLS; break;
          case '#': finfo |= INFO_TABLE; break;
          case '.': pres = -1; break;
          case ':': pres = -2; break;
          case '%': finfo |= INFO_T_NULL; break; /* never reached */
          case 'O': finfo |= INFO_T_LPC; break;
          case 's': finfo |= INFO_T_STRING; break;
          case 'd': finfo |= INFO_T_INT; break;
          case 'i': finfo |= INFO_T_INT; break;
          case 'c': finfo |= INFO_T_CHAR; break;
          case 'o': finfo |= INFO_T_OCT; break;
          case 'x': finfo |= INFO_T_HEX; break;
          case 'X': finfo |= INFO_T_C_HEX; break;
          case '\'':
            pad = &(format_str[++fpos]);
            while (1) {
              if (!format_str[fpos]) ERROR(ERR_UNEXPECTED_EOS);
              if (format_str[fpos] == '\\') { fpos += 2; continue; }
              if (format_str[fpos] == '\'') {
                if (format_str+fpos == pad) ERROR(ERR_NULL_PS);
                SAVE_CHAR(format_str+fpos);
                format_str[fpos] = '\0';
                break;
              }
              fpos++;
            }
            break;
          default: finfo |= INFO_T_ERROR;
        }
      } /* end of for () */
      if (pres < 0)
        ERROR(ERR_PRES_EXPECTED);
      /*
       * now handle the different arg types...
       */
      if (finfo & INFO_ARRAY) {
        if (carg->type != T_POINTER)
           ERROR(ERR_ARRAY_EXPECTED);
        if (carg->u.vec->size == 0) {
          fpos--; /* 'bout to get incremented */
          continue;
        }
        carg = (argv+arg)->u.vec->item;
        nelemno = 1; /* next element number */
      }
      while (1) {
        struct svalue *clean = 0;

        if ((finfo & INFO_T) == INFO_T_LPC) {
          clean = (struct svalue *)XALLOC(sizeof(struct svalue));
          clean->type = T_STRING;
          clean->subtype = STRING_MALLOC;
          clean->u.string = (char *)XALLOC(500);
          clean->u.string[0] = '\0';
          svalue_to_string(carg, &(clean->u.string), 500, 0, 0);
          carg = clean;
          finfo ^= INFO_T_LPC;
          finfo |= INFO_T_STRING;
        }
        if ((finfo & INFO_T) == INFO_T_ERROR) {
          ERROR(ERR_INVALID_FORMAT_STR);
        } else if ((finfo & INFO_T) == INFO_T_NULL) {
          /* never reached... */
          fprintf(stderr, "%s: (s)printf: INFO_T_NULL.... found.\n",
                  current_object->name);
          ADD_CHAR('%');
        } else if ((finfo & INFO_T) == INFO_T_STRING) {
          int slen;

          if (carg->type != T_STRING)
            ERROR(ERR_INCORRECT_ARG_S);
          slen = strlen(carg->u.string);
          if ((finfo & INFO_COLS) || (finfo & INFO_TABLE)) {
            cst **temp;

            if (!fs)
              ERROR(ERR_CST_REQUIRES_FS);

            temp = &csts;
            while (*temp) temp = &((*temp)->next);
            if (finfo & INFO_COLS) {
              *temp = (cst *)XALLOC(sizeof(cst));
              (*temp)->next = 0;
              (*temp)->d.col = carg->u.string;
              (*temp)->pad = pad;
              (*temp)->size = fs;
              (*temp)->pres = (pres) ? pres : fs;
              (*temp)->info = finfo;
              (*temp)->start = curpos;
              if ((add_column(temp, (((format_str[fpos] != '\n')
                    && (format_str[fpos] != '\0')) || ((finfo & INFO_ARRAY)
                      && (nelemno < (argv+arg)->u.vec->size)))) == 2)
                  && !format_str[fpos]) {
                ADD_CHAR('\n');
              }
            } else { /* (finfo & INFO_TABLE) */
#undef max
              unsigned int n, len, max;

#define TABLE carg->u.string
              (*temp) = (cst *)XALLOC(sizeof(cst));
              (*temp)->pad = pad;
              (*temp)->info = finfo;
              (*temp)->start = curpos;
              (*temp)->next = 0;
              max = len = 0;
              n = 1;
              for (i=0; TABLE[i]; i++) {
                if (TABLE[i] == '\n') {
                  if (len > max) max = len;
                  len = 0;
                  if (TABLE[i+1]) n++;
                  continue;
                }
                len++;
              }
              if (pres) {
                (*temp)->size = fs/pres;
              } else {
                if (len > max) max = len; /* the null terminated word */
                pres = fs/(max+2); /* at least two separating spaces */
                if (!pres) pres = 1;
                (*temp)->size = fs/pres;
              }
              len = n/pres; /* length of average column */
              if (n < pres) pres = n;
              if (len*pres < n) len++;
              if (len > 1 && n%pres) pres -= (pres - n%pres)/len;
              (*temp)->d.tab = (char **)XALLOC(pres*sizeof(char *));
              (*temp)->nocols = pres; /* heavy sigh */
              (*temp)->d.tab[0] = TABLE;
              if (pres == 1) goto add_table_now;
              i = 1; /* the next column number */
              n = 0; /* the current "word" number in this column */
              for (fs = 0; TABLE[fs]; fs++) { /* throwing away fs... */
                if (TABLE[fs] == '\n') {
                  if (++n >= len) {
                    SAVE_CHAR(((TABLE)+fs));
                    TABLE[fs] = '\0';
                    (*temp)->d.tab[i++] = TABLE+fs+1;
                    if (i >= pres) goto add_table_now;
                    n = 0;
                  }
                }
              }
add_table_now:
              add_table(temp, (((format_str[fpos] != '\n')
                && (format_str[fpos] != '\0')) || ((finfo & INFO_ARRAY)
                  && (nelemno < (argv+arg)->u.vec->size))));
            }
          } else { /* not column or table */
            if (pres && pres<slen) {
              SAVE_CHAR(((carg->u.string)+pres));
              carg->u.string[pres] = '\0';
              slen = pres;
            }
            if (fs && fs>slen) {
              add_justified(carg->u.string, pad, fs, finfo,
          (((format_str[fpos] != '\n') && (format_str[fpos] != '\0'))
          || ((finfo & INFO_ARRAY) && (nelemno < (argv+arg)->u.vec->size)))
          || carg->u.string[slen-1] != '\n');
            } else {
              for (i=0; i<slen; i++) ADD_CHAR(carg->u.string[i]);
            }
          }
        } else if (finfo & INFO_T_INT) { /* one of the integer types */
          char cheat[4];
          char temp[100];
  
          *cheat = '%';
          i = 1;
          switch (finfo & INFO_PP) {
            case INFO_PP_SPACE: cheat[i++] = ' '; break;
            case INFO_PP_PLUS: cheat[i++] = '+'; break;
          }
          switch (finfo & INFO_T) {
            case INFO_T_INT: cheat[i++] = 'd'; break;
            case INFO_T_CHAR: cheat[i++] = 'c'; break;
            case INFO_T_OCT: cheat[i++] = 'o'; break;
            case INFO_T_HEX: cheat[i++] = 'x'; break;
            case INFO_T_C_HEX: cheat[i++] = 'X'; break;
            default: ERROR(ERR_BAD_INT_TYPE);
          }
          if (carg->type != T_NUMBER) { /* sigh... */
#ifdef RETURN_ERROR_MESSAGES
            sprintf(buff,
              "ERROR: (s)printf(): incorrect argument type to %%%c.\n",
              cheat[i-1]);
            fprintf(stderr, "%s:%d: %s", current_object->name,
                                         get_line_number_if_any(), buff);
            return buff;
#else
            error("ERROR: (s)printf(): incorrect argument type to %%%c.\n",
                  cheat[i-1]);
#endif /* RETURN_ERROR_MESSAGES */
          }
          cheat[i] = '\0';
          sprintf(temp, cheat, carg->u.number);
          {
            int tmpl = strlen(temp);
            if (pres && tmpl > pres) temp[pres] = '\0'; /* well.... */
            if (tmpl < fs)
              add_justified(temp, pad, fs, finfo,
          (((format_str[fpos] != '\n') && (format_str[fpos] != '\0'))
          || ((finfo & INFO_ARRAY) && (nelemno < (argv+arg)->u.vec->size))));
            else
              for (i=0; i<tmpl; i++) ADD_CHAR(temp[i]);
          }
        } else /* type not found */
          ERROR(ERR_UNDEFINED_TYPE);
        if (clean) free_svalue(clean);
        if (!(finfo & INFO_ARRAY)) break;
        if (nelemno >= (argv+arg)->u.vec->size) break;
        carg = (argv+arg)->u.vec->item+nelemno++;
      } /* end of while (1) */
      fpos--; /* bout to get incremented */
      continue;
    }
    ADD_CHAR(format_str[fpos]);
  } /* end of for (fpos=0; 1; fpos++) */
  ADD_CHAR('\0');
  while (saves) {
    savechars *tmp;
    *(saves->where) = saves->what;
    tmp = saves;
    saves = saves->next;
    FREE((char *)tmp);
  }
  return buff;
} /* end of string_print_formatted() */

#endif /* defined(F_SPRINTF) || defined(F_PRINTF) */
