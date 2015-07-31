inherit "std/object";

/*
 * ok format...
 * name ({ your st, everyone st })
 * or (one arg version ie sigh)
 * name ({ your st, their st, everyone st })
 * or (two only arg version.)
 * name ({ your st, everyone st, arg st, their arg, everyone arg })
 * or (one or two arg version)
 * name ({ ({ as above }), ({ as above }) })
 * for cases where the male and female versions are different.
 * works on targets gender. should really be the two arg only version.
 * hmmm going to need some actual processing.
 *
 * 0 == search for living name
 * 1 == search for living name and change thingy
 * 2 == search for change thingy and living name
 * 3 == search for change thingy
 */

mapping soul_data;
string  lastarg, calc_long;

mapping query_soul_data() { return soul_data + ([ ]); }
 
create() {
  ::create();
  set_name("soul");
  set_short(0);
  soul_data =
([ 
  "ack" : ({ 0, ({ "You ack", "$mcname$ ack's" }) }),
  "accuse" : ({ ({ 0, "%s", 2, "%s of %s", 2, "%s %s", 3, "%s" }), ({
      "$ifarg:You accuse $arg:#$$else$The buttler did it!~$", 
      "$mcname$ $ifarg:accuses $lastarg$$else$accuses the butler~$",
      "You accuse $hcname$$ifarg: of $arg:#$~$",
      "$mcname$ accuses you$ifarg: of $lastarg$~$",
      "$mcname$ accuses $hcname$$ifarg: of $lastarg$~$" }) }),
  "apologize" : ({
      ({ 3, "%s", 1, "%s to %s", 1, "%s %s", 2, "%s %s", }), ({
	  "You apologize $arg:profusely,happily,sadly,reservedly,"+
	  "carefully,slowly,perfunctorily,curtly,visciously,"+
	  "sulkily,drunkenly,unhappily,professionally,pompously,"+
	  "wholeheartedly,partially,wearily,fishily,facetiously,"+
	  "unnecessarily,spontaneously,twice,thoroughly,humbly$",
	    "$mcname$ apologizes $lastarg$",
	  "You apologize $arg:profusely,happily,sadly,reservedly,"+
	  "carefully,slowly,perfunctorily,curtly,visciously,"+
	  "sulkily,drunkenly,unhappily,professionally,pompously,"+
	  "wholeheartedly,partially,wearily,fishily,facetiously,"+
	  "unnecessarily,spontaneously,twice,thoroughly,humbly$"+
	  "to $hcname$",
	    "$mcname$ apologizes $lastarg$ to you",
	    "$mcname$ apologizes $lastarg$ to $hcname$" }) }),
  "applaud" : ({
      ({ 0, "%s", 1, "%s %s", 2, "%s %s", 3, "%s" }), ({
	  "You applaud $arg:wholeheartedly,loudly,softly,vaguely,briefly$",
	  "$mcname$ applauds $lastarg$",
	  "You applaud $hcname$ $arg:wholeheartedly,loudly,softly,vaguely,briefly$",
	  "$mcname$ applauds you $lastarg$",
	  "$mcname$ applauds $hcname$ $lastarg$" }) }),
  "agree" : ({ ({ 0, "%s", 0, "with %s", 1, "%s with %s", 1, "%s %s",
                  2, "with %s %s", 3, "%s" }), ({
      "You agree $arg:wholeheartedly,partially,wearily$",
      "$mcname$ agrees $lastarg$",
      "You agree $arg:wholeheartedly,partially,wearily$ with $hcname$",
      "$mcname$ argees $lastarg$ with you",
      "$mcname$ agrees $lastarg$ with $hcname$" }) }),
  "bark" : ({ ({ 3, "%s" }), ({
      "You bark like a $arg:dog,cat,tree,wolf,hunting dog,labrador,"+
            "german shepard,something else that barks,"+
	    "bull terrier,train$",
	"$mcname$ barks like a $lastarg$", }) }),
  "beam" : ({ ({ 3, "%s", 1, "%s at %s", 1, "%s %s", 2, "at %s %s",
             2, "%s %s" }), ({
      "You beam $arg:brightly,admiringly,strangely,hapilly,estaticlly,fishily,"+
           "like a train$",
      "$mcname$ beams $lastarg$",
      "You beam at $hcname$ $arg:brightly,admiringly,strangely,hapilly,"+
           "estaticlly,fishily,like a train$",
      "$mcname$ beams at you $lastarg$",
      "$mcname$ beams at $hcname$ $lastarg$", }) }),
  "beep" : ({ ({ 3, "%s" }), ({
      "You beep $arg:happily,#$",
      "$mcname$ beep $arg:happily,#$" }) }),
  "beg" : ({ ({
      0, "at %s", 0, "%s", 2, "at %s %s", 2, "at %s like a %s",
      1, "like a %s at %s", 1, "%s at %s", 1, "%s %s", 1, "%s %s",
      3, "%s", 3, "like a %s", }), ({
	  "You beg like a $arg:dog,cat,frog,chicken,frisbee,cow,large bear,"+
	  "poodle,chihuahua,train,beggar,noble man,lassie,labrador,german shepard,"+
	  "cannibal,chimpanzee,orang-u-tang,librarian,fish,dolphin,tokzic,"+
	  "banshi,bloodhound,st bernard,virgin mary,husky,wolf,dingo,alsation$",
	  "$mcname$ begs like a $lastarg$",
	  "You beg like a $arg:dog,cat,frog,chicken,frisbee,cow,large bear,"+
	  "poodle,chihuahua,train,beggar,noble man,lassie,labrador,german shepard,"+
	  "cannibal,chimpanzee,orang-u-tang,librarian,fish,dolphin,tokzic,"+
	  "banshi,bloodhound,st bernard,virgin mary,husky,wolf,dingo,alsation,"+
	  "train$ at $hcname$",
	  "$mcname$ begs like a $lastarg$ at you",
	  "$mcname$ begs like a $lastarg$ at $hcname$", }) }),
  "bing" : ({ ({ 0, "at %s", 0, "%s", 1, "%s at %s", 1, "%s %s", 2, "%s %s",
                 2, "at %s %s", 3, "%s" }), ({
      "You bing $arg:happily,sadly,joyfully,sagely,knowingly,quickly,"+
           "loudly,carefully,balefully,excitedly,nervously,brightly,"+
           "uncertainly$",
      "$mcname$ bings $lastarg$",
      "You bing $arg:happily,sadly,joyfully,sagely,knowingly,quickly,"+
           "excitedly,nervously,brightly,uncertainly,"+
           "loudly,carefully,balefully$ at $hcname$",
      "$mcname$ bings $lastarg$ at you",
      "$mcname$ bings $lastarg$ at $hcname$" }) }),
  "bird" : ({ ({ 0, "%s" }), ({
      "You raise your middle finger and give $hcname$ the bird",
      "$mcname$ raises $mposs$ middle finger and gives you the bird",
      "$mcname$ raises $mposs$ middle finger and gives $hcname$ the bird" }) }),
  "bite" : ({ ({ 0, "%s", 2, "%s %s", 1, "%s %s" }), ({
      "You bite yourself",
      "$mcname$ bites $mobj$self",
      "You bite $hcname$$ifarg: $arg:hard,nastily,carefuly,politely,fishily,"+
           "lovingly,refreshingly,happily,like a train,pointlessly,"+
           "objectively,sulkily,gently,brightly,excitedly,uncertainly,"+
           "like a dog,wolfishly,mercilessly,ruthlessly,softly,sweetly,"+
           "profesionally,hungrily,mischeviously,ankles,arm,leg,casualy,"+
           "nose,ear,toe,finger$~$",
      "$mcname$ bites you$ifarg: $lastarg$~$",
      "$mcname$ bites $hcname$$ifarg: $lastarg$~$", }) }),
  "bleed" : ({ ({ 3, "%s" }), ({
      "$ifarg:Your $arg:heart,leg,arm,head,ear,nose,profusely"+
      "$ bleeds$else$You bleed~$",
      "$ifarg:$mcname$'s $lastarg$ bleeds$else$$mcname$ bleeds~$", }) }),
  "blink" : ({ ({ 3, "%s" }), ({
      "You blink$ifarg: $arg:owlishly,slowly,carefully$~$",
      "$mcname$ blinks$ifarg: $lastarg$~$" }) }),
  "blush" : ({ ({ 3, "%s" }), ({
      "You blush $arg:in embarassment,redly,slightly,worriedly$",
      "$mcname$ blushes $lastarg$" }) }),
  "boggle" : ({ ({ 3, "%s", 0, "at %s", 0, "%s" }), ({
      "You boggle at $arg:the concept,#$", "$mcname$ boggles at $lastarg$",
      "You boggle at $hcname$",
      "$mcname$ boggles at you",
      "$mcname$ boggles at $hcname$", }) }),
  "bop" : ({ ({ 0, "%s" }), ({
      "You bop to the beat",
      "$mcname$ bops to the beat",
      "You bop $hcname$ on the head", 
      "$mcname$ bops you on the head",
      "$mcname$ bops $hcname$ on the head" }) }),
  "bounce" : ({ ({ 0, "%s", 1, "%s around %s", 1, "%s %s", 2, "%s %s", 3, "%s" }), ({
      "$ifarg:You bounce around $arg:excitedly,fearfully,happily$"+
      "$else$B O U N C E ! !~$",
      "$mcname$ bounces around$ifarg: $lastarg$~$",
      "You bounce around $hcname$$ifarg: $arg:happily,carefully,wildly,"+
      "excitedly,pinkly,joyfully,once,slowly,suggestively,"+
      "happily,womblely$~$",
      "$mcname$ bounces around you$ifarg: $lastarg$~$",
      "$mcname$ bounces around $hcname$$ifarg: $lastarg$~$", }) }),
  "breakdance" : ({ 0, ({
      "You do a really complicated break-dance manouvre",
      "$mcname$ does a really complicated-break dance manouvre", }) }),
  "breathe" : ({ ({ 3, "%s", }), ({
      "You breathe $arg:quickly,seductively,painfully,nastily,slowly,once,"+
         "heavily,carefully,heatedly$",
      "You breathe $lastarg$", }) }),
  "burp" : ({ ({ 3, "%s" }), ({
      "You burp$ifarg: $arg:disgustingly,faintly,loudly,rudely$~$",
      "$mcname$ burps$ifarg: $lastarg$~$" }) }),
  "bow" : ({ ({ 0, "%s", 0, "to %s", 1, "%s to %s", 1, "%s %s", 3, "%s" }), ({
      "You bow$ifarg: $arg:solemnly,deeply,formally,hastily,slightly," +
         "insolently,clumsily,dextrously,wildly,colourfuly$~$",
      "$mcname$ bows$ifarg: $lastarg$~$",
      "You bow$ifarg: $arg:solemnly,deeply,formally,hastily,slightly,"+
         "insolently,clumsily,dextrously,wildly,colourfuly$~$ to $hcname$",
      "$mcname$ bows$ifarg: $lastarg$~$ to you",
      "$mcname$ bows$ifarg: $lastarg$~$ to $hcname$" }) }),
  "burn" : ({ ({ 0, "%s", 1, "%s %s", 2, "%s %s" }), ({
      "You set fire to yourself",
      "$mcname$ sets fire to $mobj$self",
      "You set fire to $hcname$'s $arg:hair,leg,arm,nose,teeth,frog,carpet,"+
           "net lag,fish,invis,bed,ego,eyes,house,furniture,tongue,keyboard,"+
            "brain,code,ideas,hopes,dreams,cat,personality,desires,beliefs$",
      "$mcname$ set fire to your $lastarg$",
      "$mcname$ set fire to $hcname$'s $lastarg$", }), }),
  "cackle" : ({ ({ 3, "%s" }), ({
      "You cackle $ifarg:$arg:with glee,maniacaly,mischeviously,nastily,"+
           "depairingly,womblely,backwards$$else$with glee~$",
      "$mcname$ $ifarg:cackles $lastarg$$else$"+
           "throws back $mposs$ head and cackles with glee~$" }) }),
  "calm" : ({ ({ 0, "%s", 0, "%s down" }), ({
      "You calm down",
      "$mcname$ calms down",
      "You calm $hcname$ down",
      "$mcname$ calms you down",
      "$mcname$ calms down $hcname$", }) }),
  "caper" : ({ ({ 3, "%s" }), ({
      "You caper around$ifarg: like a $arg:fool,chicken,womble,jester,"+
         "bimbo,blonde,sulam,seductress,eager person,vivacious person,"+
         "madman,madwomen,madperson,frog on the run,"+
         "happy chap,polite gentleperson,woman,no we aren't sexist$"+
      "$else$ madly~$", /* giggle */
      "$mcname$ capers around$ifarg: like a $lastarg$$else$ madly~$", }) }),
  "caress" : ({ ({ 0, "%s", 2, "%s %s", }), ({ 
      "You caress $hcname$$ifarg: $arg:gently,strangely,violently,"+
      "firmly,seductively,suggestively,slightly,sensually,"+
      "reluctantly,politely,lovingly,vigorously,eagerly,vivaciously$~$",
      "$mcname$ caresses you$ifarg: $lastarg$~$",
      "$mcname$ caresses $hcname$$ifarg: $lastarg$~$", }) }),
  "challenge" : ({ ({ 0, "%s", }), ({
      "Make your own challenges to $hcname$",
      "You turn $mcname$ into a fish",
      "$hcname$ turns $mcname$ into a fish", }) }),
  "cheer" : ({ ({ 0, "%s", 0, "at %s", 1, "%s at %s", 1, "%s %s", 2, "at %s %s", 3, "%s" }), ({
      "You cheer $arg:enthusiasticly,excitedly,quietly,calmly,carefully,"+
      "politely,wildly,oddly,unconvincingly,happily,slightly,up$",
      "$mcname$ cheers $lastarg$",
      "You cheer $arg:enthusiasticly at,excitedly at,quietly at,calmly at,"+
      "wildly at,unconvincingly at,carefully at,happily at,"+
      "politely at,oddly at,slightly at,up$ $hcname$",
      "$mcname$ cheers $lastarg$ you",
      "$mcname$ cheers $lastarg$ $hcname$", }) }),
  "choke" : ({ ({ 0, "%s" }), ({
      "You choke",
      "$mcname$ chokes",
      "You choke $hcname$",
      "$mcname$ chokes you",
      "$mcname$ chokes $hcname$$force#breath pain#2$" }) }),
  "chuckle" : ({ ({ 3, "%s" }), ({
      "You chuckle $arg:politely,nastily,demonically$",
      "$mcname$ chuckles $lastarg$" }) }),
  "clap" : ({ ({ 3, "%s" }), ({
      "You clap your hands $arg:briefly,loudly,enthusiasticly,halfheartedly$",
      "$mcname$ claps $mposs$ hands $lastarg$" }) }),
  "comfort" : ({ ({ 0, "%s", 1, "%s %s", 2, "%s %s" }), ({
      "You comfort $hcname$$ifarg: $arg:sincerly,wholeheartly,mischeviously,"+
            "halfheartedly,vaguely,eagerly,completely,lovingly,gently,"+
            "tenderly,sadly,in earnest,with your frog,in the ear,in the leg,"+
            "twice,anxiously,carefully,delicately,like someone else$~$",
      "$mcname$ comforts you$ifarg: $lastarg$~$",
      "$mcname$ comforts $hcname$$ifarg: $lastarg$~$" }) }),
  "confess" : ({ ({ 3, "%s" }), ({
      "I confess $ifarg:$arg:#$$else$to being a witch~$",
      "$mcname$ confess's $ifarg:$lastarg$$else$to being a witch, burn "+
           "$mposs$~$", }), }),
  "congrat" : ({ ({ 0, "%s", 1, "%s %s", 2, "%s %s" }), ({
      "You$ifarg: $arg:heartily,warmly$~$ congratulate everyone",
      "$mcname$$ifarg: $lastarg$~$ congratulates everyone",
      "You$ifarg: $arg:heartily,warmly$~$ congratulate $hcname$",
      "$mcname$$ifarg: $lastarg$~$ congratulates you",
      "$mcname$$ifarg: $lastarg$~$ congratulates $hcname$" }) }),
  "cough" : ({ 0, ({ "You cough", "$mcname$ coughs" }) }),
  "cower" : ({ ({ 3, "%s" }), ({
       "You cower $arg:in a corner,fearfully,slyly,shyly,in shame,"+
             "nastily$",
       "$mcname$ cowers $lastarg$", }), }),
  "cringe" : ({ 0, ({
      "You cringe$ifarg: in $arg:terror,embarassment$~$",
      "$mcname$ cringes$ifarg: in $lastarg$~$" }) }),
  "croak" : ({ 0, ({
      "Ribbit",
      "$mcname$ croaks (ribbit)" }) }),
  "cross" : ({ ({ 3, "%s", 3, "my %s" }), ({
      "You cross your $arg:fingers,legs,eyes$",
      "$mcname$ crosses $mposs$ $lastarg$", }) }),
  "cry" : ({ 0, ({ "Wahhhhhh", "$mcname$ crys loudly" }) }),
  "cuddle" : ({ ({ 0, "%s", 1, "%s %s", 2, "%s %s" }), ({
      "You cuddle $hcname$",
      "$mcname$ cuddles you",
      "$mcname$ cuddles $hcname$" }) }),
  "curtsey" : ({ ({
      0, "%s", 0, "to %s", 0, "before %s", 1, "%s %s",
      2, "%s before %s", 2, "%s to %s",3, "%s" }), ({
	  "You curtesy$ifarg: $arg:gracefully,humbly,quickly,low,slightly$~$",
	  "$mcname$ curtsies$ifarg: $lastarg$~$",
	  "You curtesy$ifarg: $arg:gracefully,humbly,quickly,low,slightly$~$ to $hcname$",
	  "$mcname$ curtsies$ifarg: $lastarg$~$ before you",
	  "$mcname$ curtsies$ifarg: $lastagr$~$ to $hcname$" }) }),
  "dance" : ({ ({ 0, "%s", 0, "with %s" }), ({
      "You do the disco duck.  Feels silly doesn't it?",
      "$mcname$ dances the disco duck",
      "You sweep $hcname$ across the dance floor",
      "$mcname$ sweeps you across the dance floor",
      "$mcname$ sweeps $hcname$ across the dance floor" }) }),
  "daydream" : ({ ({ 3, "%s", }), ({
      "You daydream$ifarg: of $arg:#$~$",
      "$mcname$ daydreams$ifarg: of $lastarg$~$", }) }),
  "drool" : ({ ({ 0, "%s", 0, "at %s" }), ({
      "You start to drool",
      "$mcname$ starts to drool",
      "You drool all over $hcname$",
      "$mcname$ drools all over you",
      "$mcname$ drools all over $hcname$" }) }),
  "duh" : ({ 0, ({
      "You scream \"Duh!\"",
      "$mcname$ screams \"Duh!\"", }) }),
  "eye" : ({ ({ 0, "%s" }), ({
      "You eye $hcname$ over",
      "$mcname$ eyes you over",
      "$mcname$ eyes $hcname$ over", }), }),
  "expect" : ({  ({ 0, "%s", 0, "at %s" }), ({
      "You look at expectantly at $hcname$",
      "$mcname$ looks expectantly at you",
      "$mcname$ looks expectantly at $hcname$" }) }),
  "explode" : ({ 0, ({
      "You explode into thousands of tiny pieces",
      "$mcname$ explodes into thousands of tiny pieces", }) }),
  "faint" : ({ ({ 3, "%s", 0, "%s" }), ({
      "You faint$ifarg: $arg:tiredly,out of boredom$~$",
      "$mcname$ faints$ifarg: $lastarg$~$",
      "You faint on $hcname$",
      "$mcname$ faints on you",
      "$mcname$ faints on $hcname$" }) }),
  "fart" : ({ 0, ({ /* don't like this emote, can you tell?? */ /* yes :) */
      "You fart",
      "$mcname$ farts" }) }),
  "fiddle" : ({ 0, ({
       "You fiddle with something",
       "$mcname$ fiddles with something", }), }),
  "fish" : ({ ({
      0, "%s", 0, "in %s", 1, "for %s in %s pockets",
      1, "for %s in %s", 1, "%s in %s", 2, "%s %s", 3, "%s" }), ({
	  "You fish around in your pockets$ifarg: for a $arg:#$~$",
	  "$mcname$ fishes around in $mposs$ pockets$ifarg: for a $lastarg$~$",
	  "You fish around in $hcname$'s pockets$ifarg: for a $arg:#$~$",
	  "$mcname$ fishes around in your pockets$ifarg: for a $lastarg$~$",
	  "$mcname$ fishes around in $hcname$'s pockets$ifarg: for a $lastarg$~$" }) }),
  "flap" : ({ ({ 3, "%s" }), ({
      "You flap your $arg:ears,arms,wings,eyelashes$",
      "$mcname$ flaps $mposs$ $arg:ears,arms$" }) }),
  "flex" : ({ ({ 3, "%s" }), ({
      "You flex your $arg:muscles,bisceps,triceps,legs,brain,nose,ears$",
      "$mcname$ flexes $mposs$ $lastarg$", }) }),
  "flip" : ({  ({ 3, "%s", 0, "%s", 1, "%s %s", 2, "%s %s" }), ({
      "You flip $arg:head over heels,backwards,forwards,sideways,"+
         "around,carefully,slowly,twice,doggily,awkwardly,disturbingly,"+
         "digustingly,quickly,slowly,gracefully,over the moon,"+
         "around in a really complicated double back roll flip,"+
         "half way around and land on your head, ouch$",
      "$mcname$ flips $lastarg$",
      "You flip $hcname$ $arg:head over heals,off,into the garbage bin,"+
         "visciously,dangerously,spectacularly$",
      "$mcname$ flips you $lastarg$",
      "$mcname$ flips $hcname$ $lastarg$" }) }),
  "flop" : ({ ({ 3, "%s" }), ({
      "You flop about $arg:helplessly,like a fish out of water,"+
          "badly,like zoroaster,carefully,rudely,a bit,once,like a gherkin,wonderfully,"+
          "impressively,suggestively,lazily,happily,pensively$",
      "$mcname$ flops about $lastarg$" }) }),
  "flustered" : ({ 0, ({
      "You look flustered",
      "$mcname$ looks flustered", }) }),
  "fondle" : ({ ({ 0, "%s", 1, "%s %s" }), ({
      "You fondle $hcname$$ifarg:'s $arg:#$~$",
      "$mcname$ fondles$ifarg: with your $lastarg$$else$ you~$",
      "$mcname$ fondles $ifarg:with $mcname$'s $lastarg$$else$$mcname$~$" }) }),
  "foo" : ({ ({ 3, "%s" }), ({
      "You foo $arg:vaguely,carefully,slowly,dangerously$",
      "$mcname$ foos $lastarg$", }), }),
  "forgive" : ({ ({ 0, "%s", }), ({
      "You forgive $hcname$",
      "$mcname$ forgives you",
      "$mcname$ forgives $hcname$", }), }),
  "french" : ({ ({ 0, "%s" }), ({
      "You give $hcname$ a deap and passionate kiss, it seems to last forever..",
      "$mcname$ gives you a deap and passionate kiss, it seems to last forever..",
      "$mcname$ gives $hcname$ a deap and passionate kiss, it seems to last forever.." }) }),
  "froth" : ({ 0, ({
      "You froth at the mouth",
      "$mcname$ froths at the mouth" }) }),
  "frown" : ({ ({ 3, "%s", 3, "in %s" }), ({
      "$ifarg:You frown in $arg:concentration,thought,anger,"+
         "depression,like a cat,happily$$else$Whats the problem?~$",
      "$ifarg:$mcname$ frowns in $lastarg$$else$$mcname$ frowns~$" }) }),
  "fume" : ({ ({ 3, "%s", }), ({
      "You fume $arg:quietly,insultingly,despairingly$",
      "$mcname$ fumes $lastarg$", }) }),
  "gasp" : ({ ({ 3, "%s", 3, "in %s", }), ({
      "You gasp in $arg:astonishment,excitement,terror,fear,fright,pain$",
      "$mcname$ gasps in $lastarg$" }) }),
  "gesticulate" : ({ 0, ({
      "You gesticulate wildly",
      "$mcname$ gesticulates wildly" }) }),
  "gibber" : ({ 0, ({
      "You gibber madly",
      "$mcname$ gibbers madly", }) }),
  "giggle" : ({ ({ 3, "%s" }), ({
      "You giggle $arg:inanely,loudly,stupidly,quietly,idioticly,fiendishly$",
      "$mcname$ giggles $lastarg$" }) }),
  "glare" : ({ ({
      0, "%s", 0, "at %s", 1, "%s at %s", 2, "at %s %s",
      1, "%s %s", 2, "%s %s", 3, "%s" }), ({
	  "You glare $arg:stonily,sternly,hotly,intently,solemnly,idioticly,evanly,"+
	  "evilly$ at the world in general",
	  "$mcname$ glares $lastarg$ around $mposs$",
	  "You glare $arg:stonily,sternly,hotly,intently,solemnly,evilly,evanly,"+
	  "idiotically$ at $hcname$",
	  "$mcname$ glares $lastarg$ at you", 
	  "$mcname$ glares $lastarg$ at $hcname$", }) }),
  "greet" : ({ ({ 0, "%s" }), ({
      "You greet $hcname$ $arg:happily,sadly,with open arms,"+
      "estaticly,carefully,courteously,hysterically,"+
      "reservedly,warmly,coldly,insolently,curtly,"+
      "absentmindedly$",
      "$mcname$ greets you $lastarg$",
      "$mcname$ greets $hcname$ $lastarg$", }) }),
  "grimace" : ({ ({ 3, "%s" }), ({
      "You grimace in $arg:disgust,anger,thought,power,despair$",
      "$mcname$ grimaces in $lastarg$", }), }),
  "grin" : ({ ({ 0, "%s", 0, "at %s", 1, "%s at %s", 2, "at %s %s",
                 1, "%s %s", 2, "%s %s", 3, "%s" }), ({
      "You grin$ifarg: $arg:manically,evilly,wickedly,insanely,mischeviously,"+
         "impishly,cheekily,idiotically,tentatively$~$",
      "$mcname$ grins$ifarg: $lastarg$~$",
      "You grin$ifarg: $arg:manically,evilly,wickedly,insanely,mischeviously,"+
         "impishly,cheekily,idiotically$~$ at $hcname$",
      "$mcname$ grins$ifarg: $lastarg$~$ at you",
      "$mcname$ grins$ifarg: $lastarg$~$ at $hcname$" }) }),
  "grind" : ({ ({ 3, "%s" }), ({
      "You grind your teeth $arg:noisily,seductively,in terror,together,"+
      "annoyingly,suggestively,painfully,angrily$",
      "$mcname$ grinds $mposs$ teeth $lastarg$", }) }),
  "groan" : ({ ({ 3, "%s" }), ({
      "You groan$ifarg: $arg:loudly,resignedly,tiredly$~$",
      "$mcname$ groans$ifarg: $lastarg$~$" }) }),
  "grope" : ({ ({ 0, "%s", 1, "%s %s" }), ({
      "You grope $hcname$$ifarg:'s $arg:#$~$",
      "$mcname$ gropes you$ifarg: $lastarg$~$",
      "$mcname$ gropes $hcname$$ifarg:'s $lastarg$~$" }) }),
  "grovel" : ({ ({ 0, "%s", 0, "at %s", 1, "%s %s", 1, "%s at %s", 3, "%s", 2, "at %s %s", }), ({
      "You grovel$ifarg: $arg:shamelessly,violently,unashamedly$~$",
      "$mcname$ grovels$ifarg: $lastarg$~$",
      "You grovel$ifarg: $arg:shamelessly,violently,unashamedly$~$ at"+
      " $hcname$'s feet",
      "$mcname$ grovels$ifarg: $lastarg$~$ at your feet",
      "$mcname$ grovels$ifarg: $lastarg$~$ at $hcname$'s feet" }) }),
  "growl" : ({ ({
      0, "%s", 0, "at %s", 1, "%s %s", 1, "%s at %s",
      2, "at %s %s", 2, "%s %s", 3, "%s" }), ({
	  "You growl $arg:menacingly,nastily,loudly,playfully$",
	  "$mcname$ growls $lastarg$",
	  "You growl $arg:menacingly,nastily,loudly,playfully$ at $hcname$",
	  "$mcname$ growls $lastarg$ at you",
	  "$mcname$ growls $lastarg$ at $hcname$" }) }),
  "grumble" : ({ ({ 3, "%s", }), ({
      "You grumble $arg:quietly,bitterly,menacingly,halfheartedly,"+
      "seductively,suggestively,loudly,annoyingly,devestatingly,carefully,"+
      "wildly,more or less$",
      "$mcname$ grumbles $lastarg$", }) }),
  "gurgle" : ({ ({ 3, "%s" }), ({
      "You gurgle $arg:noisily,rudely,loudly,mischievously,"+
      "happily,carefully,disgustingly,in suprise,"+
      "like a train,quickly,slowly$",
      "$mcname$ gurgles $lastarg$" }) }),
  "hang" : ({ ({ 3, "%s", }), ({
      "You hang your head in $arg:shame,sorrow,despair,happiness$",
      "$mcname$ hangs $mposs$ head in $lastarg$", }) }),
  "headbutt" : ({ ({ 0, "%s"}), ({
      "You head butt $hcname$", "$mcname$ headbutts you",
      "$mcname$ headbutts $hcname$" }) }),
  "headstand" : ({ ({ 0, "on %s", 0, "%s" }), ({
      "You stand on your head", "$mcname$ stands on $mposs$ head",
      "You stand on $hcname$'s head", "$mcname$ stands on your head",
      "$mcname$ stands on $hcname$'s head" }) }),
  "hiccup" : ({ 0, ({ /* exciting stuff */
      "You hiccup",
      "$mcname$ hiccups" }) }),
  "hold" : ({ ({ 0, "%s", 0, "onto %s", 1, "%s %s", 2, "%s %s" }), ({ /* Hold onto me? */
      "You hold $hcname$$ifarg: $arg:close,tightly,warmly,gently,lovingly,"+
            "back$~$",
      "$mcname$ holds you$ifarg: $lastarg$~$",
      "$mcname$ holds $hcname$$ifarg: $lastarg$~$" }) }),
  "hop" : ({ 0, ({ "You hop around a bit", "$mcname$ hops around a bit" }) }),
  "hug" : ({ ({ 0, "%s", 1, "%s %s", 2, "%s %s" }), ({
      "You hug $hcname$$ifarg: $arg:tightly,warmly,gently,close$~$",
      "$mcname$ hugs you$ifarg: $lastarg$~$",
      "$mcname$ hugs $hcname$$ifarg: $lastarg$~$" }) }),
  "hum" : ({ ({ 3, "%s" }), ({
      "You hum $arg:a merry tune,tunelessly,annoyingly,vaguely,absentmindedly,"+
            "whilst you work,distractedly,pointlessly,silently,loudly,"+
            "rudely$",
      "$mcname$ hums $lastarg$" }), }),
  "idle" : ({ ({ 0, "%s", 3, "%s" }), ({
      "You idle $arg:like pinkfish,like zoroaster,like biggles,like sloths do,"+
           "like a gherkin,like everyone else is,nonchalantly,quickly,"+
           "unbelievably well,without using your hands,upside-down,"+
           "hyperactively,standing still,artfully,around,intensely,like bastian$",
      "$mcname$ idles $lastarg$",
      "You idle like $hcname$",
      "$mcname$ idles like you",
      "$mcname$ idles like $hcname$", }), }),
  "ignore" : ({ ({ 0, "%s" }), ({
      "You ignore the world",
      "$mcname$ ignores the world",
      "You ignore $hcname$",
      "$mcname$ ignores you",
      "$mcname$ ignores $hcname$", }) }),
  "juggle" : ({ ({ 3, "%s" }), ({
      "You juggle some $arg:balls,sticks,battons,cats,dogs,frogs,chickens,"+
         "swords,knives,arms,legs,balls,trees,chain saws,bottles,"+
         "guppies,torches,bings,tree stumps,logs,zoroasters,"+
         "wine glasses,water,people,fire,ladies,men,rhinos$ around",
      "$mcname$ juggles some $lastarg$ around", }) }),
  "jump" : ({ ({ 0, "on %s", 0, "%s", 2, "on %s %s",
                 1, "%s on %s", 1, "%s %s", 2, "%s %s", 3, "%s" }), ({
      "You jump up and down $arg:excitedly,happily,carefully,slowly,"+
         "joyfully,unsteadily,estaticly,worriedly,sideways,"+
         "absentmindedly,like pinkfish,like a prancing pirana$",
      "$mcname$ jumps up and down $lastarg$",
      "You jump up and down on $hcname$$ifarg: $arg:"+
      "excitedly,happily,carefully,slowly,"+
      "joyfully,unsteadily,estaticly,worriedly,sideways,"+
      "absentmindedly,like pinkfish$~$",
      "$mcname$ jumps up and down on you$ifarg: $lastarg$~$",
      "$mcname$ jumps up and down on $hcname$$ifarg: $lastarg$~$" }) }),
  "kick" : ({ ({ 0, "%s", 1, "%s %s" }), ({
      "You kick yourself",
      "$mcname$ kicks $mobj$self",
      "You kick $hcname$$ifarg: $arg:#$~$",
      "$mcname$ kicks you$ifarg: $lastarg$~$",
      "$mcname$ kicks $hcname$$ifarg: $lastarg$~$" }) }),
  "kiss" : ({ ({ 0, "%s", 1, "%s %s", 2, "%s %s" }), ({
      "You kiss $hcname$$ifarg: $arg:passionately,deeply,tenderly,gently,"+
      "lovingly,lengthily,fishily,carefully,demandingly,sensusouly$~$",
      "$mcname$ kisses you$ifarg: $lastarg$~$",
      "$mcname$ kisses $hcname$$ifarg: $lastarg$~$" }) }),
  "knee" : ({ ({ 0, "%s", 1, "%s %s" }), ({
      "You knee $hcname$$ifarg: $arg:#$~$",
      "$mcname$ knees you$ifarg: $lastarg$~$",
      "$mcname$ knees $hcname$$ifarg: $lastarg$~$" }) }),
  "laugh" : ({ ({
      0, "at %s", 0, "%s", 1, "%s %s", 1, "%s at %s",
      2, "at %s %s", 3, "%s"}), ({
	  "$ifarg:You laugh $arg:nastily,nicely,quietly,uncontrollably,politely,"+
	  "happily,manically,estaticly,demonically$"+
	  "$else$You fall down laughing~$",
	  "$ifarg:$mcname$ laughs $lastarg$$else$$mcname$ falls down laughing~$",
	  "You laugh$ifarg: $arg:nastily,nicely,quietly,uncontrollably,politely,"+
	  "happily,manically,estaticly,demonically$~$"+
	  " at $hcname$",
	  "$mcname$ laughs$ifarg: $lastarg$~$ at you",
	  "$mcname$ laughs$ifarg: $lastarg$~$ at $hcname$" }) }),
  "leap" : ({ ({ 3, "%s" }), ({
      "You leap$ifarg: $arg:longingly,excitedly,quickly,slowly,awkwardly,"+
         "politlely,quietly,noisily$~$ into the air",
      "$mcname$ leaps$ifarg: $lastarg$~$ into the air", }) }),
  "leer" : ({ ({ 0, "%s", 0, "at %s" }), ({
       "You leer at yourself",
       "$mcname$ leers at $mobj$self, hmm interesting",
       "You leer at $hcname$",
       "$mcname$ leers at you",
       "$mcname$ leers at $hcname$", }), }),
  "lick" : ({ ({ 0, "%s" }), ({
      "You lick your lips",
      "$mcname$ licks $mposs$ lips",
      "You lick $hcname$",
      "$mcname$ licks you",
      "$mcname$ licks $hcname$" }) }),
  "loom" : ({ ({ 0, "%s", 0, "over %s" }), ({
      "You loom over $hcname$",
      "$mcname$ looms over you",
      "$mcname$ looms over $hcname$", }) }),
  "love" : ({ ({ 0, "%s" }), ({
      "You whisper sweet nothings into $hcname$'s ear",
      "$mcname$ whispers sweet nothings in your ear",
      "$mcname$ whispers to $hcname$" }) }),
  "lower" : ({ 0, ({
      "You lower your eyebrows",
      "$mcname$ lowers $mposs$ eyebrows", }) }),
  "massage" : ({ ({ 0, "%s", 2, "%s %s", }), ({
      "You give $hcname$ a$ifarg: $arg:good,bad,ugly,long,"+
         "refreshing,pink,full body,seductive,sensuous,strange,"+
         "painful,deep,meaningful,ergonomic,economic,"+
         "painless,memorable,different,unwanted,unwarranted,"+
         "unprovoked,unknown,sneaky,unnoticed,unwieldly,back,"+
         "professional,unprofessional,happy,sad,trained,untrained,"+
         "train,virgin mary,compact,quick,foot,leg,arm,head,teeth,"+
         "ear,neck,shoulder,nose$~$ massage",
      "$mcname$ gives you a$ifarg: $lastarg$~$ massage",
      "$mcname$ gives $hcname$ a$ifarg: $lastarg$~$ massage", }) }),
  "moan" : ({ ({ 3, "%s" }), ({
      "You moan$ifarg: $arg:gently,deeply,sensually,agonisingly,in terror$~$",
      "$mcname$ moans$ifarg: $lastarg$~$" }) }),
  "moon" : ({ 0, ({
      "You drop your pants and moon $hcname$",
      "$mcname$ drops $mposs$ pants and moons you",
      "$mcname$ drops $mposs$ pants and moons $hcname$", }), }),
  "mosh" : ({ 0, ({
      "You mosh $arg:violently,angrily,carefully,thoughtfully,"+
          "wonderingly,vigously,insanely,loudly$ around the room",
      "$mcname$ moshes $lastarg$ around the room", }) }),
  "mutter" : ({ ({ 3, "%s" }), ({
      "You $ifarg:mutter $arg:#$ under your breath$else$mutter under "+
          "your breath~$",
      "$mcname$ mutters $ifarg:$arg:#$ ~$$mposs$ breath", }) }),
  "nestle" : ({ ({ 0, "%s", }), ({
      "You nestle up to $hcname$ $arg:affectionately,lovingly,curiously$",
      "$mcname$ nestle up to you $lastarg$",
      "$mcname$ nestle up to $hcname$ $lastarg$", }) }),
  "nibble" : ({ ({ 0, "%s", 1, "%s %s", 2, "%s %s" }), ({
      "You nibble on $hcname$'s $arg:ear,finger,lip,arm,leg,toe,nose$",
      "$mcname$ nibbles on your $lastarg$",
      "$mcname$ nibbles on $hcname$'s $lastarg$" }) }),
  "nod" : ({ ({
      0, "%s", 1, "%s at %s", 1, "%s %s", 2, "at %s %s",
      2, "%s %s", 3, "%s" }), ({
	  "You nod $arg:solemnly,happily,brightly,curtly,patiently,slowly,"+
	  "knowingly,vigorously,in disagreement,tentatively$",
	  "$mcname$ nods $lastarg$",
	  "You nod $arg:solemnly,happily,brightly,curtly,patiently,slowly,"+
	  "knowingly,vigorously$"+
	  " at $hcname$",
	  "$mcname$ nods $lastarg$ at $hcname$",
	    "$mcname$ nods $lastarg$ at $hcname$", }) }),
  "noise" : ({ ({ 3, "%s" }), ({
      "$ifarg:You make a $arg:bungle,flurgle,zongle-wop,wop,whirr,burble,frog,"+
         "moo,train,hairy,twisted,wap,pop,schwing,gong,koala,"+
         "rat,mouse,chicken,womble,guppy,cow,rabbit,kangaroo,gumby$ noise"+
      "$else$You make a funny noise~$",
      "$ifarg:$mcname$ makes a $lastarg$ noise$else$$mcname$ makes a funny noise~$" }) }),
  "nudge" : ({ ({ 0, "%s", }), ({
      "You nudge $hcname$",
      "$mcname$ nudges you",
      "$mcname$ nudges $hcname$", }) }),
  "nuzzle" : ({ ({ 0, "%s" }), ({
      "You nuzzle $hcname$ affectionately",
      "$mcname$ nuzzles you affectionately",
      "$mcname$ nuzzles $hcname$ affectionately", }), }),
  "raise" : ({ 0, ({ "You raise your eyebrows", "$mcname$ raises $mposs$ eyebrows" }) }),
  "razz" : ({ ({ 0, "%s" }), ({
      "You stick your thumb on your nose and razz $hcname$",
      "$mcname$ sticks $mposs$ thumb on $mposs$ nose and razz's you",
      "$mcname$ sticks $mposs$ thumb on $mposs$ nose and razz's $haname$" }) }),
  "rub" : ({ ({ 0, "%s", 3, "%s" }), ({
      "You rub your $arg:hands together,eyes,tummy,nose,leg,arm,eyebrows,head$",
      "$mcname$ rubs $mposs$ $lastarg$",
      "You rub $hcname$ up the wrong way",
      "$mcname$ rubs you up the wrong way",
      "$mcname$ rubs $hcname$ up the wrong way", }) }),
  "panic" : ({ 0, ({
      "You panic",
      "$mcname$ panics", }) }),
  "pat" : ({ ({ 0, "%s", 2, "%s on the %s", 2, "%s on %s", 1, "%s %s", 2, "%s %s" }), ({
      "You pat $hcname$ on the $arg:head,back,leg,arm,nose,bottom$",
      "$mcname$ pats you on the $lastarg$",
      "$mcname$ pats $hcname$ on the $lastarg$" }) }),
  "pet" : ({ ({ 0, "%s" }), ({
      "You pet Goldie",
      "$mcname$ pets Goldie",
      "You pet $hcname$",
      "$mcname$ pets you",
     /* What???  Zoroaster    */
      "$mcname$ pets $hcname$", }) }),
  "peer" : ({ ({
      0, "%s", 0, "at %s", 1, "%s at %s", 1, "%s %s", 2, "at %s %s",
      2, "%s %s", 3, "%s" }), ({
	  "You peer$ifarg: $arg:suspiciously,quizically,"+
	  "anoxiously,stupidly,warningly,expectantly,happily,sadly,desperately,"+
	  "worriedly,strangely,uncertainly,lopsidedly,lazily,rudely,disappointedly,"+
	  "fishily,fearfully,like a train,curiously,tentatively,"+
	  "warily,nervously,carefully,warningly,owlishly,dazedly$~$ around the room",
	  "$mcname$ peers$ifarg: $lastarg$ around the room~$",
	  "You peer$ifarg: $arg:suspiciously,quizically,warilly,nervously,"+
	  "anoxiously,stupidly,warninly,expectantly,happily,sadly,desperately,"+
	  "woriedly,strangely,uncertainly,lopsidedly,lazily,rudely,disapointedly,"+
	  "fishily,fearfully,like a train,curiously,"+
	  "carefully,warningly,intently,owlishly,dazedly$~$ at $hcname$",
	  "$mcname$ peers$ifarg: $lastarg$~$ at you",
	  "$mcname$ peers$ifarg: $lastarg$~$ at $hcname$", }) }),
  "pinch" : ({ ({ 0, "%s", 2, "%s on %s", 2, "%s on the %s", 1, "%s %s", 2, "%s %s" }),({
      "You pinch yourself",
      "$mcname$ pinches $mobj$self",
      "You pinch $hcname$$ifarg: on the $arg:bottom,leg,arm,frog,cheek$~$",
      "$mcname$ pinches you$ifarg: on the $lastarg$~$",
      "$mcname$ pinches $hcname$$ifarg: on the $lastarg$~$",}) }),
  "ping" : ({ ({ 0, "%s" }), ({
      "You ping $hcname$", "$mcname$ pings you$force#ack#2$",
      "$mcname$ pings $hcname$" })  }),
  "plead" : ({ ({ 3, "%s", }), ({
      "$mcname$ pleads for $arg:mercy,frogs,lots of money,a Decstation 5000,"+
      "this stupid soul to go away,an X windows terminal,insanity,"+
      "creators,bil,pinkfish's sanity$",
      "$mcname$ pleads for $lastarg$", }) }),
  "point" : ({ ({ 0, "%s", 0, "at %s", 1, "%s at %s", 2, "at %s %s" }), ({
      "You point$ifarg: $arg:accusingly,straight$~$ at $hcname$",
      "$mcname$ points$ifarg: $lastarg$~$ at you",
      "$mcname$ points$ifarg: $lastarg$~$ at $hcname$" }) }),
  "poke" : ({ ({ 0, "%s", 2, "%s in the %s", 2, "%s in %s", 2, "%s %s", 1, "%s %s" }), ({
      "You poke $hcname$$ifarg: in the $arg:eyes,ribs,legs,nose,ear,"+
             "stomach,#$~$",
      "$mcname$ pokes you$ifarg: in the $lastarg$~$",
      "$mcname$ pokes $hcname$$ifarg: in the $lastarg$~$"}) }),
  "ponder" : ({ 0, ({
      "You ponder the situation",
      "$mcname$ ponders the situation" }) }),
  "pout" : ({ ({ 3, "%s" }), ({
      "You pout$ifarg: $arg:childishly,silently,resignedly$~$",
      "$mcname$ pouts$ifarg: $lastarg$~$" }) }),
  "puke" : ({ ({ 0, "on %s", 0, "%s", 0, "at %s"  }), ({
      "You brighten up the place", "$mcname$ does a technicolour yawn",
      "You puke on $hcname$", "$mcname$ pukes on you",
      "$mcname$ pukes on $hcname$" }) }),
  "punch" : ({ ({ 0, "%s" }), ({
      "You punch yourself",
      "$mcname$ punches $mobj$self",
      "You punch $hcname$",
      "$mcname$ punches You",
      "$mcname$ punches $hcname$", }) }),
  "purr" : ({ ({ 3, "%s" }), ({
      "You purr $arg:contentedly,loudly,happily,sensuously$",
      "$mcname$ purrs $lastarg$" }) }),
  "push" : ({ ({ 0, "%s", }), ({
      "You push yourself over",
      "$mcname$ pushes $mobj$self over",
      "You push $hcname$ over",
      "$mcname$ pushes you over",
      "$mcname$ pushes $hcname$ over", }) }),
  "puzzle" : ({ ({ 3, "%s" }), ({
      "$ifarg:You puzzle over $arg:#$$else$You look puzzled~$",
      "$mcname$ $ifarg: puzzles over $lastarg$$else$looks puzzled~$", }) }),
  "quirk" : ({ 0, ({
      "You quirk one eyebrow",
      "$mcname$ quirks one eyebrow" }) }),
  "quivers" : ({ 0, ({
      "You quiver",
      "$mcname$ quivers" }), }),
  "roll" : ({ ({ 3, "%s", 0, "%s" }), ({
      "You roll $arg:your eyes,around on the floor laughing,"+
             "over and play dead,up into a ball,your own cigarette,"+
             "sideways,backwards,profesionaly,like a spy through an "+
             "imaginary door,fearfully,tightly,slowly$",
      "$mcname$ rolls $lastarg$",
      "You roll $ifarg:$arg:quickly,slowly,painfully,excitededly,lovingly,"+
             "carefully$ ~$over $hcname$",
      "$mcname$ rolls $ifarg:$lastarg$ ~$over you",
      "$mcname$ rolls $ifarg:$lastarg$ ~$over $hcname$" }) }),
  "ruffle" : ({ ({ 0, "%s", 2, "%s %s", 1, "%s %s" }), ({
      "You ruffle $hcname$'s hair$ifarg: $arg:playfully,affectionately$~$",
      "$mcname$ ruffles your hair$ifarg: $lastarg$~$",
      "$mcname$ ruffles $hcname$'s hair$ifarg: $lastarg$~$" }) }),
  "scratch" : ({ 0, ({
      "You scratch your head",
      "$mcname$ scratches $mposs$ head",
      "$ifarg:You $arg:visciously,slightly,wombely$ scratch $hcname$'s "+
           "head$else$You scratch $hcname$~$",
      "$ifarg:$mcname$ $lastarg$ scratches your head$else$"+
           "$mcname$ $lastarg$ scratches you~$"+
      "$hcname$ $ifarg:$lastarg$ ~$scratches $hcname$$ifarg:$else$'s head~$"
           }) }),
  "scream" : ({ ({ 3, "%s", 3, "in %s" }), ({
      "$ifarg:You scream in $arg:terror,fright,fear,pain,surprise,frustration,"+
      "despair,anger$$else$Arrggghhhhh!!!!~$",
      "$mcname$ screams$ifarg: in $lastarg$$else$ loudly~$" }) }),
  "shake" : ({ ({ 0, "%s", 1, "%s with %s", 1, "%s %s", 1, "%s %s", 3, "%s",
                  1, "%s at %s", 3, "my %s", }), ({
      "You shake your head$ifarg: $arg:sorrowfully,carefully,slowly,"+
          "once,quickly,curtly,pinkly,vigously,completely,fishily,"+
          "tentatively,in agreement,in disbelief$~$",
      "$mcname$ shakes $mposs$ head$ifarg: $lastarg$~$",
      "You shake$ifarg: $arg:hands with,fins with,fingers with,legs with,"+
             "pinkys with,tentacles with,your fist at$~$ $hcname$",
      "$mcname$ shakes$ifarg: $lastarg$~$ you",
      "$mcname$ shakes$ifarg: $lastarg$~$ $hcname$" }) }),
  "shiver" : ({ ({ 3, "with %s", 3, "in %s" }), ({
      "You shiver$ifarg: $arg:fear,with cold,slightly,#$~$",
      "$mcname$ shivers$ifarg: $lastarg$~$" }) }),
  "shrug" : ({ 0, ({ "You shrug", "$mcname$ shrugs", }) }),
  "shudder" : ({ ({ 3, "%s" }), ({
      "You shudder $ifarg:in $arg:fear,revulsion,disapointment,ecstasy"+
      "$$else$from horror~$",
      "$mcname$ shudders $ifarg:in $lastarg$$else$from horror~$", }) }),
  "sigh" : ({ ({ 3, "%s" }), ({
      "You sigh$ifarg: $arg:deeply,silently,desperately,tiredly,sadly,slowly,"+
        "deamily,happily,"+
        "heavily,suggestively,sarcasticly,in relief,with relief$~$",
      "$mcname$ sighs$ifarg: $lastarg$~$" }) }),
  "sing" : ({ ({ 0, "%s", 3, "%s" }), ({
      "You sing $arg:in appreciation of life,#$",
      "$mcname$ $ifarg:$lastarg$$else$bursts out into song~$",
      "You seranade $hcname$",
      "$mcname$ seranades you",
      "$mcname$ seranades $hcname$" }) }),
  "slap" : ({ ({ 0, "%s" }), ({
      "You slap your forehead",
      "$mcname$ slaps his forehead",
      "You slap $hcname$", "$mcname$ slaps you",
      "$mcname$ slaps $hcname$" }) }),
  "sleep" : ({ ({ 0, "%s" }), ({
      "You go to sleep",
      "$mcname$ goes to sleep",
      "You go to sleep in $hcname$'s arms",
      "$mcname$ go to sleep in your arms",
      "$mcname$ goes to sleep in $hcname$'s arms", }) }),
  "smirk" : ({ 0, ({ "You smirk", "$mcname$ smirks" }) }),
  "smile" : ({ ({
      0, "%s", 0, "at %s", 1, "%s at %s", 1, "%s %s", 2, "at %s %s",
      2, "%s %s", 3, "%s", }), ({
	  "You smile$ifarg: $arg:happily,sadly,nervously,wryly,broadly,crookedly,"+
	  "stupidly,idiotically,condesendingly,ironically,patiently,brightly,"+
	  "slightly,nastily,excitedly,strangely,with forced patience,plasticly,"+
	  "carefully,sadisticly,lopsidedly,fishily,catishly,faintly,"+
	  "like a train,angily,paranoicaly,innocently,frogily,slyly,"+
	  "weakly,humbly,paradoxicaly,sarcasticly,vicously,garishly,"+
          "sweetly,innocently,lovingly,dreamily,radiantly,enthusiasticly,"+
	  "insolently,quirkily,completely,unconsciously,drunkenly,"+
          "insanely,cutely,maliciously,"+
	  "absentmindedly,curtly,disturbingly,unbeliveably,quietly,"+
	  "loudly,differently,winningly,in triumph,seductively,"+
          "tightly,gosh there are lots of aruments,softly,"+
          "lividly,demonicaly,"+
	  "manically,warily,sardonically,lazily,serenely,disappointedly$~$",
	  "$mcname$ smiles$ifarg: $lastarg$~$",
	  "You smile$ifarg: $arg:happily,sadly,nervously,wryly,broadly,crookedly,"+
	  "stupidly,idiotically,condesendingly,ironically,patiently,brightly,"+
	  "slightly,nastily,excitedly,strangely,with forced patience,plasticly,"+
	  "manically,carefully,sadisticly,lopsidedly,fishily,catishly,faintly,"+
	  "weakly,humbly,paradoxicaly,sarcasticly,vicously,garishly,"+
	  "insolently,quirkily,completely,unconsciously,drunkenly,"+
	  "absentmindedly,curtly,disturbingly,unbeliveably,quietly,"+
	  "loudly,differently,winningly,in triumph,seductively,"+
          "sweetly,innocently,lovingly,dreamily,radiantly,enthusiasticly,"+
	  "like a train,angrily,paranoicaly,innocently,frogily,slyly,"+
	  "macabrely,ruefully,tentatively,like a dog,"+
          "tightly,gosh there are lots of aruments,softly,"+
          "insanely,cutely,maliciously,"+
          "lividly,demonicaly,"+
	  "sardonically,warily,lazily,serenely,disappointedly$~$ at $hcname$",
	  "$mcname$ smiles$ifarg: $lastarg$~$ at you",
	  "$mcname$ smiles$ifarg: $lastarg$~$ at $hcname$" }) }),
  "snap" : ({ ({ 0, "%s", 1, "%s at %s", 1, "my %s at %s", 3, "%s" }), ({
      "You snap your $arg:fingers,leg,arm in a quick salute$",
      "$mcname$ snaps $mposs$ $lastarg$",
      "You snap your $arg:fingers,leg,arm in a quick salute$ at $hcname$",
      "$mcname$ snaps $mposs$ $lastarg$ at you",
      "$mcname$ snaps $mposs$ $lastarg$ at $hcname$" }) }),
  "snarl" : ({ ({ 0, "%s", 1, "%s at %s", 1, "%s %s", 2, "at %s %s" }), ({
      "You snarl $arg:visiously,nastily,demonicaly,happily,sadly,"+
         "unexpectedly,suddenly$ at $hcname$",
      "$mcname$ snarls $lastarg$ at you",
      "$mcname$ snarls $lastarg$ at $hcname$", }) }),
  "sneer" : ({ ({ 0, "%s" }), ({
      "You sneer at $hcname$",
      "$mcname$ sneers at you",
      "$mcname$ sneers at $hcname$", }) }),
  "sneeze" : ({ ({ 0, "at %s", 2, "%s at %s", 3, "%s" }), ({
      "You sneeze $arg:violently,loudly,silently,quietly,suddenly,unexpectedly$",
      "$mcname$ sneezes $lastarg$",
      "You sneeze $arg:violently,loudly,silently,quietly,suddenly,unexpectedly$"+
      " at $hcname$",
      "$mcname$ sneezes $lastarg$ at you",
      "$mcname$ sneezes $lastarg$ at $hcname$" }) }),
  "snicker" : ({ ({ 3, "%s" }), ({
      "You snicker$ifarg: $arg:mischeviously,nastily,a bit,like skippy$~$",
      "$mcname$ snickers$ifarg: $lastarg$~$" }) }),
  "sniff" : ({ ({ 3, "%s" }), ({
      "You sniff$ifarg: $arg:dejectedly,noisily,silently,loudly,in disdain$~$",
      "$mcname$ sniffs$ifarg: $lastarg$~$" }) }),
  "snigger" : ({ 0, ({
      "You snigger",
      "$mcname$ sniggers", }) }),
  "snore" : ({ 0, ({
      "You snore loudly",
      "$mcname$ snores loudly" }) }),
  "snort" : ({ 0, ({
      "You snort",
      "$mcname$ snorts", }) }),
  "snuggle" : ({ ({ 0, "%s", 1, "%s %s", 2, "%s %s" }), ({
      "You snuggle up to $hcname$ $arg:affectionately,warmly,gently$",
      "$mcname$ snuggles up to you $lastarg$",
      "$mcname$ snuggles up to $hcname$ $lastarg$" }) }),
  "sob" : ({ ({ 0, "%s", 3, "%s",  0, "on %s" }), ({
      "You sob $arg:quietly,loudly,despairingly,forlornly,happily,"+
               "completely,unexpectedly,silently,cheekily$",
      "$mcname$ sobs $lastarg$",
      "You sob on $hcname$'s shoulder",
      "$mcname$ sobs on your shoulder",
      "$mcname$ sobs on $hcname$'s shoulder", }) }),
  "sorry" : ({ ({ 0, "%s", 1, "%s %s", 2, "%s %s", }), ({
      "You appologise$ifarg: $arg:profusely,reservedly,profunctily,"+
      "carefully,insultingly,insolently$~$ to $hcname$",
      "$mcname$ appologises$ifarg: $lastarg$~$ to you",
      "$mcname$ appologises$ifarg: $lastarg$~$ to $hcname$", }) }),
  "spank" : ({ ({ 0, "%s", 2, "%s %s" }), ({
      "You spank $hcname$$ifarg: $arg:seductively,carefully,gently,"+
      "uncontrollably,politely,sensuously,selectively,unexpectedly,"+
      "with an evil smirk,painfully,sneakily,unprovoked,professionaly,vigorously,"+
      "reflexively,with a cat,like a train,like a virgin mary,"+
      "with a virgin mary$~$",
      "$mcname$ spanks you$ifarg: $lastarg$~$",
      "$mcname$ spanks $hcname$$ifarg: $lastarg$~$", }) }),
  "spit" : ({ ({ 0, "%s", 0, "at %s" }), ({
  "You spit.",
  "$mcname$ spits.",
  "You spit in $hcname$'s face",
  "$mcname$ spits in your face",
  "$mcname$ spits in $hcname$'s face" }) }),
  "spock" : ({ 0, ({
      "You raise one eyebrow",
      "$mcname$ raises one eyebrow", }) }),
  "squeeze" : ({ ({ 0, "%s", 1, "%s %s", 2, "%s %s" }), ({
      "You squeeze $hcname$ $arg:fondly,gently,tightly$",
      "$mcname$ squeezes you $lastarg$",
      "$mcname$ squeezes $hcname$ $lastarg$" }) }),
  "stagger" : ({ ({ 3, "%s" }), ({
      "You stagger around$ifarg: $arg:#$~$",
      "$mcname$ staggers around$ifarg: $lastarg$~$" }) }),
  "stamp" : ({ ({ 3, "%s", }), ({
      "You stamp your foot$ifarg: $arg:angrily,impatiently,warningly,"+
      "fighteningly$~$",
      "$mcname$ stamps $mposs$ foot$ifarg: $lastarg$~$" }) }),
  "stand" : ({ ({ 3, "%s", }), ({
  "You stand $arg:in uffish thought,about,patiently$",
  "$mcname$ stands $lastarg$" }) }),
  "stare" : ({ ({ 0, "%s", 1, "%s %s", 1, "%s at %s", 3, "%s" }), ({
      "You stare$ifarg: $arg:happily,sadly,woriedly,lazily,rudely,dazedly,"+
         "hungrily,macabrely,absent-mindedly,fanaticaly,lovingly,"+
         "respectfully,womblely,frogily,desperately,sternly,"+
         "tentatively,politely,listlessly,"+
         "dreamily,intently,mindlessly,expectantly,longingly$~$ into space",
      "$mcname$ stares$ifarg: $lastarg$~$ into space",
      "You stare$ifarg: $arg:happily,sadly,woriedly,lazily,rudely,dazedly,"+
         "hungrily,macabrely,absent-mindedly,fanaticaly,lovingly,"+
         "respectfully,womblely,frogily,desperately,sternly,"+
         "tentatively,politely,listlessly,"+
         "dreamily,intently,mindlessly,romantically,expectantly,longingly,"+
         "continuously,annoyingly$~$ at $hcname$",
      "$mcname$ stares$ifarg: $lastarg$~$ at you",
      "$mcname$ stares$ifarg: $lastarg$~$ at $hcname$" }) }),
  "stifle" : ({ 0, ({
      "You stiffle a giggle",
      "$mcname$ stiffles a giggle", }) }),
  "strangle" : ({ ({ 0, "%s", 1, "%s about %s", 1, "%s %s" }), ({
      "You strangle yourself",
      "$mcname$ strangles $mobj$self",
      "You strangle $hcname$$ifarg: about $arg:#$~$",
      "$mcname$ strangles you$ifarg: about $arg:#$~$",
      "$hcname$ strangles $hcname$$ifarg: about $arg:#$~$", }) }),
  "stroke" : ({ ({ 0, "%s" }), ({
      "You stroke $hcname$",
      "$mcname$ strokes you",
      "$mcname$ strokes $hcname$", }) }),
  "strut" : ({ 0, ({
      "You strut your stuff",
      "$mcname$ struts proudly around the room" }) }),
  "stumble" : ({ ({ 3, "%s" }), ({
      "You stumble $ifarg:$arg:#$$else$around the room~$",
      "$mcname$ stumbles $ifarg:$arg:#$$else$around the room~$" }) }),
  "sulk" : ({ 0, ({
      "You sulk in the corner",
      "$mcname$ sulks in the corner" }) }),
  "swim" : ({ 0, ({
      "You give a good imitation of swimming", "$mcname$ swims around" }) }),
  "tackle" : ({ ({ 0, "%s", }), ({
      "You tackle $hcname$ in the $arg:chest,leg,arm,knee,head,"+
      "foot,toe,finger$",
      "$mcname$ tackles you in the $lastarg$",
      "$mcname$ tackles $hcname$ in the $lastarg$", }) }),
  "tap" : ({ ({ 
      0, "%s", 2, "%s on the %s", 2, "%s on %s",
      2, "%s %s", 1, "%s %s", 3, "%s" }), ({ /* this is furrys */
	  "You tap your $arg:foot,head,left leg,right leg,fingers,nose,"+
	  "shoulder,teeth,arm$ impatiently",
	  "$mcname$ taps $mposs$ $lastarg$ impatiently",
	  "You tap $hcname$ on the $arg:shoulder,head,arm,nose,leg,teeth,foot,ear,"+
	  "frog,other thing$",
	  "$mcname$ taps you on the $lastarg$",
	  "$mcname$ taps $hcname$ on the $lastarg$" }) }),
  "taunt" : ({ ({ 0, "%s" }), ({
      "You taunt yourself",
      "$mcname$ taunts $mposs$sely, how strange",
      "You taunt $hcname$",
      "$mcname$ taunts you",
      "$mcname$ taunts $hcname$" }) }),
  "tease" : ({ ({ 0, "%s", 1, "%s %s", 2, "%s %s", }), ({
      "You tease yourself",
      "$mcname$ teases $mbj$self, he appears to enjoy it",
      "You tease $hcname$ $arg:mercilessly,insistantly,annoyingly,"+
           "lovingly,mischeviously,rudely,carefully,quickly,slowly,"+
           "halfheartedly,idlely,obnoxiously$",
      "$mcname$ teases you $lastarg$",
      "$mcname$ teases $hcname$ $lastarg$", }) }),
  "thank" : ({ ({ 0, "%s", 2, "%s %s" }), ({
      "You thank $hcname$$ifarg: $arg:profusely,gratuitously,lots,slightly,"+
      "reservedly$~$",
      "$mcname$ thanks you$ifarg: $lastarg$~$",
      "$mcname$ thanks $hcname$$ifarg: $lastarg$~$" }) }),
  "think" : ({ ({ 3, "%s", }), ({
      "You think $ifarg:$arg:#$$else$carefully~$",
      "$mcname$ thinks $ifarg:$lastarg$$else$carefully~$" }) }),
  "throw" : ({ ({ 0, "%s" }), ({
      "Hi yyyyaaa",
      "$mcname$ throws you to the floor$force#smile h at $mname$#2$",
      "$mcname$ throws $hcname$ to the floor in a happy embrace" }) }),
  "tickle" : ({ ({ 0,"%s", 1, "%s %s", 2, "%s %s" }), ({
      "You tickle $hcname$$ifarg: $arg:mercilessly,ruthlessly,fearlessly,"+
            "quickly,sneakily,weakly,carefully,gently,harshly,inconcievablely"+
            "slowly$~$",
      "$mcname$ tickles you$force#giggle#2$$ifarg: $lastarg$~$",
      "$mcname$ tickles $hcname$$ifarg: $lastarg$~$" }) }),
  "tongue" : ({ ({ 0,"%s", }), ({
      "You poke your tongue out at $hcname$",
      "$mcname$ poke your tongue out at you",
      "$mcname$ poke your tongue out at $hcname$", }) }),
  "tremble" : ({ ({ 3, "%s" }), ({
      "You tremble $arg:in fear,in nervousness,in pain,slightly,violently,"+
           "insistantly,carefully,in a pink fit$",
      "$mcname$ trembles $lastarg$", }), }),
  "trip" : ({ ({ 0, "%s" }), ({
      "You trip up",
      "$mcname$ trips up",
      "You trip $hcname$",
      "$mcname$ trips you",
      "$mcname$ trips $hcname$" }) }),
  "tweak" : ({ ({ 0, "%s", 1, "%s %s", 2, "%s %s", }), ({
      "You tweak $hcname$'s $arg:bottom,ear,arm,leg,nose,bits$",
      "$mcname$ tweaks your $lastarg$",
      "$mcname$ tweaks $hcname$'s $lastarg$" }), }),
  "twiddle" : ({ ({ 3, "%s" }), ({
      "You twiddle your $arg:thumbs,fingers,hair,nose$",
      "$mcname$ twidles $mposs$ $lastarg$" }) }),
  "twitch" : ({ 0, ({
       "You twitch your $arg:nose,arm,body,finger,igloo$",
       "$mcname$ twitches $mposs$ $lastarg$" }) }),
  "waggle" : ({ ({ 0, "%s", 0, "at %s" }), ({
      "$ifarg:You waggle your $arg:arm,leg,head,nose,ear,frog,pinky,"+
           "toe$$else$What did $hcname$ do wrong??!?~$",
      "$mcname$ waggles $mposs$ $ifarg:$lastarg$$else$finger~$ at you",
      "$mcname$ waggles $mposs$ $ifarg:$lastarg$$else$finger~$ at $hcname$",
        }) }),
  "waltz" : ({ ({ 0, "%s", }), ({
      "You waltz around the room with $hcname$",
      "$mcname$ waltzs around the room with you",
      "$mcname$ waltzs around the room with $hcname$", }) }),
  "wave" : ({ ({
      0, "%s", 0, "at %s", 0, "to %s", 2, "at %s %s", 1, "%s %s", 
      1, "%s to %s", 2, "to %s %s", 0, "at %s", 1, "%s at %s",
      2, "%s %s", 3, "%s" }), ({
	  "You wave$ifarg: $arg:tiredly,sleepily,profunctialy,lazily,sadly,"+
	  "happilly,redundently,forlornly,excitedly,longingly,pointlessly,"+
          "your tentacles$~$",
	  "$mcname$ waves$ifarg: $lastarg$~$",
	  "You wave$ifarg: $arg:sleepily,tiredly,profunctialy,lazily,sadly,"+
	  "happilly,forlornly,excitedly,longingly,redundently,pointlessly,"+
	  "pationately,greenly,your tentacles$~$ to $hcname$",
	  "$mcname$ waves$ifarg: $lastarg$~$ to you",
	    "$mcname$ waves$ifarg: $lastarg$~$ to $hcname$" }) }),
  "wibble" : ({ ({ 3, "%s" }), ({
      "You wibble $arg:like bil,loudly,constantly,anoyingly,"+
           "around,up,down,sideways$",
      "$mcname$ wibbles $lastarg$", }) }),
  "wimper" : ({ ({ 3, "%s" }), ({
      "You wimper$ifarg: $arg:painfully,feafully,carefully,dimuatively,"+
            "happily,winningly,in triumph,with the wombles$~$",
      "$mcname$ wimpers$ifarg: $lastarg$~$", }), }),
  "whicker" : ({ 0, ({
      "You wicker like a horse",
      "$mcname$ whickers like a horse", }) }),
  "whistle" : ({ ({ 0, "%s %s", 2, "%s at %s", 3, "%s" }), ({
      "You whistle $arg:innocently,appreciatively,loudly,musically,pleasantly,"+
      "discordantly$",
      "$mcname$ whistles $lastarg$",
      "You whistle $arg:appreciatively,innocently,loudly,musically,pleasantly,"+
      "discordantly$",
      "$mcname$ whistles $lastarg$ at you",
      "$mcname$ whistles $lastarg$ at $hcname$" }) }),
  "wince" : ({ 0, ({ "You wince", "$mcname$ winces" }) }),
  "wiggle" : ({ ({ 0, "%s", 2, "%s at %s", 3, "%s" }), ({
      "You wiggle your bottom$ifarg: $arg:enticingly,suggestively,vigourously,"+
        "arrogantly,nonchalantly,quickly$~$",
      "$mcname$ wiggles $mposs$ bottom$ifarg: $lastarg$~$",
      "You wiggle your bottom$ifarg: $arg:enticingly,suggestively,vigourously,"+
      "arrogantly,nochalantly,quickly$~$ at $hcname$",
      "$mcname$ wiggles $mposs$ bottom$ifarg: $lastarg$~$ at you",
      "$mcname$ wiggles $mposs$ bottom$ifarg: $lastarg$~$ at $hcname$" }) }),
  "wink" : ({ ({ 0, "%s", 0, "at %s", 1, "%s at %s", 1, "%s %s",
                 2, "at %s %s", 2, "%s %s", 3, "%s" }), ({
      "You wink$ifarg: $arg:suggestively,broadly,slowly,loudly,grumpily,"+
           "despairingly,coyly,desperately,insolently,curtly,lovingly,"+
           "patiently,sadisticly,warily,seducitive,lazily$~$",
      "$mcname$ winks$ifarg: $lastarg$~$",
      "You wink$ifarg: $arg:suggestively,broadly,slowly,loudly,grumpily,"+
           "despairingly,coyly,desperately,insolently,curtly,lovingly,"+
           "patiently,sadisticly,warily,seducitive,lazily$~$ at $hcname$",
      "$mcname$ winks$ifarg: $lastarg$~$ at you",
      "$mcname$ winks$ifarg: $lastarg$~$ at $hcname$" }) }),
  "wipe" : ({ ({ 0, "%s", 0, "on %s", 1, "%s on %s" }), ({
      "You wipe the sweat from your brow",
      "$mcname$ wipes the sweat from $mposs$ brow",
      "You wipe your $arg:hands,leg,arm,nose$ on $hcname$",
      "$mcname$ wipes $mposs$ $lastarg$ on you",
      "$mcname$ wipes $mposs$ $lastarg$ on $hcname$" }) }),
  "wobble" : ({ ({ 3, "%s" }), ({
      "You wobble $arg:around a bit,like jelly,unsteadily,"+
      "happily$",
      "$mcname$ wobbles $lastarg$", }) }),
  "womble" : ({ ({ 3, "%s" }), ({ 
      "You womble around$ifarg: $arg:#$~$",
      "$mcname$ wombles around$ifarg: $lastarg$~$", }) }),
  "wonder" : ({ ({ 3, "%s" }), ({
      "You wonder $ifarg:$arg:#$$else$about reality and how "+
      "boring it can be~$",
      "$mcname$ wonder $ifarg:$arg:#$$else$about reality and how "+
      "boring it can be~$", }) }),
  "worship" : ({ ({ 0, "%s", 3, "%s" }), ({
      "You worship $ifarg:$arg:#$$else$pinkfish~$",
      "$mcname$ worships $ifarg:$lastarg$$else$pinkfish~$",
      "You worship $hcname$",
      "$mcname$ worships you", "$mcname$ worships $hcname$" }) }),
  "yawn" : ({ ({ 3, "%s" }), ({
      "You yawn $arg:tiredly,boredly,sleepily$",
      "$mcname$ yawns $lastarg$" }) }) ,
  "zip" : ({ ({ 0, "%s" }), ({
      "You zip around the room",
      "$mcname$ zips around the room",
      "You zip around $hcname$",
      "$mcname$ zip around you",
      "$mcname$ zip around $hcname$", }), }),
]);
}

string query_long() {
     int i, j;
     string s, s1, s2, bit;
     string data;

     string *cmds;
     mixed  *values;

     s = "Here are the current soul commands in this hoopy soul.\n"+
         "The # before a soul command means it has strange arguments.\n"+
        "The * after a soul command means you can only use it at someone.\n"+
    "Total number of soul data is "+m_sizeof(soul_data)+"\n"+
   "The + after a soul command means you can use it both at someone and not.\n";
     if (calc_long) {
	 s += sprintf("%#-*s\nShare and enjoy.\nPlease use the \"soul\" " +
		      "command to report errors and additions.\n",
		      this_player()->query_cols(), calc_long);
	 return s; 
     }
     cmds   = m_indices(soul_data);
     values = m_values(soul_data);
     j = 0;
     data = "";
     for (i = 0; i < sizeof(cmds);i ++) {
	 j++;
	 bit = cmds[i];
	 if (sizeof(values[i][1]) == 3) {
	     bit += "*";
	 } else	if (sizeof(values[i][1]) == 5) {
	     bit += "+";
	 }
	 if (sscanf(values[i][1][0],"%s$arg:%s",s1,s2) == 2) {
	     bit = "#"+bit;
	 }
	 data += bit+"\n";
     }

     s += sprintf("%#-*s\nShare and enjoy.\nPlease use the \"soul\" command "+
		  "to report errors and additions.\n",
		  this_player()->query_cols(), data);
     calc_long = data;
     return s;
}
 
parse_string(s,ob,arg) {
  string s1,s2,s3,s4,str,arr;
  int i;
 
  str=s;
  s4 = "";
  while (sscanf(str,"%s$%s$%s",s1,s2,s3) == 3)
    switch (s2) {
      case "mcname" :
        str = s1+this_player()->query_cap_name()+s3;
        break;
      case "hcname" :
        str = s1+ob->query_cap_name()+s3;
        break;
      case "mname" :
        str = s1+this_player()->query_name()+s3;
        break;
      case "hname" :
        str = s1+ob->query_name()+s3;
        break;
      case "mpronoun" :
        str = s1+this_player()->query_pronoun()+s3;
        break;
      case "hpronoun" :
        str = s1+ob->query_pronoun()+s3;
        break;
      case "mobj" :
        str = s1+this_player()->query_objective()+s3;
        break;
      case "hobj" :
        str = s1+ob->query_objective()+s3;
        break;
      case "mposs" :
        str = s1+this_player()->query_possessive()+s3;
        break;
      case "hposs" :
        str = s1+ob->query_possessive()+s3;
        break;
      case "lastarg" :
        str = s1+lastarg+s3;
        break;
      default :
        s4 += s1+"$"+s2;
        if (!s3)
          str = "$";
        else
          str = "$"+s3;
        break;
      }
  str = s4+str;
  while (sscanf(str,"%s$force#%s#%d$%s",s1,s2,i,s3)==4) {
    call_out("do_force", i, ({s2, ob}));
    str = s1+s3;
  }
  while (sscanf(str,"%s$arg:%s$%s",s1,s2,s3)==3) {
    if (s2 == "#") {
      lastarg = implode(explode(arg, "your"),
                        this_player()->query_possessive());
      str = s1+arg+s3;
    } else {
      arr = explode(s2,",");
      if (s2[0] == ',')
        arr = ({ "" }) + arr;
      if (member_array(arg,arr)<0)
        if (!arg) {
          str = s1+arr[0]+s3;
          lastarg = implode(explode("&"+arr[0]+"&", "your"),
                    this_player()->query_possessive());
          lastarg = lastarg[1..strlen(lastarg)-2];
        } else {
/* try for best match */
          for (i=0;i<sizeof(arr);i++)
            if (sscanf(arr[i],arg+"%s",s2)) {
              str = s1+arr[i]+s3;
              lastarg = implode(explode("&"+arr[i]+"&", "your"),
                        this_player()->query_possessive());
              lastarg = lastarg[1..strlen(lastarg)-2];
              break;
            } else if (arr[i] == "#") {
              str = s1+arg+s3;
              lastarg = implode(explode("&"+arr[i]+"&", "your"),
                        this_player()->query_possessive());
              lastarg = lastarg[1..strlen(lastarg)-2];
              break;
            }
          if (i==sizeof(arr)) {
            if (arr[0] == "")
              arr = delete(arr,0);
            write(
       "You cannot do that. Available options are "+implode(arr,", ")+"\n");
             return 0;
          }
        }
      else {
        str = s1+arg+s3;
        lastarg = implode(explode("&"+arg+"&", "your"),
                  this_player()->query_possessive());
        lastarg = lastarg[1..strlen(lastarg)-2];
      }
    }
  }
  while (sscanf(str,"%s$ifarg:%s~$%s",s1,s2,s3)==3) {
    string estr;
 
    sscanf(s2,"%s$else$%s",s2,estr);
    if (arg && arg != "")
      str = s1+parse_string(s2,ob,arg)+s3;
    else
      if (estr)
        str = s1+parse_string(estr,ob,arg)+s3;
      else
        str = s1+s3;
  }
  return str;
}
 
find_all_liv(str) {
  object ob, ret;
  int i;

  ret = ({ });
  str = lower_case(str);
  if (str == "everyone") {
    ob = users();
    if ((i = member_array(this_player(), ob)) != -1)
      ob = delete(ob, i, 1);
    return ob;
  }
  ob = find_match(str,environment(this_player()));
  for (i=0;i<sizeof(ob);i++)
    if (living(ob[i]) && ob[i] != this_player())
      ret += ({ ob[i] });
  if ((ob = find_living(str)))
    if (member_array(ob, ret) == -1)
      ret += ({ ob });
  return ret;
}
 
soul_command(verb,str) {
     int i, cur, j, lvl;
     object *ob, *tmp_ob;
     string tmp, nick, last, liv, other, s1, s2, livfail;
     mixed  *data;
 
     lastarg = "";
     ob = ({ });
     data = soul_data[verb];
     if (!data) {
	 return 0;
     }

     if (str && sizeof(data[1]) == 2 && data[1][0] != '#') {
	 notify_fail("No matter how hard you try, you fail to manage this.\n");
     }
     if (!data[0] || !str) {
	 liv = 0;
	 other = "";
     } else for (j = 0; j < sizeof(data[0]); j += 2)
     switch (data[0][j]) {
      case 0 :
          if (lvl > 1)
            break;
          if (sscanf(str, data[0][j+1], s1)==1)
            if (sizeof((tmp_ob=find_all_liv(s1)))) {
              lvl = 1;
              other = "";
              liv = s1;
              ob = tmp_ob;
            } else
              livfail = s1;
          break;
      case 1 :
          if (lvl > 2)
            break;
          if (sscanf(str, data[0][j+1], s1, s2)==2)
            if (sizeof((tmp_ob=find_all_liv(s2)))) {
              lvl = 2;
              liv = s2;
              other = s1;
              if (data[0][j+1] != "%s %s")
                lvl++;
              ob = tmp_ob;
            } else
              livfail = s2;
          break;
      case 2 :
          if (lvl > 2)
            break;
          if (sscanf(str, data[0][j+1], s1, s2) ==2)
            if (sizeof((tmp_ob=find_all_liv(s1)))) {
              lvl = 2;
              liv = s1;
              other = s2;
              if (data[0][j+1] != "%s %s")
                lvl++;
              ob = tmp_ob;
            } else
              livfail = s2;
          break;
      case 3 :
          if (lvl > 0)
            break;
          if (sscanf(str, data[0][j+1], other) == 1) {
            liv = 0;
            ob = ({ });
          }
          break;
    }
  if (liv) {
    string me,him,every;
 
    if (!liv && (str && data[1][0] != '#'))
      liv = str;
    str = other;
    if (!sizeof(ob)) {
      notify_fail("Sorry, but '"+liv+"' is not logged in.\n");
      return 0;
    }
    if (sizeof(data[1])==3) {
	me = data[1][0];
	him = data[1][1];
	every = data[1][2];
    } else if (sizeof(data[1])==5) {
	me = data[1][2];
	him = data[1][3];
	every = data[1][4];
    } else {
      notify_fail("You cannot use that soul command in two thingy mode.\n");
      return 0;
    }
    while (cur<sizeof(ob)) {
      if (environment(ob[cur]) != environment(this_player()))
        if (this_player()->query_sp() < 0) {
          notify_fail("Not enough power points to do that.\n");
          return 0;
        } else
          this_player()->restore_spell_points(-10);
      tmp = parse_string(me,ob[cur],str,last);
      if (!tmp) {
        return 1;
      }
      this_player()->event_soul(this_player(), tmp+".\n");
      ob[cur]->event_soul(this_player(),
                         parse_string(him,ob[cur],str,last)+".\n");
      ob[cur]->event_soul_command(this_object(),verb,this_player(), liv, other);
      event(environment(previous_object()), "soul",
              parse_string(every, ob[cur], str, last)+".\n", ({ ob[cur], previous_object() }) );
      if (environment(previous_object()) != environment(ob[cur]) &&
          environment(ob[cur]))
        event(environment(ob[cur]), "soul",
              parse_string(every, ob[cur], str, last)+".\n", ({ ob[cur], previous_object() }) );
      cur++;
    }
    return 1;
  } else if (sizeof(data[1])==3) {
    if (!livfail)
      notify_fail("You don't have the courage to do that.\n");
    else
      notify_fail("Cannot find '"+livfail+"'.\n");
    return 0;
  }
  tmp = parse_string(data[1][0],ob,other);
  if (!tmp) {
    return 1;
  }
  this_player()->event_soul(this_player(), tmp+".\n");
  event(environment(previous_object()), "soul",
           parse_string(data[1][1], this_player(),other,last)+".\n",  ({this_player()}) );
  return 1;
}

do_force(str) {
       str[1]->soul_com_force(str[0]);
}

void add_soul_command(string name, mixed format, mixed thingo) {
     if (soul_data[name]) {
	 return;
     }

  if (format && !pointerp(format))
    return ;
  if (!stringp(name))
    return ;
  if (!pointerp(thingo))
    return ;
  if (sizeof(thingo) != 3 && sizeof(thingo) != 2 && sizeof(thingo) != 5)
    return ;
  soul_data[name] = ({ format, thingo });
}

void delete_soul_command(string name) {
     soul_data = m_delete(soul_data, name);
}

string help_soul(string str) {
     int j;
     int i, off;
     string s1,s2,s3,s4,ret, *bit;
     object ob;
     mixed *data;
     
     data = soul_data[str];
     if (!data) {
	 return 0;
     }
     ret = "";
     if (!pointerp(data[0])) {
	 return "The soul command "+str+" has no optional paramaters at all.\n";
     }
     for (j = 0; j < sizeof(data[0]);j += 2) {
	 bit = explode(" " + data[0][j+1]+" ","%s");
	   switch (data[0][j]) {
	     case 0: 
	       ret += str+bit[0]+"<person>"+bit[1];
	       break;
	     case 2:
	       ret += str+bit[0]+"<person>"+bit[1]+"<argument>"+bit[2];
	       break;
	     case 1:
	       ret += str+bit[0]+"<argument>"+bit[1]+"<person>"+bit[2];
	       break;
	     case 3:
	       ret += str+bit[0]+"<argument>"+bit[1];
	       break;
	   }
	   ret += "\n";
       }
       ret = "Usage of the soul command "+str+"\n"+
	 sprintf("%-#*s\n\n\n", this_player()->query_cols(), ret);
       ob = clone_object("/obj/monster");
       ob->set_name("harry");
       ob->set_gender(random(2)+1);
       if (sizeof(data[1]) == 2 || sizeof(data[1]) == 5) {
	   ret += "In no living object mode.\n";
	   bit = ({ });
	   if (sscanf(data[1][0], "%s$arg:%s$%s", s1,s2,s3) == 3)
	     bit = explode(s2,",");
	   if (sscanf(data[1][0], "%s$ifarg:%s~$%s", s1, s2, s3) == 3) {
	       ret += "Has a different no arguments command.\n"+
		 "The arguments are selected from "+
		   implode(bit,", ")+"\n";
	   } else if (!sizeof(bit))
	     ret += "Has no cute arguments\n";
	   else
	     ret += "Has cute arguments of "+implode(bit,", ")+"\n";
	   ret += "\n\n";
	   off = 2;
       }
       if (sizeof(data[1]) == 3 || sizeof(data[1]) == 5) {
	   ret += "In living object mode\n";
	   bit = ({ });
	   if (sscanf(data[1][off+0], "%s$arg:%s$%s", s1,s2,s3) == 3)
	     bit = explode(s2,",");
	   if (sscanf(data[1][off+0], "%s$ifarg:%s~$%s", s1, s2, s3) == 3) {
	       ret += "Has a different no arguments command.\n"+
		 "The arguments are selected from "+
		   implode(bit,", ")+"\n";
	   } else if (!sizeof(bit))
	     ret += "Has no cute arguments\n";
	   else
	     ret += "Has cute arguments of "+implode(bit,", ")+"\n";
	   off = 2;
       }
       return ret;
}
