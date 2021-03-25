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
    return 0


container = read_file(filedata)
sol = stupid_solution(container)
pass
