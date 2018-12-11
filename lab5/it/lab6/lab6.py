f1 = open('example.bmp', 'rb') # Открытие 3х файлов в режиме 'rb'
f2 = open('example1.bmp', 'rb')
f3 = open('example2.bmp', 'rb')

fn = open('answer.bmp', 'wb')
fn1 = open('answer1.bmp', 'wb')
fn2 = open('answer2.bmp', 'wb')
fn3 = open('answer3.bmp', 'wb')

temp1 = f1.read() 
temp2 = f2.read()
temp3 = f3.read()

def put(lst): # Получение списков значений 
    l = []
    for e in lst:
        l.append(e)
    return l


l_start1 = put(temp1)[:54]
l_end1 = put(temp1)[54:]

l_start2 = put(temp2)[:54]
l_end2 = put(temp2)[54:-2]

l_start3 = put(temp3)[:54]
l_end3 = put(temp3)[54:-2]


def inverse(l_start, l_end): # Инверсия цвета
    for i in range(len(l_end)):
        l_end[i] = 255 - int(l_end[i])

    return l_start+l_end


#l = inverse(l_start1, l_end1)
#fn2.write(bytes(l))


def turn(l_start, l_end): # Поворот против часовой стрелки
    tmp = []
    ind = 0
    acc = 0
    l = []
    for e in range(0, len(l_end)):        
        if acc <= 2:
            l.append(l_end[e])
            acc+=1
        else:
            if (ind + 1)%100 != 0:
                ind1 = (100*(ind+1) - (100*(ind+1)+10000)//10000) - 10000*(((100*(ind+1)+10000)//10000)-1)
            else:
                ind1 = (100*(ind+1) - (100*(ind+1))//10000) - 10000*(((100*(ind+1))//10000)-1)
            l.insert(0, ind1)
            tmp.append(l)
            ind+=1
            l = []
            l.append(l_end[e])
            acc = 1
    tmp = sorted(tmp)
    tmp.insert(9900, [9900]+tmp[-99][1:])
    tmp1 = []
    tmp2 = []
    for i in tmp:
        tmp2.append(i[1:])
        tmp1 += tmp2[0]
        tmp2 = []
    return l_start+tmp1


#l1 = turn(l_start1, l_end1)
#l1 = turn(l1[:54], l1[54:])
#l1 = turn(l1[:54], l1[54:])
#fn.write(bytes(l1))



#l2 = inverse(l_start2, l_end2)
#l5 = inverse(l_start3, l_end3)
#for e in range(2):
#l2 = turn(l_start2, l_end2)
#l2 = turn(l2[:54], l2[54:-2])


#fn1.write(bytes(l2))



#l3 = turn(l_start3, l_end3)
#l4 = turn(l3[:54], l3[54:-2])
#l5 = turn(l4[:54], l4[54:-2])
#fn2.write(bytes(l5))

''' typ = 0 - склеивается сверху
    typ = 1 - склеивается снизу
    typ = 2 - склеивается справа 
    typ = 3 - склеивается слева '''

    
def join(l_start, typ, *args): # Склеивание 
    if typ == 0:
        l_end = []
        for el in args:
            l_end += el
        if len(args)*100 < 256:
            l_start[22] = len(args)*100
        else:
            l_start[22] = 44
            l_start[23] = 1
        return l_start+l_end

    elif typ == 1:
        l_end = []
        for e in range(len(args)-1, -1, -1):
            l_end += args[e]
        if len(args)*100 < 256:
            l_start[22] = len(args)*100
        else:
            l_start[22] = 44
            l_start[23] = 1
        return l_start+l_end

    elif typ == 2:
        l_end = []
        for e in range(0,  30000, 300):
            for el in args:
                l_end += el[e:e+300]
        if len(args)*100 < 256:
            l_start[18] = len(args)*100
        else:
            l_start[18] = 44
            l_start[19] = 1
        return l_start+l_end

    elif typ == 3:
        l_end = []
        for e in range(0,  30000, 300):
            for i in range(len(args)-1, -1, -1):
                l_end += args[i][e:e+300]
        if len(args)*100 < 256:
            l_start[18] = len(args)*100
        else:
            l_start[18] = 44
            l_start[19] = 1
        return l_start+l_end  
print(l_start1)

l1 = join(l_start1, 2, l_end1, l_end2)
#l = inverse(l[:54], l[54:])

fn.write(bytes(l1))



