class Container:
    def __init__(self, days, hospitals):
        self.days = days
        self.hospitals = hospitals


filedata = "11 3\n42 -10 8 1 11 -6 -12 16 -15 -11 13"


def read_file(f):
    # open file and read filedata
    [metadata, data] = f.splitlines()

    hc = int(metadata.split(' ')[1])
    days = [int(d) for d in data.split(' ')]

    container = Container(days, hc)
    return container


def stupid_solution(container):
    l = len(container.days)
    best = 0

    for i in range(0, l):
        for j in range(i, l):
            curr_seq = container.days[i:j+1]
            curr_len = len(curr_seq)
            metric = sum(curr_seq) / (curr_len * container.hospitals)

            if metric <= -1 and curr_len > best:
                best = curr_len
    return best


container = read_file(filedata)
sol = stupid_solution(container)
pass
