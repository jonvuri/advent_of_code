def load_lines(filename):
    with open(filename) as f:
        lines = f.readlines()
        return list(filter(None, [line.strip() for line in lines]))

def load_test_lines(lines):
    return list(filter(None, lines.splitlines()))
