# ЗАПУСКАТЬ ЧЕРЕЗ GIT, а не Ctrl+B

def binary_tree(value):
    return [value, [], []]

def get_left_child(node):
    return node[1]

def get_right_child(node):
    return node[2]

def get_root_value(node):
    return node[0]

def insert_left(node, value):
    child  = node.pop(1)
    if len(child) > 1:
        node.insert(1, [value, child, []])
    else:
        node.insert(1, [value, [], []])
        return node

def insert_right(node, value):
    child  = node.pop(1)
    if len(child) > 1:
        node.insert(2, [value, child, []])
    else:
        node.insert(2, [value, [], []])
        return node

def set_root_value(node, new_value):
    node[0] = new_value

# Для словарей dict.keys() - все ключи 
# dict.values() - все значения
# dict.items() - значение и ключи в формате кортежа



fp = open("l3.txt", "r")
rd = fp.read()
print(rd)


def make_dictionary(lst):
    d = {}
    for i in lst:
        if i not in d:
            d[i] = [1, [], []]
        else:
            d[i][0] += 1
    return d

dictone = make_dictionary(rd)
lst_values = dictone.values()
lst_items = dictone.items()

print(dictone)


def make_tree(l):
    if len(l) == 1:
        return l[0]
    else:        
        l = sorted(l)
        l.append([l[0][0] + l[1][0], l[0], l[1]])
        l.remove(l[0])
        l.remove(l[0])
        return make_tree(l)

tree1 = make_tree(lst_values)
print(tree1)


def find(tree, e, name, line = []): # поиск пути от корня дерева к элементу с сответствующим формированием кода
    if tree == []:
        pass
    else:
        if get_root_value(tree) == e:
            if get_left_child(tree) == [] and get_right_child(tree) == []:
                insert_left(tree, name)
                return True
        for temp in tree[1:]:
            result = find(temp, e, name, line)
            if result:
                if temp == get_left_child(tree):
                    line.append(0)
                elif temp == get_right_child(tree):
                    line.append(1)
                return line
        return False




def make_line(tree, d): # устанавливаем соответствие между символом и его кодом
    for e in sorted(lst_items):
        print('e', e[0], e[1][0])
        lst = find(tree, e[1][0], e[0], line = [])
        temp = []
        for i in range(len(lst)-1, -1, -1):
            temp.append(lst[i])
        d[e[0]] = temp
        print(temp)
    return ''

#print(make_line(tree1, dictone))
#print(tree1)
#print(dictone)


def len_text(d, text): # Вычисление размеров текста до сжатия и после
    len_end = []
    len_start = len(text)*8 #размер входного текста в битах (до сжатия)
    for e in text:
        len_end+=d[e]
    len_end = len(len_end) #размер входного текста в битах (после сжатия)
    return len_start, len_end

#print(len_text(dictone, rd))


def compress():    # подсчет коэффициента сжатия                 
    (len_start, len_end) = len_text(dictone,rd) 
    k = len_start/len_end # размер исходного текста/ размер сжатого
    return k

#print(compress())









