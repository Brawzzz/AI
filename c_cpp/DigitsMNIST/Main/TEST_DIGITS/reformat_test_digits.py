import csv
from PIL import Image

img_width = 28

def grey_shades(r, g, b):
    return( (r + g + b) / 3)

file = open("./datas/reformat_test_digits.csv", "w")

with open('test.csv', 'r') as fichier_csv:
    lecteur_csv = csv.reader(fichier_csv)

    row_counter = 0

    for row in lecteur_csv:
        file.write(f"Test : {row_counter}\n")
        row_counter += 1
        for i in range (img_width):
            for j in range(img_width):
                file.write(f"{row[j + i * 28]}\t")
            file.write("\n")
    file.close()



