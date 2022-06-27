def load_lines(filename, strip_empty = True):
    with open(filename) as f:
        lines = f.readlines()

        if strip_empty:
            return list(filter(None, [line.strip() for line in lines]))
        else:
            return [line.strip() for line in lines]

def load_test_lines(lines, strip_empty = True):
    if strip_empty:
        return list(filter(None, lines.splitlines()))
    else:
        return lines.splitlines()
