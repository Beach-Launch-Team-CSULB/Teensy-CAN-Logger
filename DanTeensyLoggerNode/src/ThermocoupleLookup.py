with open('type_t_parsed.txt') as f:
    lines = f.readlines()
lines = lines[::-1]
Ttypethermodict = {}

negative = False
for i in range(len(lines)):
    data = lines[i].split("\t")
    if not data[0].lstrip('-').isnumeric():
        negative = True
        continue
    for j in range(1,len(data)):
        if data[j] == "" or data[j] == "\n":
            continue
        if negative:
            Ttypethermodict[data[j].strip('\n')] = str(float(data[0].strip('\n')) + j-10)
        else:
            Ttypethermodict[data[j].strip('\n')] = str(float(data[0].strip('\n')) + j-1)

with open('type_k_parsed.txt') as f:
    lines = f.readlines()
lines = lines[::-1]
Ktypethermodict = {}

negative = False
for i in range(len(lines)):
    data = lines[i].split("\t")
    if not data[0].lstrip('-').isnumeric():
        negative = True
        continue
    for j in range(1,len(data)):
        if data[j] == "" or data[j] == "\n":
            continue
        if negative:
            Ktypethermodict[data[j].strip('\n')] = str(float(data[0].strip('\n')) + j-10)
        else:
            Ktypethermodict[data[j].strip('\n')] = str(float(data[0].strip('\n')) + j-1)

def getTemp(TYPE, voltage, RDT):
    
    if TYPE == "t":
        dictionary = Ttypethermodict
    else:
        dictionary = Ktypethermodict

    for key, value in dictionary.items():
        if(float(value) == float(RDT)):
            voltage += float(key)
    try:
        return dictionary[str(voltage)]
    except:
        while str(round(voltage,4)) not in dictionary:
            voltage += 0.001
            print(str(round(voltage,4)))
        return dictionary[str(round(voltage,4))]


    
























