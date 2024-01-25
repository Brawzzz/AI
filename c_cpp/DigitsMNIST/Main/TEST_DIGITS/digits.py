from PIL import Image

all = False
render = False
datas_format = True

if(not all):
    label_i = 0

img_width = 28
img_extension = ".png"

def grey_shades(r, g, b):
    return( (r + g + b) / 3)

numbers = ((0, "zero"), (1, "one"), (2, "two"), (3, "three"), (4, "four"),
            (5, "five"), (6, "six"), (7, "seven"), (8, "eight"), (9, "nine") )

if(all):

    for i in range(10):

        label = numbers[i][0]
        number = numbers[i][1]

        image = Image.open("C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/examples/frame/" + number + img_extension)  

        if(render):
            render_file = open("C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/examples/csv/" + number + "_render.csv", "w")
        if(datas_format):
            datas_file = open("C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/examples/csv/" + number + ".csv", "w")
            
        largeur, hauteur = image.size
        pixel_r = 0
        pixel_g = 0
        pixel_b = 0

        size_counter = 0

        if(datas_format):
                      
            for y in range(hauteur):
                for x in range(largeur):
                    pixel_rgba = image.getpixel((x, y))

                    r = pixel_rgba[0]
                    g = pixel_rgba[1]
                    b = pixel_rgba[2]
                    a = pixel_rgba[3]
                    
                    grey_level = grey_shades(r, g, b)

                    datas_file.write(f"{int(grey_level)},")

            datas_file.write("\n")
            datas_file.close()

        if(render):
            for y in range(hauteur):
                for x in range(largeur):
                    pixel_rgba = image.getpixel((x, y))

                    r = pixel_rgba[0]
                    g = pixel_rgba[1]
                    b = pixel_rgba[2]
                    a = pixel_rgba[3]
                    
                    grey_level = grey_shades(r, g, b)
                    
                    if(size_counter >= img_width):
                        size_counter = 0
                        render_file.write("\n")
                    render_file.write(f"{int(grey_level)}\t")
                    size_counter += 1
            render_file.close()

else:

    label = numbers[label_i][0]
    number = numbers[label_i][1]

    image = Image.open("C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/examples/frame/" + number + img_extension)  

    if(render):
        render_file = open("C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/examples/csv/" + number + "_render.csv", "w")
    if(datas_format):
        datas_file = open("C:/YA TROP DE CHOSE/AI/ALGORITHM/c_cpp/DigitsMNIST/Main/TEST_DIGITS/examples/csv/" + number + ".csv", "a")
        
    largeur, hauteur = image.size
    pixel_r = 0
    pixel_g = 0
    pixel_b = 0

    size_counter = 0

    if(datas_format):
                    
        for y in range(hauteur):
            for x in range(largeur):
                pixel_rgba = image.getpixel((x, y))

                r = pixel_rgba[0]
                g = pixel_rgba[1]
                b = pixel_rgba[2]
                a = pixel_rgba[3]
                
                grey_level = grey_shades(r, g, b)

                if(x == largeur-1 and y == hauteur -1):
                    datas_file.write(f"{int(grey_level)}")
                else:
                    datas_file.write(f"{int(grey_level)},")
        

        datas_file.write("\n")
        datas_file.close()

    if(render):
        for y in range(hauteur):
            for x in range(largeur):
                pixel_rgba = image.getpixel((x, y))

                r = pixel_rgba[0]
                g = pixel_rgba[1]
                b = pixel_rgba[2]
                a = pixel_rgba[3]
                
                grey_level = grey_shades(r, g, b)
                
                if(size_counter >= img_width):
                    size_counter = 0
                    render_file.write("\n")
                render_file.write(f"{int(grey_level)}\t")
                size_counter += 1

        datas_file.close()
        
    