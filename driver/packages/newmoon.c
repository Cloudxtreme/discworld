/* AuroraMUD efuns.
 * Rewritten February 1995, Chrisy
 *
 * Hacked for MudOS v21.5 by LordSutch@NewMoon
 * - Took out a few efuns now in contrib.c
 * - Rewrote var specifications for v21.5 typedefs
 * - Changed to properly use PROT(()) and Px() for non-ANSI compatibility
 *
 * Patched cleanly into MudOS v21.6 by LordSutch@NewMoon
 * Ditto for v21.7b4: LordSutch 951111
 * Added stuff so it would work under AmigaOS: LordSutch 951120
 * Fixed the pop instruction on f_event(): LordSutch 951120
 */

#ifdef LATTICE
#include "/lpc_incl.h"
#include "/file_incl.h"
#else
#include "../lpc_incl.h"
#include "../file_incl.h"
#endif

/* Prototypes... */

void au_event PROT ((svalue_t *, char *, int, svalue_t *));
array_t *au_actions_defined PROT ((object_t *,
                                   object_t *, int));
/* Da efuns.. :) */


#ifdef F_EVENT
void f_event PROT ((void))
{
  int num;

  num = st_num_arg;

  au_event ((sp - num + 1), (sp - num + 2)->u.string, num - 2, (sp - num + 3));

  pop_n_elems(num);
}
#endif

#ifdef F_ACTIONS_DEFINED
void f_actions_defined PROT ((void))
{
  array_t *v;
  object_t *arg1, *arg2;
  int arg3, num;

  num = st_num_arg;
  
  if (st_num_arg && ((sp - st_num_arg + 1)->type == T_OBJECT))
    arg1 = (sp - st_num_arg + 1)->u.ob;
  else
    arg1 = 0;

  if ((st_num_arg > 1) && ((sp - st_num_arg)->type == T_OBJECT))
    arg2 = (sp - st_num_arg)->u.ob;
  else
    arg2 = 0;

  /* only first two arg types caught by parser */
  if (st_num_arg > 2)
    {
      if (sp->type != T_NUMBER)
	bad_arg (3, F_ACTIONS_DEFINED);
      else
	arg3 = sp->u.number;
    }
  else
    {
      arg3 = 0;
    }

  v = au_actions_defined (arg1, arg2, arg3);

  pop_n_elems(num);
  push_refed_array(v);
}
#endif


/* AuroraMUD efunction and various code.
 * Rewritten february 1995, Chrisy
 * What joy.
 */

int au_aliases P1 (char *, str)
{
  static char *key[] =
  {
    "n", "s", "e", "w",
    "ne", "nw", "se", "sw",
    "u", "d",
    NULL,
  };
  static char *val[] =
  {
    "north", "south", "east", "west",
    "northeast", "northwest", "southeast", "southwest",
    "up", "down",
    NULL,
  };
  int i;

  i = 0;

  while (key[i] != NULL)
    {
      if (!strcmp (str, key[i]))
	{
	  strcpy (str, val[i]);
	  return 1;
	}
      i++;
    }
  return 0;
}

#ifdef F_EVENT

/* EVENTS!
 * Okay. This is pretty simple.
 * Calls the function "event_"+event_name in the object specified.
 * If the object is an array, calls it in each.
 * If the object is a room, call it on its inventory.
 *  [Incorrect: actually calls it on all_inventory() of any object but
 *   only if addressed uniquely]
 * Passes all the parameters too.
 */

#define EVENT_PREFIX "event_"

void au_event P4 (svalue_t *, event_ob, char *, event_fun, int, numparam,
		  svalue_t *, event_param)
{
  object_t *ob, *origin;
  char *name;
  int i;

  origin = current_object;

  name = (char *) MALLOC (strlen (event_fun) + strlen (EVENT_PREFIX) + 1);
  push_malloced_string (name);

  strcpy (name, EVENT_PREFIX);
  strcat (name, event_fun);

  if (event_ob->type == T_ARRAY)
    {
      int ind;

      for (ind = 0; ind < event_ob->u.arr->size; ind++)
	{
	  if (event_ob->u.arr->item[ind].type != T_OBJECT ||
	      event_ob->u.arr->item[ind].u.ob->flags &
	      O_DESTRUCTED)
	    continue;

	  push_object (origin);
	  for (i = 0; i < numparam; i++)
	    push_svalue (event_param + i);

	  apply (name, event_ob->u.arr->item[ind].u.ob,
		 numparam + 1, ORIGIN_EFUN);
	}
    }
  else if(event_ob->type == T_OBJECT)
    {
      /* First we call the event on the object itself */

      push_object (origin);
      for (i = 0; i < numparam; i++)
	push_svalue (event_param + i);

      apply (name, event_ob->u.ob, numparam + 1, ORIGIN_EFUN);

      /* And then call it on it's inventory... */

      for (ob = event_ob->u.ob->contains; ob; ob = ob->next_inv)
	{
	  if (ob == origin)
	    continue;

	  if (ob->flags & O_DESTRUCTED)
	    continue;

	  push_object (origin);
	  for (i = 0; i < numparam; i++)
	    push_svalue (event_param + i);

	  apply (name, ob, numparam + 1, ORIGIN_EFUN);
	}
    }
  sp--;
  FREE (name);
}
#endif

#ifdef F_ACTIONS_DEFINED

/* Gives us useful information about the defined actions on and from
 * given objects. From the comment on the old version:
 *
 * Returns the actions defined by "by" on "on" in the format:
 * (assuming all data possible is returned)
 * ({
 *   "command", ({
 *      object_defining_action,
 *      "function called",
 *      short_verb,
 *      priority
 *   })
 * })
 * if "by" is NULL then returns all commands defined.
 * "on"defaults to command_giver.
 * "flags" has the following format:
 *   0000 : no optional data, return array of command names.
 *   1xxx : include the object defining the action.
 *   x1xx : include the name of the function called.
 *   xx1x : include position of star + 1 (0 if not exist).
 *   xxx1 : include the actions priority.
 */

array_t *au_actions_defined P3 (object_t *, by, object_t *, on, int, flags)
{
  /* Number of bits required to store a given number. */
  static char bits[] =
  {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2,
    3, 2, 3, 3, 4
  };

  sentence_t *s;
  array_t *ret;
  int num, ind;

  if (on == NULL)
    on = command_giver;

  num = 0;

  for (s = on->sent; s != NULL; s = s->next)
    if (by == NULL || s->ob == by)
      num++;

  if (!num)
    return allocate_array (0);

  if (flags)
    num *= 2;

  ret = allocate_array (num);

  num = 0;

  for (s = on->sent; s != NULL; s = s->next)
    if (by == NULL || s->ob == by)
      {
	ret->item[num].type = T_STRING;
	ret->item[num].subtype = STRING_MALLOC;
	ret->item[num].u.string = string_copy (s->verb,
					       "actions_defined");
	num++;

	if (!flags)
	  continue;

	ret->item[num].type = T_ARRAY;
	ret->item[num].u.arr = allocate_array ((int) bits[flags]);
	ind = 0;

	if (flags & 0x8)	/* Object defining action */
	  {
	    ret->item[num].u.arr->item[0].type = T_OBJECT;
	    ret->item[num].u.arr->item[0].u.ob = s->ob;
	    add_ref (s->ob, "actions_defined");
	    ind++;
	  }

	if (flags & 0x4)	/* Function name */
	  {
	    ret->item[num].u.arr->item[ind].type = T_STRING;
	    ret->item[num].u.arr->item[ind].subtype =
	      STRING_MALLOC;

            if(s->flags & V_FUNCTION)
            {
              ret->item[num].u.arr->item[ind].u.string =
                string_copy ("<pointer>", "actions_defined");
            }
            else
            {
              ret->item[num].u.arr->item[ind].u.string = 
                string_copy (s->function.s, "actions_defined");
            }

	    ind++;
	  }

	if (flags & 0x2)	/* The position of the *, if one */
	  {
	    ret->item[num].u.arr->item[ind].type = T_NUMBER;
	    ret->item[num].u.arr->item[ind].u.number = s->star;
	    ind++;
	  }

	if (flags & 0x1)	/* Priority */
	  {
	    ret->item[num].u.arr->item[ind].type = T_NUMBER;
	    ret->item[num].u.arr->item[ind].u.number = s->pri;
	    ind++;
	  }

	num++;
      }

  return ret;
}
#endif
