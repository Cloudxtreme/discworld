inherit "/std/object";

object worn_by;

void setup() {
  set_name("suit");
  set_short("black suit");
  set_long("A smart looking black suit, looks like it should be "+
           "worn to a wedding.\n");
}


string query_type() {
  return "suit";
}

int set_worn_by(object ob) {
  if (!ob) {
    worn_by->remove_extra_look(this_object());
    worn_by = 0;
    return 1;
  }
  worn_by = this_player();
  worn_by->add_extra_look(this_object());
  return 1;
}

string query_plural() {
  if (worn_by)
    return ::query_plural()+" (worn)";
  return ::query_plural();
}

int query_armour() { return 1; }

string pretty_plural() {
  if (worn_by)
    return ::pretty_plural()+" (worn)";
  return ::pretty_plural();
}

string pretty_short(int dark) {
  if (worn_by)
    return ::short(dark)+" (worn)";
  return ::short(dark);
}

string short(int dark) {
  if (worn_by)
    return ::short(dark)+" (worn)";
  return ::short(dark);
}

string extra_look() {
  return "Is wearing a black smart suit, he must be going to get married.\n";
}

move(object dest, string mess_in, string mess_out) {
  int i;


  i = ::move(dest, mess_in, mess_out);
  if (!i && worn_by && dest != worn_by) {
    worn_by->remove_extra_look(this_object());
    worn_by = 0;
  }
  return i;
}
