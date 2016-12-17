import PIL
from PIL import Image
import random
import sys

def open_image(filepath):
    return Image.open(filepath) # open colour image

# resize image
def resize_image(basewidth, image_file):
    basewidth = basewidth
    wpercent = (basewidth/float(image_file.size[0]))
    hsize = int((float(image_file.size[1])*float(wpercent)))
    image_file = image_file.resize((basewidth,hsize), PIL.Image.ANTIALIAS)
    width, height = image_file.size
    return image_file, width, height

def convert_to_bw(image_file, threshold):
    ipixels = image_file.convert('1').load()

    out_im = Image.new('1', image_file.size, None)
    opixels = out_im.load()
    y_range = image_file.size[1]
    x_range = image_file.size[0]

    for x in range(x_range):
        for y in range(y_range):
            opixels[x,y] = 255
    x = 0
    y = 0
    block_size = 3
    sample_range = 4

    while y < y_range:
        while x < x_range:
            adj_sum = 0
            adj_pixels = 0
            for a in range(sample_range):
                for b in range(sample_range):
                    adj_pixels += 1
                    adj_sum += ipixels[(x+a) % x_range, (y+b) % y_range]
            if (adj_sum / (sample_range**2)) < threshold:
                opixels[min(x_range-1, (x + (random.random()*sample_range))), min(y_range-1, (y + (random.random() * sample_range)))] = 0
            else:
                opixels[x,y] = 255

            x += block_size
        x = 0
        y += block_size


    return out_im

def get_pixels(image_file):
    return image_file.load()

def convert_image_to_bw_pixels(filepath, name, basewidth, threshold):
    image_file = open_image(filepath)
    image_file, width, height = resize_image(basewidth, image_file)
    image_file = convert_to_bw(image_file, threshold)
    image_file.save("drawings/" + str(basewidth) + "_" + name, "jpeg")
    pixels = get_pixels(image_file)

    for x in range(width):
        line = ""
        for y in range(height):
            line += str(pixels[x,y]) + " "
        print(line)

    return pixels, width, height


if __name__ == "__main__":
    filename = sys.argv[1]
    name = sys.argv[2]
    basewidth = int(sys.argv[3])
    threshold = int(sys.argv[4])
    pixels, width, height = convert_image_to_bw_pixels(filename, name, basewidth, threshold)





