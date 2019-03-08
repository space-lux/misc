futhark="ᚫᛒᚲᛞᛖᚠᚷᚺᛁᛃᚲᛚᛗᚾᛟᛈᛜᚱᛊᛏᚢᚹᚹᛝᛇᛉ"

def_repl={
    "e":"éëêè",
    "a":"âäà",
    "o":"ôö",
    "u":"ùüû",
    "i":"îï"
    }

repl={}

for c in def_repl:
    for d in def_repl[c]:
        repl[d]=c

def remove_accents(c):
    try:
        return repl[c]
    except KeyError:
        return c

def tr_char(c):
    try:
        if c==" ":
            return "᛫"
        return futhark[ord(remove_accents(c.lower()))-ord("a")]
    except:
        return c

def tr_str(s):
    return ''.join(map(tr_char,s.replace("th","ᚦ")))
