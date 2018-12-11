f = open('l5.xml', 'r') # Открытие XML-файла
temp = f.read()
temp = list(temp)


def put_tegs(t):  # Получение всех тегов из файла
    lst_all, lst = [], []
    result = False
    for e in t:        
        if e == '<':
            result = True
        elif (e == ' ' or e == '>') and result == True:
            lst.append('>')
            result = False
        if result == True:
            lst.append(e)
        elif result == False:
            if lst != []:
                lst_all.append(lst)
                lst = []
    for i in range(len(lst_all)):
        lst_all[i] = ''.join(lst_all[i])
    return lst_all



#print(tegs)


def check_tegs(teg): # Проверка на первый символ и на закрытость тегов
    num = 0
    for e in teg:
        if e[1] in '0123456789':
            return False
        if '/' not in e:
            if str(e[0]+'/'+e[1:]) in teg:
                ind = teg.index(str(e[0]+'/'+e[1:]))
                ind2 = teg.index(e)
                teg[ind] += '*'
                teg[ind2] += '*'
                num += 1
    if num != len(teg)/2:
        return False
    else:
        return True

#print(len(tegs))

#tegs = put_tegs(temp)
#print(tegs)

def create_index(teg): # Получение индексов тегов
    lst = [] 
    for num in range(len(teg)):
        if '/' not in teg[num]:
            ind1 = num
            for i in range(num, len(teg)):
                if teg[i] == teg[num][0]+'/'+teg[num][1:]:
                    ind2 = i
                    break
            yield ind1, ind2

#tmp = list(create_index(tegs))
#print("Tmp", tmp)


def second_check(teg): # Проверка на вложенность тегов
    for e in tmp:
        acc = teg[e[0]:e[1]+1]
        if check_tegs(acc) == False:
            return False
    return True



def end(j):
    if check_tegs(tegs) != True or second_check(tegs) != True:
        return 'Ошибка документа'
    print('Tест')
    lst = []
    for e in j:
        if e != '\n' and e != '\t':
            lst.append(e)
    lst = ''.join(lst)
    lst = lst.split('question')

    for i in lst:
        i = i.split('answer')
        qst = []
        #print(i)
        for id_n in i:
            if 'id =' in id_n and id_n[7] in '0123456789':
                #print('hey')
                #print(id_n)
                qst.append(list(id_n[7:8]))
                #print(id_n)
                print('\n'+id_n[7:8]+'.'+id_n[14:-5]+'\n')
            elif 'id =' in id_n:
                qst.append(id_n[11])
                qst.append(id_n[7])
                print(id_n[11:-2])
        if len(qst) >= 2:
            answ = input('Ваш ответ: ')
            if answ in qst[1:]:
                ind = qst.index(answ)
                if qst[ind+1] == '+':
                    print('Ваш ответ верен.')
                elif qst[ind+1] == '-':
                    correct = qst.index('+')
                    print('Ваш ответ не верен, Правильный ответ:', qst[correct-1]+'.')
                else:
                    #print(qst)
                    print('Ошибка теста.')
            else:
                correct = qst.index('+')
                print('Ошибка ввода. Правильный ответ:', qst[correct-1]+'.','Переход к следующему вопросу.')
    return ''

#print(end(temp))

#def put(text):



def xmlparser(xmltext, tag, buf):
    st = ""
    for i in xmltext:
        st+=i

    tags = put_tegs(xmltext)[1:]
    #print
    tags_new = []
    for e in tags:
        tags_new.append(e[1:-1])

    #st = st.split('>')
    st = st.split('\n')
    #st = st.split('\t')


    def repit(stroka):


        for e in stroka:
            if '<'+tag+'>' in e:
                ind1 = stroka.index(e)+1
                print(ind1)
            if '<'+'/'+tag+'>' in e:
                ind2 = stroka.index(e)
            
            #print(st[ind1:ind2])

            for i in stroka[ind1:ind2]:
                print(put_tegs(list(i)))
                i = i.strip(put_tegs(list(i))[1])
                i = i[i.index('>')+1:]

                print(i)
                buf+=i

                stroka = stroka[ind2+1:]
                print(buf)
    print(repit(st))



 

    #ind1 = st.index('<'+tag+'>'
    #ind2 = st.index('<'+'/'+tag+'>')
    #print(st)


      #print("hey")
        




    return ''


xmlparser(temp, "item", '')



