# Reads coordinates from instance file
def read_coordinates(file_lines):
    coords = []
    # For each line, split numbers and enumerate into coordinate
    for line in file_lines:
        line = line.strip()
        if line == "EOF":
            break
        split_line = line.split()
        coordinate = tuple([float(item) for i, item in enumerate(split_line) if i > 0])
        coords.append(coordinate)
        
    return coords

# Parses instance file
def parse_instance(fileName):
    coords = None
    # Open file
    with open(fileName, 'r') as file:
        file_lines = file.readlines()
        # Looks for NODE_COORD_SECTION and retrieve coordinates
        for i, line in enumerate(file_lines):
            line = line.strip()
            split_line = line.split()
            
            if split_line[0] == "NODE_COORD_SECTION":
                coords = read_coordinates(file_lines[i + 1:])
                break
    
    return coords
# Scales the points found from instance files to fit in screen
def instance_scaler(points, center_x, center_y, width, height):
    border_tolerance = 15 # Pixels from edge of graph
    min_x = points[0][0]
    max_x = points[0][0]
    min_y = points[0][1]
    max_y = points[0][1]
    # Find min and max of x and y
    for point in points:
        min_x = min(min_x, point[0])
        max_x = max(max_x, point[0])
        min_y = min(min_y, point[1])
        max_y = max(max_y, point[1])
    # Get left x coord and top y coord of graph
    left = center_x - (width / 2) + border_tolerance
    top = center_y - (height / 2) + border_tolerance
    # Scale points to fit in graph exactly
    scale_x = (width - 2 * border_tolerance) / (max_x - min_x)
    scale_y = (height - 2 * border_tolerance) / (max_y - min_y)
    # Scale and translate each point to fit in graph
    for i in range(len(points)):
        points[i] = (left + (points[i][0] - min_x) * scale_x, top + (points[i][1] - min_y) * scale_y)        

    return points