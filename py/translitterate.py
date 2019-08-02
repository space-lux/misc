futhark="ᚫᛒᚲᛞᛖᚠᚷᚺᛁᛃᚲᛚᛗᚾᛟᛈᛜᚱᛊᛏᚢᚹᚹᛝᛇᛉ"

def_repl={
    "e":"éëêè",
    "a":"âäà",
    "o":"ôö",
    "u":"ùüû",
    "i":"îï",
    "c":"ç"
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
        o=ord(remove_accents(c.lower()))-ord("a")
        if o<0:
            raise Exception
        return futhark[o]
    except:
        return c

def tr_str(s):
    return ''.join(map(tr_char,s.replace("th","ᚦ")))

def utr_char(c):
    if c=="ᚦ":
        return "th";
    if c=="᛫":
        return " ";
    try:
        return chr(ord('a')+futhark.index(c))
    except:
        return c

def utr_str(s):
    return ''.join(map(utr_char,s))
