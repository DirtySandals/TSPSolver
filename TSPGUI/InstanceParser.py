def read_coordinates(file_lines):
    coords = []
    
    for line in file_lines:
        line = line.strip()
        if line == "EOF":
            break
        split_line = line.split()
        coordinate = tuple([float(item) for i, item in enumerate(split_line) if i > 0])
        coords.append(coordinate)
        
    return coords

def parse_instance(fileName):
    coords = None
    
    with open(fileName, 'r') as file:
        file_lines = file.readlines()
        
        for i, line in enumerate(file_lines):
            line = line.strip()
            split_line = line.split()
            
            if split_line[0] == "NODE_COORD_SECTION":
                coords = read_coordinates(file_lines[i + 1:])
                break
    
    return coords

