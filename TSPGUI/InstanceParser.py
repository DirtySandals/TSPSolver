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

def instance_scaler(points, center_x, center_y, width, height):
    border_tolerance = 15
    min_x = points[0][0]
    max_x = points[0][0]
    min_y = points[0][1]
    max_y = points[0][1]
    
    for point in points:
        min_x = min(min_x, point[0])
        max_x = max(max_x, point[0])
        min_y = min(min_y, point[1])
        max_y = max(max_y, point[1])
    
    left = center_x - (width / 2) + border_tolerance
    top = center_y - (height / 2) + border_tolerance

    scale_x = (width - 2 * border_tolerance) / (max_x - min_x)
    scale_y = (height - 2 * border_tolerance) / (max_y - min_y)

    for i in range(len(points)):
        points[i] = (left + (points[i][0] - min_x) * scale_x, top + (points[i][1] - min_y) * scale_y)        

    return points